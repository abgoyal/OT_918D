/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*
 *  Zorro board IDs
 *
 *  Please keep sorted.
 */


#define ZORRO_MANUF_PACIFIC_PERIPHERALS				0x00D3
#define  ZORRO_PROD_PACIFIC_PERIPHERALS_SE_2000_A500		ZORRO_ID(PACIFIC_PERIPHERALS, 0x00, 0)
#define  ZORRO_PROD_PACIFIC_PERIPHERALS_SCSI			ZORRO_ID(PACIFIC_PERIPHERALS, 0x0A, 0)

#define ZORRO_MANUF_MACROSYSTEMS_USA_2				0x0100
#define  ZORRO_PROD_MACROSYSTEMS_WARP_ENGINE			ZORRO_ID(MACROSYSTEMS_USA_2, 0x13, 0)

#define ZORRO_MANUF_KUPKE_1					0x00DD
#define  ZORRO_PROD_KUPKE_GOLEM_RAM_BOX_2MB			ZORRO_ID(KUPKE_1, 0x00, 0)

#define ZORRO_MANUF_MEMPHIS					0x0100
#define  ZORRO_PROD_MEMPHIS_STORMBRINGER			ZORRO_ID(MEMPHIS, 0x00, 0)

#define ZORRO_MANUF_3_STATE					0x0200
#define  ZORRO_PROD_3_STATE_MEGAMIX_2000			ZORRO_ID(3_STATE, 0x02, 0)

#define ZORRO_MANUF_COMMODORE_BRAUNSCHWEIG			0x0201
#define  ZORRO_PROD_CBM_A2088_A2286				ZORRO_ID(COMMODORE_BRAUNSCHWEIG, 0x01, 0)
#define  ZORRO_PROD_CBM_A2286					ZORRO_ID(COMMODORE_BRAUNSCHWEIG, 0x02, 0)
#define  ZORRO_PROD_CBM_A4091_1					ZORRO_ID(COMMODORE_BRAUNSCHWEIG, 0x54, 0)
#define  ZORRO_PROD_CBM_A2386SX_1				ZORRO_ID(COMMODORE_BRAUNSCHWEIG, 0x67, 0)

#define ZORRO_MANUF_COMMODORE_WEST_CHESTER_1			0x0202
#define  ZORRO_PROD_CBM_A2090A					ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x01, 0)
#define  ZORRO_PROD_CBM_A590_A2091_1				ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x02, 0)
#define  ZORRO_PROD_CBM_A590_A2091_2				ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x03, 0)
#define  ZORRO_PROD_CBM_A2090B					ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x04, 0)
#define  ZORRO_PROD_CBM_A2060					ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x09, 0)
#define  ZORRO_PROD_CBM_A590_A2052_A2058_A2091			ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x0A, 0)
#define  ZORRO_PROD_CBM_A560_RAM				ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x20, 0)
#define  ZORRO_PROD_CBM_A2232_PROTOTYPE				ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x45, 0)
#define  ZORRO_PROD_CBM_A2232					ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x46, 0)
#define  ZORRO_PROD_CBM_A2620					ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x50, 0)
#define  ZORRO_PROD_CBM_A2630					ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x51, 0)
#define  ZORRO_PROD_CBM_A4091_2					ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x54, 0)
#define  ZORRO_PROD_CBM_A2065_1					ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x5A, 0)
#define  ZORRO_PROD_CBM_ROMULATOR				ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x60, 0)
#define  ZORRO_PROD_CBM_A3000_TEST_FIXTURE			ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x61, 0)
#define  ZORRO_PROD_CBM_A2386SX_2				ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x67, 0)
#define  ZORRO_PROD_CBM_A2065_2					ZORRO_ID(COMMODORE_WEST_CHESTER_1, 0x70, 0)

#define ZORRO_MANUF_COMMODORE_WEST_CHESTER_2			0x0203
#define  ZORRO_PROD_CBM_A2090A_CM				ZORRO_ID(COMMODORE_WEST_CHESTER_2, 0x03, 0)

#define ZORRO_MANUF_PROGRESSIVE_PERIPHERALS_AND_SYSTEMS_2	0x02F4
#define  ZORRO_PROD_PPS_EXP8000					ZORRO_ID(PROGRESSIVE_PERIPHERALS_AND_SYSTEMS_2, 0x02, 0)

#define ZORRO_MANUF_KOLFF_COMPUTER_SUPPLIES			0x02FF
#define  ZORRO_PROD_KCS_POWER_PC_BOARD				ZORRO_ID(KOLFF_COMPUTER_SUPPLIES, 0x00, 0)

#define ZORRO_MANUF_CARDCO_1					0x03EC
#define  ZORRO_PROD_CARDCO_KRONOS_2000_1			ZORRO_ID(CARDCO_1, 0x04, 0)
#define  ZORRO_PROD_CARDCO_A1000_1				ZORRO_ID(CARDCO_1, 0x0C, 0)
#define  ZORRO_PROD_CARDCO_ESCORT				ZORRO_ID(CARDCO_1, 0x0E, 0)
#define  ZORRO_PROD_CARDCO_A2410				ZORRO_ID(CARDCO_1, 0xF5, 0)

#define ZORRO_MANUF_A_SQUARED					0x03ED
#define  ZORRO_PROD_A_SQUARED_LIVE_2000				ZORRO_ID(A_SQUARED, 0x01, 0)

#define ZORRO_MANUF_COMSPEC_COMMUNICATIONS			0x03EE
#define  ZORRO_PROD_COMSPEC_COMMUNICATIONS_AX2000		ZORRO_ID(COMSPEC_COMMUNICATIONS, 0x01, 0)

#define ZORRO_MANUF_ANAKIN_RESEARCH				0x03F1
#define  ZORRO_PROD_ANAKIN_RESEARCH_EASYL			ZORRO_ID(ANAKIN_RESEARCH, 0x01, 0)

