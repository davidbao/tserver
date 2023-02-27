#ifndef __CODB_NET_H__
#define __CODB_NET_H__

#include "odb_net_m.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
//#include "serviceglobal.h"
#include "servicesclient.h"

namespace RTDB_SERVER
{
	class CODB_NET
	{
		public:
		CODB_NET();
		~CODB_NET();
		
		public:
		int TableCreate(REQ_STDB_CREATE& req_create);//1
		int TableDelete(REQ_STDB_DELETE& req_delete);//2
				
		int TableGet(REQ_READ& read_req, RSP_READ& read_rsp);//3
		int TableGetByName(REQ_READ& read_req, RSP_READ& read_rsp);//4
		int TableGetByFieldValue(REQ_CON& read_req, RSP_CON& read_rsp);//5
		int TableWrite(REQ_KEY_WRITE& write_req);//6
		int TableModify(REQ_MODIFY& modify_req);//7
		int TableUpdate(REQ_UPDATE& update_req);//8
		int DeleteRecord(REQ_KEY_ERASE& delete_req);//9
		int TableClear(REQ_TABLE_CLEAR& clear_req);//10
				
		int GetTablePara(REQ_PARAMETER& para_req, RSP_PARAMETER& para_rsp);//11
		int GetTableParameter(REQ_PARAMETER& para_req, RSP_READ& para_rsp);//12
		int GetAppTableParameter(REQ_APP_PARAMETER& para_req, RSP_APP_PARAMETER& para_rsp);//13
		int GetFieldPara(REQ_FIELD_BASE_INFO& para_req , RSP_FIELD_BASE_INFO& para_rsp);//14
				
		int GetKeyInfo(RSP_KEYINFO& key_rsp);//15
		int TableGetBySql(REQ_SQL& select_req, RSP_READ& select_rsp);//16
		int MenuRead(REQ_MENU& menu_req, RSP_MENU& menu_rsp);//17
		int TableReleMenuRead(REQ_RELE_MENU& rele_req, RSP_RELE_MENU& rele_rsp);//18
		int GetNameStringByKeyID(REQ_NAME_STRING& name_req, RSP_NAME_STRING& name_rsp);//19
		int GetRefMenuString(REQ_REF_MENU& ref_req, RSP_REF_MENU& ref_rsp);//20
				
		int GetNameStringByMultiKeyID(REQ_MULTI_NAME_STRING& name_req, RSP_MULTI_NAME_STRING& name_rsp);//21
			
		int GraphGetData(GRAPH_REAL_ODB_REQ& real_req, GRAPH_REAL_RSP& real_rsp);//22
		int GraphGetAppInfo(RTDB_SERVER::GRAPH_ORDER_KEY_REQ& app_req, RTDB_SERVER::GRAPH_ORDER_KEY_RSP& app_rsp);//23
				
		int ExchangeNameNo(REQ_ENTITY& object_req, RSP_ENTITY& object_rsp);//24
				
		int GetFacNameByKeyNo(REQ_FAC& fac_req, RSP_FAC& fac_rsp);//25
		int GetFacNameByFacID(REQ_FAC& fac_req, RSP_FAC& fac_rsp);//26
		int GetFacNameByFacNo(REQ_FAC& fac_req, RSP_FAC& fac_rsp);//27
				
		int GetFacIDByFacName(REQ_FAC& fac_req, RSP_FAC& fac_rsp);//28
		int GetFacNoByFacName(REQ_FAC& fac_req, RSP_FAC& fac_rsp);//29
				
		int GetFacIDByFacNo(REQ_FAC& fac_req, RSP_FAC& fac_rsp);//30
		int GetFacNoByFacID(REQ_FAC& fac_req, RSP_FAC& fac_rsp);//31
				
		int GetValueAndStatus(REQ_VAL_STA& req_key_id, RSP_INT_VAL_STA* rsp_int_val, RSP_FLOAT_VAL_STA* rsp_float_val, RSP_CHAR_VAL_STA* rsp_char_val, RSP_LONG_VAL_STA* rsp_long_val, RSP_UNION_VAL_STA* rsp_union_val);//32
				
		int PingRtdbServer(int req_data , int& rsp_data);//33
		int GraphDataSend(GRAPH_REAL_ODB_REQ &real_req,Handle &hdl) ;//22
		int GraphDataRecv(GRAPH_REAL_RSP &real_rsp,Handle &hdl,int &ret);
		
		int DeleteRecords(DEL_REQ& delete_req);//34
		
		public:
		int SetServiceInfo(ServiceInfo& service_info);
		int SetDomainInfo(const DomainInfo* p_domain_info);
		void SetHandle(const Handle handle);
		void ClearHandle(const Handle handle);
		static CODB_NET* _narrow(ServiceInfo& serv_info, DomainInfo* p_domain_info = NULL);
		
		private:
		ServiceInfo m_Serviceinfo;
		DomainInfo* m_pDomaininfo;
		char m_IpAddress[64];
		int m_Port;
		Handle m_Handle;
	};
}

#endif
