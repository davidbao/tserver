/*******************************************************************************
ModuleName:   ODB Real Time Database management system
FileName:	  odb_field.h
DESCRIPTION:  class table struct define
FUNCTION LIST:
COMMENT:      field struct define
History:                                                        
Date          Version     Modifier                Activities
2002-10-10    1.0                              modify
================================================================================
2002-10-10    1.0                              created
*******************************************************************************/
#ifndef	__ODB_FIELD_H__
#define	__ODB_FIELD_H__

#include "odb_public.h"
//#include "odb_struct.h"
#include "odb_prv_struct.h"
#include "odb_common.h"


using namespace std;

namespace  ODB
{

//字段访问类
class COdbField
{
public:
	COdbField();
	COdbField(const char* db_area_ptr); 				 //表头位置
	COdbField(const char* db_area_ptr, int r_field_no);  //表头位置 + 商用字段号
	COdbField(const char* db_area_ptr, char* field_name);
	~COdbField();
			
	int SetFieldTabPtr(const char* db_area_ptr);//设置本字段控制区信息
		
	int SetRecordPtr(char* record_ptr);   //设置表的当前记录位置
		
	//移动到指定字段位置
	int MoveTo(const int r_field_no);          //根据field_no定位到当前app,table的STDB_FIELD_TAB
	int MoveTo(const char* field_name);
	int MoveToByChn(const char* field_name);
	
	//查询
	char* GetFieldName();                       //return a pointer
	int   GetFieldName(char* field_name);       //get_fieldname_by_fieldno()//copy to a array
	int   GetFieldChnName(char* field_name);
	int   SetFieldName(const char* field_name); //set chinese field name
	int   GetFieldNo(bool is_real = false);     //get_fieldno_by_fieldname()
	
	int   GetFieldPara(int& data_type, int& offset, int& field_length, int& is_keyword);  //get_field_para_byno()
	int   GetFieldLength(int& field_length);
	
	int   GetLimitValue(char* field_value_ptr, const int buf_size); //limited data length
	int   GetFieldValue(char** field_value_ptr);
	int   GetFieldValue(char** field_value_ptr, int& field_length);
	int   GetFieldValue(char** field_value_ptr, int& field_length, const int r_field_no);
	int   SetFieldValue(const char* modify_field_ptr, const int modify_field_length);

private:
	COdbField(const COdbField& odb_field);
	COdbField& operator=(const COdbField& odb_field);
						       	
private:
//	struct STDB_TAB*        stdb_tab_ptr;       //表首指针
//	struct STDB_FIELD_TAB*	stdb_field_tab_ptr; //字段首控制区信息,从OdbTable得到
//	struct STDB_FIELD_TAB*	cur_field_tab_ptr;  //当前字段控制区信息
//	char*  cur_record_ptr;                      //当前记录数据
	
	struct STDB_TAB*        m_StdbTabPtr;       //表首指针
	struct STDB_FIELD_TAB*	m_StdbFieldTabPtr;  //字段首控制区信息,从OdbTable得到
	struct STDB_FIELD_TAB*	m_CurFieldTabPtr;   //当前字段控制区信息
	char*  m_CurRecordPtr;                      //当前记录数据
};

}

#endif
