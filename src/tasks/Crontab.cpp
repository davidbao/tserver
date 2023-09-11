//
// Created by baowei on 2023/8/21.
//

#include "Crontab.h"
#include "diag/Trace.h"
#include "diag/Stopwatch.h"
#include "database/SnowFlake.h"
#include "TaskContext.h"
#include "Execution.h"
#include "Schedule.h"

using namespace Diag;

Crontab::Crontab() : name(String::Empty), enable(true), _schedule(nullptr), _execution(nullptr) {
}

Crontab::Crontab(const String &name, Schedule *schedule, Execution *execution, const StringArray &resultNames) :
        name(name), enable(true), _schedule(schedule), _execution(execution), resultNames(resultNames) {
}

Crontab::Crontab(const Crontab &other) : enable(true), _schedule(nullptr), _execution(nullptr) {
    Crontab::evaluates(other);
}

Crontab::~Crontab() {
    delete _schedule;
    delete _execution;
}

bool Crontab::equals(const Crontab &other) const {
    if (!(name == other.name && enable == other.enable)) {
        return false;
    }

    if (_schedule != nullptr && other._schedule != nullptr) {
        if (!_schedule->equals(*other._schedule)) {
            return false;
        }
    } else if (_schedule != nullptr && other._schedule == nullptr) {
        return false;
    } else if (_schedule == nullptr && other._schedule != nullptr) {
        return false;
    }

    if (_execution != nullptr && other._execution != nullptr) {
        if (!_execution->equals(*other._execution)) {
            return false;
        }
    } else if (_execution != nullptr && other._execution == nullptr) {
        return false;
    } else if (_execution == nullptr && other._execution != nullptr) {
        return false;
    }

    if (resultNames != other.resultNames) {
        return false;
    }

    return true;
}

void Crontab::evaluates(const Crontab &other) {
    name = other.name;
    enable = other.enable;

    delete _schedule;
    _schedule = nullptr;
    if (other._schedule != nullptr) {
        _schedule = other._schedule->clone();
    }

    delete _execution;
    _execution = nullptr;
    if (other._execution != nullptr) {
        _execution = other._execution->clone();
    }

    resultNames = other.resultNames;
}

Crontab &Crontab::operator=(const Crontab &other) {
    if (this != &other) {
        Crontab::evaluates(other);
    }
    return *this;
}

const Schedule *Crontab::schedule() const {
    return _schedule;
}

const Execution *Crontab::execution() const {
    return _execution;
}

bool Crontab::isTimeUp() {
    return isValid() && _schedule->isTimeUp(name);
}

bool Crontab::execute() {
    if (_execution != nullptr) {
        Trace::verb(String::format("Start to execute a task'%s'.", name.c_str()));
        Stopwatch sw;
        Execution::Result result = _execution->execute();
        sw.stop(false);
        if (result == Execution::Succeed) {
            Trace::verb(String::format("Execute a task'%s' successfully, elapsed: %s s.",
                                       name.c_str(), Double(sw.elapsed().totalSeconds()).toString().c_str()));
        } else if (result == Execution::FailedToStartProcess) {
            Trace::error(String::format("Failed to start a process, task'%s'!", name.c_str()));
        } else if (result == Execution::NotFound) {
            Trace::error(String::format("The app or script was not found, task'%s'!", name.c_str()));
        } else if (result == Execution::Timeout) {
            Trace::error(String::format("The task'%s' execution time out, elapsed: %s s!",
                                        name.c_str(), Double(sw.elapsed().totalSeconds()).toString().c_str()));
        } else if (result == Execution::FailedToExecuteSql) {
            Trace::error(String::format("Failed to execute a sql, task'%s'!", name.c_str()));
        }
        return result == Execution::Succeed;
    } else {
        Trace::error(String::format("Can not find execution in task'%s'!", name.c_str()));
        return false;
    }
}

DataRow Crontab::toDataRow(const DataTable &table) const {
    if (isValid()) {
        return DataRow({
                               DataCell(table.columns()["name"], name),
                               DataCell(table.columns()["schedule"], _schedule->toJsonNode().toString()),
                               DataCell(table.columns()["execution"], _execution->toJsonNode().toString()),
                               DataCell(table.columns()["result"], resultNames.toString()),
                       });
    } else {
        return DataRow({
                               DataCell(table.columns()["name"], name),
                       });
    }
}

