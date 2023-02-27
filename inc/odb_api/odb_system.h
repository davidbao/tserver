/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  odb_system.h
DESCRIPTION:  class system struct define
FUNCTION LIST:
COMMENT:
History:                                                        
Date          Version     Modifier                Activities
2002-09-01    1.0                              modify
================================================================================
2002-09-01    1.0                              created
*******************************************************************************/

#ifndef	__ODB_SYSTEM_H__
#define	__ODB_SYSTEM_H__

//#include "odb_struct.h"
#include "odb_prv_struct.h"
#include "odb_public.h"
#include "odb_common.h"
#include "odb_profile.h"
#include "odb_app2id.h"


using namespace std;

namespace ODB
{
//系统表管理类
class COdbSystem
{
public:
	COdbSystem();
	COdbSystem(const int app_no);
	COdbSystem(const char* all_name);	
	//COdbSystem(int context_no);           //指定context
	//COdbSystem(char* context_name);
	COdbSystem(const bool is_loaded);
	~COdbSystem();
	
	// //构造函数的辅助函数
    //根据应用信息初始化
	int InitSystem(const int app_no, const int table_no=0);
	int InitSystem(const char* all_name, const int table_no=0);
	int InitSystem();
	
	//
	//恢复原来当前应用
	//int LoadSystem(const int& app_no);
	//int LoadSystem(const char* all_name);
	int LoadSystem(const short context_no, const int app_no, bool read_only = false);         //2003-03-25
	int LoadSystem(const short context_no, const char* all_name, bool read_only = false);     //////
	int LoadSystem(const char* context_name, const int app_no);
	int LoadSystem(const char* context_name, const char* all_name); //////
	int LoadSystem(bool read_only = false); //恢复原来当前应用,当前应用难以判断?

	//
	//处理态的内容
	int LoadContxt(const short context_no, bool read_only = false);
	int LoadContxt(const char* context_name, bool read_only = false);
	int LoadContxt(bool read_only = false);

private:
	int LoadApp(const char* all_name, const int r_table_no, bool read_only = false);
	int SetDefaultEnv();
	int CleanAppEnv();
	int InitEnv();

public:
	//2002-10-21
	int MoveTo(const short context_no, bool read_only = false);
	int MoveTo(const char* context_name, bool read_only = false);
	int ShiftTo(const int app_no);
	int ShiftTo(const char* all_name, bool read_only = false);
	
	//
	//管理态的名称和数量
	int SetCurContxtName(const char* context_name);
	
	int AddContext(struct DB_CONTEXT_TAB* context_ptr);
	//int DelContext(struct DB_CONTEXT_TAB* context_ptr);
	int DelContext(const short context_no);
	//int DelContext(int context_no);
	int DelContext(const char* context_name);
	
	int AddApp(const int app_no);
	int AddApp(const char* all_name, bool read_only = false);
	int FastAddApp( const char *all_name ,const short context_no, bool read_only = false);
	//int AddApp(struct DB_APP_TAB* app_ptr);  //current context//未全部实现
	int DelApp(const int app_no);
	int DelApp(const char* all_name);
	
	//int GetSysMap();
	
	//2002-10-21
//	int	MinusDbNumTab();
//	int	AddDbNumTab();
	
	//
	//得到系统信息指针
	char* GetSysContxtTabPtr();

	//2002-10-22
	struct	DB_SYSTEM_TAB*	GetSysTabPtr();
	struct	DB_STATICS_TAB*	GetStaticsTabPtr();
//	int	    SetStaticsParaPtr(int tag, int op_time);
	
	//得到态信息指针
	struct  DB_CONTEXT_TAB* GetCurContextTabPtr();
	struct  DB_CONTEXT_TAB* GetContextTabPtr(short context_no);
	struct  DB_CONTEXT_TAB* GetContextTabPtr(char* context_name);
	
	
	//得到当前应用信息
	struct  DB_APP_TAB*     GetCurAppTabPtr();                   //current context current app
	struct  DB_APP_TAB*     GetAppTabPtr(const int app_no);      //current context
	struct  DB_APP_TAB*     GetAppTabPtr(const char* all_name);
	char* GetAppPtr(int app_no);      //current context
	char* GetAppPtr(const char* all_name);
	
	struct	DB_CTRL_TAB*	GetCtrlTabPtr();               //current context and app, first DB_CTRL_TAB
	struct	DB_CTRL_TAB*	GetADbCtrlPtr(const char* table_name); //
	struct	DB_CTRL_TAB*	GetADbCtrlPtr(const int r_table_no);   //
	//struct	DB_CTRL_TAB*	GetANewCtrlPtr(int odb_no);    //与上同
	struct DB_CTRL_TAB* GetANewCtrlPtr(const int r_table_no);
	
	//lmj add 050405
	int SetCurAppResInfo(int nDownLoadApp);
	
public:
	int   GetCurAppName(char* all_name);
	//char* GetAppNameByNo(const short& app_no);
	int   GetAppNameByNo(char* all_name, const int app_no);
	int   GetAppNoByName(const char* all_name);

//    2004-06-25	
//	int   GetAppNoByTableNo(const int r_table_no);
//	int   GetAppNameByTableNo(string& all_name, const int r_table_no);
	
