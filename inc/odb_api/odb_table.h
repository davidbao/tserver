/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  odb_table.h
DESCRIPTION:  class table struct define
FUNCTION LIST:
COMMENT:
History:                                                        
Date          Version     Modifier                Activities
2002-09-01    1.0                              modify
================================================================================
2002-09-01    1.0                              created
*******************************************************************************/

#ifndef	__ODB_TABLE_H__
#define	__ODB_TABLE_H__

//#include "odb_struct.h"
#include "odb_prv_struct.h"
#include "odb_public.h"
#include "odb_common.h"
#include "odb_baseindex.h"
#include "DeleteRecorder.h"


using namespace std;
struct FIX_INDEX_DEF;
class CRWLock;

//Test Case
class CaseSearch;
class CaseTableWrite;
class CaseDeleteIndex;

#define MAX_AREA_IDX_NUM 10

namespace ODB
{
	class Tab2Idx;
	
	enum DIRECTION
	{
		FORWARD = 0,
		BACKWARD = 1 ,
	};

	
class COdbTable
{
public:
	COdbTable();
	COdbTable(struct DB_APP_TAB* app_tab_ptr, struct DB_CTRL_TAB* ctrl_tab_ptr);
	//COdbTable(struct DB_APP_TAB* app_tab_ptr, struct DB_CTRL_TAB* ctrl_tab_ptr, char* odb_file_name);
	//COdbTable(struct DB_APP_TAB* app_tab_ptr, struct DB_CTRL_TAB* ctrl_tab_ptr, int odb_no);
	//COdbTable(struct DB_APP_TAB* app_tab_ptr, struct DB_CTRL_TAB* ctrl_tab_ptr, char* odb_name);	
	COdbTable(struct DB_APP_TAB* app_tab_ptr, int odb_no);
	COdbTable(struct DB_APP_TAB* app_tab_ptr, char* odb_name);
	
	COdbTable(char* app_ptr);
	~COdbTable();
	
	//set environment after default constructor
	int  SetAppCtrl(struct DB_APP_TAB* app_tab_ptr, struct DB_CTRL_TAB* ctrl_tab_ptr = NULL);
		
	//
	//create & delete
	//
	int  CreateTable(const struct STDB_CREAT_REQ* odb_creat_ptr, const struct FIELD_CREAT_REQ* odb_field_descrb_ptr, const std::vector<MEMBER_DEFINITION>& vec_offset,INDEX_DEFINE_ARRAY &index_define_array);
	int  DeleteTable(/*struct STDB_DELETE_REQ* stdb_delete_req_ptr*/);
	int  AddOneTable(const int r_table_no, const char* table_name_eng);
	//int DeleteTable();
	
	int  CreateAreaTable(const struct STDB_CREAT_REQ* odb_creat_ptr, const struct FIELD_CREAT_REQ* odb_field_descrb_ptr, const std::vector<MEMBER_DEFINITION>& vec_offset,INDEX_DEFINE_ARRAY &index_define_array);
	int  DeleteAreaTable();
	
	int  ClearTable();
	int  ClearAreaTable();
	int  ClearAreaTableHoleDirect();
	
	//write record
	int	 WriteRecords(const char* buf_ptr, const int buf_size);
	
	//
	//operation by key word
	//
	int  ReadRecordByKey(const char* keyword, char** record_data_ptr, int& record_data_size);
	int  WriteRecordByKey(const char* keyword, const char* write_record_ptr, const int write_record_size);
	int  DeleteRecordByKey(const char* keyword);
	int  UpdateRecordByKey(const char* keyword, const char* update_record_ptr, const int update_record_size);
	int  ModifyRecordByKey(const char* keyword, const char* modify_record_ptr, const int modify_record_size);
	
	int DeleteAlignedRecords(const char* keyword, int direction = FORWARD);
	int DeleteRecords(const char* keyword, const int key_num);
	
