#ifndef	__ODB_STRUCT_H__
#define	__ODB_STRUCT_H__

#include "odb_define.h"

////////////////////////////////////////////////////////

	struct KEY_STRU
	{
		int key;
		short field_id;
		short table_no;
	};
	
	struct KEY_STRU_AREA
	{
		int key:24;
		int area:8;
		short field_id;
		short table_no;
	};
	
	typedef long KEY_ID_STRU;
	
	struct APP_KEY_STRU
	{
		int app_id;
		KEY_ID_STRU key_id;
		
		bool operator>(const APP_KEY_STRU& kr) const
		{
			return ( (this->app_id > kr.app_id) ||
			         ((this->app_id == kr.app_id) && (this->key_id > kr.key_id)) );
		}
		
		bool operator<(const APP_KEY_STRU& kr) const
		{
			return ( (this->app_id < kr.app_id) ||
			         ((this->app_id == kr.app_id) && (this->key_id < kr.key_id)) );
		}
		
		bool operator==(const APP_KEY_STRU& kr) const
		{
			return ( (this->app_id == kr.app_id) && (this->key_id == kr.key_id) );
		}

		bool operator!=(const APP_KEY_STRU& kr) const
		{
			if(this->app_id != kr.app_id)
				return true;
			if(this->key_id == kr.key_id)
				return true;
			return false;
		}
	};
	
	struct APP_ID_STRU
	{
		int app_id;
		KEY_ID_STRU key_id;
		
		bool operator>(const APP_ID_STRU& kr) const
		{
			return ( (this->app_id > kr.app_id) ||
			         ((this->app_id == kr.app_id) && (this->key_id > kr.key_id)) );
		}
		
		bool operator<(const APP_ID_STRU& kr) const
		{
			return ( (this->app_id < kr.app_id) ||
			         ((this->app_id == kr.app_id) && (this->key_id < kr.key_id)) );
		}
		
		bool operator==(const APP_ID_STRU& kr) const
		{
			return ( (this->app_id == kr.app_id) && (this->key_id == kr.key_id) );
		}

		bool operator!=(const APP_ID_STRU& kr) const
		{
			if(this->app_id != kr.app_id)
				return true;
			if(this->key_id == kr.key_id)
				return true;
			return false;
		}
	};
	
#ifdef _BIGENDIAN
	struct COMM_KEY_STRU
	{
		short table_no;
		short column_id;
		int record_id;
	};
#else
	struct COMM_KEY_STRU
	{
		int record_id;
		short column_id;
		short table_no;
	};
#endif

#ifdef _BIGENDIAN
	struct COMM_KEY_AREA_STRU
	{
		short table_no;
		short column_id;
		int area_id:8
		int record_id:24;
	};
#else
	struct COMM_KEY_AREA_STRU
	{
		int record_id:24;
		int area_id:8;
		short column_id;
		short table_no;
	};
#endif
	//typedef APP_KEY_STRU APP_ID_STRU;

//	struct KEY_ID_STRU
//	{
//		int   record_id;
//		short column_id; 
//	 
//		bool operator>(const struct KEY_ID_STRU& kr) const
//		{
//			return ( (this->record_id > kr.record_id) ||
//			         ((this->record_id == kr.record_id) && (this->column_id > kr.column_id)) );
//		}
//		
//		bool operator<(const struct KEY_ID_STRU& kr)  const
//		{
//			return ( (this->record_id < kr.record_id) ||
//			         ((this->record_id == kr.record_id) && (this->column_id < kr.column_id)) );
//		}

//		bool operator==(const struct KEY_ID_STRU& kr) const
//		{
//			return ( (this->record_id == kr.record_id) && (this->column_id == kr.column_id) );
//		}
//	};
//
////
////Added 2003-09-01
////
///* ---------------------------------------------------------------------------*/
//	struct APP_KEY_STRU
//	{
//		int   app_id;
//		int   record_id;
//		short column_id;
//	
//		bool operator>(const struct APP_KEY_STRU& kr) const
//		{
//			return ( (this->app_id > kr.app_id) ||
//			         ((this->app_id == kr.app_id) && (this->record_id > kr.record_id)) ||
//			         ((this->app_id == kr.app_id) && (this->record_id == kr.record_id) && (this->column_id > kr.column_id)) );
//		}
//		
//		bool operator<(const struct APP_KEY_STRU& kr) const
//		{
//			return ( (this->app_id < kr.app_id) ||
//			         ((this->app_id == kr.app_id) && (this->record_id < kr.record_id)) ||
//			         ((this->app_id == kr.app_id) && (this->record_id == kr.record_id) && (this->column_id < kr.column_id)) );
//		}
//		
//		bool operator==(const struct APP_KEY_STRU& kr) const
//		{
//			return ( (this->app_id == kr.app_id) && (this->record_id == kr.record_id) && (this->column_id == kr.column_id) );
//		}
//	
//	};
////
////Added 2003-09-01
////
///* ---------------------------------------------------------------------------*/
//	struct APP_ID_STRU
//	{
//		int   app_id;
//		int   record_id;
//	
//		bool operator>(const struct APP_ID_STRU& kr)  const
//		{
//			return ( (this->app_id > kr.app_id) ||
//			         ((this->app_id == kr.app_id) && (this->record_id > kr.record_id)) );
//		}
//		
//		bool operator<(const struct APP_ID_STRU& kr)  const
//		{
//			return ( (this->app_id < kr.app_id) ||
//			         ((this->app_id == kr.app_id) && (this->record_id < kr.record_id)) );
//		}
//		
//		bool operator==(const struct APP_ID_STRU& kr)  const
//		{
//			return ( (this->app_id == kr.app_id) && (this->record_id == kr.record_id) );
//		}
//	
//	};


	
	//
	//data define for record sort of real time db
	//
	union SORT_DATA_TYPE
	{
		char         to_str[CHN_NAME_LEN];
		int          to_int;
		float        to_float;
		double       to_double;
		long 				 to_long;
		unsigned int to_uint;
		KEY_ID_STRU  to_key;
		APP_KEY_STRU to_appkey;
		APP_ID_STRU  to_appid;
	};
	
	//lmj add 2004.0521

	struct INT_VALUE_STATUS
	{
		int  			value;
		int			status;
	
	};
	
	struct FLOAT_VALUE_STATUS
	{
		float  			value;
		int			status;
	
	};
	
	
	struct CHAR_VALUE_STATUS
	{
	//	char  			value[CHN_NAME_LEN];
		unsigned char           value;
		int			status;
	
	};
	
	struct LONG_VALUE_STATUS
	{
		long value;
		int  status;
	
	};
	
	struct UNION_VALUE_STATUS
	{
		SORT_DATA_TYPE  	value;
		int			status;
	};
	
	
	struct KEY_ID_STA_STRU
	{
		KEY_ID_STRU	key_id_stru;
		short  			status_field;	
	
	};

	

#endif