#define ZORRO_MANUF_MICROBOTICS					0x03F2
#define  ZORRO_PROD_MICROBOTICS_STARBOARD_II			ZORRO_ID(MICROBOTICS, 0x00, 0)
#define  ZORRO_PROD_MICROBOTICS_STARDRIVE			ZORRO_ID(MICROBOTICS, 0x02, 0)
#define  ZORRO_PROD_MICROBOTICS_8_UP_A				ZORRO_ID(MICROBOTICS, 0x03, 0)
#define  ZORRO_PROD_MICROBOTICS_8_UP_Z				ZORRO_ID(MICROBOTICS, 0x04, 0)
#define  ZORRO_PROD_MICROBOTICS_DELTA_RAM			ZORRO_ID(MICROBOTICS, 0x20, 0)
#define  ZORRO_PROD_MICROBOTICS_8_STAR_RAM			ZORRO_ID(MICROBOTICS, 0x40, 0)
#define  ZORRO_PROD_MICROBOTICS_8_STAR				ZORRO_ID(MICROBOTICS, 0x41, 0)
#define  ZORRO_PROD_MICROBOTICS_VXL_RAM_32			ZORRO_ID(MICROBOTICS, 0x44, 0)
#define  ZORRO_PROD_MICROBOTICS_VXL_68030			ZORRO_ID(MICROBOTICS, 0x45, 0)
#define  ZORRO_PROD_MICROBOTICS_DELTA				ZORRO_ID(MICROBOTICS, 0x60, 0)
#define  ZORRO_PROD_MICROBOTICS_MBX_1200_1200Z_RAM		ZORRO_ID(MICROBOTICS, 0x81, 0)
#define  ZORRO_PROD_MICROBOTICS_HARDFRAME_2000_1		ZORRO_ID(MICROBOTICS, 0x96, 0)
#define  ZORRO_PROD_MICROBOTICS_HARDFRAME_2000_2		ZORRO_ID(MICROBOTICS, 0x9E, 0)
#define  ZORRO_PROD_MICROBOTICS_MBX_1200_1200Z			ZORRO_ID(MICROBOTICS, 0xC1, 0)

#define ZORRO_MANUF_ACCESS_ASSOCIATES_ALEGRA			0x03F4

#define ZORRO_MANUF_EXPANSION_TECHNOLOGIES			0x03F6

#define ZORRO_MANUF_ASDG					0x03FF
#define  ZORRO_PROD_ASDG_MEMORY_1				ZORRO_ID(ASDG, 0x01, 0)
#define  ZORRO_PROD_ASDG_MEMORY_2				ZORRO_ID(ASDG, 0x02, 0)
#define  ZORRO_PROD_ASDG_EB920_LAN_ROVER			ZORRO_ID(ASDG, 0xFE, 0)
#define  ZORRO_PROD_ASDG_GPIB_DUALIEEE488_TWIN_X		ZORRO_ID(ASDG, 0xFF, 0)

#define ZORRO_MANUF_IMTRONICS_1					0x0404
#define  ZORRO_PROD_IMTRONICS_HURRICANE_2800_1			ZORRO_ID(IMTRONICS_1, 0x39, 0)
#define  ZORRO_PROD_IMTRONICS_HURRICANE_2800_2			ZORRO_ID(IMTRONICS_1, 0x57, 0)

#define ZORRO_MANUF_CBM_UNIVERSITY_OF_LOWELL			0x0406
#define  ZORRO_PROD_CBM_A2410					ZORRO_ID(CBM_UNIVERSITY_OF_LOWELL, 0x00, 0)

#define ZORRO_MANUF_AMERISTAR					0x041D
#define  ZORRO_PROD_AMERISTAR_A2065				ZORRO_ID(AMERISTAR, 0x01, 0)
#define  ZORRO_PROD_AMERISTAR_A560				ZORRO_ID(AMERISTAR, 0x09, 0)
#define  ZORRO_PROD_AMERISTAR_A4066				ZORRO_ID(AMERISTAR, 0x0A, 0)

#define ZORRO_MANUF_SUPRA					0x0420
#define  ZORRO_PROD_SUPRA_SUPRADRIVE_4x4			ZORRO_ID(SUPRA, 0x01, 0)
#define  ZORRO_PROD_SUPRA_1000_RAM				ZORRO_ID(SUPRA, 0x02, 0)
#define  ZORRO_PROD_SUPRA_2000_DMA				ZORRO_ID(SUPRA, 0x03, 0)
#define  ZORRO_PROD_SUPRA_500					ZORRO_ID(SUPRA, 0x05, 0)
#define  ZORRO_PROD_SUPRA_500_SCSI				ZORRO_ID(SUPRA, 0x08, 0)
#define  ZORRO_PROD_SUPRA_500XP_2000_RAM			ZORRO_ID(SUPRA, 0x09, 0)
#define  ZORRO_PROD_SUPRA_500RX_2000_RAM			ZORRO_ID(SUPRA, 0x0A, 0)
#define  ZORRO_PROD_SUPRA_2400ZI				ZORRO_ID(SUPRA, 0x0B, 0)
#define  ZORRO_PROD_SUPRA_500XP_SUPRADRIVE_WORDSYNC		ZORRO_ID(SUPRA, 0x0C, 0)
#define  ZORRO_PROD_SUPRA_SUPRADRIVE_WORDSYNC_II		ZORRO_ID(SUPRA, 0x0D, 0)
#define  ZORRO_PROD_SUPRA_2400ZIPLUS				ZORRO_ID(SUPRA, 0x10, 0)

#define ZORRO_MANUF_COMPUTER_SYSTEMS_ASSOCIATES			0x0422
#define  ZORRO_PROD_CSA_MAGNUM					ZORRO_ID(COMPUTER_SYSTEMS_ASSOCIATES, 0x11, 0)
#define  ZORRO_PROD_CSA_12_GAUGE				ZORRO_ID(COMPUTER_SYSTEMS_ASSOCIATES, 0x15, 0)

#define ZORRO_MANUF_MARC_MICHAEL_GROTH				0x0439

#define ZORRO_MANUF_M_TECH					0x0502
#define  ZORRO_PROD_MTEC_AT500_1				ZORRO_ID(M_TECH, 0x03, 0)

#define ZORRO_MANUF_GREAT_VALLEY_PRODUCTS_1			0x06E1
#define  ZORRO_PROD_GVP_IMPACT_SERIES_I				ZORRO_ID(GREAT_VALLEY_PRODUCTS_1, 0x08, 0)

#define ZORRO_MANUF_BYTEBOX					0x07DA
#define  ZORRO_PROD_BYTEBOX_A500				ZORRO_ID(BYTEBOX, 0x00, 0)

#define ZORRO_MANUF_DKB_POWER_COMPUTING				0x07DC
#define  ZORRO_PROD_DKB_POWER_COMPUTING_SECUREKEY		ZORRO_ID(DKB_POWER_COMPUTING, 0x09, 0)
#define  ZORRO_PROD_DKB_POWER_COMPUTING_DKM_3128		ZORRO_ID(DKB_POWER_COMPUTING, 0x0E, 0)
#define  ZORRO_PROD_DKB_POWER_COMPUTING_RAPID_FIRE		ZORRO_ID(DKB_POWER_COMPUTING, 0x0F, 0)
#define  ZORRO_PROD_DKB_POWER_COMPUTING_DKM_1202		ZORRO_ID(DKB_POWER_COMPUTING, 0x10, 0)
#define  ZORRO_PROD_DKB_POWER_COMPUTING_COBRA_VIPER_II_68EC030	ZORRO_ID(DKB_POWER_COMPUTING, 0x12, 0)
#define  ZORRO_PROD_DKB_POWER_COMPUTING_WILDFIRE_060_1		ZORRO_ID(DKB_POWER_COMPUTING, 0x17, 0)
#define  ZORRO_PROD_DKB_POWER_COMPUTING_WILDFIRE_060_2		ZORRO_ID(DKB_POWER_COMPUTING, 0xFF, 0)