	int DeleteAreaTableRecords(const char* keyword, const int key_num);

	//
	//query the whole table //2002-11-07
	//
	//int	 GetAllRecords(char* buf_ptr, int& buf_size);
	int	 GetAllRecords(char* buf_ptr, const int buf_size);
	int	 GetAreaTableAllRecords(char* buf_ptr, const int buf_size);

	//
	//move table among application
	//
	int  MoveTo(const int r_table_no);
	int  MoveTo(const char* table_name);
	
	//
	//move record each other
	//
	void RecordFirst();
	int  RecordPre();
	int  RecordNext();
	int  RecordTo(const int no);
	int  RecordTo(const char* key_ptr);
	void RecordLast();
		
	//
	//parameter //2002-10-16
	//
	struct DB_CTRL_TAB*	    GetCtrlTabPtr();
	struct STDB_TAB*        GetStdbTabPtr();
	struct STDB_TAB*        GetStdbTabPtr(const int area_no);
	struct STDB_FIELD_TAB*	GetFieldTabPtr();
	char*  GetDbPtr();
	char*  GetCurRecordPtr();
	int    GetKeyWordLength();
	int    GetRecordNum();
	int    GetPara(short& field_num, int& record_num, int& record_size);
	int    GetPara(struct TABLE_PARA& table_para);

	//
	//physical file name
	//
	int   GetFileName(const char* table_name_eng);
	int   GetWholeMapFileName(char* str_file_name);
	int   GetFileNameWithAreaName(const char* table_name_eng, const char* area_name);

	char* GetTableName();
	int   GetTableName(char* table_name);
	int   GetTableNo(bool is_real = false);
	
	//wj added for area tables
	int SetAreaEnv(const int area_no, const bool is_manage = false);
	int CleanEnv();
	
	DB_APP_TAB*  GetAppTabPtr();
	STDB_TAB*    GetAreaStdbTabPtr();
	time_t       GetDbOpenTime(const int table_no);
	char*  GetTableFileName();
	
	void   SetAppTabPtr(DB_APP_TAB* app_tab_ptr);
	
private:
	//
	//Access environment //2002-10-15
	//
	int SetDefaultEnv();
	int SetAppEnv(char* app_file_ptr);
	int SetTableEnv(const struct DB_CTRL_TAB* const init_ctrl_tab_ptr);
	//int CleanEnv();
	


	//
	//lock 2002-10-28
	//
	bool  Lock(const DB_LOCK_T& lock_type);
	bool  UnLock(const DB_LOCK_T& lock_type);
	
	//
	//stat 2002-10-28
	//
	int	 MinusDbNumTab();
	int	 AddDbNumTab();
	int	 SetStaticsParaPtr(int tag, int op_time);
	
	//
	//Operation to Create // 2002-10-10
	//
	int fill_stdb_tab(const struct STDB_CREAT_REQ* stdb_creat_req_ptr);
	//int fill_field_tab(const struct STDB_CREAT_REQ* stdb_creat_req_ptr, const struct STDB_FIELD_DESCRIBLE* stdb_field_describle_ptr);
	int fill_field_tab(const struct FIELD_CREAT_REQ* stdb_field_describle_ptr, const int field_num, const std::vector<MEMBER_DEFINITION>& vec_offset);
	int get_keyword();
	int fill_sequent_tab(const struct STDB_CREAT_REQ* stdb_creat_req_ptr, int& stdb_describ_area_size, int& stab_total_size);
	int fill_db_ctrl_tab(const struct STDB_CREAT_REQ* stdb_creat_req_ptr,int& stdb_describ_area_size);
	
	//
	int search(const char*keyword,int &record_pointer);
	int    binary_search(const char* keyword, int& record_pointer,bool ext_flag=false);//ext_flag means the search area is in overflow table space in the hole direct table
	struct RECORD_HEAD_STRU* get_record_head_ptr(const int record_pointer);
	char*  get_record_ptr(const int record_pointer);
	char*  get_record_ptr(const int record_pointer, int& record_size );
	int compare_keyword(const char* keyword1_ptr, const char* keyword2_ptr,  const int keyword_len);
	
