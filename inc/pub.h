#ifndef	__PUBLIC_H__
#define	__PUBLIC_H__

#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#ifdef  _WINDOWS32
#ifdef PUB_EXPORTS
#define PUB_API __declspec(dllexport)
#else
#define PUB_API __declspec(dllimport)
#endif
#else 
#define PUB_API
#endif

#include "pub_buf.h"
#include "pub_ctx.h"

class PUB_API CPublic
{
public:
	CPublic() {};
	~CPublic() {};
	
	static char* MapFile(const char* file_name, const int stab_total_size);
	static int UmapFile(char* addr_t, const int size);

#ifndef _WINDOWS32
	// semaphore operations
	static int InitSem(const key_t key, const int nsems=1);
	static int p(const int sem_id, const int sem_num=0);
	static int v(const int sem_id, const int sem_num=0);
	static int GetValue(const int sem_id, const int sem_num=0);

	// shared memory operations
	static char* GetShmPtr(const key_t key, const int size);
	static int   GetShmID(const key_t key, const int size);
	static char* GetShmPtrByID(const int shm_id);
#else
	// semaphore operations
	static int p(const char* sem_name, const int sem_num);
	static int v(const char* sem_name, const int sem_num=1, const int release_num=1);
#endif
	
        // time
        static void  TimevalSub(struct timeval* end, struct timeval* start);
	
	// memory operation
	static void* Malloc(const int buf_size);
	static void* Realloc(void* pointer, const int buf_size);
	static void  Free(void* pointer);
};


#endif
