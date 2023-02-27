/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  odb_tablenet.h
DESCRIPTION:  RTDBMS NET ACCESS INTERFACE
FUNCTION LIST:
COMMENT:     
History:                                                        
Date          Version     Modifier                Activities
================================================================================
2009-06-09    1.0             wj                 created
*******************************************************************************/

		const short	C_DATATYPE_STRING   	= 1;
		const short	C_DATATYPE_UCHAR    	= 2;
		const short	C_DATATYPE_SHORT    	= 3;
		const short	C_DATATYPE_INT		= 4;
		const short	C_DATATYPE_DATETIME	= 5;
		const short	C_DATATYPE_FLOAT    	= 6;
		const short	C_DATATYPE_DOUBLE	= 7;
		const short	C_DATATYPE_KEYID    	= 8;
		const short	C_DATATYPE_BINARY	= 9;
		const short	C_DATATYPE_TEXT		= 10;
		const short	C_DATATYPE_IMAGE    	= 11;
		
		const short 	C_DATATYPE_APPKEYID 	= 12;
		const short 	C_DATATYPE_APPID    	= 13;
		const short   C_DATATYPE_UINT       = 14;
		const short   C_DATATYPE_LONG       = 15;
		
		typedef vector<varchar> SEQ_STRING;
		typedef binary   SEQ_CHAR;
		typedef vector<float>  SEQ_FLOAT;
		typedef vector<int>   SEQ_LONG;
		typedef vector<long> SEQ_LONGLONG;
/*------------------------------------------------------------------------------
		1. RTDBMS NET ACCESS for create and delete a table  
		   but only used by down load until now 2003-02-28
------------------------------------------------------------------------------*/
		
		/* ---------------------------------------------------------------------
		create a table
		----------------------------------------------------------------------*/

		typedef long KEYID_STRUCT;
		
		struct APPKEY_STRUCT
		{
			int app_id;
			KEYID_STRUCT key_id;
		};
		
		typedef APPKEY_STRUCT APPID_STRUCT;
		
		struct  KEYID_STATUS_STRUCT
		{
			KEYID_STRUCT	key_id;
			short 		status;
		};
		
		typedef vector<KEYID_STRUCT>  	SEQ_KEY_ID;
		typedef vector<APPKEY_STRUCT> 	SEQ_APPKEY_ID;
		typedef vector<APPID_STRUCT>  	SEQ_APP_ID;
		typedef vector<KEYID_STATUS_STRUCT>  	SEQ_KEY_ID_STA;
		
		struct union_data_type
		{
			int type;
			union
			{
				varchar        c_char; 		//C_DATATYPE_STRING
				unsigned char  c_uchar; 	//C_DATATYPE_UCHAR
				short          c_short;		//C_DATATYPE_SHORT
				int            c_int;		//C_DATATYPE_INT
				long           c_time;		//C_DATATYPE_DATETIME
				float          c_float;		//C_DATATYPE_FLOAT
				double         c_double;	//C_DATATYPE_DOUBLE
				KEYID_STRUCT   c_keyid;		//C_DATATYPE_KEYID
				SEQ_CHAR       c_binary;	//C_DATATYPE_BINARY
				SEQ_CHAR       c_text;		//C_DATATYPE_TEXT
				SEQ_CHAR       c_image;		//C_DATATYPE_IMAGE
			    APPKEY_STRUCT  c_appkeyid;	//C_DATATYPE_APPKEYID
			    APPID_STRUCT   c_appid;		//C_DATATYPE_APPID
			    unsigned int   c_uint;		//C_DATATYPE_UINT
			    long           c_long;		//C_DATATYPE_LONG
			}val;
		};
		
		typedef vector<union_data_type> SEQ_COMMON_DATA;
		
		struct	TABLE_STRU
		{
			int   table_no;  //表号  RDBMS
			short  app_no;
			short  column_num;
			varchar table_name_eng;
			varchar table_name_chn;
			short  record_length;
			int   record_num;
			int   max_record_num;
			unsigned char  is_auto_generate;
			unsigned char  is_record_app;
		};
		
		struct	COLUMN_STRU
		{
			short   field_no;      //域号  RDBMS
			varchar  field_name_eng;
			varchar  field_name_chn;
			short   data_length;
			unsigned char   data_type;
			
			unsigned char   allow_null;
			unsigned char   is_display;
			unsigned char   display_type;
			short   display_length;
			unsigned char   display_precision;
			
			varchar  menu_name;
			unsigned char	reference_flag;
			unsigned char	is_key;
			unsigned char	is_index;
			unsigned char	sort_order_no;
			
			unsigned char	reference_display;
			unsigned char	is_input;
			unsigned char	is_fix;
			
			varchar  init_value;
			varchar  min_value;			
			varchar  max_value;
			
			int    reference_table;
			short   reference_column;
			int    column_special;
		};
		
		typedef vector<COLUMN_STRU> SEQ_COLUMN_INFO;
		
		
		//from download idl //OutStruct
		struct REQ_STDB_CREATE
		{
			//short            app_no;          //应用号
			//int             table_no;        //表号  RDBMS
			//int             record_num;      //记录 个数
			//int             field_num;       //域个数
			
			TABLE_STRU        table_info;       //表信息
			SEQ_COLUMN_INFO   column_info;      //域信息
			SEQ_COMMON_DATA   data_value;       //值
		};
		
		/* ---------------------------------------------------------------------
		delete a table
		----------------------------------------------------------------------*/		
		struct REQ_STDB_DELETE
		{
			short  			 app_no;
			int  			 table_no;
			varchar 			 passwd;
		};

