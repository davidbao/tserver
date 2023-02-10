/*******************************************************************************
  Copyright (c) 2023 com.

  All rights reserved. This program and the accompanying materials
  are made available under the terms of the EaseSuite Public License v1.0
  and EaseSuite Distribution License v1.0 which accompany this distribution.

  Only for kingbase v7.

  Contributors:
     Baowei - initial version
*******************************************************************************/

--CREATE TABLESPACE TSERVER(NAME = TSERVER FILENAME = 'TSERVER' SIZE = 100 MAXSIZE = 16000 FILEGROWTH = 10);

-- 1. 首页表
-- --------------------------------------------
-- Table structure for home_total
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_total;
CREATE TABLE TSERVER.home_total
(
    id             bigint NOT NULL,
    p_s_capacity   real   NOT NULL,
    p_power        int    NOT NULL,
    p_p_capacity   int    NOT NULL,
    p_a_capacity   int    NOT NULL,
    p_max_load     real   NOT NULL,
    d_s_capacity   real   NOT NULL,
    d_power        int    NOT NULL,
    d_p_capacity   int    NOT NULL,
    d_a_capacity   int    NOT NULL,
    d_max_load     real   NOT NULL,
    day_epencent   real   NOT NULL,
    total_income   real   NOT NULL,
    day_vroc       real   NOT NULL,
    total_p_income real   NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_total IS '总体数据表';
COMMENT ON COLUMN TSERVER.home_total.p_s_capacity IS '光储联动台区储能容量，单位：kW/h';
COMMENT ON COLUMN TSERVER.home_total.p_power IS '光储联动台区额定功率，单位：kW';
COMMENT ON COLUMN TSERVER.home_total.p_p_capacity IS '光储联动台区光伏装机，单位：kVA';
COMMENT ON COLUMN TSERVER.home_total.p_a_capacity IS '光储联动台区台区容量，单位：kVA';
COMMENT ON COLUMN TSERVER.home_total.p_max_load IS '光储联动台区最大负荷，单位：kW';
COMMENT ON COLUMN TSERVER.home_total.d_s_capacity IS '柔直互联台区储能容量，单位：kW/h';
COMMENT ON COLUMN TSERVER.home_total.d_power IS '柔直互联台区额定功率，单位：kW';
COMMENT ON COLUMN TSERVER.home_total.d_p_capacity IS '柔直互联台区光伏装机，单位：kVA';
COMMENT ON COLUMN TSERVER.home_total.d_a_capacity IS '柔直互联台区台区容量，单位：kVA';
COMMENT ON COLUMN TSERVER.home_total.d_max_load IS '柔直互联台区最大负荷，单位：kW';
COMMENT ON COLUMN TSERVER.home_total.day_epencent IS '当日就地消纳，单位：%';
COMMENT ON COLUMN TSERVER.home_total.total_income IS '累计峰谷差收益，单位：￥元';
COMMENT ON COLUMN TSERVER.home_total.day_vroc IS '当日最大电压偏差，单位：%';
COMMENT ON COLUMN TSERVER.home_total.total_p_income IS '累计光伏自用收益，单位：￥元';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_elimination
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_elimination;
CREATE TABLE TSERVER.home_elimination
(
    id              bigint NOT NULL,
    p_year_pencent  real   NOT NULL,
    p_month_pencent real   NOT NULL,
    p_day_pencent   real   NOT NULL,
    d_year_pencent  real   NOT NULL,
    d_month_pencent real   NOT NULL,
    d_day_pencent   real   NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_elimination IS '就地消纳表';
COMMENT ON COLUMN TSERVER.home_elimination.p_year_pencent IS '光储联动台区年消纳率，单位：%';
COMMENT ON COLUMN TSERVER.home_elimination.p_month_pencent IS '光储联动台区月消纳率，单位：%';
COMMENT ON COLUMN TSERVER.home_elimination.p_day_pencent IS '光储联动台区日消纳率，单位：%';
COMMENT ON COLUMN TSERVER.home_elimination.d_year_pencent IS '柔直互联台区年消纳率，单位：%';
COMMENT ON COLUMN TSERVER.home_elimination.d_month_pencent IS '柔直互联台区月消纳率，单位：%';
COMMENT ON COLUMN TSERVER.home_elimination.d_day_pencent IS '柔直互联台区日消纳率，单位：%';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_vroc
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_vroc;
CREATE TABLE TSERVER.home_vroc
(
    id           bigint NOT NULL,
    p_year_vroc  real   NOT NULL,
    p_month_vroc real   NOT NULL,
    p_day_vroc   real   NOT NULL,
    d_year_vroc  real   NOT NULL,
    d_month_vroc real   NOT NULL,
    d_day_vroc   real   NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_vroc IS '就地消纳表';
COMMENT ON COLUMN TSERVER.home_vroc.p_year_vroc IS '光储联动台区年最大偏差，单位：%';
COMMENT ON COLUMN TSERVER.home_vroc.p_month_vroc IS '光储联动台区月最大偏差，单位：%';
COMMENT ON COLUMN TSERVER.home_vroc.p_day_vroc IS '光储联动台区日最大偏差，单位：%';
COMMENT ON COLUMN TSERVER.home_vroc.d_year_vroc IS '柔直互联台区年最大偏差，单位：%';
COMMENT ON COLUMN TSERVER.home_vroc.d_month_vroc IS '柔直互联台区月最大偏差，单位：%';
COMMENT ON COLUMN TSERVER.home_vroc.d_day_vroc IS '柔直互联台区日最大偏差，单位：%';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_p_dload_power
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_p_dload_power;
CREATE TABLE TSERVER.home_p_dload_power
(
    id    bigint  NOT NULL,
    time  time(0) NOT NULL,
    value real    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_p_dload_power IS '光储联动台区负荷日功率表';
COMMENT ON COLUMN TSERVER.home_p_dload_power.time IS '时间，时、分有效';
COMMENT ON COLUMN TSERVER.home_p_dload_power.value IS '台区负荷功率，单位：kW';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_p_pgeneration_power
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_p_pgeneration_power;
CREATE TABLE TSERVER.home_p_pgeneration_power
(
    id    bigint  NOT NULL,
    time  time(0) NOT NULL,
    value real    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_p_pgeneration_power IS '光储联动台区光伏日发电功率表';
COMMENT ON COLUMN TSERVER.home_p_pgeneration_power.time IS '时间，时、分有效';
COMMENT ON COLUMN TSERVER.home_p_pgeneration_power.value IS '光伏发电功率，单位：kW';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_p_senergy_power
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_p_senergy_power;
CREATE TABLE TSERVER.home_p_senergy_power
(
    id    bigint  NOT NULL,
    time  time(0) NOT NULL,
    value real    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_p_senergy_power IS '光储联动台区储能日功率表';
COMMENT ON COLUMN TSERVER.home_p_senergy_power.time IS '时间，时、分有效';
COMMENT ON COLUMN TSERVER.home_p_senergy_power.value IS '储能功率，单位：kW';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_p_cgrid_power
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_p_cgrid_power;
CREATE TABLE TSERVER.home_p_cgrid_power
(
    id    bigint  NOT NULL,
    time  time(0) NOT NULL,
    value real    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_p_cgrid_power IS '光储联动台区上网功率表';
COMMENT ON COLUMN TSERVER.home_p_cgrid_power.time IS '时间，时、分有效';
COMMENT ON COLUMN TSERVER.home_p_cgrid_power.value IS '上网功率，单位：kW';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_p_battery_info
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_p_battery_info;
CREATE TABLE TSERVER.home_p_battery_info
(
    id   bigint NOT NULL,
    soc1 real,
    soh1 real,
    soc2 real,
    soh2 real,
    soc3 real,
    soh3 real,
    soc4 real,
    soh4 real,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_p_battery_info IS '光储联动台区电池信息表';
COMMENT ON COLUMN TSERVER.home_p_battery_info.soc1 IS '电池簇1剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_p_battery_info.soh1 IS '电池簇1当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_p_battery_info.soc2 IS '电池簇2剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_p_battery_info.soh2 IS '电池簇2当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_p_battery_info.soc3 IS '电池簇3剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_p_battery_info.soh3 IS '电池簇3当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_p_battery_info.soc4 IS '电池簇4剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_p_battery_info.soh4 IS '电池簇4当前容量百分比，单位：%';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_d_dload_power
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_d_dload_power;
CREATE TABLE TSERVER.home_d_dload_power
(
    id    bigint  NOT NULL,
    time  time(0) NOT NULL,
    value real    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_d_dload_power IS '柔直互联台区负荷日功率表';
COMMENT ON COLUMN TSERVER.home_d_dload_power.time IS '时间，时、分有效';
COMMENT ON COLUMN TSERVER.home_d_dload_power.value IS '台区负荷功率，单位：kW';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_d_pgeneration_power
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_d_pgeneration_power;
CREATE TABLE TSERVER.home_d_pgeneration_power
(
    id    bigint  NOT NULL,
    time  time(0) NOT NULL,
    value real    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_d_pgeneration_power IS '柔直互联台区光伏日发电功率表';
COMMENT ON COLUMN TSERVER.home_d_pgeneration_power.time IS '时间，时、分有效';
COMMENT ON COLUMN TSERVER.home_d_pgeneration_power.value IS '光伏发电功率，单位：kW';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_d_senergy_power
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_d_senergy_power;
CREATE TABLE TSERVER.home_d_senergy_power
(
    id    bigint  NOT NULL,
    time  time(0) NOT NULL,
    value real    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_d_senergy_power IS '柔直互联台区储能日功率表';
COMMENT ON COLUMN TSERVER.home_d_senergy_power.time IS '时间，时、分有效';
COMMENT ON COLUMN TSERVER.home_d_senergy_power.value IS '储能功率，单位：kW';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_d_cgrid_power
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_d_cgrid_power;
CREATE TABLE TSERVER.home_d_cgrid_power
(
    id    bigint  NOT NULL,
    time  time(0) NOT NULL,
    value real    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_d_cgrid_power IS '柔直互联台区上网功率表';
COMMENT ON COLUMN TSERVER.home_d_cgrid_power.time IS '时间，时、分有效';
COMMENT ON COLUMN TSERVER.home_d_cgrid_power.value IS '上网功率，单位：kW';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_d_battery_info
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_d_battery_info;
CREATE TABLE TSERVER.home_d_battery_info
(
    id   bigint NOT NULL,
    soc1 real,
    soh1 real,
    soc2 real,
    soh2 real,
    soc3 real,
    soh3 real,
    soc4 real,
    soh4 real,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_d_battery_info IS '柔直互联台区电池信息表';
COMMENT ON COLUMN TSERVER.home_d_battery_info.soc1 IS '电池簇1剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_d_battery_info.soh1 IS '电池簇1当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_d_battery_info.soc2 IS '电池簇2剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_d_battery_info.soh2 IS '电池簇2当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_d_battery_info.soc3 IS '电池簇3剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_d_battery_info.soh3 IS '电池簇3当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_d_battery_info.soc4 IS '电池簇4剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.home_d_battery_info.soh4 IS '电池簇4当前容量百分比，单位：%';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_real_eflux
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.home_real_eflux;
CREATE TABLE TSERVER.home_real_eflux
(
    id     bigint      NOT NULL,
    name   varchar(64) NOT NULL,
    flag   int         NOT NULL,
    value1 real        NOT NULL,
    value2 real        NOT NULL,
    value3 real        NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.home_real_eflux IS '实时能流表';
COMMENT ON COLUMN TSERVER.home_real_eflux.name IS '能流节点名称';
COMMENT ON COLUMN TSERVER.home_real_eflux.flag IS '与父节点连接线的能流方向，1: in, 2:out, 0: none';
COMMENT ON COLUMN TSERVER.home_real_eflux.value1 IS '节点实时数据1';
COMMENT ON COLUMN TSERVER.home_real_eflux.value2 IS '节点实时数据2';
COMMENT ON COLUMN TSERVER.home_real_eflux.value3 IS '节点实时数据3';
COMMIT TRANSACTION;

-- 2. 光储联动台区-储能运行状态表
-- --------------------------------------------
-- Table structure for p_seoverview_acdata
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_seoverview_acdata;
CREATE TABLE TSERVER.p_seoverview_acdata
(
    id              bigint NOT NULL,
    P               real   NOT NULL,
    Q               real   NOT NULL,
    Ia              real   NOT NULL,
    Ib              real   NOT NULL,
    Ic              real   NOT NULL,
    Ua              real   NOT NULL,
    Ub              real   NOT NULL,
    Uc              real   NOT NULL,
    PCS_comm_error  int    NOT NULL,
    PCS_ctrl_fail   int    NOT NULL,
    EMS_comm_error  int    NOT NULL,
    rack_vdiff_high int    NOT NULL,
    rack_cdiff_high int    NOT NULL,
    em_shutdown     int    NOT NULL,
    ac_comm_error   int    NOT NULL,
    overv_3rd_level int    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_seoverview_acdata IS '交流侧量测和保护信号表';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.Q IS '单位：kVar';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.Ia IS '单位：A';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.Ib IS '单位：A';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.Ic IS '单位：A';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.Ua IS '单位：V';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.Ub IS '单位：V';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.Uc IS '单位：V';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.PCS_comm_error IS 'PCS通讯故障报警，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.PCS_ctrl_fail IS 'PCS控制失效报警，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.EMS_comm_error IS 'EMS通讯故障报警，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.rack_vdiff_high IS 'Rack压差过大报警，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.rack_cdiff_high IS 'Rack电流差过大报警，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.em_shutdown IS '紧急停机，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.ac_comm_error IS '空调通信故障，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_acdata.overv_3rd_level IS '单体过压三级报警，1有效';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_seoverview_accurve
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_seoverview_accurve;
CREATE TABLE TSERVER.p_seoverview_accurve
(
    id   bigint       NOT NULL,
    time timestamp(0) NOT NULL,
    P    real         NOT NULL,
    Q    real         NOT NULL,
    Ia   real         NOT NULL,
    Ib   real         NOT NULL,
    Ic   real         NOT NULL,
    Ua   real         NOT NULL,
    Ub   real         NOT NULL,
    Uc   real         NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_seoverview_accurve IS '交流侧曲线数据表';
COMMENT ON COLUMN TSERVER.p_seoverview_accurve.time IS '预测和实测数据的时间戳，精度到分';
COMMENT ON COLUMN TSERVER.p_seoverview_accurve.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.p_seoverview_accurve.Q IS '单位：kVar';
COMMENT ON COLUMN TSERVER.p_seoverview_accurve.Ia IS '单位：A';
COMMENT ON COLUMN TSERVER.p_seoverview_accurve.Ib IS '单位：A';
COMMENT ON COLUMN TSERVER.p_seoverview_accurve.Ic IS '单位：A';
COMMENT ON COLUMN TSERVER.p_seoverview_accurve.Ua IS '单位：V';
COMMENT ON COLUMN TSERVER.p_seoverview_accurve.Ub IS '单位：V';
COMMENT ON COLUMN TSERVER.p_seoverview_accurve.Uc IS '单位：V';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_seoverview_dcdata
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_seoverview_dcdata;
CREATE TABLE TSERVER.p_seoverview_dcdata
(
    id              bigint NOT NULL,
    P               real   NOT NULL,
    I               real   NOT NULL,
    U               real   NOT NULL,
    bc1_I           real   NOT NULL,
    bc1_U           real   NOT NULL,
    bc1_soc         real   NOT NULL,
    bc1_soh         real   NOT NULL,
    bc2_I           real   NOT NULL,
    bc2_U           real   NOT NULL,
    bc2_soc         real   NOT NULL,
    bc2_soh         real   NOT NULL,
    bc3_I           real   NOT NULL,
    bc3_U           real   NOT NULL,
    bc3_soc         real   NOT NULL,
    bc3_soh         real   NOT NULL,
    bc4_I           real   NOT NULL,
    bc4_U           real   NOT NULL,
    bc4_soc         real   NOT NULL,
    bc4_soh         real   NOT NULL,
    PCS_comm_error  int    NOT NULL,
    PCS_ctrl_fail   int    NOT NULL,
    EMS_comm_error  int    NOT NULL,
    rack_vdiff_high int    NOT NULL,
    rack_cdiff_high int    NOT NULL,
    em_shutdown     int    NOT NULL,
    ac_comm_error   int    NOT NULL,
    overv_3rd_level int    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_seoverview_dcdata IS '直流侧量测和保护信号表';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.I IS '单位：A';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.U IS '单位：V';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc1_I IS '电池簇1电流，单位：A';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc1_U IS '电池簇1电压，单位：V';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc1_soc IS '电池簇1剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc1_soh IS '电池簇1当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc2_I IS '电池簇2电流，单位：A';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc2_U IS '电池簇2电压，单位：V';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc2_soc IS '电池簇2剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc2_soh IS '电池簇2当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc3_I IS '电池簇3电流，单位：A';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc3_U IS '电池簇3电压，单位：V';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc3_soc IS '电池簇3剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc3_soh IS '电池簇1当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc4_I IS '电池簇4电流，单位：A';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc4_U IS '电池簇4电压，单位：V';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc4_soc IS '电池簇4剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.bc4_soh IS '电池簇4当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.PCS_comm_error IS 'PCS通讯故障报警，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.PCS_ctrl_fail IS 'PCS控制失效报警，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.EMS_comm_error IS 'EMS通讯故障报警，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.rack_vdiff_high IS 'Rack压差过大报警，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.rack_cdiff_high IS 'Rack电流差过大报警，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.em_shutdown IS '紧急停机，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.ac_comm_error IS '空调通信故障，1有效';
COMMENT ON COLUMN TSERVER.p_seoverview_dcdata.overv_3rd_level IS '单体过压三级报警，1有效';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_seoverview_dccurve
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_seoverview_dccurve;
CREATE TABLE TSERVER.p_seoverview_dccurve
(
    id   bigint       NOT NULL,
    time timestamp(0) NOT NULL,
    P    real         NOT NULL,
    I    real         NOT NULL,
    U    real         NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_seoverview_dccurve IS '直流侧曲线数据表';
COMMENT ON COLUMN TSERVER.p_seoverview_dccurve.time IS '预测和实测数据的时间戳，精度到分';
COMMENT ON COLUMN TSERVER.p_seoverview_dccurve.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.p_seoverview_dccurve.I IS '单位：A';
COMMENT ON COLUMN TSERVER.p_seoverview_dccurve.U IS '单位：V';
COMMIT TRANSACTION;

-- 3. 光储联动台区-光伏运行状态表
-- --------------------------------------------
-- Table structure for p_poverview_station
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_poverview_station;
CREATE TABLE TSERVER.p_poverview_station
(
    id   bigint      NOT NULL,
    pid  bigint      NOT NULL,
    name varchar(64) NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_poverview_station IS '光伏信息表';
COMMENT ON COLUMN TSERVER.p_poverview_station.pid IS '光伏电站id';
COMMENT ON COLUMN TSERVER.p_poverview_station.name IS '光伏电站名称';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_poverview_overview
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_poverview_overview;
CREATE TABLE TSERVER.p_poverview_overview
(
    id        bigint NOT NULL,
    pid       bigint NOT NULL,
    capacity  real   NOT NULL,
    power     real   NOT NULL,
    max_power real   NOT NULL,
    min_power real   NOT NULL,
    daily_EP  real   NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_poverview_overview IS '光伏总览表';
COMMENT ON COLUMN TSERVER.p_poverview_overview.pid IS '光伏电站id';
COMMENT ON COLUMN TSERVER.p_poverview_overview.capacity IS '光伏电站容量，单位：kW';
COMMENT ON COLUMN TSERVER.p_poverview_overview.power IS '光伏电站实时功率，单位：kW';
COMMENT ON COLUMN TSERVER.p_poverview_overview.max_power IS '光伏电站当日最大功率，单位：kW';
COMMENT ON COLUMN TSERVER.p_poverview_overview.min_power IS '光伏电站当日最小功率，单位：kW';
COMMENT ON COLUMN TSERVER.p_poverview_overview.daily_EP IS '光伏电站当日发电量，单位：kW/h';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_poverview_mesa
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_poverview_mesa;
CREATE TABLE TSERVER.p_poverview_mesa
(
    id  bigint NOT NULL,
    pid bigint NOT NULL,
    P   real   NOT NULL,
    Q   real   NOT NULL,
    Ia  real   NOT NULL,
    Ib  real   NOT NULL,
    Ic  real   NOT NULL,
    Ua  real   NOT NULL,
    Ub  real   NOT NULL,
    Uc  real   NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_poverview_mesa IS '光伏量测数据表';
COMMENT ON COLUMN TSERVER.p_poverview_mesa.pid IS '光伏电站id';
COMMENT ON COLUMN TSERVER.p_poverview_mesa.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.p_poverview_mesa.Q IS '单位：kVar';
COMMENT ON COLUMN TSERVER.p_poverview_mesa.Ia IS '单位：A';
COMMENT ON COLUMN TSERVER.p_poverview_mesa.Ib IS '单位：A';
COMMENT ON COLUMN TSERVER.p_poverview_mesa.Ic IS '单位：A';
COMMENT ON COLUMN TSERVER.p_poverview_mesa.Ua IS '单位：V';
COMMENT ON COLUMN TSERVER.p_poverview_mesa.Ub IS '单位：V';
COMMENT ON COLUMN TSERVER.p_poverview_mesa.Uc IS '单位：V';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_poverview_protect
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_poverview_protect;
CREATE TABLE TSERVER.p_poverview_protect
(
    id                 bigint NOT NULL,
    pid                bigint NOT NULL,
    lcl                int    NOT NULL,
    prot_act           int    NOT NULL,
    ocswitch_act       int    NOT NULL,
    interval_exception int    NOT NULL,
    interval_fail      int    NOT NULL,
    interval_singal    int    NOT NULL,
    mp_comm_error      int    NOT NULL,
    bh_position        int    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_poverview_protect IS '光伏保护信号表';
COMMENT ON COLUMN TSERVER.p_poverview_protect.pid IS '光伏电站id';
COMMENT ON COLUMN TSERVER.p_poverview_protect.lcl IS 'PCS通讯故障报警，1有效';
COMMENT ON COLUMN TSERVER.p_poverview_protect.prot_act IS 'PCS控制失效报警，1有效';
COMMENT ON COLUMN TSERVER.p_poverview_protect.ocswitch_act IS 'EMS通讯故障报警，1有效';
COMMENT ON COLUMN TSERVER.p_poverview_protect.interval_exception IS 'Rack压差过大报警，1有效';
COMMENT ON COLUMN TSERVER.p_poverview_protect.interval_fail IS 'Rack电流差过大报警，1有效';
COMMENT ON COLUMN TSERVER.p_poverview_protect.interval_singal IS '紧急停机，1有效';
COMMENT ON COLUMN TSERVER.p_poverview_protect.mp_comm_error IS '空调通信故障，1有效';
COMMENT ON COLUMN TSERVER.p_poverview_protect.bh_position IS '单体过压三级报警，1有效';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_poverview_curve
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_poverview_curve;
CREATE TABLE TSERVER.p_poverview_curve
(
    id   bigint       NOT NULL,
    pid  bigint       NOT NULL,
    time timestamp(0) NOT NULL,
    P_c  real         NOT NULL,
    Q_c  real         NOT NULL,
    Ia_c real         NOT NULL,
    Ib_c real         NOT NULL,
    Ic_c real         NOT NULL,
    Ua_c real         NOT NULL,
    Ub_c real         NOT NULL,
    Uc_c real         NOT NULL,
    P    real         NOT NULL,
    Q    real         NOT NULL,
    Ia   real         NOT NULL,
    Ib   real         NOT NULL,
    Ic   real         NOT NULL,
    Ua   real         NOT NULL,
    Ub   real         NOT NULL,
    Uc   real         NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_poverview_curve IS '光伏曲线数据表';
COMMENT ON COLUMN TSERVER.p_poverview_curve.pid IS '光伏电站id';
COMMENT ON COLUMN TSERVER.p_poverview_curve.time IS '预测和实测数据的时间戳，精度到分';
COMMENT ON COLUMN TSERVER.p_poverview_curve.P_c IS '预测值，单位：kW';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Q_c IS '预测值，单位：kVar';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Ia_c IS '预测值，单位：A';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Ib_c IS '预测值，单位：A';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Ic_c IS '预测值，单位：A';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Ua_c IS '预测值，单位：V';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Ub_c IS '预测值，单位：V';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Uc_c IS '预测值，单位：V';
COMMENT ON COLUMN TSERVER.p_poverview_curve.P IS '实测值，单位：kW';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Q IS '实测值，单位：kVar';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Ia IS '实测值，单位：A';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Ib IS '实测值，单位：A';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Ic IS '实测值，单位：A';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Ua IS '实测值，单位：V';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Ub IS '实测值，单位：V';
COMMENT ON COLUMN TSERVER.p_poverview_curve.Uc IS '实测值，单位：V';
COMMIT TRANSACTION;

-- 4. 光储联动台区-台区运行状态表
-- --------------------------------------------
-- Table structure for p_aoverview_overview
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_aoverview_overview;
CREATE TABLE TSERVER.p_aoverview_overview
(
    id            bigint NOT NULL,
    p_real_power  real   NOT NULL,
    load_power    real   NOT NULL,
    combine_power real   NOT NULL,
    daily_EP      real   NOT NULL,
    load_EP       real   NOT NULL,
    total_EP      real   NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_aoverview_overview IS '台区总览表';
COMMENT ON COLUMN TSERVER.p_aoverview_overview.p_real_power IS '光伏实时发电功率，单位：kW';
COMMENT ON COLUMN TSERVER.p_aoverview_overview.load_power IS '负荷实时功率，单位：kW';
COMMENT ON COLUMN TSERVER.p_aoverview_overview.combine_power IS '上网功率，单位：kW';
COMMENT ON COLUMN TSERVER.p_aoverview_overview.daily_EP IS '光伏当日发电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.p_aoverview_overview.load_EP IS '负荷当日用电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.p_aoverview_overview.total_EP IS '累计上网电量，单位：kW/h';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_aoverview_mesa
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_aoverview_mesa;
CREATE TABLE TSERVER.p_aoverview_mesa
(
    id bigint NOT NULL,
    P  real   NOT NULL,
    Q  real   NOT NULL,
    Ia real   NOT NULL,
    Ib real   NOT NULL,
    Ic real   NOT NULL,
    Ua real   NOT NULL,
    Ub real   NOT NULL,
    Uc real   NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_aoverview_mesa IS '台区量测数据表';
COMMENT ON COLUMN TSERVER.p_aoverview_mesa.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.p_aoverview_mesa.Q IS '单位：kVar';
COMMENT ON COLUMN TSERVER.p_aoverview_mesa.Ia IS '单位：A';
COMMENT ON COLUMN TSERVER.p_aoverview_mesa.Ib IS '单位：A';
COMMENT ON COLUMN TSERVER.p_aoverview_mesa.Ic IS '单位：A';
COMMENT ON COLUMN TSERVER.p_aoverview_mesa.Ua IS '单位：V';
COMMENT ON COLUMN TSERVER.p_aoverview_mesa.Ub IS '单位：V';
COMMENT ON COLUMN TSERVER.p_aoverview_mesa.Uc IS '单位：V';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_aoverview_protect
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_aoverview_protect;
CREATE TABLE TSERVER.p_aoverview_protect
(
    id                 bigint NOT NULL,
    lcl                int    NOT NULL,
    prot_act           int    NOT NULL,
    ocswitch_act       int    NOT NULL,
    interval_exception int    NOT NULL,
    interval_fail      int    NOT NULL,
    interval_singal    int    NOT NULL,
    mp_comm_error      int    NOT NULL,
    bh_position        int    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_aoverview_protect IS '台区保护信号表';
COMMENT ON COLUMN TSERVER.p_aoverview_protect.lcl IS '就地，1有效';
COMMENT ON COLUMN TSERVER.p_aoverview_protect.prot_act IS '保护动作，1有效';
COMMENT ON COLUMN TSERVER.p_aoverview_protect.ocswitch_act IS '分合闸动作，1有效';
COMMENT ON COLUMN TSERVER.p_aoverview_protect.interval_exception IS '间隔设备异常，1有效';
COMMENT ON COLUMN TSERVER.p_aoverview_protect.interval_fail IS '间隔设备故障，1有效';
COMMENT ON COLUMN TSERVER.p_aoverview_protect.interval_singal IS '间隔事故信号，1有效';
COMMENT ON COLUMN TSERVER.p_aoverview_protect.mp_comm_error IS '测保装置通讯中断，1有效';
COMMENT ON COLUMN TSERVER.p_aoverview_protect.bh_position IS '手车工作位置，1有效';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_aoverview_curve
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_aoverview_curve;
CREATE TABLE TSERVER.p_aoverview_curve
(
    id   bigint       NOT NULL,
    time timestamp(0) NOT NULL,
    P_c  real         NOT NULL,
    Q_c  real         NOT NULL,
    Ia_c real         NOT NULL,
    Ib_c real         NOT NULL,
    Ic_c real         NOT NULL,
    Ua_c real         NOT NULL,
    Ub_c real         NOT NULL,
    Uc_c real         NOT NULL,
    P    real         NOT NULL,
    Q    real         NOT NULL,
    Ia   real         NOT NULL,
    Ib   real         NOT NULL,
    Ic   real         NOT NULL,
    Ua   real         NOT NULL,
    Ub   real         NOT NULL,
    Uc   real         NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_aoverview_curve IS '台区保护信号表';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.time IS '预测和实测数据的时间戳，精度到分';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.P_c IS '预测值，单位：kW';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Q_c IS '预测值，单位：kVar';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Ia_c IS '预测值，单位：A';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Ib_c IS '预测值，单位：A';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Ic_c IS '预测值，单位：A';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Ua_c IS '预测值，单位：V';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Ub_c IS '预测值，单位：V';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Uc_c IS '预测值，单位：V';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.P IS '实测值，单位：kW';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Q IS '实测值，单位：kVar';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Ia IS '实测值，单位：A';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Ib IS '实测值，单位：A';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Ic IS '实测值，单位：A';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Ua IS '实测值，单位：V';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Ub IS '实测值，单位：V';
COMMENT ON COLUMN TSERVER.p_aoverview_curve.Uc IS '实测值，单位：V';
COMMIT TRANSACTION;

-- 5. 光储联动台区-经济效益分析表
-- --------------------------------------------
-- Table structure for p_ebanalysis_overview
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_ebanalysis_overview;
CREATE TABLE TSERVER.p_ebanalysis_overview
(
    id                 bigint           NOT NULL,
    total_charge_EP    double precision NOT NULL,
    total_discharge_EP double precision NOT NULL,
    total_pg_EP        double precision NOT NULL,
    total_pc_EP        double precision NOT NULL,
    total_eb           money            NOT NULL,
    total_pvd_benefit  money            NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_ebanalysis_overview IS '经济效益分析总览表';
COMMENT ON COLUMN TSERVER.p_ebanalysis_overview.total_charge_EP is '储能累计充电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.p_ebanalysis_overview.total_discharge_EP is '储能累计放电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.p_ebanalysis_overview.total_pg_EP is '光伏累计发电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.p_ebanalysis_overview.total_pc_EP is '光伏累计上网电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.p_ebanalysis_overview.total_eb is '累计经济效益，单位：元';
COMMENT ON COLUMN TSERVER.p_ebanalysis_overview.total_pvd_benefit is '累计峰谷差收益，单位：元';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_ebanalysis_curve
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.p_ebanalysis_curve;
CREATE TABLE TSERVER.p_ebanalysis_curve
(
    id           bigint NOT NULL,
    time         date   NOT NULL,
    charge_EP    real   NOT NULL,
    discharge_EP real   NOT NULL,
    pg_EP        real   NOT NULL,
    pc_EP        real   NOT NULL,
    eb           money  NOT NULL,
    pvd_benefit  money  NOT NULL,
    pp_benefit   money  NOT NULL,

    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.p_ebanalysis_curve IS '经济效益分析总览表';
COMMENT ON COLUMN TSERVER.p_ebanalysis_curve.time IS '数据时间戳，精度到日';
COMMENT ON COLUMN TSERVER.p_ebanalysis_curve.charge_EP IS '储能月度充电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.p_ebanalysis_curve.discharge_EP IS '储能月度放电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.p_ebanalysis_curve.pg_EP IS '光伏月度发电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.p_ebanalysis_curve.pc_EP IS '光伏月度上网电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.p_ebanalysis_curve.eb IS '月度经济效益，单位：元';
COMMENT ON COLUMN TSERVER.p_ebanalysis_curve.pvd_benefit IS '月度峰谷差收益，单位：元';
COMMENT ON COLUMN TSERVER.p_ebanalysis_curve.pp_benefit IS '月度光伏自用收益，单位：元';
COMMIT TRANSACTION;

-- 6. 光储联动台区-协调控制表
-- todo:

-- 7. 柔直互联台区-储能运行状态
-- --------------------------------------------
-- Table structure for d_seoverview_station
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_seoverview_station;
CREATE TABLE TSERVER.d_seoverview_station
(
    id   bigint      NOT NULL,
    sid  bigint      NOT NULL,
    name varchar(64) NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_seoverview_station IS '储能信息表';
COMMENT ON COLUMN TSERVER.d_seoverview_station.sid is '储能设备id';
COMMENT ON COLUMN TSERVER.d_seoverview_station.name is '储能设备名称';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_seoverview_dc1data
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_seoverview_dc1data;
CREATE TABLE TSERVER.d_seoverview_dc1data
(
    id              bigint NOT NULL,
    P               real   NOT NULL,
    I               real   NOT NULL,
    U               real   NOT NULL,
    bc1_I           real   NOT NULL,
    bc1_U           real   NOT NULL,
    bc1_soc         real   NOT NULL,
    bc1_soh         real   NOT NULL,
    bc2_I           real   NOT NULL,
    bc2_U           real   NOT NULL,
    bc2_soc         real   NOT NULL,
    bc2_soh         real   NOT NULL,
    bc3_I           real   NOT NULL,
    bc3_U           real   NOT NULL,
    bc3_soc         real   NOT NULL,
    bc3_soh         real   NOT NULL,
    bc4_I           real   NOT NULL,
    bc4_U           real   NOT NULL,
    bc4_soc         real   NOT NULL,
    bc4_soh         real   NOT NULL,
    PCS_comm_error  int    NOT NULL,
    PCS_ctrl_fail   int    NOT NULL,
    EMS_comm_error  int    NOT NULL,
    rack_vdiff_high int    NOT NULL,
    rack_cdiff_high int    NOT NULL,
    em_shutdown     int    NOT NULL,
    ac_comm_error   int    NOT NULL,
    overv_3rd_level int    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_seoverview_dc1data IS '直流侧1量测和保护信号表';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.I IS '单位：A';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.U IS '单位：V';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc1_I IS '电池簇1电流，单位：A';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc1_U IS '电池簇1电压，单位：V';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc1_soc IS '电池簇1剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc1_soh IS '电池簇1当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc2_I IS '电池簇2电流，单位：A';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc2_U IS '电池簇2电压，单位：V';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc2_soc IS '电池簇2剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc2_soh IS '电池簇2当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc3_I IS '电池簇3电流，单位：A';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc3_U IS '电池簇3电压，单位：V';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc3_soc IS '电池簇3剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc3_soh IS '电池簇1当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc4_I IS '电池簇4电流，单位：A';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc4_U IS '电池簇4电压，单位：V';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc4_soc IS '电池簇4剩余电量百分比，单位：%';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.bc4_soh IS '电池簇4当前容量百分比，单位：%';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.PCS_comm_error IS 'PCS通讯故障报警，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.PCS_ctrl_fail IS 'PCS控制失效报警，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.EMS_comm_error IS 'EMS通讯故障报警，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.rack_vdiff_high IS 'Rack压差过大报警，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.rack_cdiff_high IS 'Rack电流差过大报警，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.em_shutdown IS '紧急停机，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.ac_comm_error IS '空调通信故障，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1data.overv_3rd_level IS '单体过压三级报警，1有效';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_seoverview_dc1curve
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_seoverview_dc1curve;
CREATE TABLE TSERVER.d_seoverview_dc1curve
(
    id   bigint       NOT NULL,
    sid  bigint       NOT NULL,
    time timestamp(0) NOT NULL,
    P    real         NOT NULL,
    I    real         NOT NULL,
    U    real         NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_seoverview_dc1curve IS '直流侧1曲线数据表';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1curve.sid is '储能设备id';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1curve.time IS '实测数据的时间戳，精度到分';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1curve.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1curve.I IS '单位：A';
COMMENT ON COLUMN TSERVER.d_seoverview_dc1curve.U IS '单位：V';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_seoverview_dc2data
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_seoverview_dc2data;
CREATE TABLE TSERVER.d_seoverview_dc2data
(
    id              bigint NOT NULL,
    P               real   NOT NULL,
    I               real   NOT NULL,
    U               real   NOT NULL,
    PCS_comm_error  int    NOT NULL,
    PCS_ctrl_fail   int    NOT NULL,
    EMS_comm_error  int    NOT NULL,
    rack_vdiff_high int    NOT NULL,
    rack_cdiff_high int    NOT NULL,
    em_shutdown     int    NOT NULL,
    ac_comm_error   int    NOT NULL,
    overv_3rd_level int    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_seoverview_dc2data IS '直流侧1曲线数据表';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2data.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2data.I IS '单位：A';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2data.U IS '单位：V';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2data.PCS_comm_error IS 'PCS通讯故障报警，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2data.PCS_ctrl_fail IS 'PCS控制失效报警，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2data.EMS_comm_error IS 'EMS通讯故障报警，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2data.rack_vdiff_high IS 'Rack压差过大报警，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2data.rack_cdiff_high IS 'Rack电流差过大报警，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2data.em_shutdown IS '紧急停机，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2data.ac_comm_error IS '空调通信故障，1有效';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2data.overv_3rd_level IS '单体过压三级报警，1有效';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_seoverview_dc2data
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_seoverview_dc2curve;
CREATE TABLE TSERVER.d_seoverview_dc2curve
(
    id   bigint       NOT NULL,
    sid  int          NOT NULL,
    time timestamp(0) NOT NULL,
    P    real         NOT NULL,
    I    real         NOT NULL,
    U    real         NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_seoverview_dc2curve IS '直流侧1曲线数据表';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2curve.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2curve.sid IS '储能设备id';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2curve.time IS '实测数据的时间戳，精度到分';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2curve.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2curve.I IS '单位：A';
COMMENT ON COLUMN TSERVER.d_seoverview_dc2curve.U IS '单位：V';
COMMIT TRANSACTION;

-- 8. 柔直互联台区-光伏运行状态表
-- --------------------------------------------
-- Table structure for d_poverview_station
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_poverview_station;
CREATE TABLE TSERVER.d_poverview_station
(
    id   bigint      NOT NULL,
    pid  bigint      NOT NULL,
    name varchar(64) NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_poverview_station IS '光伏信息表';
COMMENT ON COLUMN TSERVER.d_poverview_station.pid is '光伏电站id';
COMMENT ON COLUMN TSERVER.d_poverview_station.name is '光伏电站名称';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_poverview_overview
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_poverview_overview;
CREATE TABLE TSERVER.d_poverview_overview
(
    id        bigint NOT NULL,
    pid       bigint NOT NULL,
    capacity  real   NOT NULL,
    power     real   NOT NULL,
    max_power real   NOT NULL,
    min_power real   NOT NULL,
    daily_EP  real   NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_poverview_overview IS '光伏总览表';
COMMENT ON COLUMN TSERVER.d_poverview_overview.pid is '光伏电站id';
COMMENT ON COLUMN TSERVER.d_poverview_overview.capacity is '光伏电站容量，单位：kW';
COMMENT ON COLUMN TSERVER.d_poverview_overview.power is '光伏电站实时功率，单位：kW';
COMMENT ON COLUMN TSERVER.d_poverview_overview.max_power is '光伏电站当日最大功率，单位：kW';
COMMENT ON COLUMN TSERVER.d_poverview_overview.min_power is '光伏电站当日最小功率，单位：kW';
COMMENT ON COLUMN TSERVER.d_poverview_overview.daily_EP is '光伏电站当日发电量，单位：kW/h';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_poverview_mesa
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_poverview_mesa;
CREATE TABLE TSERVER.d_poverview_mesa
(
    id  bigint NOT NULL,
    pid bigint NOT NULL,
    P   real   NOT NULL,
    Q   real   NOT NULL,
    Ia  real   NOT NULL,
    Ib  real   NOT NULL,
    Ic  real   NOT NULL,
    Ua  real   NOT NULL,
    Ub  real   NOT NULL,
    Uc  real   NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_poverview_mesa IS '光伏量测数据表';
COMMENT ON COLUMN TSERVER.d_poverview_mesa.pid IS '光伏电站id';
COMMENT ON COLUMN TSERVER.d_poverview_mesa.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.d_poverview_mesa.Q IS '单位：kVar';
COMMENT ON COLUMN TSERVER.d_poverview_mesa.Ia IS '单位：A';
COMMENT ON COLUMN TSERVER.d_poverview_mesa.Ib IS '单位：A';
COMMENT ON COLUMN TSERVER.d_poverview_mesa.Ic IS '单位：A';
COMMENT ON COLUMN TSERVER.d_poverview_mesa.Ua IS '单位：V';
COMMENT ON COLUMN TSERVER.d_poverview_mesa.Ub IS '单位：V';
COMMENT ON COLUMN TSERVER.d_poverview_mesa.Uc IS '单位：V';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_poverview_protect
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_poverview_protect;
CREATE TABLE TSERVER.d_poverview_protect
(
    id                 bigint NOT NULL,
    pid                bigint NOT NULL,
    lcl                int    NOT NULL,
    prot_act           int    NOT NULL,
    ocswitch_act       int    NOT NULL,
    interval_exception int    NOT NULL,
    interval_fail      int    NOT NULL,
    interval_singal    int    NOT NULL,
    mp_comm_error      int    NOT NULL,
    bh_position        int    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_poverview_protect IS '光伏保护信号表';
COMMENT ON COLUMN TSERVER.d_poverview_protect.pid IS '光伏电站id';
COMMENT ON COLUMN TSERVER.d_poverview_protect.lcl IS '就地，1有效';
COMMENT ON COLUMN TSERVER.d_poverview_protect.prot_act IS '保护动作，1有效';
COMMENT ON COLUMN TSERVER.d_poverview_protect.ocswitch_act IS '分合闸动作，1有效';
COMMENT ON COLUMN TSERVER.d_poverview_protect.interval_exception IS '间隔设备异常，1有效';
COMMENT ON COLUMN TSERVER.d_poverview_protect.interval_fail IS '间隔设备故障，1有效';
COMMENT ON COLUMN TSERVER.d_poverview_protect.interval_singal IS '间隔事故信号，1有效';
COMMENT ON COLUMN TSERVER.d_poverview_protect.mp_comm_error IS '测保装置通讯中断，1有效';
COMMENT ON COLUMN TSERVER.d_poverview_protect.bh_position IS '手车工作位置，1有效';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_poverview_curve
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_poverview_curve;
CREATE TABLE TSERVER.d_poverview_curve
(
    id   bigint       NOT NULL,
    pid  bigint       NOT NULL,
    time timestamp(0) NOT NULL,
    P_c  real         NOT NULL,
    Q_c  real         NOT NULL,
    Ia_c real         NOT NULL,
    Ib_c real         NOT NULL,
    Ic_c real         NOT NULL,
    Ua_c real         NOT NULL,
    Ub_c real         NOT NULL,
    Uc_c real         NOT NULL,
    P    real         NOT NULL,
    Q    real         NOT NULL,
    Ia   real         NOT NULL,
    Ib   real         NOT NULL,
    Ic   real         NOT NULL,
    Ua   real         NOT NULL,
    Ub   real         NOT NULL,
    Uc   real         NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_poverview_curve IS '光伏曲线数据表';
COMMENT ON COLUMN TSERVER.d_poverview_curve.pid IS '光伏电站id';
COMMENT ON COLUMN TSERVER.d_poverview_curve.time IS '预测和实测数据的时间戳，精度到分';
COMMENT ON COLUMN TSERVER.d_poverview_curve.P_c IS '预测值，单位：kW';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Q_c IS '预测值，单位：kVar';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Ia_c IS '预测值，单位：A';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Ib_c IS '预测值，单位：A';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Ic_c IS '预测值，单位：A';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Ua_c IS '预测值，单位：V';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Ub_c IS '预测值，单位：V';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Uc_c IS '预测值，单位：V';
COMMENT ON COLUMN TSERVER.d_poverview_curve.P IS '实测值，单位：kW';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Q IS '实测值，单位：kVar';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Ia IS '实测值，单位：A';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Ib IS '实测值，单位：A';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Ic IS '实测值，单位：A';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Ua IS '实测值，单位：V';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Ub IS '实测值，单位：V';
COMMENT ON COLUMN TSERVER.d_poverview_curve.Uc IS '实测值，单位：V';
COMMIT TRANSACTION;

-- 9. 柔直互联台区-台区运行状态
-- --------------------------------------------
-- Table structure for d_aoverview_overview
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_aoverview_overview;
CREATE TABLE TSERVER.d_aoverview_overview
(
    id            bigint NOT NULL,
    p_real_power  real   NOT NULL,
    load_power    real   NOT NULL,
    combine_power real   NOT NULL,
    daily_EP      real   NOT NULL,
    load_EP       real   NOT NULL,
    total_EP      real   NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_aoverview_overview IS '台区总览表';
COMMENT ON COLUMN TSERVER.d_aoverview_overview.p_real_power IS '光伏实时发电功率，单位：kW';
COMMENT ON COLUMN TSERVER.d_aoverview_overview.load_power IS '负荷实时功率，单位：kW';
COMMENT ON COLUMN TSERVER.d_aoverview_overview.combine_power IS '上网功率，单位：kW';
COMMENT ON COLUMN TSERVER.d_aoverview_overview.daily_EP IS '光伏当日发电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.d_aoverview_overview.load_EP IS '负荷当日用电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.d_aoverview_overview.total_EP IS '累计上网电量，单位：kW/h';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_aoverview_mesa
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_aoverview_mesa;
CREATE TABLE TSERVER.d_aoverview_mesa
(
    id bigint NOT NULL,
    P  real   NOT NULL,
    Q  real   NOT NULL,
    Ia real   NOT NULL,
    Ib real   NOT NULL,
    Ic real   NOT NULL,
    Ua real   NOT NULL,
    Ub real   NOT NULL,
    Uc real   NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_aoverview_mesa IS '台区量测数据表';
COMMENT ON COLUMN TSERVER.d_aoverview_mesa.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.d_aoverview_mesa.Q IS '单位：kVar';
COMMENT ON COLUMN TSERVER.d_aoverview_mesa.Ia IS '单位：A';
COMMENT ON COLUMN TSERVER.d_aoverview_mesa.Ib IS '单位：A';
COMMENT ON COLUMN TSERVER.d_aoverview_mesa.Ic IS '单位：A';
COMMENT ON COLUMN TSERVER.d_aoverview_mesa.Ua IS '单位：V';
COMMENT ON COLUMN TSERVER.d_aoverview_mesa.Ub IS '单位：V';
COMMENT ON COLUMN TSERVER.d_aoverview_mesa.Uc IS '单位：V';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_aoverview_protect
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_aoverview_protect;
CREATE TABLE TSERVER.d_aoverview_protect
(
    id                 bigint NOT NULL,
    lcl                int    NOT NULL,
    prot_act           int    NOT NULL,
    ocswitch_act       int    NOT NULL,
    interval_exception int    NOT NULL,
    interval_fail      int    NOT NULL,
    interval_singal    int    NOT NULL,
    mp_comm_error      int    NOT NULL,
    bh_position        int    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_aoverview_protect IS '台区保护信号表';
COMMENT ON COLUMN TSERVER.d_aoverview_protect.lcl IS '就地，1有效';
COMMENT ON COLUMN TSERVER.d_aoverview_protect.prot_act IS '保护动作，1有效';
COMMENT ON COLUMN TSERVER.d_aoverview_protect.ocswitch_act IS '分合闸动作，1有效';
COMMENT ON COLUMN TSERVER.d_aoverview_protect.interval_exception IS '间隔设备异常，1有效';
COMMENT ON COLUMN TSERVER.d_aoverview_protect.interval_fail IS '间隔设备故障，1有效';
COMMENT ON COLUMN TSERVER.d_aoverview_protect.interval_singal IS '间隔事故信号，1有效';
COMMENT ON COLUMN TSERVER.d_aoverview_protect.mp_comm_error IS '测保装置通讯中断，1有效';
COMMENT ON COLUMN TSERVER.d_aoverview_protect.bh_position IS '手车工作位置，1有效';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_aoverview_curve
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_aoverview_curve;
CREATE TABLE TSERVER.d_aoverview_curve
(
    id   bigint       NOT NULL,
    time timestamp(0) NOT NULL,
    P_c  real         NOT NULL,
    Q_c  real         NOT NULL,
    Ia_c real         NOT NULL,
    Ib_c real         NOT NULL,
    Ic_c real         NOT NULL,
    Ua_c real         NOT NULL,
    Ub_c real         NOT NULL,
    Uc_c real         NOT NULL,
    P    real         NOT NULL,
    Q    real         NOT NULL,
    Ia   real         NOT NULL,
    Ib   real         NOT NULL,
    Ic   real         NOT NULL,
    Ua   real         NOT NULL,
    Ub   real         NOT NULL,
    Uc   real         NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_aoverview_curve IS '台区曲线数据表';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.time IS '预测和实测数据的时间戳，精度到分';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.P_c IS '预测值，单位：kW';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Q_c IS '预测值，单位：kVar';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Ia_c IS '预测值，单位：A';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Ib_c IS '预测值，单位：A';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Ic_c IS '预测值，单位：A';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Ua_c IS '预测值，单位：V';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Ub_c IS '预测值，单位：V';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Uc_c IS '预测值，单位：V';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.P IS '实测值，单位：kW';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Q IS '实测值，单位：kVar';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Ia IS '实测值，单位：A';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Ib IS '实测值，单位：A';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Ic IS '实测值，单位：A';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Ua IS '实测值，单位：V';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Ub IS '实测值，单位：V';
COMMENT ON COLUMN TSERVER.d_aoverview_curve.Uc IS '实测值，单位：V';
COMMIT TRANSACTION;

-- 10. 柔直互联台区-柔直互联运行状态表
-- --------------------------------------------
-- Table structure for d_fsoverview_acdata
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_fsoverview_acdata;
CREATE TABLE TSERVER.d_fsoverview_acdata
(
    id              bigint NOT NULL,
    P               real   NOT NULL,
    I               real   NOT NULL,
    U               real   NOT NULL,
    dc_EP           real   NOT NULL,
    dc_bus_sc       int    NOT NULL,
    dc_over_current int    NOT NULL,
    bb_over_current int    NOT NULL,
    dc_reserse      int    NOT NULL,
    dc_v_low        int    NOT NULL,
    dc_v_high       int    NOT NULL,
    dc_charge_error int    NOT NULL,
    dc_relay_sc     int    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_fsoverview_acdata IS '交流侧量测和保护信号表';
COMMENT ON COLUMN TSERVER.d_fsoverview_acdata.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.d_fsoverview_acdata.I IS '单位：A';
COMMENT ON COLUMN TSERVER.d_fsoverview_acdata.U IS '直流母线电压，单位：V';
COMMENT ON COLUMN TSERVER.d_fsoverview_acdata.dc_EP IS '直流交互能量，单位：kW/h';
COMMENT ON COLUMN TSERVER.d_fsoverview_acdata.dc_bus_sc IS '直流母线短路，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_acdata.dc_over_current IS '直流过流，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_acdata.bb_over_current IS '平衡桥过流，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_acdata.dc_reserse IS '直流电压反接，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_acdata.dc_v_low IS '直流电压低，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_acdata.dc_v_high IS '直流电压高，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_acdata.dc_charge_error IS '直流预充电故障，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_acdata.dc_relay_sc IS '直流继电器短路，1有效';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_fsoverview_accurve
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_fsoverview_accurve;
CREATE TABLE TSERVER.d_fsoverview_accurve
(
    id   bigint       NOT NULL,
    did  bigint       NOT NULL,
    time timestamp(0) NOT NULL,
    P    real         NOT NULL,
    U    real         NOT NULL,
    I    real         NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_fsoverview_accurve IS '交流侧曲线数据表';
COMMENT ON COLUMN TSERVER.d_fsoverview_accurve.did IS '柔直互联单元id';
COMMENT ON COLUMN TSERVER.d_fsoverview_accurve.time IS '实测数据的时间戳，精度到分';
COMMENT ON COLUMN TSERVER.d_fsoverview_accurve.P IS '实测值，单位：kW';
COMMENT ON COLUMN TSERVER.d_fsoverview_accurve.U IS '实测值，单位：V';
COMMENT ON COLUMN TSERVER.d_fsoverview_accurve.I IS '实测值，单位：A';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_fsoverview_dcdata
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_fsoverview_dcdata;
CREATE TABLE TSERVER.d_fsoverview_dcdata
(
    id              bigint NOT NULL,
    P               real   NOT NULL,
    Q               real   NOT NULL,
    Ia              real   NOT NULL,
    Ib              real   NOT NULL,
    Ic              real   NOT NULL,
    PCS_comm_error  int    NOT NULL,
    PCS_ctrl_fail   int    NOT NULL,
    EMS_comm_error  int    NOT NULL,
    rack_vdiff_high int    NOT NULL,
    rack_cdiff_high int    NOT NULL,
    em_shutdown     int    NOT NULL,
    ac_comm_error   int    NOT NULL,
    overv_3rd_level int    NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_fsoverview_dcdata IS '直流侧量测和保护信号表';
COMMENT ON COLUMN TSERVER.d_fsoverview_dcdata.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.d_fsoverview_dcdata.Q IS '单位：kVar';
COMMENT ON COLUMN TSERVER.d_fsoverview_dcdata.Ia IS '单位：A';
COMMENT ON COLUMN TSERVER.d_fsoverview_dcdata.Ib IS '单位：A';
COMMENT ON COLUMN TSERVER.d_fsoverview_dcdata.Ic IS '单位：A';
COMMENT ON COLUMN TSERVER.d_fsoverview_dcdata.PCS_comm_error IS 'A相输出过载告警，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_dcdata.PCS_ctrl_fail IS 'B相输出过载告警，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_dcdata.EMS_comm_error IS 'C相输出过载告警，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_dcdata.rack_vdiff_high IS 'A相输出过流，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_dcdata.rack_cdiff_high IS 'B相输出过流，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_dcdata.em_shutdown IS 'C相输出过流，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_dcdata.ac_comm_error IS '交流继电器短路，1有效';
COMMENT ON COLUMN TSERVER.d_fsoverview_dcdata.overv_3rd_level IS 'AC逐波限流关机，1有效';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_fsoverview_dccurve
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_fsoverview_dccurve;
CREATE TABLE TSERVER.d_fsoverview_dccurve
(
    id   bigint       NOT NULL,
    did  bigint       NOT NULL,
    time timestamp(0) NOT NULL,
    P    real         NOT NULL,
    Q    real         NOT NULL,
    Ia   real         NOT NULL,
    Ib   real         NOT NULL,
    Ic   real         NOT NULL,
    Ua   real         NOT NULL,
    Ub   real         NOT NULL,
    Uc   real         NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_fsoverview_dccurve IS '直流侧曲线数据表';
COMMENT ON COLUMN TSERVER.d_fsoverview_dccurve.did IS '柔直互联单元id';
COMMENT ON COLUMN TSERVER.d_fsoverview_dccurve.time IS '实测数据的时间戳，精度到分';
COMMENT ON COLUMN TSERVER.d_fsoverview_dccurve.P IS '单位：kW';
COMMENT ON COLUMN TSERVER.d_fsoverview_dccurve.Q IS '单位：kVar';
COMMENT ON COLUMN TSERVER.d_fsoverview_dccurve.Ia IS '单位：A';
COMMENT ON COLUMN TSERVER.d_fsoverview_dccurve.Ib IS '单位：A';
COMMENT ON COLUMN TSERVER.d_fsoverview_dccurve.Ic IS '单位：A';
COMMENT ON COLUMN TSERVER.d_fsoverview_dccurve.Ua IS '单位：V';
COMMENT ON COLUMN TSERVER.d_fsoverview_dccurve.Ub IS '单位：V';
COMMENT ON COLUMN TSERVER.d_fsoverview_dccurve.Uc IS '单位：V';
COMMIT TRANSACTION;

-- 11. 柔直互联台区-经济效益分析表
-- --------------------------------------------
-- Table structure for d_ebanalysis_overview
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_ebanalysis_overview;
CREATE TABLE TSERVER.d_ebanalysis_overview
(
    id                 bigint           NOT NULL,
    total_charge_EP    double precision NOT NULL,
    total_discharge_EP double precision NOT NULL,
    total_pg_EP        double precision NOT NULL,
    total_pc_EP        double precision NOT NULL,
    total_eb           money            NOT NULL,
    total_pvd_benefit  money            NOT NULL,
    total_pp_benefit   money            NOT NULL,
    total_cer          double precision NOT NULL,
    total_sscq         double precision NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_ebanalysis_overview IS '经济效益分析总览表';
COMMENT ON COLUMN TSERVER.d_ebanalysis_overview.total_charge_EP IS '储能累计充电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.d_ebanalysis_overview.total_discharge_EP IS '储能累计放电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.d_ebanalysis_overview.total_pg_EP IS '光伏累计发电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.d_ebanalysis_overview.total_pc_EP IS '光伏累计上网电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.d_ebanalysis_overview.total_eb IS '累计经济效益，单位：元';
COMMENT ON COLUMN TSERVER.d_ebanalysis_overview.total_pvd_benefit IS '累计峰谷差收益，单位：元';
COMMENT ON COLUMN TSERVER.d_ebanalysis_overview.total_pp_benefit IS '累计光伏自用收益，单位：元';
COMMENT ON COLUMN TSERVER.d_ebanalysis_overview.total_cer IS '累计碳减排量，单位：吨';
COMMENT ON COLUMN TSERVER.d_ebanalysis_overview.total_sscq IS '累计节约标煤量，单位：吨';
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_ebanalysis_curve
-- --------------------------------------------
BEGIN TRANSACTION;
DROP TABLE IF EXISTS TSERVER.d_ebanalysis_curve;
CREATE TABLE TSERVER.d_ebanalysis_curve
(
    id           bigint NOT NULL,
    time         date   NOT NULL,
    charge_EP    real   NOT NULL,
    discharge_EP real   NOT NULL,
    pg_EP        real   NOT NULL,
    pc_EP        real   NOT NULL,
    eb           money  NOT NULL,
    pvd_benefit  money  NOT NULL,
    pp_benefit   money  NOT NULL,
    PRIMARY KEY (id) USING INDEX TABLESPACE "TSERVER"
) WITHOUT OIDS
  TABLESPACE "TSERVER";
COMMENT ON TABLE TSERVER.d_ebanalysis_curve IS '经济效益分析总览表';
COMMENT ON COLUMN TSERVER.d_ebanalysis_curve.time IS '数据时间戳，精度到日';
COMMENT ON COLUMN TSERVER.d_ebanalysis_curve.charge_EP IS '储能月度充电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.d_ebanalysis_curve.discharge_EP IS '储能月度放电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.d_ebanalysis_curve.pg_EP IS '光伏月度发电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.d_ebanalysis_curve.pc_EP IS '光伏月度上网电量，单位：kW/h';
COMMENT ON COLUMN TSERVER.d_ebanalysis_curve.eb IS '月度经济效益，单位：元';
COMMENT ON COLUMN TSERVER.d_ebanalysis_curve.pvd_benefit IS '月度峰谷差收益，单位：元';
COMMENT ON COLUMN TSERVER.d_ebanalysis_curve.pp_benefit IS '月度光伏自用收益，单位：元';
COMMIT TRANSACTION;

-- 12. 柔直互联台区-协调控制表
-- todo: