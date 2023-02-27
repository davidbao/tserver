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
#ifndef _CHASHPK
#define _CHASHPK

namespace ODB
{
class CHashPK:public CBaseIndex
{
public:
	virtual int need_rebuilt(char *existed_table_ptr,struct STDB_TAB *stdb_tab_ptr,struct STDB_FIELD_TAB *stdb_field_tab_ptr,INDEX_DEFINE &index_define,int &shm_id);
	virtual int CreateIndex(struct STDB_TAB *stdb_tab_ptr,struct STDB_FIELD_TAB *stdb_field_tab_ptr,INDEX_DEFINE &index_define,int &shm_id,int &shm_size);
	virtual int Open(char *index_file_name,int table_id,int index_id); //table_id��index_id������INDEX_STDB_TAB::table_id��INDEX_STDB_TAB::field_idƥ��У��
	virtual int Open(int index_shmid,int table_id,int index_id); //table_id��index_id������INDEX_STDB_TAB::table_id��INDEX_STDB_TAB::field_idƥ��У��
	virtual int SetIndexPtr(char *index_ptr); //�ѻ����ָ�븳ֵ�������࣬�����Ͳ���ʹ��ǰ���Open������
	virtual int GetKeyByIndex(char *index_ptr,std::vector<char *> &index_array);
	virtual int GetKeyPointerByIndex(char *index_ptr, int& pointer);//ֻ������������Ч������index�õ���¼������λ��
	virtual int IndexInsert(char *index_ptr,char *key_ptr);
	virtual int BulkIndexInsert(char *bulk_index_key_ptr);//��װ��ʱ��
	virtual int Indexdelete(char *index_ptr,char *key_ptr);
	virtual int IndexClear();		
	virtual int GetIdxMthd();
	virtual int FreeResource();//Ʃ��unmap����shmdt��ָ�롣
	virtual int DebugDisp(int type);//��������ã���ͬ��type����ʾ��ͬ������ϸ�̶Ȳ�ͬ����Ϣ��
	virtual int GetIndexRecNum();//����������¼�ܵĸ���������������������������������Ч��¼�������ܺ͡����������HASH����ֻ�Ƿ���0��
	virtual DB_STATUS_T GetIndexStatus()//����������״̬
	{
		return m_IndexStdbPtr->index_status;
	}
	
	virtual void SetIndexStatus(DB_STATUS_T idx_status)//��������ͷ�ϵ�����״̬
	{
		m_IndexStdbPtr->index_status = idx_status;
	}
	
	//virtual int ModifyIndex(char *pk_ptr, int pos);
	~CHashPK();//���������������������FreeResource()
private:	
	struct INDEX_STDB_TAB *m_IndexStdbPtr;
	struct INDEX_STDB_FIELD_TAB *m_IndexStdbFieldPtr;	
	struct STDB_INDEX_HASH *m_StdbIndexHashPtr;
	int *m_FreePointer;//�����malloc_chunk��free_chunkʹ�á�
	char *m_IndexStgHashPtr;
	char *m_OverflowStgHashPtr;	
	int (*hash_func_ptr)(char *index,int length,int hash_prime);
	int index_equal(char *outer_index_ptr,char *inner_index_ptr);
	int malloc_chunk(int &new_record_pointer);
	int free_chunk(struct INDEX_STORAGE_HASH_REC_HEAD *root_index_stg_head_ptr,int unused_record_pointer);
	int assign_index(char *dest_index_ptr,char *sour_index_ptr);
	int disp_index_stdb_tab();
	int disp_index_stdb_field_tab();
	int disp_stdb_index_hash();
	int disp_free_pointer();
	int disp_index_stg();
	int disp_index_overflow();
	int disp_all();
	int disp_index_contents(char *index_ptr);
	int disp_key_contents(char *key_ptr);
};
}
#endif

