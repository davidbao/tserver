/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  odb_prv_struct.h
DESCRIPTION:  data struct define
FUNCTION LIST:
COMMENT:
History:                                                        
Date          Version     Modifier                Activities
2002-09-01    1.0                               modify
================================================================================
2002-09-01    1.0                               created
*******************************************************************************/

#ifndef	__ODB_PRV_STRUCT_H__
#define	__ODB_PRV_STRUCT_H__

#include <sys/sem.h>
#include <time.h>
#include <string.h>

#include <string>
#include <map>
#include <set>
#include <vector>

#ifndef __ODB_DEFINE_H__
#include "odb_define.h"
#endif

#include "odb_struct.h"
namespace ODB
{
/* Lock struct */
struct LOCK_STRU
{
	DB_OBJECT_T lock_object;
	DB_LOCK_T   lock_type;
	int         lock_num;
	int         lock_pid;
	int         sem_key;
	int         sem_no;
	int         back_no;
	int	    reserved[2];
	struct LOCK_STRU& operator= (const struct LOCK_STRU& lock2)
	{
		if( this == &lock2)
		{
			return *this;
		}

		lock_object = lock2.lock_object;
		lock_type   = lock2.lock_type;
		lock_num    = lock2.lock_num;
		lock_pid    = lock2.lock_pid;
		sem_key     = lock2.sem_key;
		sem_no      = lock2.sem_no;
		back_no     = lock2.back_no;
		reserved[0] = reserved[0];
		reserved[1] = reserved[1];
		return *this;
	};
};


struct SYS_CONTEXT_TAB
{
	char		   		byte_tag;  //must use type cast from  DB_BYTE_T   // 2004-07-07
	short    			context_num;    //real context个数
	short    			cur_context_no; //
	//	DB_BYTE_T   		byte_tag;

	struct LOCK_STRU   	lock;           //2002-10-23
};

/*PAS应用，研究态多CASE支持*/
struct DB_CONTEXT_TAB
{
	short 				ctx_inner_no;
	short				context_no;
	char				context_name[MAX_NAME_STRING_LEN]
	;

	char 				file_app[MAX_APP_NUM][120];       //app_name + app_alias
	int				app_size[MAX_APP_NUM];            //file size  //2002-10-22
	//char* 				context_app_ptr[MAX_APP_NUM];     //此context的app结构指针(映射后的内存地址)//  2002-12-20

	int   				app_num;                          //real app个数
	char   				cur_app_name[MAX_NAME_STRING_LEN];//app_name + app_alias
	bool  				is_in_use;                       //whether this struct info is valid.

	DB_CONTEXT_T 			context_type;                     //REAL_TIME or not
	int	    			reserved[64];
};


/* Database application table  */
//
//judge application by app_name + app_status
//
struct DB_APP_TAB
{
	char				byte_tag;          //must use type cast from  DB_BYTE_T   // 2004-07-07
	int				app_no;
	int				download_app_no;	//for multi_context
	char				app_name[MAX_NAME_STRING_LEN];
	char				app_alias[MAX_NAME_STRING_LEN]; //应用别名
	short  				context_no;
	char				context_name[MAX_NAME_STRING_LEN];

	bool  				is_in_use;               //is current application, or not
	//	DB_BYTE_T   			byte_tag;                 //this host's byte order
	struct LOCK_STRU   		lock;

	short       			table_sum;                //all table allowed to be created, from odb_app.sys; it is also used to solve the problem of down_load if add new table
	short       			table_num;                //table number in fact, may be increase/decrease when add/delete
	//short					table_upper; 		//to solve the problem of down_load if add new table
	short       			table_no[MAX_DB_NUMBER]	;  //table relation between rtdbms & [rdbms]

	time_t				download_time;//means the create time of app.dat(such as scada.dat , if the machine has been rebooted, the time will be refreshed) 
	int	    			reserved[64];
};

/* Db system table */
struct DB_SYSTEM_TAB
{
	int    db_max_sum;
	int    proc_open_max_sum;
	int    db_sum;
	//	int    memory_db_num;
	//	int    disk_db_num;
	int    stdb_num;
	int    view_num;
	int    index_db_num;
	int    data_db_num;
	int    max_string_length;
	int    max_keyword_length;
	int    max_subkey_num;
	int	    		reserved[64];
	//	key_t  dbms_cur_shmkey;
	//	int    ctrl_tab_shmid;
	//	int    ctrl_tab_shmsize;
};


/* Db statics table */
struct DB_STATICS_TAB
{
	time_t op_time;
	int    op_sum;
	int    success_num;
	int    fail_num;
	int    open_num;
	int    close_num;
	int    read_num;
	int    write_num;
	int    modify_num;
	int    erase_num;
};




/* Database control table  */
struct DB_CTRL_TAB
{
	short       		table_no;	//rtdbms
	short			r_table_no;	//对应商用库
	int			app_no;
	int          		db_id;  	//net model edition //  2002-12-03
	DB_STATUS_T  		db_status;  //TABLE_NORMAL or not
	DB_CLASS_T   		db_class;   //STDB, VIEW
	DB_TYPES_T   		db_type;    //DATA, INDEX

	char         		table_name_eng[ENG_NAME_LEN];
	char         		table_name_chn[CHN_NAME_LEN];

	unsigned char       	is_record_apped;
	//	short	     		fac_rel_fieldno;
	//	short	     		ulevel_rel_field_no;

	char         		passwd[MAX_PASSWD_LEN];
	time_t       		last_close_time;
	int          		open_count;              //被打开或访问次数

	char         		db_file_name[120]; //table data file
	int          		db_size;           //table all len
	//char*        		db_ptr;            //表映射后的内存地址 //根据此来判断表是否可访问, default is NULL//  2002-12-20

	int          		db_offset;             //?
	int          		db_describ_area_size;  //表des area大小(STDB_TAB+SEQUENT_TAB+STD_FILELD_TAB+排序区)
	int          		db_data_area_size;     //表data area大小
	int	    		reserved[64];
	int             area_num;
	int             area_no[MAX_AREA_NUM]; //区域号，下标代表该区域号
	int				r_area_no[MAX_AREA_NUM]; //内部区域号，从下标0开始记录一共用了多少个区域，每个区域号是多少
	char            area_name[MAX_AREA_NUM][ENG_NAME_LEN]; //区域名，下标代表区域号，值代表区域名
};


/* Subkey describle */
struct SUBKEY_STRU
{
	int    subkey_type;
	int    min_subkey;
	int    max_subkey;
	int    fieldvalue_sum;
};

/* Keyword describle table */
struct KEYWORD_STRU
{
	int    key_type;
	int    key_len;
	int    intkey_len;
	int    charkey_len;
	int    subkey_num;
	//int    hash_sum;     //应该不需要
	//int    hash_prime;
	//int    hash_count;
	struct SUBKEY_STRU subkey[MAX_SUBKEY_NUMBER];
};

/*
struct LOCK_ID   //处理读锁
{
	int       lock_id;  //与lock_num一致
	int       first_record_pointer;  //for table's record 
	int       last_record_pointer;
};*/

/* Database describe struct table */
struct STDB_TAB
{
	char		    byte_tag;          //must use type cast from  DB_BYTE_T // 2004-07-07
	int                 record_size;                 //record size
	int                 phy_record_size;             //physical record size with head
	int                 record_number;			//record number in table if the table is compact or the record number in main table if the table is imcompatible .
	int    	            record_sum;                  //record allow to created all
	short               field_sum;                   //field sum
	short	            field_no[MAX_TABLE_FIELD];   //field relation between rtdbms & [rdbms]