#define ZORRO_MANUF_GREAT_VALLEY_PRODUCTS_2			0x07E1
#define  ZORRO_PROD_GVP_IMPACT_SERIES_I_4K			ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x01, 0)
#define  ZORRO_PROD_GVP_IMPACT_SERIES_I_16K_2			ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x02, 0)
#define  ZORRO_PROD_GVP_IMPACT_SERIES_I_16K_3			ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x03, 0)
#define  ZORRO_PROD_GVP_IMPACT_3001_IDE_1			ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x08, 0)
#define  ZORRO_PROD_GVP_IMPACT_3001_RAM				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x09, 0)
#define  ZORRO_PROD_GVP_IMPACT_SERIES_II_RAM_1			ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0A, 0)
#define  ZORRO_PROD_GVP_EPC_BASE				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0)
#define  ZORRO_PROD_GVP_GFORCE_040_1				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0x20)
#define  ZORRO_PROD_GVP_GFORCE_040_SCSI_1			ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0x30)
#define  ZORRO_PROD_GVP_A1291					ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0x40)
#define  ZORRO_PROD_GVP_COMBO_030_R4				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0x60)
#define  ZORRO_PROD_GVP_COMBO_030_R4_SCSI			ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0x70)
#define  ZORRO_PROD_GVP_PHONEPAK				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0x78)
#define  ZORRO_PROD_GVP_IO_EXTENDER				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0x98)
#define  ZORRO_PROD_GVP_GFORCE_030				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0xa0)
#define  ZORRO_PROD_GVP_GFORCE_030_SCSI				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0xb0)
#define  ZORRO_PROD_GVP_A530					ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0xc0)
#define  ZORRO_PROD_GVP_A530_SCSI				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0xd0)
#define  ZORRO_PROD_GVP_COMBO_030_R3				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0xe0)
#define  ZORRO_PROD_GVP_COMBO_030_R3_SCSI			ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0xf0)
#define  ZORRO_PROD_GVP_SERIES_II				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0B, 0xf8)
#define  ZORRO_PROD_GVP_IMPACT_3001_IDE_2			ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0D, 0)
/*#define  ZORRO_PROD_GVP_A2000_030				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0D, 0)*/
/*#define  ZORRO_PROD_GVP_GFORCE_040_SCSI_2			ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x0D, 0)*/
#define  ZORRO_PROD_GVP_GFORCE_040_060				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x16, 0)
#define  ZORRO_PROD_GVP_IMPACT_VISION_24			ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0x20, 0)
#define  ZORRO_PROD_GVP_GFORCE_040_2				ZORRO_ID(GREAT_VALLEY_PRODUCTS_2, 0xFF, 0)

#define ZORRO_MANUF_CALIFORNIA_ACCESS_SYNERGY			0x07E5
#define  ZORRO_PROD_CALIFORNIA_ACCESS_SYNERGY_MALIBU		ZORRO_ID(CALIFORNIA_ACCESS_SYNERGY, 0x01, 0)

#define ZORRO_MANUF_XETEC					0x07E6
#define  ZORRO_PROD_XETEC_FASTCARD				ZORRO_ID(XETEC, 0x01, 0)
#define  ZORRO_PROD_XETEC_FASTCARD_RAM				ZORRO_ID(XETEC, 0x02, 0)
#define  ZORRO_PROD_XETEC_FASTCARD_PLUS				ZORRO_ID(XETEC, 0x03, 0)

#define ZORRO_MANUF_PROGRESSIVE_PERIPHERALS_AND_SYSTEMS		0x07EA
#define  ZORRO_PROD_PPS_MERCURY					ZORRO_ID(PROGRESSIVE_PERIPHERALS_AND_SYSTEMS, 0x00, 0)
#define  ZORRO_PROD_PPS_A3000_68040				ZORRO_ID(PROGRESSIVE_PERIPHERALS_AND_SYSTEMS, 0x01, 0)
#define  ZORRO_PROD_PPS_A2000_68040				ZORRO_ID(PROGRESSIVE_PERIPHERALS_AND_SYSTEMS, 0x69, 0)
#define  ZORRO_PROD_PPS_ZEUS					ZORRO_ID(PROGRESSIVE_PERIPHERALS_AND_SYSTEMS, 0x96, 0)
#define  ZORRO_PROD_PPS_A500_68040				ZORRO_ID(PROGRESSIVE_PERIPHERALS_AND_SYSTEMS, 0xBB, 0)

#define ZORRO_MANUF_XEBEC					0x07EC

#define ZORRO_MANUF_SPIRIT_TECHNOLOGY				0x07F2
#define  ZORRO_PROD_SPIRIT_TECHNOLOGY_INSIDER_IN1000		ZORRO_ID(SPIRIT_TECHNOLOGY, 0x01, 0)
#define  ZORRO_PROD_SPIRIT_TECHNOLOGY_INSIDER_IN500		ZORRO_ID(SPIRIT_TECHNOLOGY, 0x02, 0)
#define  ZORRO_PROD_SPIRIT_TECHNOLOGY_SIN500			ZORRO_ID(SPIRIT_TECHNOLOGY, 0x03, 0)
#define  ZORRO_PROD_SPIRIT_TECHNOLOGY_HDA_506			ZORRO_ID(SPIRIT_TECHNOLOGY, 0x04, 0)
#define  ZORRO_PROD_SPIRIT_TECHNOLOGY_AX_S			ZORRO_ID(SPIRIT_TECHNOLOGY, 0x05, 0)
#define  ZORRO_PROD_SPIRIT_TECHNOLOGY_OCTABYTE			ZORRO_ID(SPIRIT_TECHNOLOGY, 0x06, 0)
#define  ZORRO_PROD_SPIRIT_TECHNOLOGY_INMATE			ZORRO_ID(SPIRIT_TECHNOLOGY, 0x08, 0)

#define ZORRO_MANUF_SPIRIT_TECHNOLOGY_2				0x07F3

#define ZORRO_MANUF_BSC_ALFADATA_1				0x07FE
#define  ZORRO_PROD_BSC_ALF_3_1					ZORRO_ID(BSC_ALFADATA_1, 0x03, 0)

#define ZORRO_MANUF_BSC_ALFADATA_2				0x0801
#define  ZORRO_PROD_BSC_ALF_2_1					ZORRO_ID(BSC_ALFADATA_2, 0x01, 0)
#define  ZORRO_PROD_BSC_ALF_2_2					ZORRO_ID(BSC_ALFADATA_2, 0x02, 0)
#define  ZORRO_PROD_BSC_ALF_3_2					ZORRO_ID(BSC_ALFADATA_2, 0x03, 0)

