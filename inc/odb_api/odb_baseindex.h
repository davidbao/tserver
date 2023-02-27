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
//#include <vector>
#include "odb_prv_struct.h"
#ifndef _CBASEINDEX
#define _CBASEINDEX

namespace ODB
{
class  CBaseIndex
{
public:
	virtual int need_rebuilt(char *existed_table_ptr,struct STDB_TAB *stdb_tab_ptr,struct STDB_FIELD_TAB *stdb_field_tab_ptr,INDEX_DEFINE &index_define,int &shm_id) =0;
	virtual int CreateIndex(struct STDB_TAB *stdb_tab_ptr,struct STDB_FIELD_TAB *stdb_field_tab_ptr,INDEX_DEFINE &index_define,int &shm_id,int &shm_size) =0;
	virtual int Open(char *index_file_name,int table_id,int index_id)=0; //table_id、index_id用来和INDEX_STDB_TAB::table_id、INDEX_STDB_TAB::field_id匹配校验
	virtual int Open(int index_shmid,int table_id,int index_id) = 0; //table_id、index_id用来和INDEX_STDB_TAB::table_id、INDEX_STDB_TAB::field_id匹配校验
	virtual int SetIndexPtr(char *index_ptr)= 0; //把缓存的指针赋值给索引类，这样就不用使用前面的Open函数。
	virtual int GetKeyByIndex(char *index_ptr,std::vector<char *> &index_array)=0;//考虑多个关键字可能索引的值相同的情况。
	virtual int GetKeyPointerByIndex(char *index_ptr, int& pointer)=0;//只对主键索引有效，根据index得到记录的物理位置
	virtual int IndexInsert(char *index_ptr,char *key_ptr)=0;
	virtual int BulkIndexInsert(char *bulk_index_key_ptr)=0;//下装的时候,对于顺序索引肯定是有用的。相当于在索引外部排好序，然后一把把所有的索引全部插入进去。
	virtual int Indexdelete(char *index_ptr,char *key_ptr)=0;
	virtual int IndexClear()=0;
	virtual int GetIdxMthd()=0;
	virtual int FreeResource()=0;//譬如unmap或者shmdt掉指针。
	virtual int DebugDisp(int type)=0;//方便调试用，不同的type，显示不同或者详细程度不同的信息。
	virtual int GetIndexRecNum() = 0;//返回索引记录总的个数，包括索引区、溢出区、主溢出区有效记录个数的总和。但是无溢出HASH索引只是返回0。
	virtual DB_STATUS_T GetIndexStatus()=0;//返回索引的状态
	virtual void SetIndexStatus(DB_STATUS_T idx_status)=0;//设置索引头上的索引状态
	virtual ~CBaseIndex(){};
};
}

#endif
