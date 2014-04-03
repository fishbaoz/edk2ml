/* $NoKeywords:$ */
/**
 * @file
 *
 * mpSml.c
 *
 * Platform specific settings for ML DDR3 SO-DIMM system
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Ps/ML)
 * @e \$Revision: 281178 $ @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 **/
/*****************************************************************************
  *
  * Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
  *
  * AMD is granting you permission to use this software (the Materials)
  * pursuant to the terms and conditions of your Software License Agreement
  * with AMD.  This header does *NOT* give you permission to use the Materials
  * or any rights under AMD's intellectual property.  Your use of any portion
  * of these Materials shall constitute your acceptance of those terms and
  * conditions.  If you do not agree to the terms and conditions of the Software
  * License Agreement, please do not use any portion of these Materials.
  *
  * CONFIDENTIALITY:  The Materials and all other information, identified as
  * confidential and provided to you by AMD shall be kept confidential in
  * accordance with the terms and conditions of the Software License Agreement.
  *
  * LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
  * PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
  * WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
  * MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
  * OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
  * IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
  * (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
  * INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
  * GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
  * RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
  * THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
  * EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
  * THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
  *
  * AMD does not assume any responsibility for any errors which may appear in
  * the Materials or any other related information provided to you by AMD, or
  * result from use of the Materials or any related information.
  *
  * You agree that you will not reverse engineer or decompile the Materials.
  *
  * NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
  * further information, software, technical information, know-how, or show-how
  * available to you.  Additionally, AMD retains the right to modify the
  * Materials at any time, without notice, and is not obligated to provide such
  * modified Materials to you.
  *
  * U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
  * "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
  * subject to the restrictions as set forth in FAR 52.227-14 and
  * DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
  * Government constitutes acknowledgement of AMD's proprietary rights in them.
  *
  * EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
  * direct product thereof will be exported directly or indirectly, into any
  * country prohibited by the United States Export Administration Act and the
  * regulations thereunder, without the required authorization from the U.S.
  * government nor will be used for any purpose prohibited by the same.
  * ***************************************************************************
  *
 */

#include "AGESA.h"
#include "AdvancedApi.h"
#include "mport.h"
#include "ma.h"
#include "Ids.h"
#include "cpuFamRegisters.h"
#include "cpuRegisters.h"
#include "mm.h"
#include "mn.h"
#include "mp.h"
#include "mu.h"
#include "OptionMemory.h"
#include "PlatformMemoryConfiguration.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_PS_ML_MPSML3_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
/*
 *-----------------------------------------------------------------------------
 *                                EXPORTED FUNCTIONS
 *
 *-----------------------------------------------------------------------------
 */