JsonNode Crontab::toJsonNode() const {
    JsonNode node;
    node.add(JsonNode("name", name));
    if (_schedule != nullptr) {
        node.add(JsonNode("schedule", _schedule->toJsonNode()));
    }
    if (_execution != nullptr) {
        node.add(JsonNode("execution", _execution->toJsonNode()));
    }
    if (hasResult()) {
        JsonNode rNode("names", resultNames);
        node.add(JsonNode("result", rNode));
    }
    return node;
}

void Crontab::removeFile() {
    if (_execution != nullptr) {
        _execution->removeFile();
    }
}

bool Crontab::isValid() const {
    return _schedule != nullptr && _execution != nullptr;
}

bool Crontab::hasResult() const {
    return resultNames.count() > 0;
}

bool Crontab::findName(const String &n) const {
    return n.isNullOrEmpty() || this->name.find(n) >= 0;
}

void Crontab::updateYmlProperties(YmlNode::Properties &properties, int pos) const {
    properties.add(String::format(TasksPrefix "name", pos), name);
    {
        String type = _schedule->type();
        properties.add(String::format(SchedulePrefix "type", pos), type);
        if (type == "cycle") {
            auto cycle = dynamic_cast<const CycleSchedule *>(_schedule);
            assert(cycle);
            properties.add(String::format(SchedulePrefix "interval", pos), cycle->interval());
        } else if (type == "cron" || type == "crontab") {
            auto crontab = dynamic_cast<const CronSchedule *>(_schedule);
            assert(crontab);
            properties.add(String::format(SchedulePrefix "field", pos), crontab->field());
        }
    }
    {
        String type = _execution->type();
        properties.add(String::format(ExecutionPrefix "type", pos), type);
        properties.add(String::format(ExecutionPrefix "sync.enable", pos), _execution->sync());
        properties.add(String::format(ExecutionPrefix "sync.timeout", pos), _execution->timeout());
        if (type == "app") {
            auto app = dynamic_cast<const AppExecution *>(_execution);
            assert(app);
            properties.add(String::format(ExecutionPrefix "app", pos), app->app());
            properties.add(String::format(ExecutionPrefix "param", pos), app->param());
        } else if (type == "sql") {
            auto sql = dynamic_cast<const SqlExecution *>(_execution);
            assert(sql);
            if (!sql->isFile()) {
                properties.add(String::format(ExecutionPrefix "sql", pos), sql->sql());
            } else {
                properties.add(String::format(ExecutionPrefix "file", pos), sql->fileName());
            }
        } else if (type == "python") {
            auto python = dynamic_cast<const PythonExecution *>(_execution);
            assert(python);
            if (!python->isFile()) {
                properties.add(String::format(ExecutionPrefix "script", pos), python->script());
            } else {
                properties.add(String::format(ExecutionPrefix "file", pos), python->fileName());
                properties.add(String::format(ExecutionPrefix "param", pos), python->param());
            }
        }
    }
}

void Crontab::removeYmlProperties(YmlNode::Properties &properties, int pos) {
    properties.remove(String::format(TasksPrefix "name", pos));
    properties.remove(String::format(TasksPrefix "enable", pos));
    StringArray removeKeys;
    for (auto it = properties.begin(); it != properties.end(); ++it) {
        const String &k = it.key();
        if (k.find(String::format(TasksPrefix "schedule", pos)) >= 0) {
            removeKeys.add(k);
        } else if (k.find(String::format(TasksPrefix "execution", pos)) >= 0) {
            removeKeys.add(k);
        } else if (k.find(String::format(TasksPrefix "result", pos)) >= 0) {
            removeKeys.add(k);
        }
    }
    for (int i = 0; i < removeKeys.count(); ++i) {
        properties.remove(removeKeys[i]);
    }
}

bool Crontab::parse(const StringMap &request, Crontab &crontab) {
    if (!request.at("name", crontab.name)) {
        return false;
    }
    parseSchedule(request["schedule"], crontab._schedule);
    if (!parseExecution(request["execution"], crontab._execution)) {
        return false;
    }
    parseResult(request["result"], crontab.resultNames);

    return true;
}