#define ZORRO_MANUF_CARDCO_2					0x0802
#define  ZORRO_PROD_CARDCO_KRONOS_2000_2			ZORRO_ID(CARDCO_2, 0x04, 0)
#define  ZORRO_PROD_CARDCO_A1000_2				ZORRO_ID(CARDCO_2, 0x0C, 0)

#define ZORRO_MANUF_JOCHHEIM					0x0804
#define  ZORRO_PROD_JOCHHEIM_RAM				ZORRO_ID(JOCHHEIM, 0x01, 0)

#define ZORRO_MANUF_CHECKPOINT_TECHNOLOGIES			0x0807
#define  ZORRO_PROD_CHECKPOINT_TECHNOLOGIES_SERIAL_SOLUTION	ZORRO_ID(CHECKPOINT_TECHNOLOGIES, 0x00, 0)

#define ZORRO_MANUF_EDOTRONIK					0x0810
#define  ZORRO_PROD_EDOTRONIK_IEEE_488				ZORRO_ID(EDOTRONIK, 0x01, 0)
#define  ZORRO_PROD_EDOTRONIK_8032				ZORRO_ID(EDOTRONIK, 0x02, 0)
#define  ZORRO_PROD_EDOTRONIK_MULTISERIAL			ZORRO_ID(EDOTRONIK, 0x03, 0)
#define  ZORRO_PROD_EDOTRONIK_VIDEODIGITIZER			ZORRO_ID(EDOTRONIK, 0x04, 0)
#define  ZORRO_PROD_EDOTRONIK_PARALLEL_IO			ZORRO_ID(EDOTRONIK, 0x05, 0)
#define  ZORRO_PROD_EDOTRONIK_PIC_PROTOYPING			ZORRO_ID(EDOTRONIK, 0x06, 0)
#define  ZORRO_PROD_EDOTRONIK_ADC				ZORRO_ID(EDOTRONIK, 0x07, 0)
#define  ZORRO_PROD_EDOTRONIK_VME				ZORRO_ID(EDOTRONIK, 0x08, 0)
#define  ZORRO_PROD_EDOTRONIK_DSP96000				ZORRO_ID(EDOTRONIK, 0x09, 0)

#define ZORRO_MANUF_NES_INC					0x0813
#define  ZORRO_PROD_NES_INC_RAM					ZORRO_ID(NES_INC, 0x00, 0)

#define ZORRO_MANUF_ICD						0x0817
#define  ZORRO_PROD_ICD_ADVANTAGE_2000_SCSI			ZORRO_ID(ICD, 0x01, 0)
#define  ZORRO_PROD_ICD_ADVANTAGE_IDE				ZORRO_ID(ICD, 0x03, 0)
#define  ZORRO_PROD_ICD_ADVANTAGE_2080_RAM			ZORRO_ID(ICD, 0x04, 0)

#define ZORRO_MANUF_KUPKE_2					0x0819
#define  ZORRO_PROD_KUPKE_OMTI					ZORRO_ID(KUPKE_2, 0x01, 0)
#define  ZORRO_PROD_KUPKE_SCSI_II				ZORRO_ID(KUPKE_2, 0x02, 0)
#define  ZORRO_PROD_KUPKE_GOLEM_BOX				ZORRO_ID(KUPKE_2, 0x03, 0)
#define  ZORRO_PROD_KUPKE_030_882				ZORRO_ID(KUPKE_2, 0x04, 0)
#define  ZORRO_PROD_KUPKE_SCSI_AT				ZORRO_ID(KUPKE_2, 0x05, 0)

#define ZORRO_MANUF_GREAT_VALLEY_PRODUCTS_3			0x081D
#define  ZORRO_PROD_GVP_A2000_RAM8				ZORRO_ID(GREAT_VALLEY_PRODUCTS_3, 0x09, 0)
#define  ZORRO_PROD_GVP_IMPACT_SERIES_II_RAM_2			ZORRO_ID(GREAT_VALLEY_PRODUCTS_3, 0x0A, 0)

#define ZORRO_MANUF_INTERWORKS_NETWORK				0x081E

#define ZORRO_MANUF_HARDITAL_SYNTHESIS				0x0820
#define  ZORRO_PROD_HARDITAL_SYNTHESIS_TQM_68030_68882		ZORRO_ID(HARDITAL_SYNTHESIS, 0x14, 0)

#define ZORRO_MANUF_APPLIED_ENGINEERING				0x0828
#define  ZORRO_PROD_APPLIED_ENGINEERING_DL2000			ZORRO_ID(APPLIED_ENGINEERING, 0x10, 0)
#define  ZORRO_PROD_APPLIED_ENGINEERING_RAM_WORKS		ZORRO_ID(APPLIED_ENGINEERING, 0xE0, 0)

#define ZORRO_MANUF_BSC_ALFADATA_3				0x082C
#define  ZORRO_PROD_BSC_OKTAGON_2008				ZORRO_ID(BSC_ALFADATA_3, 0x05, 0)
#define  ZORRO_PROD_BSC_TANDEM_AT_2008_508			ZORRO_ID(BSC_ALFADATA_3, 0x06, 0)
#define  ZORRO_PROD_BSC_ALFA_RAM_1200				ZORRO_ID(BSC_ALFADATA_3, 0x07, 0)
#define  ZORRO_PROD_BSC_OKTAGON_2008_RAM			ZORRO_ID(BSC_ALFADATA_3, 0x08, 0)
#define  ZORRO_PROD_BSC_MULTIFACE_I				ZORRO_ID(BSC_ALFADATA_3, 0x10, 0)
#define  ZORRO_PROD_BSC_MULTIFACE_II				ZORRO_ID(BSC_ALFADATA_3, 0x11, 0)
#define  ZORRO_PROD_BSC_MULTIFACE_III				ZORRO_ID(BSC_ALFADATA_3, 0x12, 0)
#define  ZORRO_PROD_BSC_FRAMEMASTER_II				ZORRO_ID(BSC_ALFADATA_3, 0x20, 0)
#define  ZORRO_PROD_BSC_GRAFFITI_RAM				ZORRO_ID(BSC_ALFADATA_3, 0x21, 0)
#define  ZORRO_PROD_BSC_GRAFFITI_REG				ZORRO_ID(BSC_ALFADATA_3, 0x22, 0)
#define  ZORRO_PROD_BSC_ISDN_MASTERCARD				ZORRO_ID(BSC_ALFADATA_3, 0x40, 0)
#define  ZORRO_PROD_BSC_ISDN_MASTERCARD_II			ZORRO_ID(BSC_ALFADATA_3, 0x41, 0)

