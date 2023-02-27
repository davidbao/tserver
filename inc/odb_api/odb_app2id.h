#ifndef _APPNAME2ID_
#define _APPNAME2ID_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



#define MAXSIZE 8092

struct ID2NameNode
{
	int app_id;
	char app_name[32];
	
	struct ID2NameNode * next_pointer;
};

#if 0
class CAppNameIDTrans
{
	private :
		CAppNameIDTrans();
		
		CAppNameIDTrans(const CAppNameIDTrans&);
		
		void operator= (const CAppNameIDTrans&);
		
		static pthread_mutex_t  s_guard;
		static CAppNameIDTrans * s_instance ;
		
		struct ID2NameNode * m_hashTable;
		struct ID2NameNode * second;
		
		long * freePos;
		long freePointer;
		
	public:
		static CAppNameIDTrans * getInstance();
		
		char *  getAppNameByID(const int app_id);
		
		
		int createHashTable();
		
		int putData2Table();
	
		
};
#else
class CAppNameIDTrans
{
	public :
		CAppNameIDTrans();
		
		CAppNameIDTrans(const CAppNameIDTrans&);
		
		~CAppNameIDTrans();
		
		void operator= (const CAppNameIDTrans&);
		
	private:
		
		static pthread_mutex_t  s_guard;
		static CAppNameIDTrans * s_instance ;
		
		ID2NameNode * m_hashTable;
		ID2NameNode * second;
		char* m_InitPtr;
		
		long * freePos;
		long freePointer;
		
	public:
		static CAppNameIDTrans * getInstance();
		
		char *  getAppNameByID(const int app_id);
		
		
		int createHashTable();
		
		int putData2Table();
	
		
};
#endif

#endif
