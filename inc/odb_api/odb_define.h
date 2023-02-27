/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  odb_define.h
DESCRIPTION:  macro define and error no
FUNCTION LIST:
COMMENT:
History:                                                        
Date          Version     Modifier                Activities
2002-09-01    1.0                              modify
================================================================================
2002-09-01    1.0                              created
*******************************************************************************/
#include "system.h"
#include "d5000_err.h"

#ifndef	__ODB_DEFINE_H__
#define	__ODB_DEFINE_H__

const   int    MAX_DB_NUMBER       = 16000;
const 	int	   MAX_AREA_NUM        = 256;
                                             // 2002-10-17
const   int    MAX_APP_TABLE       = 256;    //for each application
const   int    MAX_CONTEXT_NUM     = 64;
const   int    MAX_APP_NUM         = 256;
const   int    MAX_TABLE_FIELD     = 512;   //512
const   short  MAX_CONTEXT_NO	   = 16384;
/*----------------------------------------------------------------------------*/

const	int	   MAX_SUBKEY_NUMBER   =  4;

const	unsigned int	   MAX_KEY_LEN         =  40;
const	unsigned int	   MAX_FILE_PATH_LEN   =  40;
const	unsigned int	   MAX_NAME_STRING_LEN =  40;
const	unsigned int	   MAX_DATAFIELD_LEN   =  16;
const	unsigned int	   MAX_PASSWD_LEN      =  8;

const	unsigned int	   ENG_NAME_LEN 	   = 32;
const	unsigned int	   CHN_NAME_LEN 	   = 64;

const	unsigned int	   ORIGIN_MENU_NAME_LEN 	   = 40;
const	unsigned int	   MENU_NAME_LEN 	   = 64;
const	unsigned int	   MENU_DISPLAY_LEN    = 40;
const	unsigned int	   DEFAULT_VALUE_LEN   = 20;
const	unsigned int	   ARRAY_NAME_LEN 	   = 32;
const	unsigned int	   DESCRIPTION_LEN 	   = 100;
/*----------------------------------------------------------------------------*/

const   unsigned int   RECORD_HEAD_LEN      = 4;
const   unsigned int   MV_SIZE              = 1024;
const   unsigned int   INT_ADDRESS_ASSIGN   = 4;

const   unsigned int   MAX_PHY_RECORD_LEN   = 15000;

const   int   RTDB_APP_MOD         = AF_AP_MOD;   //change from 100 at 2004-07-14
const   int   ORB_TRY_TIMES        = 3 ;     //initial 20
/*----------------------------------------------------------------------------*/

const   int   RTDB_SYSTEM_SEM_KEY  = 0x20000;
const   int   RTDB_APP_SEM_KEY     = 0x20010;
//const   int   RTDB_TABLE_SEM_KEY   = 0x3200;
const   int   RTDB_TABLE_SEM_KEY   = 0x201;

/***************for CKeyManager SHMKEY and SEMKEY  Begin******************************/
const int RTDB_TABLE_SHM_KEY = 0x8000;
const int RTDB_KEY_MANAGE_SHM_KEY = 0x7999;
const int RTDB_KEY_MANAGE_SEM_KEY = 0x20001;
/***************for CKeyManager SHMKEY and SEMKEY  End *******************************/

const   int   MAX_SEM_NUM          = 25;
const   int   RTDB_SEM_NUM         = 12;
/*----------------------------------------------------------------------------*/
const   short ENTITY_APP           = 1;
const   short ENTITY_TABLE         = 2;
const   short ENTITY_FIELD         = 3;

const unsigned char MODE_CHN        = 0;
const unsigned char MODE_ENG        = 1;

const unsigned char MODE_NAME_BY_NO = 0;
const unsigned char MODE_NO_BY_NAME = 1;
/*----------------------------------------------------------------------------*/


const   char  PATH_VIRGULE        = '/';

//#else
//
//const   char  PATH_VIRGULE        = '\';
//
//#endif

/*----------------------------------------------------------------------------*/
const   int   MODE_API             = 1;


//      CONDITION_TYPE: condition_type
/*----------------------------------------------------------------------------*/


