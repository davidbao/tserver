/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  odb_where.h
DESCRIPTION:  interface to hubo
FUNCTION LIST:
COMMENT:
History:                                                        
Date          Version     Modifier                Activities
2002-11-26    1.0                              modify
================================================================================
2002-11-26    1.0                              created
*******************************************************************************/

#ifndef __ODB_WHERE_H__
#define __ODB_WHERE_H__

#include <vector>

#include "odb_struct.h"
//#include "where_syntax_anls.h"
//#include "interpreter_define.h"

using namespace std;

//class FOR_WHERE::CWhereSyntaxAnalyser;

/*
#define LEFT_SIDE    0x10
#define RIGHT_SIDE   0x20
*/
/*
#ifdef  WIN32
//为NT版动态库增加
#ifdef ODB_WHERE_EXPORTS
#define ODB_WHERE_API __declspec(dllexport)
#else
#define ODB_WHERE_API __declspec(dllimport)
#endif
#endif
*/

/*
struct CODE_TABLE_STRU;
class CWhereSyntaxAnalyser;
class CInterpreter;
*/

namespace FOR_WHERE
{
	class CWhereSyntaxAnalyser;
}
	
namespace  ODB
{
	
//struct SQL_KEY_ID
//{
////	int   table_no;
////	short field_no;
//	int   record_id;
//	short column_id;
//};
//
//struct SQL_APP_ID
//{
//	int   app_id;
//	int   record_id;
//};
//
//struct SQL_APP_KEY
//{
//	int   app_id;
//	int   record_id;
//	short column_id;
//};

typedef long SQL_KEY_ID;

struct SQL_APP_KEY
{
	int app_id;
	SQL_KEY_ID key_id;
};

typedef SQL_APP_KEY SQL_APP_ID;

union SQL_DATA_TYPE
{
	//char       to_str[40];
	char        to_char;
	char*       to_str;
	short       to_short;
	int         to_int;
	long      	to_long;
	unsigned int to_uint;
	float       to_float;
	double      to_double;
	SQL_KEY_ID  to_key;
	SQL_APP_ID  to_appid;
	SQL_APP_KEY to_appkey;
	//binary
};

/*
#ifdef  WIN32
struct ODB_WHERE_API FIELD_STRU
#else
struct FIELD_STRU
#endif
*/
struct FIELD_STRU
{
	char field_name[64];   //CHN_NAME_LEN
	int  data_type;
	std::vector<SQL_DATA_TYPE> vec_value;
//	std::vector<SQL_APP_KEY> vec_stru_value;
};


struct SQLIDX_STRU
{
	char field_name[64];
	int  data_type;
	char field_no;
	int is_keyword;
};

struct IDX4SEARCH_STRU
{
	enum SIDE{ UNKNOW = 0x1 , LEFT_SIDE = 0x10 , RIGHT_SIDE = 0x20};
	
	int field_no;
	int data_type;
	int operation_type;  // == >= <=
	SQL_DATA_TYPE value;
	int taken_num;
	SIDE side;
	int is_keyword;
	
	
	IDX4SEARCH_STRU()
	{
		this->data_type = 0;
		this->taken_num = 0;
		this->operation_type = -1;
		this->side = UNKNOW;
		this->is_keyword = 0;
	
	}
	int clear()
	{
		this->field_no = -1;
		this->data_type = 0;
		this->operation_type = -1;
		this->value.to_int = -1;
		this->taken_num = 0;
		this->side = UNKNOW;
		this->is_keyword = 0;
		return 1;
	}
};


/*
#ifdef  WIN32
class ODB_WHERE_API CWhere
#else
class CWhere
#endif
*/

	
class CWhere
{
public:	
	CWhere();
/*	{ 
		sql_translate = NULL;
	};*/
	~CWhere();
	/*{
		 if(sql_translate)
		 	{
				delete sql_translate;
			}
	};*/
	bool GetWhere(std::vector<bool>& vec_where, const char* where_str, const std::vector<FIELD_STRU>& vec_sql);
	void Show();
	
	int ExpressSql(char * where_str , std::vector<SQLIDX_STRU> & vec_sqlidx, std::vector<IDX4SEARCH_STRU> & idx_vec);
	
	private:
	FOR_WHERE::CWhereSyntaxAnalyser* sql_translate;
/*
private :
	

  float  real_const_tab[MAX_REALCONSTTABLE_LEN];   //实常量表
  int    real_const_tab_len;
  char   *string_tab[MAX_STRINGTABLE_LEN];         //字符串表
  int    string_tab_len;
  CODE_TABLE_STRU*    code_tab[MAX_CODETABLE_LEN];  //中间代码表
  int    code_tab_len;   
  CWhereSyntaxAnalyser* sql_translate;              //语法分析器
  CInterpreter* sql_itprt;                          //解释器

*/
};

}
#endif
