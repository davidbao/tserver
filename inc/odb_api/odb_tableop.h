/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  odb_tableop.h
DESCRIPTION:  class system struct define
FUNCTION LIST:
COMMENT:
History:                                                        
Date          Version     Modifier                Activities
1004-06-28    1.1                              add context_no parameter
2002-11-06    1.0                              modify
================================================================================
2002-10-30    1.0                              created
*******************************************************************************/
#ifndef	__ODB_TABLEOP_H__
#define	__ODB_TABLEOP_H__

#ifndef	__ODB_SYSTEM_H__
#include "odb_system.h"
#endif

#ifndef	__ODB_TABLE_H__
#include "odb_table.h"
#endif

#ifndef	__ODB_FIELD_H__
#include "odb_field.h"
#endif

#ifndef __ODB_WHERE_H__
#include "odb_where.h"
#endif

#ifndef	__ODB_TABLEBASE_H__
#include "odb_tablebase.h"
#endif

#include "odb_baseindex.h"

using namespace std;

namespace  ODB
{
	typedef struct   
	{
		int type;
	} TTableOpPriv;
	
//	enum DIRECTION
//	{
//		FORWARD = 0,
//		BACKWARD = 1 ,
//	};


class CTableOp: public CTableBase
{
public:
	//
	//constructor & destructor // 2002-11-06
	//
	CTableOp();   //then Open()
	CTableOp(const int app_no, const int table_no, const short context_no = 0, bool is_map = false);
	CTableOp(const int app_no, const char* table_name, const short context_no = 0, bool is_map = false);
	CTableOp(const char* app_name, const int table_no, const short context_no = 0, bool is_map = false);
	CTableOp(const char* app_name, const char* table_name, const short context_no = 0, bool is_map = false);
	CTableOp(const string& app_name, const string& table_name, const short context_no = 0, bool is_map = false);
	CTableOp(const int app_no, const short context_no = 0);
	//CTableOp(const char* context_name);    //abandon context_name // 2004-06-25

	~CTableOp();
	
	int SetAppNo(const int app_no, const short context_no = 0);
//	int Open(const std::pair<const int, const int>&, const short context_no = 0);
	int Open(std::pair<int, int>, short context_no = 0, bool is_map = false);
	int Open(const int app_no, const int table_no, const short context_no = 0, bool is_map = false);
	int Open(const char* app_name, const int table_no, const short context_no = 0, bool is_map = false);          //
	int Open(const int app_no, const char* table_name, const short context_no = 0, bool is_map = false);			//  2004-06-25
	int Open(const char* app_name, const char* table_name, const short context_no = 0, bool is_map = false); //
//	int Open(const int app_no, const char* table_name, const char* context_name = NULL);		 //abandon context_name // 2004-06-25
//	int Open(const char* app_name, const char* table_name, const char* context_name = NULL); //
		
	//create & delete
	int	TableCreate(const struct STDB_CREAT_REQ* stdb_creat_req_ptr, const struct FIELD_CREAT_REQ* multi_fields_ptr, 
		const std::vector<MEMBER_DEFINITION>& vec_offset, INDEX_DEFINE_ARRAY &index_define_array);  //创建、删除函数,包括索引	
	int	TableWrite(const char* buf_ptr, const int record_num, const int record_size, bool is_map = false);
	int	TableWrite(const char* buf_ptr, const int buf_size, bool is_map = false);		                      //write one or many records
	//int	TableDelete(const int app_no, const int table_no, const char* passwd_ptr);        //delete by table no
	//int	TableDelete(const int app_no, const char* table_name, const char* passwd_ptr);    //delete by table name

	int	TableClear(bool is_map = false);                                                                     //clear a table	

	//
	//query	the whole table
	//
	int	TableGet(CBuffer& buf_base);
	int	TableGet(const int field_no, CBuffer& buf_base);
	int	TableGet(const char* field_name, CBuffer& buf_base);
	int	TableGet(const std::vector<int>& vec_field_no, CBuffer& buf_base);

	//
	int	TableGet(char* buf_ptr, const int buf_size);                                     //get all record of a table
	
	int	TableGet(char** buf_ptr, int& buf_size);
	int	TableGet(const int field_no, char** field_buf_ptr, int& buf_size) ;         //get one field of all record by field no	
	int	TableGet(const char* field_name, char** field_buf_ptr, int& buf_size);    //get one or many fields of all record by field name //2002-11-20
	int	TableGet(const std::vector<int>& vec_field_no, char** field_buf_ptr, int& buf_size);//get one or many fields of all record
	
