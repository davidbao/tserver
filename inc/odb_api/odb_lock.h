/*******************************************************************************
ModuleName: ODB
FileName:	odb_lock.h 
DESCRIPTION:     
History:                                                        
Date          Version     Modifier                Activities
2002-10-22    1.0                              modify
================================================================================
2002-10-22    1.0                              created
*******************************************************************************/

#ifndef	__ODB_LOCK_H__
#define	__ODB_LOCK_H__

//#include "odb_struct.h"
#include "odb_prv_struct.h"
#include "odb_public.h"


namespace ODB
{ 
class COdbLock
{
public:
	COdbLock(){};
	~COdbLock(){};
	
	static int  InitLock(struct LOCK_STRU& lock);
	static int  ClnLock(struct LOCK_STRU& lock);
	
	static bool CtlLock(const struct LOCK_STRU& lock, const DB_LOCK_T& lock_type);
	static bool CtlUnLock(const struct LOCK_STRU& lock, const DB_LOCK_T& lock_type);
	
	static bool DtLock(const struct LOCK_STRU& lock, const DB_LOCK_T& lock_type);
	static bool DtUnLock(const struct LOCK_STRU& lock, const DB_LOCK_T& lock_type);
	
	static bool NpLock(struct LOCK_STRU& lock, const DB_LOCK_T& lock_type);
	static bool NpUnLock(struct LOCK_STRU& lock, const DB_LOCK_T& lock_type);
	
	static bool Lock(struct LOCK_STRU& lock, const DB_LOCK_T& lock_type);
	static bool UnLock(struct LOCK_STRU& lock, const DB_LOCK_T& lock_type);
	
	static bool Lock(struct LOCK_STRU& lock, const DB_LOCK_T& lock_type, int& table_semid);    //for OPTLOCK
	static bool UnLock(struct LOCK_STRU& lock, const DB_LOCK_T& lock_type, int& table_semid);  //for OPTLOCK

private:

	static bool CtlLock(const struct LOCK_STRU& lock);
	static bool CtlUnLock(const struct LOCK_STRU& lock);
	
	static bool CtlLock(const struct LOCK_STRU& lock, int& table_semid);   //for OPTLOCK
	static bool CtlUnLock(const struct LOCK_STRU& lock, int& table_semid); //for OPTLOCK
	
	static bool DtLock(const struct LOCK_STRU& lock);
	static bool DtUnLock(const struct LOCK_STRU& lock);
	
	static bool TryLock(struct LOCK_STRU& lock, const DB_LOCK_T& lock_type);
	static bool TryLock(struct LOCK_STRU& lock, const DB_LOCK_T& lock_type, int& table_semid); //for OPTLOCK
};

}
#endif