#define ZORRO_MANUF_PHOENIX					0x0835
#define  ZORRO_PROD_PHOENIX_ST506				ZORRO_ID(PHOENIX, 0x21, 0)
#define  ZORRO_PROD_PHOENIX_SCSI				ZORRO_ID(PHOENIX, 0x22, 0)
#define  ZORRO_PROD_PHOENIX_RAM					ZORRO_ID(PHOENIX, 0xBE, 0)

#define ZORRO_MANUF_ADVANCED_STORAGE_SYSTEMS			0x0836
#define  ZORRO_PROD_ADVANCED_STORAGE_SYSTEMS_NEXUS		ZORRO_ID(ADVANCED_STORAGE_SYSTEMS, 0x01, 0)
#define  ZORRO_PROD_ADVANCED_STORAGE_SYSTEMS_NEXUS_RAM		ZORRO_ID(ADVANCED_STORAGE_SYSTEMS, 0x08, 0)

#define ZORRO_MANUF_IMPULSE					0x0838
#define  ZORRO_PROD_IMPULSE_FIRECRACKER_24			ZORRO_ID(IMPULSE, 0x00, 0)

#define ZORRO_MANUF_IVS						0x0840
#define  ZORRO_PROD_IVS_GRANDSLAM_PIC_2				ZORRO_ID(IVS, 0x02, 0)
#define  ZORRO_PROD_IVS_GRANDSLAM_PIC_1				ZORRO_ID(IVS, 0x04, 0)
#define  ZORRO_PROD_IVS_OVERDRIVE				ZORRO_ID(IVS, 0x10, 0)
#define  ZORRO_PROD_IVS_TRUMPCARD_CLASSIC			ZORRO_ID(IVS, 0x30, 0)
#define  ZORRO_PROD_IVS_TRUMPCARD_PRO_GRANDSLAM			ZORRO_ID(IVS, 0x34, 0)
#define  ZORRO_PROD_IVS_META_4					ZORRO_ID(IVS, 0x40, 0)
#define  ZORRO_PROD_IVS_WAVETOOLS				ZORRO_ID(IVS, 0xBF, 0)
#define  ZORRO_PROD_IVS_VECTOR_1				ZORRO_ID(IVS, 0xF3, 0)
#define  ZORRO_PROD_IVS_VECTOR_2				ZORRO_ID(IVS, 0xF4, 0)

#define ZORRO_MANUF_VECTOR_1					0x0841
#define  ZORRO_PROD_VECTOR_CONNECTION_1				ZORRO_ID(VECTOR_1, 0xE3, 0)

#define ZORRO_MANUF_XPERT_PRODEV				0x0845
#define  ZORRO_PROD_XPERT_PRODEV_VISIONA_RAM			ZORRO_ID(XPERT_PRODEV, 0x01, 0)
#define  ZORRO_PROD_XPERT_PRODEV_VISIONA_REG			ZORRO_ID(XPERT_PRODEV, 0x02, 0)
#define  ZORRO_PROD_XPERT_PRODEV_MERLIN_RAM			ZORRO_ID(XPERT_PRODEV, 0x03, 0)
#define  ZORRO_PROD_XPERT_PRODEV_MERLIN_REG_1			ZORRO_ID(XPERT_PRODEV, 0x04, 0)
#define  ZORRO_PROD_XPERT_PRODEV_MERLIN_REG_2			ZORRO_ID(XPERT_PRODEV, 0xC9, 0)

#define ZORRO_MANUF_HYDRA_SYSTEMS				0x0849
#define  ZORRO_PROD_HYDRA_SYSTEMS_AMIGANET			ZORRO_ID(HYDRA_SYSTEMS, 0x01, 0)

#define ZORRO_MANUF_SUNRIZE_INDUSTRIES				0x084F
#define  ZORRO_PROD_SUNRIZE_INDUSTRIES_AD1012			ZORRO_ID(SUNRIZE_INDUSTRIES, 0x01, 0)
#define  ZORRO_PROD_SUNRIZE_INDUSTRIES_AD516			ZORRO_ID(SUNRIZE_INDUSTRIES, 0x02, 0)
#define  ZORRO_PROD_SUNRIZE_INDUSTRIES_DD512			ZORRO_ID(SUNRIZE_INDUSTRIES, 0x03, 0)

#define ZORRO_MANUF_TRICERATOPS					0x0850
#define  ZORRO_PROD_TRICERATOPS_MULTI_IO			ZORRO_ID(TRICERATOPS, 0x01, 0)

#define ZORRO_MANUF_APPLIED_MAGIC				0x0851
#define  ZORRO_PROD_APPLIED_MAGIC_DMI_RESOLVER			ZORRO_ID(APPLIED_MAGIC, 0x01, 0)
#define  ZORRO_PROD_APPLIED_MAGIC_DIGITAL_BROADCASTER		ZORRO_ID(APPLIED_MAGIC, 0x06, 0)

#define ZORRO_MANUF_GFX_BASE					0x085E
#define  ZORRO_PROD_GFX_BASE_GDA_1_VRAM				ZORRO_ID(GFX_BASE, 0x00, 0)
#define  ZORRO_PROD_GFX_BASE_GDA_1				ZORRO_ID(GFX_BASE, 0x01, 0)

#define ZORRO_MANUF_ROCTEC					0x0860
#define  ZORRO_PROD_ROCTEC_RH_800C				ZORRO_ID(ROCTEC, 0x01, 0)
#define  ZORRO_PROD_ROCTEC_RH_800C_RAM				ZORRO_ID(ROCTEC, 0x01, 0)

#define ZORRO_MANUF_KATO					0x0861
#define  ZORRO_PROD_KATO_MELODY					ZORRO_ID(KATO, 0x80, 0)
/* ID clash!! */
#define ZORRO_MANUF_HELFRICH_1					0x0861
#define  ZORRO_PROD_HELFRICH_RAINBOW_II				ZORRO_ID(HELFRICH_1, 0x20, 0)
#define  ZORRO_PROD_HELFRICH_RAINBOW_III			ZORRO_ID(HELFRICH_1, 0x21, 0)

#define ZORRO_MANUF_ATLANTIS					0x0862

#define ZORRO_MANUF_PROTAR					0x0864

#define ZORRO_MANUF_ACS						0x0865

#define ZORRO_MANUF_SOFTWARE_RESULTS_ENTERPRISES		0x0866
#define  ZORRO_PROD_SOFTWARE_RESULTS_ENTERPRISES_GOLDEN_GATE_2_BUS_PLUS	ZORRO_ID(SOFTWARE_RESULTS_ENTERPRISES, 0x01, 0)

