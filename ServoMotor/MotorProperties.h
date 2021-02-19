#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "CanOpenCia402KitechDef.h"

#define	NUMBER_OF_MOTOR_PROPERTY					22

#define	MOTOR_ID_NONE								0x00000000
	
//	Maxon Motor
#define	MAXONMOTOR_EC60_FLAT_100W_408057			0x00000001
#define	MAXONMOTOR_EC45_250W_136207					0x00000002
#define	MAXONMOTOR_EC_I_40_50W_449469_ENC_1024		0x00000003
#define	MAXONMOTOR_EC60_FLAT_100W_ROHAU				0x00000004
#define	MAXONMOTOR_ECI52_60W						0x00000005

//	Leadshine
#define	LEADSHINE_BLM57025							0x01000001
#define	LEADSHINE_BLM57050							0x01000002
#define	LEADSHINE_BLM57090							0x01000003
#define	LEADSHINE_BLM57130							0x01000004
#define	LEADSHINE_BLM57180							0x01000005
#define	LEADSHINE_57BL180							0x01000006

#define	LEADSHINE_ACM601V36							0x01000010
#define	LEADSHINE_ACM602V36							0x01000011
#define	LEADSHINE_ACM602V60							0x01000012
#define	LEADSHINE_ACM604V60							0x01000013
#define	LEADSHINE_ACM602V36_30						0x01000014
#define	LEADSHINE_ACM604V60_30						0x01000015
	
//	Kollmorgen
#define	KOLLMORGEN_01212_C							0x02000001

#define	ERAETECH_RP32_313V24						0xFF000001
#define	OZBM60_200D2_RE								0xFF010001
#define	OZBM80_500D2_GE								0xFF010002
#define	COMOTEK_NS_W370A							0xFF020001
#define	WACO_MOTOR									0xFF030001		//	Resolver
#define	TM_TECH_I_TM13_A2023						0xFF040001
#define	CELERA_UTH_63_B18_Cx000						0xFF050001
	
#define	KETI_MOTOR_48V_70W_SECOND_YEAR				0xFFFF0001
#define	KETI_MOTOR_48V_100W_SECOND_YEAR				0xFFFF0002
#define	H2W_TECH_NCC20_18_020_1A					0xFFFF0101		

extern MOTOR_PROPERTY gMotorProperties[];
	
#ifdef __cplusplus
}
#endif