	struct KEYWORD_STRU key;
	struct LOCK_STRU   	lock;

	int                 index_count;
	int                 data_area_offset;
	int                 data_area_size;

	//
	//	int                 table_type;                  //for graph, etc
	unsigned char       is_auto_generated;
/******Added named fields begin *********/
	int named_fields_num;
	short named_field_no[20];//It stores STDB_FIELD_TAB::field_no , not STDB_FIELD_TAB::r_field_no, and it is ordered by STDB_FIELD_TAB::
/******Added named fields end***********/

/******Added for index begin ************/
	short index_num; //索引的数量
	char index_storage_type; //索引的存储类型，文件映射内存方式或者共享内存方式。
	int fields_used_in_index_num; //参加建立索引的域的数量，一个域被多个索引使用的话，field_used_in_index_num就要被加上几次。它要被用来描述FIELD_INDEX_DEFINE_TAB的大小和后面要提到的FIELDS_IN_INDEXS_DESC_TAB数组的大小
	int indexed_fields_num; //有多少个域被索引，一个域被多个索引使用，只是计算一次。
	int pk_storage_type; //主键索引存储方法，譬如共享内存或者文件映射内存
	int pk_mem_alloc_type;//主键内存分配方法，譬如带溢出HASH的分配方法；无溢出的HASH分配方法。
	char primary_key_name[ENG_NAME_LEN]; //主键索引文件名，给文件映射内存用。
	int primary_key_id; //主键索引共享内存ID，给共享内存使用。
	int pk_index_method; //主键索引方法，做成菜单，譬如直接定位、HASH等
	int pk_shm_size;//主键索引建立的共享内存的大小。
	int pk_index_length;//主键索引的长度，由于主键索引是紧凑排列的，所以它可能会比key.key_length短。
	int pk_index_status;//主键索引的状态
	time_t download_time; //从商用库下装的时间。这部分内容还不能仅仅放到COdbTable里面去.因为确定表文件是否是机器reboot后产生的，必须要有这个时间作为比较。
	char version[10];//预留，如果将来实时库升级导致表头发生变化，需要往version里面填信息。
	char reserved[20]; //预留
/******Added for index end**************/

/******Added for overflow table bein ***/
	int ext_record_number;//对于带空洞的表，这个成员变量表示溢出表体的记录个数	
	int area_num;
/******Added for overflow table end ***/
	//	unsigned char       is_system_table;             //Added 2003-09-01
	//	unsigned char       is_record_app;
	//	unsigned char       is_record_lock;
	//	unsigned char       is_record_resp;

	//	int                 reserved_1;
	//	int                 reserved_2;
	//	int                 reserved_3;
	//	int                 reserved_4;
	//	int                 reserved_5;
};

/* Database fields describe table */
struct STDB_FIELD_TAB
{
	short               field_no;  //rtdbms
	short               r_field_no;
	short               field_id;
	short               column_id;
	short               field_length;
	short               check_tag;  //
	int                 offset;     //data offset from data pointer, after RECORD_HEAD_STRU of this record

	int                 column_special;
	int                 ref_tableno;
	short               ref_fieldno;

	unsigned char       data_type;
	unsigned char       is_keyword;
	unsigned char       is_index;
	unsigned char       allow_null;
	unsigned char       sort_order_no;

	unsigned char       display_type;
	unsigned char       reference_flag;
	unsigned char       reference_mode;
	unsigned char       reference_display;

	unsigned char       is_app_syn;
	unsigned char       auto_meas_type;

	char                field_name_eng[ENG_NAME_LEN]
	;
	char                field_name_chn[CHN_NAME_LEN];

	char                menu_name[MENU_NAME_LEN];
	char                default_asciival[DEFAULT_VALUE_LEN];
	char                min_asciival[DEFAULT_VALUE_LEN];
	char                max_asciival[DEFAULT_VALUE_LEN];
/******Added for index begin ************/
	int 		in_field_index_pos;		//表示在前面描述的FIELD_INDEX_DEFINE_TAB数组中，该域出现的第一个位置。
	int 		in_index_num; 	// 表示几个索引使用了该域。也就是说FIELD_INDEX_DEFINE_TAB数组中，该域出现了多少次。
/******Added for index end *************/

	int                 search_attribute;   //  检索器的域特性           //Added 2003-09-01
	int                 statics_attribute;  //  统计的域特性
	int                 reserved_1;         //  系统保留
	int                 reserved_2;         //  系统保留
	int                 reserved_3;         //  系统保留
	int                 reserved_4;         //  系统保留
	int                 reserved_5;         //  系统保留
};


/* sequent area ctrl tab */ //排序
struct SEQUENT_TAB
{
	int  seq_field_no;  //指商用库的字段号
	int  seq_field_data_type;
	int  seq_field_offset;
	int  seq_area_offset;
	int  seq_area_size;
};

/*----------------------------------------------------------------------------*/



/* record head struct */
struct RECORD_HEAD_STRU
{
	unsigned    char exist_tag;
	unsigned    char select_tag;
	short       next_record_pointer;
};
/* ----------------------------------------------------------------------- */



/* ---------------------------------------------------------------------------*/

struct	DATA_FILE_HEAD
{
	int	   db_size;
	int	   record_number;
	int	   record_total_number;
	int	   record_valid_sum;
	int	   record_sum;
	int	   hash_sum;
	int	   hash_prime;
	int	   data_phy_record_sum;
	int	   data_phy_record_num;
	int    data_area_size;
};


/* ---------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------*/
struct MEMBER_DEFINITION
{
	int data_index;   //maybe only used bu sql
	int data_type;
	int data_size;
	//int data_space;
	int data_offset;
};



/* ---------------------------------------------------------------------------*/
//  2002-10-18, for read sys file
struct NAME_ID
{
	char name[CHN_NAME_LEN];     //  2003-09-15 //may affect others
	int  no;
};

#if 0

class SetSort
{
public:
	bool operator()(const NAME_ID& nl, const NAME_ID& nr)
	{
		if( nl.no < nr.no )
		{
			return true;
		}
		else if( nl.no == nr.no )
		{
			if( strcmp(nl.name, nr.name) < 0 )
			{
				return true;
			}
		}

		return false;
	}
};
#endif

struct APP_ID
{
	char app_name[MAX_NAME_STRING_LEN];
	std::vector<NAME_ID> vec_name_no;
};

/* ---------------------------------------------------------------------------*/
typedef std::map<int,std::string> MAPTABLE_T;
typedef std::map<int,std::string>::value_type VALTABLE_T;
typedef std::map<std::string,MAPTABLE_T> MAPAPP_T;
typedef std::map<std::string,MAPTABLE_T>::value_type VALAPP_T;


/* ---------------------------------------------------------------------------*/
//  2002-10-31, for sql query
/*struct SQL_EXPRESS
{
	struct NAME_ID field;
	DATA_TYPES_T   field_type;
	char*          field_value;
	
	CONDITION_OPERATOR_T cond_op;

	union 
	{ 
		double cond_value;  //条件值
		string cond_str;    //条件串
	};
	
	bool value;            //条件表达式的值
};


struct SQL_NODE
{
	LOGICAL_OPERATOR_T log_op; //逻辑操作
	bool value;                //逻辑表达式的值
	int express_id;            //对应叶接点下标
	struct NODE* left;
	struct NODE* right;
};
*/

/* ---------------------------------------------------------------------------*/
/*---sql查询返回结果 ---*/
struct SQL_RESPONSE
{
	int ret_code;      /* 返回的状态 (0: 成功; 其余:失败原因) */

