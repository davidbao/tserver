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
	short    			context_num;    //real context����
	short    			cur_context_no; //
	//	DB_BYTE_T   		byte_tag;

	struct LOCK_STRU   	lock;           //2002-10-23
};

/*PASӦ�ã��о�̬��CASE֧��*/
struct DB_CONTEXT_TAB
{
	short 				ctx_inner_no;
	short				context_no;
	char				context_name[MAX_NAME_STRING_LEN]
	;

	char 				file_app[MAX_APP_NUM][120];       //app_name + app_alias
	int				app_size[MAX_APP_NUM];            //file size  //2002-10-22
	//char* 				context_app_ptr[MAX_APP_NUM];     //��context��app�ṹָ��(ӳ�����ڴ��ַ)//  2002-12-20

	int   				app_num;                          //real app����
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
	char				app_alias[MAX_NAME_STRING_LEN]; //Ӧ�ñ���
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
	short			r_table_no;	//��Ӧ���ÿ�
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
	int          		open_count;              //���򿪻���ʴ���

	char         		db_file_name[120]; //table data file
	int          		db_size;           //table all len
	//char*        		db_ptr;            //��ӳ�����ڴ��ַ //���ݴ����жϱ��Ƿ�ɷ���, default is NULL//  2002-12-20

	int          		db_offset;             //?
	int          		db_describ_area_size;  //��des area��С(STDB_TAB+SEQUENT_TAB+STD_FILELD_TAB+������)
	int          		db_data_area_size;     //��data area��С
	int	    		reserved[64];
	int             area_num;
	int             area_no[MAX_AREA_NUM]; //����ţ��±����������
	int				r_area_no[MAX_AREA_NUM]; //�ڲ�����ţ����±�0��ʼ��¼һ�����˶��ٸ�����ÿ��������Ƕ���
	char            area_name[MAX_AREA_NUM][ENG_NAME_LEN]; //���������±��������ţ�ֵ����������
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
	//int    hash_sum;     //Ӧ�ò���Ҫ
	//int    hash_prime;
	//int    hash_count;
	struct SUBKEY_STRU subkey[MAX_SUBKEY_NUMBER];
};

/*
struct LOCK_ID   //�������
{
	int       lock_id;  //��lock_numһ��
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
	short index_num; //����������
	char index_storage_type; //�����Ĵ洢���ͣ��ļ�ӳ���ڴ淽ʽ���߹����ڴ淽ʽ��
	int fields_used_in_index_num; //�μӽ������������������һ���򱻶������ʹ�õĻ���field_used_in_index_num��Ҫ�����ϼ��Ρ���Ҫ����������FIELD_INDEX_DEFINE_TAB�Ĵ�С�ͺ���Ҫ�ᵽ��FIELDS_IN_INDEXS_DESC_TAB����Ĵ�С
	int indexed_fields_num; //�ж��ٸ���������һ���򱻶������ʹ�ã�ֻ�Ǽ���һ�Ρ�
	int pk_storage_type; //���������洢������Ʃ�繲���ڴ�����ļ�ӳ���ڴ�
	int pk_mem_alloc_type;//�����ڴ���䷽����Ʃ������HASH�ķ��䷽�����������HASH���䷽����
	char primary_key_name[ENG_NAME_LEN]; //���������ļ��������ļ�ӳ���ڴ��á�
	int primary_key_id; //�������������ڴ�ID���������ڴ�ʹ�á�
	int pk_index_method; //�����������������ɲ˵���Ʃ��ֱ�Ӷ�λ��HASH��
	int pk_shm_size;//�������������Ĺ����ڴ�Ĵ�С��
	int pk_index_length;//���������ĳ��ȣ��������������ǽ������еģ����������ܻ��key.key_length�̡�
	int pk_index_status;//����������״̬
	time_t download_time; //�����ÿ���װ��ʱ�䡣�ⲿ�����ݻ����ܽ����ŵ�COdbTable����ȥ.��Ϊȷ�����ļ��Ƿ��ǻ���reboot������ģ�����Ҫ�����ʱ����Ϊ�Ƚϡ�
	char version[10];//Ԥ�����������ʵʱ���������±�ͷ�����仯����Ҫ��version��������Ϣ��
	char reserved[20]; //Ԥ��
/******Added for index end**************/

/******Added for overflow table bein ***/
	int ext_record_number;//���ڴ��ն��ı������Ա������ʾ�������ļ�¼����	
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
	int 		in_field_index_pos;		//��ʾ��ǰ��������FIELD_INDEX_DEFINE_TAB�����У�������ֵĵ�һ��λ�á�
	int 		in_index_num; 	// ��ʾ��������ʹ���˸���Ҳ����˵FIELD_INDEX_DEFINE_TAB�����У���������˶��ٴΡ�
