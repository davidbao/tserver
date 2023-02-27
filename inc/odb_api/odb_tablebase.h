/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  odb_tablebase.h
DESCRIPTION:  class system interface define
FUNCTION LIST:
COMMENT:
History:                                                        
Date          Version     Modifier                Activities
2003-09-10    1.0                              modify
2003-12-03    1.1                              modify
================================================================================
2003-09-10    1.0                              created
*******************************************************************************/
#ifndef	__ODB_TABLEBASE_H__
#define	__ODB_TABLEBASE_H__

#ifndef	__ODB_PUBLIC_H__
#include "odb_public.h"
#endif

#include "odb_prv_struct.h"

/*
#ifndef	__ODB_INCLUDE_H__
#include "odb_include.h"
#endif
*/
/*
#ifndef	__ODB_BASEBUFFER_H__
#include "odb_basebuffer.h"
#endif
*/
#include "pub.h"

using namespace std;

namespace  ODB
{
class CTableBase
{
public:

	CTableBase(){}
	virtual ~CTableBase(){}
	//
	//query	the whole table
	//
	virtual int	TableGet(CBuffer& buf_base) { return 0; }
	virtual int	TableGet(const int field_no, CBuffer& buf_base) { return 0; }
	virtual int	TableGet(const char* field_name, CBuffer& buf_base) { return 0; }
	virtual int	TableGet(const std::vector<int>& vec_field_no, CBuffer& buf_base) { return 0; }
	
	//
	//query	by key word
	//
	virtual int	TableGetByKey(const char* key_ptr, char* buf_ptr, const int buf_size) { return 0; }
	virtual int	TableGetByKey(const char* key_ptr, const int field_no, char* field_buf_ptr, const int buf_size) { return 0; }
	virtual int	TableGetByKey(const char* key_ptr, const char* field_name, char* field_buf_ptr, const int buf_size) { return 0; }
	virtual int	TableGetByKey(const char* key_ptr, const std::vector<int>& vec_field_no, char* field_buf_ptr, const int buf_size) { return 0; }
	virtual int	TableGetByKey(const char* key_ptr, const int keybuf_size, const std::vector<int>& vec_field_no, char* field_buf_ptr, const int buf_size) { return 0; }

	//
	//modify
	//
	virtual int	TableModify(const char* buf_ptr, const int record_num, const int record_size) { return 0; }
	virtual int	TableModify(const int field_no, const char* field_buf_ptr, const int buf_size) { return 0; }
	virtual int	TableModify(const char* field_name, const char* field_buf_ptr, const int buf_size) { return 0; }
	virtual int	TableModify(const std::vector<int>& vec_field_no, const char* field_buf_ptr, const int buf_size) { return 0; }
	
	
	//
	//by keyword && fields
	//
	virtual int	TableModifyByKey(const char* key_ptr, const int field_no, const char* field_buf_ptr, const int buf_size) { return 0; }
	virtual int	TableModifyByKey(const char* key_ptr, const char* field_name, const char* field_buf_ptr, const int buf_size) { return 0; }
	virtual int	TableModifyByKey(const char* key_ptr, const std::vector<int>& vec_field_no, const char* field_buf_ptr, const int buf_size) { return 0; }
	
	
	//
	//SQL interface
	//
	virtual int SqlGet(const char* str_sql, CBuffer& buf_base) { return 0; }
	
	virtual int SqlGet(const char* str_sql, char** buf_ptr, int& buf_size) { return 0; }
	
public:
	//
	//table PARA
	//
	virtual int	GetTablePara(short& field_num, int& record_num, int& record_size) { return 0; }
	virtual int	GetTablePara(struct TABLE_PARA& table_para, std::vector<struct STDB_FIELD_TAB>& vec_field_para) { return 0; }
	virtual int	GetTablePara(const char* str_attribute, char** field_para_ptr, int& buf_size) { return 0; }
	
	virtual int GetFieldInfo(const std::vector<int>  vec_field_no, std::vector<struct FIELD_BASE_INFO>& vec_field) { return 0;}

public:
	//Name and No
	virtual int GetAppNameByNo(char* app_name, const int app_no) = 0;
	virtual int GetAppNoByName(int& app_no, const char* app_name) = 0;

	virtual int GetTableNameByNo(char* table_name, const int table_no, const bool is_eng=true) = 0;
	virtual int GetTableNoByName(int& table_no, const char* table_name, const bool is_eng=true) = 0;

	virtual int GetFieldNameByNo(char* field_name, const int field_no, const bool is_eng=true) = 0;
	virtual int	GetFieldNoByName(int& field_no, const char* field_name, const bool is_eng=true) = 0;

public:
	virtual int	MenuRead(const char* menu_name, std::vector<struct MENU_INFO>& vec_menu, const int menu_status=MENU_ON){ return 0; }
	
	virtual int TableReleMenuRead(const int table_no, std::vector<struct MENU_RELEVANT>& vec_menu){ return 0; }
	
	virtual int	GetRefMenuString(const int table_no, std::vector<struct MENU_STRING>& vec_menu){ return 0; }
	
	virtual int	GetNameStringByKeyID(const KEY_ID_STRU& keyid_stru, string& ref_string){ return 0; }
	virtual int	GetNameStringByKeyID(const std::vector<KEY_ID_STRU>& vec_keyid, std::vector<string>& vec_ref_string){ return 0; }

	virtual int GetNameStringByID(const long reference_id, string& ref_string){ return 0; }

	virtual int	GetNameStringBykey(const char* key_ptr, string& ref_string){ return 0; }

	virtual int	GetNameStringByAppKeyID( const std::vector<struct APP_KEY_STRU>& vec_appkeyid, std::vector<string>& vec_name_string){ return 0; }

	virtual int	GetNameStringByAppID( const std::vector<APP_ID_STRU>& vec_appid, std::vector<string>& vec_name_string){ return 0; }
	
	virtual int ColMeasType( const std::vector<KEY_ID_STRU> vec_keyid , std::vector<int>& vec_col_prop ) { return 0; }
//	virtual int GraphGetAppInfo(const struct ODB::GRAPH_ORDER_KEY_REQ& graph_req, struct ODB::GRAPH_ORDER_KEY_RSP& graph_rsp) const;
//	virtual int GraphGetData(const struct CODB_NET::GRAPH_REAL_REQ& real_req, CODB_NET::GRAPH_REAL_RSP_var& real_rsp) const;

public:
	virtual int	GetFacNameByKeyNo(const int table_no, const char* key_ptr, string& ref_string) { return 0; }
	virtual int	GetFacNameByFacID(char* fac_name, const long fac_id) { return 0; }
	virtual int	GetFacNameByFacNo(char* fac_name, const int fac_no) { return 0; }

	virtual int	GetFacIDByFacName(long& fac_id, const char* fac_name) { return 0; }	
	virtual int	GetFacNoByFacName(int& fac_no, const char* fac_name) { return 0; }
	
	virtual int	GetFacIDByFacNo(long& fac_id, const int fac_no) { return 0; }
	virtual int GetFacNoByFacID(int& fac_no, const long fac_id) { return 0; }
	
//wj added on 20081205
public:
	virtual int	TableWrite(const char* buf_ptr, const int record_num, const int record_size) { return 0; }
	virtual int	TableWrite(const char* buf_ptr, const int buf_size) { return 0; }
	virtual int	DeleteRecord(const char* key_ptr) { return 0; }
	virtual int	TableClear() { return 0; }
	virtual int GetFieldPara(struct FIELD_BASE_INFO& field_info){ return 0; }
};

}

#endif
