/*******************************************************************************
  Copyright (c) 2023 com.

  All rights reserved. This program and the accompanying materials
  are made available under the terms of the EaseSuite Public License v1.0
  and EaseSuite Distribution License v1.0 which accompany this distribution.

  Only for kingbase v7.

  Contributors:
     Baowei - initial version
*******************************************************************************/

--CREATE TABLESPACE TSERVER(NAME = TSERVER FILENAME = 'TSERVER' SIZE = 100 MAXSIZE = 16000 FILEGROWTH = 10);

-- 1. 首页表
-- --------------------------------------------
-- Table structure for home_total
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_TOTAL";
INSERT INTO TSERVER.HOME_TOTAL
("ID", "P_S_CAPACITY", "P_POWER", "P_P_CAPACITY", "P_A_CAPACITY", "P_MAX_LOAD", "D_S_CAPACITY", "D_POWER",
 "D_P_CAPACITY", "D_A_CAPACITY", "D_MAX_LOAD", "DAY_EPENCENT", "TOTAL_INCOME", "DAY_VROC", "TOTAL_P_INCOME")
VALUES ('1734524036022534145', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_elimination
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_ELIMINATION";
INSERT INTO "TSERVER"."HOME_ELIMINATION"
("ID", "P_YEAR_PENCENT", "P_MONTH_PENCENT", "P_DAY_PENCENT", "D_YEAR_PENCENT", "D_MONTH_PENCENT", "D_DAY_PENCENT")
VALUES ('1734524036022534146', '1', '2', '3', '4', '5', '6');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_vroc
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_VROC";
INSERT INTO "TSERVER"."HOME_VROC"
("ID", "P_YEAR_VROC", "P_MONTH_VROC", "P_DAY_VROC", "D_YEAR_VROC", "D_MONTH_VROC", "D_DAY_VROC")
VALUES ('1734524036022534147', '1', '2', '3', '4', '5', '6');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_p_dload_power
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_P_DLOAD_POWER";
INSERT INTO "TSERVER"."HOME_P_DLOAD_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534148', '00:00:00', '1');
INSERT INTO "TSERVER"."HOME_P_DLOAD_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534149', '01:00:00', '2');
INSERT INTO "TSERVER"."HOME_P_DLOAD_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534150', '02:00:00', '3');
INSERT INTO "TSERVER"."HOME_P_DLOAD_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534151', '03:00:00', '4');
INSERT INTO "TSERVER"."HOME_P_DLOAD_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534152', '04:00:00', '5');
INSERT INTO "TSERVER"."HOME_P_DLOAD_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534153', '05:00:00', '6');
INSERT INTO "TSERVER"."HOME_P_DLOAD_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534154', '06:00:00', '7');
INSERT INTO "TSERVER"."HOME_P_DLOAD_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534155', '08:00:00', '8');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_p_pgeneration_power
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_P_PGENERATION_POWER";
INSERT INTO "TSERVER"."HOME_P_PGENERATION_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534156', '00:00:00', '8');
INSERT INTO "TSERVER"."HOME_P_PGENERATION_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534157', '01:00:00', '7');
INSERT INTO "TSERVER"."HOME_P_PGENERATION_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534158', '02:00:00', '6');
INSERT INTO "TSERVER"."HOME_P_PGENERATION_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534159', '03:00:00', '5');
INSERT INTO "TSERVER"."HOME_P_PGENERATION_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534160', '04:00:00', '4');
INSERT INTO "TSERVER"."HOME_P_PGENERATION_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534161', '05:00:00', '3');
INSERT INTO "TSERVER"."HOME_P_PGENERATION_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534162', '06:00:00', '2');
INSERT INTO "TSERVER"."HOME_P_PGENERATION_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534163', '08:00:00', '1');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_p_senergy_power
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_P_SENERGY_POWER";
INSERT INTO "TSERVER"."HOME_P_SENERGY_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534164', '00:00:00', '1');
INSERT INTO "TSERVER"."HOME_P_SENERGY_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534165', '01:00:00', '2');
INSERT INTO "TSERVER"."HOME_P_SENERGY_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534166', '02:00:00', '3');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_p_cgrid_power
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_P_CGRID_POWER";
INSERT INTO "TSERVER"."HOME_P_CGRID_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534167', '00:00:00', '3');
INSERT INTO "TSERVER"."HOME_P_CGRID_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534168', '01:00:00', '4');
INSERT INTO "TSERVER"."HOME_P_CGRID_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534169', '02:00:00', '5');
INSERT INTO "TSERVER"."HOME_P_CGRID_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534170', '03:00:00', '6');
INSERT INTO "TSERVER"."HOME_P_CGRID_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534171', '04:00:00', '7');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_p_battery_info
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_P_BATTERY_INFO";
INSERT INTO "TSERVER"."HOME_P_BATTERY_INFO"
    ("ID", "SOC1", "SOH1", "SOC2", "SOH2", "SOC3", "SOH3", "SOC4", "SOH4")
VALUES ('1734524036022534172', '1', '2', '3', '4', '', '', '', '');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_d_dload_power
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_D_DLOAD_POWER";
INSERT INTO "TSERVER"."HOME_D_DLOAD_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534173', '00:00:00', '1');
INSERT INTO "TSERVER"."HOME_D_DLOAD_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534174', '01:00:00', '2');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_d_pgeneration_power
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_D_PGENERATION_POWER";
INSERT INTO "TSERVER"."HOME_D_PGENERATION_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534175', '00:00:00', '5');
INSERT INTO "TSERVER"."HOME_D_PGENERATION_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534176', '01:00:00', '6');
INSERT INTO "TSERVER"."HOME_D_PGENERATION_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534177', '02:00:00', '7');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_d_senergy_power
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_D_SENERGY_POWER";
INSERT INTO "TSERVER"."HOME_D_SENERGY_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534178', '00:00:00', '0.7');
INSERT INTO "TSERVER"."HOME_D_SENERGY_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534179', '01:00:00', '0.8');
INSERT INTO "TSERVER"."HOME_D_SENERGY_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534180', '02:00:00', '0.9');
INSERT INTO "TSERVER"."HOME_D_SENERGY_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534181', '03:00:00', '1.0');
INSERT INTO "TSERVER"."HOME_D_SENERGY_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534182', '04:00:00', '1.1');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_d_cgrid_power
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_D_CGRID_POWER";
INSERT INTO "TSERVER"."HOME_D_CGRID_POWER"
    ("ID", "TIME", "VALUE")
VALUES ('1734524036022534183', '00:00:00', '11');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_d_battery_info
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_D_BATTERY_INFO";
INSERT INTO "TSERVER"."HOME_D_BATTERY_INFO"
    ("ID", "SOC1", "SOH1", "SOC2", "SOH2", "SOC3", "SOH3", "SOC4", "SOH4")
VALUES ('1734524036022534184', '6.7', '80.9', '', '', '', '', '', '');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for home_real_eflux
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."HOME_REAL_EFLUX";
INSERT INTO "TSERVER"."HOME_REAL_EFLUX"
    ("ID", "NAME", "FLAG", "VALUE1", "VALUE2", "VALUE3")
VALUES ('1734524036022534185', '光伏联动分布式储能', '1', '2', '3', '4');
INSERT INTO "TSERVER"."HOME_REAL_EFLUX"
    ("ID", "NAME", "FLAG", "VALUE1", "VALUE2", "VALUE3")
VALUES ('1734524036022534186', '交流负荷', '2', '5', '6', '7');
INSERT INTO "TSERVER"."HOME_REAL_EFLUX"
    ("ID", "NAME", "FLAG", "VALUE1", "VALUE2", "VALUE3")
VALUES ('1734524036022534187', '光伏联动分布式储能', '0', '8', '9', '10');
COMMIT TRANSACTION;

-- 2. 光储联动台区-储能运行状态表
-- --------------------------------------------
-- Table structure for p_seoverview_acdata
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_SEOVERVIEW_ACDATA";
INSERT INTO "TSERVER"."P_SEOVERVIEW_ACDATA"
("ID", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC", "PCS_COMM_ERROR", "PCS_CTRL_FAIL", "EMS_COMM_ERROR",
 "RACK_VDIFF_HIGH", "RACK_CDIFF_HIGH", "EM_SHUTDOWN", "AC_COMM_ERROR", "OVERV_3RD_LEVEL")
VALUES ('1734524036022534188', '1', '2', '3', '4', '5', '6', '7', '8', '1', '1', '1', '1', '1', '1', '1', '1');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_seoverview_accurve
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_SEOVERVIEW_ACCURVE";
INSERT INTO "TSERVER"."P_SEOVERVIEW_ACCURVE"
    ("ID", "TIME", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534189', '2023-01-01 00:00:00', '1', '2', '3', '4', '5', '6', '7', '8');
INSERT INTO "TSERVER"."P_SEOVERVIEW_ACCURVE"
    ("ID", "TIME", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534190', '2023-01-01 01:00:00', '2', '3', '4', '5', '6', '7', '8', '9');
INSERT INTO "TSERVER"."P_SEOVERVIEW_ACCURVE"
    ("ID", "TIME", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534191', '2023-01-01 02:00:00', '3', '4', '5', '6', '7', '8', '9', '10');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_seoverview_dcdata
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_SEOVERVIEW_DCDATA";
INSERT INTO "TSERVER"."P_SEOVERVIEW_DCDATA"
("ID", "P", "I", "U", "BC1_I", "BC1_U", "BC1_SOC", "BC1_SOH", "BC2_I", "BC2_U", "BC2_SOC", "BC2_SOH", "BC3_I", "BC3_U",
 "BC3_SOC", "BC3_SOH", "BC4_I", "BC4_U", "BC4_SOC", "BC4_SOH", "PCS_COMM_ERROR", "PCS_CTRL_FAIL", "EMS_COMM_ERROR",
 "RACK_VDIFF_HIGH", "RACK_CDIFF_HIGH", "EM_SHUTDOWN", "AC_COMM_ERROR", "OVERV_3RD_LEVEL")
VALUES ('1734524036022534192', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16',
        '17', '18', '19', '1', '0', '1', '0', '1', '0', '1', '0');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_seoverview_dccurve
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_SEOVERVIEW_DCCURVE";
INSERT INTO "TSERVER"."P_SEOVERVIEW_DCCURVE"
    ("ID", "TIME", "P", "I", "U")
VALUES ('1734524036022534193', '2023-01-01 00:00:00', '1', '2', '3');
COMMIT TRANSACTION;

-- 3. 光储联动台区-光伏运行状态表
-- --------------------------------------------
-- Table structure for p_poverview_station
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_POVERVIEW_STATION";
INSERT INTO "TSERVER"."P_POVERVIEW_STATION"
    ("ID", "PID", "NAME")
VALUES ('1734524036022534194', '1', '台区总光伏');
INSERT INTO "TSERVER"."P_POVERVIEW_STATION"
    ("ID", "PID", "NAME")
VALUES ('1734524036022534195', '2', '光伏电站001');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_poverview_overview
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_POVERVIEW_OVERVIEW";
INSERT INTO "TSERVER"."P_POVERVIEW_OVERVIEW"
("ID", "PID", "CAPACITY", "POWER", "MAX_POWER", "MIN_POWER", "DAILY_EP")
VALUES ('1734524036022534195', '1', '1', '2', '3', '4', '5');
INSERT INTO "TSERVER"."P_POVERVIEW_OVERVIEW"
("ID", "PID", "CAPACITY", "POWER", "MAX_POWER", "MIN_POWER", "DAILY_EP")
VALUES ('1734524036022534196', '2', '2', '3', '4', '5', '6');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_poverview_mesa
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_POVERVIEW_MESA";
INSERT INTO "TSERVER"."P_POVERVIEW_MESA"
    ("ID", "PID", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534197', '1', '1', '2', '3', '4', '5', '6', '7', '8');
INSERT INTO "TSERVER"."P_POVERVIEW_MESA"
    ("ID", "PID", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534198', '2', '2', '3', '4', '5', '6', '7', '8', '9');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_poverview_protect
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_POVERVIEW_PROTECT";
INSERT INTO "TSERVER"."P_POVERVIEW_PROTECT"
("ID", "PID", "LCL", "PROT_ACT", "OCSWITCH_ACT", "INTERVAL_EXCEPTION", "INTERVAL_FAIL", "INTERVAL_SINGAL",
 "MP_COMM_ERROR", "BH_POSITION")
VALUES ('1734524036022534199', '1', '0', '1', '0', '1', '0', '1', '0', '1');
INSERT INTO "TSERVER"."P_POVERVIEW_PROTECT"
("ID", "PID", "LCL", "PROT_ACT", "OCSWITCH_ACT", "INTERVAL_EXCEPTION", "INTERVAL_FAIL", "INTERVAL_SINGAL",
 "MP_COMM_ERROR", "BH_POSITION")
VALUES ('1734524036022534200', '2', '1', '0', '1', '0', '1', '0', '1', '0');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_poverview_curve
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_POVERVIEW_CURVE";
INSERT INTO "TSERVER"."P_POVERVIEW_CURVE"
("ID", "PID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA",
 "UB", "UC")
VALUES ('1734524036022534201', '1', '2023-01-01 00:00:00', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11',
        '12', '13', '14',
        '15', '16');
INSERT INTO "TSERVER"."P_POVERVIEW_CURVE"
("ID", "PID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA",
 "UB", "UC")
VALUES ('1734524036022534202', '1', '2023-01-01 01:00:00', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12',
        '13', '14', '15',
        '16', '17');
INSERT INTO "TSERVER"."P_POVERVIEW_CURVE"
("ID", "PID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA",
 "UB", "UC")
VALUES ('1734524036022534203', '1', '2023-01-01 02:00:00', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13',
        '14', '15',
        '16', '17', '18');
INSERT INTO "TSERVER"."P_POVERVIEW_CURVE"
("ID", "PID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA",
 "UB", "UC")
VALUES ('1734524036022534204', '2', '2023-01-01 00:00:00', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11',
        '12', '13', '14',
        '15', '16');
INSERT INTO "TSERVER"."P_POVERVIEW_CURVE"
("ID", "PID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA",
 "UB", "UC")
VALUES ('1734524036022534205', '2', '2023-01-01 01:00:00', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12',
        '13', '14', '15',
        '16', '17');
INSERT INTO "TSERVER"."P_POVERVIEW_CURVE"
("ID", "PID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA",
 "UB", "UC")
VALUES ('1734524036022534206', '2', '2023-01-01 02:00:00', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13',
        '14', '15',
        '16', '17', '18');
COMMIT TRANSACTION;

-- 4. 光储联动台区-台区运行状态表
-- --------------------------------------------
-- Table structure for p_aoverview_overview
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_AOVERVIEW_OVERVIEW";
INSERT INTO "TSERVER"."P_AOVERVIEW_OVERVIEW"
("ID", "P_REAL_POWER", "LOAD_POWER", "COMBINE_POWER", "DAILY_EP", "LOAD_EP", "TOTAL_EP")
VALUES ('1734524036022534207', '1', '2', '3', '4', '5', '6');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_aoverview_mesa
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_AOVERVIEW_MESA";
INSERT INTO "TSERVER"."P_AOVERVIEW_MESA"
    ("ID", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534208', '1', '2', '3', '4', '5', '6', '7', '8');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_aoverview_protect
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_AOVERVIEW_PROTECT";
INSERT INTO "TSERVER"."P_AOVERVIEW_PROTECT"
("ID", "LCL", "PROT_ACT", "OCSWITCH_ACT", "INTERVAL_EXCEPTION", "INTERVAL_FAIL", "INTERVAL_SINGAL", "MP_COMM_ERROR",
 "BH_POSITION")
VALUES ('1734524036022534209', '1', '0', '1', '0', '1', '0', '1', '0');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_aoverview_curve
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_AOVERVIEW_CURVE";
INSERT INTO "TSERVER"."P_AOVERVIEW_CURVE"
("ID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA", "UB",
 "UC")
VALUES ('1734524036023534210', '2023-01-01 00:00:00', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12',
        '13', '14', '15',
        '16');
INSERT INTO "TSERVER"."P_AOVERVIEW_CURVE"
("ID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA", "UB",
 "UC")
VALUES ('1734524036022534210', '2023-01-01 01:00:00', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13',
        '14', '15',
        '16', '17');
INSERT INTO "TSERVER"."P_AOVERVIEW_CURVE"
("ID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA", "UB",
 "UC")
VALUES ('1734524036022534211', '2023-01-01 02:00:00', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14',
        '15', '16',
        '17', '18');
COMMIT TRANSACTION;

-- 5. 光储联动台区-经济效益分析表
-- --------------------------------------------
-- Table structure for p_ebanalysis_overview
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_EBANALYSIS_OVERVIEW";
INSERT INTO "TSERVER"."P_EBANALYSIS_OVERVIEW"
("ID", "TOTAL_CHARGE_EP", "TOTAL_DISCHARGE_EP", "TOTAL_PG_EP", "TOTAL_PC_EP", "TOTAL_EB", "TOTAL_PVD_BENEFIT")
VALUES ('1734524036022534212', '1', '2', '3', '4', '99.67', '92.4');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for p_ebanalysis_curve
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."P_EBANALYSIS_CURVE";
INSERT INTO "TSERVER"."P_EBANALYSIS_CURVE"
("ID", "TIME", "CHARGE_EP", "DISCHARGE_EP", "PG_EP", "PC_EP", "EB", "PVD_BENEFIT", "PP_BENEFIT")
VALUES ('1734524036022534213', '2023-01-01', '1', '2', '3', '4', '5', '6', '7.34');
INSERT INTO "TSERVER"."P_EBANALYSIS_CURVE"
("ID", "TIME", "CHARGE_EP", "DISCHARGE_EP", "PG_EP", "PC_EP", "EB", "PVD_BENEFIT", "PP_BENEFIT")
VALUES ('1734524036022534214', '2023-01-01', '2', '3', '4', '5', '6', '7', '8.34');
COMMIT TRANSACTION;

-- 6. 光储联动台区-协调控制表
-- todo:

-- 7. 柔直互联台区-储能运行状态
-- --------------------------------------------
-- Table structure for d_seoverview_station
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_SEOVERVIEW_STATION";
INSERT INTO "TSERVER"."D_SEOVERVIEW_STATION"
    ("ID", "SID", "NAME")
VALUES ('1734524036022534230', '1', '储能设备001');
INSERT INTO "TSERVER"."D_SEOVERVIEW_STATION"
    ("ID", "SID", "NAME")
VALUES ('1734524036022534231', '2', '储能设备002');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_seoverview_dc1data
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_SEOVERVIEW_DC1DATA";
INSERT INTO "TSERVER"."D_SEOVERVIEW_DC1DATA"
("ID", "P", "I", "U", "BC1_I", "BC1_U", "BC1_SOC", "BC1_SOH", "BC2_I", "BC2_U", "BC2_SOC", "BC2_SOH", "BC3_I", "BC3_U",
 "BC3_SOC", "BC3_SOH", "BC4_I", "BC4_U", "BC4_SOC", "BC4_SOH", "PCS_COMM_ERROR", "PCS_CTRL_FAIL", "EMS_COMM_ERROR",
 "RACK_VDIFF_HIGH", "RACK_CDIFF_HIGH", "EM_SHUTDOWN", "AC_COMM_ERROR", "OVERV_3RD_LEVEL")
VALUES ('1734524036022534232', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16',
        '16', '18', '19', '1', '0', '1', '0', '1', '0', '1', '0');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_seoverview_dc1curve
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_SEOVERVIEW_DC1CURVE";
INSERT INTO "TSERVER"."D_SEOVERVIEW_DC1CURVE"
    ("ID", "SID", "TIME", "P", "I", "U")
VALUES ('1734524036022534233', '1', '2023-01-01 00:00:00', '1', '2', '3');
INSERT INTO "TSERVER"."D_SEOVERVIEW_DC1CURVE"
    ("ID", "SID", "TIME", "P", "I", "U")
VALUES ('1734524036022534234', '2', '2023-01-01 00:00:00', '2', '3', '4');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_seoverview_dc2data
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_SEOVERVIEW_DC2DATA";
INSERT INTO "TSERVER"."D_SEOVERVIEW_DC2DATA"
("ID", "P", "I", "U", "PCS_COMM_ERROR", "PCS_CTRL_FAIL", "EMS_COMM_ERROR", "RACK_VDIFF_HIGH", "RACK_CDIFF_HIGH",
 "EM_SHUTDOWN", "AC_COMM_ERROR", "OVERV_3RD_LEVEL")
VALUES ('1734524036022534235', '1', '2', '3', '1', '0', '1', '0', '1', '0', '1', '0');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_seoverview_dc2curve
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_SEOVERVIEW_DC2CURVE";
INSERT INTO "TSERVER"."D_SEOVERVIEW_DC2CURVE"
    ("ID", "SID", "TIME", "P", "I", "U")
VALUES ('1734524036022534236', '1', '2023-01-01 00:00:00', '1', '2', '3');
INSERT INTO "TSERVER"."D_SEOVERVIEW_DC1CURVE"
    ("ID", "SID", "TIME", "P", "I", "U")
VALUES ('1734524036022534237', '2', '2023-01-01 00:00:00', '2', '3', '4');
COMMIT TRANSACTION;

-- 8. 柔直互联台区-光伏运行状态表
-- --------------------------------------------
-- Table structure for d_poverview_station
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_POVERVIEW_STATION";
INSERT INTO "TSERVER"."D_POVERVIEW_STATION"
    ("ID", "PID", "NAME")
VALUES ('1734524036022534238', '1', '台区总光伏');
INSERT INTO "TSERVER"."P_POVERVIEW_STATION"
    ("ID", "PID", "NAME")
VALUES ('1734524036022534239', '2', '光伏电站001');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_poverview_overview
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_POVERVIEW_OVERVIEW";
INSERT INTO "TSERVER"."D_POVERVIEW_OVERVIEW"
("ID", "PID", "CAPACITY", "POWER", "MAX_POWER", "MIN_POWER", "DAILY_EP")
VALUES ('1734524036022534240', '1', '1', '2', '3', '4', '5');
INSERT INTO "TSERVER"."D_POVERVIEW_OVERVIEW"
("ID", "PID", "CAPACITY", "POWER", "MAX_POWER", "MIN_POWER", "DAILY_EP")
VALUES ('1734524036022534241', '2', '2', '3', '4', '5', '6');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_poverview_mesa
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_POVERVIEW_MESA";
INSERT INTO "TSERVER"."D_POVERVIEW_MESA"
    ("ID", "PID", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534242', '1', '1', '2', '3', '4', '5', '6', '7', '8');
INSERT INTO "TSERVER"."D_POVERVIEW_MESA"
    ("ID", "PID", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534243', '2', '2', '3', '4', '5', '6', '7', '8', '9');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_poverview_protect
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_POVERVIEW_PROTECT";
INSERT INTO "TSERVER"."D_POVERVIEW_PROTECT"
("ID", "PID", "LCL", "PROT_ACT", "OCSWITCH_ACT", "INTERVAL_EXCEPTION", "INTERVAL_FAIL", "INTERVAL_SINGAL",
 "MP_COMM_ERROR", "BH_POSITION")
VALUES ('1734524036022534244', '1', '0', '1', '0', '1', '0', '1', '0', '1');
INSERT INTO "TSERVER"."D_POVERVIEW_PROTECT"
("ID", "PID", "LCL", "PROT_ACT", "OCSWITCH_ACT", "INTERVAL_EXCEPTION", "INTERVAL_FAIL", "INTERVAL_SINGAL",
 "MP_COMM_ERROR", "BH_POSITION")
VALUES ('1734524036022534245', '2', '1', '0', '1', '0', '1', '0', '1', '0');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_poverview_curve
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_POVERVIEW_CURVE";
INSERT INTO "TSERVER"."D_POVERVIEW_CURVE"
("ID", "PID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA",
 "UB", "UC")
VALUES ('1734524036022534246', '1', '2023-01-01 00:00:00', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11',
        '12', '13', '14',
        '15', '16');
INSERT INTO "TSERVER"."D_POVERVIEW_CURVE"
("ID", "PID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA",
 "UB", "UC")
VALUES ('1734524036022534247', '1', '2023-01-01 01:00:00', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12',
        '13', '14', '15',
        '16', '17');
INSERT INTO "TSERVER"."D_POVERVIEW_CURVE"
("ID", "PID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA",
 "UB", "UC")
VALUES ('1734524036022534248', '2', '2023-01-01 00:00:00', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13',
        '14', '15',
        '16', '17', '18');
INSERT INTO "TSERVER"."D_POVERVIEW_CURVE"
("ID", "PID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA",
 "UB", "UC")
VALUES ('1734524036022534249', '2', '2023-01-01 01:00:00', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14',
        '15', '16',
        '17', '18', '19');
COMMIT TRANSACTION;

-- 9. 柔直互联台区-台区运行状态
-- --------------------------------------------
-- Table structure for d_aoverview_overview
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_AOVERVIEW_OVERVIEW";
INSERT INTO "TSERVER"."D_AOVERVIEW_OVERVIEW"
("ID", "P_REAL_POWER", "LOAD_POWER", "COMBINE_POWER", "DAILY_EP", "LOAD_EP", "TOTAL_EP")
VALUES ('1734524036022534250', '1', '2', '3', '4', '5', '6');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_aoverview_mesa
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_AOVERVIEW_MESA";
INSERT INTO "TSERVER"."D_AOVERVIEW_MESA"
    ("ID", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534251', '1', '2', '3', '4', '5', '6', '7', '8');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_aoverview_protect
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_AOVERVIEW_PROTECT";
INSERT INTO "TSERVER"."D_AOVERVIEW_PROTECT"
("ID", "LCL", "PROT_ACT", "OCSWITCH_ACT", "INTERVAL_EXCEPTION", "INTERVAL_FAIL", "INTERVAL_SINGAL", "MP_COMM_ERROR",
 "BH_POSITION")
VALUES ('1734524036022534252', '1', '0', '1', '0', '1', '0', '1', '0');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_aoverview_curve
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_AOVERVIEW_CURVE";
INSERT INTO "TSERVER"."D_AOVERVIEW_CURVE"
("ID", "TIME", "P_C", "Q_C", "IA_C", "IB_C", "IC_C", "UA_C", "UB_C", "UC_C", "P", "Q", "IA", "IB", "IC", "UA", "UB",
 "UC")
VALUES ('1734524036022534253', '2023-01-01 00:00:00', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12',
        '13', '14', '15',
        '16');
COMMIT TRANSACTION;

-- 10. 柔直互联台区-柔直互联运行状态表
-- --------------------------------------------
-- Table structure for d_fsoverview_acdata
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_FSOVERVIEW_ACDATA";
INSERT INTO "TSERVER"."D_FSOVERVIEW_ACDATA"
("ID", "P", "I", "U", "DC_EP", "DC_BUS_SC", "DC_OVER_CURRENT", "BB_OVER_CURRENT", "DC_RESERSE", "DC_V_LOW", "DC_V_HIGH",
 "DC_CHARGE_ERROR", "DC_RELAY_SC")
VALUES ('1734524036022534254', '1', '2', '3', '4', '1', '0', '1', '0', '1', '0', '1', '0');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_fsoverview_accurve
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_FSOVERVIEW_ACCURVE";
INSERT INTO "TSERVER"."D_FSOVERVIEW_ACCURVE"
    ("ID", "DID", "TIME", "P", "U", "I")
VALUES ('1734524036022534255', '1', '2023-01-01 00:00:00', '1', '2', '3');
INSERT INTO "TSERVER"."D_FSOVERVIEW_ACCURVE"
    ("ID", "DID", "TIME", "P", "U", "I")
VALUES ('1734524036022534256', '1', '2023-01-01 01:00:00', '2', '3', '4');
INSERT INTO "TSERVER"."D_FSOVERVIEW_ACCURVE"
    ("ID", "DID", "TIME", "P", "U", "I")
VALUES ('1734524036022534257', '1', '2023-01-01 02:00:00', '3', '4', '5');
INSERT INTO "TSERVER"."D_FSOVERVIEW_ACCURVE"
    ("ID", "DID", "TIME", "P", "U", "I")
VALUES ('1734524036022534258', '2', '2023-01-01 00:00:00', '1', '2', '3');
INSERT INTO "TSERVER"."D_FSOVERVIEW_ACCURVE"
    ("ID", "DID", "TIME", "P", "U", "I")
VALUES ('1734524036022534259', '2', '2023-01-01 01:00:00', '2', '3', '4');
INSERT INTO "TSERVER"."D_FSOVERVIEW_ACCURVE"
    ("ID", "DID", "TIME", "P", "U", "I")
VALUES ('1734524036022534260', '2', '2023-01-01 02:00:00', '3', '4', '5');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_fsoverview_dcdata
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_FSOVERVIEW_DCDATA";
INSERT INTO "TSERVER"."D_FSOVERVIEW_DCDATA"
("ID", "P", "Q", "IA", "IB", "IC", "PCS_COMM_ERROR", "PCS_CTRL_FAIL", "EMS_COMM_ERROR", "RACK_VDIFF_HIGH",
 "RACK_CDIFF_HIGH", "EM_SHUTDOWN", "AC_COMM_ERROR", "OVERV_3RD_LEVEL")
VALUES ('1734524036022534261', '1', '2', '3', '4', '5', '1', '0', '1', '0', '1', '0', '1', '0');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_fsoverview_dccurve
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_FSOVERVIEW_DCCURVE";
INSERT INTO "TSERVER"."D_FSOVERVIEW_DCCURVE"
    ("ID", "DID", "TIME", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534262', '1', '2023-01-01 00:00:00', '1', '2', '3', '4', '5', '6', '7', '8');
INSERT INTO "TSERVER"."D_FSOVERVIEW_DCCURVE"
    ("ID", "DID", "TIME", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534263', '1', '2023-01-01 01:00:00', '1', '2', '3', '4', '5', '6', '7', '8');
INSERT INTO "TSERVER"."D_FSOVERVIEW_DCCURVE"
    ("ID", "DID", "TIME", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534264', '1', '2023-01-01 02:00:00', '1', '2', '3', '4', '5', '6', '7', '8');
INSERT INTO "TSERVER"."D_FSOVERVIEW_DCCURVE"
    ("ID", "DID", "TIME", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534265', '1', '2023-01-01 03:00:00', '1', '2', '3', '4', '5', '6', '7', '8');
INSERT INTO "TSERVER"."D_FSOVERVIEW_DCCURVE"
    ("ID", "DID", "TIME", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534266', '2', '2023-01-01 00:00:00', '2', '3', '4', '5', '6', '7', '8', '9');
INSERT INTO "TSERVER"."D_FSOVERVIEW_DCCURVE"
    ("ID", "DID", "TIME", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534267', '2', '2023-01-01 01:00:00', '2', '3', '4', '5', '6', '7', '8', '9');
INSERT INTO "TSERVER"."D_FSOVERVIEW_DCCURVE"
    ("ID", "DID", "TIME", "P", "Q", "IA", "IB", "IC", "UA", "UB", "UC")
VALUES ('1734524036022534268', '2', '2023-01-01 02:00:00', '2', '3', '4', '5', '6', '7', '8', '9');
COMMIT TRANSACTION;

-- 11. 柔直互联台区-经济效益分析表
-- --------------------------------------------
-- Table structure for d_ebanalysis_overview
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_EBANALYSIS_OVERVIEW";
INSERT INTO "TSERVER"."D_EBANALYSIS_OVERVIEW"
("ID", "TOTAL_CHARGE_EP", "TOTAL_DISCHARGE_EP", "TOTAL_PG_EP", "TOTAL_PC_EP", "TOTAL_EB", "TOTAL_PVD_BENEFIT",
 "TOTAL_PP_BENEFIT", "TOTAL_CER", "TOTAL_SSCQ")
VALUES ('1734524036022534269', '1', '2', '3', '4', '5', '6', '7', '8', '9');
COMMIT TRANSACTION;

-- --------------------------------------------
-- Table structure for d_ebanalysis_curve
-- --------------------------------------------
BEGIN TRANSACTION;
DELETE
FROM "TSERVER"."D_EBANALYSIS_CURVE";
INSERT INTO "TSERVER"."D_EBANALYSIS_CURVE"
("ID", "TIME", "CHARGE_EP", "DISCHARGE_EP", "PG_EP", "PC_EP", "EB", "PVD_BENEFIT", "PP_BENEFIT")
VALUES ('1734524036022534270', '2023-01-01', '1', '2', '3', '4', '5', '6', '7');
INSERT INTO "TSERVER"."D_EBANALYSIS_CURVE"
("ID", "TIME", "CHARGE_EP", "DISCHARGE_EP", "PG_EP", "PC_EP", "EB", "PVD_BENEFIT", "PP_BENEFIT")
VALUES ('1734524036022534271', '2023-01-01', '2', '3', '4', '5', '6', '7', '8');
COMMIT TRANSACTION;

-- 12. 柔直互联台区-协调控制表
-- todo:
