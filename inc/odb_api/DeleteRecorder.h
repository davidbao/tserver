// insert into the odb_table.cpp

#include <pthread.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>


#define 					LOGLOCK 						0x10999			
#define 					MAXSIZE							30000000
#define						BIGSIZE							90000000
#define 					FRESH								(int)'6'
#define						NOTFRESH						(int)'A'




class DeleteRecorder
{
	private:
		FILE* _m_file_handle;
		int _m_file_size;
		int _m_status;
		
		char processName[256];
		char fileName[256];
		
		unsigned long processID;
		int lockID;
		time_t fileTag; 
		
	  static DeleteRecorder * _m_instance;
	  static pthread_mutex_t _m_lock;
	  
	  
	  DeleteRecorder(const DeleteRecorder &);
	  void operator = (const DeleteRecorder &);
	  
	  int initLock();
	  int lockLog();
	  int unlockLog();
	  
	  int initLog();
	  int checkAndRenameLog();
	  int getNameFromPid();
	  
	public:
		static DeleteRecorder * getInstance();
		
		DeleteRecorder();
		~DeleteRecorder();
		
		int openLog();
		int writeLog(int app, int tab, long logContent);
	//	int renameLog();
		int isValid();
		

		
};

union semun
{
	int              val;    /* Value for SETVAL */
 	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux specific) */
};

void WriteLog(int,int,long);
