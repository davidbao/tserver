/*******************************************************************************
FileName:    port_def.h
DESCRIPTION: port definitions
COMMENT:     
History:                                                        
Date          Version     Modifier                Activities
================================================================================
2004-07-09    1.0         YZH                     created
2004-07-19    1.0         YZH                     modify
*******************************************************************************/

#ifndef __PORT_DEF_h__ 
#define __PORT_DEF_h__

// 10000 - 10999 : ports not relative to context 
// 11000 - 11999 : ports assigned to context01 ( for real-time )
// 12000 - 12999 : ports assigned to context02 ( for training )
// 13000 - 13999 : ports assigned to context04 ( for research1 )
// 14000 - 14999 : ports assigned to context08 ( for research2 )

// real-time 
// 11000 - 11099 : platform - public
// 11100 - 11199 : platform - db_service
// 11200 - 11299 : scada/agc
// 11300 - 11399 : pas
// 11400 - 11499 : dts
// 11500 - 11699 : fes
// 11700 - 11999 : reserved

//sys_admin
const int MNG_SERV_UDP_PORT       =   8001;
const int MNG_SERV_TCP_PORT       =   8003;
const int MNG_SERV_TCPREXEC_PORT  =   8005;
const int MNG_SERV_COLD_PORT      =   8007;
const int MNG_SENDRECV_PORT       =   28001;
const int MNG_UNISEND_PORT        =   28002;
const int MNG_NODENET_PORT        =   28008; //copy from jsems08-1 by jxc.
// web sync
#define PORT_WEB_SYNC_RT	9001
#define PORT_WEB_SYNC_MODIFY	9002
#define PORT_WEB_SYNC_RDB	9003
#define PORT_WEB_SYNC_RDB_LOB	9004

#define PORT_MIDMMI_SERVER	10001
#define PORT_FILE_SERVER	10012	//10010
#define PORT_PROXY_SERVER  	30009
#define PORT_EVENT_SERVER	10027

//case_mange(model\mode\msg)
#define PORT_CASE_MODEL		10888

#define PORT_RTDB		11000
#define PORT_SYNC_RT		11006  //sync_rt_recv -> sync_rt_write

#define PORT_FILE_RECV          11105

#define PORT_DB_MONITOR		11100
#define PORT_DB_COMMIT_SAMPLE	11101
#define PORT_DB_COMMIT_SCADA	11102
#define PORT_DB_COMMIT_ALARM    11103
#define PORT_DY_COMMIT_SCADA    11140
#define PORT_DY_COMMIT_WAMS     11150

#define PORT_DB_DOWNLOAD	11110       // download_server
#define PORT_DB_MODIFY		11111       // db_modify_server
#define PORT_DB_SQL_SP          11112       // sql_sp_server
#define PORT_DB_SQL_SP_MSG      11113       // sql_sp_server for msg save
#define PORT_DB_CASE_GRAPH      11114       // case_graph_server
#define PORT_DB_SQL_SP_QUERY    11115       // sql_sp_server for query
#define PORT_DB_MODEL_MODIFY	11116		// model_modify
#define PORT_DB_MODEL_QUERY		11117		// model_query
#define PORT_DB_DOWNLOAD_DAEMON	11118		// download daemon
#define PORT_DB_DATA_MODIFY     11119           // data_modify

#define PORT_DB_SAMPLE_DEFINE   11120       // sample_define_server
#define PORT_DB_SAMPLE_DATA     11121       // sample_data_server
#define PORT_DB_TRIGGER_SAMPLE  11122       // trigger_sample_server
#define PORT_DB_QUERY_SAMPLE_PI    11124       // query_sample_server_pi for PI only
#define PORT_DB_QUERY_SAMPLE    11125       // query_sample_server
#define PORT_DB_QUERY_REPORT    11126       // query_report_server
#define PORT_DB_SAMPLE_MODIFY   11127       // sample_modify_server
#define PORT_DB_QUERY_STAT      11128       // query_stat_server
#define PORT_TAG_DEFINE_PROCESS	11129	    // tag_define_process
#define PORT_DY_SAMPLE_DEFINE	11130	    // dy_sample_define_server
#define PORT_DY_STAT_SERVER	11131	    // dy_stat_server
#define PORT_DB_SQL_SP_HISDB    11132      // sql_sp_server for msg save hisdb
#define PORT_DB_SQL_SP_SCHEDULE 11133      // sql_sp_server for msg save schedule
#define PORT_DY_COMMIT_SCADA	11140       // dy_commit scada can use 11140~11149
#define PORT_DY_COMMIT_WAMS	    11150       // dy_commit wams can use 11150~11159

