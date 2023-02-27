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
	virtual int Open(char *index_file_name,int table_id,int index_id)=0; //table_id��index_id������INDEX_STDB_TAB::table_id��INDEX_STDB_TAB::field_idƥ��У��
	virtual int Open(int index_shmid,int table_id,int index_id) = 0; //table_id��index_id������INDEX_STDB_TAB::table_id��INDEX_STDB_TAB::field_idƥ��У��
	virtual int SetIndexPtr(char *index_ptr)= 0; //�ѻ����ָ�븳ֵ�������࣬�����Ͳ���ʹ��ǰ���Open������
	virtual int GetKeyByIndex(char *index_ptr,std::vector<char *> &index_array)=0;//���Ƕ���ؼ��ֿ���������ֵ��ͬ�������
	virtual int GetKeyPointerByIndex(char *index_ptr, int& pointer)=0;//ֻ������������Ч������index�õ���¼������λ��
	virtual int IndexInsert(char *index_ptr,char *key_ptr)=0;
	virtual int BulkIndexInsert(char *bulk_index_key_ptr)=0;//��װ��ʱ��,����˳�������϶������õġ��൱���������ⲿ�ź���Ȼ��һ�Ѱ����е�����ȫ�������ȥ��
	virtual int Indexdelete(char *index_ptr,char *key_ptr)=0;
	virtual int IndexClear()=0;
	virtual int GetIdxMthd()=0;
	virtual int FreeResource()=0;//Ʃ��unmap����shmdt��ָ�롣
	virtual int DebugDisp(int type)=0;//��������ã���ͬ��type����ʾ��ͬ������ϸ�̶Ȳ�ͬ����Ϣ��
	virtual int GetIndexRecNum() = 0;//����������¼�ܵĸ���������������������������������Ч��¼�������ܺ͡����������HASH����ֻ�Ƿ���0��
	virtual DB_STATUS_T GetIndexStatus()=0;//����������״̬
	virtual void SetIndexStatus(DB_STATUS_T idx_status)=0;//��������ͷ�ϵ�����״̬
	virtual ~CBaseIndex(){};
};
}

#endif