// Slow mode, POdtOff, Address timing and Output drive compensation for SODIMM ONLY configuration
// Format :
// DimmPerCh,   DDRrate,   VDDIO,   Dimm0,   Dimm1,   Dimm2,   SlowMode,   AddTmgCtl,   ODC,  POdtOff
//
STATIC CONST PSCFG_SAO_ENTRY MLSODdr3SAO[] = {
  {_1DIMM, DDR667 + DDR800, VOLT_ALL, DIMM_SR + DIMM_DR, NP, NP, 0, 0x00000000, 0x00002222, 0},
  {_1DIMM, DDR1066, VOLT_ALL, DIMM_SR, NP, NP, 0, 0x003D3D3D, 0x10002222, 0},
  {_1DIMM, DDR1066, VOLT_ALL, DIMM_DR, NP, NP, 0, 0x00000000, 0x10002222, 0},
  {_1DIMM, DDR1333, VOLT_ALL, DIMM_SR, NP, NP, 0, 0x003D3D3D, 0x20112222, 0},
  {_1DIMM, DDR1333, VOLT_ALL, DIMM_DR, NP, NP, 0, 0x00003D3D, 0x20112222, 0},
  {_1DIMM, DDR1600, V1_5 + V1_35, DIMM_SR, NP, NP, 0, 0x003C3C3C, 0x30332222, 0},
  {_1DIMM, DDR1600, V1_5 + V1_35, DIMM_DR, NP, NP, 1, 0x00003C3C, 0x30332222, 0},
  {_1DIMM, DDR1866, V1_5, DIMM_SR, NP, NP, 0, 0x003C3C3C, 0x30332222, 0},
  {_1DIMM, DDR1866, V1_5, DIMM_DR, NP, NP, 1, 0x00003C3C, 0x30332222, 0},
  {_2DIMM, DDR667 + DDR800, VOLT_ALL, NP, DIMM_SR + DIMM_DR, NP, 0, 0x00000000, 0x00002222, 0},
  {_2DIMM, DDR667 + DDR800, VOLT_ALL, DIMM_SR + DIMM_DR, NP, NP, 0, 0x00000000, 0x00002222, 0},
  {_2DIMM, DDR667, VOLT_ALL, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, 1, 0x00000000, 0x10222323, 0},
  {_2DIMM, DDR800, VOLT_ALL, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, 1, 0x00000000, 0x20222323, 0},
  {_2DIMM, DDR1066, VOLT_ALL, NP, DIMM_SR, NP, 0, 0x003D3D3D, 0x10002222, 0},
  {_2DIMM, DDR1066, VOLT_ALL, NP, DIMM_DR, NP, 0, 0x00000000, 0x10002222, 0},
  {_2DIMM, DDR1066, VOLT_ALL, DIMM_SR, NP, NP, 0, 0x003D3D3D, 0x10002222, 0},
  {_2DIMM, DDR1066, VOLT_ALL, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, 1, 0x00000000, 0x30222323, 0},
  {_2DIMM, DDR1066, VOLT_ALL, DIMM_DR, NP, NP, 0, 0x00000000, 0x10002222, 0},
  {_2DIMM, DDR1333, VOLT_ALL, NP, DIMM_SR, NP, 0, 0x003D3D3D, 0x20112222, 0},
  {_2DIMM, DDR1333, VOLT_ALL, NP, DIMM_DR, NP, 0, 0x00003D3D, 0x20112222, 0},
  {_2DIMM, DDR1333, VOLT_ALL, DIMM_SR, NP, NP, 0, 0x003D3D3D, 0x20112222, 0},
  {_2DIMM, DDR1333, V1_5 + V1_35, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, 1, 0x00000000, 0x30222323, 0},
  {_2DIMM, DDR1333, VOLT_ALL, DIMM_DR, NP, NP, 0, 0x00003D3D, 0x20112222, 0},
  {_2DIMM, DDR1333, V1_25, DIMM_SR, DIMM_SR, NP, 1, 0x00000000, 0x30222323, 0},
  {_2DIMM, DDR1600, V1_5 + V1_35, NP, DIMM_SR, NP, 0, 0x003C3C3C, 0x30332222, 0},
  {_2DIMM, DDR1600, V1_5 + V1_35, NP, DIMM_DR, NP, 1, 0x00003C3C, 0x30332222, 0},
  {_2DIMM, DDR1600, V1_5 + V1_35, DIMM_SR, NP, NP, 0, 0x003C3C3C, 0x30332222, 0},
  {_2DIMM, DDR1600, V1_5, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, 1, 0x00000000, 0x30222323, 0},
  {_2DIMM, DDR1600, V1_5 + V1_35, DIMM_DR, NP, NP, 1, 0x00003C3C, 0x30332222, 0},
  {_2DIMM, DDR1600, V1_35, DIMM_SR, DIMM_SR, NP, 1, 0x00000000, 0x30222323, 0},
  {_2DIMM, DDR1866, V1_5, NP, DIMM_SR, NP, 0, 0x003C3C3C, 0x30332222, 0},
  {_2DIMM, DDR1866, V1_5, NP, DIMM_DR, NP, 1, 0x00003C3C, 0x30332222, 0},
  {_2DIMM, DDR1866, V1_5, DIMM_SR, NP, NP, 0, 0x003C3C3C, 0x30332222, 0},
  {_2DIMM, DDR1866, V1_5, DIMM_DR, NP, NP, 1, 0x00003C3C, 0x30332222, 0},
};
CONST PSC_TBL_ENTRY MLSAOTblEntSO3 = {
   {PSCFG_SAO, SODIMM_TYPE, _1DIMM + _2DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, MBL_DONT_CARE, MBP_DONT_CARE},
   sizeof (MLSODdr3SAO) / sizeof (PSCFG_SAO_ENTRY),
   (VOID *)&MLSODdr3SAO
};