const   short    SIMPLE_CON  	 =  0;
const   short    MULTI_CON   	 =  1;
const   short    HIS_SIMPLE_CON  =  2;
const   short    HIS_MULTI_CON   =  3;
const   short    XY_CURVE_CON    =  4;
const   short    TABLE_CON   	 =  5;
const   short	 STATISTIC_CON   =  6;

/* ---------------------------------------------------------------------------*/
/* Db data type */
typedef  unsigned char DATA_TYPES_T;         //可以考虑为enum

/* -----------------------------------------------------------
   DB_OBJECT_T:  2002-10-22
   ---------------------------------------------------------- */
enum DB_OBJECT_T
{
	OBJECT_NONE          = 0,
	OBJECT_SYSTEM        = 1,
	OBJECT_APP           = 2,
	OBJECT_TABLE         = 3
};
/* -----------------------------------------------------------
   DB_LOCK_T:  2002-10-22
   ---------------------------------------------------------- */
enum DB_LOCK_T
{
	DB_LOCK_NONE    = 0,
	DB_LOCK_READ    = 1,
	DB_LOCK_WRITE   = 2
};

/* -----------------------------------------------------------
   DB_BYTE_T:  2002-10-22
   ---------------------------------------------------------- */
#if 0
enum DB_BYTE_T
{
	BYTE_LOW_LOW    = 0,   //低 高1_8 9_16 17_24 25_32
	BYTE_LOW_HIGH   = 1,   //     8_1 16_9 24_17 32_25
	BYTE_HIGH_HIGH  = 2,   //     32_25 24_17 16_9 8_1
	BYTE_HIGH_LOW   = 3,
	BYTE_OTHER      = 4
};
#endif
enum DB_BYTE_T
{
	BYTE_LITTLE_ENDIAN   = 0,
	BYTE_BIG_ENDIAN,
	BYTE_OTHER
};

/* -----------------------------------------------------------
   DB_CONTEXT_T:  2002-10-18
   ---------------------------------------------------------- */
enum DB_CONTEXT_T
{
	CONTEXT_REAL_TIME = 0,
	CONTEXT_RESEARCH,
	CONTEXT_INSTRUCT,
	CONTEXT_HISTORY   = 3,
	CONTEXT_NONE      = 4
};

/* -----------------------------------------------------------
   DB_STATUS_T:  2002-10-16
   ---------------------------------------------------------- */
enum DB_STATUS_T
{
	TABLE_INIT    = 0,  //系统初始化,表未创建
	TABLE_NORMAL  = 1,  //已经创建
	TABLE_DELETED = 2,	//创建后删除
	TABLE_ERROR =3 ,//表或者索引出现错误，需要修复。
	TABLE_DOWNLOADING = 4 //表正在下装，下装结束后，状态置成TABLE_NORMAL
};

/* -----------------------------------------------------------
   DB_CLASS_T: db_class
   ---------------------------------------------------------- */
enum DB_CLASS_T
{
	STDB = 0,
	VIEW = 1
};

/* -----------------------------------------------------------
   DB_TYPES_T: db_type
   ---------------------------------------------------------- */
enum DB_TYPES_T
{
	DATA  = 0,
	INDEX = 1
};
/* -----------------------------------------------------------
   DB_MEDIA_T:   db_media
   ---------------------------------------------------------- */
enum DB_MEDIA_T
{
	MEMORY = 0,
	DISK   = 1
};
/* -----------------------------------------------------------
   INDEX_TYPE_T:   index_type
   ---------------------------------------------------------- */
enum INDEX_TYPE_T
{
	KEYWORD        = 0,
	RECORD_POINTER = 1
};

const int RTDB_NOT_USED = 0;
const int RTDB_USED = 1;


/* ---------------------------------------------------------
   DATA_TYPES_T: db data type
   --------------------------------------------------------- */
#if 0
enum DATA_TYPES_T
{
	C_STRING_TYPE = 1,
	C_UCHAR_TYPE  = 2,
	C_SHORT_TYPE  = 3,
	C_INT_TYPE    = 4,
	C_LONG_TYPE   = 5,
	C_FLOAT_TYPE  = 6,
	C_DOUBLE_TYPE = 7,
	C_KEYID_TYPE  = 8,
	C_BINARY_TYPE = 9
};
#endif

