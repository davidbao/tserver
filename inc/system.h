/*******************************************************************************
FileName:    system.h
DESCRIPTION: system constants definitions
COMMENT:     
History:                                                        
Date          Version     Modifier                Activities
================================================================================
2009-06-01    1.0         ZMY                     modify
2009-04-09    1.0         GY                      created
*******************************************************************************/

#ifndef __SYSTEM_h__
#define __SYSTEM_h__

#include "port_def.h"

//应用如SCADA，指一组进程，加上对应的实时库数据实体
//服务如DATA_SRV，指一组进程，但没有对应的实时库数据实体
//PUBLIC应用下的进程，提供公共服务，运行在所有服务器上
//BASE_SERVICE服务的进程，提供公共服务，运行在服务器和工作站上
//工作站上没有实时库


//hpux FALSE AND TRUE definition
#ifdef _HPUX
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#endif

#ifdef _SUN
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#endif

#ifdef _LINUX
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0
#endif
#endif

#define GET_RECORD_ID(a) ((struct COMM_KEY_STRU*)(&a))->record_id
#define GET_TABLE_NO(a) ((struct COMM_KEY_STRU*)(&a))->table_no
#define GET_COLUMN_ID(a) ((struct COMM_KEY_STRU*)(&a))->column_id
#define GET_AREA_ID(a) ((struct KEY_STRU_AREA*)(&a))->area
#define GET_RECORD_ID_IN_AREA(a) ((struct KEY_STRU_AREA*)(&a))->key

#define SET_RECORD_ID(a,b) ((struct COMM_KEY_STRU*)(&a))->record_id=b
#define SET_TABLE_NO(a,b) ((struct COMM_KEY_STRU*)(&a))->table_no=b
#define SET_COLUMN_ID(a,b) ((struct COMM_KEY_STRU*)(&a))->column_id=b
#define SET_AREA_ID(a,b) ((struct KEY_STRU_AREA*)(&a))->area=b
#define SET_RECORD_ID_IN_AREA(a,b) ((struct KEY_STRU_AREA*)(&a))->key=b

// language definition
#define LOCAL_LANGUAGE		"gbk"


// context definitions

//#define AC_MAX_NUM		5 //del by tcr 2007/03/16
#define AC_MAX_NUM		  16 

#define AC_DEFAULT_NO		0
 
#define AC_REALTIME_NO 	1
#define AC_STUDY_NO	    2
#define AC_RESEARCH_NO  2
#define AC_PLAN_NO	    3
#define AC_FUTURE_NO	3
#define AC_TEST_NO	    4
#define AC_PDR_NO	      5
#define AC_DTS_NO	      6
#define AC_TRAINING_NO	6
#define AC_SYS_NO       15

#define AC_REALTIME_NAME	"realtime"
#define AC_RESEARCH_NAME	"study"
#define AC_STUDY_NAME	    "study"
#define AC_TRAINING_NAME	"training"
#define AC_PLAN_NAME	    "plan"
#define AC_TEST_NAME	    "test"
#define AC_PDR_NAME	      "pdr"
#define AC_DTS_NAME       "dts"
#define AC_SYS_NAME       "sys"
#define AC_FUTURE_NAME       "plan"

#define AC_REALTIME_MSG	  "realtime"
#define AC_TRAINING_MSG	  "training"
#define AC_PLAN_MSG	      "plan"
#define AC_TEST_MSG	      "test"
#define AC_PDR_MSG	      "pdr"
#define AC_DTS_MSG        "dts"

// application definitions

#define MAX_AF_NUM	   64	/* 1 ~ 32768 */

#define AF_SCADA	     1
#define AF_PAS		     2
#define AF_DTS		     3
#define AF_FES		     4
#define AF_AGC		     5
#define AF_AVC		     6
#define AF_WAMS		     7
//#define AF_CIM		     8
#define AF_DSCADA	     8
#define AF_WDM         9
#define AF_TMR         10
#define AF_DESUFS      11
#define AF_SCHEDULE    12
#define	AF_DSA	       13
#define AF_LLS         14
#define	AF_OMS	       15
#define AF_PUBLIC	     16
#define AF_OSM	     17      //2015-04-23

#define AF_DATA_SRV    33
#define AF_BASE_SRV    34
#define AF_REPORT_SRV  35