// Slow mode, POdtOff, Address timing and Output drive compensation for SODIMM plus Solder-down DRAM configuration
// Format :
// DimmPerCh,   DDRrate,   VDDIO,   Dimm0,   Dimm1,   Dimm2,   SlowMode,   AddTmgCtl,   ODC,   POdtOff
//
STATIC CONST PSCFG_SAO_ENTRY MLSoDwnPlusSODIMMDdr3SAO[] = {
  {_1DIMM, DDR667 + DDR800, VOLT_ALL, NP, DIMM_SR + DIMM_DR, NP, 0, 0x00000000, 0x00002222, 0},
  {_1DIMM, DDR667 + DDR800, VOLT_ALL, DIMM_SR + DIMM_DR, NP, NP, 0, 0x00000000, 0x00002222, 0},
  {_1DIMM, DDR667, VOLT_ALL, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, 1, 0x00000000, 0x10222323, 0},
  {_1DIMM, DDR800, VOLT_ALL, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, 1, 0x00000000, 0x20222323, 0},
  {_1DIMM, DDR1066, VOLT_ALL, NP, DIMM_SR, NP, 0, 0x003D3D3D, 0x10002222, 0},
  {_1DIMM, DDR1066, VOLT_ALL, NP, DIMM_DR, NP, 0, 0x00000000, 0x10002222, 0},
  {_1DIMM, DDR1066, VOLT_ALL, DIMM_SR, NP, NP, 0, 0x003D3D3D, 0x10002222, 0},
  {_1DIMM, DDR1066, VOLT_ALL, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, 1, 0x00000000, 0x30222323, 0},
  {_1DIMM, DDR1066, VOLT_ALL, DIMM_DR, NP, NP, 0, 0x00000000, 0x10002222, 0},
  {_1DIMM, DDR1333, V1_5 + V1_35, NP, DIMM_SR, NP, 0, 0x003D3D3D, 0x20112222, 0},
  {_1DIMM, DDR1333, V1_5 + V1_35, NP, DIMM_DR, NP, 0, 0x00003D3D, 0x20112222, 0},
  {_1DIMM, DDR1333, V1_5 + V1_35, DIMM_SR, NP, NP, 0, 0x003D3D3D, 0x20112222, 0},
  {_1DIMM, DDR1333, V1_5, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, 1, 0x00000000, 0x30222323, 0},
  {_1DIMM, DDR1333, V1_5 + V1_35, DIMM_DR, NP, NP, 0, 0x00003D3D, 0x20112222, 0},
  {_1DIMM, DDR1333, V1_35, DIMM_SR, DIMM_SR, NP, 1, 0x00000000, 0x30222323, 0},
};
CONST PSC_TBL_ENTRY MLSAOTblEntSoDwnPlusSODIMM3 = {
   {PSCFG_SAO, SODWN_SODIMM_TYPE, _1DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, MBL_DONT_CARE, MBP_DONT_CARE},
   sizeof (MLSoDwnPlusSODIMMDdr3SAO) / sizeof (PSCFG_SAO_ENTRY),
   (VOID *)&MLSoDwnPlusSODIMMDdr3SAO
};

// Slow mode, POdtOff, Address timing and Output drive compensation for Solder-down DRAM ONLY configuration
// Format :
// DimmPerCh,   DDRrate,   VDDIO,   Dimm0,   Dimm1,   Dimm2,   SlowMode,   AddTmgCtl,   ODC,   POdtOff
//
STATIC CONST PSCFG_SAO_ENTRY MLSoDwnDdr3SAO[] = {
  {_DIMM_NONE, DDR667, VOLT_ALL, DIMM_SR + DIMM_DR, NP, NP, 0, 0x00000000, 0x00000000, 1},
  {_DIMM_NONE, DDR800, VOLT_ALL, DIMM_SR + DIMM_DR, NP, NP, 0, 0x00000000, 0x00000000, 0},
  {_DIMM_NONE, DDR1066, VOLT_ALL, DIMM_SR, NP, NP, 0, 0x003D3D3D, 0x10000000, 0},
  {_DIMM_NONE, DDR1066, VOLT_ALL, DIMM_DR, NP, NP, 0, 0x00000000, 0x10000000, 0},
  {_DIMM_NONE, DDR1333, VOLT_ALL, DIMM_SR, NP, NP, 0, 0x003D3D3D, 0x20000000, 0},
  {_DIMM_NONE, DDR1333, VOLT_ALL, DIMM_DR, NP, NP, 0, 0x00003D3D, 0x20110000, 0},
  {_DIMM_NONE, DDR1600, V1_5 + V1_35, DIMM_SR, NP, NP, 0, 0x003C3C3C, 0x30110000, 0},
  {_DIMM_NONE, DDR1600, V1_5 + V1_35, DIMM_DR, NP, NP, 1, 0x00003C3C, 0x30110000, 0},
  {_DIMM_NONE, DDR1866, V1_5, DIMM_SR, NP, NP, 0, 0x003C3C3C, 0x30110000, 0},
  {_DIMM_NONE, DDR1866, V1_5, DIMM_DR, NP, NP, 1, 0x00003C3C, 0x30110000, 0},
};
CONST PSC_TBL_ENTRY MLSAOTblEntSoDwn3 = {
   {PSCFG_SAO, SODWN_SODIMM_TYPE, _DIMM_NONE, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, MBL_DONT_CARE, HPM},
   sizeof (MLSoDwnDdr3SAO) / sizeof (PSCFG_SAO_ENTRY),
   (VOID *)&MLSoDwnDdr3SAO
};