	//
	//query	by key word
	//
	int	TableGetByKey(const char* key_ptr, char* buf_ptr, const int buf_size); // get one record by key word
	
	int	TableGetByKey(const char* key_ptr, const int field_no, char* field_buf_ptr, const int buf_size); //get one field  of one record by field no & key word
	int	TableGetByKey(const char* key_ptr, const int field_no, char* field_buf_ptr, const int buf_size, int& data_type, int& data_len);

	int	TableGetByKey(const char* key_ptr, const char* field_name, char* field_buf_ptr, const int buf_size);               //get one or many fields by field name & key word
	int	TableGetByKey(const char* key_ptr, const std::vector<int>& vec_field_no, char* field_buf_ptr, const int buf_size); //get one or many fields by field no & key word
	
	int	TableGetByKey(const char* key_ptr, const int keybuf_size, const std::vector<int>& vec_field_no, char* field_buf_ptr, const int buf_size);  //many key words
	
public:
	int	TableGet(const std::vector<int>& vec_field_no, char** field_buf_ptr, int& buf_size, std::vector<MEMBER_DEFINITION>& vec_offset);
	int	TableGetByKey(const char* key_ptr, const std::vector<int>& vec_field_no, char* field_buf_ptr, const int buf_size, std::vector<MEMBER_DEFINITION>& vec_offset);
	int	TableGetByKey(const char* key_ptr, const int keybuf_size, const std::vector<int>& vec_field_no, char* field_buf_ptr, const int buf_size, std::vector<MEMBER_DEFINITION>& vec_offset);

//lmj add 2004.1125
	int	TableGetByKeyAligned(const char* key_ptr, const std::vector<int>& vec_field_no, char* field_buf_ptr, const int buf_size, std::vector<MEMBER_DEFINITION>& vec_offset);
	int	TableGetByKeyAligned(const char* key_ptr, const int keybuf_size, const std::vector<int>& vec_field_no, char* field_buf_ptr, const int buf_size, std::vector<MEMBER_DEFINITION>& vec_offset);




	//
	//the whole record //modify // 2002-11-12
	//
public:
	int	TableModify(const char* buf_ptr, const int record_num, const int record_size, bool is_map = false);  //modify one or many record
	int	TableModify(const char* buf_ptr, const int buf_size, bool is_map = false);

	//
	//many fields of all record
	//
	int	TableModify(const int field_no, const char* field_buf_ptr, const int buf_size, bool is_map = false); //modify one field of all record by field no
	int	TableModify(const char* field_name, const char* field_buf_ptr, const int buf_size, bool is_map = false);                   //modify one or many fields of all record by field name
	int	TableModify(const std::vector<int>& vec_field_no, const char* field_buf_ptr, const int buf_size, bool is_map = false);     //modify many fields of all record by field no
	
	//
	//by keyword && fields
	//
	int	TableModifyByKey(const char* key_ptr, const int field_no, const char* field_buf_ptr, const int buf_size, bool is_map = false);  //modify one field by key word and field no
	int	TableModifyByKey(const char* key_ptr, const char* field_name, const char* field_buf_ptr, const int buf_size, bool is_map = false);               //modify many fields by key word and field name
	int	TableModifyByKey(const char* key_ptr, const std::vector<int>& vec_field_no, const char* field_buf_ptr, const int buf_size, bool is_map = false); //modify many fields by field no
	int TableModifyByKeyMulFlds(const char* key_ptr, const int field_num, ...);
	
	//lmj add 2004.1125
	int	TableModifyByKeyAligned(const char* key_ptr, const std::vector<int>& vec_field_no, const char* field_buf_ptr, const int buf_size, std::vector<MEMBER_DEFINITION>& vec_offset, bool is_map = false); //modify many fields by field no
	
	//
	//by keywords && fields
	//
	int	TableModifyByKey(const char* key_ptr, const int keybuf_size, const int field_no, const char* field_buf_ptr, const int buf_size, bool is_map = false);                   //modify one field or all fields of many records by key word

	int	TableModifyByKey(const char* key_ptr, const int keybuf_size, const char* field_name, const char* field_buf_ptr, const int buf_size, bool is_map = false);               //modify many fields or all fields of many records by key word
	int	TableModifyByKey(const char* key_ptr, const int keybuf_size, const std::vector<int>& vec_field_no, const char* field_buf_ptr, const int buf_size, bool is_map = false); //the same as above
	
