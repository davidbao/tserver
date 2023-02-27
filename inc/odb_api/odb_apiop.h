/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  odb_api.h
DESCRIPTION:  general function declaration
FUNCTION LIST:
COMMENT:	  public application
History:                                                        
Date          Version     Modifier                Activities
2004-06-25    1.1         						add context_no parameter
2003-05-28    1.0                              modify
================================================================================
2003-05-28    1.0                              created
*******************************************************************************/

#ifndef __ODB_APIOP_H__
#define __ODB_APIOP_H__

#ifndef	__ODB_TABLEOP_H__
#include "odb_tableop.h"
#endif


using namespace std;

namespace ODB
{
	typedef vector<int>  IntVec;
	
	typedef struct   
	{
		int type;
		CTableOp	*m_pColOp;
		CTableOp	*m_pMenuOp;
		
	} TApiOpPriv; 

class CApiOp: public CTableBase
{
public:
	CApiOp();
	CApiOp( const int app_no ,const short context_no = 0);
	CApiOp( const int app_no, const int table_no , const short context_no = 0);
	~CApiOp();
	
	int Open(const int app_no, const int table_no, const short context_no = 0);
	int Open(const int table_no);
	int SetAppNo(const int app_no, const short context_no = 0);

public:
	int	MenuRead(const char* menu_name, std::vector<struct MENU_INFO>& vec_menu, const int menu_status=MENU_ON);
	int	MenuRead(const char* menu_name, char** buf_ptr, int& buf_size, const int menu_status=MENU_ON);//obsolete
	
	//
	int TableReleMenuRead(const int table_no, const int field_no );
	int TableReleMenuRead(const int table_no, const int field_no, std::vector<struct MENU_RELEVANT>& vec_menu);
	int TableReleMenuRead(const int table_no, std::vector<struct MENU_RELEVANT>& vec_menu);
	
	int TableReleMenuRead(const int table_no, const int field_no, char** buf_ptr, int& buf_size);//obsolete
	int TableReleMenuRead(const int table_no, char** buf_ptr, int& buf_size);//obsolete
	
	//
	int	GetRefMenuString(const int table_no, std::vector<struct MENU_STRING>& vec_menu);
	int	GetRefMenuString(const int table_no, char*& menu_buf_ptr, int& menu_buf_size);//obsolete
		
	//
	int GetNameStringByID(const long reference_id, std::string& ref_string);
	int GetNameStringByID(const long reference_id, char*& referenced_namebuf, int& namebuf_size);
	
	//
	int GetNameStringByID(const std::vector<long>& vec_refid, std::vector<std::string>& vec_ref_string);

	//
	int	GetNameStringByKeyID(const KEY_ID_STRU& keyid_stru, std::string& ref_string);
	int	GetNameStringByKeyID(const std::vector<KEY_ID_STRU>& vec_keyid, std::vector<string>& vec_ref_string);
	
	int	GetNameStringByKeyID( const KEY_ID_STRU& keyid_stru, char*& referenced_namebuf, int& namebuf_size);
	
	//
	int	GetNameStringBykey(const char* key_ptr, std::string& ref_string);
	int	GetNameStringBykey(const char* key_ptr, char*& referenced_namebuf, int& namebuf_size );

	//
	int	GetNameStringByAppKeyID( const std::vector<struct APP_KEY_STRU>& vec_appkeyid, std::vector<string>& vec_name_string);
	int	GetNameStringByAppID( const std::vector<APP_ID_STRU>& vec_appid, std::vector<string>& vec_name_string);

	//
	int	GetRefFlag(const int table_no, const short field_no);//obsolete
	int	GetRefFlag(const short field_no);

public:
	int	GetFacNameByKeyNo(const int table_no, const char* key_ptr, std::string& ref_string);
	int	GetFacNameByFacID(char* fac_name, const long fac_id);
	int	GetFacNameByFacNo(char* fac_name, const int fac_no);
	
	int	GetFacIDByFacName(long& fac_id, const char* fac_name);
	int   GetFacNoByFacName(int&  fac_no, const char* fac_name);

	int   GetFacIDByFacNo(long& fac_id, const int fac_no);
	int   GetFacNoByFacID(int& fac_no, const long fac_id);
public:
	int ColMeasType( const KEY_ID_STRU keyid , int& col_prop );
	int ColMeasType( const int table_no, const int col_id , int& col_prop );
	
