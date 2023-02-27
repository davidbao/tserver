//***********************************************************************************
//
//	本公共类作为CORBA客户端调用的封装类
//	version:	no.3
//	time:		2003.03.16
//
//
// Created by Wangx / Huanghf           // 2003-01-01
//***********************************************************************************	

#include <OB/CORBA.h>
#include <OB/Properties.h>

#include <stdlib.h>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

#ifndef __ODB_ORBMGR_H__
#define __ODB_ORBMGR_H__
namespace ODB
{
class COrbMgr
{
public:
	COrbMgr();
	~COrbMgr();
	
	bool InitOrb();
	bool InitOrb(const char* n_orbname);
	bool DelInitOrb();
	bool DelOrb();
	
	bool SetOrbPara(const char* n_str_para_name, const char* n_str_para_value);
	
	CORBA::Object_var	GetRemoteObj(CORBA::ORB_var n_orb, const char* n_reffilename);
	CORBA::Object_var	GetRemoteObj(CORBA::ORB_var n_orb, const char* n_server, const char* n_port, const char* n_objectname);
	
	CORBA::ORB_var      GetOrb();
	bool GetOrb(CORBA::ORB_var & in_orb);
	
	//bool				SetOrb(CORBA::ORB_var n_orb);
	
private:
	OB::Properties_var m_Props;
	CORBA::ORB_var     m_Orb;
};
}
#endif