	//lmj add 2004.1125
	int	TableModifyByKeyAligned(const char* key_ptr, const int keybuf_size, const std::vector<int>& vec_field_no, const char* field_buf_ptr, const int buf_size, std::vector<MEMBER_DEFINITION>& vec_offset, bool is_map = false); //the same as above
	
	
	//
	//update & delete // 2002-11-12	
	//
	int	TableUpdate(const char* buf_ptr, const int buf_size, bool is_map = false); //update one or more record
	int	TableUpdate(const char* buf_ptr, const int record_num, const int record_size, bool is_map = false);
	
	int GetDefaultRecord(char *defaut_rec_ptr);
	
	int TableUpdate( const int field_no, const char* buf_ptr, const int buf_size, bool is_map = false);
	int TableUpdate( vector<int> field_vec, const char* buf_ptr, const int buf_size, bool is_map = false); //关键字域按顺序放在最前面
	int TableUpdate( const char* field_name, const char* buf_ptr, const int buf_size, bool is_map = false);  //关键字域按顺序放在最前面

	
	int	DeleteRecord(const char* key_ptr, bool is_map = false);                     //delete a record by key word

	int	DeleteRecords(const char* key_ptr, int key_num, bool is_map = false);
		//key_ptr是多个关键字组成的一个数组，key_num表示删除的个数。在做这个删除的时候，一方面要从后往前删，另外一方面要判断关键字是否连续，连续的多个关键字一把删除，而不是要分成几个部分删除。
	int	DeleteAlignedRecords(const char* key_ptr, int direction = FORWARD, bool is_map = false);
		//direction表示方向，FORWARD表示删除参数key_ptr关键字和它前面的所有记录（譬如如果key_ptr对应第9条记录，那么就删除从0～9的记录）。BACKWARD表示删除key_ptr和它后面的所有记录。

	
	//
	//query by condition  //aligned
	//
	int	ConGet(const int con_field_no, const int con_field_value, char* buf_ptr, const int buf_size);                        //one record by field(int)
	int	ConGet(const int con_field_no, const int con_field_value, char** buf_ptr, int& buf_size);                            //one record by field(int)
	int	ConGet(const int con_field_no, const char* con_field_value, char** buf_ptr, int& buf_size);                          //one record by field(string)
	int	ConGet(const int get_field_no, const int con_field_no, const int con_field_value, char* buf_ptr, const int buf_size);//one field by field(int)
	int	ConGet(const int get_field_no, const int con_field_no, const char* con_field_value, char** buf_ptr, int& buf_size);  //one field by field(string)

	int ConGet(const int con_field_no,  const long con_field_value,char **buf_ptr,int &buf_size);
	int ConGet(const int get_field_no,const int con_field_no,const long con_field_value,char **buf_ptr, int& buf_size);
	int ConGet(const std::vector<int>& con_field_no_array,const std::vector<int>& get_field_no_array,char *con_buf_ptr,char **buf_ptr,int &buf_size);
	
	int ConGetAreaTable(const int con_field_no, const char* con_field_value, char** buf_ptr, int& buf_size);
	int ConGetAreaTable(const int get_field_no, const int con_field_no, const char* con_field_value, char** buf_ptr, int& buf_size);
	int ConGetAreaTable(const std::vector<int>& con_field_no_array, const std::vector<int>& get_field_no_array, char* con_buf_ptr, char** buf_ptr, int& buf_size);
	
	int ConGetByMeasID(const int get_field_no, const char* con_field_value, char** buf_ptr, int& buf_size);
	int ConGetByMeasID(const char* con_field_value, char** buf_ptr, int& buf_size);
	int ConGetByMeasID(const std::vector<int>& get_field_no_array, char* con_field_value, char** buf_ptr, int& buf_size);
	//Added for new structure and new function for D-5000 end
	
	//
	//sort record by condition  //aligned
	//
	int	SortGet(const int field_no, char* bu_fptr, const int buf_size, const bool is_asc = true);                      //one field and sorted by it
	int	SortGet(const char* field_name, char* bu_fptr, const int buf_size, const bool is_asc = true);                  //many fields and sorted by them
	int	SortGet(const std::vector<int>& vec_field_no, char* buf_ptr, const int buf_size, const bool is_asc=true);      //many fields and sorted by them
	int	SortGet(char* buf_ptr, const int buf_size, const bool is_asc = true);                         //get key word & index field and sorted by index
	
