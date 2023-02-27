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
#include "odb_ckeymanager.h"
#ifndef _CHASHINDEX
#define _CHASHINDEX

namespace ODB
{
class CHashIndex:public CBaseIndex
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
	virtual int Indexdelete(char *index_ptr, char* key);
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
	
	~CHashIndex();//���������������������FreeResource()
	
	char * next2Ptr(int next);
	int freeChunk(int pointer);
	char* mallocChunk(int & pos);
	
	int keycmp(char * key1 , char * key2);//{return 0;}
	
	int index_equal(char * outer , char * innter);
	
	int DebugDisp(int type , char * ptr);
	
private:
	struct INDEX_STDB_TAB *m_IndexStdbPtr;
	struct INDEX_STDB_FIELD_TAB *m_IndexStdbFieldPtr;
	struct STDB_INDEX_HASH	 *m_StdbIndexHashPtr;
	int *m_FreePointer;//�����malloc_chunk��free_chunkʹ�á�
	char *m_IndexStgHashPtr;
	char *m_OverflowStgHashPtr;
	int (*hash_func_ptr)(char *index,int length,int hash_prime);
	
	int idxLenFlag ;
};
}

#endif