/******Added for index end *************/

	int                 search_attribute;   //  ��������������           //Added 2003-09-01
	int                 statics_attribute;  //  ͳ�Ƶ�������
	int                 reserved_1;         //  ϵͳ����
	int                 reserved_2;         //  ϵͳ����
	int                 reserved_3;         //  ϵͳ����
	int                 reserved_4;         //  ϵͳ����
	int                 reserved_5;         //  ϵͳ����
};


/* sequent area ctrl tab */ //����
struct SEQUENT_TAB
{
	int  seq_field_no;  //ָ���ÿ���ֶκ�
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
		double cond_value;  //����ֵ
		string cond_str;    //������
	};
	
	bool value;            //�������ʽ��ֵ
};


struct SQL_NODE
{
	LOGICAL_OPERATOR_T log_op; //�߼�����
	bool value;                //�߼����ʽ��ֵ
	int express_id;            //��ӦҶ�ӵ��±�
	struct NODE* left;
	struct NODE* right;
};
*/

/* ---------------------------------------------------------------------------*/
/*---sql��ѯ���ؽ�� ---*/
struct SQL_RESPONSE
{
	int ret_code;      /* ���ص�״̬ (0: �ɹ�; ����:ʧ��ԭ��) */

	int field_num;     /* ���ص������ */
	struct FIELD_INFO* fldinfo;

	int record_num;    /* ���صļ�¼���� */
	int record_length;
	int* record_no;    /* ���صļ�¼�±� */
	char* val_buf;	   /* ���ص�ֵ   */
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
//app_no-->r_table_no-->table_name_eng----table_no ʵʱ����
//typedef std::map<int, NAME_ID> MAP_NO2NAMEID;
//typedef std::map<int, MAP_NO2NAMEID> MAP_APP2NO;   //app_no to r_table_no


/*----------------------------------------------------------------------------*/


struct KEY_TYPE
{
	enum { MAX = 10 };                 //ȱʡfield������

	union SORT_DATA_TYPE fld_key[MAX]; //������field��ֵ