/*------------------------------------------------------------------------------
		2. RTDBMS NET ACCESS for get table and field parameter
------------------------------------------------------------------------------*/
		
		/* ---------------------------------------------------------------------
		table parameter
		----------------------------------------------------------------------*/
		struct TABLE_PARAMETER
		{
			varchar           table_name_eng;
			varchar           table_name_chn;
			int             table_no;       //RDBMS
			int             key_len;
			int             subkey_num;

			short            field_sum;
			int             record_number;
			int             record_size;
			
			unsigned char            is_auto_generated;
			
//	int                 table_type;          //for graph, etc
//	unsigned char       is_system_table;     //Added 2003-09-01
//	unsigned char       is_record_app;
//	unsigned char       is_record_lock;
//	unsigned char       is_record_resp;
//
//	int                 reserved_1;
//	int                 reserved_2;
//	int                 reserved_3;
//	int                 reserved_4;
//	int                 reserved_5;
		};

		/* ---------------------------------------------------------------------
		field parameter, more 3 elements than struct COLUMN_STRU
		----------------------------------------------------------------------*/
		struct FIELD_PARAMETER
		{
			short	 	field_no; 
			short		r_field_no;     //
			short       field_id;       //////
			short       column_id;      //////
			
			varchar		field_name_eng;
			varchar		field_name_chn;
			short  		field_length;
			unsigned char  		data_type;
			
			unsigned char		allow_null;
			//unsigned char		is_display;          ////
			unsigned char		display_type;
			//short		display_length;	     ////
			//unsigned char		display_precision;   ////
			
			varchar		menu_name;
			unsigned char		reference_flag;
			unsigned char 		is_keyword;
			unsigned char		is_index;
			int  		offset;         //
			int  		check_tag;      //
			unsigned char		sort_order_no;
			
			unsigned char       reference_mode;    //////
			unsigned char		reference_display;
			//unsigned char		is_input;          ////
			//unsigned char		is_fix;            ////

			unsigned char       is_app_syn;        //////
			unsigned char	    auto_meas_type;    //////
			
			varchar      default_asciival;
			varchar      min_asciival;
			varchar      max_asciival;
			
			int        ref_tableno;
			short       ref_fieldno;
			//unsigned char   	column_special;
			int   	    column_special;
			
			int        search_attribute;   //Added 2003-09-01
			int        statics_attribute;
			int        reserved_1;
			int        reserved_2;
			int        reserved_3;
			int        reserved_4;
			int        reserved_5;
		};

		struct R_FIELD_BASE_INFO
		{
			int            offset;
			int            field_length;
			short           field_no;
			unsigned char   	data_type;
			unsigned char   	is_keyword;
		};
		
		typedef vector<R_FIELD_BASE_INFO> SEQ_FIELD_BASE_INFO;