// Slow mode, POdtOff, Address timing and Output drive compensation for Solder-down DRAM ONLY <Low Power Motherboard Design> configuration
// Format :
// DimmPerCh,   DDRrate,   VDDIO,   Dimm0,   Dimm1,   Dimm2,   SlowMode,   AddTmgCtl,   ODC,   POdtOff
//
STATIC CONST PSCFG_SAO_ENTRY MLSoDwnDdr3SAOLPM[] = {
  {_DIMM_NONE, DDR667, V1_5 + V1_35, DIMM_SR + DIMM_DR, NP, NP, 0, 0x00303030, 0x00000000, 1},
  {_DIMM_NONE, DDR800, V1_5 + V1_35, DIMM_SR + DIMM_DR, NP, NP, 0, 0x00303030, 0x00001111, 1},
  {_DIMM_NONE, DDR1066, V1_5 + V1_35, DIMM_SR, NP, NP, 0, 0x00303030, 0x00002111, 1},
  {_DIMM_NONE, DDR1066, V1_5 + V1_35, DIMM_DR, NP, NP, 0, 0x00303030, 0x00002222, 1},
  {_DIMM_NONE, DDR1333, V1_5 + V1_35, DIMM_SR + DIMM_DR, NP, NP, 0, 0x00303030, 0x00112222, 1},
};
CONST PSC_TBL_ENTRY MLSAOTblEntSoDwn3LPM = {
   {PSCFG_SAO, SODWN_SODIMM_TYPE, _DIMM_NONE, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, MBL_DONT_CARE, LPM},
   sizeof (MLSoDwnDdr3SAOLPM) / sizeof (PSCFG_SAO_ENTRY),
   (VOID *)&MLSoDwnDdr3SAOLPM
};