	int field_num;     /* 返回的域个数 */
	struct FIELD_INFO* fldinfo;

	int record_num;    /* 返回的记录个数 */
	int record_length;
	int* record_no;    /* 返回的记录下标 */
	char* val_buf;	   /* 返回的值   */
};


struct FIELD_INFO
{
	char		  field_name[ENG_NAME_LEN];
	int  		  field_length;
	unsigned char data_type;
};

struct SQL_SORT
{
	struct NAME_ID field;
	bool is_asc;
};


/* ---------------------------------------------------------------------------*/
//app_no-->r_table_no-->table_name_eng----table_no 实时库表号
//typedef std::map<int, NAME_ID> MAP_NO2NAMEID;
//typedef std::map<int, MAP_NO2NAMEID> MAP_APP2NO;   //app_no to r_table_no


/*----------------------------------------------------------------------------*/


struct KEY_TYPE
{
	enum { MAX = 10 };                 //缺省field最大个数

	union SORT_DATA_TYPE fld_key[MAX]; //各个子field的值

	short fld_type[MAX];               //各field数据类型
	int   key_num;                     //实际field个数

	bool operator>= (const KEY_TYPE& K)  const
	{
		int iSize = key_num;
		for(int i = 0; i < iSize; ++i)
		{
			switch(fld_type[i])
			{
			case C_INT_TYPE:
				{
					if(fld_key[i].to_int == K.fld_key[i].to_int)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_int > K.fld_key[i].to_int);
					}
				}
			case C_STRING_TYPE:
				{
					int result = strcmp(fld_key[i].to_str, K.fld_key[i].to_str);
					if(result == 0)
					{
						continue;
					}
					else if(result > 0)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			case C_FLOAT_TYPE:
				{
					if(fld_key[i].to_float == K.fld_key[i].to_float)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_float > K.fld_key[i].to_float);
					}
				}
			case C_DOUBLE_TYPE:
				{
					if(fld_key[i].to_double == K.fld_key[i].to_double)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_double > K.fld_key[i].to_double);
					}
				}
			case C_KEYID_TYPE:
				{
#if 0
					if(fld_key[i].to_key == K.fld_key[i].to_key)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_key > K.fld_key[i].to_key);
					}
#else
					short table_no,table_no_K;
					short field_id,field_id_K;
					int key,key_K;

#if !defined(BIGENDIAN)

					memcpy((void *)&table_no,(char *)&fld_key[i].to_key+sizeof(int)+sizeof(short),sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_key+sizeof(int)+sizeof(short),sizeof(short));
#else

					memcpy((void *)&table_no,(char *)&fld_key[i].to_key,sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_key,sizeof(short));
#endif

					if(table_no > table_no_K)
						return true;

					if(table_no < table_no_K)
						return false;

#if !defined(BIGENDIAN)

					memcpy((void *)&key,(char *)&fld_key[i].to_key,sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_key,sizeof(int));
#else

					memcpy((void *)&key,(char *)&fld_key[i].to_key+sizeof(short)+sizeof(short),sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_key+sizeof(short)+sizeof(short),sizeof(int));
#endif

					if(key > key_K)
						return true;
					if(key< key_K )
						return false;

#if !defined(BIGENDIAN)

					memcpy((void *)&field_id,(char *)&fld_key[i].to_key+sizeof(int),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_key+sizeof(int),sizeof(short));
#else

					memcpy((void *)&field_id,(char *)&fld_key[i].to_key+sizeof(short),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_key+sizeof(short),sizeof(short));
#endif

					if(field_id > field_id_K)
						return true;
					if(field_id < field_id_K)
						return false;

					continue;

#endif

				}
			case C_APPKEY_TYPE:
				{
#if 0
					if(fld_key[i].to_appkey == K.fld_key[i].to_appkey)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_appkey > K.fld_key[i].to_appkey);
					}
#else
					if(fld_key[i].to_appkey.app_id> K.fld_key[i].to_appkey.app_id)
						return true;
					if(fld_key[i].to_appkey.app_id< K.fld_key[i].to_appkey.app_id)
						return false;

					short table_no,table_no_K;
					short field_id,field_id_K;
					int key,key_K;

#if !defined(BIGENDIAN)

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appkey.key_id+sizeof(int)+sizeof(short),sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(int)+sizeof(short),sizeof(short));
#else

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appkey.key_id,sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appkey.key_id,sizeof(short));
#endif

					if(table_no > table_no_K)
						return true;

					if(table_no < table_no_K)
						return false;

#if !defined(BIGENDIAN)

					memcpy((void *)&key,(char *)&fld_key[i].to_appkey.key_id,sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appkey.key_id,sizeof(int));
#else

					memcpy((void *)&key,(char *)&fld_key[i].to_appkey.key_id+sizeof(short)+sizeof(short),sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(short)+sizeof(short),sizeof(int));
#endif

					if(key > key_K)
						return true;
					if(key< key_K )
						return false;

#if !defined(BIGENDIAN)

					memcpy((void *)&field_id,(char *)&fld_key[i].to_appkey.key_id+sizeof(int),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(int),sizeof(short));
#else

					memcpy((void *)&field_id,(char *)&fld_key[i].to_appkey.key_id+sizeof(short),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(short),sizeof(short));
#endif

					if(field_id > field_id_K)
						return true;
					if(field_id < field_id_K)
						return false;

					continue;
#endif

				}
			case C_APPID_TYPE:
				{
#if 0
					if(fld_key[i].to_appid == K.fld_key[i].to_appid)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_appid > K.fld_key[i].to_appid);
					}
#else
					if(fld_key[i].to_appkey.app_id> K.fld_key[i].to_appid.app_id)
						return true;
					if(fld_key[i].to_appkey.app_id< K.fld_key[i].to_appid.app_id)
						return false;

					short table_no,table_no_K;
					int key,key_K;

#if !defined(BIGENDIAN)

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appid.key_id+sizeof(int)+sizeof(short),sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appid.key_id+sizeof(int)+sizeof(short),sizeof(short));
#else

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appid.key_id,sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appid.key_id,sizeof(short));
#endif

					if(table_no > table_no_K)
						return true;
					if(table_no < table_no_K)
						return false;

#if !defined(BIGENDIAN)

					memcpy((void *)&key,(char *)&fld_key[i].to_appid.key_id,sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appid.key_id,sizeof(int));
#else

					memcpy((void *)&key,(char *)&fld_key[i].to_appid.key_id+sizeof(short)+sizeof(short),sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appid.key_id+sizeof(short)+sizeof(short),sizeof(int));
#endif

					if(key > key_K)
						return true;
					if(key< key_K )
						return false;

					continue;
#endif

				}
			case C_LONG_TYPE:
				short table_no,table_no_K;
				int key,key_K;

#if !defined(BIGENDIAN)

				memcpy((void *)&table_no,(char *)&fld_key[i].to_long+sizeof(int)+sizeof(short),sizeof(short));
				memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_long+sizeof(int)+sizeof(short),sizeof(short));
#else

				memcpy((void *)&table_no,(char *)&fld_key[i].to_long,sizeof(short));
				memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_long,sizeof(short));
#endif

				if(table_no > table_no_K)
					return true;
				if(table_no < table_no_K)
					return false;

#if !defined(BIGENDIAN)

				memcpy((void *)&key,(char *)&fld_key[i].to_long,sizeof(int));
				memcpy((void *)&key_K,(char *)&K.fld_key[i].to_long,sizeof(int));