const int C_STRING_TYPE   = 1;
const int C_UCHAR_TYPE    = 2;
const int C_SHORT_TYPE    = 3;
const int C_INT_TYPE      = 4;
const int C_DATETIME_TYPE = 5;
const int C_FLOAT_TYPE    = 6;
const int C_DOUBLE_TYPE   = 7;
const int C_KEYID_TYPE    = 8;
const int C_BINARY_TYPE   = 9;
const int C_TEXT_TYPE     = 10;
const int C_IMAGE_TYPE    = 11;
const int C_APPKEY_TYPE   = 12;
const int C_APPID_TYPE    = 13;
const int C_UINT_TYPE     = 14;
const int C_LONG_TYPE     = 15;




/* ---------------------------------------------------------
   DATA_TYPES_DISPLAY: db data type
   --------------------------------------------------------- */
enum DATA_DISPLAY_T
{
	D_CHAR_TYPE              = 1,
	D_UCHAR_TYPE             = 2,
	D_SHORT_TYPE             = 3,
	D_INT_TYPE               = 4,
	D_LONG_TYPE              = 5,
	D_FLOAT_TYPE             = 6,
	D_DOUBLE_TYPE            = 7,
	D_SINGLEMENU_TYPE        = 8,
	D_MUTIMENU_TYPE          = 9,
	D_STRING_TYPE            = 10,
	D_DATE_TYPE              = 11,
	D_TIME_TYPE              = 12,
	D_DATETIME_TYPE          = 13,
	D_MULTISTRING_TYPE       = 14,
	D_KEYID_TYPE             = 15,
	D_BINARY_TYPE            = 16,
	D_PASSWORD_TYPE          = 17,
	D_TEXT_TYPE              = 18,
	D_IMAGE_TYPE             = 19,
	D_FORMULASTRING_TYPE     = 20,
	D_FIELD_NAME_STRING_TYPE = 21
};

   
#if 0
const int D_CHAR_TYPE             	1
const int D_UCHAR_TYPE            	2
const int D_SHORT_TYPE            	3
const int D_INT_TYPE              	4
const int D_LONG_TYPE             	5
const int D_FLOAT_TYPE            	6
const int D_DOUBLE_TYPE           	7
const int D_SINGLEMENU_TYPE       	8
const int D_MUTIMENU_TYPE         	9
const int D_STRING_TYPE  	       	10
const int D_DATE_TYPE         		11
const int D_TIME_TYPE         		12
const int D_DATETIME_TYPE         	13
const int D_MULTISTRING_TYPE      	14
const int D_KEYID_TYPE            	15
const int D_BINARY_TYPE           	16
const int D_PASSWORD_TYPE         	17
const int D_TEXT_TYPE         		18
const int D_IMAGE_TYPE         		19
const int D_FORMULASTRING_TYPE    	20
const int D_FIELD_NAME_STRING_TYPE	21
#endif

const int MENU_ALL = 2;
const int MENU_OFF = 1;
const int MENU_ON  = 0;

/* -------------------------------------------------------------
   ACCESS_METHODS_T: db storage methods
   ------------------------------------------------------------- */
enum ACCESS_METHODS_T
{
	ACCESS_SEQUENT            = 0,
	ACCESS_DIRECT             = 1,
	ACCESS_HASH               = 2,
	ACCESS_DIRECT_HASH        = 3,
	ACCESS_HASH_INDEX         = 4,
	ACCESS_DIRECTINDEX_HASH   = 5,
	ACCESS_FLOW               = 6,
	ACCESS_NAMEID_HASH        = 7,
	ACCESS_DIRECTINDEX_FLOW   = 8,	
	ACCESS_FIELD_SEQUENT_HASH = 9
};

/* -------------------------------------------------------------
   PHY_ACCESS_METHODS_T: db physical storage methods
   ------------------------------------------------------------- */
enum PHY_ACCESS_METHODS_T
{
	PHY_SEQUENT_ACCESS   = 0,
	PHY_LINK_ACCESS      = 1,
	PHY_BLOCKLINK_ACCESS = 2
};

/* -------------------------------------------------------------
   sequent order
   ------------------------------------------------------------- */

#define  ASC               	0
#define  DEC               	1

/* ------------------------------------------------------------
   db open mode
   ------------------------------------------------------------ */

#define  READ_OPEN         	0
#define  WRITE_OPEN        	1
#define  RDWR_OPEN         	2

/* ------------------------------------------------------------
   db record head  status
   ------------------------------------------------------------ */