// Dram Term and Dynamic Dram Term for SODIMM ONLY configuration
// Format :
// DimmPerCh,   DDRrate,   VDDIO,   Dimm0,   Dimm1,   Dimm2,   Dimm,   Rank,   RttNom,    RttWr
//
// RttNom:
// 0 On die termination disabled
// 1 60ohms
// 2 120ohms
// 3 40ohms
// 4 20ohms
// 5 30ohms
// RttWr:
// 0 Dynamic termination for writes disabled.
// 1 60ohms
// 2 120ohms
STATIC CONST PSCFG_RTT_ENTRY MLDramTermSODIMM3[] = {
  {_1DIMM, DDR667 + DDR800 + DDR1066, VOLT_ALL, DIMM_SR, NP, NP, DIMM_SR, R0, 2, 0},
  {_1DIMM, DDR667 + DDR800 + DDR1066, VOLT_ALL, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 2, 0},
  {_1DIMM, DDR1333, VOLT_ALL, DIMM_SR, NP, NP, DIMM_SR, R0, 1, 0},
  {_1DIMM, DDR1333, VOLT_ALL, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 1, 0},
  {_1DIMM, DDR1600, V1_5 + V1_35, DIMM_SR, NP, NP, DIMM_SR, R0, 1, 0},
  {_1DIMM, DDR1600, V1_5 + V1_35, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 1, 0},
  {_1DIMM, DDR1866, V1_5, DIMM_SR, NP, NP, DIMM_SR, R0, 1, 0},
  {_1DIMM, DDR1866, V1_5, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 1, 0},
  {_2DIMM, DDR667 + DDR800 + DDR1066, VOLT_ALL, NP, DIMM_SR, NP, DIMM_SR, R0, 2, 0},
  {_2DIMM, DDR667 + DDR800 + DDR1066, VOLT_ALL, NP, DIMM_DR, NP, DIMM_DR, R0 + R1, 2, 0},
  {_2DIMM, DDR667 + DDR800 + DDR1066, VOLT_ALL, DIMM_SR, NP, NP, DIMM_SR, R0, 2, 0},
  {_2DIMM, DDR667 + DDR800, VOLT_ALL, DIMM_SR, DIMM_SR + DIMM_DR, NP, DIMM_SR, R0, 3, 2},
  {_2DIMM, DDR667 + DDR800, VOLT_ALL, DIMM_SR, DIMM_DR, NP, DIMM_DR, R0 + R1, 3, 2},
  {_2DIMM, DDR667 + DDR800 + DDR1066, VOLT_ALL, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 2, 0},
  {_2DIMM, DDR667 + DDR800, VOLT_ALL, DIMM_DR, DIMM_SR, NP, DIMM_SR, R0, 3, 2},
  {_2DIMM, DDR667 + DDR800, VOLT_ALL, DIMM_DR, DIMM_SR + DIMM_DR, NP, DIMM_DR, R0 + R1, 3, 2},
  {_2DIMM, DDR1066, VOLT_ALL, DIMM_SR, DIMM_SR + DIMM_DR, NP, DIMM_SR, R0, 5, 2},
  {_2DIMM, DDR1066, VOLT_ALL, DIMM_SR, DIMM_DR, NP, DIMM_DR, R0 + R1, 5, 2},
  {_2DIMM, DDR1066, VOLT_ALL, DIMM_DR, DIMM_SR, NP, DIMM_SR, R0, 5, 2},
  {_2DIMM, DDR1066, VOLT_ALL, DIMM_DR, DIMM_SR + DIMM_DR, NP, DIMM_DR, R0 + R1, 5, 2},
  {_2DIMM, DDR1333, VOLT_ALL, NP, DIMM_SR, NP, DIMM_SR, R0, 1, 0},
  {_2DIMM, DDR1333, VOLT_ALL, NP, DIMM_DR, NP, DIMM_DR, R0 + R1, 1, 0},
  {_2DIMM, DDR1333, VOLT_ALL, DIMM_SR, NP, NP, DIMM_SR, R0, 1, 0},
  {_2DIMM, DDR1333, V1_5 + V1_35, DIMM_SR, DIMM_SR + DIMM_DR, NP, DIMM_SR, R0, 5, 2},
  {_2DIMM, DDR1333, V1_5 + V1_35, DIMM_SR, DIMM_DR, NP, DIMM_DR, R0 + R1, 5, 2},
  {_2DIMM, DDR1333, VOLT_ALL, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 1, 0},
  {_2DIMM, DDR1333, V1_5 + V1_35, DIMM_DR, DIMM_SR, NP, DIMM_SR, R0, 5, 2},
  {_2DIMM, DDR1333, V1_5 + V1_35, DIMM_DR, DIMM_SR + DIMM_DR, NP, DIMM_DR, R0 + R1, 5, 2},
  {_2DIMM, DDR1333, V1_25, DIMM_SR, DIMM_SR, NP, DIMM_SR, R0, 5, 2},
  {_2DIMM, DDR1600, V1_5 + V1_35, NP, DIMM_SR, NP, DIMM_SR, R0, 1, 0},
  {_2DIMM, DDR1600, V1_5 + V1_35, NP, DIMM_DR, NP, DIMM_DR, R0 + R1, 1, 0},
  {_2DIMM, DDR1600, V1_5 + V1_35, DIMM_SR, NP, NP, DIMM_SR, R0, 1, 0},
  {_2DIMM, DDR1600, V1_5, DIMM_SR, DIMM_SR + DIMM_DR, NP, DIMM_SR, R0, 4, 1},
  {_2DIMM, DDR1600, V1_5, DIMM_SR, DIMM_DR, NP, DIMM_DR, R0 + R1, 4, 1},
  {_2DIMM, DDR1600, V1_5 + V1_35, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 1, 0},
  {_2DIMM, DDR1600, V1_5, DIMM_DR, DIMM_SR, NP, DIMM_SR, R0, 4, 1},
  {_2DIMM, DDR1600, V1_5, DIMM_DR, DIMM_SR + DIMM_DR, NP, DIMM_DR, R0 + R1, 4, 1},
  {_2DIMM, DDR1600, V1_35, DIMM_SR, DIMM_SR, NP, DIMM_SR, R0, 4, 1},
  {_2DIMM, DDR1866, V1_5, NP, DIMM_SR, NP, DIMM_SR, R0, 1, 0},
  {_2DIMM, DDR1866, V1_5, NP, DIMM_DR, NP, DIMM_DR, R0 + R1, 1, 0},
  {_2DIMM, DDR1866, V1_5, DIMM_SR, NP, NP, DIMM_SR, R0, 1, 0},
  {_2DIMM, DDR1866, V1_5, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 1, 0},

};
CONST PSC_TBL_ENTRY MLDramTermTblEntSO3 = {
   {PSCFG_RTT, SODIMM_TYPE, _1DIMM + _2DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, MBL_DONT_CARE, MBP_DONT_CARE},
   sizeof (MLDramTermSODIMM3) / sizeof (PSCFG_RTT_ENTRY),
   (VOID *)&MLDramTermSODIMM3
};