/*------------------------------------------------------------------------------
		3. RTDBMS NET DATA ACCESS structure for get, write, delete, modify, update
------------------------------------------------------------------------------*/		
		
		
		/* ---------------------------------------------------------------------
		one or many records; one or many fields;
		the common structure used by others followed
		----------------------------------------------------------------------*/
		struct FIELD_STRU
		{  
			short 		field_no;
			short 		field_type;                   //data type for get; byte len for write
			short 		field_length;                 // 2003-07-31 //reserved for later
			 
		};
		
		typedef vector<FIELD_STRU> SEQ_FIELD_STRU;
		
		struct REQ_BASE
		{
			short		 context_no;   //long
			int 		 app_no;      //short
			int             table_no;
			char             byte_tag;  //DB_BYTE_T   	 byte_tag;
			SEQ_FIELD_STRU   seq_field;
		};
		
		/* ---------------------------------------------------------------------
		get one or many fields of one or many records;
		maybe by key_word; if no keyword, default is current record
		----------------------------------------------------------------------*/
		struct REQ_READ
		{
			REQ_BASE         base_info;
			
			SEQ_CHAR         seq_keyword;       //usually only one key word
			int             keybuf_size;       //0--no key_word, else key word data size
			
			varchar           str_fname_list;
			
			//short get_mode;                  // 0 -- by field no
			                                   // 1 -- by field name
			                                   // 2 -- by key word
		};
		
		struct RSP_READ
		{
			int             record_num;        //记录 个数
			int             record_size;
			short            field_num;         //域个数
			SEQ_FIELD_STRU   seq_field;         //field信息
			
			SEQ_CHAR         seq_data;			//return data value
			int             data_size;         //return data size; 总长度
		};
		/* ---------------------------------------------------------------------
		write one record, include key_word
		----------------------------------------------------------------------*/
		struct REQ_KEY_WRITE
		{
			REQ_BASE         base_info;
			
			SEQ_CHAR         record_value;      //record data value
			int             buf_size;          //record data size
		};
		
		/* ---------------------------------------------------------------------
		modify one or many fields of one or many records,
		maybe by key_word; if no keyword, default is current record
		----------------------------------------------------------------------*/
		struct REQ_MODIFY
		{
			REQ_BASE         base_info;
			short		 is_field_name;	    //是否按域名修改 是 1 否 0
			varchar		 field_name_str;    //需要修改的域名
			
			SEQ_CHAR         seq_keyword;       //key word data value
			int             keybuf_size;       //key word data size
			
			SEQ_CHAR         field_value;       //field data buffer
			int             buf_size;          //field data size
		};

		/* ---------------------------------------------------------------------
		update f one or many records by key_word
		----------------------------------------------------------------------*/
		struct REQ_UPDATE
		{
			REQ_BASE         base_info;
			
			SEQ_CHAR         field_value;       //data buffer
			int             buf_size;          //data size
		};
		
		/* ---------------------------------------------------------------------
		delete a record by key_word
		----------------------------------------------------------------------*/
		struct REQ_KEY_ERASE
		{
			REQ_BASE         base_info;
			
			SEQ_CHAR         seq_keyword;       //str_keyword;//2003-02-26
			int             keybuf_size;       //redundant
		};
		
		/* ---------------------------------------------------------------------
		delete many records by key
		----------------------------------------------------------------------*/
		struct DEL_REQ
		{
			REQ_BASE         base_info;
			int				 del_direction;
			int              del_num;
			SEQ_CHAR         seq_keyword;
			int              keybuf_size;
		};
		
		/* ---------------------------------------------------------------------
		clear one table
		----------------------------------------------------------------------*/
		struct REQ_TABLE_CLEAR
		{
			REQ_BASE         base_info;
		};
		
		struct REQ_FIELD_BASE_INFO
		{
			REQ_BASE        base_info;
			SEQ_LONG 	seq_field_no;
		};
		
		struct RSP_FIELD_BASE_INFO
		{
			SEQ_FIELD_BASE_INFO 	lseq_field_base_info;
		};
		
		/* ---------------------------------------------------------------------
		get table and fields parameter (one)
		----------------------------------------------------------------------*/
		struct REQ_PARAMETER
		{
			REQ_BASE         base_info;
			//short 		 para_type; //1--table parameter, 10--fields parameter
			
			varchar           str_field_list;  //field's attribute that wanted to be query
		};

		typedef vector<FIELD_PARAMETER> SEQ_FIELD_PARAMETER;
		
		struct RSP_PARAMETER
		{
			TABLE_PARAMETER     	table_para;
			SEQ_FIELD_PARAMETER 	seq_field_para;
		};

		/* ---------------------------------------------------------------------
		get table and fields parameter (two)
		----------------------------------------------------------------------*/
		struct REQ_APP_PARAMETER
		{
			int    app_no;
			varchar  app_name;    //all_name
		};

		//this is rsp
		typedef vector<TABLE_PARAMETER> SEQ_TABLE_PARAMETER;
		
		struct RSP_APP_PARAMETER
		{
			int    app_no;
			varchar  app_name;    //all_name

			SEQ_TABLE_PARAMETER     	seq_table_para;
		};

