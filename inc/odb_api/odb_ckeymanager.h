#ifndef  _CKEYMAN
#define  _CKEYMAN
#include "odb_define.h"
#include "odb_prv_struct.h"
#include "sys/time.h"
#include "odb_autolock.h"


#define MAGICLEN   16 

#define MAX_FIX_INDEX_NUM 100
struct FIX_INDEX_DEF
{
	short ctx_no;
	int app_no;
	int table_no;
	int idx_seq_no;
};

struct KEY_MANAGE_TAB
{
	char magic[MAGICLEN];
	int valid ;// = INVALID;
	int shmKey;
	int semKey;
	time_t createTime; 
	
	int fix_index_num;
	struct FIX_INDEX_DEF fix_index_def_array[MAX_FIX_INDEX_NUM];
};



class CKeyManager
{
private:
	static struct KEY_MANAGE_TAB* m_ShmManTabPtr;
	static CKeyManager * m_KeyManPtr;
	static int m_SemId;

	CKeyManager();

	CKeyManager(const CKeyManager&);
	CKeyManager& operator =(const CKeyManager&);
	int Init();
	
	
public:
	int GetShmKey();
	int GetSemKey();
	time_t GetCreateTime();

	int GetFixIdxDef(struct FIX_INDEX_DEF **fix_idx_array_ptr);
	int InsertFixIdxDef(struct FIX_INDEX_DEF &fix_idx_def);
	int DeleteFixIdxDef(struct FIX_INDEX_DEF &fix_idx_def);
	
	static CKeyManager * GetInstance();
};

#endif