// Dram Term and Dynamic Dram Term for SODIMM plus Solder-down DRAM configuration
// Format :
// DimmPerCh,   DDRrate,   VDDIO,   Dimm0,   Dimm1,   Dimm2,   Dimm,   Rank,   RttNom,    RttWr
//
// RttNom:
// 0 On die termination disabled
// 1 60ohms
// 2 120ohms
// 3 40ohms
// 4 20ohms
// 5 30ohms
// RttWr:
// 0 Dynamic termination for writes disabled.
// 1 60ohms
// 2 120ohms
STATIC CONST PSCFG_RTT_ENTRY MLDramTermSoDwnPlusSODIMM3[] = {
  {_1DIMM, DDR667 + DDR800 + DDR1066, VOLT_ALL, NP, DIMM_SR, NP, DIMM_SR, R0, 2, 0},
  {_1DIMM, DDR667 + DDR800 + DDR1066, VOLT_ALL, NP, DIMM_DR, NP, DIMM_DR, R0 + R1, 2, 0},
  {_1DIMM, DDR667 + DDR800 + DDR1066, VOLT_ALL, DIMM_SR, NP, NP, DIMM_SR, R0, 2, 0},
  {_1DIMM, DDR667 + DDR800, VOLT_ALL, DIMM_SR, DIMM_SR + DIMM_DR, NP, DIMM_SR, R0, 3, 2},
  {_1DIMM, DDR667 + DDR800, VOLT_ALL, DIMM_SR, DIMM_DR, NP, DIMM_DR, R0 + R1, 3, 2},
  {_1DIMM, DDR667 + DDR800 + DDR1066, VOLT_ALL, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 2, 0},
  {_1DIMM, DDR667 + DDR800, VOLT_ALL, DIMM_DR, DIMM_SR, NP, DIMM_SR, R0, 3, 2},
  {_1DIMM, DDR667 + DDR800, VOLT_ALL, DIMM_DR, DIMM_SR + DIMM_DR, NP, DIMM_DR, R0 + R1, 3, 2},
  {_1DIMM, DDR1066, VOLT_ALL, DIMM_SR, DIMM_SR + DIMM_DR, NP, DIMM_SR, R0, 5, 2},
  {_1DIMM, DDR1066, VOLT_ALL, DIMM_SR, DIMM_DR, NP, DIMM_DR, R0 + R1, 5, 2},
  {_1DIMM, DDR1066, VOLT_ALL, DIMM_DR, DIMM_SR, NP, DIMM_SR, R0, 5, 2},
  {_1DIMM, DDR1066, VOLT_ALL, DIMM_DR, DIMM_SR + DIMM_DR, NP, DIMM_DR, R0 + R1, 5, 2},
  {_1DIMM, DDR1333, V1_5 + V1_35, NP, DIMM_SR, NP, DIMM_SR, R0, 1, 0},
  {_1DIMM, DDR1333, V1_5 + V1_35, NP, DIMM_DR, NP, DIMM_DR, R0 + R1, 1, 0},
  {_1DIMM, DDR1333, V1_5 + V1_35, DIMM_SR, NP, NP, DIMM_SR, R0, 1, 0},
  {_1DIMM, DDR1333, V1_5, DIMM_SR, DIMM_SR + DIMM_DR, DIMM_SR, R0, 5, 2},
  {_1DIMM, DDR1333, V1_5, DIMM_SR, DIMM_DR, NP, DIMM_DR, R0 + R1, 5, 2},
  {_1DIMM, DDR1333, V1_5 + V1_35, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 1, 0},
  {_1DIMM, DDR1333, V1_5, DIMM_DR, DIMM_SR, NP, DIMM_SR, R0, 5, 2},
  {_1DIMM, DDR1333, V1_5, DIMM_DR, DIMM_SR + DIMM_DR, NP, DIMM_DR, R0 + R1, 5, 2},
  {_1DIMM, DDR1333, V1_35, DIMM_SR, DIMM_SR, NP, DIMM_SR, R0, 5, 2},
};
CONST PSC_TBL_ENTRY MLDramTermTblEntSoDwnPlusSODIMM3 = {
   {PSCFG_RTT, SODWN_SODIMM_TYPE, _1DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, MBL_DONT_CARE, MBP_DONT_CARE},
   sizeof (MLDramTermSoDwnPlusSODIMM3) / sizeof (PSCFG_RTT_ENTRY),
   (VOID *)&MLDramTermSoDwnPlusSODIMM3
};

