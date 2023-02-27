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

//�ֶη�����
class COdbField
{
public:
	COdbField();
	COdbField(const char* db_area_ptr); 				 //��ͷλ��
	COdbField(const char* db_area_ptr, int r_field_no);  //��ͷλ�� + �����ֶκ�
	COdbField(const char* db_area_ptr, char* field_name);
	~COdbField();
			
	int SetFieldTabPtr(const char* db_area_ptr);//���ñ��ֶο�������Ϣ
		
	int SetRecordPtr(char* record_ptr);   //���ñ�ĵ�ǰ��¼λ��
		
	//�ƶ���ָ���ֶ�λ��
	int MoveTo(const int r_field_no);          //����field_no��λ����ǰapp,table��STDB_FIELD_TAB
	int MoveTo(const char* field_name);
	int MoveToByChn(const char* field_name);
	
	//��ѯ
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
//	struct STDB_TAB*        stdb_tab_ptr;       //����ָ��
//	struct STDB_FIELD_TAB*	stdb_field_tab_ptr; //�ֶ��׿�������Ϣ,��OdbTable�õ�
//	struct STDB_FIELD_TAB*	cur_field_tab_ptr;  //��ǰ�ֶο�������Ϣ
//	char*  cur_record_ptr;                      //��ǰ��¼����
	
	struct STDB_TAB*        m_StdbTabPtr;       //����ָ��
	struct STDB_FIELD_TAB*	m_StdbFieldTabPtr;  //�ֶ��׿�������Ϣ,��OdbTable�õ�
	struct STDB_FIELD_TAB*	m_CurFieldTabPtr;   //��ǰ�ֶο�������Ϣ
	char*  m_CurRecordPtr;                      //��ǰ��¼����
};

}

#endif