#define  RECORD_EMPTY      	0
#define  RECORD_EXIST      	1
#define  RECORD_ERASED     	3


/* ------------------------------------------------------------
   db lock type
   ------------------------------------------------------------ */

#define  RECORD_LOCK        	0x01
#define  FIELD_LOCK         	0x02

/* ------------------------------------------------------------
   db lock status
   ------------------------------------------------------------ */

#define  UNLOCK     		0
#define  LOCK        		1


/*------------------------------------------------------------------------------
   keyword type
------------------------------------------------------------------------------*/
const int  INT_KEY           =  1;
const int  STRING_KEY        =  2;
const int  INT_STRING_KEY  	 =  3;
const int  KEYID_KEY         =  4;
const int  APPKEY_KEY        =  5;
const int  APPID_KEY         =  6;
const int  LONG_KEY          =  7;



/* -------------------------------------------------------------
   ------------------------------------------------------------- */
#define	ALL_FIELD		-1
#define	ALL_RECORD		-1

/*------------------------------------------------------------------------------
   real time dbms error retcode   2002-10-14
------------------------------------------------------------------------------*/
const int  DB_OK             =  0;        /*  success                           */
const int  DB_ERROR          =  ERR_RTDB_ERROR;       /*  error                             */
const int  DBE_PARA			 =  ERR_RTDB_PARA;       /*  bad para_parameter		*/

//access
const int  DBE_ACCESSMETH    = ERR_RTDB_ACCESSMETH;   /*  access_meth is  error             */
const int  DBE_EXIST         = ERR_RTDB_EXIST;   /*  Db name is exist, cannot creat    */
const int  DBE_NODB          = ERR_RTDB_NODB;   /*  Db name not founded               */
const int  DBE_INVDBNO       = ERR_RTDB_INVDBNO;   /*  Db no  is wrong                   */
const int  DBE_INVFNAME      = ERR_RTDB_INVFNAME;   /*  Field name not founded            */
const int  DBE_HASOPENED     = ERR_RTDB_HASOPENED;   /*  The db has been opened	          */
const int  DBE_FILE_NOPEN    = ERR_RTDB_FILE_NOPEN;   /*  The file cannot open              */
const int  DBE_READ_FILE     = ERR_RTDB_READ_FILE;   /*  The file cannot read              */
const int  DBE_APP_NO_DB     = ERR_RTDB_APP_NO_DB;   /*  This table isn't in current application */
const int  DBE_DB_NONE       = ERR_RTDB_DB_NONE;   /*  The table is deleted or hasn't been created*/

//lock
const int  DBE_LOCK          = ERR_RTDB_LOCK;   /*  The db is locked                  */
const int  DBE_UNLOCK        = ERR_RTDB_UNLOCK;   /*  The db isnot locked               */
const int  DBE_INVUNLOCK     = ERR_RTDB_INVUNLOCK; 	/*  locker is not me                  */
const int  DBE_NOINDEX       = ERR_RTDB_NOINDEX; 	/*  havenot index                     */

const int  DBE_DIFFSIZE      = ERR_RTDB_DIFFSIZE;   /*  size is different                 */
const int  DBE_NOTCHKEY      = ERR_RTDB_NOTCHKEY; 	/*          ??                        */
const int  DBE_NOCACHE       = ERR_RTDB_NOCACHE; 	/*  no cache block                    */
const int  DBE_NOTE_NEND     = ERR_RTDB_NOTE_NEND; 	/*  NOTE must be last field           */

const int  DBE_CTRLFULL      = ERR_RTDB_CTRLFULL; 	/*  The db ctrl table is full         */
const int  DBE_DBFULL        = ERR_RTDB_DBFULL; 	/*  The db is full                    */
const int  DBE_EODB          = ERR_RTDB_EODB; 	/*  End of Database                   */
const int  DBE_INVSIZE       = ERR_RTDB_INVSIZE;  	/*  The size is invalid               */
const int  DBE_DBBEGIN       = ERR_RTDB_DBBEGIN; 	/*  Have reach the begin of db        */
const int  DBE_DBEND         = ERR_RTDB_DBEND; 	/*  Have reach the end of db          */