#define ZORRO_MANUF_MASOBOSHI					0x086D
#define  ZORRO_PROD_MASOBOSHI_MASTER_CARD_SC201			ZORRO_ID(MASOBOSHI, 0x03, 0)
#define  ZORRO_PROD_MASOBOSHI_MASTER_CARD_MC702			ZORRO_ID(MASOBOSHI, 0x04, 0)
#define  ZORRO_PROD_MASOBOSHI_MVD_819				ZORRO_ID(MASOBOSHI, 0x07, 0)

#define ZORRO_MANUF_MAINHATTAN_DATA				0x086F
#define  ZORRO_PROD_MAINHATTAN_DATA_IDE				ZORRO_ID(MAINHATTAN_DATA, 0x01, 0)

#define ZORRO_MANUF_VILLAGE_TRONIC				0x0877
#define  ZORRO_PROD_VILLAGE_TRONIC_DOMINO_RAM			ZORRO_ID(VILLAGE_TRONIC, 0x01, 0)
#define  ZORRO_PROD_VILLAGE_TRONIC_DOMINO_REG			ZORRO_ID(VILLAGE_TRONIC, 0x02, 0)
#define  ZORRO_PROD_VILLAGE_TRONIC_DOMINO_16M_PROTOTYPE		ZORRO_ID(VILLAGE_TRONIC, 0x03, 0)
#define  ZORRO_PROD_VILLAGE_TRONIC_PICASSO_II_II_PLUS_RAM	ZORRO_ID(VILLAGE_TRONIC, 0x0B, 0)
#define  ZORRO_PROD_VILLAGE_TRONIC_PICASSO_II_II_PLUS_REG	ZORRO_ID(VILLAGE_TRONIC, 0x0C, 0)
#define  ZORRO_PROD_VILLAGE_TRONIC_PICASSO_II_II_PLUS_SEGMENTED_MODE	ZORRO_ID(VILLAGE_TRONIC, 0x0D, 0)
#define  ZORRO_PROD_VILLAGE_TRONIC_PICASSO_IV_Z2_MEM1		ZORRO_ID(VILLAGE_TRONIC, 0x15, 0)
#define  ZORRO_PROD_VILLAGE_TRONIC_PICASSO_IV_Z2_MEM2		ZORRO_ID(VILLAGE_TRONIC, 0x16, 0)
#define  ZORRO_PROD_VILLAGE_TRONIC_PICASSO_IV_Z2_REG		ZORRO_ID(VILLAGE_TRONIC, 0x17, 0)
#define  ZORRO_PROD_VILLAGE_TRONIC_PICASSO_IV_Z3		ZORRO_ID(VILLAGE_TRONIC, 0x18, 0)
#define  ZORRO_PROD_VILLAGE_TRONIC_ARIADNE			ZORRO_ID(VILLAGE_TRONIC, 0xC9, 0)
#define  ZORRO_PROD_VILLAGE_TRONIC_ARIADNE2			ZORRO_ID(VILLAGE_TRONIC, 0xCA, 0)

#define ZORRO_MANUF_UTILITIES_UNLIMITED				0x087B
#define  ZORRO_PROD_UTILITIES_UNLIMITED_EMPLANT_DELUXE		ZORRO_ID(UTILITIES_UNLIMITED, 0x15, 0)
#define  ZORRO_PROD_UTILITIES_UNLIMITED_EMPLANT_DELUXE2		ZORRO_ID(UTILITIES_UNLIMITED, 0x20, 0)

#define ZORRO_MANUF_AMITRIX					0x0880
#define  ZORRO_PROD_AMITRIX_MULTI_IO				ZORRO_ID(AMITRIX, 0x01, 0)
#define  ZORRO_PROD_AMITRIX_CD_RAM				ZORRO_ID(AMITRIX, 0x02, 0)

#define ZORRO_MANUF_ARMAX					0x0885
#define  ZORRO_PROD_ARMAX_OMNIBUS				ZORRO_ID(ARMAX, 0x00, 0)

#define ZORRO_MANUF_ZEUS					0x088D
#define  ZORRO_PROD_ZEUS_SPIDER					ZORRO_ID(ZEUS, 0x04, 0)

#define ZORRO_MANUF_NEWTEK					0x088F
#define  ZORRO_PROD_NEWTEK_VIDEOTOASTER				ZORRO_ID(NEWTEK, 0x00, 0)

#define ZORRO_MANUF_M_TECH_GERMANY				0x0890
#define  ZORRO_PROD_MTEC_AT500_2				ZORRO_ID(M_TECH_GERMANY, 0x01, 0)
#define  ZORRO_PROD_MTEC_68030					ZORRO_ID(M_TECH_GERMANY, 0x03, 0)
#define  ZORRO_PROD_MTEC_68020I					ZORRO_ID(M_TECH_GERMANY, 0x06, 0)
#define  ZORRO_PROD_MTEC_A1200_T68030_RTC			ZORRO_ID(M_TECH_GERMANY, 0x20, 0)
#define  ZORRO_PROD_MTEC_VIPER_MK_V_E_MATRIX_530		ZORRO_ID(M_TECH_GERMANY, 0x21, 0)
#define  ZORRO_PROD_MTEC_8_MB_RAM				ZORRO_ID(M_TECH_GERMANY, 0x22, 0)
#define  ZORRO_PROD_MTEC_VIPER_MK_V_E_MATRIX_530_SCSI_IDE	ZORRO_ID(M_TECH_GERMANY, 0x24, 0)

#define ZORRO_MANUF_GREAT_VALLEY_PRODUCTS_4			0x0891
#define  ZORRO_PROD_GVP_EGS_28_24_SPECTRUM_RAM			ZORRO_ID(GREAT_VALLEY_PRODUCTS_4, 0x01, 0)
#define  ZORRO_PROD_GVP_EGS_28_24_SPECTRUM_REG			ZORRO_ID(GREAT_VALLEY_PRODUCTS_4, 0x02, 0)

#define ZORRO_MANUF_APOLLO_1					0x0892
#define  ZORRO_PROD_APOLLO_A1200				ZORRO_ID(APOLLO_1, 0x01, 0)

#define ZORRO_MANUF_HELFRICH_2					0x0893
#define  ZORRO_PROD_HELFRICH_PICCOLO_RAM			ZORRO_ID(HELFRICH_2, 0x05, 0)
#define  ZORRO_PROD_HELFRICH_PICCOLO_REG			ZORRO_ID(HELFRICH_2, 0x06, 0)
#define  ZORRO_PROD_HELFRICH_PEGGY_PLUS_MPEG			ZORRO_ID(HELFRICH_2, 0x07, 0)
#define  ZORRO_PROD_HELFRICH_VIDEOCRUNCHER			ZORRO_ID(HELFRICH_2, 0x08, 0)
#define  ZORRO_PROD_HELFRICH_SD64_RAM				ZORRO_ID(HELFRICH_2, 0x0A, 0)
#define  ZORRO_PROD_HELFRICH_SD64_REG				ZORRO_ID(HELFRICH_2, 0x0B, 0)