#else

				memcpy((void *)&key,(char *)&fld_key[i].to_long+sizeof(short)+sizeof(short),sizeof(int));
				memcpy((void *)&key_K,(char *)&K.fld_key[i].to_long+sizeof(short)+sizeof(short),sizeof(int));
#endif

				if(key > key_K)
					return true;
				if(key< key_K )
					return false;

				continue;

				break;
			default:
				break;
			}//end switch
		} // end for i

		return true;
	}

	bool operator<= (const KEY_TYPE& K)  const
	{
		int iSize = key_num;
		for(int i = 0; i < iSize; ++i)
		{
			switch(fld_type[i])
			{
			case C_INT_TYPE:
				{
					if(fld_key[i].to_int == K.fld_key[i].to_int)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_int < K.fld_key[i].to_int);
					}
				}
			case C_STRING_TYPE:
				{
					int result = strcmp(fld_key[i].to_str, K.fld_key[i].to_str);
					if(result == 0)
					{
						continue;
					}
					else if(result < 0)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			case C_FLOAT_TYPE:
				{
					if(fld_key[i].to_float == K.fld_key[i].to_float)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_float < K.fld_key[i].to_float);
					}
				}
			case C_DOUBLE_TYPE:
				{
					if(fld_key[i].to_double == K.fld_key[i].to_double)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_double < K.fld_key[i].to_double);
					}
				}
			case C_KEYID_TYPE:
				{
#if 0
					if( fld_key[i].to_key == K.fld_key[i].to_key )
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_key < K.fld_key[i].to_key);
					}
#else
					short table_no,table_no_K;
					short field_id,field_id_K;
					int key,key_K;

#if !defined(BIGENDIAN)

					memcpy((void *)&table_no,(char *)&fld_key[i].to_key+sizeof(int)+sizeof(short),sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_key+sizeof(int)+sizeof(short),sizeof(short));
#else

					memcpy((void *)&table_no,(char *)&fld_key[i].to_key,sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_key,sizeof(short));
#endif

					if(table_no > table_no_K)
						return false;

					if(table_no < table_no_K)
						return true;

#if !defined(BIGENDIAN)

					memcpy((void *)&key,(char *)&fld_key[i].to_key,sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_key,sizeof(int));
#else

					memcpy((void *)&key,(char *)&fld_key[i].to_key+sizeof(short)+sizeof(short),sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_key+sizeof(short)+sizeof(short),sizeof(int));
#endif

					if(key > key_K)
						return false;
					if(key< key_K )
						return true;

#if !defined(BIGENDIAN)

					memcpy((void *)&field_id,(char *)&fld_key[i].to_key+sizeof(int),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_key+sizeof(int),sizeof(short));
#else

					memcpy((void *)&field_id,(char *)&fld_key[i].to_key+sizeof(short),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_key+sizeof(short),sizeof(short));
#endif

					if(field_id > field_id_K)
						return false;
					if(field_id < field_id_K)
						return true;

					continue;
#endif

				}
			case C_APPKEY_TYPE:
				{
#if 0
					if(fld_key[i].to_appkey == K.fld_key[i].to_appkey)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_appkey < K.fld_key[i].to_appkey);
					}
#else
					if(fld_key[i].to_appkey.app_id> K.fld_key[i].to_appkey.app_id)
						return false;
					if(fld_key[i].to_appkey.app_id< K.fld_key[i].to_appkey.app_id)
						return true;

					short table_no,table_no_K;
					short field_id,field_id_K;
					int key,key_K;

#if !defined(BIGENDIAN)

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appkey.key_id+sizeof(int)+sizeof(short),sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(int)+sizeof(short),sizeof(short));
#else

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appkey.key_id,sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appkey.key_id,sizeof(short));
#endif

					if(table_no > table_no_K)
						return false;

					if(table_no < table_no_K)
						return true;

#if !defined(BIGENDIAN)

					memcpy((void *)&key,(char *)&fld_key[i].to_appkey.key_id,sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appkey.key_id,sizeof(int));
#else

					memcpy((void *)&key,(char *)&fld_key[i].to_appkey.key_id+sizeof(short)+sizeof(short),sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(short)+sizeof(short),sizeof(int));
#endif

					if(key > key_K)
						return false;
					if(key< key_K )
						return true;

#if !defined(BIGENDIAN)

					memcpy((void *)&field_id,(char *)&fld_key[i].to_appkey.key_id+sizeof(int),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(int),sizeof(short));
#else

					memcpy((void *)&field_id,(char *)&fld_key[i].to_appkey.key_id+sizeof(short),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(short),sizeof(short));
#endif

					if(field_id > field_id_K)
						return false;
					if(field_id < field_id_K)
						return true;

					continue;
#endif

				}
			case C_APPID_TYPE:
				{
#if 0
					if(fld_key[i].to_appid == K.fld_key[i].to_appid)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_appid < K.fld_key[i].to_appid);
					}
#else
					if(fld_key[i].to_appkey.app_id> K.fld_key[i].to_appid.app_id)
						return false;
					if(fld_key[i].to_appkey.app_id< K.fld_key[i].to_appid.app_id)
						return true;

					short table_no,table_no_K;
					int key,key_K;

#if !defined(BIGENDIAN)

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appid.key_id+sizeof(int)+sizeof(short),sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appid.key_id+sizeof(int)+sizeof(short),sizeof(short));
#else

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appid.key_id,sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appid.key_id,sizeof(short));
#endif

					if(table_no > table_no_K)
						return false;
					if(table_no < table_no_K)
						return true;

#if !defined(BIGENDIAN)

					memcpy((void *)&key,(char *)&fld_key[i].to_appid.key_id,sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appid.key_id,sizeof(int));
#else

					memcpy((void *)&key,(char *)&fld_key[i].to_appid.key_id+sizeof(short)+sizeof(short),sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appid.key_id+sizeof(short)+sizeof(short),sizeof(int));
#endif

					if(key > key_K)
						return false;
					if(key< key_K )
						return true;

					continue;
#endif

				}
			case C_LONG_TYPE:
				{
					short table_no,table_no_K;
					int key,key_K;

#if !defined(BIGENDIAN)

					memcpy((void *)&table_no,(char *)&fld_key[i].to_long+sizeof(int)+sizeof(short),sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_long+sizeof(int)+sizeof(short),sizeof(short));
#else

					memcpy((void *)&table_no,(char *)&fld_key[i].to_long,sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_long,sizeof(short));
#endif

					if(table_no > table_no_K)
						return false;
					if(table_no < table_no_K)
						return true;

#if !defined(BIGENDIAN)

					memcpy((void *)&key,(char *)&fld_key[i].to_long,sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_long,sizeof(int));
#else

					memcpy((void *)&key,(char *)&fld_key[i].to_long+sizeof(short)+sizeof(short),sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_long+sizeof(short)+sizeof(short),sizeof(int));
#endif

					if(key > key_K)
						return false;
					if(key< key_K )
						return true;

					continue;

					break;
				}
			default:
				break;
			}//end switch
		} // end for i

		return true;
	}

	bool operator > (const KEY_TYPE& K) const
	{

		int iSize = key_num;

		for(int i = 0; i < iSize; ++i)
		{
			switch(fld_type[i])
			{
			case C_INT_TYPE:
				{

					if(fld_key[i].to_int == K.fld_key[i].to_int)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_int > K.fld_key[i].to_int);
					}

				}
			case C_STRING_TYPE:
				{
					int result = strcmp(fld_key[i].to_str, K.fld_key[i].to_str);

					if(result == 0)
					{
						continue;
					}
					else if(result > 0)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			case C_FLOAT_TYPE:
				{
					if(fld_key[i].to_float == K.fld_key[i].to_float)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_float > K.fld_key[i].to_float);
					}

				}
			case C_DOUBLE_TYPE:
				{
					if(fld_key[i].to_double == K.fld_key[i].to_double)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_double > K.fld_key[i].to_double);
					}
				}
			case C_KEYID_TYPE:
				{
#if 0
					if( fld_key[i].to_key == K.fld_key[i].to_key )
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_key > K.fld_key[i].to_key);
					}