bool Crontab::parse(const YmlNode::Properties &properties, int pos, Crontab &crontab) {
    String name;
    if (properties.at(String::format(TasksPrefix "name", pos), name)) {
        crontab.name = name;
        properties.at(String::format(TasksPrefix "enable", pos), crontab.enable);
        // schedule.
        {
            String type;
            properties.at(String::format(SchedulePrefix "type", pos), type);
            if (type == "cycle") {
                TimeSpan interval;
                properties.at(String::format(SchedulePrefix "interval", pos), interval);
                if (interval != TimeSpan::Zero) {
                    crontab._schedule = new CycleSchedule(interval);
                } else {
                    Trace::error(String::format("Task'%s' cycle interval is invalid.", name.c_str()));
                }
            } else if (type == "cron" || type == "crontab") {
                String field;
                properties.at(String::format(SchedulePrefix "field", pos), field);
                CronSchedule schedule;
                if (CronSchedule::parse(field, schedule)) {
                    crontab._schedule = new CronSchedule(schedule);
                } else {
                    Trace::error(String::format("Task'%s' cron field is invalid.", name.c_str()));
                }
            } else {
//                Trace::error(String::format("Can not find schedule type'%s'!", type.c_str()));
            }
        }

        // execution.
        {
            String type;
            properties.at(String::format(ExecutionPrefix "type", pos), type);
            bool sync = true;
            properties.at(String::format(ExecutionPrefix "sync.enable", pos), sync);
            TimeSpan timeout = TimeSpan::fromSeconds(60);
            properties.at(String::format(ExecutionPrefix "sync.timeout", pos), timeout);
            if (type == "app") {
                String app, param;
                properties.at(String::format(ExecutionPrefix "app", pos), app);
                properties.at(String::format(ExecutionPrefix "param", pos), param);
                crontab._execution = new AppExecution(sync, timeout, app, param);
            } else if (type == "sql") {
                String sql, fileName;
                properties.at(String::format(ExecutionPrefix "sql", pos), sql);
                properties.at(String::format(ExecutionPrefix "file", pos), fileName);
                crontab._execution = !sql.isNullOrEmpty() ?
                                     new SqlExecution(sync, timeout, sql, true) :
                                     new SqlExecution(sync, timeout, fileName, false);
            } else if (type == "python") {
                String script, fileName, param;
                properties.at(String::format(ExecutionPrefix "script", pos), script);
                properties.at(String::format(ExecutionPrefix "file", pos), fileName);
                properties.at(String::format(ExecutionPrefix "param", pos), param);
                crontab._execution = !script.isNullOrEmpty() ?
                                     new PythonExecution(sync, timeout, script) :
                                     new PythonExecution(sync, timeout, fileName, param);
            } else {
                Trace::error(String::format("Can not find the execution type'%s'!", type.c_str()));
            }
        }

        // result
        {
            String result;
            properties.at(String::format(TasksPrefix "result.name", pos), result);
            StringArray resultNames;
            StringArray::parse(result, resultNames, ',');
            for (size_t i = 0; i < resultNames.count(); ++i) {
                crontab.resultNames.add(resultNames[0].trim());
            }
        }

        return true;
    }
    return false;
}

bool Crontab::parse(const DataRow &row, Crontab &crontab) {
    if (!row["name"].value().getValue(crontab.name)) {
        return false;
    }
    parseSchedule(row["schedule"].valueStr(), crontab._schedule);
    if (!parseExecution(row["execution"].valueStr(), crontab._execution)) {
        return false;
    }
    parseResult(row["result"].valueStr(), crontab.resultNames);

    return true;
}

String Crontab::toSelectSqlStr(const String &prefix, const String &name) {
    String taskTableName = getTableName(prefix, TaskTableName);
    String sql = String::format("SELECT name,schedule,execution FROM %s WHERE name='%s' LIMIT 1",
                                taskTableName.c_str(),
                                name.c_str());
    return sql;
}

String Crontab::toSelectSqlStr(const String &prefix, int pos) {
    String taskTableName = getTableName(prefix, TaskTableName);
    String sql = String::format("SELECT name,schedule,execution FROM %s ORDER BY id asc LIMIT %d,1",
                                taskTableName.c_str(),
                                pos);
    return sql;
}

String Crontab::toSelectSqlStr(const String &prefix, const SqlSelectFilter &filter) {
    String taskTableName = getTableName(prefix, TaskTableName);
    String sql = String::format("SELECT name,schedule,execution FROM %s WHERE %s ORDER BY %s LIMIT %d,%d",
                                taskTableName.c_str(),
                                filter.toLikeStr("name").c_str(),
                                !filter.orderBy().isNullOrEmpty() ? filter.orderBy().c_str() : "name asc",
                                filter.offset(), filter.limit());
    return sql;
}