#define ZORRO_MANUF_MACROSYSTEMS_USA				0x089B
#define  ZORRO_PROD_MACROSYSTEMS_WARP_ENGINE_40xx		ZORRO_ID(MACROSYSTEMS_USA, 0x13, 0)

#define ZORRO_MANUF_ELBOX_COMPUTER				0x089E
#define  ZORRO_PROD_ELBOX_COMPUTER_1200_4			ZORRO_ID(ELBOX_COMPUTER, 0x06, 0)

#define ZORRO_MANUF_HARMS_PROFESSIONAL				0x0A00
#define  ZORRO_PROD_HARMS_PROFESSIONAL_030_PLUS			ZORRO_ID(HARMS_PROFESSIONAL, 0x10, 0)
#define  ZORRO_PROD_HARMS_PROFESSIONAL_3500			ZORRO_ID(HARMS_PROFESSIONAL, 0xD0, 0)

#define ZORRO_MANUF_MICRONIK					0x0A50
#define  ZORRO_PROD_MICRONIK_RCA_120				ZORRO_ID(MICRONIK, 0x0A, 0)

#define ZORRO_MANUF_MICRONIK2					0x0F0F
#define  ZORRO_PROD_MICRONIK2_Z3I				ZORRO_ID(MICRONIK2, 0x01, 0)

#define ZORRO_MANUF_MEGAMICRO					0x1000
#define  ZORRO_PROD_MEGAMICRO_SCRAM_500				ZORRO_ID(MEGAMICRO, 0x03, 0)
#define  ZORRO_PROD_MEGAMICRO_SCRAM_500_RAM			ZORRO_ID(MEGAMICRO, 0x04, 0)

#define ZORRO_MANUF_IMTRONICS_2					0x1028
#define  ZORRO_PROD_IMTRONICS_HURRICANE_2800_3			ZORRO_ID(IMTRONICS_2, 0x39, 0)
#define  ZORRO_PROD_IMTRONICS_HURRICANE_2800_4			ZORRO_ID(IMTRONICS_2, 0x57, 0)

/* unofficial ID */
#define ZORRO_MANUF_INDIVIDUAL_COMPUTERS			0x1212
#define  ZORRO_PROD_INDIVIDUAL_COMPUTERS_BUDDHA			ZORRO_ID(INDIVIDUAL_COMPUTERS, 0x00, 0)
#define  ZORRO_PROD_INDIVIDUAL_COMPUTERS_X_SURF			ZORRO_ID(INDIVIDUAL_COMPUTERS, 0x17, 0)
#define  ZORRO_PROD_INDIVIDUAL_COMPUTERS_CATWEASEL		ZORRO_ID(INDIVIDUAL_COMPUTERS, 0x2A, 0)

#define ZORRO_MANUF_KUPKE_3					0x1248
#define  ZORRO_PROD_KUPKE_GOLEM_HD_3000				ZORRO_ID(KUPKE_3, 0x01, 0)

#define ZORRO_MANUF_ITH						0x1388
#define  ZORRO_PROD_ITH_ISDN_MASTER_II				ZORRO_ID(ITH, 0x01, 0)

#define ZORRO_MANUF_VMC						0x1389
#define  ZORRO_PROD_VMC_ISDN_BLASTER_Z2				ZORRO_ID(VMC, 0x01, 0)
#define  ZORRO_PROD_VMC_HYPERCOM_4				ZORRO_ID(VMC, 0x02, 0)

#define ZORRO_MANUF_INFORMATION					0x157C
#define  ZORRO_PROD_INFORMATION_ISDN_ENGINE_I			ZORRO_ID(INFORMATION, 0x64, 0)

#define ZORRO_MANUF_VORTEX					0x2017
#define  ZORRO_PROD_VORTEX_GOLDEN_GATE_80386SX			ZORRO_ID(VORTEX, 0x07, 0)
#define  ZORRO_PROD_VORTEX_GOLDEN_GATE_RAM			ZORRO_ID(VORTEX, 0x08, 0)
#define  ZORRO_PROD_VORTEX_GOLDEN_GATE_80486			ZORRO_ID(VORTEX, 0x09, 0)

#define ZORRO_MANUF_EXPANSION_SYSTEMS				0x2062
#define  ZORRO_PROD_EXPANSION_SYSTEMS_DATAFLYER_4000SX		ZORRO_ID(EXPANSION_SYSTEMS, 0x01, 0)
#define  ZORRO_PROD_EXPANSION_SYSTEMS_DATAFLYER_4000SX_RAM	ZORRO_ID(EXPANSION_SYSTEMS, 0x02, 0)

#define ZORRO_MANUF_READYSOFT					0x2100
#define  ZORRO_PROD_READYSOFT_AMAX_II_IV			ZORRO_ID(READYSOFT, 0x01, 0)

#define ZORRO_MANUF_PHASE5					0x2140
#define  ZORRO_PROD_PHASE5_BLIZZARD_RAM				ZORRO_ID(PHASE5, 0x01, 0)
#define  ZORRO_PROD_PHASE5_BLIZZARD				ZORRO_ID(PHASE5, 0x02, 0)
#define  ZORRO_PROD_PHASE5_BLIZZARD_1220_IV			ZORRO_ID(PHASE5, 0x06, 0)
#define  ZORRO_PROD_PHASE5_FASTLANE_Z3_RAM			ZORRO_ID(PHASE5, 0x0A, 0)
#define  ZORRO_PROD_PHASE5_BLIZZARD_1230_II_FASTLANE_Z3_CYBERSCSI_CYBERSTORM060	ZORRO_ID(PHASE5, 0x0B, 0)
#define  ZORRO_PROD_PHASE5_BLIZZARD_1220_CYBERSTORM		ZORRO_ID(PHASE5, 0x0C, 0)
#define  ZORRO_PROD_PHASE5_BLIZZARD_1230			ZORRO_ID(PHASE5, 0x0D, 0)
#define  ZORRO_PROD_PHASE5_BLIZZARD_1230_IV_1260		ZORRO_ID(PHASE5, 0x11, 0)
#define  ZORRO_PROD_PHASE5_BLIZZARD_2060			ZORRO_ID(PHASE5, 0x18, 0)
#define  ZORRO_PROD_PHASE5_CYBERSTORM_MK_II			ZORRO_ID(PHASE5, 0x19, 0)
#define  ZORRO_PROD_PHASE5_CYBERVISION64			ZORRO_ID(PHASE5, 0x22, 0)
#define  ZORRO_PROD_PHASE5_CYBERVISION64_3D_PROTOTYPE		ZORRO_ID(PHASE5, 0x32, 0)
#define  ZORRO_PROD_PHASE5_CYBERVISION64_3D			ZORRO_ID(PHASE5, 0x43, 0)
#define  ZORRO_PROD_PHASE5_CYBERSTORM_MK_III			ZORRO_ID(PHASE5, 0x64, 0)
#define  ZORRO_PROD_PHASE5_BLIZZARD_603E_PLUS			ZORRO_ID(PHASE5, 0x6e, 0)