#else
					short table_no,table_no_K;
					short field_id,field_id_K;
					int key,key_K;

#if !defined(BIGENDIAN)

					memcpy((void *)&table_no,(char *)&fld_key[i].to_key+sizeof(int)+sizeof(short),sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_key+sizeof(int)+sizeof(short),sizeof(short));
#else

					memcpy((void *)&table_no,(char *)&fld_key[i].to_key,sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_key,sizeof(short));
#endif

					if(table_no > table_no_K)
						return true;

					if(table_no < table_no_K)
						return false;

#if !defined(BIGENDIAN)

					memcpy((void *)&key,(char *)&fld_key[i].to_key,sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_key,sizeof(int));
#else

					memcpy((void *)&key,(char *)&fld_key[i].to_key+sizeof(short)+sizeof(short),sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_key+sizeof(short)+sizeof(short),sizeof(int));
#endif

					if(key > key_K)
						return true;
					if(key< key_K )
						return false;

#if !defined(BIGENDIAN)

					memcpy((void *)&field_id,(char *)&fld_key[i].to_key+sizeof(int),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_key+sizeof(int),sizeof(short));
#else

					memcpy((void *)&field_id,(char *)&fld_key[i].to_key+sizeof(short),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_key+sizeof(short),sizeof(short));
#endif

					if(field_id > field_id_K)
						return true;
					if(field_id < field_id_K)
						return false;

					continue;
#endif

				}
			case C_APPKEY_TYPE:
				{
#if 0
					if(fld_key[i].to_appkey == K.fld_key[i].to_appkey)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_appkey > K.fld_key[i].to_appkey);
					}
#else
					if(fld_key[i].to_appkey.app_id> K.fld_key[i].to_appkey.app_id)
						return true;
					if(fld_key[i].to_appkey.app_id< K.fld_key[i].to_appkey.app_id)
						return false;

					short table_no,table_no_K;
					short field_id,field_id_K;
					int key,key_K;

#if !defined(BIGENDIAN)

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appkey.key_id+sizeof(int)+sizeof(short),sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(int)+sizeof(short),sizeof(short));
#else

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appkey.key_id,sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appkey.key_id,sizeof(short));
#endif

					if(table_no > table_no_K)
						return true;

					if(table_no < table_no_K)
						return false;

#if !defined(BIGENDIAN)

					memcpy((void *)&key,(char *)&fld_key[i].to_appkey.key_id,sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appkey.key_id,sizeof(int));
#else

					memcpy((void *)&key,(char *)&fld_key[i].to_appkey.key_id+sizeof(short)+sizeof(short),sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(short)+sizeof(short),sizeof(int));
#endif

					if(key > key_K)
						return true;
					if(key< key_K )
						return false;

#if !defined(BIGENDIAN)

					memcpy((void *)&field_id,(char *)&fld_key[i].to_appkey.key_id+sizeof(int),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(int),sizeof(short));
#else

					memcpy((void *)&field_id,(char *)&fld_key[i].to_appkey.key_id+sizeof(short),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(short),sizeof(short));
#endif

					if(field_id > field_id_K)
						return true;
					if(field_id < field_id_K)
						return false;

					continue;

#endif

				}
			case C_APPID_TYPE:
				{
#if 0
					if(fld_key[i].to_appid == K.fld_key[i].to_appid)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_appid > K.fld_key[i].to_appid);
					}
#else
					if(fld_key[i].to_appkey.app_id> K.fld_key[i].to_appid.app_id)
						return true;
					if(fld_key[i].to_appkey.app_id< K.fld_key[i].to_appid.app_id)
						return false;

					short table_no,table_no_K;
					int key,key_K;

#if !defined(BIGENDIAN)

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appid.key_id+sizeof(int)+sizeof(short),sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appid.key_id+sizeof(int)+sizeof(short),sizeof(short));
#else

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appid.key_id,sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appid.key_id,sizeof(short));
#endif

					if(table_no > table_no_K)
						return true;
					if(table_no < table_no_K)
						return false;

#if !defined(BIGENDIAN)

					memcpy((void *)&key,(char *)&fld_key[i].to_appid.key_id,sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appid.key_id,sizeof(int));
#else

					memcpy((void *)&key,(char *)&fld_key[i].to_appid.key_id+sizeof(short)+sizeof(short),sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appid.key_id+sizeof(short)+sizeof(short),sizeof(int));
#endif

					if(key > key_K)
						return true;
					if(key< key_K )
						return false;

					continue;

#endif

				}
			case C_LONG_TYPE:
				short table_no,table_no_K;
				int key,key_K;

#if !defined(BIGENDIAN)

				memcpy((void *)&table_no,(char *)&fld_key[i].to_long+sizeof(int)+sizeof(short),sizeof(short));
				memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_long+sizeof(int)+sizeof(short),sizeof(short));
#else

				memcpy((void *)&table_no,(char *)&fld_key[i].to_long,sizeof(short));
				memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_long,sizeof(short));
#endif

				if(table_no > table_no_K)
					return true;
				if(table_no < table_no_K)
					return false;

#if !defined(BIGENDIAN)

				memcpy((void *)&key,(char *)&fld_key[i].to_long,sizeof(int));
				memcpy((void *)&key_K,(char *)&K.fld_key[i].to_long,sizeof(int));
#else

				memcpy((void *)&key,(char *)&fld_key[i].to_long+sizeof(short)+sizeof(short),sizeof(int));
				memcpy((void *)&key_K,(char *)&K.fld_key[i].to_long+sizeof(short)+sizeof(short),sizeof(int));
#endif

				if(key > key_K)
					return true;
				if(key< key_K )
					return false;

				continue;

				break;

			default:
				break;
			}//end switch
		} // end for i

		return true;
	}

	bool operator< (const KEY_TYPE& K) const
	{
		//static int sta_num = 0;

		int iSize = key_num;

		for(int i = 0; i < iSize; ++i)
		{
			switch(fld_type[i])
			{
			case C_INT_TYPE:
				{

					if(fld_key[i].to_int == K.fld_key[i].to_int)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_int < K.fld_key[i].to_int);
					}

				}
			case C_STRING_TYPE:
				{
					int result = strcmp(fld_key[i].to_str, K.fld_key[i].to_str);

					if(result == 0)
					{
						continue;
					}
					else if(result < 0)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			case C_FLOAT_TYPE:
				{
					if(fld_key[i].to_float == K.fld_key[i].to_float)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_float < K.fld_key[i].to_float);
					}

				}
			case C_DOUBLE_TYPE:
				{
					if(fld_key[i].to_double == K.fld_key[i].to_double)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_double < K.fld_key[i].to_double);
					}
				}
			case C_KEYID_TYPE:
				{
#if 0
					if( fld_key[i].to_key == K.fld_key[i].to_key )
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_key < K.fld_key[i].to_key);
					}
#else
					short table_no,table_no_K;
					short field_id,field_id_K;
					int key,key_K;