	int ColMeasType( const std::vector<KEY_ID_STRU> vec_keyid , std::vector<int>& vec_col_prop );
	
public:	
	//
	int GraphGetAppInfo(const struct GRAPH_ORDER_KEY_REQ& graph_req, struct GRAPH_ORDER_KEY_RSP& graph_rsp);
	int GraphGetValue();
	
public:
	int GetAppNameByNo(char* app_name, const int app_no) { return 0;}
	int GetAppNoByName(int& app_no, const char* app_name) { return 0;}

	int GetTableNameByNo(char* table_name, const int table_no, const bool is_eng=true) { return 0;}
	int GetTableNoByName(int& table_no, const char* table_name, const bool is_eng=true) { return 0;}

	int GetFieldNameByNo(char* field_name, const int field_no, const bool is_eng=true) { return 0;}
	int GetFieldNoByName(int& field_no, const char* field_name, const bool is_eng=true) { return 0;}

public:
	int GetValueAndStatus(const std::vector<KEY_ID_STRU> &struVec , std::vector< struct INT_VALUE_STATUS >  &KeyIdValVec);  //ym

	int GetValueAndStatus(const std::vector<KEY_ID_STRU> &struVec , std::vector< struct FLOAT_VALUE_STATUS> & KeyIdValVec); //yc

	int GetValueAndStatus(const std::vector<KEY_ID_STRU> &struVec , std::vector< struct CHAR_VALUE_STATUS > & KeyIdValVec); //yx
	
	int GetValueAndStatus(const std::vector<KEY_ID_STRU> &struVec , std::vector< struct LONG_VALUE_STATUS > & KeyIdValVec); //long

	int GetValueAndStatus(const std::vector<KEY_ID_STRU> &struVec , std::vector< struct UNION_VALUE_STATUS> & KeyIdValVec  );//union type
	
	int GetValueAndStatus(const std::vector<struct KEY_ID_STA_STRU > &staStruVec , std::vector< struct INT_VALUE_STATUS> & KeyIdValVec);//ym
	
	int GetValueAndStatus(const std::vector<struct KEY_ID_STA_STRU > &staStruVec , std::vector< struct FLOAT_VALUE_STATUS> & KeyIdValVec);//yc
		
	int GetValueAndStatus(const std::vector<struct KEY_ID_STA_STRU> &staStruVec , std::vector< struct CHAR_VALUE_STATUS > & KeyIdValVec); //yx
	
	int GetValueAndStatus(const std::vector<struct KEY_ID_STA_STRU> &staStruVec , std::vector< struct LONG_VALUE_STATUS > & KeyIdValVec); //long
	
	int GetValueAndStatus(const std::vector<struct KEY_ID_STA_STRU> &staStruVec , std::vector< struct UNION_VALUE_STATUS> & KeyIdValVec  );//union type
	
	int GetFieldNoFromKeyId(const std::vector<KEY_ID_STRU> &struVec , std::vector<int> &FieldNoVec);
private:
		
	int GetFieldValueByNo(char** field_value_ptr, int& field_length, const char* record_ptr, const int field_no);
	int GetFieldValueByName(char** field_value_ptr, int& field_length, const char* record_ptr, const char* field_name);
	
	int GetRecordInfo(char** buf_ptr, short& field_num, int& record_num, int& record_size);
	int GetRecordInfoByKey(char** buf_ptr, short& field_num, int& record_num, int& record_size, const char* key_ptr);
	
	int GetMenuStringByValue(const char* menu_name, const int menu_val, const int menu_type, char*& menu_string, int& string_len);	
	
	int get_string(char* total_namestring, const char* read_record_ptr, const struct STDB_FIELD_TAB* mov_field_ptr);
	int GetNameString(std::string& total_namestring, const char* read_record_ptr, const struct STDB_FIELD_TAB* mov_field_ptr);

	int Start();
	int SetContext(const short context_no);
	
	int GetPosFromKeyIdStru(const std::vector<KEY_ID_STRU> &struVec , vector<int> &errPosVec , vector<IntVec> &posVec);
	
private:
	CApiOp(const CApiOp&);
	CApiOp& operator=(const CApiOp&);
	
private:
	enum
	{
		TABLE_COLUMN_INFO   = 2,
		TABLE_MENU_INFO     = 5,
		TABLE_RELEVANT_MENU = 6,
		TABLE_FAC_INFO      = 405,
	};

	short		m_ContextNo;
	int          	m_AppNo;
	int         	m_TableNo;
	
	CTableOp*    	m_TableOp;
	//CTableOp*	m_ColOp;
	
	TApiOpPriv	*m_pPrvStru;
};
}


#endif
