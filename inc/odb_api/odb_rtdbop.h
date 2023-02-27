/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  odb_rtdbbase.h
DESCRIPTION:  Unified RTDB system interface definition
FUNCTION LIST:
COMMENT:
History:                                                        
Date          Version     Modifier                Activities
2004-06-25    1.1         						add context_no parameter
2003-08-18    1.0                              modify
2003-12-03    1.0                              modify //formal
================================================================================
2003-08-18    1.0                              created
*******************************************************************************/
#ifndef	__ODB_RTDBOP_H__
#define	__ODB_RTDBOP_H__

#ifndef	__ODB_RTDBBASE_H__
#include "odb_rtdbbase.h"
#endif

#ifndef	__ODB_RTDBTABLE_H__
#include "odb_rtdbtable.h"
#endif

using namespace std;

namespace ODB
{
class CRtdbOp: public CTableBase
{
public:
	CRtdbOp();
	CRtdbOp(const int app_no, const short context_no = 0);
	CRtdbOp(const int app_no, const int table_no, const short context_no = 0);
	//CRtdbOp(const char *hostname);

	~CRtdbOp();

public:
	int Open(const int table_no);
	int Open(const int app_no, const int table_no, const short context_no = 0);
	//int Open(const char *hostname);  
	
	int SetAppNo(const int app_no, const short context_no = 0);
	
	int SetHostMode(const char* host_name); 

//all interface
public:
	//
	//query	the whole table
	//
	int	TableGet(CBuffer& buf_base) const;
	int	TableGet(const int field_no, CBuffer& buf_base) const;
	int	TableGet(const char* field_name, CBuffer& buf_base) const;
	int	TableGet(const std::vector<int>& vec_field_no, CBuffer& buf_base) const;
	
	//
	//query	by key word
	//
	int	TableGetByKey(const char* key_ptr, char* buf_ptr, const int buf_size) const;
	int	TableGetByKey(const char* key_ptr, const int field_no, char* field_buf_ptr, const int buf_size) const;
	int	TableGetByKey(const char* key_ptr, const char* field_name, char* field_buf_ptr, const int buf_size) const;
	int	TableGetByKey(const char* key_ptr, const std::vector<int>& vec_field_no, char* field_buf_ptr, const int buf_size) const;
	int	TableGetByKey(const char* key_ptr, const int keybuf_size, const std::vector<int>& vec_field_no, char* field_buf_ptr, const int buf_size) const;

	//
	//modify
	//
	int	TableModify(const char* buf_ptr, const int record_num, const int record_size) const;
	int	TableModify(const int field_no, const char* field_buf_ptr, const int buf_size) const;
	int	TableModify(const char* field_name, const char* field_buf_ptr, const int buf_size) const;
	int	TableModify(const std::vector<int>& vec_field_no, const char* field_buf_ptr, const int buf_size) const;
	
	//
	//by keyword && fields
	//
	int	TableModifyByKey(const char* key_ptr, const int field_no, const char* field_buf_ptr, const int buf_size) const;
	int	TableModifyByKey(const char* key_ptr, const char* field_name, const char* field_buf_ptr, const int buf_size) const;
	int	TableModifyByKey(const char* key_ptr, const std::vector<int>& vec_field_no, const char* field_buf_ptr, const int buf_size) const;
	
	//
	//SQL interface
	//
	int SqlGet(const char* str_sql, CBuffer& buf_base) const;
	
public:
	int	GetTablePara(short& field_num, int& record_num, int& record_size) const;
	int	GetTablePara(struct TABLE_PARA& table_para, std::vector<struct STDB_FIELD_TAB>& vec_field_para) const;
	int	GetTablePara(const char* str_attribute, CBuffer& buf_base) const;
	
	int GetFieldInfo(const std::vector<int>  vec_field_no, std::vector<struct FIELD_BASE_INFO>& vec_field) const;

public:
	int GetFieldNameByNo(char* field_name, const int field_no, const bool is_eng=true);
	int	GetFieldNoByName(int& field_no, const char* field_name, const bool is_eng=true);
	
	int GetTableNameByNo(char* table_name, const int table_no, const bool is_eng=true);
	int GetTableNoByName(int& table_no, const char* table_name, const bool is_eng=true);
	
	int GetAppNameByNo(char* app_name, const int app_no);
	int GetAppNoByName(int& app_no, const char* app_name);	

public:
	int	MenuRead(const char* menu_name, std::vector<struct MENU_INFO>& vec_menu, const int menu_status=MENU_ON);
	
	int TableReleMenuRead(const int table_no, std::vector<struct MENU_RELEVANT>& vec_menu);
	
	int	GetRefMenuString(const int table_no, std::vector<struct ODB::MENU_STRING>& vec_menu);
	
	int	GetNameStringByKeyID(const KEY_ID_STRU& keyid_stru, string& ref_string);
	
	int	GetNameStringByKeyID(const std::vector<KEY_ID_STRU>& vec_keyid, std::vector<string>& vec_ref_string);

	int GetNameStringByID(const long reference_id, string& ref_string);

	int	GetNameStringBykey(const char* key_ptr, string& ref_string);

	int	GetNameStringByAppKeyID( const std::vector<struct APP_KEY_STRU>& vec_appkeyid, std::vector<string>& vec_name_string);

	int	GetNameStringByAppID( const std::vector<APP_ID_STRU>& vec_appid, std::vector<string>& vec_name_string);
	
	int ColMeasType( const std::vector<KEY_ID_STRU> vec_keyid , std::vector<int>& vec_col_prop );
//	virtual int GraphGetAppInfo(const struct ODB::GRAPH_ORDER_KEY_REQ& graph_req, struct ODB::GRAPH_ORDER_KEY_RSP& graph_rsp) const = 0;

//	virtual int GraphGetData(const struct CODB_NET::GRAPH_REAL_REQ& real_req, CODB_NET::GRAPH_REAL_RSP_var& real_rsp) const = 0;

public:
	int	GetFacNameByKeyNo(const int table_no, const char* key_ptr, string& ref_string);
	int	GetFacNameByFacID(char* fac_name, const long fac_id);
	int	GetFacNameByFacNo(char* fac_name, const int fac_no);

	int	GetFacIDByFacName(long& fac_id, const char* fac_name);
	int   GetFacNoByFacName(int& fac_no, const char* fac_name);

	int   GetFacIDByFacNo(long& fac_id, const int fac_no);
	int   GetFacNoByFacID(int& fac_no, const long fac_id);

public:
	int GetValueAndStatus(const std::vector<KEY_ID_STRU> &struVec , std::vector< struct INT_VALUE_STATUS >  &KeyIdValVec);  //ym

	int GetValueAndStatus(const std::vector<KEY_ID_STRU> &struVec , std::vector< struct FLOAT_VALUE_STATUS> & KeyIdValVec); //yc

	int GetValueAndStatus(const std::vector<KEY_ID_STRU> &struVec , std::vector< struct CHAR_VALUE_STATUS > & KeyIdValVec); //yx

	int GetValueAndStatus(const std::vector<KEY_ID_STRU> &struVec , std::vector< struct UNION_VALUE_STATUS> & KeyIdValVec  );//union type


private:
   void Instance();
   int  Refresh(const int mode_rtdb=0) const;

private:
	CRtdbOp(const CRtdbOp&);
	CRtdbOp& operator=(const CRtdbOp&);

private:
	CRtdbTable* m_TablePtr;
	
	short       m_ContextNo;
	int         m_AppNo;
	int         m_TableNo;
	bool		m_HostMode;	
	char        m_HostName[MAX_NAME_STRING_LEN];
};

}

#endif