#if !defined(BIGENDIAN)

					memcpy((void *)&table_no,(char *)&fld_key[i].to_key+sizeof(int)+sizeof(short),sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_key+sizeof(int)+sizeof(short),sizeof(short));
#else

					memcpy((void *)&table_no,(char *)&fld_key[i].to_key,sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_key,sizeof(short));
#endif

					if(table_no > table_no_K)
						return false;

					if(table_no < table_no_K)
						return true;

#if !defined(BIGENDIAN)

					memcpy((void *)&key,(char *)&fld_key[i].to_key,sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_key,sizeof(int));
#else

					memcpy((void *)&key,(char *)&fld_key[i].to_key+sizeof(short)+sizeof(short),sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_key+sizeof(short)+sizeof(short),sizeof(int));
#endif

					if(key > key_K)
						return false;
					if(key< key_K )
						return true;

#if !defined(BIGENDIAN)

					memcpy((void *)&field_id,(char *)&fld_key[i].to_key+sizeof(int),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_key+sizeof(int),sizeof(short));
#else

					memcpy((void *)&field_id,(char *)&fld_key[i].to_key+sizeof(short),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_key+sizeof(short),sizeof(short));
#endif

					if(field_id > field_id_K)
						return false;
					if(field_id < field_id_K)
						return true;

					continue;

#endif

				}
			case C_APPKEY_TYPE:
				{
#if 0
					if(fld_key[i].to_appkey == K.fld_key[i].to_appkey)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_appkey < K.fld_key[i].to_appkey);
					}
#else
					if(fld_key[i].to_appkey.app_id> K.fld_key[i].to_appkey.app_id)
						return false;
					if(fld_key[i].to_appkey.app_id< K.fld_key[i].to_appkey.app_id)
						return true;

					short table_no,table_no_K;
					short field_id,field_id_K;
					int key,key_K;

#if !defined(BIGENDIAN)

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appkey.key_id+sizeof(int)+sizeof(short),sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(int)+sizeof(short),sizeof(short));
#else

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appkey.key_id,sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appkey.key_id,sizeof(short));
#endif

					if(table_no > table_no_K)
						return false;

					if(table_no < table_no_K)
						return true;

#if !defined(BIGENDIAN)

					memcpy((void *)&key,(char *)&fld_key[i].to_appkey.key_id,sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appkey.key_id,sizeof(int));
#else

					memcpy((void *)&key,(char *)&fld_key[i].to_appkey.key_id+sizeof(short)+sizeof(short),sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(short)+sizeof(short),sizeof(int));
#endif

					if(key > key_K)
						return false;
					if(key< key_K )
						return true;

#if !defined(BIGENDIAN)

					memcpy((void *)&field_id,(char *)&fld_key[i].to_appkey.key_id+sizeof(int),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(int),sizeof(short));
#else

					memcpy((void *)&field_id,(char *)&fld_key[i].to_appkey.key_id+sizeof(short),sizeof(short));
					memcpy((void *)&field_id_K,(char *)&K.fld_key[i].to_appkey.key_id+sizeof(short),sizeof(short));
#endif

					if(field_id > field_id_K)
						return false;
					if(field_id < field_id_K)
						return true;

					continue;

#endif

				}
			case C_APPID_TYPE:
				{
#if 0
					if(fld_key[i].to_appid == K.fld_key[i].to_appid)
					{
						continue;
					}
					else
					{
						return (fld_key[i].to_appid < K.fld_key[i].to_appid);
					}
#else
					if(fld_key[i].to_appkey.app_id> K.fld_key[i].to_appid.app_id)
						return false;
					if(fld_key[i].to_appkey.app_id< K.fld_key[i].to_appid.app_id)
						return true;

					short table_no,table_no_K;
					int key,key_K;

#if !defined(BIGENDIAN)

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appid.key_id+sizeof(int)+sizeof(short),sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appid.key_id+sizeof(int)+sizeof(short),sizeof(short));
#else

					memcpy((void *)&table_no,(char *)&fld_key[i].to_appid.key_id,sizeof(short));
					memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_appid.key_id,sizeof(short));
#endif

					if(table_no > table_no_K)
						return false;
					if(table_no < table_no_K)
						return true;

#if !defined(BIGENDIAN)

					memcpy((void *)&key,(char *)&fld_key[i].to_appid.key_id,sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appid.key_id,sizeof(int));
#else

					memcpy((void *)&key,(char *)&fld_key[i].to_appid.key_id+sizeof(short)+sizeof(short),sizeof(int));
					memcpy((void *)&key_K,(char *)&K.fld_key[i].to_appid.key_id+sizeof(short)+sizeof(short),sizeof(int));
#endif

					if(key > key_K)
						return false;
					if(key< key_K )
						return true;

					continue;

#endif

				}
			case C_LONG_TYPE:
				short table_no,table_no_K;
				int key,key_K;

#if !defined(BIGENDIAN)

				memcpy((void *)&table_no,(char *)&fld_key[i].to_long+sizeof(int)+sizeof(short),sizeof(short));
				memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_long+sizeof(int)+sizeof(short),sizeof(short));
#else

				memcpy((void *)&table_no,(char *)&fld_key[i].to_long,sizeof(short));
				memcpy((void *)&table_no_K,(char *)&K.fld_key[i].to_long,sizeof(short));
#endif

				if(table_no > table_no_K)
					return false;
				if(table_no < table_no_K)
					return true;

#if !defined(BIGENDIAN)

				memcpy((void *)&key,(char *)&fld_key[i].to_long,sizeof(int));
				memcpy((void *)&key_K,(char *)&K.fld_key[i].to_long,sizeof(int));
#else

				memcpy((void *)&key,(char *)&fld_key[i].to_long+sizeof(short)+sizeof(short),sizeof(int));
				memcpy((void *)&key_K,(char *)&K.fld_key[i].to_long+sizeof(short)+sizeof(short),sizeof(int));
#endif

				if(key > key_K)
					return false;
				if(key< key_K )
					return true;

				continue;

				break;

			default:
				break;
			}//end switch
		} // end for i

		return true;
	}
};

struct RCD_TYPE
{
	struct KEY_TYPE key;  //complex keyword
	int pos;              //原来位置index
	int value;            //数据

	struct RCD_TYPE& operator = (const struct RCD_TYPE& T)
	{
		if( this == &T)
		{
			return *this;
		}

		pos   = T.pos;
		value = T.value;
		key   = T.key;

		return *this;
	}

	bool operator >  (const struct RCD_TYPE& T) const
	{
		return key > T.key;
	}

	bool operator <  (const struct RCD_TYPE& T) const
	{
		return key < T.key;
	}
};


/*----------------------------------------------------------------------------*/
union semun
{
	int     val;
	struct semid_ds* buf;
	unsigned short* array;
};

/*----------------------------------------------------------------------------*/
union DATA_VALUE
{
	//char       to_str[40];
	char       to_char;
	char*      to_str;
	short      to_short;
	int        to_int;
	long       to_long;
	float      to_float;
	double     to_double;
	KEY_ID_STRU to_key;
	//binary
};

/*----------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////
struct MENU_INFO
{
	char          menu_name[ORIGIN_MENU_NAME_LEN];      /* 菜单名*/
	unsigned char menu_no;                       /* 序号*/
	int           actual_value;                  /* 实际值*/
	char          display_value[ORIGIN_MENU_NAME_LEN];  /* 显示值*/
	unsigned char menu_status;                   /*菜单状态 */
	char          menu_macro_name[50];
};