// Dram Term and Dynamic Dram Term for Solder-down DRAM ONLY configuration
// Format :
// DimmPerCh,   DDRrate,   VDDIO,   Dimm0,   Dimm1,   Dimm2,   Dimm,   Rank,   RttNom,    RttWr
//
// RttNom:
// 0 On die termination disabled
// 1 60ohms
// 2 120ohms
// 3 40ohms
// 4 20ohms
// 5 30ohms
// RttWr:
// 0 Dynamic termination for writes disabled.
// 1 60ohms
// 2 120ohms
STATIC CONST PSCFG_RTT_ENTRY MLDramTermSoDwn3[] = {
  {_DIMM_NONE, DDR667, VOLT_ALL, DIMM_SR, NP, NP, DIMM_SR, R0, 0, 0},
  {_DIMM_NONE, DDR667, VOLT_ALL, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 0, 0},
  {_DIMM_NONE, DDR800 + DDR1066, VOLT_ALL, DIMM_SR, NP, NP, DIMM_SR, R0, 2, 0},
  {_DIMM_NONE, DDR800 + DDR1066, VOLT_ALL, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 2, 0},
  {_DIMM_NONE, DDR1333, VOLT_ALL, DIMM_SR, NP, NP, DIMM_SR, R0, 1, 0},
  {_DIMM_NONE, DDR1333, VOLT_ALL, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 1, 0},
  {_DIMM_NONE, DDR1600, V1_5 + V1_35, DIMM_SR, NP, NP, DIMM_SR, R0, 1, 0},
  {_DIMM_NONE, DDR1600, V1_5 + V1_35, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 1, 0},
  {_DIMM_NONE, DDR1866, V1_5, DIMM_SR, NP, NP, DIMM_SR, R0, 1, 0},
  {_DIMM_NONE, DDR1866, V1_5, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 1, 0},
};
CONST PSC_TBL_ENTRY MLDramTermTblEntSoDwn3 = {
   {PSCFG_RTT, SODWN_SODIMM_TYPE, _DIMM_NONE, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, MBL_DONT_CARE, HPM},
   sizeof (MLDramTermSoDwn3) / sizeof (PSCFG_RTT_ENTRY),
   (VOID *)&MLDramTermSoDwn3
};

// Dram Term and Dynamic Dram Term for Solder-down DRAM ONLY <Low Power Motherboard Design> configuration
// Format :
// DimmPerCh,   DDRrate,   VDDIO,   Dimm0,   Dimm1,   Dimm2,   Dimm,   Rank,   RttNom,    RttWr
//
// RttNom:
// 0 On die termination disabled
// 1 60ohms
// 2 120ohms
// 3 40ohms
// 4 20ohms
// 5 30ohms
// RttWr:
// 0 Dynamic termination for writes disabled.
// 1 60ohms
// 2 120ohms
STATIC CONST PSCFG_RTT_ENTRY MLDramTermSoDwn3LPM[] = {
  {_DIMM_NONE, DDR667 + DDR800 + DDR1066 + DDR1333, V1_5 + V1_35, DIMM_SR, NP, NP, DIMM_SR, R0, 0, 0},
  {_DIMM_NONE, DDR667 + DDR800 + DDR1066 + DDR1333, V1_5 + V1_35, DIMM_DR, NP, NP, DIMM_DR, R0 + R1, 0, 0},
};
CONST PSC_TBL_ENTRY MLDramTermTblEntSoDwn3LPM = {
   {PSCFG_RTT, SODWN_SODIMM_TYPE, _DIMM_NONE, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, MBL_DONT_CARE, LPM},
   sizeof (MLDramTermSoDwn3LPM) / sizeof (PSCFG_RTT_ENTRY),
   (VOID *)&MLDramTermSoDwn3LPM
};