const int  DBE_INVCCOMM      = ERR_RTDB_INVCCOMM; 	/*  Invalid comparition command       */
const int  DBE_TMOPEN        = ERR_RTDB_TMOPEN; 	/*  Too many open                     */

////key
const int  DBE_KEYNOTFIND    = ERR_RTDB_KEYNOTFIND; 	/*  keyword not founded               */
const int  DBE_MULTIKEY      = ERR_RTDB_MULTIKEY; 	/*  Multi-keywords                    */
const int  DBE_INVKEY        = ERR_RTDB_INVKEY; 	/*  The keyword is invalid            */
const int  DBE_MKEYFIELD     = ERR_RTDB_MKEYFIELD; 	/*  The keyword field can be only one */
const int  DBE_KEYFLEN       = ERR_RTDB_KEYFLEN; 	/*  The keyword length must be 4 times*/
const int  DBE_DBTYPE        = ERR_RTDB_DBTYPE; 	/*  Invalid db type                   */
const int  DBE_KEYTYPE       = ERR_RTDB_KEYTYPE; 	/*  Invalid keyword type              */
const int  DBE_PASSWD        = ERR_RTDB_PASSWD; 	/*  Invalid passwd                    */

////table
const int DBE_TABLE_NO       = ERR_RTDB_TABLE_NO;  /*  no match table or no*/
const int DBE_TABLE_STA		 = ERR_RTDB_TABLE_STA;  /*  table status is abnormal */

////record
const int  DBE_EXISTREC      = ERR_RTDB_EXISTREC; 	/*  Record existed, cannot write      */
const int  DBE_NEXISTREC     = ERR_RTDB_NEXISTREC; 	/*  Record not existed, cannot modify */
const int  DBE_SUPMAX        = ERR_RTDB_SUPMAX; 	/*  record value too bigger           */
const int  DBE_BELMIN        = ERR_RTDB_BELMIN; 	/*  record value too smaller          */
const int  DBE_RECSIZE       = ERR_RTDB_RECSIZE; 	/*  record size wrong                 */
const int  DBE_NOTERASE      = ERR_RTDB_NOTERASE; 	/*  record not erased                 */
const int  DBE_RECERASE      = ERR_RTDB_RECERASE; 	/*  The record is erased              */
const int  DBE_POINTER       = ERR_RTDB_POINTER;   /*  record_pointer error              */
const int  DBE_CONT_ERR      = ERR_RTDB_CONT_ERR; 	/*  condition express error           */

////field
const int  DBE_DATATYPE      = ERR_RTDB_DATATYPE; 	/*  field data type is wrong          */
const int  DBE_DIFFTYPE      = ERR_RTDB_DIFFTYPE; 	/*  type is different                 */
const int  DBE_KEYFIELD      = ERR_RTDB_KEYFIELD; 	/*  The field is keyword              */
const int  DBE_BIGFIELDNO    = ERR_RTDB_BIGFIELDNO; 	/*  The field no is too big           */
const int  DBE_DLT_WRITE     = ERR_RTDB_DLT_WRITE; 	/*  The direct link hash cannot write */
const int  DBE_INVFIELDNO    = ERR_RTDB_INVFIELDNO;   /*  field_no is wrong		          */
const int  DBE_NOTFIND       = ERR_RTDB_NOTFIND; 	/*  Not find the value                */
const int  DBE_FIELD_NO      = ERR_RTDB_FIELD_NO;  /*  no match field name or no          */
const int  DBE_FIELD_SIZE    = ERR_RTDB_FIELD_SIZE;  /*  field length wrong                 */
const int  DBE_NONE_FIELD    = ERR_RTDB_NONE_FIELD;  /*  this table has no field            */


//operation
const int  DBE_INVCOMM       = ERR_RTDB_INVCOMM;   /*  Invalid db operater command       */
const int  DBE_NPERMIT       = ERR_RTDB_NPERMIT; 	/*  The OP is not permit              */

const int  DBE_CONTEXT       = ERR_RTDB_CONTEXT;   /*  no context or application         */
const int  DBE_APP_UNLOAD    = ERR_RTDB_APP_UNLOAD;   /*  application isn't loaded          */


const int DBE_SQL_COMMAND    = ERR_RTDB_SQL_COMMAND;   /*select error*/
const int DBE_SQL_FORM       = ERR_RTDB_SQL_FORM;   /*form error*/
const int DBE_SQL_WHERE      = ERR_RTDB_SQL_WHERE;   /*where condition error*/
const int DBE_SQL_ORDER      = ERR_RTDB_SQL_ORDER;   /*order by error*/



