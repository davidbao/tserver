//#include "odb_tableop.h"
#include "system.h"
#ifndef _TAB2IDX
#define _TAB2IDX
namespace ODB
{
#define	ALL_INDEX -2
#define 	PK_INDEX -1
	class CTableOp;
	class CBaseIndex;
	class Tab2Idx
	{
	private:
		Tab2Idx(Tab2Idx &orig_class);
		int m_AppNo;
		int m_TableNo;
		short m_ContextNo;

		int WriteTimeAndIdxStatusNormal(CTableOp& theTableOp,int idx_pos = ALL_INDEX);//idx_pos == -2 means all indexes including pk index, -1 means primary key index, 0 means the first index and so on.
		int WriteIdxStatusInit(CTableOp &theTableOp,int idx_pos = ALL_INDEX);//idx_pos == -2 means all indexes including pk index, -1 means primary key index, 0 means the first index and so on.
		CBaseIndex *GetPkIndexPtr(struct STDB_TAB* stdb_tab_ptr,int table_no);
		CBaseIndex *GetIndexPtr(int seq,struct INDEX_DEFINE_TAB *idx_def_tab_ptr,struct STDB_TAB* stdb_tab_ptr,int table_no);
	public:
		Tab2Idx(){};
		Tab2Idx(int app_no,int table_no,short ctx_no= AC_REALTIME_NO);
		int SetPara(int app_no,int table_no,int ctx_no = AC_REALTIME_NO);
		int CreateIdxFromTab(); //This function can create and repair all the indexes including pk index in the table
		int RepairIdxFromTab(int idx_pos);//idx_pos == -1 means primary key index, 0 means the first index and so on. It is only used for repairing index , so it is not used for create index for a table not down_load or creating index for case
		//int RmIdx();
		int RmIdxInfo();
		
		int CreateIdxFromAreaTab(); //This function can create and repair all the indexes including pk indexes in the area table
		int RepairIdxFromAreaTab(int idx_pos);//idx_pos == -1 means primary key index, 0 means the first index and so on. It is only used for repairing index , so it is not used for create index for a table not down_load or creating index for case
		int RmAreaIdxInfo();
	};
}
#endif