#define ZORRO_MANUF_DPS						0x2169
#define  ZORRO_PROD_DPS_PERSONAL_ANIMATION_RECORDER		ZORRO_ID(DPS, 0x01, 0)

#define ZORRO_MANUF_APOLLO_2					0x2200
#define  ZORRO_PROD_APOLLO_A620_68020_1				ZORRO_ID(APOLLO_2, 0x00, 0)
#define  ZORRO_PROD_APOLLO_A620_68020_2				ZORRO_ID(APOLLO_2, 0x01, 0)

#define ZORRO_MANUF_APOLLO_3					0x2222
#define  ZORRO_PROD_APOLLO_AT_APOLLO				ZORRO_ID(APOLLO_3, 0x22, 0)
#define  ZORRO_PROD_APOLLO_1230_1240_1260_2030_4040_4060	ZORRO_ID(APOLLO_3, 0x23, 0)

#define ZORRO_MANUF_PETSOFF_LP					0x38A5
#define  ZORRO_PROD_PETSOFF_LP_DELFINA				ZORRO_ID(PETSOFF_LP, 0x00, 0)
#define  ZORRO_PROD_PETSOFF_LP_DELFINA_LITE			ZORRO_ID(PETSOFF_LP, 0x01, 0)

#define ZORRO_MANUF_UWE_GERLACH					0x3FF7
#define  ZORRO_PROD_UWE_GERLACH_RAM_ROM				ZORRO_ID(UWE_GERLACH, 0xd4, 0)

#define ZORRO_MANUF_ACT						0x4231
#define  ZORRO_PROD_ACT_PRELUDE					ZORRO_ID(ACT, 0x01, 0)

#define ZORRO_MANUF_MACROSYSTEMS_GERMANY			0x4754
#define  ZORRO_PROD_MACROSYSTEMS_MAESTRO			ZORRO_ID(MACROSYSTEMS_GERMANY, 0x03, 0)
#define  ZORRO_PROD_MACROSYSTEMS_VLAB				ZORRO_ID(MACROSYSTEMS_GERMANY, 0x04, 0)
#define  ZORRO_PROD_MACROSYSTEMS_MAESTRO_PRO			ZORRO_ID(MACROSYSTEMS_GERMANY, 0x05, 0)
#define  ZORRO_PROD_MACROSYSTEMS_RETINA				ZORRO_ID(MACROSYSTEMS_GERMANY, 0x06, 0)
#define  ZORRO_PROD_MACROSYSTEMS_MULTI_EVOLUTION		ZORRO_ID(MACROSYSTEMS_GERMANY, 0x08, 0)
#define  ZORRO_PROD_MACROSYSTEMS_TOCCATA			ZORRO_ID(MACROSYSTEMS_GERMANY, 0x0C, 0)
#define  ZORRO_PROD_MACROSYSTEMS_RETINA_Z3			ZORRO_ID(MACROSYSTEMS_GERMANY, 0x10, 0)
#define  ZORRO_PROD_MACROSYSTEMS_VLAB_MOTION			ZORRO_ID(MACROSYSTEMS_GERMANY, 0x12, 0)
#define  ZORRO_PROD_MACROSYSTEMS_ALTAIS				ZORRO_ID(MACROSYSTEMS_GERMANY, 0x13, 0)
#define  ZORRO_PROD_MACROSYSTEMS_FALCON_040			ZORRO_ID(MACROSYSTEMS_GERMANY, 0xFD, 0)

#define ZORRO_MANUF_COMBITEC					0x6766

#define ZORRO_MANUF_SKI_PERIPHERALS				0x8000
#define  ZORRO_PROD_SKI_PERIPHERALS_MAST_FIREBALL		ZORRO_ID(SKI_PERIPHERALS, 0x08, 0)
#define  ZORRO_PROD_SKI_PERIPHERALS_SCSI_DUAL_SERIAL		ZORRO_ID(SKI_PERIPHERALS, 0x80, 0)

#define ZORRO_MANUF_REIS_WARE_2					0xA9AD
#define  ZORRO_PROD_REIS_WARE_SCAN_KING				ZORRO_ID(REIS_WARE_2, 0x11, 0)

#define ZORRO_MANUF_CAMERON					0xAA01
#define  ZORRO_PROD_CAMERON_PERSONAL_A4				ZORRO_ID(CAMERON, 0x10, 0)

#define ZORRO_MANUF_REIS_WARE					0xAA11
#define  ZORRO_PROD_REIS_WARE_HANDYSCANNER			ZORRO_ID(REIS_WARE, 0x11, 0)

#define ZORRO_MANUF_PHOENIX_2					0xB5A8
#define  ZORRO_PROD_PHOENIX_ST506_2				ZORRO_ID(PHOENIX_2, 0x21, 0)
#define  ZORRO_PROD_PHOENIX_SCSI_2				ZORRO_ID(PHOENIX_2, 0x22, 0)
#define  ZORRO_PROD_PHOENIX_RAM_2				ZORRO_ID(PHOENIX_2, 0xBE, 0)

#define ZORRO_MANUF_COMBITEC_2					0xC008
#define  ZORRO_PROD_COMBITEC_HD					ZORRO_ID(COMBITEC_2, 0x2A, 0)
#define  ZORRO_PROD_COMBITEC_SRAM				ZORRO_ID(COMBITEC_2, 0x2B, 0)


    /*
     *  Test and illegal Manufacturer IDs.
     */

#define ZORRO_MANUF_HACKER					0x07DB
#define  ZORRO_PROD_GENERAL_PROTOTYPE				ZORRO_ID(HACKER, 0x00, 0)
#define  ZORRO_PROD_HACKER_SCSI					ZORRO_ID(HACKER, 0x01, 0)
#define  ZORRO_PROD_RESOURCE_MANAGEMENT_FORCE_QUICKNET_QN2000	ZORRO_ID(HACKER, 0x02, 0)
#define  ZORRO_PROD_VECTOR_CONNECTION_2				ZORRO_ID(HACKER, 0xE0, 0)
#define  ZORRO_PROD_VECTOR_CONNECTION_3				ZORRO_ID(HACKER, 0xE1, 0)
#define  ZORRO_PROD_VECTOR_CONNECTION_4				ZORRO_ID(HACKER, 0xE2, 0)
#define  ZORRO_PROD_VECTOR_CONNECTION_5				ZORRO_ID(HACKER, 0xE3, 0)
