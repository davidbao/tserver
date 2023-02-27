/*******************************************************************************
ModuleName:   Real Time Database management system
FileName:	  rtdb_api.cpp
DESCRIPTION:  rtdb api interfaces implementation
FUNCTION LIST:
COMMENT:
History:                                                        
Date          Version     Modifier                Activities
2009-01-10    1.1                              add net interfaces
================================================================================
2008-10-30    1.0                              created
*******************************************************************************/
#include "odb_baseindex.h"
#ifndef _CNOOVERFLOWPK
#define _CNOOVERFLOWPK

namespace ODB
{

class CNoOverflowPK:public CBaseIndex
{
public:
	virtual int need_rebuilt(char *existed_table_ptr,struct STDB_TAB *stdb_tab_ptr,struct STDB_FIELD_TAB *stdb_field_tab_ptr,INDEX_DEFINE &index_define,int &shm_id);
	virtual int CreateIndex(struct STDB_TAB *stdb_tab_ptr,struct STDB_FIELD_TAB *stdb_field_tab_ptr,INDEX_DEFINE &index_define,int &shm_id,int &shm_size);
	virtual int Open(char *index_file_name,int table_id,int index_id); //table_id、index_id用来和INDEX_STDB_TAB::table_id、INDEX_STDB_TAB::field_id匹配校验
	virtual int Open(int index_shmid,int table_id,int index_id); //table_id、index_id用来和INDEX_STDB_TAB::table_id、INDEX_STDB_TAB::field_id匹配校验
	virtual int SetIndexPtr(char *index_ptr); //把缓存的指针赋值给索引类，这样就不用使用前面的Open函数。
	virtual int GetKeyByIndex(char *index_ptr,std::vector<char *> &index_array);
	virtual int GetKeyPointerByIndex(char *index_ptr, int& pointer);//只对主键索引有效，根据index得到记录的物理位置
	virtual int IndexInsert(char *index_ptr,char *key_ptr);
	virtual int BulkIndexInsert(char *bulk_index_key_ptr);//下装的时候
	virtual int Indexdelete(char *index_ptr,char *key_ptr);
	virtual int IndexClear();		
	virtual int GetIdxMthd();
	virtual int FreeResource();//譬如unmap或者shmdt掉指针。
	virtual int DebugDisp(int type);//方便调试用，不同的type，显示不同或者详细程度不同的信息。
	virtual int GetIndexRecNum();//返回索引记录总的个数，包括索引区、溢出区、主溢出区有效记录个数的总和。但是无溢出HASH索引只是返回0。
	virtual DB_STATUS_T GetIndexStatus()//返回索引的状态
	{
		return m_IndexStdbPtr->index_status;
	}
	virtual void SetIndexStatus(DB_STATUS_T idx_status)//设置索引头上的索引状态
	{
		m_IndexStdbPtr->index_status = idx_status;
	}

	//virtual int ModifyIndex(char *pk_ptr, int pos);
	CNoOverflowPK();
	~CNoOverflowPK();//考虑在析构函数里面调用FreeResource()
	
public:
	int GetMaxIndexPointer();
	int SetTableEnv(struct STDB_TAB *stdb_tab_ptr,char *table_data_area_ptr); 
	
private:
	struct INDEX_STDB_TAB* m_IndexStdbPtr;
	struct STDB_TAB *m_StdbTabPtr; //added for optimization of IndexDelete
	char *m_TableDataAreaPtr; //added for optimization of IndexDelete
	//struct INDEX_STDB_FIELD_TAB *m_IndexStdbFieldPtr;	
	int* m_PkIndexStgDirectPtr;
	int (*hash_func_ptr)(char *index,int length,int hash_prime);//这种索引算法只有一种，不一定使用
};

}
#endif

