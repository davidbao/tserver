/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  odb_rtdbfactory.h
DESCRIPTION:  class system interface define
FUNCTION LIST:
COMMENT:
History:                                                        
Date          Version     Modifier                Activities
2003-09-10    1.0                              modify
2003-12-05    1.1                              modify
================================================================================
2003-09-10    1.0                              created
*******************************************************************************/
#ifndef	__ODB_RTDBFACTORY_H__
#define	__ODB_RTDBFACTORY_H__

#ifndef	__ODB_TABLEBASE_H__
#include "odb_tablebase.h"
#endif

#ifndef	__ODB_TABLEOP_H__
#include "odb_tableop.h"
#endif

#ifndef __ODB_APIOP_H__
#include "odb_apiop.h"
#endif

#ifndef __ODB_TABLENET_h__
#include "odb_tablenet.h"
#endif

#ifndef __ODB_APINET_h__
#include "odb_apinet.h"
#endif
#include "sam_service.h"

namespace ODB
{
class COdbFactory
{
public:
	COdbFactory();
	~COdbFactory();
	
	virtual CTableBase* Make(const int app_no, const int table_no, const short context_no, const char* hostname, const int host_mode, const int rtdb_mode=0) ;
	
private:
	CServicesManage*  m_ServicesManage;
	
	CTableBase*		  m_Instance;
	CTableOp*		  m_TableOp;
	CApiOp*		 	  m_ApiOp;
	NET_ODB::CTableNet*		  m_TableNet;	
	NET_ODB::CApiNet*		  m_ApiNet;
	
	short		m_ContextNo;
	int         m_AppNo;
	int         m_TableNo;
	bool		m_HostMode;	
	bool 		m_RtdbMode;
	char        m_HostName[MAX_NAME_STRING_LEN];
};

}

#endif