struct MENU_RELEVANT
{
	int          table_id;                       /* 表ID号 */
	short        column_id;                      /* 域ID号 */
	int          column_value;                   /* 域值 */
	short        r_column_id;                    /* 对应域ID号 */
	char         r_menu_name[ORIGIN_MENU_NAME_LEN];     /* 对应菜单名 */
	int          default_value;                  /* 默认值 */
};

struct MENU_STRING
{
	long  order_no;
	char menu_name[MENU_NAME_LEN];
};


struct ORDER_KEY_REQ
{
	int   order_no;
	long   key_id;
};

struct GRAPH_ORDER_KEY_REQ
{

	int   app_no;
	long   graph_id;
	std::vector<struct ORDER_KEY_REQ> vec_keyid;
};

struct ORDER_KEY_RSP
{
	int   order_no;
	bool  is_in_app;
};

struct GRAPH_ORDER_KEY_RSP
{
	long  graph_id;
	std::vector<struct ORDER_KEY_RSP> vec_app;
};
///////////////////////////////////////////////////////////////////////////////

/******** download appended information begin ******************************/
struct INDEX_FIELD_DEFINE
{
	short field_no;
	int reserved[8];
};
typedef  std::vector< INDEX_FIELD_DEFINE > IndexFieldsArray;

struct INDEX_DEFINE
{
	//string index_name;//或者int index_no
	int table_no;//added for verification
	int index_no;
	int index_method;
	int first_hash_size;//针对双溢出索引，第一级溢出区的大小。其他索引不用这条信息。
	int storage_type;
	int mem_alloc_type;//索引内存分配方法，譬如带溢出HASH的分配方法；无溢出的HASH分配方法。
	int record_app;
	int reserved[8];
	IndexFieldsArray index_fields; //要求download_server通过select field_no from index_field_define where index_id = index_id and table_id = table_no and bitop(record_app,APP_NO) = APP_NO order by order_in_index 来做。
};

typedef std::vector<INDEX_DEFINE> INDEX_DEFINE_ARRAY;
/******** download End ********************************/


/********* index description in the table header begin*************/
struct INDEX_DEFINE_TAB
{
	//char index_name[FIELD_NAME_LEN]; //索引名称,或者改成int index_no
	int index_no;
	char file_name[4* ENG_NAME_LEN];//索引文件名称，给文件映射内存使用。
	int shm_id;//如果使用共享内存的话，需要这个信息。
	int index_shm_size; //该索引对应的共享内存的大小。
	char index_method; //索引排序方法， 譬如多种HASH算法，顺序等。
	int mem_alloc_type;//
	int storage_type;
	time_t create_time;//这个时间和索引头的时间一起判断索引缓存页是否过期。
	int index_desc_pos; //表示索引的第一个域在下面提到的FIELDS_IN_INDEXS_DESC_TAB的位置；
	int field_num; //表示该索引总共用了几个域。同样是为了定位改索引在FIELDS_IN_INDEXS_DESC_TAB中的长度。
	int index_length;//一条索引记录里面，索引占用的长度，等于所有索引域的长度之和。
	int index_rec_length; //一条索引记录的长度。需要向Index中插入信息时，同样需要
	int status;//索引的状态，譬如NOT_CREATED , NOT_INITIALIZED , INITIALIZING, OK，ABNORMAL, REPAIRING等状态,如果状态不为OK，那么取记录就不走索引。
	int first_hash_size;//为双溢出HASH索引而留，用于根据表生成索引。
	char reserved[20]; //预留
} ;//数组维数为STDB_TAB ::index_num

struct FIELDS_IN_INDEXS_DESC_TAB 
{
	short field_no;
	short data_offset; //注意，此处的data_offset是指这个域在索引里面的偏移量。需要注意的是索引中的每个域是紧凑排列的，所以第一个域的data_offset就是0，第二个域的data_offset就是第一个域的长度.....
	char reserved[20];
};//数组维数为STDB_TAB:: fields_used_in _index_num

struct FIELD_INDEX_DEFINE_TAB
{
		short field_no; //域号
		short index_no; //属于第几个索引，索引号从0 开始。
		short in_index_pos; //该域在该索引中属于第几个域，从0开始算位置。
		char reserved[20]; //预留
};//数组维数为STDB_TAB::fields_used_in _index_num

struct FIELDS_USED_IN_INDEX_TAB
{
	short field_no;
	char reserved[20]; //预留
};//数组维数为STDB_TAB :: indexed_fields_num
/********* index description  in the table header end**************/

/********* index head description begin***********************/
struct INDEX_STDB_TAB
{
	char index_name[ENG_NAME_LEN]; //索引名称
	int table_id; //为了校验打开的索引是正确的，增加这个域。
	int index_id;//为了校验打开的索引是正确的，增加这个域。
	int shm_id; //如果使用共享内存的话，需要这个信息
	int index_method;//索引排序方法， 譬如HASH，顺序等
	//int mem_alloc_type;//
	//int storage_type;
	time_t create_time;//这个时间和索引头的时间一起判断索引缓存页是否过期。
	int hash_prime; //如果为HASH算法的话，它是HASH因子。
	int first_hash_size; //针对双溢出HASH的索引区和第一级溢出区的HASH因子，来自索引定义表中的first_hash_size域，如果索引定义表中的first_hash_size域为0的话，就赋值为hash_prime。
	int first_hash_prime;//根据first_hash_size算出来的hash因子，譬如就等于first_hash_size,或者不大于first_hash_size的最大整数。
	int table_record_sum;
	int table_record_num;//HASH索引可能用不上
	int index_record_sum;
	int phy_record_size; //一条完全索引记录的长度
	int phy_record_size_pri_overflow; //一条完全主溢出区记录的长度
	int phy_record_size_overflow;//一条完全溢出区记录的长度
	int record_size;//一条有效记录的长度
	int index_length; //索引数据在一条索引记录中占用的长度
	int key_length;//主键数据在一条索引记录中占用的长度
	int index_offset; //在索引区的一条记录里面，索引数据相对记录头的偏移量。
	int index_offset_overflow; //在溢出区的一条记录里面，索引数据相对记录头的偏移量。
	int index_offset_pri_overflow; //在主溢出区的一条记录里面，索引数据相对记录头的偏移量。
	int key_offset;//在索引区的一条记录里面，主键数据相对记录头的偏移量。
	int key_offset_overflow;//在溢出区的一条记录里面，主键数据相对记录头的偏移量。
	int key_offset_pri_overflow;//在主溢出区的一条记录里面，主键数据相对记录头的偏移量。
	int key_max_assign;//对齐会用到
	int field_sum;
	//int data_area_offset;
	int stdb_index_hash_offset; //STDB_INDEX_HASH  结构相对索引共享内存首指针的偏移
	int free_array_offset; //溢出区空闲管理栈首址
	int index_storage_offset;//索引区 相对索引共享内存首指针的偏移
	int index_overflow_offset;//溢出区(或者二级溢出区)相对索引共享内存首指针的偏移
	int index_pri_overflow_offset;//一级溢出区相对索引共享内存首指针的偏移
	int data_area_size;//可能会没有用处
	struct KEYWORD_STRU key;
	DB_STATUS_T index_status;//表征索引的状态，
	char reserved[20]; //预留
};

struct INDEX_STDB_FIELD_TAB
{
	short offset; //这个域在索引记录中相对于第一个域的偏移量
	short data_type; //这个域的数据类型
	short data_length; //这个域的长度	
	char reserved[20]; //预留
};//数组维数为INDEX_STDB_TAB::field_sum
/********* index head description end************************/