// Max Freq. for SODIMM ONLY <6-layer Motherboard Design> configuration
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC CONST PSCFG_MAXFREQ_ENTRY ROMDATA MLMaxFreqSODIMM6L[] = {
  {_1DIMM, 1, 1, 0, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_1DIMM, 1, 0, 1, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 1, 1, 0, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 1, 0, 1, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 2, 2, 0, 0, DDR1600_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 2, 1, 1, 0, DDR1600_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_2DIMM, 2, 0, 2, 0, DDR1600_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
};
CONST PSC_TBL_ENTRY MLMaxFreqTblEntSO6L = {
   {PSCFG_MAXFREQ, SODIMM_TYPE, _1DIMM + _2DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, _6LAYERS, MBP_DONT_CARE},
   sizeof (MLMaxFreqSODIMM6L) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MLMaxFreqSODIMM6L
};

// Max Freq. for SODIMM ONLY <4-layer Motherboard Design> configuration
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC CONST PSCFG_MAXFREQ_ENTRY ROMDATA MLMaxFreqSODIMM4L[] = {
  {_1DIMM, 1, 1, 0, 0, DDR1600_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_1DIMM, 1, 0, 1, 0, DDR1600_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_2DIMM, 1, 1, 0, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_2DIMM, 1, 0, 1, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_2DIMM, 2, 2, 0, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_2DIMM, 2, 1, 1, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_2DIMM, 2, 0, 2, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
};
CONST PSC_TBL_ENTRY MLMaxFreqTblEntSO4L = {
   {PSCFG_MAXFREQ, SODIMM_TYPE, _1DIMM + _2DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, _4LAYERS, MBP_DONT_CARE},
   sizeof (MLMaxFreqSODIMM4L) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MLMaxFreqSODIMM4L
};

// Max Freq. for SODIMM plus Solder-down DRAM <6-layer Motherboard Design> configuration
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC CONST PSCFG_MAXFREQ_ENTRY ROMDATA MLMaxFreqSoDwnPlusSODIMM6L[] = {
  {_1DIMM, 1, 1, 0, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_1DIMM, 1, 0, 1, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_1DIMM, 2, 2, 0, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_1DIMM, 2, 1, 1, 0, DDR1066_FREQUENCY, DDR1066_FREQUENCY, DDR1066_FREQUENCY},
  {_1DIMM, 2, 0, 2, 0, DDR1066_FREQUENCY, DDR1066_FREQUENCY, DDR1066_FREQUENCY},
};
CONST PSC_TBL_ENTRY MLMaxFreqTblEntSoDwnPlusSODIMM6L = {
   {PSCFG_MAXFREQ, SODWN_SODIMM_TYPE, _1DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, _6LAYERS, MBP_DONT_CARE},
   sizeof (MLMaxFreqSoDwnPlusSODIMM6L) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MLMaxFreqSoDwnPlusSODIMM6L
};

// Max Freq. for SODIMM plus Solder-down DRAM <4-layer Motherboard Design> configuration
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC CONST PSCFG_MAXFREQ_ENTRY ROMDATA MLMaxFreqSoDwnPlusSODIMM4L[] = {
  {_1DIMM, 1, 1, 0, 0, DDR1066_FREQUENCY, DDR1066_FREQUENCY, DDR1066_FREQUENCY},
  {_1DIMM, 1, 0, 1, 0, DDR1066_FREQUENCY, DDR1066_FREQUENCY, DDR1066_FREQUENCY},
  {_1DIMM, 2, 2, 0, 0, DDR1066_FREQUENCY, DDR1066_FREQUENCY, DDR1066_FREQUENCY},
  {_1DIMM, 2, 1, 1, 0, DDR1066_FREQUENCY, DDR1066_FREQUENCY, DDR1066_FREQUENCY},
  {_1DIMM, 2, 0, 2, 0, DDR1066_FREQUENCY, DDR1066_FREQUENCY, DDR1066_FREQUENCY},
};
CONST PSC_TBL_ENTRY MLMaxFreqTblEntSoDwnPlusSODIMM4L = {
   {PSCFG_MAXFREQ, SODWN_SODIMM_TYPE, _1DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, _4LAYERS, MBP_DONT_CARE},
   sizeof (MLMaxFreqSoDwnPlusSODIMM4L) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MLMaxFreqSoDwnPlusSODIMM4L
};

// Max Freq. for Solder-down DRAM ONLY configuration
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC CONST PSCFG_MAXFREQ_ENTRY ROMDATA MLMaxFreqSoDwn[] = {
  {_DIMM_NONE, 1, 1, 0, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_DIMM_NONE, 1, 0, 1, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
};
CONST PSC_TBL_ENTRY MLMaxFreqTblEntSoDwn = {
   {PSCFG_MAXFREQ, SODWN_SODIMM_TYPE, _DIMM_NONE, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, MBL_DONT_CARE, HPM},
   sizeof (MLMaxFreqSoDwn) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MLMaxFreqSoDwn
};

// Max Freq. for Solder-down DRAM ONLY <Lower Power Motherboard Design> configuration
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC CONST PSCFG_MAXFREQ_ENTRY ROMDATA MLMaxFreqSoDwnLPM[] = {
  {_DIMM_NONE, 1, 1, 0, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_DIMM_NONE, 1, 0, 1, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
};
CONST PSC_TBL_ENTRY MLMaxFreqTblEntSoDwnLPM = {
   {PSCFG_MAXFREQ, SODWN_SODIMM_TYPE, _DIMM_NONE, {AMD_FAMILY_16_ML, AMD_F16_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY, MBL_DONT_CARE, LPM},
   sizeof (MLMaxFreqSoDwnLPM) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MLMaxFreqSoDwnLPM
};