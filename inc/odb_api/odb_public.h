/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  publics.h
DESCRIPTION:  public define
FUNCTION LIST:
COMMENT:      ������ؽṹ����  ͬodb_api_struct.h
History:                                                        
Date          Version     Modifier                Activities
2002-09-01    1.0                              modify
================================================================================
2002-09-01    1.0                              created
*******************************************************************************/

#ifndef	__ODB_PUBLIC_H__
#define	__ODB_PUBLIC_H__

//����ͷ�ļ�
#include <sys/types.h>
#include <sys/stat.h>

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <stdarg.h>

#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/times.h>

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <utility>
#include <iostream>

#ifndef __ODB_DEFINE_H__
#include "odb_define.h"
#endif

#include "odb_struct.h"

//namespace ODB
//{
	struct TABLE_PARA
	{
		char                table_name_eng[ENG_NAME_LEN];
		char                table_name_chn[CHN_NAME_LEN];
		int                 key_len;
		int                 subkey_num;
		int                 record_number;
		int                 record_size;
		short               field_sum;
		unsigned char       is_auto_generated;   //key_generate_type
	
	};
	
	struct FIELD_PARA
	{
		int                 table_id;						//��ID��		INT
		short               field_id;						//��ID��	        SHORT
		short               column_id;						//���ڲ�ID��           	SHORT
		char                field_name_eng[ENG_NAME_LEN];			//��Ӣ����	        STRING(32)
		char                field_name_chn[CHN_NAME_LEN];			//��������	        STRING(64)
		short               field_length;					//���ݳ���	        SHORT
		unsigned char       data_type;						//��������	        UCHAR
		unsigned char       is_keyword;						//�Ƿ�����	        UCHAR
		unsigned char       allow_null;						//�Ƿ������ֵ	       	UCHAR
			
		int                 app_type;						//Ӧ������	        INT
		unsigned char       is_app_syn;						//�Ƿ����ʱȫӦ��ͬ�� 	UCHAR
		unsigned char       index_order_no;					//����������˳���	UCHAR
		unsigned char       sort_order_no;					//Ĭ������˳���	UCHAR
	
	//   //   //   //	//
		unsigned char       is_input;						//�Ƿ���������	       	UCHAR
		unsigned char       is_display;						//�Ƿ���ʾ	        UCHAR
		short               display_order_no;					//��ʾ˳���	       	SHORT
		unsigned char       is_fix;						//���ʱ�Ƿ�̶�	UCHAR
		short               display_length;					//Ĭ����ʾ����	        SHORT
		unsigned char       display_precision;					//С����ʾ����	        UCHAR
		unsigned char       align_type;						//���뷽ʽ	        UCHAR
		unsigned char       display_type;					//��ʾ����	        UCHAR
		char                menu_name[MENU_NAME_LEN];				//�˵���	        STRING(40)
		
		unsigned char       reference_flag;					//������������	       	UCHAR
		unsigned char       reference_mode;					//�������÷�ʽ	       	UCHAR
		int                 reference_table;					//���ñ���	        INT
		short               reference_column;					//�������	        SHORT
		unsigned char       reference_display;					//������ʱ�Ƿ���ʾ	UCHAR
		
		char                init_value[DEFAULT_VALUE_LEN];			//ȱʡֵ	        STRING(20)
		char                min_value[DEFAULT_VALUE_LEN];			//��Сֵ	        STRING(20)
		char                max_value[DEFAULT_VALUE_LEN];			//���ֵ	        STRING(20)
		
		int                 column_special;					//����������	       	INT        
		unsigned char       auto_meas_type;					//�Զ�������������	UCHAR
		short               gen_array_dimension;				//����ά��	        SHORT
		char                gen_array_name[ARRAY_NAME_LEN];			//��������	        STRING(32)
		char                column_description[DESCRIPTION_LEN];		//��������Ϣ   		STRING(100)
	
		unsigned char       is_index;						//	
		int                 offset;                         			//added by RTDBMS
	};
	
	
	
	////////////////////////////////////////////////////////////////////////////////
	//      for down load                                           //
	//                                                                            //
	////////////////////////////////////////////////////////////////////////////////
	
	/*============================================================================*/
	
	struct STDB_CREAT_REQ
	{
		int                 app_no;	
		int                 table_no;
		int                 table_version;
		int                 field_sum;          //short
		char                table_name_eng[ENG_NAME_LEN];
		char                table_name_chn[CHN_NAME_LEN];
		int                 record_num;
		int                 record_sum;
		int                 record_length;      //short
			
		unsigned char       is_system_table;
		unsigned char       is_auto_generated;  //key_generate_type;
		unsigned char       is_record_apped;
		unsigned char       is_record_lock;
		unsigned char       is_record_resp;
		
		DB_MEDIA_T          stdb_media;                            //no used
		ACCESS_METHODS_T    access_meth;                           //
		int                 seq_field_no;                          //
		char                passwd[MAX_NAME_STRING_LEN];           //
		
		
		int                 table_type;                            //for graph, etc
		
		/******Added for index begin ************/
		int	pk_index_method;
		int	pk_storage_type;//�����洢������Ʃ�繲���ڴ�����ļ�ӳ���ڴ档
		int	pk_mem_alloc_type;//�����ڴ���䷽����Ʃ������HASH�ķ��䷽�����������HASH���䷽����
		/******Added for index end**************/

		int                 reserved_1;
		int                 reserved_2;
		int                 reserved_3;
		int                 reserved_4;
		int                 reserved_5;
		int             	area_num;
		int             	area_no[MAX_AREA_NUM];
		char             	area_name[MAX_AREA_NUM][ENG_NAME_LEN];
	};
	
	struct FIELD_CREAT_REQ
	{
		int                 app_no;             //app_type
		short               field_no;
		short               r_field_no;
		short               field_id;
		short               column_id;
		short               field_length;
		char                field_name_eng[ENG_NAME_LEN];
		char                field_name_chn[CHN_NAME_LEN];
			
		int                 column_special;
		int                 ref_tableno;        //long reference_table; 
		short               ref_fieldno;        //short reference_column;
		
		unsigned char       reference_flag;
		unsigned char       reference_mode;
		unsigned char       reference_display;
		
		unsigned char       data_type;
		unsigned char       allow_null;
		unsigned char       is_keyword;
		unsigned char       display_type;
		unsigned char       is_index;           //octet index_order_no;
		unsigned char       sort_order_no;
	
		unsigned char       is_app_syn;
		unsigned char       auto_meas_type;
		
		char                menu_name[MENU_NAME_LEN];
		char                default_asciival[DEFAULT_VALUE_LEN];  //init_value
		char                min_asciival[DEFAULT_VALUE_LEN];      //min_value
		char                max_asciival[DEFAULT_VALUE_LEN];      //max_value
	
	
		int                 search_attribute;   //  ��������������           //Added 2003-09-01
		int                 statics_attribute;  //  ͳ�Ƶ�������
		int                 reserved_1;         //  ϵͳ����
		int                 reserved_2;         //  ϵͳ����
		int                 reserved_3;         //  ϵͳ����
		int                 reserved_4;         //  ϵͳ����
		int                 reserved_5;         //  ϵͳ����
	};
	
	
	struct STDB_DELETE_REQ 
	{
		int  app_no;
		int  table_no;
		char passwd[MAX_PASSWD_LEN];
	};
	
	
	//struct FIELD_TYPE
	//{
	//	short field_no;
	//	short field_type;                   //data type for get; byte len for write
	//};
	
	struct FIELD_BASE_INFO
	{
		int             offset;
		int             field_length;
		short           field_no;
		unsigned char   data_type;
		unsigned char   is_keyword;
	};
	
#ifndef __i386	
	static char strFILE[100];
	static int iLINE;
#endif
	
#ifndef __i386	
//	#define TRACE  strFILE =  __FILE__; iLINE = __LINE__;  odb_trace
	#define TRACE  strcpy(strFILE, __FILE__); iLINE = __LINE__;  printf("\t%s:%d-->",strFILE,iLINE); odb_trace
#else
	#define TRACE(...)   odb_trace(__FILE__, __LINE__, __VA_ARGS__)
#endif
		
#ifndef __i386
	inline void odb_trace(const char *p,...)
	{
#else
	inline void odb_trace(const char *strFILE, const int iLINE, const char *p,...)
	{
		printf("\t%s:%d-->",strFILE,iLINE);
#endif
		static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
		va_list va;
		va_start(va,p);
		
		pthread_mutex_lock(&lock);
		vfprintf(stdout,p,va);
		fflush(stdout);
		pthread_mutex_unlock(&lock);
		va_end(va);
	}
	
	/*============================================================================*/
	
	extern "C"
	{
		void do_nothing1(int nn);
		void do_nothing2(int nn);
	}
//}	
#endif	
