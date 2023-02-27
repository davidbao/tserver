#ifndef  _PVLOCK
#define  _PVLOCK
#include "odb_common.h"
#include "odb_define.h"
#include "pthread.h"
#include "odb_lock.h"

class PVLock
{
private:
	PVLock(const PVLock&);
	PVLock& operator=(const PVLock&);
	int m_semid;
	int m_semno;
public:
	
	PVLock(key_t key , int no);
	~PVLock();
};


class PMutexLock
{
private:
	PMutexLock(const PMutexLock&);
	PMutexLock& operator=(const PMutexLock&);
	pthread_mutex_t * m_mutex;
	
public:
	PMutexLock(pthread_mutex_t * out_mutex):m_mutex(out_mutex)
	{
		if( 0 != pthread_mutex_lock(m_mutex ))
		{
			printf("Error in pthread mutex lock ...,errno === %d \n" , errno);
		}
	}
	~PMutexLock()
	{
		if(0 != pthread_mutex_unlock(m_mutex))
		{
			printf("Error in pthread mutex unlock ... errno === %d \n" , errno);
		}
	}
	
};

#if 0
class CRWLock
{
private:
	struct ODB::LOCK_STRU *m_LockPtr;
	DB_LOCK_T m_RWType;
	CRWLock();
	CRWLock(const CRWLock& theLock);
	CRWLock& operator=(const CRWLock& theLock);
public:
	CRWLock(struct ODB::LOCK_STRU *lock_ptr,const DB_LOCK_T lock_type)
	{
		ODB::COdbLock::Lock(*lock_ptr, lock_type);
		m_LockPtr = lock_ptr;
		m_RWType = lock_type;
	}
	~CRWLock()
	{
		ODB::COdbLock::UnLock(*m_LockPtr, m_RWType);
	}

	int ChangeLockType(const DB_LOCK_T lock_type)
	{
		if(lock_type != m_RWType)
		{
			ODB::COdbLock::UnLock(*m_LockPtr,m_RWType);
			ODB::COdbLock::Lock(*m_LockPtr,lock_type);
			m_RWType = lock_type;
		}
		return 0;
	}

};
#else //for OPTLOCK
class CRWLock
{
private:
	struct ODB::LOCK_STRU *m_LockPtr;
	DB_LOCK_T m_RWType;
	int* m_SemId;
	CRWLock();
	CRWLock(const CRWLock& theLock);
	CRWLock& operator=(const CRWLock& theLock);
public:
		
	//jinjing 20120429
	CRWLock(struct ODB::LOCK_STRU *lock_ptr,const DB_LOCK_T lock_type, int& table_semid);
	~CRWLock();
	int ChangeLockType(const DB_LOCK_T lock_type);
//	CRWLock(struct ODB::LOCK_STRU *lock_ptr,const DB_LOCK_T lock_type, int& table_semid)
//	{
//		//jinjing 20120428
////		ODB::COdbLock::Lock(*lock_ptr, lock_type, table_semid);
////		m_LockPtr = lock_ptr;
////		m_RWType = lock_type;
////		m_SemId = table_semid;
//		m_SemId = table_semid;
//		TRACE("m_SemId1 = %d\n lock_type = %d", m_SemId, lock_type);
//		TRACE("!!!!!Addr_AutoLock.m_SemId = %d\n", &m_SemId);
//		ODB::COdbLock::Lock(*lock_ptr, lock_type, m_SemId);
//		TRACE("!!!!!Addr_AutoLock.m_SemId2 = %d\n", &m_SemId);
//		TRACE("m_SemId2 = %d\n", m_SemId);
//		m_LockPtr = lock_ptr;
//		m_RWType = lock_type;
//		table_semid = m_SemId;
//		TRACE("table_semid = %d\n", table_semid);
//	}
//	~CRWLock()
//	{
//		ODB::COdbLock::UnLock(*m_LockPtr, m_RWType, m_SemId);
//	}
//
//	int ChangeLockType(const DB_LOCK_T lock_type)
//	{
//		if(lock_type != m_RWType)
//		{
//			ODB::COdbLock::UnLock(*m_LockPtr,m_RWType, m_SemId);
//			ODB::COdbLock::Lock(*m_LockPtr,lock_type, m_SemId);
//			m_RWType = lock_type;
//		}
//		return 0;
//	}

};
#endif

#endif