String Crontab::toCountSqlStr(const String &prefix, const SqlSelectFilter &filter) {
    String taskTableName = getTableName(prefix, TaskTableName);
    String sql = String::format("SELECT COUNT(1) FROM %s WHERE %s",
                                taskTableName.c_str(),
                                filter.toLikeStr("name").c_str());
    return sql;
}

String Crontab::toInsertSqlStr(const String &prefix) const {
    String sql;
    if (isValid()) {
        DateTime now = DateTime::now();
        String taskTableName = getTableName(prefix, TaskTableName);
        sql = String::format("INSERT INTO %s VALUES(%lld,'%s','%s','%s','%s','','%s','');",
                             taskTableName.c_str(),
                             SnowFlake::generateId(), name.c_str(),
                             _schedule->toString().c_str(), _execution->toString().c_str(),
                             now.toString().c_str(), now.toString().c_str());
    }
    return sql;
}

String Crontab::toReplaceSqlStr(const String &prefix) const {
    String sql;
    if (isValid()) {
        DateTime now = DateTime::now();
        String taskTableName = getTableName(prefix, TaskTableName);
        sql = String::format(
                "UPDATE %s SET schedule='%s',execution='%s',update_time='%s',update_user='' WHERE name='%s';",
                taskTableName.c_str(),
                _schedule->toString().c_str(), _execution->toString().c_str(),
                now.toString().c_str(), name.c_str());
    }
    return sql;
}

String Crontab::toDeleteSqlStr(const String &prefix, const String &labelName) {
    String sql;
    String taskTableName = getTableName(prefix, TaskTableName);
    sql = String::format("DELETE FROM %s WHERE name='%s';",
                         taskTableName.c_str(),
                         labelName.c_str());
    return sql;
}

bool Crontab::parseSchedule(const String &str, Schedule *&schedule) {
    JsonNode scheduleNode;
    if (JsonNode::parse(str, scheduleNode)) {
        String type = scheduleNode.getAttribute("type");
        if (type == "cycle") {
            TimeSpan interval;
            scheduleNode.getAttribute("interval", interval);
            if (interval != TimeSpan::Zero) {
                schedule = new CycleSchedule(interval);
                return true;
            } else {
                Trace::error("Cycle interval is invalid.");
            }
        } else if (type == "cron" || type == "crontab") {
            String field;
            scheduleNode.getAttribute("field", field);
            CronSchedule s;
            if (CronSchedule::parse(field, s)) {
                schedule = new CronSchedule(s);
            } else {
                Trace::error("Cron field is invalid.");
            }
        } else {
            Trace::error(String::format("Can not find schedule type'%s'!", type.c_str()));
        }
    }
    return false;
}

bool Crontab::parseExecution(const String &str, Execution *&execution) {
    JsonNode executionNode;
    if (JsonNode::parse(str, executionNode)) {
        String type = executionNode.getAttribute("type");
        JsonNode syncNode = executionNode["sync"];
        bool sync = true;
        syncNode.getAttribute("enable", sync);
        TimeSpan timeout = TimeSpan::fromSeconds(10);
        syncNode.getAttribute("timeout", timeout);
        if (type == "app") {
            String app, param;
            executionNode.getAttribute("app", app);
            executionNode.getAttribute("param", param);
            execution = new AppExecution(sync, timeout, app, param);
            return true;
        } else if (type == "sql") {
            String sql, fileName;
            executionNode.getAttribute("sql", sql);
            executionNode.getAttribute("file", fileName);
            execution = !sql.isNullOrEmpty() ?
                        new SqlExecution(sync, timeout, sql, true) :
                        new SqlExecution(sync, timeout, fileName, false);
            return true;
        } else if (type == "python") {
            String script, fileName, param;
            executionNode.getAttribute("script", script);
            executionNode.getAttribute("file", fileName);
            executionNode.getAttribute("param", param);
            execution = !script.isNullOrEmpty() ?
                        new PythonExecution(sync, timeout, script) :
                        new PythonExecution(sync, timeout, fileName, param);
            return true;
        } else {
            // Can not find execution type.
        }
    }
    return false;
}

bool Crontab::parseResult(const String &str, StringArray &results) {
    StringArray resultNames;
    StringArray::parse(str, resultNames, ',');
    for (size_t i = 0; i < resultNames.count(); ++i) {
        results.add(resultNames[0].trim());
    }
    return results.count() > 0;
}

String Crontab::getTableName(const String &prefix, const String &tableName) {
    return prefix.isNullOrEmpty() ? tableName : String::format("%s.%s", prefix.c_str(), tableName.c_str());
}