	//
	//SQL interface
	//
	int SqlUpdate(const char* str_sql);
	int SqlGet(const char* str_sql, CBuffer& buf_base);
	int SqlGet(const char* str_sql, char** buf_ptr, int& buf_size);
	int SqlGet(const char* str_sql, char** buf_ptr, int& buf_size, std::vector<MEMBER_DEFINITION>& vec_offset);

public:	
	int	GetTablePara(short& field_num, int& record_num, int& record_size);
	int	GetTablePara(struct TABLE_PARA& table_para);
	int	GetTablePara(struct STDB_FIELD_TAB* &field_para_ptr,  int &buf_size);
	int	GetTablePara(struct TABLE_PARA& table_para, struct STDB_FIELD_TAB* &field_para_ptr, int &buf_size);
	int	GetTablePara(struct TABLE_PARA& table_para, std::vector<struct STDB_FIELD_TAB>& vec_field_para);
	//int	GetTablePara(std::vector<struct FIELD_PARA>& vec_field_para);              // 2003-05-21
	int	GetTablePara(const char* str_attribute, char** field_para_ptr, int& buf_size);
	int	GetTablePara(const char* str_attribute, char** field_para_ptr, int& buf_size, std::vector<MEMBER_DEFINITION>& vec_offset);  //by net daemon

//	int	GetRecordNum();
//	int GetKeyWordLength();

	//
	//field's parameter
	//
	int GetFieldEngNameByNo(char* field_name, const int field_no);
	int GetFieldChnNameByNo(char* field_name, const int field_no);
	int GetFieldNameByNo(char* field_name, const int field_no, const bool is_eng=true);
	
	int GetFieldNoByName(int& field_no, const char* field_name, const bool is_eng=true); //only one
	int GetFieldNoByName(std::vector<int>& vec_field_no, const char* field_name);        //many fields
	
	int SetFieldNameByNo(const char* field_name, const int field_no);
	
	int GetFieldNoBySql(std::vector<int>& vec_field_no, const char* str_sql);
	//int GetRtNoByFieldNo(int& field_no, const int r_field_no);
	
	//
	//table's parameter
	//
	int GetTableNameByNo(char* table_name, const int table_no, const bool is_eng=true);
	int GetTableNoByName(int& table_no, const char* table_name, const bool is_eng=true);
	int GetTableNoByName(const char* table_name);

	//
	//application's parameter
	//
	int GetAppNameByNo(char* app_name, const int app_no);
	int GetAppNoByName(int& app_no, const char* app_name);
	int GetAppNoByName(const char* app_name);

	//int GetAllOdbNoByAppName(short& app_no, std::vector<NAME_ID>& table, const char* app_name);  // 2002-12-02
	int GetAllOdbNoByAppName(int& app_no, std::vector<int>& table, const char* app_name);  // 2002-12-02
	int	GetAllOdbNoByAppNo(char* app_name, std::vector<NAME_ID>& vec_table, const int app_no, const bool is_eng = true);
	int GetAllAppNo(std::vector<struct NAME_ID>& vec_appno);
	int WriteTableStatus(DB_STATUS_T status);
	int GetTableStatus(DB_STATUS_T &status);
	
	int SetSemKey(const int app_no, const int r_table_no, const short ctx_no); //for OPTLOCK

public:
	struct STDB_TAB* GetCurStdbTab();                     // 2003-01-18
	struct STDB_FIELD_TAB* GetCurStdbFieldTab();          // 2003-03-03
	
	const struct STDB_TAB* GetStdbTab();                     // 2003-12-18
	const struct STDB_FIELD_TAB* GetStdbFieldTab();          // 2003-12-03
	const struct STDB_TAB* GetStdbTab(const int area_no);
	
	int GetFieldPara(struct FIELD_BASE_INFO& field_info);
	int GetFieldParaByNo(std::vector<MEMBER_DEFINITION>& vec_offset, const std::vector<int>& vec_field_no);
	int GetFieldInfo(const std::vector<int>  vec_field_no, std::vector<struct FIELD_BASE_INFO>& vec_field);
	
	//exchange byte order for buffer data
	int ExchangeData(char* buf_ptr, const int buf_size, const std::vector<int>& vec_field, const DB_BYTE_T& remote_byte);
	int ExchangeKeyData(char* buf_ptr, const int buf_size, const DB_BYTE_T& remote_byte);
	