/*------------------------------------------------------------------------------
		4. RTDBMS key word length of all table
								       
							2003-03-10
------------------------------------------------------------------------------*/
		struct TABLE_KEYINFO
		{
			//int  table_no;      //RDBMS table no
			short keyword_len;     //total key word length
		};
		
		typedef vector<TABLE_KEYINFO> SEQ_TABLE_KEYINFO;
		
		struct RSP_KEYINFO
		{
			SEQ_TABLE_KEYINFO seq_keyinfo;
		};
		
/*------------------------------------------------------------------------------
		5. SQL INTERFACE
								       
							2003-03-26
------------------------------------------------------------------------------*/		
		struct REQ_SQL
		{
			REQ_BASE         base_info;
			varchar           str_select;
			//SEQ_CHAR       seq_select;
		};
/*------------------------------------------------------------------------------
		6. some function for gui
						     
					  2003-06-05
------------------------------------------------------------------------------*/
		/* ---------------------------------------------------------------------
		menu read
		----------------------------------------------------------------------*/
		struct REQ_MENU
		{
			REQ_BASE         base_info;
			//SEQ_CHAR         seq_menu_name;
			varchar           str_menu_name;
			unsigned char            menu_status;
		};
		
		struct MENU_INFO_STRU
		{
			varchar          menu_name;                     /* 菜单名*/
			unsigned char 			menu_no;                       /* 序号*/
			int            actual_value;                  /* 实际值*/
			varchar          display_value;                 /* 显示值*/
			unsigned char 			menu_status;                   /* 菜单状态 */
			varchar          menu_macro_name;
		};
		
		typedef vector<MENU_INFO_STRU> SEQ_MENU_INFO;
		
		struct RSP_MENU
		{
			int            menu_num;
			//int            record_size;
			SEQ_MENU_INFO   seq_menu;
		};

		/* ---------------------------------------------------------------------
		relevant menu read
		----------------------------------------------------------------------*/
		struct REQ_RELE_MENU
		{
			REQ_BASE         base_info;
		};

		struct MENU_RELEVANT_STRU
		{
			int          table_id;                       /* 表ID号 */
			short         column_id;                      /* 域ID号 */
			int          column_value;                   /* 域值   */
			short         r_column_id;                    /* 对应域ID号 */
			varchar        r_menu_name;                    /* 对应菜单名 */
			int          default_value;                  /* 默认值 */
		};
		
		typedef vector<MENU_RELEVANT_STRU> SEQ_MENU_RELEVANT;

		struct RSP_RELE_MENU
		{
			int            menu_num;
			//int            record_size;
			SEQ_MENU_RELEVANT  seq_menu;
		};
				
		/* ---------------------------------------------------------------------
		get name varchar by key id
		----------------------------------------------------------------------*/
		struct REQ_NAME_STRING
		{
			short            req_type;
			
			REQ_BASE         base_info;
			KEYID_STRUCT     keyid_stru;        //by key id  -- 1
			long        reference_id;      //by id      -- 2
			SEQ_CHAR         seq_keyword;       //usually only one key word
			int             keybuf_size;       //0--no key_word, else key word data size
		};

		struct RSP_NAME_STRING
		{
			int            name_len;
			varchar          ref_string;
		};

		/* ---------------------------------------------------------------------
		get name varchar by many key ids
		----------------------------------------------------------------------*/
		struct REQ_MULTI_NAME_STRING
		{
			short            req_type;
			
			REQ_BASE         base_info;
			SEQ_KEY_ID       seq_keyid_stru;    // -- 1
			
			SEQ_APPKEY_ID    seq_appkeyid_stru; // -- 2
			
			SEQ_APP_ID       seq_appid_stru;    // -- 3
			
			SEQ_LONGLONG   seq_id;			// -- 4
		};

        typedef vector<RSP_NAME_STRING> SEQ_RSP_NAME_STRING;
        
        struct RSP_MULTI_NAME_STRING
        {
        	SEQ_RSP_NAME_STRING seq_name_string;
        };

		/* ---------------------------------------------------------------------
		reference menu read
		----------------------------------------------------------------------*/
		struct REQ_REF_MENU
		{
			REQ_BASE         base_info;
		};
		
		struct REF_MENU_STRING
		{
			long        order_no;
			varchar    menu_name;
		};
		
		typedef vector<REF_MENU_STRING> SEQ_REF_MENU;
		
		struct RSP_REF_MENU
		{
			SEQ_REF_MENU  seq_menu;
		};
