/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  odb_common.h
DESCRIPTION:  common data struct define
FUNCTION LIST:
COMMENT:	  public application
History:                                                        
Date          Version     Modifier                Activities
2002-09-01    1.0                              modify
================================================================================
2002-09-01    1.0                              created
*******************************************************************************/

#ifndef	__ODB_COMMON_H__
#define	__ODB_COMMON_H__

#include "odb_public.h"
//#include "odb_struct.h"
#include "odb_prv_struct.h"

using namespace std;

namespace ODB
{

//系统表管理类
class CCommon
{
public:
	CCommon(){};
	~CCommon(){};
	
	//map file // 2002-10-01
	static char* MapFile(const char* file_name, const int stab_total_size, const bool read_only = false);
	static int UmapFile(char* addr_t, const int size);

	//sort // 2002-11-20
	//static void QkSort(std::vector<RCD_TYPE>& r, int base, int top);
	static void QkSort(std::vector<RCD_TYPE>& r, const int base, const int top);
	 static void StlSort(std::vector<RCD_TYPE>& r);
	
	//char
	static char* itoa(const int value);
	static int str_lwr(char* str_dst, const char* str_src);
	static int str_upr(char* str_dst, const char* str_src);
	
	static bool strip_blank1(char* ptr);	
	static int  strip_blank2(char* str_src);
	static bool StripBlank(char* str_src);
	//static void str_up(char* str);
	//static void str_lower(char* str);
	//inline static char* dwcase(char* str);
	
	//sem  2002-12-28
	static int InitSem(const key_t key, const int nsems=1);
	static int p(const int sem_id, const int sem_num=0);
	static int v(const int sem_id, const int sem_num=0);
	static int GetValue(const int sem_id, const int sem_num=0);

	//shm
	static char* GetShmPtr(const key_t key, const int size);
	static int   GetShmID(const key_t key, const int size);
	static char* GetShmPtrByID(const int shm_id);
	
	//time
	static void  TimevalSub(struct timeval* end, struct timeval* start);
	
	static bool IsApplication(const char* app_name);
	static bool IsApplication(const int app_no);
	
	static void* Malloc(const int buf_size);
	static void Free(void* pointer);

	static int GetOdbId(const char* odb_id_sysfile);
	
	static int TransTime(time_t cur_time, char* time_str);

public:
	static int ExchangeData(char* buf_ptr, const int buf_size, const std::vector<MEMBER_DEFINITION>& vec_offset, const DB_BYTE_T& remote_byte);
	static int ExchangeByteOrder(char* buf_ptr, const int len);
	static DB_BYTE_T GetByteTag();

	static int GetSize(std::vector<MEMBER_DEFINITION>& vec_member);

public:
	static int PrintCtrlTab(const struct DB_CTRL_TAB* ctrl_tab_ptr);
	static int PrintStdbTab(const struct STDB_TAB* stdb_tab_ptr);
	static int PrintStabField(const struct STDB_FIELD_TAB* stdb_field_tab_ptr);
	static int PrintField(const int data_type, const char* field_data_ptr);

private:
	//static void QkPass(std::vector<RCD_TYPE>& r, int start, int tend, int& i);
	static void QkPass(std::vector<RCD_TYPE>& r, const int start, const int tend, int& i);
	
	static int CheckSize(std::vector<MEMBER_DEFINITION>& m_vecMember);
	static int MakeOffset(std::vector<MEMBER_DEFINITION>& m_vecMember);
	static int AssignOffset(int& index_pos, const int assign_val);

public:
	static void  keyid_to_long(struct KEY_STRU *key_stru_ptr,unsigned long*key_id_ptr);
	static void long_to_keyid(unsigned long key_id,struct KEY_STRU *key_stru_ptr);

	static void long_to_tableNo(unsigned long key_id,short *table_no_ptr);
	static void long_to_field_id(unsigned long key_id,short *field_id_ptr);
	static void long_to_key(unsigned long key_id,int  *key_ptr);

	static void key_id_to_long(int key,short field_id,short table_id,unsigned long*key_id_ptr);
	static void long_to_key_id(unsigned long key_id,int *key_ptr,short *field_id_ptr,short *table_id_ptr);
	
	static int long_to_area_no(const long key_id);
	static void long_to_key_in_area(const long key_id, int& key_inarea);
	static void area_key_id_to_long(int area_no, int key, short field_id, short table_id, long& key_id);
	static void long_to_long_without_area(const long id_with_area, long& id_without_area);
	
	static int find_max_prime(int value);
	
	static  int hash33(char * code , int len , int max);
	static  int hash37(char * code , int len , int max);
	static  int hash_string(char * code , int len , int max);
	static  int hash_int(char * code , int len , int max);
	static  int hash_long(char * code , int len , int max);
	static int 	ShmDetach(char *& shm_ptr , int size);
	static int 	ShmRemove(int shm_id,int type);
	static  int  Equal(char *,char *,int length);
	static  int MemCpy(char *dst,char *src,int length);
	static void  FormatAppKeyStru(char* app_key_stru_ptr);
	static int FieldCompare(char *src_ptr,char *dst_ptr,int type,int length); //return val : 0 means equal , others means not equal
	//static int NeedFixIndex(int ctx_no,int app_no,int table_no,int index_seq_no);//这儿的index_seq_no是索引在表头里面的顺序号0、1、2....,而不是索引定义表中定义的索引ID。
	//int RCDCmp(const void *ee1,const void *ee2); 
	
//	//nothing
//	static void do_nothing1(int nn);
//	static void do_nothing2(int nn);
};

}

#endif