	short fld_type[MAX];               //��field��������
	int   key_num;                     //ʵ��field����

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
	int pos;              //ԭ��λ��index
	int value;            //����

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
	char          menu_name[ORIGIN_MENU_NAME_LEN];      /* �˵���*/
	unsigned char menu_no;                       /* ���*/
	int           actual_value;                  /* ʵ��ֵ*/
	char          display_value[ORIGIN_MENU_NAME_LEN];  /* ��ʾֵ*/
	unsigned char menu_status;                   /*�˵�״̬ */
	char          menu_macro_name[50];
};


struct MENU_RELEVANT
{
	int          table_id;                       /* ��ID�� */
	short        column_id;                      /* ��ID�� */
	int          column_value;                   /* ��ֵ */
	short        r_column_id;                    /* ��Ӧ��ID�� */
	char         r_menu_name[ORIGIN_MENU_NAME_LEN];     /* ��Ӧ�˵��� */
	int          default_value;                  /* Ĭ��ֵ */
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
	//string index_name;//����int index_no
	int table_no;//added for verification
	int index_no;
	int index_method;
	int first_hash_size;//���˫�����������һ��������Ĵ�С��������������������Ϣ��
	int storage_type;
	int mem_alloc_type;//�����ڴ���䷽����Ʃ������HASH�ķ��䷽�����������HASH���䷽����
	int record_app;
	int reserved[8];
	IndexFieldsArray index_fields; //Ҫ��download_serverͨ��select field_no from index_field_define where index_id = index_id and table_id = table_no and bitop(record_app,APP_NO) = APP_NO order by order_in_index ������
};

typedef std::vector<INDEX_DEFINE> INDEX_DEFINE_ARRAY;
/******** download End ********************************/


/********* index description in the table header begin*************/
struct INDEX_DEFINE_TAB
{
	//char index_name[FIELD_NAME_LEN]; //��������,���߸ĳ�int index_no
	int index_no;
	char file_name[4* ENG_NAME_LEN];//�����ļ����ƣ����ļ�ӳ���ڴ�ʹ�á�
	int shm_id;//���ʹ�ù����ڴ�Ļ�����Ҫ�����Ϣ��
	int index_shm_size; //��������Ӧ�Ĺ����ڴ�Ĵ�С��
	char index_method; //�������򷽷��� Ʃ�����HASH�㷨��˳��ȡ�
	int mem_alloc_type;//
	int storage_type;
	time_t create_time;//���ʱ�������ͷ��ʱ��һ���ж���������ҳ�Ƿ���ڡ�
	int index_desc_pos; //��ʾ�����ĵ�һ�����������ᵽ��FIELDS_IN_INDEXS_DESC_TAB��λ�ã�
	int field_num; //��ʾ�������ܹ����˼�����ͬ����Ϊ�˶�λ��������FIELDS_IN_INDEXS_DESC_TAB�еĳ��ȡ�
	int index_length;//һ��������¼���棬����ռ�õĳ��ȣ���������������ĳ���֮�͡�
	int index_rec_length; //һ��������¼�ĳ��ȡ���Ҫ��Index�в�����Ϣʱ��ͬ����Ҫ
	int status;//������״̬��Ʃ��NOT_CREATED , NOT_INITIALIZED , INITIALIZING, OK��ABNORMAL, REPAIRING��״̬,���״̬��ΪOK����ôȡ��¼�Ͳ���������
	int first_hash_size;//Ϊ˫���HASH�������������ڸ��ݱ�����������
	char reserved[20]; //Ԥ��
} ;//����ά��ΪSTDB_TAB ::index_num

struct FIELDS_IN_INDEXS_DESC_TAB 
{
	short field_no;
	short data_offset; //ע�⣬�˴���data_offset��ָ����������������ƫ��������Ҫע����������е�ÿ�����ǽ������еģ����Ե�һ�����data_offset����0���ڶ������data_offset���ǵ�һ����ĳ���.....
	char reserved[20];
};//����ά��ΪSTDB_TAB:: fields_used_in _index_num

struct FIELD_INDEX_DEFINE_TAB
{
		short field_no; //���
		short index_no; //���ڵڼ��������������Ŵ�0 ��ʼ��
		short in_index_pos; //�����ڸ����������ڵڼ����򣬴�0��ʼ��λ�á�
		char reserved[20]; //Ԥ��
};//����ά��ΪSTDB_TAB::fields_used_in _index_num

struct FIELDS_USED_IN_INDEX_TAB
{
	short field_no;
	char reserved[20]; //Ԥ��
};//����ά��ΪSTDB_TAB :: indexed_fields_num
/********* index description  in the table header end**************/

/********* index head description begin***********************/
struct INDEX_STDB_TAB
{
	char index_name[ENG_NAME_LEN]; //��������
	int table_id; //Ϊ��У��򿪵���������ȷ�ģ����������
	int index_id;//Ϊ��У��򿪵���������ȷ�ģ����������
	int shm_id; //���ʹ�ù����ڴ�Ļ�����Ҫ�����Ϣ
	int index_method;//�������򷽷��� Ʃ��HASH��˳���
	//int mem_alloc_type;//
	//int storage_type;
	time_t create_time;//���ʱ�������ͷ��ʱ��һ���ж���������ҳ�Ƿ���ڡ�
	int hash_prime; //���ΪHASH�㷨�Ļ�������HASH���ӡ�
	int first_hash_size; //���˫���HASH���������͵�һ���������HASH���ӣ���������������е�first_hash_size���������������е�first_hash_size��Ϊ0�Ļ����͸�ֵΪhash_prime��
	int first_hash_prime;//����first_hash_size�������hash���ӣ�Ʃ��͵���first_hash_size,���߲�����first_hash_size�����������
	int table_record_sum;
	int table_record_num;//HASH���������ò���
	int index_record_sum;
	int phy_record_size; //һ����ȫ������¼�ĳ���
	int phy_record_size_pri_overflow; //һ����ȫ���������¼�ĳ���
	int phy_record_size_overflow;//һ����ȫ�������¼�ĳ���
	int record_size;//һ����Ч��¼�ĳ���
	int index_length; //����������һ��������¼��ռ�õĳ���
	int key_length;//����������һ��������¼��ռ�õĳ���
	int index_offset; //����������һ����¼���棬����������Լ�¼ͷ��ƫ������
	int index_offset_overflow; //���������һ����¼���棬����������Լ�¼ͷ��ƫ������
	int index_offset_pri_overflow; //�����������һ����¼���棬����������Լ�¼ͷ��ƫ������
	int key_offset;//����������һ����¼���棬����������Լ�¼ͷ��ƫ������
	int key_offset_overflow;//���������һ����¼���棬����������Լ�¼ͷ��ƫ������
	int key_offset_pri_overflow;//�����������һ����¼���棬����������Լ�¼ͷ��ƫ������
	int key_max_assign;//������õ�
	int field_sum;
	//int data_area_offset;
	int stdb_index_hash_offset; //STDB_INDEX_HASH  �ṹ������������ڴ���ָ���ƫ��
	int free_array_offset; //��������й���ջ��ַ
	int index_storage_offset;//������ ������������ڴ���ָ���ƫ��
	int index_overflow_offset;//�����(���߶��������)������������ڴ���ָ���ƫ��
	int index_pri_overflow_offset;//һ�������������������ڴ���ָ���ƫ��
	int data_area_size;//���ܻ�û���ô�
	struct KEYWORD_STRU key;
	DB_STATUS_T index_status;//����������״̬��
	char reserved[20]; //Ԥ��
};

struct INDEX_STDB_FIELD_TAB
{
	short offset; //�������������¼������ڵ�һ�����ƫ����
	short data_type; //��������������
	short data_length; //�����ĳ���	
	char reserved[20]; //Ԥ��
};//����ά��ΪINDEX_STDB_TAB::field_sum
/********* index head description end************************/

/********* structure for hash index body  begin *********************/
struct STDB_INDEX_HASH
{
	//int overflow_record_size;//�������¼����,��INDEX_STDB_TAB���Ѿ����壬���Բ��ٶ���
	//int overflow_area_offset; //�������ƫ������ͷ��λ��,��INDEX_STDB_TAB���Ѿ����壬���Բ��ٶ���
	//int overflow_area_size; //������Ĵ�С,��INDEX_STDB_TAB���Ѿ����壬���Բ��ٶ���
	//int *free_array_ptr;//�����ά����INDEX_STDB_TAB::table_record_sum,���ǲ�����ָ�����ʽ���������档
	int free_pos;
	int storage_used_num;// INDEX_STORAGE_HASH�������Ѿ�ʹ�õļ�¼�ĸ�������Ҫע����ǣ����INDEX_STORAGE_HASH�ļ�¼�Ѿ���ɾ����Ҳ����used_flag == NOT_USED���������ĺ���ָ�벻Ϊ�յĻ�����ô������¼��Ȼ������storage_used_num���档
	char reserved[20];
};
struct INDEX_STORAGE_HASH_REC_HEAD
{
	//char *index_ptr ;//����ΪINDEX_STDB_TAB::index_length;
	//char *key_ptr;//����ΪINDEX_STDB_TAB::key_length;
	int next_pointer;//��ͻ����£���һ��ָ���λ�ã������OVERFLOW_STORAGE 0�ż�¼��ƫ����
	int tail_pointer;//��Ӧͬһ��HASH��������һ��λ�õ�ָ�룬������ٲ��롣���������ž���һ��������������ظ�����������Ψһ��������Ҫ��.
	int same_hash_num;//��ͬHASH����Ĺؼ�����������ĸ���,ע�ⲻ�������������Ǹ��ؼ��֡�ͬʱ��Ϊһ���ж�����������оݡ�
	int used_flag; //ʹ�ñ�־
};//��¼����INDEX_STDB_TAB::hash_prime

struct OVERFLOW_STORAGE_HASH_REC_HEAD
{
	//char *index_ptr ;//����ΪINDEX_STDB_TAB::key_length;
	//char *key_ptr;//����ΪINDEX_STDB_TAB::key_length;
	int next_pointer;//��ͻ����£���һ��ָ���λ�ã������OVERFLOW_STORAGE��ַ��ƫ����
	int prev_pointer;//ǰ��һ��λ�õ�ָ�룬������2��bit��00�Ļ��������OVERFLOW_STORAGE_HASH 0�ż�¼��ƫ������	������2��bit��10�Ļ�����ȥ��������bit�����õ�ֵ�������INDEX_STORAGE_HASH��λ�á�
	//int used_flag; //ʹ�ñ�־��USED��NOT_USED,û�б�Ҫʹ��
};//��¼����ΪINDEX_STDB_TAB::table_record_sum

/********* sstructure for HASH index body  end* *********************/

/********* structure for sequence index body   begin *********************/
/********* structure for sequence index body end ***********************/

/********* structure for bi-level index body   begin *********************/
struct STDB_INDEX_BILEVEL
{
	int first_level_index_rec_sum; //����������ͨ�����ñ�Ʃ�糧վ��������¼����������ͨ�������������������һ���������������Ϣ?
	int data_offset;
	int overflow_used_num;//�����ʹ�õļ�¼����
	char reserved[20];//����
};

struct FIRST_LEVEL_INDEX_REC_HEAD
{
	int begin_pos;
	int length;
	int used_flag; //USED or NOT_USED
};//��¼����ΪSTDB_INDEX_BILEVEL::first_level_index_rec_sum

struct FIRST_LEVEL_INDEX_OVERFLOW_REC_HEAD
{
	int begin_pos;
	int length;
};//��¼����ΪSTDB_INDEX_BILEVEL::first_level_index_rec_sum
/********* structure for bi-level index body end ***********************/

/********* structure for double-overflow hash  index body   begin *********************/
struct STDB_INDEX_HASH_DOVERFLOW
{
	//int overflow_record_size;//�������¼����,��INDEX_STDB_TAB���Ѿ����壬���Բ��ٶ���
	//int overflow_area_offset; //�������ƫ������ͷ��λ��,��INDEX_STDB_TAB���Ѿ����壬���Բ��ٶ���
	//int overflow_area_size; //������Ĵ�С,��INDEX_STDB_TAB���Ѿ����壬���Բ��ٶ���
	//int *prime_free_array_ptr; //����ά��ΪINDEX_STDB_TAB::table_record_sum],���������ʹ�õĿ��ж��й������顣
	int prime_free_pos;//
	//int *free_array_ptr;//����ά��ΪINDEX_STDB_TAB::table_record_sum
	int free_pos;
	int storage_used_num;// INDEX_STORAGE_HASH�������Ѿ�ʹ�õļ�¼�ĸ�������Ҫע����ǣ����INDEX_STORAGE_HASH�ļ�¼�Ѿ���ɾ����Ҳ����used_flag == NOT_USED���������ĺ���ָ�벻Ϊ�յĻ�����ô������¼��Ȼ������storage_used_num���档
	char reserved[20];
};

struct INDEX_STORAGE_HASH_DOVERFLOW_REC_HEAD
{
	//char *index_ptr ;//����ΪINDEX_STDB_TAB::index_length;
	//char *key_ptr;//����ΪINDEX_STDB_TAB::key_length;
	int prime_next_pointer;//���HASH����������λ�úͣ�����index��ֵ�����λ��index��ֵ����ȵ�����£���ô˳�����������PRIMARY_OVERFLOW_STORAGE���������ߣ�ֱ����PRIMARY_OVERFLOW_STORAGE_HASH�ҵ�һ��indexֵ���ȵ�����������ҵ�һ������λ�ã������㷨����������Prime_next_pointer�������PRIMARY_OVERFLOW_STORAGE 0�ż�¼��ƫ������
	int next_pointer;//��ͻ����£�����indexָ����ȵ�����£�����һ��ָ���λ�ã������OVERFLOW_STORAGE 0�ż�¼��ƫ����
	int tail_pointer;//���һ��λ�õ�ָ�룬������ٲ��롣
	int same_hash_num;//��ͬHASH����Ĺؼ��ֵĸ�����ͬʱ��Ϊһ���ж�����������оݡ�
	int used_flag; //ʹ�ñ�־
};//��¼����ΪINDEX_STDB_TAB::first_hash_prime

struct PRIMARY_OVERFLOW_STORAGE_HASH_REC_HEAD
{
	//char *index_ptr ;//����ΪINDEX_STDB_TAB::key_length;
	//char *key_ptr;//����ΪINDEX_STDB_TAB::key_length;
	int prime_next_pointer;//��PRIMARY_OVERFLOW��indexֵ��ȵ�����£���OVERFLOW_STORAGE_HASH����һ��ָ���λ�ã������PRIMARY_OVERFLOW_STORAGE��ַ��ƫ����
	int prime_prev_pointer;//��PRIMARY_OVERFLOW�е�ǰ��ָ�롣
	int next_pointer;// ��PRIMARY_OVERFLOW��indexֵ���ȵ�����³�ͻ����£���һ��ָ���λ�ã������OVERFLOW_STORAGE�׼�¼��ƫ����
	int tail_pointer;//���һ��λ�õ�ָ�룬������ٲ��롣
	int same_hash_num;//��ͬHASH����Ĺؼ��ֵĸ�����ͬʱ��Ϊһ���ж�����������оݡ�
	int used_flag; //ʹ�ñ�־��USED��NOT_USED
};//��¼����ΪINDEX_STDB_TAB::first_hash_size

//ͬ����Ҫstruct OVERFLOW_STORAGE_HASH_REC_HEAD,��¼����ΪINDEX_STDB_TAB::table_record_sum�������HASH����һ����

/********* structure for double-overflow hash  index body   begin *********************/

/********* structure for  hash  PK bodywithout overflow area     begin *********************/
//ʹ���ڲ����ݽṹ�����ٶ���
/********* structure for  hash  PK bodywithout overflow area     end *********************/

/********* structure for  hash PK   body   begin *********************/
//���ݽṹ��HASH����һģһ��
/********* structure for  hash PK body   begin **********************/

}
#endif