#define 	  AP_SCADA				 100000
#define 	  AP_PAS_MODEL		 200000
#define 	  AP_DTS         	 300000
#define 	  AP_FES					 400000
#define     AP_FES_GPS       410000
#define     AP_AGC					 500000
//#define     AP_AVC_SVC       600000
#define     AP_WAMS       	 700000
//#define     AP_CIM					 800000
#define         AP_DSCADA       800000
#define     AP_WDM           900000
#define     AP_TMR           1000000
#define     AP_DESUFS        1100000
#define     AP_SCHEDULE      1200000
#define     AP_DSA           1300000
#define     AP_LLS           1400000
#define     AP_OMS           1500000
#define     AP_PUBLIC        1600000
#define     AP_OSM        1700000    //2015-04-23

#define     AP_DATA_SRV      3300000
#define     AP_GIS_SRV       3310000
#define     AP_BASE_SRV      3400000 
#define		AP_REPORT_SRV	 3500000
   
#define     AP_DEFAULT	     -999


#define AF_SCADA_NAME	    "scada"
#define AF_DSCADA_NAME	    "dscada"
#define AF_PAS_NAME	      "pas"
#define AF_DTS_NAME	      "dts"
#define AF_FES_NAME	      "fes"
#define AF_AGC_NAME	      "agc"
#define AF_AVC_NAME	   "avc"
#define AF_WAMS_NAME		  "wams"
#define AF_CIM_NAME		    "cim"
#define AF_WDM_NAME       "wdm"
#define AF_TMR_NAME       "tmr"
#define AF_DESUFS_NAME    "desufs"
#define AF_SCHEDULE_NAME  "schedule"
#define	AF_DSA_NAME	      "dsa"
#define	AF_LLS_NAME	      "lls"
#define	AF_OMS_NAME	      "oms"	
#define AF_PUBLIC_NAME	  "public"
#define AF_OSM_NAME       "osm"

#define AF_DATA_SRV_NAME  "data_srv"
#define AF_BASE_SRV_NAME	"base_srv"
#define AF_REPORT_SRV_NAME	"report_srv"


#define AF_AP_MOD	           100000
#define AP_RESEARCH_MOD     100

#define     AP_PAS_RTNET	201100
#define     AP_PAS_RTNET	201100
#define AP_PAS_DPF	        201200
#define AP_PAS_LF	        201300
#define AP_PAS_NETCA	        201400
#define AP_PAS_FAULT		201500
#define AP_PAS_BUSLF		201600
#define AP_PAS_SENS		201700
#define AP_PAS_OPF		201800
#define AP_PAS_NETEQ		201900
#define AP_PAS_SCD		202000
#define AP_PAS_RTS  		202100
#define AP_PAS_SCED 		202200
#define AP_PAS_AVC		202500
#define AP_PAS_PLAN		202600
#define AP_PAS_AVCDF    	202700
#define AP_PAS_GENSK    	202800
#define AP_PAS_STUSQA      202900
#define AP_PAS_STUSQB      203000
#define AP_PAS_ATC      203100

#define AP_WAMS_FES     702100
	
#define AP_PAS_RESEARCH_1 202900
#define AP_PAS_RESEARCH_2 203000

#define AP_SCADA_SIM            101000
#define AP_SCADA_RED            1110
#define AP_SCADA_RELAY			102000
#define AP_SCADA_ADM			103000
#define AP_SCADA_IFA			104000
#define AP_SCADA_ISW			105000
#define AP_SCADA_OAA			106000
#define AP_SCADA_LTA			107000
#define AP_SCADA_OSM      108000
#define AP_OSM            108000
#define AP_SCADA_CASE           109000
#define AP_DFES           490000
#define AP_AVC					600000
#define AP_AVC_SVC				604000
#define AP_AVC_CTLNS            601000
#define AP_AVC_ROPF            602000

//SCHEDULE
#define AP_SCHEDULE_LF  1200100
#define AP_SCHEDULE_BUSLF  1200200
#define AP_SCHEDULE_RPP  1210100
#define AP_SCHEDULE_IPP  1210200
#define AP_SCHEDULE_SPP  1210300
#define AP_SCHEDULE_MPP  1210400
#define AP_SCHEDULE_LPP  1210500
#define AP_SCHEDULE_ROP  1220100
#define AP_SCHEDULE_SOP  1220200
#define AP_SCHEDULE_WOP  1220300
#define AP_SCHEDULE_MOP  1220400
#define AP_SCHEDULE_LOP  1220500

// 

#define     HOME_ENV_NAME	   "D5000_HOME"

// column IDs

#define COLID_T0002_COLUMN_SPECIAL	31

//DB FAULT STATUS (1+N)
#define DB_FAULT_STATUS      -201


#define  SET_DATA	4508
#define  CONF_DIR  "conf"
#define  MAX_MSG_PACKET_SIZE 32767

#endif