/*------------------------------------------------------------------------------
		7. some function for graph
						     
					  2003-06-16
------------------------------------------------------------------------------*/
		/* ---------------------------------------------------------------------
		get real time data from rtdbms server
		----------------------------------------------------------------------*/
		struct GRAPH_SIMPLE_REQ
		{
			int   order_no;
			long  keyword_id;
			short  table_no;
			short  field_no;
		};
		
		struct GRAPH_XY_REQ
		{
			int   order_no;
			long   keyword_id;
			short  table_no;
			short  field_no;
			unsigned char  mode;                //
			short data_num;
		};
		
		//
		struct GRAPH_FIELD_STRU
		{
			int   order_no;
			short  field_no;        
			varchar  field_alias;	//lmj add for graph list query by sql where		
			     
			short  rele1_table_no;   
			short  rele1_field_no;   			 
			
			short  rele2_table_no;   //lmj add for graph list query by 2 index	
			short  rele2_field_no;   //lmj add for graph list query by 2 index	
		};
		
		typedef vector<GRAPH_FIELD_STRU> SEQ_GRAPH_FIELD;
		struct GRAPH_LIST_REQ
		{
			short  table_no;
			varchar condition;
			SEQ_STRING where_field_seq; //lmj add for graph list query by sql where			
			SEQ_GRAPH_FIELD seq_field;
		};
		 
		typedef vector<GRAPH_SIMPLE_REQ> SEQ_SIMPLE_REQ;
		typedef vector<GRAPH_XY_REQ>     SEQ_XY_REQ;
		typedef vector<GRAPH_LIST_REQ>   SEQ_LIST_REQ;
		
		struct GRAPH_REAL_REQ
		{
			long graph_id;           	//lmj add for GraphGetDataEx
			int cal_type;   		//lmj add for before calculate 
			SEQ_SIMPLE_REQ simple_req;
			SEQ_XY_REQ     xy_req;
			SEQ_LIST_REQ   list_req;
		};
		
		struct GRAPH_REAL_ODB_REQ
		{
			REQ_BASE       base_info;
			long graph_id;           //lmj add for GraphGetDataEx
			int cal_type;   	//lmj add for before calculate 
			SEQ_SIMPLE_REQ simple_req;
			SEQ_XY_REQ     xy_req;
			SEQ_LIST_REQ   list_req;
		};
		
		/*--------------------------------------------------------------------*/
		struct GRAPH_SIMPLE_RSP
		{
			int            order_no;
			union_data_type data_value;
		};

		struct GRAPH_XY_RSP
		{
			int            order_no;
			SEQ_FLOAT       seq_data_value;
		};

		struct ORDER_VALUE
		{
			int   order_no;
			SEQ_COMMON_DATA seq_data_value;
		};
		
		typedef vector<ORDER_VALUE> SEQ_ORDER_VALUE;

		struct GRAPH_LIST_RSP
		{
			SEQ_ORDER_VALUE seq_list;
		};
		
		//
		//
		typedef vector<GRAPH_SIMPLE_RSP> SEQ_SIMPLE_RSP;
		typedef vector<GRAPH_XY_RSP>     SEQ_XY_RSP;
		typedef vector<GRAPH_LIST_RSP>   SEQ_LIST_RSP;
		
		struct GRAPH_REAL_RSP
		{
			long graph_id;           //lmj add for GraphGetDataEx
			int cal_type;   	//lmj add for before calculate 
			SEQ_SIMPLE_RSP simple_rsp;
			SEQ_XY_RSP     xy_rsp;
			SEQ_LIST_RSP   list_rsp;
		};

		/* ---------------------------------------------------------------------
		get app info from rtdbms server
		----------------------------------------------------------------------*/
		struct ORDER_KEY_REQ
		{
			int   order_no;
			long key_id;
		};
		
		typedef vector<ORDER_KEY_REQ> SEQ_ORDER_KEY_REQ;
		struct GRAPH_ORDER_KEY_REQ
		{
			REQ_BASE        base_info;
			int   		app_no;
			long 	graph_id;
			SEQ_ORDER_KEY_REQ seq_order_key;
		};
		
		struct ORDER_KEY_RSP
		{
			int   order_no;
			unsigned char  is_in_app;
		};
		
		typedef vector<ORDER_KEY_RSP> SEQ_ORDER_KEY_RSP;
		struct GRAPH_ORDER_KEY_RSP
		{
			long graph_id;
			SEQ_ORDER_KEY_RSP seq_app;
		};