	char* GetTableNameByNo(const int r_table_no);
	int   GetTableNameByNo(char* table_name, const int r_table_no, const bool is_eng=true); // 返回数组
	
	int   GetTableNoByName(const char* table_name, bool is_real = false);
	int   GetTableNoByChnName(const char* table_name, bool is_real = false);
	
	//查询应用的表信息
	int   GetAllOdbNoByAppName(int& app_no, std::vector<int>& table, const char* app_name);  // 2002-12-02
	int   GetAllAppNo(std::vector<struct NAME_ID>& vec_appno);
	int   GetAppOdbNoFromFile( int& app_no, std::vector<int>& table, const char* app_name);
	
	//判断表和应用的关系
	bool  IsTableInApp(const int r_table_no, const int app_no);
	bool  IsTableInApp(const int r_table_no, const char* const app_name);
	bool  IsTableInApp(const char* const table_name, const char* const app_name);
	int   PrintSysMap();
	int   PrintTableMap( const MAPTABLE_T& tm);

private:	
	//2002-10-23
	bool Lock(const DB_OBJECT_T& object_type, const DB_LOCK_T& lock_type);
	bool UnLock(const DB_OBJECT_T& object_type, const DB_LOCK_T& lock_type);

private:
	//read from file *.sys
	//static int GetSysMap();      //   2004-06-23
	int GetSysMap();
	int GetTableMap(const char* app_name, MAPTABLE_T& map_name_no);
	
	//for init
	int InitSysContext(struct SYS_CONTEXT_TAB* sys_ptr);
	int InitContext(struct DB_CONTEXT_TAB* context_ptr);
	int InitAContxt(struct DB_CONTEXT_TAB* one_context_ptr, const short context_no);
	
	int InitApp(struct DB_APP_TAB* app_ptr);
	int InitSystemTab(struct DB_SYSTEM_TAB* sys_ptr);
	int InitStaticsTab(struct DB_STATICS_TAB* sta_ptr);
	int InitCtrlTab(struct DB_CTRL_TAB* first_ctrl_tab_ptr);
	

	//reset info for context & app.
	int SetCurAppTabPtr(char* app_file_ptr);
	int SetContxtTabPtr(char* system_file_ptr);
	
	//refresh cur_app's table list
	int RefreshTableList();
	
private:
	int   GetAppPara(const char* all_name, char* app_name, char* app_alias, int& app_no);
	
	//
	//get file name
	//
	int GetAppFile(char* file_name, const struct DB_APP_TAB* app_ptr);
	int GetAppFile(char* file_name, const char* all_name);
//	int GetAppFile(char* file_name, const char* context_name, const char* all_name);
	int GetAppFile(char* file_name, const short context_no, const char* all_name);   // 2004-07-07
	
	bool IsAppFile(const char* all_name, const char* file_app);
	
	int GetTableFile(char* file_name, struct DB_APP_TAB* app_ptr, char* table_name);
	int GetTableFile(char* file_name, char* table_name);

private:
	COdbSystem(const COdbSystem& odb_system);
	COdbSystem& operator=(const COdbSystem& odb_system);
	
private:
//	static char*	m_SysAreaPtr;
//	static struct   SYS_CONTEXT_TAB* m_SysContxtTabPtr;      //系统context管理
//	static struct   DB_CONTEXT_TAB*  m_OdbContxtTabPtr;      //context首地址指针 //[MAX_CONTEXT_NUM]

	char*    m_SysAreaPtr;
	struct   SYS_CONTEXT_TAB* m_SysContxtTabPtr;             //系统context管理
	struct   DB_CONTEXT_TAB*  m_OdbContxtTabPtr;			 //context首地址指针 //[MAX_CONTEXT_NUM]

	struct DB_CONTEXT_TAB*  m_CurContxtTabPtr;
	
	std::map<string, char*> m_MapAppFile;
	
	struct DB_APP_TAB*      m_CurAppTabPtr;                  //当前app
	struct	DB_SYSTEM_TAB*	odbsys_tabptr;
	struct  DB_STATICS_TAB*	odbstatics_tabptr;
	struct 	DB_CTRL_TAB*	odbctrl_tabptr;                  //首地址指针，初始化使用
	
	MAPAPP_T m_SysMap;
	
	//static NProfile::CProfile m_Profile;
	NProfile::CProfile* m_Profile;
	CAppNameIDTrans* m_AppIdTrans;

	//static MAPAPP_T m_SysMap;								// 2004-06-23
	//static std::vector<APP_ID> m_SysMap;                  // 2002-12-02  for global access
	//std::vector<APP_ID> m_SysMap;                         // 2002-12-02  for global access
	static bool m_Inited;									// 2004-06-23
	
	char m_SysFile[120];
	char m_OdbFile[120];
	char m_OdbIni[120];
};

}

#endif