/********* structure for hash index body  begin *********************/
struct STDB_INDEX_HASH
{
	//int overflow_record_size;//溢出区记录长度,在INDEX_STDB_TAB中已经定义，所以不再定义
	//int overflow_area_offset; //溢出区的偏离索引头的位置,在INDEX_STDB_TAB中已经定义，所以不再定义
	//int overflow_area_size; //溢出区的大小,在INDEX_STDB_TAB中已经定义，所以不再定义
	//int *free_array_ptr;//数组的维数是INDEX_STDB_TAB::table_record_sum,但是不能以指针的形式存在这里面。
	int free_pos;
	int storage_used_num;// INDEX_STORAGE_HASH区里面已经使用的记录的个数，需要注意的是，如果INDEX_STORAGE_HASH的记录已经被删掉（也就是used_flag == NOT_USED，但是它的后向指针不为空的话，那么这条记录仍然不算在storage_used_num里面。
	char reserved[20];
};
struct INDEX_STORAGE_HASH_REC_HEAD
{
	//char *index_ptr ;//长度为INDEX_STDB_TAB::index_length;
	//char *key_ptr;//长度为INDEX_STDB_TAB::key_length;
	int next_pointer;//冲突情况下，下一个指针的位置，相对于OVERFLOW_STORAGE 0号记录的偏移量
	int tail_pointer;//对应同一个HASH结果，最后一个位置的指针，方便快速插入。但是怎样杜绝在一个链子里面出现重复的现象？留意唯一性索引的要求.
	int same_hash_num;//相同HASH结果的关键字在溢出区的个数,注意不包含索引区的那个关键字。同时作为一个判断索引错误的判据。
	int used_flag; //使用标志
};//记录个数INDEX_STDB_TAB::hash_prime

struct OVERFLOW_STORAGE_HASH_REC_HEAD
{
	//char *index_ptr ;//长度为INDEX_STDB_TAB::key_length;
	//char *key_ptr;//长度为INDEX_STDB_TAB::key_length;
	int next_pointer;//冲突情况下，下一个指针的位置，相对于OVERFLOW_STORAGE首址的偏移量
	int prev_pointer;//前面一个位置的指针，如果最高2个bit是00的话，相对于OVERFLOW_STORAGE_HASH 0号记录的偏移量，	如果最高2个bit是10的话，则去掉这两个bit后所得的值是相对于INDEX_STORAGE_HASH的位置。
	//int used_flag; //使用标志，USED、NOT_USED,没有必要使用
};//记录个数为INDEX_STDB_TAB::table_record_sum

/********* sstructure for HASH index body  end* *********************/

/********* structure for sequence index body   begin *********************/
/********* structure for sequence index body end ***********************/

/********* structure for bi-level index body   begin *********************/
struct STDB_INDEX_BILEVEL
{
	int first_level_index_rec_sum; //怎样得来，通过引用表（譬如厂站表）的最大记录数来定还是通过在索引定义表中增加一个域来描述这个信息?
	int data_offset;
	int overflow_used_num;//溢出区使用的记录个数
	char reserved[20];//保留
};

struct FIRST_LEVEL_INDEX_REC_HEAD
{
	int begin_pos;
	int length;
	int used_flag; //USED or NOT_USED
};//记录个数为STDB_INDEX_BILEVEL::first_level_index_rec_sum

struct FIRST_LEVEL_INDEX_OVERFLOW_REC_HEAD
{
	int begin_pos;
	int length;
};//记录个数为STDB_INDEX_BILEVEL::first_level_index_rec_sum
/********* structure for bi-level index body end ***********************/

/********* structure for double-overflow hash  index body   begin *********************/
struct STDB_INDEX_HASH_DOVERFLOW
{
	//int overflow_record_size;//溢出区记录长度,在INDEX_STDB_TAB中已经定义，所以不再定义
	//int overflow_area_offset; //溢出区的偏离索引头的位置,在INDEX_STDB_TAB中已经定义，所以不再定义
	//int overflow_area_size; //溢出区的大小,在INDEX_STDB_TAB中已经定义，所以不再定义
	//int *prime_free_array_ptr; //数组维数为INDEX_STDB_TAB::table_record_sum],给主溢出区使用的空闲队列管理数组。
	int prime_free_pos;//
	//int *free_array_ptr;//数组维数为INDEX_STDB_TAB::table_record_sum
	int free_pos;
	int storage_used_num;// INDEX_STORAGE_HASH区里面已经使用的记录的个数，需要注意的是，如果INDEX_STORAGE_HASH的记录已经被删掉（也就是used_flag == NOT_USED，但是它的后向指针不为空的话，那么这条记录仍然不算在storage_used_num里面。
	char reserved[20];
};

struct INDEX_STORAGE_HASH_DOVERFLOW_REC_HEAD
{
	//char *index_ptr ;//长度为INDEX_STDB_TAB::index_length;
	//char *key_ptr;//长度为INDEX_STDB_TAB::key_length;
	int prime_next_pointer;//如果HASH算出来在这个位置和，但是index的值和这个位置index的值不相等的情况下，那么顺着这个链子在PRIMARY_OVERFLOW_STORAGE里面往下走，直到在PRIMARY_OVERFLOW_STORAGE_HASH找到一个index值不等的情况，或者找到一个空闲位置，具体算法后面描述。Prime_next_pointer是相对于PRIMARY_OVERFLOW_STORAGE 0号记录的偏移量。
	int next_pointer;//冲突情况下，而且index指针相等的情况下，在下一个指针的位置，相对于OVERFLOW_STORAGE 0号记录的偏移量
	int tail_pointer;//最后一个位置的指针，方便快速插入。
	int same_hash_num;//相同HASH结果的关键字的个数。同时作为一个判断索引错误的判据。
	int used_flag; //使用标志
};//记录个数为INDEX_STDB_TAB::first_hash_prime

struct PRIMARY_OVERFLOW_STORAGE_HASH_REC_HEAD
{
	//char *index_ptr ;//长度为INDEX_STDB_TAB::key_length;
	//char *key_ptr;//长度为INDEX_STDB_TAB::key_length;
	int prime_next_pointer;//在PRIMARY_OVERFLOW中index值相等的情况下，在OVERFLOW_STORAGE_HASH中下一个指针的位置，相对于PRIMARY_OVERFLOW_STORAGE首址的偏移量
	int prime_prev_pointer;//在PRIMARY_OVERFLOW中的前向指针。
	int next_pointer;// 在PRIMARY_OVERFLOW中index值不等的情况下冲突情况下，下一个指针的位置，相对于OVERFLOW_STORAGE首记录的偏移量
	int tail_pointer;//最后一个位置的指针，方便快速插入。
	int same_hash_num;//相同HASH结果的关键字的个数。同时作为一个判断索引错误的判据。
	int used_flag; //使用标志，USED、NOT_USED
};//记录个数为INDEX_STDB_TAB::first_hash_size

//同样需要struct OVERFLOW_STORAGE_HASH_REC_HEAD,记录个数为INDEX_STDB_TAB::table_record_sum，定义和HASH索引一样。

/********* structure for double-overflow hash  index body   begin *********************/

/********* structure for  hash  PK bodywithout overflow area     begin *********************/
//使用内部数据结构，不再定义
/********* structure for  hash  PK bodywithout overflow area     end *********************/

/********* structure for  hash PK   body   begin *********************/
//数据结构和HASH索引一模一样
/********* structure for  hash PK body   begin **********************/

}
#endif