/*------------------------------------------------------------------------------
		8. some function for exchange name & no (app/table/field, include chn/eng)
						     
					  2003-12-16
------------------------------------------------------------------------------*/

		struct REQ_ENTITY
		{
			REQ_BASE base_info;
			
			varchar   object_name;
			short    object_id;     //app/table/field
			unsigned char    object_mode;   // name ---> no // no --->name
			unsigned char    object_eng;
		};

		struct RSP_ENTITY
		{
			int     object_no;
			varchar   object_name;
		};
/*------------------------------------------------------------------------------
		9. some function for factory name & no & id
						     
					  2004-01-06
------------------------------------------------------------------------------*/
		struct REQ_FAC
		{
			REQ_BASE base_info;

			int        table_no;
			SEQ_CHAR    seq_keyword;       //usually only one key word
			int        keybuf_size;       //0--no key_word, else key word data size
			
			long fac_no;			// hcr: fac_no & fac_id both use this member, so reserve as long but not short
			varchar      fac_name;
		};
		
		struct RSP_FAC
		{
			long  fac_no;
			varchar      fac_name;
		};
		
		
		struct REQ_VAL_STA
		{
			short           req_type; // 1 yc , 2  yx , 3 ym  , 4 union type 5 yc , 6  yx , 7 ym  , 8 union type		
			REQ_BASE        base_info;
			SEQ_KEY_ID     	keyid_stru_seq;  
			SEQ_KEY_ID_STA 	keyid_sta_stru_seq;       
		};
			
		
		struct  TIntValueStatus
		{
			int		value;
			int 	status;
		};


		struct  TFloatValueStatus
		{
			float		value;
			int 		status;
		};

		struct  TCharValueStatus
		{
			unsigned char		value;
			int 		status;
		};
		
		struct  TLongValueStatus
		{
			long value;
			int status;
		};

		struct  TUnionValueStatus
		{
			union_data_type	value;
			int 		status;
		};
		
		

		typedef vector< TIntValueStatus >  	SEQ_INT_VAL_STA;

		typedef vector< TFloatValueStatus >  	SEQ_FLOAT_VAL_STA;

		typedef vector< TCharValueStatus > 	SEQ_CHAR_VAL_STA;
		
		typedef vector< TLongValueStatus > 	SEQ_LONG_VAL_STA;
		 
		typedef vector< TUnionValueStatus > 	SEQ_UNION_VAL_STA;
		
		struct RSP_INT_VAL_STA
		{
			SEQ_INT_VAL_STA  rsp_val_sta;
		};

		struct RSP_FLOAT_VAL_STA
		{
			SEQ_FLOAT_VAL_STA  rsp_val_sta;
		};

		struct RSP_CHAR_VAL_STA
		{
			SEQ_CHAR_VAL_STA rsp_val_sta;
		};
		
		struct RSP_LONG_VAL_STA
		{
			SEQ_LONG_VAL_STA rsp_val_sta;
		};
		
		struct RSP_UNION_VAL_STA
		{
			SEQ_UNION_VAL_STA  rsp_val_sta;
		};