	//
	//memory data
	//
	int move_record(int targ_reccord_pointer,int sour_record_pointer, const int move_num);
	static int mem_move(char* targ_ptr, char* sour_ptr, const int count);
	int	assign_position(int* position_ptr, int  assign_val);
	
	//
	//Operation to a record
	//
	int read_record(const int record_pointer, char** record_data_ptr, int& record_data_size);                //locked by caller
	int write_record(const int record_pointer, const char* write_record_ptr, const int write_record_size);   //locked by caller
	int delete_record(const int record_pointer);  							                                 //locked
	int modify_record(const int record_pointer, const char* modify_record_ptr, const int modify_record_size,CRWLock &theLock);//locked
	
	//
	//Operation to Record Head //2002-10-15
	//
	int set_record_head(char* record_ptr, const int exist_tag);
	int get_record_head(char* record_ptr);
	int need_rebuilt(char * table_file_ptr, int shm_size,int table_size);

public:
	int sprint_key(char *key_ptr,char *key_string);
	int sprint_field(const int data_type, const char* field_data_ptr,char *field_string);
	
private:
	COdbTable(const COdbTable& odb_table);
	COdbTable& operator=(const COdbTable& odb_table);

/***********Added for index begin ****************/
public:
	INDEX_DEFINE_TAB* GetIdxDfnPtr();
	FIELD_INDEX_DEFINE_TAB* GetFldIdxDfnPtr();
	FIELDS_IN_INDEXS_DESC_TAB* GetFldsInIdxDescPtr();
	FIELDS_USED_IN_INDEX_TAB* GetFldsUsedInIdxPtr();
	CBaseIndex *GetIndexPtr(int seq_no);//返回idx_op_ptr_array的第seq_no个元素，如果第seq_no个元素是NULL的话，new一个元素出来。
	CBaseIndex *GetPKIndexPtr();//如果pk_index_ptr不等于NULL，就返回一个pk_index_ptr；否则new一个新的返回。
	int fill_index_tab(INDEX_DEFINE_ARRAY& seq_idx, std::vector<short>& vec_index_fieldno);
	int DebugDisp(int index_seq_no,int type);
	int GetAppFile(char *file_name,const struct DB_APP_TAB*app_ptr);
private:
	INDEX_DEFINE_TAB *m_IdxDefPtr;
	FIELDS_IN_INDEXS_DESC_TAB *m_FieldsInIdxDescPtr;
	FIELD_INDEX_DEFINE_TAB *m_FieldIdxDefPtr;
	FIELDS_USED_IN_INDEX_TAB *m_FieldsUsedInIndexPtr;
	std::vector<CBaseIndex *> m_IdxOpPtrArray;//打开一张表的时候，就先给idx_op_ptr_array分配对应的空间。只有真正使用到这个索引的时候，再构造里面具体的CBaseIndex。在切换到另外一张表的时候，要删除每一个CBaseIndex* ，然后再重新分配idx_op_ptr_array。
	//CNoOverflowPK* direct_pk_ptr;//打开表的时候就要构造好direct_pk_ptr和hash_pk_ptr，切换表的时候，要unmap或者detach原来里面的指针，然后mmap或者attach里面的指针。
	//CHashPK *hash_pk_ptr;		
	CBaseIndex *m_PkIdxPtr;//考虑到扩展性，还是用这样一个指针好一点。
	