//memory
const int  DBE_MEMORY        = ERR_RTDB_MEMORY;   /*  Cannot malloc or calloc memory    */
const int  DBE_BUFFFULL      = ERR_RTDB_BUFFFULL; 	 /*  Buffer is full                    */
const int  DBE_MEM_NOALLOC   = ERR_RTDB_MEM_NOALLOC;   /*  The memory cannot allocation      */

const int  DBE_MAP           = ERR_RTDB_MAP;   /*  map file error                   */

//const int  DBE_PTABFULL = -10;    	/*  process tab is full               */

//sql
const int DBE_SQL			 = ERR_RTDB_SQL;   /*  sql syntax error	*/



//
const int  DBE_DBNAMENOTF    = ERR_RTDB_DBNAMENOTF;     /*  db_name not founded                */
const int  DBE_FACNAMENOTF   = ERR_RTDB_FACNAMENOTF;     /*  fac_name not founded	              */
const int  DBE_VALNAMENOTF   = ERR_RTDB_VALNAMENOTF;     /*  val_name not founded	              */
const int  DBE_FIELDNAMENOTF = ERR_RTDB_FIELDNAMENOTF;     /*  field_name not founded	          */

//
const int  DBE_VIEWFIELD     = ERR_RTDB_VIEWFIELD;     /*  view get fieldnum over error       */
const int  DBE_SYBASE	     = ERR_RTDB_SYBASE;     /*  modify sybase error                */
const int  DBE_INDEXERR	     = ERR_RTDB_INDEXERR;     /*  index number error                 */
const int  DBE_MODIFYSIZE    = ERR_RTDB_MODIFYSIZE;     /*  modify bufsize error               */

const int  API_INVCOMM       = ERR_RTDB_API_INVCOMM;  	/*  Invalid db operater command       */
const int  API_NOAPP         = ERR_RTDB_API_NOAPP;  	/*  App name not founded              */
const int  API_NODB          = ERR_RTDB_API_NODB;  	/*  Db  name not founded              */
const int  API_INVDBNO       = ERR_RTDB_API_INVDBNO;  	/*  Db no  is wrong                   */

const int  API_NODEOFF       = ERR_RTDB_API_NODEOFF;  	/*  node  is offline                  */
const int  API_ERRINIT       = ERR_RTDB_API_ERRINIT;  	/*  odbms init error                  */
const int  API_NEWERR	     = ERR_RTDB_API_NEWERR;  	/*  new space error                   */
const int  API_DATAERR       = ERR_RTDB_API_DATAERR;  	/*  data tye error                    */
const int  DBE_NOSPACE       = ERR_RTDB_NOSPACE;  	/*  no space left in disk             */



/* -----------------------------------------------------------
   for sql query:  2002-10-31
   ---------------------------------------------------------- */
enum CONDITION_OPERATOR_T
{
	SQL_COND_LSC = 10,          /*   <     */
	SQL_COND_LEC = 11,			/*   <=    */
	SQL_COND_EQC = 12,			/*   ==    */
	SQL_COND_GEC = 13,			/*   >=    */
	SQL_COND_GTC = 14,			/*   >     */
	SQL_COND_NEQ = 15           /*   !=    */
};

enum LOGICAL_OPERATOR_T
{
	SQL_LOG_AND	= 16,			/*   &&    */
	SQL_LOG_OR	= 17,           /*   ||    */
	SQL_LOG_NOT	= 18,			/*   ~     */
	SQL_LOG_NONE= 20
};


enum SQL_METHOD_T
{
	METHOD_SELECT = 1,
	METHOD_UPDATE = 2,
	METHOD_DELETE = 3,
	METHOD_CREATE = 4
};


/******************************************************************************
*									debug display option														*
******************************************************************************/
#define  DBG_INDEX_STDB_TAB 1
#define DBG_INDEX_STDB_FIELD_TAB 2
#define DBG_STDB_INDEX_HASH 3
#define  DBG_FREE_POINTER 4
#define DBG_INDEX_STG 5
#define DBG_INDEX_OVERFLOW 6
#define DBG_ALL 7
#endif