//wj added for ConGet -- 2008.08.11		
		struct REQ_CON
		{
			REQ_BASE         base_info;//base_info.seq_field.length() == 1
			//FIELD_STRU       get_field_info;//non or only one get_field
			SEQ_FIELD_STRU   get_field_info;//non or more than one get_field
			SEQ_CHAR         con_field_value;
		};
		
		struct RSP_CON
		{
			int             fit_num;
			int             one_fit_size;//if non get_field, one_fit_size == one_record_size
			SEQ_CHAR         seq_data;
			int             data_size;
		};		


/*------------------------------------------------------------------------------
		10. rtdb_server function number
						     
					  2008-11-28

------------------------------------------------------------------------------*/
		const int  FUNC_TABLECREATE_NO = 1;
		const int  FUNC_TABLEDELETE_NO = 2;
				
		const int  FUNC_TABLEGET_NO = 3;
		const int  FUNC_TABLEGETBYNAME_NO = 4;
		const int  FUNC_TABLEGETBYFIELDVALUE_NO = 5;
		const int  FUNC_TABLEWRITE_NO = 6;
		const int  FUNC_TABLEMODIFY_NO = 7;
		const int  FUNC_TABLEUPDATE_NO = 8;
		const int  FUNC_DELETERECORD_NO = 9;
		const int  FUNC_TABLECLEAR_NO = 10;
				
		const int  FUNC_GETTABLEPARA_NO = 11;
		const int  FUNC_GETTABLEPARAMETER_NO = 12;
		const int  FUNC_GETAPPTABLEPARAMETER_NO = 13;
		const int  FUNC_GETFIELDPARA_NO = 14;
				
		const int  FUNC_GETKEYINFO_NO = 15;
		const int  FUNC_TABLEGETBYSQL_NO = 16;
		const int  FUNC_MENUREAD_NO = 17;
		const int  FUNC_TABLERELEMENUREAD_NO = 18;
		const int  FUNC_GETNAMESTRINGBYKEYID_NO = 19;
		const int  FUNC_GETREFMENUSTRING_NO = 20;
				
		const int  FUNC_GETNAMESTRINGBYMULTIKEYID_NO = 21;
			
		const int  FUNC_GRAPHGETDATA_NO = 22;
		const int  FUNC_GRAPHGETAPPINFO_NO = 23;
				
		const int  FUNC_EXCHANGENAMENO_NO = 24;
				
		const int  FUNC_GETFACNAMEBYKEYNO_NO = 25;
		const int  FUNC_GETFACNAMEBYFACID_NO = 26;
		const int  FUNC_GETFACNAMEBYFACNO_NO = 27;
				
		const int  FUNC_GETFACIDBYFACNAME_NO = 28;
		const int  FUNC_GETFACNOBYFACNAME_NO = 29;
				
		const int  FUNC_GETFACIDBYFACNO_NO = 30;
		const int  FUNC_GETFACNOBYFACID_NO = 31;
				
		const int  FUNC_GETVALUEANDSTATUS_NO = 32;
				
		const int  FUNC_PINGRTDBSERVER_NO = 33;
		const int  FUNC_DELETERECORDS_NO  = 34;
		