	//CBaseIndex* m_AreaIdxPtr[MAX_AREA_NUM][MAX_AREA_IDX_NUM]; //only for area tables' indexes
	
public:
	int  FormIdxErrReport(int idx_seq_no,struct FIX_INDEX_DEF &fix_idx_def );//idx_seq_no = -1 means primary key index
	
/***********Added for index end ****************/

/*********Added for imcompact table begin ********/
private:
	int *GetHoleIdxPtr();
	int HoleIdxInsert(int record_pointer);
	int HoleIdxDelete(int record_pointer);
	int HoleIdxDelete(int del_pointer_start, int del_pointer_end);
	int m_CurHolePKPointer;//为带洞表的RecordFirst、RecordNext、RecordLast函数、RecordPre函数几个函数服务的。
	
	char* m_ExtDataAreaPtr;	//溢出表体数据区的指针
	int *m_HoleIdxPtr; //带洞表主键索引指针。
	int  hole_direct_search (const char* in_keyword, int &record_pointer);
	
	int  hole_idx_search (const int record_pointer, int& real_pos);//根据record_pointer得到m_HoleIdxPtr下标
	int  CheckDownloadTime();
	int  ClearTableHoleDirect();
	int  WriteRecordByKeyHoleDirect(const char* keyword, const char* write_record_ptr, const int write_record_size);
	int  UpdateRecordByKeyHoleDirect(const char* keyword, const char* update_record_ptr, const int update_record_size);
	int  DeleteRecordByKeyHoleDirect(const char* keyword);
	int  DeletePK(vector<int>& vec_del_start, vector<int>& vec_del_end);
	int  DeleteHolePK(vector<int>& vec_del_start, vector<int>& vec_del_end);
	int  DeleteAlignedRecordsHoleDirect(const char* keyword, int direction = FORWARD);

/*********Added for imcompact table end   ********/

public:
	int& GetSemId(const int table_no);   //for OPTLOCK   jinjing 20120428
	void SetSemId(const int table_no, const int sem_id);   //for OPTLOCK   jinjing 20120428
	int SetSemKey(const int app_no, const int r_table_no, const short ctx_no); //for OPTLOCK
	
private:
	struct DB_APP_TAB*      m_AppTabPtr;        //App info area
	struct DB_SYSTEM_TAB*	m_SysTabPtr;
	struct DB_STATICS_TAB*  m_StaticsTabPtr;
	struct DB_CTRL_TAB*     m_CtrlTabPtr;       //Ctrl info area
		
	struct DB_CTRL_TAB*     m_CurCtrlTabPtr;    //current table's Ctrl info
	struct STDB_TAB*        m_StdbTabPtr;       //
	struct STDB_TAB*        m_StdbRootTabPtr;   //only for area table's root
	struct SEQUENT_TAB*     m_SequentTabPtr;    //
	struct STDB_FIELD_TAB*	m_StdbFieldTabPtr;  //

	//std::map<int, char*> m_DbFilePtr;
	char* m_DbFilePtr[MAX_APP_TABLE];
	char* m_AreaFilePtr[MAX_APP_TABLE][MAX_AREA_NUM];                        //only for area table's root
	time_t m_DbOpenTime[MAX_APP_TABLE];			//for re-open tables when download
	int  m_MapSize[MAX_APP_TABLE];				//for re-open tables when download
	int  m_SemId[MAX_APP_TABLE+1];                //for OPTLOCK
	int  m_CurTableNo;                          //for OPTLOCK
	char* m_SequentAreaPtr;                     //current sequent area
	char* m_DataAreaPtr;                        //current data area
	
	int   m_CurRecordPointer;                   //current record position
	int	  m_CurAreaNo;
	int   m_LastAreaNo;                         //only for GetPKIdxPtr
	int   m_LastIdxAreaNo[MAX_AREA_IDX_NUM];    //only for GetIndexPtr
	char  m_FileName[120];                      //current table's physical file name
	char  m_RootFileName[120];                  //only for area table's root
	
	//for Recreate Index Shm
	friend class Tab2Idx;
	
	//Only for Test Case
	friend class ::CaseSearch;
	friend class ::CaseTableWrite;
	friend class ::CaseDeleteIndex;
};

}

#endif