	int GetDbID(int& db_id);
	int SetDbID(const int db_id);
	
public:
	int AddApp(const char *all_name, const short context_no );
	int AddOneTable(const int r_table_no, const char* table_name_eng);
	int AddContext( const short context_no );

private:
	//sql
	int GetFieldNames(const char* str_sql, std::vector<NAME_ID>& vec_field);  //from select clause, call GetFieldNameBySelect()	
	
	bool GetFieldNameBySelect(const char* str_field, std::vector<NAME_ID>& vec_field) const;
	bool GetFieldNameByWhere(const char* str_condition, std::vector<NAME_ID>& vec_field) const;
	bool GetFieldNameByOrder(const char* str_orderby, std::vector<SQL_SORT>& vec_field_sort) const;
	
	int  GetFieldWhere(std::vector<FIELD_STRU>& vec_field_sql, const std::vector<NAME_ID>& vec_field_where);
	int  destory_field_where( std::vector<FIELD_STRU>& vec_field_sql );
	
	//
	//sort, only used by itself
	//
	int	SortGet(std::vector<NAME_ID>& vec_select, char** buf_ptr, int& buf_size, const std::vector<bool>& vec_where, std::vector<SQL_SORT>& vec_order);
	int	SortGet(std::vector<NAME_ID>& vec_select, char** buf_ptr, int& buf_size, const std::vector<bool>& vec_where, std::vector<SQL_SORT>& vec_order, std::vector<MEMBER_DEFINITION>& vec_offset);
	
	int GetSortedKey( std::vector<RCD_TYPE>& vec_sort,
	                  const char* all_record_ptr,
	                  const std::vector<int>& vec_field_no,
	                  const struct STDB_TAB* stdb_tab_ptr,
	                  const struct STDB_FIELD_TAB* stdb_field_tab_ptr);
	                  
	bool  ParseLike(const char* str_condition, std::vector<NAME_ID>& vec_field) const;
	char* CheckLike(const char* str_condition, const char* str_like) const;
	
	int  SortGetForSql(std::vector<int>& SortVecNo , char * orgBufPtr , char * & bufPtr , int recNum, int is_asc);

private:
	int Start();

	int MoveTo(const char* app_name, const int table_no);
	int MoveTo(const int table_no);
	int MoveTo(const char* table_name);

	//delete
	//int TableDelete(struct STDB_DELETE_REQ* stdb_delete_req_ptr);

/*********Added for index begin *****************/	
public:
	INDEX_DEFINE_TAB* GetIdxDfnPtr();
	FIELD_INDEX_DEFINE_TAB* GetFldIdxDfnPtr();
	FIELDS_IN_INDEXS_DESC_TAB* GetFldsInIdxDescPtr();
	FIELDS_USED_IN_INDEX_TAB* GetFldsUsedInIdxPtr();
	CBaseIndex *GetIndexPtr(int seq_no);//返回idx_op_ptr_array的第seq_no个元素，如果第seq_no个元素是NULL的话，new一个元素出来。
	CBaseIndex *GetPKIndexPtr();//如果pk_index_ptr不等于NULL，就返回一个pk_index_ptr；否则new一个新的返回。
public:
	int DebugDisp(int index_no,int type);// 显示第index_no个索引的信息，譬如index_no为0、1、......，type为信息的详细程度标志。
	int TableDelete();
/*********Added for index end  ******************/	
public: //These functions are used only for debug
	int SprintKey(char *key_ptr,char *key_string);
	//int Search(char *key_ptr,int &record_pointer);
private:
	CTableOp(const CTableOp&);
	CTableOp& operator=(const CTableOp&);
	
	TTableOpPriv  *m_pPrvStru;
	
	bool  m_IsMap;

public:
	COdbSystem* GetOdbSystemPtr();
	COdbTable*  GetOdbTablePtr();
	COdbField*  GetOdbFieldPtr();
	
public:
	const struct DB_CTRL_TAB* GetDbCtrlTabPtr();
	DB_APP_TAB*  GetAppTabPtr();
	char*  GetTableFileName();
	
private:
	void SetAppTabPtr(DB_APP_TAB* app_tab_ptr);
	
protected:
	COdbSystem* m_OdbSystemPtr;
	COdbTable*  m_OdbTablePtr;
	COdbField*  m_OdbFieldPtr;
};

}

#endif