#define PORT_WARN_SERVICE 	11170  	    // for receiving local warn message.  copy from jsems08-1 by jxc.
#define PORT_PRIV_SERVER	11179		// priv_server

#define PORT_DB_QUERY_SAMPLE_PI1    11180 //query_sample_server服务的PI取数1号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI2    11181 //query_sample_server服务的PI取数2号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI3    11182 //query_sample_server服务的PI取数3号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI4    11183 //query_sample_server服务的PI取数4号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI5    11184 //query_sample_server服务的PI取数5号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI6    11185 //query_sample_server服务的PI取数6号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI7    11186 //query_sample_server服务的PI取数7号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI8    11187 //query_sample_server服务的PI取数8号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI9    11188 //query_sample_server服务的PI取数9号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI10   11189 //query_sample_server服务的PI取数10号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI11   11190 //query_sample_server服务的PI取数11号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI12   11191 //query_sample_server服务的PI取数12号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI13   11192 //query_sample_server服务的PI取数13号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI14   11193 //query_sample_server服务的PI取数14号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI15   11194 //query_sample_server服务的PI取数15号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI16   11195 //query_sample_server服务的PI取数16号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI17   11196 //query_sample_server服务的PI取数17号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI18   11197 //query_sample_server服务的PI取数18号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI19   11198 //query_sample_server服务的PI取数19号子进程端口
#define PORT_DB_QUERY_SAMPLE_PI20   11199 //query_sample_server服务的PI取数20号子进程端口

//scada
#define PORT_SCADA_STATISTICS   11200       //scada_statistics_server
#define PORT_SCADA_CTRL         11202       //sca_ctrl_server

// pas
#define PORT_LF_SERVER		11300		// lf_server

//reserved
#define PORT_HDAI_NARI		11701	    //hdai_nari_server


#define MSG_PROXY_PORT          12063       //message bus proxy for web 
#define MSG_PROXY_BROAD_PORT	12064
#define MSG_PROXY_WEB_PORT      12065       //for web client
#define MSG_PROXY_WEB_SERVER_PORT 12066
#define MSG_TEST_PORT           12068       //for test mulitple connections

#define	PORT_WEB_UNIQUE			12069
#define PORT_SVG_PRIV_SERVER	12071
#define	PORT_WEB_HOST_SERVER	12075
#define	PORT_WEB_WARN_SERVER	12076
#define PORT_WEB_REPORT_SERVER	12077

#define	PORT_TOMCAT1			8000
#define	PORT_TOMCAT2			8080
#define	PORT_TOMCAT3			8009
#define PORT_TOMCAT4			8443
#define PORT_WEBLOGIC1			7001

#define PORT_OP_TICKET_SIMU     20000
#define PORT_FLOW_INTERFACE     22222


// fes
//#define PORT_FES_READ_VAR	from 11500 to 15100
#define PORT_FES_READ_VAR	11500
#define FES_DB_SEND_PORT       	11590
#define FES_DB_RECV_PORT       	11691
//#define MOXA_PORT		from 950 to 981
//#define CISCO_PORT		from 4033 to 4048
//#define CHASE_PORT		from 10001 to 10016
//#define NET104_PORT		from 2400 to 2599

//121** reserved for sync_rt
#define PORT_SYNC_RT_CTX_1  12100
#define PORT_SYNC_RT_CTX_2  12101
#define PORT_SYNC_RT_CTX_4  12102
#define PORT_SYNC_RT_CTX_8  12103
#define PORT_SYNC_RT_CTX_16 12104
//

//switch
#define PORT_OPTICKET_TOPO_SERVER	30000
#endif 

