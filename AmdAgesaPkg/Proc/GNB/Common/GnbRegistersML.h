/* $NoKeywords:$ */
/**
 * @file
 *
 * AGESA gnb file
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  GNB
 * @e \$Revision: 282985 $   @e \$Date: 2014-01-20 12:02:23 -0600 (Mon, 20 Jan 2014) $
 *
 */
/*
 ******************************************************************************
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
 ******************************************************************************
 */
#ifndef _GNBREGISTERSML_H_
#define _GNBREGISTERSML_H_
#define  TYPE_D0F0                  0x1
#define  TYPE_D0F0x64               0x2
#define  TYPE_D0F0x98               0x3
#define  TYPE_D0F0xBC               0x4
#define  TYPE_D0F0xE4               0x5
#define  TYPE_DxF0                  0x6
#define  TYPE_DxF0xE4               0x7
#define  TYPE_D0F2                  0x8
#define  TYPE_D0F2xF4               0x9
#define  TYPE_D0F2xFC               0xa
#define  TYPE_D18F1                 0xb
#define  TYPE_D18F2                 0xc
#define  TYPE_D18F3                 0xd
#define  TYPE_D18F4                 0xe
#define  TYPE_D18F5                 0xf
#define  TYPE_MSR                   0x10
#define  TYPE_D1F0                  0x11
#define  TYPE_GMM                   0x12
#define  TYPE_D18F2x9C_dct0         0x13
#define  TYPE_D18F2x9C_dct0_mp0     0x14
#define  TYPE_D18F2x9C_dct0_mp1     0x15
#define  TYPE_D18F2x9C_dct1         0x16
#define  TYPE_D18F2x9C_dct1_mp0     0x17
#define  TYPE_D18F2x9C_dct1_mp1     0x18
#define  TYPE_D18F2_dct0            0x19
#define  TYPE_D18F2_dct1            0x1a
#define  TYPE_D18F2_dct0_mp0        0x1b
#define  TYPE_D18F2_dct0_mp1        0x1c
#define  TYPE_D1F1                  0x1d
#define  TYPE_D18F2_dct1_mp0        0x1e
#define  TYPE_D18F2_dct1_mp1        0x1f
#define  TYPE_CGIND                 0x20
#define  TYPE_SMU_MSG               0x21
#define  TYPE_D0F0xD4               0x22
#define  TYPE_D0F0xFC               0x23
#define  TYPE_D18F0                 0x24
#define  TYPE_D8F0xEC               0x25


#ifndef WRAP_SPACE
  #define  WRAP_SPACE(w, x)   (0x01300000 | (w << 16) | (x))
#endif
#ifndef CORE_SPACE
  #define  CORE_SPACE(c, x)   (0x00010000 | (c << 24) | (x))
#endif
#ifndef PHY_SPACE
  #define  PHY_SPACE(w, p, x) (0x00200000 | ((p + 1) << 24) | (w << 16) | (x))
#endif
#ifndef PIF_SPACE
  #define  PIF_SPACE(w, p, x) (0x00100000 | ((p + 1) << 24) | (w << 16) | (x))
#endif

// SMU MSG
#define  INVALID_SMU_MSG                    0xFF
#define  SMU_MSG_TYPE                       TYPE_SMU_MSG
#define  SMC_MSG_TEST                       INVALID_SMU_MSG // 1
#define  SMC_MSG_RECONFIGURE                25
// #define  SMC_MSG_FORCE_LCLK_DPM_STATE       INVALID_SMU_MSG // 0x1C
// #define  SMC_MSG_UNFORCE_LCLK_DPM_STATE     INVALID_SMU_MSG // 0x1D
// #define  SMC_MSG_DSMU                       INVALID_SMU_MSG // 0x24
// #define  SMC_MSG_RESET                      INVALID_SMU_MSG // 0x29
#define  SMC_MSG_VDDNB_REQUEST              0x3A
#define  SMC_MSG_PWR_OFF_IOMMU              INVALID_SMU_MSG //0x48
#define  SMC_MSG_PWR_ON_IOMMU               INVALID_SMU_MSG //0x49
// #define  SMC_MSG_LCLKDPM_SETENABLEMASK      INVALID_SMU_MSG // 0x4C
#define  SMC_MSG_READ_SCS                   0x50
#define  SMC_MSG_POWERDOWNGPU               INVALID_SMU_MSG //0x55
#define  SMC_MSG_POWERUPGPU                 INVALID_SMU_MSG //0x56
#define  SMC_MSG_PCIE_NATIVE_GEN1_PLLSWITCH 0x57
#define  SMC_MSG_BATTERY_STATUS             INVALID_SMU_MSG //0x5D
#define  SMC_MSG_PWR_STATUS                 INVALID_SMU_MSG //0x5E
#define  SMC_MSG_ENABLE_ALL_SMU_FEATURES    0x5f
#define  SMC_MSG_DISABLE_ALL_SMU_FEATURES   0x60
// #define  SMC_MSG_CONNSTDBY_GETSAVEDREGSPTR  INVALID_SMU_MSG // 0x65
#define  SMC_MSG_DOCK_STATUS                0x66
//#define  SMC_MSG_CS_WAIT_FOR_HINT           0x67
#define  SMC_MSG_MONITOR_PORT80_ENABLE      INVALID_SMU_MSG
#define  SMC_MSG_MONITOR_PORT80_DISABLE     INVALID_SMU_MSG
#define  SMC_MSG_PSPCLKDPM_ENABLE           0x73
#define  SMC_MSG_PSPCLKDPM_DISABLE          0x74


// SMU feature mask
#define  BAPM_MASK                          0x0001
#define  POWER_ESTIMATOR_MASK               0x0002
#define  THERMAL_CONTROLLER_MASK            0x0004
#define  TDC_LIMITING_MASK                  0x0008
#define  PKG_PWR_LIMITING_MASK              0x0010
#define  LCLK_DPM_MASK                      0x0020
#define  VOLTAGE_CONTROLLER_MASK            0x0040
#define  GLOBAL_EAPM_MASK                   0x0080
#define  VPC_ACCUMULATOR_MASK               0x0100
#define  IBS_MASK                           0x0200
#define  PSTATE_ARBITER_MASK                0x0400
#define  NB_DPM_MASK                        0x0800
#define  LPMX_MASK                          0x1000
#define  WHISPER_MODE_MASK                  0x2000
#define  GFX_CU_PG_MASK                     0x4000
#define  CONN_STDBY_MASK                    0x8000
#define  MULTI_VID_MASK                     0x10000
#define  CHTC_MASK                          0x20000
#define  ALLCLK_MON_MASK                    0x40000
#define  SCLK_DPM_MASK                      0x80000
#define  PRE_FETCH_DATA_MASK                0x100000
#define  LCLK_DEEP_SLEEP_MASK               0x200000
#define  STAPM_MASK                         0x400000
#define  PTC_MASK                           0x800000
#define  BBB_MASK                           0x1000000
#define  DCTDP_MASK                         0x2000000
#define  APU_PWR_CTRL_MASK                  0x4000000
#define  LOCAL_EAPM_MASK                    0x8000000
#define  CORE_RRT_MASK                      0x10000000
#define  GFX_RRT_MASK                       0x20000000
#define  STAPM_TSKIN_CALC_MASK              0x40000000

#define  L1_SEL_PPD                         0
#define  L1_SEL_SBG                         1
#define  L1_SEL_BIF                         2
#define  L1_SEL_INTGEN                      3
#define  L1_SEL_SPG                         4

// **** D18F0x110 Register Definition ****
// Address
#define D18F0x110_ADDRESS                                       0x110

// Type
#define D18F0x110_TYPE                                          TYPE_D18F0

// **** D18F1x44 Register Definition ****
// Address
#define D18F1x44_ADDRESS                                        0x44
// Type
#define D18F1x44_TYPE                                           TYPE_D18F1

// Field Data
#define D18F1x44_DstNode_OFFSET                                 0
#define D18F1x44_DstNode_WIDTH                                  3
#define D18F1x44_DstNode_MASK                                   0x7
#define D18F1x44_Reserved_7_3_OFFSET                            3
#define D18F1x44_Reserved_7_3_WIDTH                             5
#define D18F1x44_Reserved_7_3_MASK                              0xF8
#define D18F1x44_Reserved_10_8_OFFSET                           8
#define D18F1x44_Reserved_10_8_WIDTH                            3
#define D18F1x44_Reserved_10_8_MASK                             0x700
#define D18F1x44_Reserved_15_11_OFFSET                          11
#define D18F1x44_Reserved_15_11_WIDTH                           5
#define D18F1x44_Reserved_15_11_MASK                            0xF800
#define D18F1x44_DramLimit_39_24_OFFSET                         16
#define D18F1x44_DramLimit_39_24_WIDTH                          16
#define D18F1x44_DramLimit_39_24_MASK                           0xFFFF0000

/// D18F1x44
typedef union {
  struct {                                                            ///<
    UINT32                                                 DstNode:3; ///<
    UINT32                                            Reserved_7_3:5; ///<
    UINT32                                           Reserved_10_8:3; ///<
    UINT32                                          Reserved_15_11:5; ///<
    UINT32                                         DramLimit_39_24:16; ///<

  } Field;

  UINT32 Value;
} D18F1x44_STRUCT;

// **** D18F1xB8 Register Definition ****
// Address
#define D18F1xB8_ADDRESS                                        0xb8

// Type
#define D18F1xB8_TYPE                                           TYPE_D18F1
// Field Data
#define D18F1xB8_RE_OFFSET                                      0
#define D18F1xB8_RE_WIDTH                                       1
#define D18F1xB8_RE_MASK                                        0x1
#define D18F1xB8_WE_OFFSET                                      1
#define D18F1xB8_WE_WIDTH                                       1
#define D18F1xB8_WE_MASK                                        0x2
#define D18F1xB8_CpuDis_OFFSET                                  2
#define D18F1xB8_CpuDis_WIDTH                                   1
#define D18F1xB8_CpuDis_MASK                                    0x4
#define D18F1xB8_Lock_OFFSET                                    3
#define D18F1xB8_Lock_WIDTH                                     1
#define D18F1xB8_Lock_MASK                                      0x8
#define D18F1xB8_Reserved_7_4_OFFSET                            4
#define D18F1xB8_Reserved_7_4_WIDTH                             4
#define D18F1xB8_Reserved_7_4_MASK                              0xf0
#define D18F1xB8_MMIOBase_39_16__OFFSET                         8
#define D18F1xB8_MMIOBase_39_16__WIDTH                          24
#define D18F1xB8_MMIOBase_39_16__MASK                           0xffffff00

/// D18F1xB8
typedef union {
  struct {                                                              ///<
    UINT32                                                       RE:1 ; ///<
    UINT32                                                       WE:1 ; ///<
    UINT32                                                   CpuDis:1 ; ///<
    UINT32                                                     Lock:1 ; ///<
    UINT32                                             Reserved_7_4:4 ; ///<
    UINT32                                          MMIOBase_39_16_:24; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F1xB8_STRUCT;

// **** D18F1xBC Register Definition ****
// Address
#define D18F1xBC_ADDRESS                                        0xbc

// Type
#define D18F1xBC_TYPE                                           TYPE_D18F1
// Field Data
#define D18F1xBC_DstNode_OFFSET                                 0
#define D18F1xBC_DstNode_WIDTH                                  3
#define D18F1xBC_DstNode_MASK                                   0x7
#define D18F1xBC_Reserved_3_3_OFFSET                            3
#define D18F1xBC_Reserved_3_3_WIDTH                             1
#define D18F1xBC_Reserved_3_3_MASK                              0x8
#define D18F1xBC_DstLink_OFFSET                                 4
#define D18F1xBC_DstLink_WIDTH                                  2
#define D18F1xBC_DstLink_MASK                                   0x30
#define D18F1xBC_DstSubLink_OFFSET                              6
#define D18F1xBC_DstSubLink_WIDTH                               1
#define D18F1xBC_DstSubLink_MASK                                0x40
#define D18F1xBC_NP_OFFSET                                      7
#define D18F1xBC_NP_WIDTH                                       1
#define D18F1xBC_NP_MASK                                        0x80
#define D18F1xBC_MMIOLimit_39_16__OFFSET                        8
#define D18F1xBC_MMIOLimit_39_16__WIDTH                         24
#define D18F1xBC_MMIOLimit_39_16__MASK                          0xffffff00

/// D18F1xBC
typedef union {
  struct {                                                              ///<
    UINT32                                                  DstNode:3 ; ///<
    UINT32                                             Reserved_3_3:1 ; ///<
    UINT32                                                  DstLink:2 ; ///<
    UINT32                                               DstSubLink:1 ; ///<
    UINT32                                                  FieldNP:1 ; ///<
    UINT32                                         MMIOLimit_39_16_:24; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F1xBC_STRUCT;

// **** D18F1x10C Register Definition ****
// Address
#define D18F1x10C_ADDRESS                                       0x10c

// Type
#define D18F1x10C_TYPE                                          TYPE_D18F1
// Field Data
#define D18F1x10C_DctCfgSel_OFFSET                              0
#define D18F1x10C_DctCfgSel_WIDTH                               1
#define D18F1x10C_DctCfgSel_MASK                                0x1
#define D18F1x10C_Reserved_2_1_OFFSET                           1
#define D18F1x10C_Reserved_2_1_WIDTH                            2
#define D18F1x10C_Reserved_2_1_MASK                             0x6
#define D18F1x10C_MemPsSel_OFFSET                               3
#define D18F1x10C_MemPsSel_WIDTH                                1
#define D18F1x10C_MemPsSel_MASK                                 0x8
#define D18F1x10C_NbPsSel_OFFSET                                4
#define D18F1x10C_NbPsSel_WIDTH                                 2
#define D18F1x10C_NbPsSel_MASK                                  0x30
#define D18F1x10C_Unused_31_6_OFFSET                            6
#define D18F1x10C_Unused_31_6_WIDTH                             26
#define D18F1x10C_Unused_31_6_MASK                              0xffffffc0

/// D18F1x10C
typedef union {
  struct {                                                              ///<
    UINT32                                                DctCfgSel:1 ; ///<
    UINT32                                             Reserved_2_1:2 ; ///<
    UINT32                                                 MemPsSel:1 ; ///<
    UINT32                                                  NbPsSel:2 ; ///<
    UINT32                                              Unused_31_6:26; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F1x10C_STRUCT;

// **** D18F2x90_dct0 Register Definition ****
// Address
#define D18F2x90_dct0_ADDRESS                                   0x90
// Type
#define D18F2x90_dct0_TYPE                                      TYPE_D18F2

// Field Data
#define D18F2x90_dct0_InitDram_OFFSET                           0
#define D18F2x90_dct0_InitDram_WIDTH                            1
#define D18F2x90_dct0_InitDram_MASK                             0x1
#define D18F2x90_dct0_ExitSelfRef_OFFSET                        1
#define D18F2x90_dct0_ExitSelfRef_WIDTH                         1
#define D18F2x90_dct0_ExitSelfRef_MASK                          0x2
#define D18F2x90_dct0_Reserved_7_2_OFFSET                       2
#define D18F2x90_dct0_Reserved_7_2_WIDTH                        6
#define D18F2x90_dct0_Reserved_7_2_MASK                         0xfc
#define D18F2x90_dct0_Reserved_8_8_OFFSET                       8
#define D18F2x90_dct0_Reserved_8_8_WIDTH                        1
#define D18F2x90_dct0_Reserved_8_8_MASK                         0x100
#define D18F2x90_dct0_Reserved_10_9_OFFSET                      9
#define D18F2x90_dct0_Reserved_10_9_WIDTH                       2
#define D18F2x90_dct0_Reserved_10_9_MASK                        0x600
#define D18F2x90_dct0_Reserved_11_11_OFFSET                     11
#define D18F2x90_dct0_Reserved_11_11_WIDTH                      1
#define D18F2x90_dct0_Reserved_11_11_MASK                       0x800
#define D18F2x90_dct0_Reserved_15_12_OFFSET                     12
#define D18F2x90_dct0_Reserved_15_12_WIDTH                      4
#define D18F2x90_dct0_Reserved_15_12_MASK                       0xf000
#define D18F2x90_dct0_UnbuffDimm_OFFSET                         16
#define D18F2x90_dct0_UnbuffDimm_WIDTH                          1
#define D18F2x90_dct0_UnbuffDimm_MASK                           0x10000
#define D18F2x90_dct0_UnbuffDimm_VALUE                          0x1
#define D18F2x90_dct0_EnterSelfRef_OFFSET                       17
#define D18F2x90_dct0_EnterSelfRef_WIDTH                        1
#define D18F2x90_dct0_EnterSelfRef_MASK                         0x20000
#define D18F2x90_dct0_PendRefPayback_OFFSET                     18
#define D18F2x90_dct0_PendRefPayback_WIDTH                      1
#define D18F2x90_dct0_PendRefPayback_MASK                       0x40000
#define D18F2x90_dct0_PendRefPayback_VALUE                      0x0
#define D18F2x90_dct0_DimmEccEn_OFFSET                          19
#define D18F2x90_dct0_DimmEccEn_WIDTH                           1
#define D18F2x90_dct0_DimmEccEn_MASK                            0x80000
#define D18F2x90_dct0_DynPageCloseEn_OFFSET                     20
#define D18F2x90_dct0_DynPageCloseEn_WIDTH                      1
#define D18F2x90_dct0_DynPageCloseEn_MASK                       0x100000
#define D18F2x90_dct0_IdleCycLowLimit_OFFSET                    21
#define D18F2x90_dct0_IdleCycLowLimit_WIDTH                     2
#define D18F2x90_dct0_IdleCycLowLimit_MASK                      0x600000
#define D18F2x90_dct0_ForceAutoPchg_OFFSET                      23
#define D18F2x90_dct0_ForceAutoPchg_WIDTH                       1
#define D18F2x90_dct0_ForceAutoPchg_MASK                        0x800000
#define D18F2x90_dct0_StagRefEn_OFFSET                          24
#define D18F2x90_dct0_StagRefEn_WIDTH                           1
#define D18F2x90_dct0_StagRefEn_MASK                            0x1000000
#define D18F2x90_dct0_StagRefEn_VALUE                           0x1
#define D18F2x90_dct0_PendRefPaybackS3En_OFFSET                 25
#define D18F2x90_dct0_PendRefPaybackS3En_WIDTH                  1
#define D18F2x90_dct0_PendRefPaybackS3En_MASK                   0x2000000
#define D18F2x90_dct0_PendRefPaybackS3En_VALUE                  0x1
#define D18F2x90_dct0_Reserved_26_26_OFFSET                     26
#define D18F2x90_dct0_Reserved_26_26_WIDTH                      1
#define D18F2x90_dct0_Reserved_26_26_MASK                       0x4000000
#define D18F2x90_dct0_DisDllShutdownSR_OFFSET                   27
#define D18F2x90_dct0_DisDllShutdownSR_WIDTH                    1
#define D18F2x90_dct0_DisDllShutdownSR_MASK                     0x8000000
#define D18F2x90_dct0_IdleCycLimit_OFFSET                       28
#define D18F2x90_dct0_IdleCycLimit_WIDTH                        4
#define D18F2x90_dct0_IdleCycLimit_MASK                         0xf0000000
#define D18F2x90_dct0_IdleCycLimit_VALUE                        0x8

/// D18F2x90_dct0
typedef union {
  struct {                                                              ///<
    UINT32                                                 InitDram:1 ; ///<
    UINT32                                              ExitSelfRef:1 ; ///<
    UINT32                                             Reserved_7_2:6 ; ///<
    UINT32                                             Reserved_8_8:1 ; ///<
    UINT32                                            Reserved_10_9:2 ; ///<
    UINT32                                           Reserved_11_11:1 ; ///<
    UINT32                                           Reserved_15_12:4 ; ///<
    UINT32                                               UnbuffDimm:1 ; ///<
    UINT32                                             EnterSelfRef:1 ; ///<
    UINT32                                           PendRefPayback:1 ; ///<
    UINT32                                                DimmEccEn:1 ; ///<
    UINT32                                           DynPageCloseEn:1 ; ///<
    UINT32                                          IdleCycLowLimit:2 ; ///<
    UINT32                                            ForceAutoPchg:1 ; ///<
    UINT32                                                StagRefEn:1 ; ///<
    UINT32                                       PendRefPaybackS3En:1 ; ///<
    UINT32                                           Reserved_26_26:1 ; ///<
    UINT32                                         DisDllShutdownSR:1 ; ///<
    UINT32                                             IdleCycLimit:4 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F2x90_dct0_STRUCT;


// **** D18F2x94_dct0 Register Definition ****
// Address
#define D18F2x94_dct0_ADDRESS                                   0x94
// Type
#define D18F2x94_dct0_TYPE                                      TYPE_D18F2_dct0

// Field Data
#define D18F2x94_dct0_MemClkFreq_OFFSET                         0
#define D18F2x94_dct0_MemClkFreq_WIDTH                          5
#define D18F2x94_dct0_MemClkFreq_MASK                           0x1f
#define D18F2x94_dct0_Reserved_6_5_OFFSET                       5
#define D18F2x94_dct0_Reserved_6_5_WIDTH                        2
#define D18F2x94_dct0_Reserved_6_5_MASK                         0x60
#define D18F2x94_dct0_MemClkFreqVal_OFFSET                      7
#define D18F2x94_dct0_MemClkFreqVal_WIDTH                       1
#define D18F2x94_dct0_MemClkFreqVal_MASK                        0x80
#define D18F2x94_dct0_Reserved_9_8_OFFSET                       8
#define D18F2x94_dct0_Reserved_9_8_WIDTH                        2
#define D18F2x94_dct0_Reserved_9_8_MASK                         0x300
#define D18F2x94_dct0_ZqcsInterval_OFFSET                       10
#define D18F2x94_dct0_ZqcsInterval_WIDTH                        2
#define D18F2x94_dct0_ZqcsInterval_MASK                         0xc00
#define D18F2x94_dct0_RDqsEn_OFFSET                             12
#define D18F2x94_dct0_RDqsEn_WIDTH                              1
#define D18F2x94_dct0_RDqsEn_MASK                               0x1000
#define D18F2x94_dct0_Reserved_13_13_OFFSET                     13
#define D18F2x94_dct0_Reserved_13_13_WIDTH                      1
#define D18F2x94_dct0_Reserved_13_13_MASK                       0x2000
#define D18F2x94_dct0_DisDramInterface_OFFSET                   14
#define D18F2x94_dct0_DisDramInterface_WIDTH                    1
#define D18F2x94_dct0_DisDramInterface_MASK                     0x4000
#define D18F2x94_dct0_PowerDownEn_OFFSET                        15
#define D18F2x94_dct0_PowerDownEn_WIDTH                         1
#define D18F2x94_dct0_PowerDownEn_MASK                          0x8000
#define D18F2x94_dct0_PowerDownMode_OFFSET                      16
#define D18F2x94_dct0_PowerDownMode_WIDTH                       1
#define D18F2x94_dct0_PowerDownMode_MASK                        0x10000
#define D18F2x94_dct0_PowerDownMode_VALUE                       0x1
#define D18F2x94_dct0_Reserved_17_17_OFFSET                     17
#define D18F2x94_dct0_Reserved_17_17_WIDTH                      1
#define D18F2x94_dct0_Reserved_17_17_MASK                       0x20000
#define D18F2x94_dct0_Reserved_18_18_OFFSET                     18
#define D18F2x94_dct0_Reserved_18_18_WIDTH                      1
#define D18F2x94_dct0_Reserved_18_18_MASK                       0x40000
#define D18F2x94_dct0_Reserved_19_19_OFFSET                     19
#define D18F2x94_dct0_Reserved_19_19_WIDTH                      1
#define D18F2x94_dct0_Reserved_19_19_MASK                       0x80000
#define D18F2x94_dct0_SlowAccessMode_OFFSET                     20
#define D18F2x94_dct0_SlowAccessMode_WIDTH                      1
#define D18F2x94_dct0_SlowAccessMode_MASK                       0x100000
#define D18F2x94_dct0_FreqChgInProg_OFFSET                      21
#define D18F2x94_dct0_FreqChgInProg_WIDTH                       1
#define D18F2x94_dct0_FreqChgInProg_MASK                        0x200000
#define D18F2x94_dct0_BankSwizzleMode_OFFSET                    22
#define D18F2x94_dct0_BankSwizzleMode_WIDTH                     1
#define D18F2x94_dct0_BankSwizzleMode_MASK                      0x400000
#define D18F2x94_dct0_ProcOdtDis_OFFSET                         23
#define D18F2x94_dct0_ProcOdtDis_WIDTH                          1
#define D18F2x94_dct0_ProcOdtDis_MASK                           0x800000
#define D18F2x94_dct0_DcqBypassMax_OFFSET                       24
#define D18F2x94_dct0_DcqBypassMax_WIDTH                        5
#define D18F2x94_dct0_DcqBypassMax_MASK                         0x1f000000
#define D18F2x94_dct0_Reserved_30_29_OFFSET                     29
#define D18F2x94_dct0_Reserved_30_29_WIDTH                      2
#define D18F2x94_dct0_Reserved_30_29_MASK                       0x60000000
#define D18F2x94_dct0_DphyMemPsSelEn_OFFSET                     31
#define D18F2x94_dct0_DphyMemPsSelEn_WIDTH                      1
#define D18F2x94_dct0_DphyMemPsSelEn_MASK                       0x80000000
#define D18F2x94_dct0_DphyMemPsSelEn_VALUE                      0x1

/// D18F2x94_dct0
typedef union {
  struct {                                                              ///<
    UINT32                                               MemClkFreq:5 ; ///<
    UINT32                                             Reserved_6_5:2 ; ///<
    UINT32                                            MemClkFreqVal:1 ; ///<
    UINT32                                             Reserved_9_8:2 ; ///<
    UINT32                                             ZqcsInterval:2 ; ///<
    UINT32                                                   RDqsEn:1 ; ///<
    UINT32                                           Reserved_13_13:1 ; ///<
    UINT32                                         DisDramInterface:1 ; ///<
    UINT32                                              PowerDownEn:1 ; ///<
    UINT32                                            PowerDownMode:1 ; ///<
    UINT32                                           Reserved_17_17:1 ; ///<
    UINT32                                           Reserved_18_18:1 ; ///<
    UINT32                                           Reserved_19_19:1 ; ///<
    UINT32                                           SlowAccessMode:1 ; ///<
    UINT32                                            FreqChgInProg:1 ; ///<
    UINT32                                          BankSwizzleMode:1 ; ///<
    UINT32                                               ProcOdtDis:1 ; ///<
    UINT32                                             DcqBypassMax:5 ; ///<
    UINT32                                           Reserved_30_29:2 ; ///<
    UINT32                                           DphyMemPsSelEn:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F2x94_dct0_STRUCT;

// **** D18F2x98_dct0 Register Definition ****
// Address
#define D18F2x98_dct0_ADDRESS                                   0x98

// Type
#define D18F2x98_dct0_TYPE                                      TYPE_D18F2_dct0
// Field Data
#define D18F2x98_dct0_DctOffset_OFFSET                          0
#define D18F2x98_dct0_DctOffset_WIDTH                           30
#define D18F2x98_dct0_DctOffset_MASK                            0x3fffffff
#define D18F2x98_dct0_DctAccessWrite_OFFSET                     30
#define D18F2x98_dct0_DctAccessWrite_WIDTH                      1
#define D18F2x98_dct0_DctAccessWrite_MASK                       0x40000000
#define D18F2x98_dct0_Reserved_31_31_OFFSET                     31
#define D18F2x98_dct0_Reserved_31_31_WIDTH                      1
#define D18F2x98_dct0_Reserved_31_31_MASK                       0x80000000

/// D18F2x98_dct0
typedef union {
  struct {                                                              ///<
    UINT32                                                DctOffset:30; ///<
    UINT32                                           DctAccessWrite:1 ; ///<
    UINT32                                           Reserved_31_31:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F2x98_dct0_STRUCT;

#define D18F2xA8_dct0_MemPhyPllPdMode_OFFSET                    16
#define D18F2xA8_dct0_MemPhyPllPdMode_WIDTH                     2

// **** D18F2x2E0_dct0 Register Definition ****
// Address
#define D18F2x2E0_dct0_ADDRESS                                  0x2e0

// Type
#define D18F2x2E0_dct0_TYPE                                     TYPE_D18F2
// Field Data

#define D18F2x2E0_dct0_M1MemClkFreq_OFFSET                      24
#define D18F2x2E0_dct0_M1MemClkFreq_WIDTH                       5

/// D18F2x2E0_dct0
typedef union {
  struct {                                                              ///<
    UINT32                                               Reserved_1:1 ; ///<
    UINT32                                            Reserved_19_1:19; ///<
    UINT32                                           Reserved_22_20:3 ; ///<
    UINT32                                           Reserved_23_23:1 ; ///<
    UINT32                                             M1MemClkFreq:5 ; ///<
    UINT32                                           Reserved_29_29:1 ; ///<
    UINT32                                           Reserved_30_30:1 ; ///<
    UINT32                                           Reserved_31_31:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F2x2E0_dct0_STRUCT;

// **** D18F3x44 Register Definition ****
// Address
#define D18F3x44_ADDRESS                                        0x44

// Type
#define D18F3x44_TYPE                                           TYPE_D18F3
// Field Data
#define D18F3x44_Reserved_0_0_OFFSET                            0
#define D18F3x44_Reserved_0_0_WIDTH                             1
#define D18F3x44_Reserved_0_0_MASK                              0x1
#define D18F3x44_CpuRdDatErrEn_OFFSET                           1
#define D18F3x44_CpuRdDatErrEn_WIDTH                            1
#define D18F3x44_CpuRdDatErrEn_MASK                             0x2
#define D18F3x44_SyncFloodOnDramUcEcc_OFFSET                    2
#define D18F3x44_SyncFloodOnDramUcEcc_WIDTH                     1
#define D18F3x44_SyncFloodOnDramUcEcc_MASK                      0x4
#define D18F3x44_SyncFloodOnDramUcEcc_VALUE                     0x1
#define D18F3x44_SyncPktGenDis_OFFSET                           3
#define D18F3x44_SyncPktGenDis_WIDTH                            1
#define D18F3x44_SyncPktGenDis_MASK                             0x8
#define D18F3x44_SyncPktGenDis_VALUE                            0x0
#define D18F3x44_SyncPktPropDis_OFFSET                          4
#define D18F3x44_SyncPktPropDis_WIDTH                           1
#define D18F3x44_SyncPktPropDis_MASK                            0x10
#define D18F3x44_SyncPktPropDis_VALUE                           0x0
#define D18F3x44_IoMstAbortDis_OFFSET                           5
#define D18F3x44_IoMstAbortDis_WIDTH                            1
#define D18F3x44_IoMstAbortDis_MASK                             0x20
#define D18F3x44_CpuErrDis_OFFSET                               6
#define D18F3x44_CpuErrDis_WIDTH                                1
#define D18F3x44_CpuErrDis_MASK                                 0x40
#define D18F3x44_CpuErrDis_VALUE                                0x1
#define D18F3x44_IoErrDis_OFFSET                                7
#define D18F3x44_IoErrDis_WIDTH                                 1
#define D18F3x44_IoErrDis_MASK                                  0x80
#define D18F3x44_WDTDis_OFFSET                                  8
#define D18F3x44_WDTDis_WIDTH                                   1
#define D18F3x44_WDTDis_MASK                                    0x100
#define D18F3x44_WDTCntSel_2_0__OFFSET                          9
#define D18F3x44_WDTCntSel_2_0__WIDTH                           3
#define D18F3x44_WDTCntSel_2_0__MASK                            0xe00
#define D18F3x44_WDTBaseSel_OFFSET                              12
#define D18F3x44_WDTBaseSel_WIDTH                               2
#define D18F3x44_WDTBaseSel_MASK                                0x3000
#define D18F3x44_GenLinkSel_OFFSET                              14
#define D18F3x44_GenLinkSel_WIDTH                               2
#define D18F3x44_GenLinkSel_MASK                                0xc000
#define D18F3x44_GenCrcErrByte0_OFFSET                          16
#define D18F3x44_GenCrcErrByte0_WIDTH                           1
#define D18F3x44_GenCrcErrByte0_MASK                            0x10000
#define D18F3x44_GenCrcErrByte1_OFFSET                          17
#define D18F3x44_GenCrcErrByte1_WIDTH                           1
#define D18F3x44_GenCrcErrByte1_MASK                            0x20000
#define D18F3x44_GenSubLinkSel_OFFSET                           18
#define D18F3x44_GenSubLinkSel_WIDTH                            2
#define D18F3x44_GenSubLinkSel_MASK                             0xc0000
#define D18F3x44_SyncFloodOnWDT_OFFSET                          20
#define D18F3x44_SyncFloodOnWDT_WIDTH                           1
#define D18F3x44_SyncFloodOnWDT_MASK                            0x100000
#define D18F3x44_SyncFloodOnWDT_VALUE                           0x1
#define D18F3x44_SyncFloodOnAnyUcErr_OFFSET                     21
#define D18F3x44_SyncFloodOnAnyUcErr_WIDTH                      1
#define D18F3x44_SyncFloodOnAnyUcErr_MASK                       0x200000
#define D18F3x44_SyncFloodOnAnyUcErr_VALUE                      0x1
#define D18F3x44_DramEccEn_OFFSET                               22
#define D18F3x44_DramEccEn_WIDTH                                1
#define D18F3x44_DramEccEn_MASK                                 0x400000
#define D18F3x44_ChipKillEccCap_OFFSET                          23
#define D18F3x44_ChipKillEccCap_WIDTH                           1
#define D18F3x44_ChipKillEccCap_MASK                            0x800000
#define D18F3x44_IoRdDatErrEn_OFFSET                            24
#define D18F3x44_IoRdDatErrEn_WIDTH                             1
#define D18F3x44_IoRdDatErrEn_MASK                              0x1000000
#define D18F3x44_DisPciCfgCpuErrRsp_OFFSET                      25
#define D18F3x44_DisPciCfgCpuErrRsp_WIDTH                       1
#define D18F3x44_DisPciCfgCpuErrRsp_MASK                        0x2000000
#define D18F3x44_FlagMcaCorrErr_OFFSET                          26
#define D18F3x44_FlagMcaCorrErr_WIDTH                           1
#define D18F3x44_FlagMcaCorrErr_MASK                            0x4000000
#define D18F3x44_NbMcaToMstCpuEn_OFFSET                         27
#define D18F3x44_NbMcaToMstCpuEn_WIDTH                          1
#define D18F3x44_NbMcaToMstCpuEn_MASK                           0x8000000
#define D18F3x44_NbMcaToMstCpuEn_VALUE                          0x1
#define D18F3x44_DisTgtAbortCpuErrRsp_OFFSET                    28
#define D18F3x44_DisTgtAbortCpuErrRsp_WIDTH                     1
#define D18F3x44_DisTgtAbortCpuErrRsp_MASK                      0x10000000
#define D18F3x44_DisMstAbortCpuErrRsp_OFFSET                    29
#define D18F3x44_DisMstAbortCpuErrRsp_WIDTH                     1
#define D18F3x44_DisMstAbortCpuErrRsp_MASK                      0x20000000
#define D18F3x44_SyncFloodOnDramAdrParErr_OFFSET                30
#define D18F3x44_SyncFloodOnDramAdrParErr_WIDTH                 1
#define D18F3x44_SyncFloodOnDramAdrParErr_MASK                  0x40000000
#define D18F3x44_SyncFloodOnDramAdrParErr_VALUE                 0x1
#define D18F3x44_NbMcaLogEn_OFFSET                              31
#define D18F3x44_NbMcaLogEn_WIDTH                               1
#define D18F3x44_NbMcaLogEn_MASK                                0x80000000

/// D18F3x44
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_0_0:1 ; ///<
    UINT32                                            CpuRdDatErrEn:1 ; ///<
    UINT32                                     SyncFloodOnDramUcEcc:1 ; ///<
    UINT32                                            SyncPktGenDis:1 ; ///<
    UINT32                                           SyncPktPropDis:1 ; ///<
    UINT32                                            IoMstAbortDis:1 ; ///<
    UINT32                                                CpuErrDis:1 ; ///<
    UINT32                                                 IoErrDis:1 ; ///<
    UINT32                                                   WDTDis:1 ; ///<
    UINT32                                           WDTCntSel_2_0_:3 ; ///<
    UINT32                                               WDTBaseSel:2 ; ///<
    UINT32                                               GenLinkSel:2 ; ///<
    UINT32                                           GenCrcErrByte0:1 ; ///<
    UINT32                                           GenCrcErrByte1:1 ; ///<
    UINT32                                            GenSubLinkSel:2 ; ///<
    UINT32                                           SyncFloodOnWDT:1 ; ///<
    UINT32                                      SyncFloodOnAnyUcErr:1 ; ///<
    UINT32                                                DramEccEn:1 ; ///<
    UINT32                                           ChipKillEccCap:1 ; ///<
    UINT32                                             IoRdDatErrEn:1 ; ///<
    UINT32                                       DisPciCfgCpuErrRsp:1 ; ///<
    UINT32                                           FlagMcaCorrErr:1 ; ///<
    UINT32                                          NbMcaToMstCpuEn:1 ; ///<
    UINT32                                     DisTgtAbortCpuErrRsp:1 ; ///<
    UINT32                                     DisMstAbortCpuErrRsp:1 ; ///<
    UINT32                                 SyncFloodOnDramAdrParErr:1 ; ///<
    UINT32                                               NbMcaLogEn:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F3x44_STRUCT;

// **** D18F4x110 Register Definition ****
// Address
#define D18F4x110_ADDRESS                                       0x110

// Type
#define D18F4x110_TYPE                                          TYPE_D18F4
// Field Data
#define D18F4x110_CSampleTimer_OFFSET                           0
#define D18F4x110_CSampleTimer_WIDTH                            12
#define D18F4x110_CSampleTimer_MASK                             0xfff
#define D18F4x110_CSampleTimer_VALUE                            0x2
#define D18F4x110_FastCSampleTimer_OFFSET                       12
#define D18F4x110_FastCSampleTimer_WIDTH                        1
#define D18F4x110_FastCSampleTimer_MASK                         0x1000
#define D18F4x110_MinResTmr_OFFSET                              13
#define D18F4x110_MinResTmr_WIDTH                               8
#define D18F4x110_MinResTmr_MASK                                0x1fe000
#define D18F4x110_Reserved_31_21_OFFSET                         21
#define D18F4x110_Reserved_31_21_WIDTH                          11
#define D18F4x110_Reserved_31_21_MASK                           0xffe00000

/// D18F4x110
typedef union {
  struct {                                                              ///<
    UINT32                                             CSampleTimer:12; ///<
    UINT32                                         FastCSampleTimer:1 ; ///<
    UINT32                                                MinResTmr:8 ; ///<
    UINT32                                           Reserved_31_21:11; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F4x110_STRUCT;

// **** D18F5xE0 Register Definition ****
// Address
#define D18F5xE0_ADDRESS                                        0xe0

// Type
#define D18F5xE0_TYPE                                           TYPE_D18F5
// Field Data
#define D18F5xE0_RunAvgRange_OFFSET                             0
#define D18F5xE0_RunAvgRange_WIDTH                              4
#define D18F5xE0_RunAvgRange_MASK                               0xf
#define D18F5xE0_RunAvgRange_VALUE                              0x2
#define D18F5xE0_Reserved_31_4_OFFSET                           4
#define D18F5xE0_Reserved_31_4_WIDTH                            28
#define D18F5xE0_Reserved_31_4_MASK                             0xfffffff0

/// D18F5xE0
typedef union {
  struct {                                                              ///<
    UINT32                                              RunAvgRange:4 ; ///<
    UINT32                                           Reserved_31_4:28 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F5xE0_STRUCT;

// **** D18F5x160 Register Definition ****
// Address
#define D18F5x160_ADDRESS                                       0x160

// Type
#define D18F5x160_TYPE                                          TYPE_D18F5
#define D18F5x160_NbPstateEn_OFFSET                             0
#define D18F5x160_NbPstateEn_WIDTH                              1

#define D18F5x160_MemPstate_OFFSET                              18
#define D18F5x160_MemPstate_WIDTH                               1
#define D18F5x160_NbFid_OFFSET                                  1
#define D18F5x160_NbFid_WIDTH                                   6
#define D18F5x160_NbDid_OFFSET                                  7
#define D18F5x160_NbDid_WIDTH                                   1
#define D18F5x160_NbVid_6_0_OFFSET                             10
#define D18F5x160_NbVid_6_0_WIDTH                              7
#define D18F5x160_NbVid_7_OFFSET                               21
#define D18F5x160_NbVid_7_WIDTH                                1

#define D18F5x160_NbVid_7__OFFSET                               21
#define D18F5x160_NbVid_7__WIDTH                                1

/// D18F5x160
typedef union {
  struct {                                                              ///<
    UINT32                                               NbPstateEn:1 ; ///<
    UINT32                                                    NbFid:6 ; ///<
    UINT32                                                    NbDid:1 ; ///<
    UINT32                                             Reserved_9_8:2 ; ///<
    UINT32                                               NbVid_6_0_:7 ; ///<
    UINT32                                           Reserved_17_17:1 ; ///<
    UINT32                                                MemPstate:1 ; ///<
    UINT32                                           Reserved_20_19:2 ; ///<
    UINT32                                                 NbVid_7_:1 ; ///<
    UINT32                                           Reserved_23_22:2 ; ///<
    UINT32                                           Reserved_31_24:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F5x160_STRUCT;

// **** D18F5x164 Register Definition ****
// Address
#define D18F5x164_ADDRESS                                       0x164

// Type
#define D18F5x164_TYPE                                          TYPE_D18F5
#define D18F5x164_NbPstateEn_OFFSET                             0
#define D18F5x164_NbPstateEn_WIDTH                              1
#define D18F5x164_MemPstate_OFFSET                              18
#define D18F5x164_MemPstate_WIDTH                               1
#define D18F5x164_NbFid_OFFSET                                  1
#define D18F5x164_NbFid_WIDTH                                   6
#define D18F5x164_NbDid_OFFSET                                  7
#define D18F5x164_NbDid_WIDTH                                   1
#define D18F5x164_NbVid_6_0_OFFSET                             10
#define D18F5x164_NbVid_6_0_WIDTH                              7
#define D18F5x164_NbVid_7_OFFSET                               21
#define D18F5x164_NbVid_7_WIDTH                                1

// **** D18F5x168 Register Definition ****
// Address
#define D18F5x168_ADDRESS                                       0x168

// Type
#define D18F5x168_TYPE                                          TYPE_D18F5
#define D18F5x168_NbPstateEn_OFFSET                             0
#define D18F5x168_NbPstateEn_WIDTH                              1
#define D18F5x168_NbFid_OFFSET                                  1
#define D18F5x168_NbFid_WIDTH                                   6
#define D18F5x168_NbDid_OFFSET                                  7
#define D18F5x168_NbDid_WIDTH                                   1
#define D18F5x168_NbVid_6_0_OFFSET                             10
#define D18F5x168_NbVid_6_0_WIDTH                              7
#define D18F5x168_MemPstate_OFFSET                              18
#define D18F5x168_MemPstate_WIDTH                               1
#define D18F5x168_NbVid_7_OFFSET                               21
#define D18F5x168_NbVid_7_WIDTH                                1

// **** D18F5x16C Register Definition ****
// Address
#define D18F5x16C_ADDRESS                                       0x16c

// Type
#define D18F5x16C_TYPE                                          TYPE_D18F5
#define D18F5x16C_NbPstateEn_OFFSET                             0
#define D18F5x16C_NbPstateEn_WIDTH                              1
#define D18F5x16C_NbFid_OFFSET                                  1
#define D18F5x16C_NbFid_WIDTH                                   6
#define D18F5x16C_NbDid_OFFSET                                  7
#define D18F5x16C_NbDid_WIDTH                                   1
#define D18F5x16C_NbVid_6_0_OFFSET                             10
#define D18F5x16C_NbVid_6_0_WIDTH                              7
#define D18F5x16C_MemPstate_OFFSET                              18
#define D18F5x16C_MemPstate_WIDTH                               1
#define D18F5x16C_NbVid_7_OFFSET                               21
#define D18F5x16C_NbVid_7_WIDTH                                1

// **** D18F5x170 Register Definition ****
// Address
#define D18F5x170_ADDRESS                                       0x170

// Type
#define D18F5x170_TYPE                                          TYPE_D18F5
// Field Data
#define D18F5x170_NbPstateMaxVal_OFFSET                         0
#define D18F5x170_NbPstateMaxVal_WIDTH                          2
#define D18F5x170_NbPstateMaxVal_MASK                           0x3
#define D18F5x170_Reserved_2_2_OFFSET                           2
#define D18F5x170_Reserved_2_2_WIDTH                            1
#define D18F5x170_Reserved_2_2_MASK                             0x4
#define D18F5x170_NbPstateLo_OFFSET                             3
#define D18F5x170_NbPstateLo_WIDTH                              2
#define D18F5x170_NbPstateLo_MASK                               0x18
#define D18F5x170_Reserved_5_5_OFFSET                           5
#define D18F5x170_Reserved_5_5_WIDTH                            1
#define D18F5x170_Reserved_5_5_MASK                             0x20
#define D18F5x170_NbPstateHi_OFFSET                             6
#define D18F5x170_NbPstateHi_WIDTH                              2
#define D18F5x170_NbPstateHi_MASK                               0xc0
#define D18F5x170_Reserved_8_8_OFFSET                           8
#define D18F5x170_Reserved_8_8_WIDTH                            1
#define D18F5x170_Reserved_8_8_MASK                             0x100
#define D18F5x170_NbPstateThreshold_OFFSET                      9
#define D18F5x170_NbPstateThreshold_WIDTH                       3
#define D18F5x170_NbPstateThreshold_MASK                        0xe00
#define D18F5x170_Reserved_12_12_OFFSET                         12
#define D18F5x170_Reserved_12_12_WIDTH                          1
#define D18F5x170_Reserved_12_12_MASK                           0x1000
#define D18F5x170_NbPstateDisOnP0_OFFSET                        13
#define D18F5x170_NbPstateDisOnP0_WIDTH                         1
#define D18F5x170_NbPstateDisOnP0_MASK                          0x2000
#define D18F5x170_SwNbPstateLoDis_OFFSET                        14
#define D18F5x170_SwNbPstateLoDis_WIDTH                         1
#define D18F5x170_SwNbPstateLoDis_MASK                          0x4000
#define D18F5x170_Reserved_22_15_OFFSET                         15
#define D18F5x170_Reserved_22_15_WIDTH                          8
#define D18F5x170_Reserved_22_15_MASK                           0x3f8000
#define D18F5x170_NbPstateGnbSlowDis_OFFSET                     23
#define D18F5x170_NbPstateGnbSlowDis_WIDTH                      1
#define D18F5x170_NbPstateGnbSlowDis_MASK                       0x800000
#define D18F5x170_NbPstateLoRes_OFFSET                          24
#define D18F5x170_NbPstateLoRes_WIDTH                           3
#define D18F5x170_NbPstateLoRes_MASK                            0x7000000
#define D18F5x170_NbPstateHiRes_OFFSET                          27
#define D18F5x170_NbPstateHiRes_WIDTH                           3
#define D18F5x170_NbPstateHiRes_MASK                            0x38000000
#define D18F5x170_NbPstateFidVidSbcEn_OFFSET                    30
#define D18F5x170_NbPstateFidVidSbcEn_WIDTH                     1
#define D18F5x170_NbPstateFidVidSbcEn_MASK                      0x40000000
#define D18F5x170_MemPstateDis_OFFSET                           31
#define D18F5x170_MemPstateDis_WIDTH                            1
#define D18F5x170_MemPstateDis_MASK                             0x80000000

/// D18F5x170
typedef union {
  struct {                                                              ///<
    UINT32                                           NbPstateMaxVal:2 ; ///<
    UINT32                                             Reserved_2_2:1 ; ///<
    UINT32                                               NbPstateLo:2 ; ///<
    UINT32                                             Reserved_5_5:1 ; ///<
    UINT32                                               NbPstateHi:2 ; ///<
    UINT32                                             Reserved_8_8:1 ; ///<
    UINT32                                        NbPstateThreshold:3 ; ///<
    UINT32                                           Reserved_12_12:1 ; ///<
    UINT32                                          NbPstateDisOnP0:1 ; ///<
    UINT32                                          SwNbPstateLoDis:1 ; ///<
    UINT32                                           Reserved_22_15:8 ; ///<
    UINT32                                       NbPstateGnbSlowDis:1 ; ///<
    UINT32                                            NbPstateLoRes:3 ; ///<
    UINT32                                            NbPstateHiRes:3 ; ///<
    UINT32                                      NbPstateFidVidSbcEn:1 ; ///<
    UINT32                                             MemPstateDis:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F5x170_STRUCT;

// **** D18F2x118 Register Definition ****
// Address
#define D18F2x118_ADDRESS                                       0x118
// Type
#define D18F2x118_TYPE                                          TYPE_D18F2

// Field Data
#define D18F2x118_MctPriCpuRd_OFFSET                            0
#define D18F2x118_MctPriCpuRd_WIDTH                             2
#define D18F2x118_MctPriCpuRd_MASK                              0x3
#define D18F2x118_MctPriCpuWr_OFFSET                            2
#define D18F2x118_MctPriCpuWr_WIDTH                             2
#define D18F2x118_MctPriCpuWr_MASK                              0xC
#define D18F2x118_MctPriIsocRd_OFFSET                           4
#define D18F2x118_MctPriIsocRd_WIDTH                            2
#define D18F2x118_MctPriIsocRd_MASK                             0x30
#define D18F2x118_MctPriIsocWr_OFFSET                           6
#define D18F2x118_MctPriIsocWr_WIDTH                            2
#define D18F2x118_MctPriIsocWr_MASK                             0xC0
#define D18F2x118_MctPriDefault_OFFSET                          8
#define D18F2x118_MctPriDefault_WIDTH                           2
#define D18F2x118_MctPriDefault_MASK                            0x300
#define D18F2x118_MctPriWr_OFFSET                               10
#define D18F2x118_MctPriWr_WIDTH                                2
#define D18F2x118_MctPriWr_MASK                                 0xC00
#define D18F2x118_MctPriIsoc_OFFSET                             12
#define D18F2x118_MctPriIsoc_WIDTH                              2
#define D18F2x118_MctPriIsoc_MASK                               0x3000
#define D18F2x118_MctPriTrace_OFFSET                            14
#define D18F2x118_MctPriTrace_WIDTH                             2
#define D18F2x118_MctPriTrace_MASK                              0xC000
#define D18F2x118_MctPriScrub_OFFSET                            16
#define D18F2x118_MctPriScrub_WIDTH                             2
#define D18F2x118_MctPriScrub_MASK                              0x30000
#define D18F2x118_CC6SaveEn_OFFSET                              18
#define D18F2x118_CC6SaveEn_WIDTH                               1
#define D18F2x118_CC6SaveEn_MASK                                0x40000
#define D18F2x118_LockDramCfg_OFFSET                            19
#define D18F2x118_LockDramCfg_WIDTH                             1
#define D18F2x118_LockDramCfg_MASK                              0x80000
#define D18F2x118_McqMedPriByPassMax_OFFSET                     20
#define D18F2x118_McqMedPriByPassMax_WIDTH                      3
#define D18F2x118_McqMedPriByPassMax_MASK                       0x700000
#define D18F2x118_Reserved_23_23_OFFSET                         23
#define D18F2x118_Reserved_23_23_WIDTH                          1
#define D18F2x118_Reserved_23_23_MASK                           0x800000
#define D18F2x118_McqHiPriByPassMax_OFFSET                      24
#define D18F2x118_McqHiPriByPassMax_WIDTH                       3
#define D18F2x118_McqHiPriByPassMax_MASK                        0x7000000
#define D18F2x118_Reserved_27_27_OFFSET                         27
#define D18F2x118_Reserved_27_27_WIDTH                          1
#define D18F2x118_Reserved_27_27_MASK                           0x8000000
#define D18F2x118_MctVarPriCntLmt_OFFSET                        28
#define D18F2x118_MctVarPriCntLmt_WIDTH                         4
#define D18F2x118_MctVarPriCntLmt_MASK                          0xF0000000

/// D18F2x118
typedef union {
  struct {                                                            ///<
    UINT32                                             MctPriCpuRd:2; ///<
    UINT32                                             MctPriCpuWr:2; ///<
    UINT32                                            MctPriIsocRd:2; ///<
    UINT32                                            MctPriIsocWr:2; ///<
    UINT32                                           MctPriDefault:2; ///<
    UINT32                                                MctPriWr:2; ///<
    UINT32                                              MctPriIsoc:2; ///<
    UINT32                                             MctPriTrace:2; ///<
    UINT32                                             MctPriScrub:2; ///<
    UINT32                                               CC6SaveEn:1; ///<
    UINT32                                             LockDramCfg:1; ///<
    UINT32                                      McqMedPriByPassMax:3; ///<
    UINT32                                          Reserved_23_23:1; ///<
    UINT32                                       McqHiPriByPassMax:3; ///<
    UINT32                                          Reserved_27_27:1; ///<
    UINT32                                         MctVarPriCntLmt:4; ///<

  } Field;

  UINT32 Value;
} D18F2x118_STRUCT;


// **** D18F3x64 Register Definition ****
// Address
#define D18F3x64_ADDRESS                                        0x64

// Type
#define D18F3x64_TYPE                                           TYPE_D18F3
// Field Data
#define D18F3x64_HtcEn_OFFSET                                   0
#define D18F3x64_HtcEn_WIDTH                                    1
#define D18F3x64_HtcEn_MASK                                     0x1
#define D18F3x64_HtcEn_VALUE                                    0x0
#define D18F3x64_HtcSbcEn_OFFSET                                1
#define D18F3x64_HtcSbcEn_WIDTH                                 1
#define D18F3x64_HtcSbcEn_MASK                                  0x2
#define D18F3x64_HtcPstateSel_OFFSET                            2
#define D18F3x64_HtcPstateSel_WIDTH                             1
#define D18F3x64_HtcPstateSel_MASK                              0x4
#define D18F3x64_Reserved_3_3_OFFSET                            3
#define D18F3x64_Reserved_3_3_WIDTH                             1
#define D18F3x64_Reserved_3_3_MASK                              0x8
#define D18F3x64_HtcAct_OFFSET                                  4
#define D18F3x64_HtcAct_WIDTH                                   1
#define D18F3x64_HtcAct_MASK                                    0x10
#define D18F3x64_HtcActSts_OFFSET                               5
#define D18F3x64_HtcActSts_WIDTH                                1
#define D18F3x64_HtcActSts_MASK                                 0x20
#define D18F3x64_PslApicHiEn_OFFSET                             6
#define D18F3x64_PslApicHiEn_WIDTH                              1
#define D18F3x64_PslApicHiEn_MASK                               0x40
#define D18F3x64_PslApicLoEn_OFFSET                             7
#define D18F3x64_PslApicLoEn_WIDTH                              1
#define D18F3x64_PslApicLoEn_MASK                               0x80
#define D18F3x64_HtcClkAct_OFFSET                               8
#define D18F3x64_HtcClkAct_WIDTH                                3
#define D18F3x64_HtcClkAct_MASK                                 0x700
#define D18F3x64_Reserved_11_11_OFFSET                          11
#define D18F3x64_Reserved_11_11_WIDTH                           1
#define D18F3x64_Reserved_11_11_MASK                            0x800
#define D18F3x64_HtcClkInact_OFFSET                             12
#define D18F3x64_HtcClkInact_WIDTH                              3
#define D18F3x64_HtcClkInact_MASK                               0x7000
#define D18F3x64_Reserved_15_15_OFFSET                          15
#define D18F3x64_Reserved_15_15_WIDTH                           1
#define D18F3x64_Reserved_15_15_MASK                            0x8000
#define D18F3x64_HtcTmpLmt_OFFSET                               16
#define D18F3x64_HtcTmpLmt_WIDTH                                7
#define D18F3x64_HtcTmpLmt_MASK                                 0x7f0000
#define D18F3x64_HtcSlewSel_OFFSET                              23
#define D18F3x64_HtcSlewSel_WIDTH                               1
#define D18F3x64_HtcSlewSel_MASK                                0x800000
#define D18F3x64_HtcHystLmt_OFFSET                              24
#define D18F3x64_HtcHystLmt_WIDTH                               4
#define D18F3x64_HtcHystLmt_MASK                                0xf000000
#define D18F3x64_HtcPstateLimit_OFFSET                          28
#define D18F3x64_HtcPstateLimit_WIDTH                           3
#define D18F3x64_HtcPstateLimit_MASK                            0x70000000
#define D18F3x64_Reserved_31_31_OFFSET                          31
#define D18F3x64_Reserved_31_31_WIDTH                           1
#define D18F3x64_Reserved_31_31_MASK                            0x80000000

/// D18F3x64
typedef union {
  struct {                                                              ///<
    UINT32                                                    HtcEn:1 ; ///<
    UINT32                                                 HtcSbcEn:1 ; ///<
    UINT32                                             HtcPstateSel:1 ; ///<
    UINT32                                             Reserved_3_3:1 ; ///<
    UINT32                                                   HtcAct:1 ; ///<
    UINT32                                                HtcActSts:1 ; ///<
    UINT32                                              PslApicHiEn:1 ; ///<
    UINT32                                              PslApicLoEn:1 ; ///<
    UINT32                                                HtcClkAct:3 ; ///<
    UINT32                                           Reserved_11_11:1 ; ///<
    UINT32                                              HtcClkInact:3 ; ///<
    UINT32                                           Reserved_15_15:1 ; ///<
    UINT32                                                HtcTmpLmt:7 ; ///<
    UINT32                                               HtcSlewSel:1 ; ///<
    UINT32                                               HtcHystLmt:4 ; ///<
    UINT32                                           HtcPstateLimit:3 ; ///<
    UINT32                                           Reserved_31_31:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F3x64_STRUCT;


// **** D18F3xA0 Register Definition ****
// Address
#define D18F3xA0_ADDRESS                                        0xa0

// Type
#define D18F3xA0_TYPE                                           TYPE_D18F3
// Field Data
#define D18F3xA0_PsiVid_6_0__OFFSET                             0
#define D18F3xA0_PsiVid_6_0__WIDTH                              7
#define D18F3xA0_PsiVid_6_0__MASK                               0x7f
#define D18F3xA0_PsiVidEn_OFFSET                                7
#define D18F3xA0_PsiVidEn_WIDTH                                 1
#define D18F3xA0_PsiVidEn_MASK                                  0x80
#define D18F3xA0_PsiVid_7__OFFSET                               8
#define D18F3xA0_PsiVid_7__WIDTH                                1
#define D18F3xA0_PsiVid_7__MASK                                 0x100
#define D18F3xA0_Reserved_9_9_OFFSET                            9
#define D18F3xA0_Reserved_9_9_WIDTH                             1
#define D18F3xA0_Reserved_9_9_MASK                              0x200
#define D18F3xA0_IdleExitEn_OFFSET                              10
#define D18F3xA0_IdleExitEn_WIDTH                               1
#define D18F3xA0_IdleExitEn_MASK                                0x400
#define D18F3xA0_PllLockTime_OFFSET                             11
#define D18F3xA0_PllLockTime_WIDTH                              3
#define D18F3xA0_PllLockTime_MASK                               0x3800
#define D18F3xA0_PllLockTime_VALUE                              0x1
#define D18F3xA0_Svi2HighFreqSel_OFFSET                         14
#define D18F3xA0_Svi2HighFreqSel_WIDTH                          1
#define D18F3xA0_Svi2HighFreqSel_MASK                           0x4000
#define D18F3xA0_Svi2HighFreqSel_VALUE                          0x1
#define D18F3xA0_PwrGateMasterDis_OFFSET                        15
#define D18F3xA0_PwrGateMasterDis_WIDTH                         1
#define D18F3xA0_PwrGateMasterDis_MASK                          0x8000
#define D18F3xA0_ConfigId_OFFSET                                16
#define D18F3xA0_ConfigId_WIDTH                                 12
#define D18F3xA0_ConfigId_MASK                                  0xfff0000
#define D18F3xA0_NbPstateForce_OFFSET                           28
#define D18F3xA0_NbPstateForce_WIDTH                            1
#define D18F3xA0_NbPstateForce_MASK                             0x10000000
#define D18F3xA0_Reserved_29_29_OFFSET                          29
#define D18F3xA0_Reserved_29_29_WIDTH                           1
#define D18F3xA0_Reserved_29_29_MASK                            0x20000000
#define D18F3xA0_DualVdd_OFFSET                                 30
#define D18F3xA0_DualVdd_WIDTH                                  1
#define D18F3xA0_DualVdd_MASK                                   0x40000000
#define D18F3xA0_CofVidProg_OFFSET                              31
#define D18F3xA0_CofVidProg_WIDTH                               1
#define D18F3xA0_CofVidProg_MASK                                0x80000000

/// D18F3xA0
typedef union {
  struct {                                                              ///<
    UINT32                                              PsiVid_6_0_:7 ; ///<
    UINT32                                                 PsiVidEn:1 ; ///<
    UINT32                                                PsiVid_7_:1 ; ///<
    UINT32                                             Reserved_9_9:1 ; ///<
    UINT32                                               IdleExitEn:1 ; ///<
    UINT32                                              PllLockTime:3 ; ///<
    UINT32                                          Svi2HighFreqSel:1 ; ///<
    UINT32                                         PwrGateMasterDis:1 ; ///<
    UINT32                                                 ConfigId:12; ///<
    UINT32                                            NbPstateForce:1 ; ///<
    UINT32                                           Reserved_29_29:1 ; ///<
    UINT32                                                  DualVdd:1 ; ///<
    UINT32                                               CofVidProg:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F3xA0_STRUCT;


// **** D18F3xA8 Register Definition ****
// Address
#define D18F3xA8_ADDRESS                                        0xA8
// Type
#define D18F3xA8_TYPE                                           TYPE_D18F3

// Field Data
#define D18F3xA8_Reserved_28_0_OFFSET                           0
#define D18F3xA8_Reserved_28_0_WIDTH                            29
#define D18F3xA8_Reserved_28_0_MASK                             0x1FFFFFFF
#define D18F3xA8_PopDownPstate_OFFSET                           29
#define D18F3xA8_PopDownPstate_WIDTH                            3
#define D18F3xA8_PopDownPstate_MASK                             0xE0000000

/// D18F3xA8
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_28_0:29; ///<
    UINT32                                           PopDownPstate:3; ///<

  } Field;

  UINT32 Value;
} D18F3xA8_STRUCT;

// **** D18F5x12C Register Definition ****
// Address
#define D18F5x12C_ADDRESS                                       0x12C
// Type
#define D18F5x12C_TYPE                                          TYPE_D18F5

// Field Data
#define D18F5x12C_CoreOffsetTrim_OFFSET                         0
#define D18F5x12C_CoreOffsetTrim_WIDTH                          2
#define D18F5x12C_CoreOffsetTrim_MASK                           0x3
#define D18F5x12C_CoreLoadLineTrim_OFFSET                       2
#define D18F5x12C_CoreLoadLineTrim_WIDTH                        3
#define D18F5x12C_CoreLoadLineTrim_MASK                         0x1C
#define D18F5x12C_CorePsi1En_OFFSET                             5
#define D18F5x12C_CorePsi1En_WIDTH                              1
#define D18F5x12C_CorePsi1En_MASK                               0x20
#define D18F5x12C_CoreTfn_OFFSET                                6
#define D18F5x12C_CoreTfn_WIDTH                                 1
#define D18F5x12C_CoreTfn_MASK                                  0x40
#define D18F5x12C_RAZ_29_7_OFFSET                               7
#define D18F5x12C_RAZ_29_7_WIDTH                                23
#define D18F5x12C_RAZ_29_7_MASK                                 0x3FFFFF80
#define D18F5x12C_WaitVidCompDis_OFFSET                         30
#define D18F5x12C_WaitVidCompDis_WIDTH                          1
#define D18F5x12C_WaitVidCompDis_MASK                           0x40000000
#define D18F5x12C_Svi2CmdBusy_OFFSET                            31
#define D18F5x12C_Svi2CmdBusy_WIDTH                             1
#define D18F5x12C_Svi2CmdBusy_MASK                              0x80000000

/// D18F5x12C
typedef union {
  struct {                                                            ///<
    UINT32                                          CoreOffsetTrim:2; ///<
    UINT32                                        CoreLoadLineTrim:3; ///<
    UINT32                                              CorePsi1En:1; ///<
    UINT32                                                 CoreTfn:1; ///<
    UINT32                                                RAZ_29_7:23; ///<
    UINT32                                          WaitVidCompDis:1; ///<
    UINT32                                             Svi2CmdBusy:1; ///<

  } Field;

  UINT32 Value;
} D18F5x12C_STRUCT;

// **** D18F5x178 Register Definition ****
// Address
#define D18F5x178_ADDRESS                                       0x178
// Type
#define D18F5x178_TYPE                                          TYPE_D18F5

// Field Data
#define D18F5x178_Reserved_1_0_OFFSET                           0
#define D18F5x178_Reserved_1_0_WIDTH                            2
#define D18F5x178_Reserved_1_0_MASK                             0x3
#define D18F5x178_CstateFusionDis_OFFSET                        2
#define D18F5x178_CstateFusionDis_WIDTH                         1
#define D18F5x178_CstateFusionDis_MASK                          0x4
#define D18F5x178_CstateThreeWayHsEn_OFFSET                     3
#define D18F5x178_CstateThreeWayHsEn_WIDTH                      1
#define D18F5x178_CstateThreeWayHsEn_MASK                       0x8
#define D18F5x178_Reserved_17_4_OFFSET                          4
#define D18F5x178_Reserved_17_4_WIDTH                           14
#define D18F5x178_Reserved_17_4_MASK                            0x3FFF0
#define D18F5x178_CstateFusionHsDis_OFFSET                      18
#define D18F5x178_CstateFusionHsDis_WIDTH                       1
#define D18F5x178_CstateFusionHsDis_MASK                        0x40000
#define D18F5x178_SwGfxDis_OFFSET                               19
#define D18F5x178_SwGfxDis_WIDTH                                1
#define D18F5x178_SwGfxDis_MASK                                 0x80000
#define D18F5x178_Reserved_31_20_OFFSET                         20
#define D18F5x178_Reserved_31_20_WIDTH                          12
#define D18F5x178_Reserved_31_20_MASK                           0xFFF00000

/// D18F5x178
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_1_0:2; ///<
    UINT32                                         CstateFusionDis:1; ///<
    UINT32                                      CstateThreeWayHsEn:1; ///<
    UINT32                                           Reserved_17_4:14; ///<
    UINT32                                       CstateFusionHsDis:1; ///<
    UINT32                                                SwGfxDis:1; ///<
    UINT32                                          Reserved_31_20:12; ///<

  } Field;

  UINT32 Value;
} D18F5x178_STRUCT;

// **** D18F5x188 Register Definition ****
// Address
#define D18F5x188_ADDRESS                                       0x188
// Type
#define D18F5x188_TYPE                                          TYPE_D18F5

// Field Data
#define D18F5x188_NbOffsetTrim_OFFSET                           0
#define D18F5x188_NbOffsetTrim_WIDTH                            2
#define D18F5x188_NbOffsetTrim_MASK                             0x3
#define D18F5x188_NbLoadLineTrim_OFFSET                         2
#define D18F5x188_NbLoadLineTrim_WIDTH                          3
#define D18F5x188_NbLoadLineTrim_MASK                           0x1C
#define D18F5x188_NbPsi1_OFFSET                                 5
#define D18F5x188_NbPsi1_WIDTH                                  1
#define D18F5x188_NbPsi1_MASK                                   0x20
#define D18F5x188_NbTfn_OFFSET                                  6
#define D18F5x188_NbTfn_WIDTH                                   1
#define D18F5x188_NbTfn_MASK                                    0x40
#define D18F5x188_RAZ_31_7_OFFSET                               7
#define D18F5x188_RAZ_31_7_WIDTH                                25
#define D18F5x188_RAZ_31_7_MASK                                 0xFFFFFF80

/// D18F5x188
typedef union {
  struct {                                                            ///<
    UINT32                                            NbOffsetTrim:2; ///<
    UINT32                                          NbLoadLineTrim:3; ///<
    UINT32                                                  NbPsi1:1; ///<
    UINT32                                                   NbTfn:1; ///<
    UINT32                                                RAZ_31_7:25; ///<

  } Field;

  UINT32 Value;
} D18F5x188_STRUCT;

// **** D0F0x04 Register Definition ****
// Address
#define D0F0x04_ADDRESS                                         0x4
// Type
#define D0F0x04_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0x04_IoAccessEn_OFFSET                               0
#define D0F0x04_IoAccessEn_WIDTH                                1
#define D0F0x04_IoAccessEn_MASK                                 0x1
#define D0F0x04_MemAccessEn_OFFSET                              1
#define D0F0x04_MemAccessEn_WIDTH                               1
#define D0F0x04_MemAccessEn_MASK                                0x2
#define D0F0x04_BusMasterEn_OFFSET                              2
#define D0F0x04_BusMasterEn_WIDTH                               1
#define D0F0x04_BusMasterEn_MASK                                0x4
#define D0F0x04_Reserved_19_3_OFFSET                            3
#define D0F0x04_Reserved_19_3_WIDTH                             17
#define D0F0x04_Reserved_19_3_MASK                              0xFFFF8
#define D0F0x04_CapList_OFFSET                                  20
#define D0F0x04_CapList_WIDTH                                   1
#define D0F0x04_CapList_MASK                                    0x100000
#define D0F0x04_Reserved_31_21_OFFSET                           21
#define D0F0x04_Reserved_31_21_WIDTH                            11
#define D0F0x04_Reserved_31_21_MASK                             0xFFE00000

/// D0F0x04
typedef union {
  struct {                                                            ///<
    UINT32                                              IoAccessEn:1; ///<
    UINT32                                             MemAccessEn:1; ///<
    UINT32                                             BusMasterEn:1; ///<
    UINT32                                           Reserved_19_3:17; ///<
    UINT32                                                 CapList:1; ///<
    UINT32                                          Reserved_31_21:11; ///<

  } Field;

  UINT32 Value;
} D0F0x04_STRUCT;

// **** D0F0x64 Register Definition ****
// Address
#define D0F0x64_ADDRESS                                         0x64
// Type
#define D0F0x64_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0x64_MiscIndData_OFFSET                              0
#define D0F0x64_MiscIndData_WIDTH                               32
#define D0F0x64_MiscIndData_MASK                                0xFFFFFFFF

/// D0F0x64
typedef union {
  struct {                                                            ///<
    UINT32                                             MiscIndData:32; ///<

  } Field;

  UINT32 Value;
} D0F0x64_STRUCT;

// **** DxF0x3C Register Definition ****
// Address
#define DxF0x3C_ADDRESS                                         0x3c

// Type
#define DxF0x3C_TYPE                                            TYPE_D4F0
// Field Data
#define DxF0x3C_IntLine_OFFSET                                  0
#define DxF0x3C_IntLine_WIDTH                                   8
#define DxF0x3C_IntLine_MASK                                    0xff
#define DxF0x3C_IntPin_OFFSET                                   8
#define DxF0x3C_IntPin_WIDTH                                    3
#define DxF0x3C_IntPin_MASK                                     0x700
#define DxF0x3C_IntPinR_OFFSET                                  11
#define DxF0x3C_IntPinR_WIDTH                                   5
#define DxF0x3C_IntPinR_MASK                                    0xf800
#define DxF0x3C_ParityResponseEn_OFFSET                         16
#define DxF0x3C_ParityResponseEn_WIDTH                          1
#define DxF0x3C_ParityResponseEn_MASK                           0x10000
#define DxF0x3C_SerrEn_OFFSET                                   17
#define DxF0x3C_SerrEn_WIDTH                                    1
#define DxF0x3C_SerrEn_MASK                                     0x20000
#define DxF0x3C_IsaEn_OFFSET                                    18
#define DxF0x3C_IsaEn_WIDTH                                     1
#define DxF0x3C_IsaEn_MASK                                      0x40000
#define DxF0x3C_VgaEn_OFFSET                                    19
#define DxF0x3C_VgaEn_WIDTH                                     1
#define DxF0x3C_VgaEn_MASK                                      0x80000
#define DxF0x3C_Vga16En_OFFSET                                  20
#define DxF0x3C_Vga16En_WIDTH                                   1
#define DxF0x3C_Vga16En_MASK                                    0x100000
#define DxF0x3C_MasterAbortMode_OFFSET                          21
#define DxF0x3C_MasterAbortMode_WIDTH                           1
#define DxF0x3C_MasterAbortMode_MASK                            0x200000
#define DxF0x3C_SecondaryBusReset_OFFSET                        22
#define DxF0x3C_SecondaryBusReset_WIDTH                         1
#define DxF0x3C_SecondaryBusReset_MASK                          0x400000
#define DxF0x3C_FastB2BCap_OFFSET                               23
#define DxF0x3C_FastB2BCap_WIDTH                                1
#define DxF0x3C_FastB2BCap_MASK                                 0x800000
#define DxF0x3C_Reserved_31_24_OFFSET                           24
#define DxF0x3C_Reserved_31_24_WIDTH                            8
#define DxF0x3C_Reserved_31_24_MASK                             0xff000000

/// DxF0x3C
typedef union {
  struct {                                                              ///<
    UINT32                                                  IntLine:8 ; ///<
    UINT32                                                   IntPin:3 ; ///<
    UINT32                                                  IntPinR:5 ; ///<
    UINT32                                         ParityResponseEn:1 ; ///<
    UINT32                                                   SerrEn:1 ; ///<
    UINT32                                                    IsaEn:1 ; ///<
    UINT32                                                    VgaEn:1 ; ///<
    UINT32                                                  Vga16En:1 ; ///<
    UINT32                                          MasterAbortMode:1 ; ///<
    UINT32                                        SecondaryBusReset:1 ; ///<
    UINT32                                               FastB2BCap:1 ; ///<
    UINT32                                           Reserved_31_24:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} DxF0x3C_STRUCT;

// **** DxF0x58 Register Definition ****
// Address
#define DxF0x58_ADDRESS                                         0x58

// Type
#define DxF0x58_TYPE                                            TYPE_D4F0
// Field Data
#define DxF0x58_CapID_OFFSET                                    0
#define DxF0x58_CapID_WIDTH                                     8
#define DxF0x58_CapID_MASK                                      0xff
#define DxF0x58_NextPtr_OFFSET                                  8
#define DxF0x58_NextPtr_WIDTH                                   8
#define DxF0x58_NextPtr_MASK                                    0xff00
#define DxF0x58_Version_OFFSET                                  16
#define DxF0x58_Version_WIDTH                                   4
#define DxF0x58_Version_MASK                                    0xf0000
#define DxF0x58_DeviceType_OFFSET                               20
#define DxF0x58_DeviceType_WIDTH                                4
#define DxF0x58_DeviceType_MASK                                 0xf00000
#define DxF0x58_SlotImplemented_OFFSET                          24
#define DxF0x58_SlotImplemented_WIDTH                           1
#define DxF0x58_SlotImplemented_MASK                            0x1000000
#define DxF0x58_IntMessageNum_OFFSET                            25
#define DxF0x58_IntMessageNum_WIDTH                             5
#define DxF0x58_IntMessageNum_MASK                              0x3e000000
#define DxF0x58_Reserved_31_30_OFFSET                           30
#define DxF0x58_Reserved_31_30_WIDTH                            2
#define DxF0x58_Reserved_31_30_MASK                             0xc0000000

/// DxF0x58
typedef union {
  struct {                                                              ///<
    UINT32                                                    CapID:8 ; ///<
    UINT32                                                  NextPtr:8 ; ///<
    UINT32                                                  Version:4 ; ///<
    UINT32                                               DeviceType:4 ; ///<
    UINT32                                          SlotImplemented:1 ; ///<
    UINT32                                            IntMessageNum:5 ; ///<
    UINT32                                           Reserved_31_30:2 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} DxF0x58_STRUCT;

// **** DxFxx68 Register Definition ****
// Address
#define DxFxx68_ADDRESS                                         0x68
// Type
#define DxFxx68_TYPE                                            TYPE_D2F1

// Field Data
#define DxFxx68_PmControl_OFFSET                                0
#define DxFxx68_PmControl_WIDTH                                 2
#define DxFxx68_PmControl_MASK                                  0x3
#define DxFxx68_Reserved_2_2_OFFSET                             2
#define DxFxx68_Reserved_2_2_WIDTH                              1
#define DxFxx68_Reserved_2_2_MASK                               0x4
#define DxFxx68_ReadCplBoundary_OFFSET                          3
#define DxFxx68_ReadCplBoundary_WIDTH                           1
#define DxFxx68_ReadCplBoundary_MASK                            0x8
#define DxFxx68_LinkDis_OFFSET                                  4
#define DxFxx68_LinkDis_WIDTH                                   1
#define DxFxx68_LinkDis_MASK                                    0x10
#define DxFxx68_RetrainLink_OFFSET                              5
#define DxFxx68_RetrainLink_WIDTH                               1
#define DxFxx68_RetrainLink_MASK                                0x20
#define DxFxx68_CommonClockCfg_OFFSET                           6
#define DxFxx68_CommonClockCfg_WIDTH                            1
#define DxFxx68_CommonClockCfg_MASK                             0x40
#define DxFxx68_ExtendedSync_OFFSET                             7
#define DxFxx68_ExtendedSync_WIDTH                              1
#define DxFxx68_ExtendedSync_MASK                               0x80
#define DxFxx68_ClockPowerManagementEn_OFFSET                   8
#define DxFxx68_ClockPowerManagementEn_WIDTH                    1
#define DxFxx68_ClockPowerManagementEn_MASK                     0x100
#define DxFxx68_HWAutonomousWidthDisable_OFFSET                 9
#define DxFxx68_HWAutonomousWidthDisable_WIDTH                  1
#define DxFxx68_HWAutonomousWidthDisable_MASK                   0x200
#define DxFxx68_LinkBWManagementEn_OFFSET                       10
#define DxFxx68_LinkBWManagementEn_WIDTH                        1
#define DxFxx68_LinkBWManagementEn_MASK                         0x400
#define DxFxx68_LinkAutonomousBWIntEn_OFFSET                    11
#define DxFxx68_LinkAutonomousBWIntEn_WIDTH                     1
#define DxFxx68_LinkAutonomousBWIntEn_MASK                      0x800
#define DxFxx68_Reserved_15_12_OFFSET                           12
#define DxFxx68_Reserved_15_12_WIDTH                            4
#define DxFxx68_Reserved_15_12_MASK                             0xF000
#define DxFxx68_LinkSpeed_OFFSET                                16
#define DxFxx68_LinkSpeed_WIDTH                                 4
#define DxFxx68_LinkSpeed_MASK                                  0xF0000
#define DxFxx68_NegotiatedLinkWidth_OFFSET                      20
#define DxFxx68_NegotiatedLinkWidth_WIDTH                       6
#define DxFxx68_NegotiatedLinkWidth_MASK                        0x3F00000
#define DxFxx68_Reserved_26_26_OFFSET                           26
#define DxFxx68_Reserved_26_26_WIDTH                            1
#define DxFxx68_Reserved_26_26_MASK                             0x4000000
#define DxFxx68_LinkTraining_OFFSET                             27
#define DxFxx68_LinkTraining_WIDTH                              1
#define DxFxx68_LinkTraining_MASK                               0x8000000
#define DxFxx68_SlotClockCfg_OFFSET                             28
#define DxFxx68_SlotClockCfg_WIDTH                              1
#define DxFxx68_SlotClockCfg_MASK                               0x10000000
#define DxFxx68_DlActive_OFFSET                                 29
#define DxFxx68_DlActive_WIDTH                                  1
#define DxFxx68_DlActive_MASK                                   0x20000000
#define DxFxx68_LinkBWManagementStatus_OFFSET                   30
#define DxFxx68_LinkBWManagementStatus_WIDTH                    1
#define DxFxx68_LinkBWManagementStatus_MASK                     0x40000000
#define DxFxx68_LinkAutonomousBWStatus_OFFSET                   31
#define DxFxx68_LinkAutonomousBWStatus_WIDTH                    1
#define DxFxx68_LinkAutonomousBWStatus_MASK                     0x80000000

/// DxFxx68
typedef union {
  struct {                                                            ///<
    UINT32                                               PmControl:2; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                         ReadCplBoundary:1; ///<
    UINT32                                                 LinkDis:1; ///<
    UINT32                                             RetrainLink:1; ///<
    UINT32                                          CommonClockCfg:1; ///<
    UINT32                                            ExtendedSync:1; ///<
    UINT32                                  ClockPowerManagementEn:1; ///<
    UINT32                                HWAutonomousWidthDisable:1; ///<
    UINT32                                      LinkBWManagementEn:1; ///<
    UINT32                                   LinkAutonomousBWIntEn:1; ///<
    UINT32                                          Reserved_15_12:4; ///<
    UINT32                                               LinkSpeed:4; ///<
    UINT32                                     NegotiatedLinkWidth:6; ///<
    UINT32                                          Reserved_26_26:1; ///<
    UINT32                                            LinkTraining:1; ///<
    UINT32                                            SlotClockCfg:1; ///<
    UINT32                                                DlActive:1; ///<
    UINT32                                  LinkBWManagementStatus:1; ///<
    UINT32                                  LinkAutonomousBWStatus:1; ///<

  } Field;

  UINT32 Value;
} DxFxx68_STRUCT;

// **** D0F0x7C Register Definition ****
// Address
#define D0F0x7C_ADDRESS                                         0x7C
// Type
#define D0F0x7C_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0x7C_ForceIntGfxDisable_OFFSET                       0
#define D0F0x7C_ForceIntGfxDisable_WIDTH                        1
#define D0F0x7C_ForceIntGfxDisable_MASK                         0x1
#define D0F0x7C_Reserved_31_1_OFFSET                            1
#define D0F0x7C_Reserved_31_1_WIDTH                             31
#define D0F0x7C_Reserved_31_1_MASK                              0xFFFFFFFE

/// D0F0x7C
typedef union {
  struct {                                                            ///<
    UINT32                                      ForceIntGfxDisable:1; ///<
    UINT32                                           Reserved_31_1:31; ///<

  } Field;

  UINT32 Value;
} D0F0x7C_STRUCT;

// **** D0F0x98 Register Definition ****
// Address
#define D0F0x98_ADDRESS                                         0x98
// Type
#define D0F0x98_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0x98_OrbIndData_OFFSET                               0
#define D0F0x98_OrbIndData_WIDTH                                32
#define D0F0x98_OrbIndData_MASK                                 0xFFFFFFFF

/// D0F0x98
typedef union {
  struct {                                                            ///<
    UINT32                                              OrbIndData:32; ///<

  } Field;

  UINT32 Value;
} D0F0x98_STRUCT;

// **** D0F0xBC Register Definition ****
// Address
#define D0F0xBC_ADDRESS                                         0xBC
// Type
#define D0F0xBC_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0xBC_NbSmuIndData_OFFSET                             0
#define D0F0xBC_NbSmuIndData_WIDTH                              32
#define D0F0xBC_NbSmuIndData_MASK                               0xFFFFFFFF

/// D0F0xBC
typedef union {
  struct {                                                            ///<
    UINT32                                            NbSmuIndData:32; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_STRUCT;

// **** D0F0xC8 Register Definition ****
// Address
#define D0F0xC8_ADDRESS                                         0xC8
// Type
#define D0F0xC8_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0xC8_NbDevIndAddr_OFFSET                             0
#define D0F0xC8_NbDevIndAddr_WIDTH                              7
#define D0F0xC8_NbDevIndAddr_MASK                               0x7F
#define D0F0xC8_Reserved_15_7_OFFSET                            7
#define D0F0xC8_Reserved_15_7_WIDTH                             9
#define D0F0xC8_Reserved_15_7_MASK                              0xFF80
#define D0F0xC8_NbDevIndSel_OFFSET                              16
#define D0F0xC8_NbDevIndSel_WIDTH                               8
#define D0F0xC8_NbDevIndSel_MASK                                0xFF0000
#define D0F0xC8_Reserved_31_24_OFFSET                           24
#define D0F0xC8_Reserved_31_24_WIDTH                            8
#define D0F0xC8_Reserved_31_24_MASK                             0xFF000000

/// D0F0xC8
typedef union {
  struct {                                                            ///<
    UINT32                                            NbDevIndAddr:7; ///<
    UINT32                                           Reserved_15_7:9; ///<
    UINT32                                             NbDevIndSel:8; ///<
    UINT32                                          Reserved_31_24:8; ///<

  } Field;

  UINT32 Value;
} D0F0xC8_STRUCT;

// **** D0F0xCC Register Definition ****
// Address
#define D0F0xCC_ADDRESS                                         0xCC
// Type
#define D0F0xCC_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0xCC_NbDevIndData_OFFSET                             0
#define D0F0xCC_NbDevIndData_WIDTH                              32
#define D0F0xCC_NbDevIndData_MASK                               0xFFFFFFFF

/// D0F0xCC
typedef union {
  struct {                                                            ///<
    UINT32                                            NbDevIndData:32; ///<

  } Field;

  UINT32 Value;
} D0F0xCC_STRUCT;

// **** D0F0xE4 Register Definition ****
// Address
#define D0F0xE4_ADDRESS                                         0xE4
// Type
#define D0F0xE4_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0xE4_PcieIndxData_OFFSET                             0
#define D0F0xE4_PcieIndxData_WIDTH                              32
#define D0F0xE4_PcieIndxData_MASK                               0xFFFFFFFF

/// D0F0xE4
typedef union {
  struct {                                                            ///<
    UINT32                                            PcieIndxData:32; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_STRUCT;

// **** DxFxxE4_x6A Register Definition ****
// Address
#define DxFxxE4_x6A_ADDRESS                                     0x6A
// Type
#define DxFxxE4_x6A_TYPE                                        TYPE_D2F1xE4

// Field Data
#define DxFxxE4_x6A_ErrReportingDis_OFFSET                          0
#define DxFxxE4_x6A_ErrReportingDis_WIDTH                           1
#define DxFxxE4_x6A_ErrReportingDis_MASK                            0x1

// **** DxFxxE4_xA2 Register Definition ****
// Address
#define DxFxxE4_xA2_ADDRESS                                     0xA2
// Type
#define DxFxxE4_xA2_TYPE                                        TYPE_D2F1xE4

// Field Data
#define DxFxxE4_xA2_LcLinkWidth_OFFSET                          0
#define DxFxxE4_xA2_LcLinkWidth_WIDTH                           3
#define DxFxxE4_xA2_LcLinkWidth_MASK                            0x7
#define DxFxxE4_xA2_Reserved_3_3_OFFSET                         3
#define DxFxxE4_xA2_Reserved_3_3_WIDTH                          1
#define DxFxxE4_xA2_Reserved_3_3_MASK                           0x8
#define DxFxxE4_xA2_LcLinkWidthRd_OFFSET                        4
#define DxFxxE4_xA2_LcLinkWidthRd_WIDTH                         3
#define DxFxxE4_xA2_LcLinkWidthRd_MASK                          0x70
#define DxFxxE4_xA2_LcReconfigArcMissingEscape_OFFSET           7
#define DxFxxE4_xA2_LcReconfigArcMissingEscape_WIDTH            1
#define DxFxxE4_xA2_LcReconfigArcMissingEscape_MASK             0x80
#define DxFxxE4_xA2_LcReconfigNow_OFFSET                        8
#define DxFxxE4_xA2_LcReconfigNow_WIDTH                         1
#define DxFxxE4_xA2_LcReconfigNow_MASK                          0x100
#define DxFxxE4_xA2_LcRenegotiationSupport_OFFSET               9
#define DxFxxE4_xA2_LcRenegotiationSupport_WIDTH                1
#define DxFxxE4_xA2_LcRenegotiationSupport_MASK                 0x200
#define DxFxxE4_xA2_LcRenegotiateEn_OFFSET                      10
#define DxFxxE4_xA2_LcRenegotiateEn_WIDTH                       1
#define DxFxxE4_xA2_LcRenegotiateEn_MASK                        0x400
#define DxFxxE4_xA2_LcShortReconfigEn_OFFSET                    11
#define DxFxxE4_xA2_LcShortReconfigEn_WIDTH                     1
#define DxFxxE4_xA2_LcShortReconfigEn_MASK                      0x800
#define DxFxxE4_xA2_LcUpconfigureSupport_OFFSET                 12
#define DxFxxE4_xA2_LcUpconfigureSupport_WIDTH                  1
#define DxFxxE4_xA2_LcUpconfigureSupport_MASK                   0x1000
#define DxFxxE4_xA2_LcUpconfigureDis_OFFSET                     13
#define DxFxxE4_xA2_LcUpconfigureDis_WIDTH                      1
#define DxFxxE4_xA2_LcUpconfigureDis_MASK                       0x2000
#define DxFxxE4_xA2_Reserved_19_14_OFFSET                       14
#define DxFxxE4_xA2_Reserved_19_14_WIDTH                        6
#define DxFxxE4_xA2_Reserved_19_14_MASK                         0xFC000
#define DxFxxE4_xA2_LcUpconfigCapable_OFFSET                    20
#define DxFxxE4_xA2_LcUpconfigCapable_WIDTH                     1
#define DxFxxE4_xA2_LcUpconfigCapable_MASK                      0x100000
#define DxFxxE4_xA2_LcDynLanesPwrState_OFFSET                   21
#define DxFxxE4_xA2_LcDynLanesPwrState_WIDTH                    2
#define DxFxxE4_xA2_LcDynLanesPwrState_MASK                     0x600000
#define DxFxxE4_xA2_Reserved_31_23_OFFSET                       23
#define DxFxxE4_xA2_Reserved_31_23_WIDTH                        9
#define DxFxxE4_xA2_Reserved_31_23_MASK                         0xFF800000

/// DxFxxE4_xA2
typedef union {
  struct {                                                            ///<
    UINT32                                             LcLinkWidth:3; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                           LcLinkWidthRd:3; ///<
    UINT32                              LcReconfigArcMissingEscape:1; ///<
    UINT32                                           LcReconfigNow:1; ///<
    UINT32                                  LcRenegotiationSupport:1; ///<
    UINT32                                         LcRenegotiateEn:1; ///<
    UINT32                                       LcShortReconfigEn:1; ///<
    UINT32                                    LcUpconfigureSupport:1; ///<
    UINT32                                        LcUpconfigureDis:1; ///<
    UINT32                                          Reserved_19_14:6; ///<
    UINT32                                       LcUpconfigCapable:1; ///<
    UINT32                                      LcDynLanesPwrState:2; ///<
    UINT32                                          Reserved_31_23:9; ///<

  } Field;

  UINT32 Value;
} DxFxxE4_xA2_STRUCT;

// **** DxFxx128 Register Definition ****
// Address
#define DxFxx128_ADDRESS                                        0x128
// Type
#define DxFxx128_TYPE                                           TYPE_D2F1

// Field Data
#define DxFxx128_Reserved_15_0_OFFSET                           0
#define DxFxx128_Reserved_15_0_WIDTH                            16
#define DxFxx128_Reserved_15_0_MASK                             0xFFFF
#define DxFxx128_PortArbTableStatus_OFFSET                      16
#define DxFxx128_PortArbTableStatus_WIDTH                       1
#define DxFxx128_PortArbTableStatus_MASK                        0x10000
#define DxFxx128_VcNegotiationPending_OFFSET                    17
#define DxFxx128_VcNegotiationPending_WIDTH                     1
#define DxFxx128_VcNegotiationPending_MASK                      0x20000
#define DxFxx128_Reserved_31_18_OFFSET                          18
#define DxFxx128_Reserved_31_18_WIDTH                           14
#define DxFxx128_Reserved_31_18_MASK                            0xFFFC0000

/// DxFxx128
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_15_0:16; ///<
    UINT32                                      PortArbTableStatus:1; ///<
    UINT32                                    VcNegotiationPending:1; ///<
    UINT32                                          Reserved_31_18:14; ///<

  } Field;

  UINT32 Value;
} DxFxx128_STRUCT;

// **** D0F0x64_x0D Register Definition ****
// Address
#define D0F0x64_x0D_ADDRESS                                     0xD
// Type
#define D0F0x64_x0D_TYPE                                        TYPE_D0F0x64

// Field Data
#define D0F0x64_x0D_PciDev0Fn2RegEn_OFFSET                      0
#define D0F0x64_x0D_PciDev0Fn2RegEn_WIDTH                       1
#define D0F0x64_x0D_PciDev0Fn2RegEn_MASK                        0x1
#define D0F0x64_x0D_Reserved_30_1_OFFSET                        1
#define D0F0x64_x0D_Reserved_30_1_WIDTH                         30
#define D0F0x64_x0D_Reserved_30_1_MASK                          0x7FFFFFFE
#define D0F0x64_x0D_IommuDis_OFFSET                             31
#define D0F0x64_x0D_IommuDis_WIDTH                              1
#define D0F0x64_x0D_IommuDis_MASK                               0x80000000

/// D0F0x64_x0D
typedef union {
  struct {                                                            ///<
    UINT32                                         PciDev0Fn2RegEn:1; ///<
    UINT32                                           Reserved_30_1:30; ///<
    UINT32                                                IommuDis:1; ///<

  } Field;

  UINT32 Value;
} D0F0x64_x0D_STRUCT;

// **** D0F0x64_x16 Register Definition ****
// Address
#define D0F0x64_x16_ADDRESS                                     0x16

// Type
#define D0F0x64_x16_TYPE                                        TYPE_D0F0x64
// Field Data
#define D0F0x64_x16_AerUrMsgEn_OFFSET                           0
#define D0F0x64_x16_AerUrMsgEn_WIDTH                            1
#define D0F0x64_x16_AerUrMsgEn_MASK                             0x1
#define D0F0x64_x16_Reserved_31_1_OFFSET                        1
#define D0F0x64_x16_Reserved_31_1_WIDTH                         31
#define D0F0x64_x16_Reserved_31_1_MASK                          0xfffffffe

/// D0F0x64_x16
typedef union {
  struct {                                                              ///<
    UINT32                                               AerUrMsgEn:1 ; ///<
    UINT32                                            Reserved_31_1:31; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0x64_x16_STRUCT;

// **** D0F0x64_x1D Register Definition ****
// Address
#define D0F0x64_x1D_ADDRESS                                     0x1D
// Type
#define D0F0x64_x1D_TYPE                                        TYPE_D0F0x64

// Field Data
#define D0F0x64_x1D_Reserved_0_0_OFFSET                         0
#define D0F0x64_x1D_Reserved_0_0_WIDTH                          1
#define D0F0x64_x1D_Reserved_0_0_MASK                           0x1
#define D0F0x64_x1D_VgaEn_OFFSET                                1
#define D0F0x64_x1D_VgaEn_WIDTH                                 1
#define D0F0x64_x1D_VgaEn_MASK                                  0x2
#define D0F0x64_x1D_Reserved_2_2_OFFSET                         2
#define D0F0x64_x1D_Reserved_2_2_WIDTH                          1
#define D0F0x64_x1D_Reserved_2_2_MASK                           0x4
#define D0F0x64_x1D_Vga16En_OFFSET                              3
#define D0F0x64_x1D_Vga16En_WIDTH                               1
#define D0F0x64_x1D_Vga16En_MASK                                0x8
#define D0F0x64_x1D_Reserved_31_4_OFFSET                        4
#define D0F0x64_x1D_Reserved_31_4_WIDTH                         28
#define D0F0x64_x1D_Reserved_31_4_MASK                          0xFFFFFFF0

/// D0F0x64_x1D
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                                   VgaEn:1; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                                 Vga16En:1; ///<
    UINT32                                           Reserved_31_4:28; ///<

  } Field;

  UINT32 Value;
} D0F0x64_x1D_STRUCT;

// **** D0F0x64_x22 Register Definition ****
// Address
#define D0F0x64_x22_ADDRESS                                     0x22
// Type
#define D0F0x64_x22_TYPE                                        TYPE_D0F0x64

// Field Data
#define D0F0x64_x22_Reserved_23_0_OFFSET                        0
#define D0F0x64_x22_Reserved_23_0_WIDTH                         24
#define D0F0x64_x22_Reserved_23_0_MASK                          0x0FFFFFF
#define D0F0x64_x22_SoftOverrideClk6_OFFSET                     24
#define D0F0x64_x22_SoftOverrideClk6_WIDTH                      1
#define D0F0x64_x22_SoftOverrideClk6_MASK                       0x1000000
#define D0F0x64_x22_Reserved_25_25_OFFSET                       25
#define D0F0x64_x22_Reserved_25_25_WIDTH                        1
#define D0F0x64_x22_Reserved_25_25_MASK                         0x2000000
#define D0F0x64_x22_SoftOverrideClk4_OFFSET                     26
#define D0F0x64_x22_SoftOverrideClk4_WIDTH                      1
#define D0F0x64_x22_SoftOverrideClk4_MASK                       0x4000000
#define D0F0x64_x22_SoftOverrideClk3_OFFSET                     27
#define D0F0x64_x22_SoftOverrideClk3_WIDTH                      1
#define D0F0x64_x22_SoftOverrideClk3_MASK                       0x8000000
#define D0F0x64_x22_SoftOverrideClk2_OFFSET                     28
#define D0F0x64_x22_SoftOverrideClk2_WIDTH                      1
#define D0F0x64_x22_SoftOverrideClk2_MASK                       0x10000000
#define D0F0x64_x22_SoftOverrideClk1_OFFSET                     29
#define D0F0x64_x22_SoftOverrideClk1_WIDTH                      1
#define D0F0x64_x22_SoftOverrideClk1_MASK                       0x20000000
#define D0F0x64_x22_SoftOverrideClk0_OFFSET                     30
#define D0F0x64_x22_SoftOverrideClk0_WIDTH                      1
#define D0F0x64_x22_SoftOverrideClk0_MASK                       0x40000000
#define D0F0x64_x22_Reserved_31_31_OFFSET                       31
#define D0F0x64_x22_Reserved_31_31_WIDTH                        1
#define D0F0x64_x22_Reserved_31_31_MASK                         0x80000000

/// D0F0x64_x22
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_25_0:26; ///<
    UINT32                                        SoftOverrideClk6:1; ///<
    UINT32                                          Reserved_25_25:1; ///<
    UINT32                                        SoftOverrideClk4:1; ///<
    UINT32                                        SoftOverrideClk3:1; ///<
    UINT32                                        SoftOverrideClk2:1; ///<
    UINT32                                        SoftOverrideClk1:1; ///<
    UINT32                                        SoftOverrideClk0:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0x64_x22_STRUCT;

// **** D0F0x64_x23 Register Definition ****
// Address
#define D0F0x64_x23_ADDRESS                                     0x23
// Type
#define D0F0x64_x23_TYPE                                        TYPE_D0F0x64

// Field Data
#define D0F0x64_x23_Reserved_25_0_OFFSET                        0
#define D0F0x64_x23_Reserved_25_0_WIDTH                         26
#define D0F0x64_x23_Reserved_25_0_MASK                          0x3FFFFFF
#define D0F0x64_x23_SoftOverrideClk4_OFFSET                     26
#define D0F0x64_x23_SoftOverrideClk4_WIDTH                      1
#define D0F0x64_x23_SoftOverrideClk4_MASK                       0x4000000
#define D0F0x64_x23_SoftOverrideClk3_OFFSET                     27
#define D0F0x64_x23_SoftOverrideClk3_WIDTH                      1
#define D0F0x64_x23_SoftOverrideClk3_MASK                       0x8000000
#define D0F0x64_x23_SoftOverrideClk2_OFFSET                     28
#define D0F0x64_x23_SoftOverrideClk2_WIDTH                      1
#define D0F0x64_x23_SoftOverrideClk2_MASK                       0x10000000
#define D0F0x64_x23_SoftOverrideClk1_OFFSET                     29
#define D0F0x64_x23_SoftOverrideClk1_WIDTH                      1
#define D0F0x64_x23_SoftOverrideClk1_MASK                       0x20000000
#define D0F0x64_x23_SoftOverrideClk0_OFFSET                     30
#define D0F0x64_x23_SoftOverrideClk0_WIDTH                      1
#define D0F0x64_x23_SoftOverrideClk0_MASK                       0x40000000
#define D0F0x64_x23_Reserved_31_31_OFFSET                       31
#define D0F0x64_x23_Reserved_31_31_WIDTH                        1
#define D0F0x64_x23_Reserved_31_31_MASK                         0x80000000

/// D0F0x64_x23
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_25_0:26; ///<
    UINT32                                        SoftOverrideClk4:1; ///<
    UINT32                                        SoftOverrideClk3:1; ///<
    UINT32                                        SoftOverrideClk2:1; ///<
    UINT32                                        SoftOverrideClk1:1; ///<
    UINT32                                        SoftOverrideClk0:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0x64_x23_STRUCT;

// **** D0F0x64_x30 Register Definition ****
// Address
#define D0F0x64_x30_ADDRESS                                     0x30
// Type
#define D0F0x64_x30_TYPE                                        TYPE_D0F0x64

// Field Data
#define D0F0x64_x30_DevFnMap_OFFSET                             0
#define D0F0x64_x30_DevFnMap_WIDTH                              8
#define D0F0x64_x30_DevFnMap_MASK                               0xFF
#define D0F0x64_x30_Reserved_31_8_OFFSET                        8
#define D0F0x64_x30_Reserved_31_8_WIDTH                         24
#define D0F0x64_x30_Reserved_31_8_MASK                          0xFFFFFF00

/// D0F0x64_x30
typedef union {
  struct {                                                            ///<
    UINT32                                                DevFnMap:8; ///<
    UINT32                                           Reserved_31_8:24; ///<

  } Field;

  UINT32 Value;
} D0F0x64_x30_STRUCT;

// **** D0F0x64_x46 Register Definition ****
// Address
#define D0F0x64_x46_ADDRESS                                     0x46
// Type
#define D0F0x64_x46_TYPE                                        TYPE_D0F0x64

// Field Data
#define D0F0x64_x46_Reserved_0_0_OFFSET                          0
#define D0F0x64_x46_Reserved_0_0_WIDTH                           1
#define D0F0x64_x46_Reserved_0_0_MASK                            0x1
#define D0F0x64_x46_P2PMode_OFFSET                               1
#define D0F0x64_x46_P2PMode_WIDTH                                2
#define D0F0x64_x46_P2PMode_MASK                                 0x6
#define D0F0x64_x46_Reserved_15_3_OFFSET                         3
#define D0F0x64_x46_Reserved_15_3_WIDTH                          13
#define D0F0x64_x46_Reserved_15_3_MASK                           0xFFF8
#define D0F0x64_x46_CgttLclkOverride_OFFSET                      16
#define D0F0x64_x46_CgttLclkOverride_WIDTH                       1
#define D0F0x64_x46_CgttLclkOverride_MASK                        0x10000
#define D0F0x64_x46_Reserved_31_17_OFFSET                        17
#define D0F0x64_x46_Reserved_31_17_WIDTH                         15
#define D0F0x64_x46_Reserved_31_17_MASK                          0xFFFC0000

/// D0F0x64_x46
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                                 P2PMode:2; ///<
    UINT32                                           Reserved_15_3:13; ///<
    UINT32                                        CgttLclkOverride:1;  ///<
    UINT32                                          Reserved_31_17:15; ///<
  } Field;

  UINT32 Value;
} D0F0x64_x46_STRUCT;

// **** D0F0x94 Register Definition ****
// Address
#define D0F0x94_ADDRESS                                         0x94

// Type
#define D0F0x94_TYPE                                            TYPE_D0F0
// Field Data
#define D0F0x94_OrbIndAddr_OFFSET                               0
#define D0F0x94_OrbIndAddr_WIDTH                                7
#define D0F0x94_OrbIndAddr_MASK                                 0x7f
#define D0F0x94_Reserved_7_7_OFFSET                             7
#define D0F0x94_Reserved_7_7_WIDTH                              1
#define D0F0x94_Reserved_7_7_MASK                               0x80
#define D0F0x94_OrbIndWrEn_OFFSET                               8
#define D0F0x94_OrbIndWrEn_WIDTH                                1
#define D0F0x94_OrbIndWrEn_MASK                                 0x100
#define D0F0x94_Reserved_31_9_OFFSET                            9
#define D0F0x94_Reserved_31_9_WIDTH                             23
#define D0F0x94_Reserved_31_9_MASK                              0xfffffe00

/// D0F0x94
typedef union {
  struct {                                                              ///<
    UINT32                                               OrbIndAddr:7 ; ///<
    UINT32                                             Reserved_7_7:1 ; ///<
    UINT32                                               OrbIndWrEn:1 ; ///<
    UINT32                                            Reserved_31_9:23; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0x94_STRUCT;

// **** D0F0x98_x07 Register Definition ****
// Address
#define D0F0x98_x07_ADDRESS                                     0x7
// Type
#define D0F0x98_x07_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x07_IocBwOptEn_OFFSET                           0
#define D0F0x98_x07_IocBwOptEn_WIDTH                            1
#define D0F0x98_x07_IocBwOptEn_MASK                             0x1
#define D0F0x98_x07_IocWrROMapDis_OFFSET                        1
#define D0F0x98_x07_IocWrROMapDis_WIDTH                         1
#define D0F0x98_x07_IocWrROMapDis_MASK                          0x2
#define D0F0x98_x07_IocRdROMapDis_OFFSET                        2
#define D0F0x98_x07_IocRdROMapDis_WIDTH                         1
#define D0F0x98_x07_IocRdROMapDis_MASK                          0x4
#define D0F0x98_x07_Reserved_3_3_OFFSET                         3
#define D0F0x98_x07_Reserved_3_3_WIDTH                          1
#define D0F0x98_x07_Reserved_3_3_MASK                           0x8
#define D0F0x98_x07_IommuBwOptEn_OFFSET                         4
#define D0F0x98_x07_IommuBwOptEn_WIDTH                          1
#define D0F0x98_x07_IommuBwOptEn_MASK                           0x10
#define D0F0x98_x07_Reserved_5_5_OFFSET                         5
#define D0F0x98_x07_Reserved_5_5_WIDTH                          1
#define D0F0x98_x07_Reserved_5_5_MASK                           0x20
#define D0F0x98_x07_DmaReqRespPassPWMode_OFFSET                 6
#define D0F0x98_x07_DmaReqRespPassPWMode_WIDTH                  1
#define D0F0x98_x07_DmaReqRespPassPWMode_MASK                   0x40
#define D0F0x98_x07_IommuIsocPassPWMode_OFFSET                  7
#define D0F0x98_x07_IommuIsocPassPWMode_WIDTH                   1
#define D0F0x98_x07_IommuIsocPassPWMode_MASK                    0x80
#define D0F0x98_x07_Reserved_14_8_OFFSET                        8
#define D0F0x98_x07_Reserved_14_8_WIDTH                         7
#define D0F0x98_x07_Reserved_14_8_MASK                          0x7F00
#define D0F0x98_x07_DropZeroMaskWrEn_OFFSET                     15
#define D0F0x98_x07_DropZeroMaskWrEn_WIDTH                      1
#define D0F0x98_x07_DropZeroMaskWrEn_MASK                       0x8000
#define D0F0x98_x07_SyncFloodOnParityErr_OFFSET                 16
#define D0F0x98_x07_SyncFloodOnParityErr_WIDTH                  1
#define D0F0x98_x07_SyncFloodOnParityErr_MASK                   0x10000
#define D0F0x98_x07_Reserved_30_17_OFFSET                       17
#define D0F0x98_x07_Reserved_30_17_WIDTH                        14
#define D0F0x98_x07_Reserved_30_17_MASK                         0x7FFE0000
#define D0F0x98_x07_SMUCsrIsocEn_OFFSET                         31
#define D0F0x98_x07_SMUCsrIsocEn_WIDTH                          1
#define D0F0x98_x07_SMUCsrIsocEn_MASK                           0x80000000

/// D0F0x98_x07
typedef union {
  struct {                                                            ///<
    UINT32                                              IocBwOptEn:1; ///<
    UINT32                                           IocWrROMapDis:1; ///<
    UINT32                                           IocRdROMapDis:1; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                            IommuBwOptEn:1; ///<
    UINT32                                            Reserved_5_5:1; ///<
    UINT32                                    DmaReqRespPassPWMode:1; ///<
    UINT32                                     IommuIsocPassPWMode:1; ///<
    UINT32                                           Reserved_14_8:7; ///<
    UINT32                                        DropZeroMaskWrEn:1; ///<
    UINT32                                    SyncFloodOnParityErr:1; ///<
    UINT32                                          Reserved_30_17:14; ///<
    UINT32                                            SMUCsrIsocEn:1; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x07_STRUCT;

// **** D0F0x98_x08 Register Definition ****
// Address
#define D0F0x98_x08_ADDRESS                                     0x8
// Type
#define D0F0x98_x08_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x08_NpWrrLenA_OFFSET                            0
#define D0F0x98_x08_NpWrrLenA_WIDTH                             8
#define D0F0x98_x08_NpWrrLenA_MASK                              0xFF
#define D0F0x98_x08_NpWrrLenB_OFFSET                            8
#define D0F0x98_x08_NpWrrLenB_WIDTH                             8
#define D0F0x98_x08_NpWrrLenB_MASK                              0xFF00
#define D0F0x98_x08_NpWrrLenC_OFFSET                            16
#define D0F0x98_x08_NpWrrLenC_WIDTH                             8
#define D0F0x98_x08_NpWrrLenC_MASK                              0xFF0000
#define D0F0x98_x08_Reserved_31_24_OFFSET                       24
#define D0F0x98_x08_Reserved_31_24_WIDTH                        8
#define D0F0x98_x08_Reserved_31_24_MASK                         0xFF000000

/// D0F0x98_x08
typedef union {
  struct {                                                            ///<
    UINT32                                               NpWrrLenA:8; ///<
    UINT32                                               NpWrrLenB:8; ///<
    UINT32                                               NpWrrLenC:8; ///<
    UINT32                                          Reserved_31_24:8; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x08_STRUCT;

// **** D0F0x98_x0C Register Definition ****
// Address
#define D0F0x98_x0C_ADDRESS                                     0xC
// Type
#define D0F0x98_x0C_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x0C_GcmWrrLenA_OFFSET                           0
#define D0F0x98_x0C_GcmWrrLenA_WIDTH                            8
#define D0F0x98_x0C_GcmWrrLenA_MASK                             0xFF
#define D0F0x98_x0C_GcmWrrLenB_OFFSET                           8
#define D0F0x98_x0C_GcmWrrLenB_WIDTH                            8
#define D0F0x98_x0C_GcmWrrLenB_MASK                             0xFF00
#define D0F0x98_x0C_Reserved_29_16_OFFSET                       16
#define D0F0x98_x0C_Reserved_29_16_WIDTH                        14
#define D0F0x98_x0C_Reserved_29_16_MASK                         0x3FFF0000
#define D0F0x98_x0C_StrictSelWinnerEn_OFFSET                    30
#define D0F0x98_x0C_StrictSelWinnerEn_WIDTH                     1
#define D0F0x98_x0C_StrictSelWinnerEn_MASK                      0x40000000
#define D0F0x98_x0C_Reserved_31_31_OFFSET                       31
#define D0F0x98_x0C_Reserved_31_31_WIDTH                        1
#define D0F0x98_x0C_Reserved_31_31_MASK                         0x80000000

/// D0F0x98_x0C
typedef union {
  struct {                                                            ///<
    UINT32                                              GcmWrrLenA:8; ///<
    UINT32                                              GcmWrrLenB:8; ///<
    UINT32                                          Reserved_29_16:14; ///<
    UINT32                                       StrictSelWinnerEn:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x0C_STRUCT;

// **** D0F0x98_x1E Register Definition ****
// Address
#define D0F0x98_x1E_ADDRESS                                     0x1E
// Type
#define D0F0x98_x1E_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x1E_Reserved_0_0_OFFSET                         0
#define D0F0x98_x1E_Reserved_0_0_WIDTH                          1
#define D0F0x98_x1E_Reserved_0_0_MASK                           0x1
#define D0F0x98_x1E_HiPriEn_OFFSET                              1
#define D0F0x98_x1E_HiPriEn_WIDTH                               1
#define D0F0x98_x1E_HiPriEn_MASK                                0x2
#define D0F0x98_x1E_Reserved_23_2_OFFSET                        2
#define D0F0x98_x1E_Reserved_23_2_WIDTH                         22
#define D0F0x98_x1E_Reserved_23_2_MASK                          0xFFFFFC
#define D0F0x98_x1E_RxErrStatusDelay_OFFSET                     24
#define D0F0x98_x1E_RxErrStatusDelay_WIDTH                      8
#define D0F0x98_x1E_RxErrStatusDelay_MASK                       0xFF000000

/// D0F0x98_x1E
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                                 HiPriEn:1; ///<
    UINT32                                           Reserved_23_2:22; ///<
    UINT32                                        RxErrStatusDelay:8; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x1E_STRUCT;

// **** D0F0x98_x26 Register Definition ****
// Address
#define D0F0x98_x26_ADDRESS                                     0x26

// Type
#define D0F0x98_x26_TYPE                                        TYPE_D0F0x98
// Field Data
#define D0F0x98_x26_IOMMUUrAddr_39_32__OFFSET                   0
#define D0F0x98_x26_IOMMUUrAddr_39_32__WIDTH                    8
#define D0F0x98_x26_IOMMUUrAddr_39_32__MASK                     0xff
#define D0F0x98_x26_Reserved_31_8_OFFSET                        8
#define D0F0x98_x26_Reserved_31_8_WIDTH                         24
#define D0F0x98_x26_Reserved_31_8_MASK                          0xffffff00

/// D0F0x98_x26
typedef union {
  struct {                                                              ///<
    UINT32                                       IOMMUUrAddr_39_32_:8 ; ///<
    UINT32                                            Reserved_31_8:24; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0x98_x26_STRUCT;


// **** D0F0x98_x27 Register Definition ****
// Address
#define D0F0x98_x27_ADDRESS                                     0x27

// Type
#define D0F0x98_x27_TYPE                                        TYPE_D0F0x98
// Field Data
#define D0F0x98_x27_Reserved_5_0_OFFSET                         0
#define D0F0x98_x27_Reserved_5_0_WIDTH                          6
#define D0F0x98_x27_Reserved_5_0_MASK                           0x3f
#define D0F0x98_x27_IOMMUUrAddr_31_6__OFFSET                    6
#define D0F0x98_x27_IOMMUUrAddr_31_6__WIDTH                     26
#define D0F0x98_x27_IOMMUUrAddr_31_6__MASK                      0xffffffc0

/// D0F0x98_x27
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_5_0:6 ; ///<
    UINT32                                        IOMMUUrAddr_31_6_:26; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0x98_x27_STRUCT;

// **** D0F0x98_x28 Register Definition ****
// Address
#define D0F0x98_x28_ADDRESS                                     0x28
// Type
#define D0F0x98_x28_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x28_Reserved_0_0_OFFSET                         0
#define D0F0x98_x28_Reserved_0_0_WIDTH                          1
#define D0F0x98_x28_Reserved_0_0_MASK                           0x1
#define D0F0x98_x28_ForceCoherentIntr_OFFSET                    1
#define D0F0x98_x28_ForceCoherentIntr_WIDTH                     1
#define D0F0x98_x28_ForceCoherentIntr_MASK                      0x2
#define D0F0x98_x28_Reserved_31_2_OFFSET                        2
#define D0F0x98_x28_Reserved_31_2_WIDTH                         30
#define D0F0x98_x28_Reserved_31_2_MASK                          0xFFFFFFFC

/// D0F0x98_x28
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                       ForceCoherentIntr:1; ///<
    UINT32                                           Reserved_31_2:30; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x28_STRUCT;

// **** D0F0x98_x2C Register Definition ****
// Address
#define D0F0x98_x2C_ADDRESS                                     0x2C
// Type
#define D0F0x98_x2C_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x2C_Reserved_0_0_OFFSET                         0
#define D0F0x98_x2C_Reserved_0_0_WIDTH                          1
#define D0F0x98_x2C_Reserved_0_0_MASK                           0x1
#define D0F0x98_x2C_DynWakeEn_OFFSET                            1
#define D0F0x98_x2C_DynWakeEn_WIDTH                             1
#define D0F0x98_x2C_DynWakeEn_MASK                              0x2
#define D0F0x98_x2C_CgttLclkOverride_OFFSET                     2
#define D0F0x98_x2C_CgttLclkOverride_WIDTH                      1
#define D0F0x98_x2C_CgttLclkOverride_MASK                       0x4
#define D0F0x98_x2C_Reserved_8_3_OFFSET                         3
#define D0F0x98_x2C_Reserved_8_3_WIDTH                          6
#define D0F0x98_x2C_Reserved_8_3_MASK                           0x1F8
#define D0F0x98_x2C_SBDmaActiveMask_OFFSET                      9
#define D0F0x98_x2C_SBDmaActiveMask_WIDTH                       1
#define D0F0x98_x2C_SBDmaActiveMask_MASK                        0x200
#define D0F0x98_x2C_Reserved_15_10_OFFSET                       10
#define D0F0x98_x2C_Reserved_15_10_WIDTH                        6
#define D0F0x98_x2C_Reserved_15_10_MASK                         0xFC00
#define D0F0x98_x2C_WakeHysteresis_OFFSET                       16
#define D0F0x98_x2C_WakeHysteresis_WIDTH                        16
#define D0F0x98_x2C_WakeHysteresis_MASK                         0xFFFF0000

/// D0F0x98_x2C
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                               DynWakeEn:1; ///<
    UINT32                                        CgttLclkOverride:1; ///<
    UINT32                                            Reserved_8_3:6; ///<
    UINT32                                         SBDmaActiveMask:1; ///<
    UINT32                                          Reserved_15_10:6; ///<
    UINT32                                          WakeHysteresis:16; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x2C_STRUCT;

// **** D0F0x98_x3A Register Definition ****
// Address
#define D0F0x98_x3A_ADDRESS                                     0x3A
// Type
#define D0F0x98_x3A_TYPE                                        TYPE_D0F0x98

// **** D0F0x98_x49 Register Definition ****
// Address
#define D0F0x98_x49_ADDRESS                                     0x49
// Type
#define D0F0x98_x49_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x49_Reserved_23_0_OFFSET                        0
#define D0F0x98_x49_Reserved_23_0_WIDTH                         24
#define D0F0x98_x49_Reserved_23_0_MASK                          0xFFFFFF
#define D0F0x98_x49_SoftOverrideClk6_OFFSET                     24
#define D0F0x98_x49_SoftOverrideClk6_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk6_MASK                       0x1000000
#define D0F0x98_x49_SoftOverrideClk5_OFFSET                     25
#define D0F0x98_x49_SoftOverrideClk5_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk5_MASK                       0x2000000
#define D0F0x98_x49_SoftOverrideClk4_OFFSET                     26
#define D0F0x98_x49_SoftOverrideClk4_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk4_MASK                       0x4000000
#define D0F0x98_x49_SoftOverrideClk3_OFFSET                     27
#define D0F0x98_x49_SoftOverrideClk3_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk3_MASK                       0x8000000
#define D0F0x98_x49_SoftOverrideClk2_OFFSET                     28
#define D0F0x98_x49_SoftOverrideClk2_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk2_MASK                       0x10000000
#define D0F0x98_x49_SoftOverrideClk1_OFFSET                     29
#define D0F0x98_x49_SoftOverrideClk1_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk1_MASK                       0x20000000
#define D0F0x98_x49_SoftOverrideClk0_OFFSET                     30
#define D0F0x98_x49_SoftOverrideClk0_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk0_MASK                       0x40000000
#define D0F0x98_x49_Reserved_31_31_OFFSET                       31
#define D0F0x98_x49_Reserved_31_31_WIDTH                        1
#define D0F0x98_x49_Reserved_31_31_MASK                         0x80000000

/// D0F0x98_x49
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_23_0:24; ///<
    UINT32                                        SoftOverrideClk6:1; ///<
    UINT32                                        SoftOverrideClk5:1; ///<
    UINT32                                        SoftOverrideClk4:1; ///<
    UINT32                                        SoftOverrideClk3:1; ///<
    UINT32                                        SoftOverrideClk2:1; ///<
    UINT32                                        SoftOverrideClk1:1; ///<
    UINT32                                        SoftOverrideClk0:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x49_STRUCT;

// **** D0F0x98_x4A Register Definition ****
// Address
#define D0F0x98_x4A_ADDRESS                                     0x4A
// Type
#define D0F0x98_x4A_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x4A_Reserved_23_0_OFFSET                        0
#define D0F0x98_x4A_Reserved_23_0_WIDTH                         24
#define D0F0x98_x4A_Reserved_23_0_MASK                          0xFFFFFF
#define D0F0x98_x4A_SoftOverrideClk6_OFFSET                     24
#define D0F0x98_x4A_SoftOverrideClk6_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk6_MASK                       0x1000000
#define D0F0x98_x4A_SoftOverrideClk5_OFFSET                     25
#define D0F0x98_x4A_SoftOverrideClk5_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk5_MASK                       0x2000000
#define D0F0x98_x4A_SoftOverrideClk4_OFFSET                     26
#define D0F0x98_x4A_SoftOverrideClk4_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk4_MASK                       0x4000000
#define D0F0x98_x4A_SoftOverrideClk3_OFFSET                     27
#define D0F0x98_x4A_SoftOverrideClk3_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk3_MASK                       0x8000000
#define D0F0x98_x4A_SoftOverrideClk2_OFFSET                     28
#define D0F0x98_x4A_SoftOverrideClk2_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk2_MASK                       0x10000000
#define D0F0x98_x4A_SoftOverrideClk1_OFFSET                     29
#define D0F0x98_x4A_SoftOverrideClk1_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk1_MASK                       0x20000000
#define D0F0x98_x4A_SoftOverrideClk0_OFFSET                     30
#define D0F0x98_x4A_SoftOverrideClk0_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk0_MASK                       0x40000000
#define D0F0x98_x4A_Reserved_31_31_OFFSET                       31
#define D0F0x98_x4A_Reserved_31_31_WIDTH                        1
#define D0F0x98_x4A_Reserved_31_31_MASK                         0x80000000

/// D0F0x98_x4A
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_23_0:24; ///<
    UINT32                                        SoftOverrideClk6:1; ///<
    UINT32                                        SoftOverrideClk5:1; ///<
    UINT32                                        SoftOverrideClk4:1; ///<
    UINT32                                        SoftOverrideClk3:1; ///<
    UINT32                                        SoftOverrideClk2:1; ///<
    UINT32                                        SoftOverrideClk1:1; ///<
    UINT32                                        SoftOverrideClk0:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x4A_STRUCT;

// **** D0F0xBC_x20054 Register Definition ****
// Address
#define D0F0xBC_x20054_ADDRESS                                  0x20054

// Type
#define D0F0xBC_x20054_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x20054_BiosTableAddr_OFFSET                    0
#define D0F0xBC_x20054_BiosTableAddr_WIDTH                     32
#define D0F0xBC_x20054_BiosTableAddr_MASK                      0xFFFFFFFF

/// D0F0xBC_x3F9F4
typedef union {
  struct {                                                              ///<
    UINT32                                           BiosTableAddr:32 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x20054_STRUCT;

// **** D0F0xBC_x3F818 Register Definition ****
// Address
#define D0F0xBC_x3F818_ADDRESS                                  0x3f818

// Type
#define D0F0xBC_x3F818_TYPE                                     TYPE_D0F0xBC

/// D0F0xBC_x3F818
typedef union {
  struct {                                                              ///<
    UINT32                                                     word0:8; ///<
    UINT32                                                     word1:8; ///<
    UINT32                                                     word2:8; ///<
    UINT32                                                     word3:8; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3F818_STRUCT;

// **** D0F0xBC_x3F81C Register Definition ****
// Address
#define D0F0xBC_x3F81C_ADDRESS                                  0x3f81c

// Type
#define D0F0xBC_x3F81C_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3F81C_Reserved_7_0_OFFSET                   0
#define D0F0xBC_x3F81C_Reserved_7_0_WIDTH                    8
#define D0F0xBC_x3F81C_Reserved_7_0_MASK                     0x00ff
#define D0F0xBC_x3F81C_RECONF_WRAPPER_OFFSET                 8
#define D0F0xBC_x3F81C_RECONF_WRAPPER_WIDTH                  8
#define D0F0xBC_x3F81C_RECONF_WRAPPER_MASK                   0xff00
#define D0F0xBC_x3F81C_Reserved_31_16_OFFSET                 16
#define D0F0xBC_x3F81C_Reserved_31_16_WIDTH                  16
#define D0F0xBC_x3F81C_Reserved_31_16_MASK                   0xffff0000

/// D0F0xBC_x3F81C
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved_7_0:8; ///<
    UINT32                                            RECONF_WRAPPER:8; ///<
    UINT32                                           Reserved_31_16:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3F81C_STRUCT;

// **** D0F0xBC_x3F820 Register Definition ****
// Address
#define D0F0xBC_x3F820_ADDRESS                                  0x3f820

// Type
#define D0F0xBC_x3F820_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3F820_LclkDpm_OFFSET                           0
#define D0F0xBC_x3F820_LclkDpm_WIDTH                            8
#define D0F0xBC_x3F820_LclkDpm_MASK                             0xff
#define D0F0xBC_x3F820_ThermalCntl_OFFSET                       8
#define D0F0xBC_x3F820_ThermalCntl_WIDTH                        8
#define D0F0xBC_x3F820_ThermalCntl_MASK                         0xff00
#define D0F0xBC_x3F820_VoltageCntl_OFFSET                       16
#define D0F0xBC_x3F820_VoltageCntl_WIDTH                        8
#define D0F0xBC_x3F820_VoltageCntl_MASK                         0xff0000
#define D0F0xBC_x3F820_Loadline_OFFSET                          24
#define D0F0xBC_x3F820_Loadline_WIDTH                           8
#define D0F0xBC_x3F820_Loadline_MASK                            0xff000000

/// D0F0xBC_x3F820
typedef union {
  struct {                                                              ///<
    UINT32                                                  LclkDpm:8 ; ///<
    UINT32                                              ThermalCntl:8 ; ///<
    UINT32                                              VoltageCntl:8 ; ///<
    UINT32                                                 Loadline:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3F820_STRUCT;

// **** D0F0xBC_x3F830 Register Definition ****
// Address
#define D0F0xBC_x3F830_ADDRESS                                  0x3F830

// Type
#define D0F0xBC_x3F830_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3F830_TimerPeriod_OFFSET                       0
#define D0F0xBC_x3F830_TimerPeriod_WIDTH                        32
#define D0F0xBC_x3F830_TimerPeriod_MASK                         0xffffffff

/// D0F0xBC_x3F830
typedef union {
  struct {                                                              ///<
    UINT32                                              TimerPeriod:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3F830_STRUCT;

// **** D0F0xBC_x3F994 Register Definition ****
// Address
#define D0F0xBC_x3F994_ADDRESS                                  0x3f994

// Type
#define D0F0xBC_x3F994_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3F994_IddSpikeOCP_VDD_OFFSET                   0
#define D0F0xBC_x3F994_IddSpikeOCP_VDD_WIDTH                    16
#define D0F0xBC_x3F994_IddSpikeOCP_VDD_MASK                     0xffff
#define D0F0xBC_x3F994_IddSpikeOCP_VDDNB_OFFSET                 16
#define D0F0xBC_x3F994_IddSpikeOCP_VDDNB_WIDTH                  16
#define D0F0xBC_x3F994_IddSpikeOCP_VDDNB_MASK                   0xffff0000

/// D0F0xBC_x3F994
typedef union {
  struct {                                                              ///<
    UINT32                                          IddSpikeOCP_VDD:16; ///<
    UINT32                                        IddSpikeOCP_VDDNB:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3F994_STRUCT;

// **** D0F0xBC_x3F9E8 Register Definition ****
// Address
#define D0F0xBC_x3F9E8_ADDRESS                                  0x3F9E8
// Type
#define D0F0xBC_x3F9E8_TYPE                                     TYPE_D0F0xBC

// Field Data
#define D0F0xBC_x3F9E8_Dpm0PgNbPsLo_OFFSET                      0
#define D0F0xBC_x3F9E8_Dpm0PgNbPsLo_WIDTH                       8
#define D0F0xBC_x3F9E8_Dpm0PgNbPsLo_MASK                        0xFF
#define D0F0xBC_x3F9E8_Dpm0PgNbPsHi_OFFSET                      8
#define D0F0xBC_x3F9E8_Dpm0PgNbPsHi_WIDTH                       8
#define D0F0xBC_x3F9E8_Dpm0PgNbPsHi_MASK                        0xFF00
#define D0F0xBC_x3F9E8_DpmXNbPsLo_OFFSET                        16
#define D0F0xBC_x3F9E8_DpmXNbPsLo_WIDTH                         8
#define D0F0xBC_x3F9E8_DpmXNbPsLo_MASK                          0xFF0000
#define D0F0xBC_x3F9E8_DpmXNbPsHi_OFFSET                        24
#define D0F0xBC_x3F9E8_DpmXNbPsHi_WIDTH                         8
#define D0F0xBC_x3F9E8_DpmXNbPsHi_MASK                          0xFF000000

/// D0F0xBC_x3F9E8
typedef union {
  struct {                                                            ///<
    UINT32                                            Dpm0PgNbPsLo:8; ///<
    UINT32                                            Dpm0PgNbPsHi:8; ///<
    UINT32                                              DpmXNbPsLo:8; ///<
    UINT32                                              DpmXNbPsHi:8; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_x3F9E8_STRUCT;

// **** D0F0xBC_x3F9F4 Register Definition ****
// Address
#define D0F0xBC_x3F9F4_ADDRESS                                  0x3F9F4
// Type
#define D0F0xBC_x3F9F4_TYPE                                     TYPE_D0F0xBC

// Field Data
#define D0F0xBC_x3F9F4_SviInitLoadLineVdd_OFFSET                0
#define D0F0xBC_x3F9F4_SviInitLoadLineVdd_WIDTH                 8
#define D0F0xBC_x3F9F4_SviInitLoadLineVdd_MASK                  0xFF
#define D0F0xBC_x3F9F4_SviInitLoadLineVddNB_OFFSET              8
#define D0F0xBC_x3F9F4_SviInitLoadLineVddNB_WIDTH               8
#define D0F0xBC_x3F9F4_SviInitLoadLineVddNB_MASK                0xFF00
#define D0F0xBC_x3F9F4_SviTrimValueVdd_OFFSET                   16
#define D0F0xBC_x3F9F4_SviTrimValueVdd_WIDTH                    8
#define D0F0xBC_x3F9F4_SviTrimValueVdd_MASK                     0xFF0000
#define D0F0xBC_x3F9F4_SviTrimValueVddNB_OFFSET                 24
#define D0F0xBC_x3F9F4_SviTrimValueVddNB_WIDTH                  8
#define D0F0xBC_x3F9F4_SviTrimValueVddNB_MASK                   0xFF000000

/// D0F0xBC_x3F9F4
typedef union {
  struct {                                                            ///<
    UINT32                                      SviInitLoadLineVdd:8; ///<
    UINT32                                    SviInitLoadLineVddNB:8; ///<
    UINT32                                         SviTrimValueVdd:8; ///<
    UINT32                                       SviTrimValueVddNB:8; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_x3F9F4_STRUCT;

// **** D0F0xBC_x3F9F8 Register Definition ****
// Address
#define D0F0xBC_x3F9F8_ADDRESS                                  0x3F9F8
// Type
#define D0F0xBC_x3F9F8_TYPE                                     TYPE_D0F0xBC

// Field Data
#define D0F0xBC_x3F9F8_SviLoadLineOffsetVdd_OFFSET              0
#define D0F0xBC_x3F9F8_SviLoadLineOffsetVdd_WIDTH               8
#define D0F0xBC_x3F9F8_SviLoadLineOffsetVdd_MASK                0xFF
#define D0F0xBC_x3F9F8_SviLoadLineOffsetVddNB_OFFSET            8
#define D0F0xBC_x3F9F8_SviLoadLineOffsetVddNB_WIDTH             8
#define D0F0xBC_x3F9F8_SviLoadLineOffsetVddNB_MASK              0xFF00
#define D0F0xBC_x3F9F8_Reserved_31_16_OFFSET                    16
#define D0F0xBC_x3F9F8_Reserved_31_16_WIDTH                     16
#define D0F0xBC_x3F9F8_Reserved_31_16_MASK                      0xFFFF0000

/// D0F0xBC_x3F9F8
typedef union {
  struct {                                                            ///<
    UINT32                                    SviLoadLineOffsetVdd:8; ///<
    UINT32                                  SviLoadLineOffsetVddNB:8; ///<
    UINT32                                          Reserved_31_16:16; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_x3F9F8_STRUCT;

// **** D0F0xBC_x3F98C Register Definition ****
// Address
#define D0F0xBC_x3F98C_ADDRESS                                  0x3F98C

// Type
#define D0F0xBC_x3F98C_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3F98C_ThermalSensorCorrection_OFFSET           0
#define D0F0xBC_x3F98C_ThermalSensorCorrection_WIDTH            8
#define D0F0xBC_x3F98C_ThermalSensorCorrection_MASK             0xFF
#define D0F0xBC_x3F98C_Mantissa_OFFSET                          8
#define D0F0xBC_x3F98C_Mantissa_WIDTH                           5
#define D0F0xBC_x3F98C_Mantissa_MASK                            0x1F00
#define D0F0xBC_x3F98C_Exponent_OFFSET                          13
#define D0F0xBC_x3F98C_Exponent_WIDTH                           5
#define D0F0xBC_x3F98C_Exponent_MASK                            0x3E000
#define D0F0xBC_x3F98C_Reserved_31_18_OFFSET                    18
#define D0F0xBC_x3F98C_Reserved_31_18_WIDTH                     14
#define D0F0xBC_x3F98C_Reserved_31_18_MASK                      0xFFFC0000

/// D0F0xBC_x3F98C
typedef union {
  struct {                                                              ///<
    UINT32                                  ThermalSensorCorrection:8 ; ///<
    UINT32                                                 Mantissa:5 ; ///<
    UINT32                                                 Exponent:5 ; ///<
    UINT32                                           Reserved_31_18:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3F98C_STRUCT;

// **** D0F0xBC_x3F9D8 Register Definition ****
// Address
#define D0F0xBC_x3F9D8_ADDRESS                                  0x3F9D8
// Type
#define D0F0xBC_x3F9D8_TYPE                                     TYPE_D0F0xBC

// Field Data
#define D0F0xBC_x3F9D8_EnableVpcAccumulators_OFFSET             0
#define D0F0xBC_x3F9D8_EnableVpcAccumulators_WIDTH              1
#define D0F0xBC_x3F9D8_EnableVpcAccumulators_MASK               0x1
#define D0F0xBC_x3F9D8_EnableBapm_OFFSET                        1
#define D0F0xBC_x3F9D8_EnableBapm_WIDTH                         1
#define D0F0xBC_x3F9D8_EnableBapm_MASK                          0x2
#define D0F0xBC_x3F9D8_EnableTdcLimit_OFFSET                    2
#define D0F0xBC_x3F9D8_EnableTdcLimit_WIDTH                     1
#define D0F0xBC_x3F9D8_EnableTdcLimit_MASK                      0x4
#define D0F0xBC_x3F9D8_EnableLpmx_OFFSET                        3
#define D0F0xBC_x3F9D8_EnableLpmx_WIDTH                         1
#define D0F0xBC_x3F9D8_EnableLpmx_MASK                          0x8
#define D0F0xBC_x3F9D8_Reserved_4_4_OFFSET                      4
#define D0F0xBC_x3F9D8_Reserved_4_4_WIDTH                       1
#define D0F0xBC_x3F9D8_Reserved_4_4_MASK                        0x10
#define D0F0xBC_x3F9D8_EnableNbDpm_OFFSET                       5
#define D0F0xBC_x3F9D8_EnableNbDpm_WIDTH                        1
#define D0F0xBC_x3F9D8_EnableNbDpm_MASK                         0x20
#define D0F0xBC_x3F9D8_Reserved_23_6_OFFSET                     6
#define D0F0xBC_x3F9D8_Reserved_23_6_WIDTH                      18
#define D0F0xBC_x3F9D8_Reserved_23_6_MASK                       0xFFFFC0
#define D0F0xBC_x3F9D8_PstateAllCpusIdle_OFFSET                 24
#define D0F0xBC_x3F9D8_PstateAllCpusIdle_WIDTH                  3
#define D0F0xBC_x3F9D8_PstateAllCpusIdle_MASK                   0x7000000
#define D0F0xBC_x3F9D8_NbPstateAllCpusIdle_OFFSET               27
#define D0F0xBC_x3F9D8_NbPstateAllCpusIdle_WIDTH                1
#define D0F0xBC_x3F9D8_NbPstateAllCpusIdle_MASK                 0x8000000
#define D0F0xBC_x3F9D8_BapmCoeffOverride_OFFSET                 28
#define D0F0xBC_x3F9D8_BapmCoeffOverride_WIDTH                  1
#define D0F0xBC_x3F9D8_BapmCoeffOverride_MASK                   0x10000000
#define D0F0xBC_x3F9D8_SviMode_OFFSET                           29
#define D0F0xBC_x3F9D8_SviMode_WIDTH                            1
#define D0F0xBC_x3F9D8_SviMode_MASK                             0x20000000
#define D0F0xBC_x3F9D8_Reserved_31_30_OFFSET                    30
#define D0F0xBC_x3F9D8_Reserved_31_30_WIDTH                     2
#define D0F0xBC_x3F9D8_Reserved_31_30_MASK                      0xC0000000

/// D0F0xBC_x3F9D8
typedef union {
  struct {                                                            ///<
    UINT32                                   EnableVpcAccumulators:1; ///<
    UINT32                                              EnableBapm:1; ///<
    UINT32                                          EnableTdcLimit:1; ///<
    UINT32                                              EnableLpmx:1; ///<
    UINT32                                            Reserved_4_4:1; ///<
    UINT32                                             EnableNbDpm:1; ///<
    UINT32                                           Reserved_23_6:18; ///<
    UINT32                                       PstateAllCpusIdle:3; ///<
    UINT32                                     NbPstateAllCpusIdle:1; ///<
    UINT32                                       BapmCoeffOverride:1; ///<
    UINT32                                                 SviMode:1; ///<
    UINT32                                          Reserved_31_30:2; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_x3F9D8_STRUCT;

// **** D0F0xBC_x3FA04 Register Definition ****
// Address
#define D0F0xBC_x3FA04_ADDRESS                                  0x3FA04
// Type
#define D0F0xBC_x3FA04_TYPE                                     TYPE_D0F0xBC

// Field Data
#define D0F0xBC_x3FA04_Bitfield_15_0_OFFSET                     0
#define D0F0xBC_x3FA04_Bitfield_15_0_WIDTH                      16
#define D0F0xBC_x3FA04_Bitfield_15_0_MASK                       0xFFFF
#define D0F0xBC_x3FA04_Bitfield_31_16_OFFSET                    16
#define D0F0xBC_x3FA04_Bitfield_31_16_WIDTH                     16
#define D0F0xBC_x3FA04_Bitfield_31_16_MASK                      0xFFFF0000

// **** D0F0xBC_x3F844 Register Definition ****
// Address
#define D0F0xBC_x3F844_ADDRESS                                  0x3f844

//Type
#define D0F0xBC_x3F844_TYPE                                     TYPE_D0F0xBC

// Field Data
#define D0F0xBC_x3F844_CpuGpuPerfPri_OFFSET                     0
#define D0F0xBC_x3F844_CpuGpuPerfPri_WIDTH                      1
#define D0F0xBC_x3F844_CpuGpuPerfPri_MASK                       0x1
#define D0F0xBC_x3F844_PkgPwrLimit_OFFSET                       1
#define D0F0xBC_x3F844_PkgPwrLimit_WIDTH                        15
#define D0F0xBC_x3F844_PkgPwrLimit_MASK                         0x1fe00
#define D0F0xBC_x3F844_FchPwrCreditScale_OFFSET                 17
#define D0F0xBC_x3F844_FchPwrCreditScale_WIDTH                  6
#define D0F0xBC_x3F844_FchPwrCreditScale_MASK                   0x7e0000
#define D0F0xBC_x3F844_PkgHystCoeff_OFFSET                      23
#define D0F0xBC_x3F844_PkgHystCoeff_WIDTH                       6
#define D0F0xBC_x3F844_PkgHystCoeff_MASK                        0x1f800000
#define D0F0xBC_x3F844_Reserved_31_29_OFFSET                    29
#define D0F0xBC_x3F844_Reserved_31_29_WIDTH                     3
#define D0F0xBC_x3F844_Reserved_31_29_MASK                      0xe0000000

/// D0F0xBC_x3F844
typedef union {
  struct {                                                             ///<
    UINT32                                           CpuGpuPerfPri:1 ; ///<
    UINT32                                             PkgPwrLimit:15; ///<
    UINT32                                       FchPwrCreditScale:6 ; ///<
    UINT32                                            PkgHystCoeff:6 ; ///<
    UINT32                                          Reserved_31_29:3 ; ///<
  } Field;                                                             ///<
  UINT32 Value;                                                        ///<
} D0F0xBC_x3F844_STRUCT;

// **** D0F0xBC_x3FD00 Register Definition ****
// Address
#define D0F0xBC_x3FD00_ADDRESS                                  0x3fd00

// Type
#define D0F0xBC_x3FD00_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD00_LowVoltageReqThreshold_OFFSET            0
#define D0F0xBC_x3FD00_LowVoltageReqThreshold_WIDTH             8
#define D0F0xBC_x3FD00_LowVoltageReqThreshold_MASK              0xff
#define D0F0xBC_x3FD00_VID_OFFSET                               8
#define D0F0xBC_x3FD00_VID_WIDTH                                8
#define D0F0xBC_x3FD00_VID_MASK                                 0xff00
#define D0F0xBC_x3FD00_LclkDivider_OFFSET                       16
#define D0F0xBC_x3FD00_LclkDivider_WIDTH                        8
#define D0F0xBC_x3FD00_LclkDivider_MASK                         0xff0000
#define D0F0xBC_x3FD00_StateValid_OFFSET                        24
#define D0F0xBC_x3FD00_StateValid_WIDTH                         8
#define D0F0xBC_x3FD00_StateValid_MASK                          0xff000000

/// D0F0xBC_x3FD00
typedef union {
  struct {                                                              ///<
    UINT32                                   LowVoltageReqThreshold:8 ; ///<
    UINT32                                                      VID:8 ; ///<
    UINT32                                              LclkDivider:8 ; ///<
    UINT32                                               StateValid:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD00_STRUCT;

// **** D0F0xBC_x3FD04 Register Definition ****
// Address
#define D0F0xBC_x3FD04_ADDRESS                                  0x3fd04

/// D0F0xBC_x3FD04
typedef union {
  struct {                                                              ///<
    UINT32                                                    byte0:8 ; ///<
    UINT32                                                    byte1:8 ; ///<
    UINT32                                                    byte2:8 ; ///<
    UINT32                                                    byte3:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD04_STRUCT;

// Type
#define D0F0xBC_x3FD04_TYPE                                     TYPE_D0F0xBC

// **** D0F0xBC_x3FD08 Register Definition ****
// Address
#define D0F0xBC_x3FD08_ADDRESS                                  0x3fd08

// Type
#define D0F0xBC_x3FD08_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD08_HysteresisDown_OFFSET                    0
#define D0F0xBC_x3FD08_HysteresisDown_WIDTH                     8
#define D0F0xBC_x3FD08_HysteresisDown_MASK                      0xff
#define D0F0xBC_x3FD08_HysteresisUp_OFFSET                      8
#define D0F0xBC_x3FD08_HysteresisUp_WIDTH                       8
#define D0F0xBC_x3FD08_HysteresisUp_MASK                        0xff00
#define D0F0xBC_x3FD08_ResidencyCounter_OFFSET                  16
#define D0F0xBC_x3FD08_ResidencyCounter_WIDTH                   16
#define D0F0xBC_x3FD08_ResidencyCounter_MASK                    0xffff0000

/// D0F0xBC_x3FD08
typedef union {
  struct {                                                              ///<
    UINT32                                           HysteresisDown:8 ; ///<
    UINT32                                             HysteresisUp:8 ; ///<
    UINT32                                         ResidencyCounter:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD08_STRUCT;

// **** D0F0xBC_x3FD0C Register Definition ****
// Address
#define D0F0xBC_x3FD0C_ADDRESS                                  0x3fd0c

// Type
#define D0F0xBC_x3FD0C_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD0C_LCLK_FREQUENCY_OFFSET                    0
#define D0F0xBC_x3FD0C_LCLK_FREQUENCY_WIDTH                     32
#define D0F0xBC_x3FD0C_LCLK_FREQUENCY_MASK                      0xffffffff

/// D0F0xBC_x3FD0C
typedef union {
  struct {                                                              ///<
    UINT32                                           LCLK_FREQUENCY:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD0C_STRUCT;

// **** D0F0xBC_x3FD10 Register Definition ****
// Address
#define D0F0xBC_x3FD10_ADDRESS                                  0x3fd10

// Type
#define D0F0xBC_x3FD10_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD10_Reserved_7_0_OFFSET                      0
#define D0F0xBC_x3FD10_Reserved_7_0_WIDTH                       8
#define D0F0xBC_x3FD10_Reserved_7_0_MASK                        0xff
#define D0F0xBC_x3FD10_Reserved_15_8_OFFSET                     8
#define D0F0xBC_x3FD10_Reserved_15_8_WIDTH                      8
#define D0F0xBC_x3FD10_Reserved_15_8_MASK                       0xff00
#define D0F0xBC_x3FD10_EnabledForThrottle_OFFSET                16
#define D0F0xBC_x3FD10_EnabledForThrottle_WIDTH                 8
#define D0F0xBC_x3FD10_EnabledForThrottle_MASK                  0xff0000
#define D0F0xBC_x3FD10_ActivityThreshold_OFFSET                 24
#define D0F0xBC_x3FD10_ActivityThreshold_WIDTH                  8
#define D0F0xBC_x3FD10_ActivityThreshold_MASK                   0xff000000

/// D0F0xBC_x3FD10
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_7_0:8 ; ///<
    UINT32                                            Reserved_15_8:8 ; ///<
    UINT32                                       EnabledForThrottle:8 ; ///<
    UINT32                                        ActivityThreshold:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD10_STRUCT;

// **** D0F0xBC_x3FD14 Register Definition ****
// Address
#define D0F0xBC_x3FD14_ADDRESS                                  0x3fd14

// Type
#define D0F0xBC_x3FD14_TYPE                                     TYPE_D0F0xBC

// **** D0F0xBC_x3FD1C Register Definition ****
// Address
#define D0F0xBC_x3FD1C_ADDRESS                                  0x3fd1c

// Type
#define D0F0xBC_x3FD1C_TYPE                                     TYPE_D0F0xBC

// **** D0F0xBC_x3FD20 Register Definition ****
// Address
#define D0F0xBC_x3FD20_ADDRESS                                  0x3fd20

// Type
#define D0F0xBC_x3FD20_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD20_LCLK_FREQUENCY_OFFSET                    0
#define D0F0xBC_x3FD20_LCLK_FREQUENCY_WIDTH                     32
#define D0F0xBC_x3FD20_LCLK_FREQUENCY_MASK                      0xffffffff

/// D0F0xBC_x3FD20
typedef union {
  struct {                                                              ///<
    UINT32                                           LCLK_FREQUENCY:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD20_STRUCT;

// **** D0F0xBC_x3FD24 Register Definition ****
// Address
#define D0F0xBC_x3FD24_ADDRESS                                  0x3fd24

// Type
#define D0F0xBC_x3FD24_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD24_Reserved_15_0_OFFSET                     0
#define D0F0xBC_x3FD24_Reserved_15_0_WIDTH                      16
#define D0F0xBC_x3FD24_Reserved_15_0_MASK                       0xffff
#define D0F0xBC_x3FD24_EnabledForThrottle_OFFSET                16
#define D0F0xBC_x3FD24_EnabledForThrottle_WIDTH                 8
#define D0F0xBC_x3FD24_EnabledForThrottle_MASK                  0xff0000
#define D0F0xBC_x3FD24_ActivityThreshold_OFFSET                 24
#define D0F0xBC_x3FD24_ActivityThreshold_WIDTH                  8
#define D0F0xBC_x3FD24_ActivityThreshold_MASK                   0xff000000

/// D0F0xBC_x3FD24
typedef union {
  struct {                                                              ///<
    UINT32                                            Reserved_15_0:16; ///<
    UINT32                                       EnabledForThrottle:8 ; ///<
    UINT32                                        ActivityThreshold:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD24_STRUCT;

// **** D0F0xBC_x3FD28 Register Definition ****
// Address
#define D0F0xBC_x3FD28_ADDRESS                                  0x3fd28

// Type
#define D0F0xBC_x3FD28_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD28_LowVoltageReqThreshold_OFFSET            0
#define D0F0xBC_x3FD28_LowVoltageReqThreshold_WIDTH             8
#define D0F0xBC_x3FD28_LowVoltageReqThreshold_MASK              0xff
#define D0F0xBC_x3FD28_VID_OFFSET                               8
#define D0F0xBC_x3FD28_VID_WIDTH                                8
#define D0F0xBC_x3FD28_VID_MASK                                 0xff00
#define D0F0xBC_x3FD28_LclkDivider_OFFSET                       16
#define D0F0xBC_x3FD28_LclkDivider_WIDTH                        8
#define D0F0xBC_x3FD28_LclkDivider_MASK                         0xff0000
#define D0F0xBC_x3FD28_StateValid_OFFSET                        24
#define D0F0xBC_x3FD28_StateValid_WIDTH                         8
#define D0F0xBC_x3FD28_StateValid_MASK                          0xff000000

/// D0F0xBC_x3FD28
typedef union {
  struct {                                                              ///<
    UINT32                                   LowVoltageReqThreshold:8 ; ///<
    UINT32                                                      VID:8 ; ///<
    UINT32                                              LclkDivider:8 ; ///<
    UINT32                                               StateValid:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD28_STRUCT;

// **** D0F0xBC_x3FD2C Register Definition ****
// Address
#define D0F0xBC_x3FD2C_ADDRESS                                  0x3fd2c

// Type
#define D0F0xBC_x3FD2C_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD2C_MIN_VDDNB_OFFSET                         0
#define D0F0xBC_x3FD2C_MIN_VDDNB_WIDTH                          32
#define D0F0xBC_x3FD2C_MIN_VDDNB_MASK                           0xffffffff

/// D0F0xBC_x3FD2C
typedef union {
  struct {                                                              ///<
    UINT32                                                MIN_VDDNB:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD2C_STRUCT;

// **** D0F0xBC_x3FD30 Register Definition ****
// Address
#define D0F0xBC_x3FD30_ADDRESS                                  0x3fd30

// Type
#define D0F0xBC_x3FD30_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD30_HysteresisDown_OFFSET                    0
#define D0F0xBC_x3FD30_HysteresisDown_WIDTH                     8
#define D0F0xBC_x3FD30_HysteresisDown_MASK                      0xff
#define D0F0xBC_x3FD30_HysteresisUp_OFFSET                      8
#define D0F0xBC_x3FD30_HysteresisUp_WIDTH                       8
#define D0F0xBC_x3FD30_HysteresisUp_MASK                        0xff00
#define D0F0xBC_x3FD30_ResidencyCounter_OFFSET                  16
#define D0F0xBC_x3FD30_ResidencyCounter_WIDTH                   16
#define D0F0xBC_x3FD30_ResidencyCounter_MASK                    0xffff0000

/// D0F0xBC_x3FD30
typedef union {
  struct {                                                              ///<
    UINT32                                           HysteresisDown:8 ; ///<
    UINT32                                             HysteresisUp:8 ; ///<
    UINT32                                         ResidencyCounter:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD30_STRUCT;

// **** D0F0xBC_x3FD34 Register Definition ****
// Address
#define D0F0xBC_x3FD34_ADDRESS                                  0x3fd34

// Type
#define D0F0xBC_x3FD34_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD34_LCLK_FREQUENCY_OFFSET                    0
#define D0F0xBC_x3FD34_LCLK_FREQUENCY_WIDTH                     32
#define D0F0xBC_x3FD34_LCLK_FREQUENCY_MASK                      0xffffffff

/// D0F0xBC_x3FD34
typedef union {
  struct {                                                              ///<
    UINT32                                           LCLK_FREQUENCY:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD34_STRUCT;

// **** D0F0xBC_x3FD38 Register Definition ****
// Address
#define D0F0xBC_x3FD38_ADDRESS                                  0x3fd38

// Type
#define D0F0xBC_x3FD38_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD38_Reserved_15_0_OFFSET                     0
#define D0F0xBC_x3FD38_Reserved_15_0_WIDTH                      16
#define D0F0xBC_x3FD38_Reserved_15_0_MASK                       0xffff
#define D0F0xBC_x3FD38_EnabledForThrottle_OFFSET                16
#define D0F0xBC_x3FD38_EnabledForThrottle_WIDTH                 8
#define D0F0xBC_x3FD38_EnabledForThrottle_MASK                  0xff0000
#define D0F0xBC_x3FD38_ActivityThreshold_OFFSET                 24
#define D0F0xBC_x3FD38_ActivityThreshold_WIDTH                  8
#define D0F0xBC_x3FD38_ActivityThreshold_MASK                   0xff000000

/// D0F0xBC_x3FD38
typedef union {
  struct {                                                              ///<
    UINT32                                            Reserved_15_0:16; ///<
    UINT32                                       EnabledForThrottle:8 ; ///<
    UINT32                                        ActivityThreshold:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD38_STRUCT;

// **** D0F0xBC_x3FD3C Register Definition ****
// Address
#define D0F0xBC_x3FD3C_ADDRESS                                  0x3fd3c

// Type
#define D0F0xBC_x3FD3C_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD3C_LowVoltageReqThreshold_OFFSET            0
#define D0F0xBC_x3FD3C_LowVoltageReqThreshold_WIDTH             8
#define D0F0xBC_x3FD3C_LowVoltageReqThreshold_MASK              0xff
#define D0F0xBC_x3FD3C_VID_OFFSET                               8
#define D0F0xBC_x3FD3C_VID_WIDTH                                8
#define D0F0xBC_x3FD3C_VID_MASK                                 0xff00
#define D0F0xBC_x3FD3C_LclkDivider_OFFSET                       16
#define D0F0xBC_x3FD3C_LclkDivider_WIDTH                        8
#define D0F0xBC_x3FD3C_LclkDivider_MASK                         0xff0000
#define D0F0xBC_x3FD3C_StateValid_OFFSET                        24
#define D0F0xBC_x3FD3C_StateValid_WIDTH                         8
#define D0F0xBC_x3FD3C_StateValid_MASK                          0xff000000

/// D0F0xBC_x3FD3C
typedef union {
  struct {                                                              ///<
    UINT32                                   LowVoltageReqThreshold:8 ; ///<
    UINT32                                                      VID:8 ; ///<
    UINT32                                              LclkDivider:8 ; ///<
    UINT32                                               StateValid:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD3C_STRUCT;

// **** D0F0xBC_x3FD40 Register Definition ****
// Address
#define D0F0xBC_x3FD40_ADDRESS                                  0x3fd40

// Type
#define D0F0xBC_x3FD40_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD40_MIN_VDDNB_OFFSET                         0
#define D0F0xBC_x3FD40_MIN_VDDNB_WIDTH                          32
#define D0F0xBC_x3FD40_MIN_VDDNB_MASK                           0xffffffff

/// D0F0xBC_x3FD40
typedef union {
  struct {                                                              ///<
    UINT32                                                MIN_VDDNB:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD40_STRUCT;

// **** D0F0xBC_x3FD44 Register Definition ****
// Address
#define D0F0xBC_x3FD44_ADDRESS                                  0x3fd44

// Type
#define D0F0xBC_x3FD44_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD44_HysteresisDown_OFFSET                    0
#define D0F0xBC_x3FD44_HysteresisDown_WIDTH                     8
#define D0F0xBC_x3FD44_HysteresisDown_MASK                      0xff
#define D0F0xBC_x3FD44_HysteresisUp_OFFSET                      8
#define D0F0xBC_x3FD44_HysteresisUp_WIDTH                       8
#define D0F0xBC_x3FD44_HysteresisUp_MASK                        0xff00
#define D0F0xBC_x3FD44_ResidencyCounter_OFFSET                  16
#define D0F0xBC_x3FD44_ResidencyCounter_WIDTH                   16
#define D0F0xBC_x3FD44_ResidencyCounter_MASK                    0xffff0000

/// D0F0xBC_x3FD44
typedef union {
  struct {                                                              ///<
    UINT32                                           HysteresisDown:8 ; ///<
    UINT32                                             HysteresisUp:8 ; ///<
    UINT32                                         ResidencyCounter:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD44_STRUCT;

// **** D0F0xBC_x3FD48 Register Definition ****
// Address
#define D0F0xBC_x3FD48_ADDRESS                                  0x3fd48

// Type
#define D0F0xBC_x3FD48_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD48_LCLK_FREQUENCY_OFFSET                    0
#define D0F0xBC_x3FD48_LCLK_FREQUENCY_WIDTH                     32
#define D0F0xBC_x3FD48_LCLK_FREQUENCY_MASK                      0xffffffff

/// D0F0xBC_x3FD48
typedef union {
  struct {                                                              ///<
    UINT32                                           LCLK_FREQUENCY:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD48_STRUCT;

// **** D0F0xBC_x3FD4C Register Definition ****
// Address
#define D0F0xBC_x3FD4C_ADDRESS                                  0x3fd4c

// Type
#define D0F0xBC_x3FD4C_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD4C_Reserved_15_0_OFFSET                     0
#define D0F0xBC_x3FD4C_Reserved_15_0_WIDTH                      16
#define D0F0xBC_x3FD4C_Reserved_15_0_MASK                       0xffff
#define D0F0xBC_x3FD4C_EnabledForThrottle_OFFSET                16
#define D0F0xBC_x3FD4C_EnabledForThrottle_WIDTH                 8
#define D0F0xBC_x3FD4C_EnabledForThrottle_MASK                  0xff0000
#define D0F0xBC_x3FD4C_ActivityThreshold_OFFSET                 24
#define D0F0xBC_x3FD4C_ActivityThreshold_WIDTH                  8
#define D0F0xBC_x3FD4C_ActivityThreshold_MASK                   0xff000000

/// D0F0xBC_x3FD4C
typedef union {
  struct {                                                              ///<
    UINT32                                            Reserved_15_0:16; ///<
    UINT32                                       EnabledForThrottle:8 ; ///<
    UINT32                                        ActivityThreshold:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD4C_STRUCT;

// **** D0F0xBC_x3FD50 Register Definition ****
// Address
#define D0F0xBC_x3FD50_ADDRESS                                  0x3fd50

// Type
#define D0F0xBC_x3FD50_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD50_LowVoltageReqThreshold_OFFSET            0
#define D0F0xBC_x3FD50_LowVoltageReqThreshold_WIDTH             8
#define D0F0xBC_x3FD50_LowVoltageReqThreshold_MASK              0xff
#define D0F0xBC_x3FD50_VID_OFFSET                               8
#define D0F0xBC_x3FD50_VID_WIDTH                                8
#define D0F0xBC_x3FD50_VID_MASK                                 0xff00
#define D0F0xBC_x3FD50_LclkDivider_OFFSET                       16
#define D0F0xBC_x3FD50_LclkDivider_WIDTH                        8
#define D0F0xBC_x3FD50_LclkDivider_MASK                         0xff0000
#define D0F0xBC_x3FD50_StateValid_OFFSET                        24
#define D0F0xBC_x3FD50_StateValid_WIDTH                         8
#define D0F0xBC_x3FD50_StateValid_MASK                          0xff000000

/// D0F0xBC_x3FD50
typedef union {
  struct {                                                              ///<
    UINT32                                   LowVoltageReqThreshold:8 ; ///<
    UINT32                                                      VID:8 ; ///<
    UINT32                                              LclkDivider:8 ; ///<
    UINT32                                               StateValid:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD50_STRUCT;

// **** D0F0xBC_x3FD54 Register Definition ****
// Address
#define D0F0xBC_x3FD54_ADDRESS                                  0x3fd54

// Type
#define D0F0xBC_x3FD54_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD54_MIN_VDDNB_OFFSET                         0
#define D0F0xBC_x3FD54_MIN_VDDNB_WIDTH                          32
#define D0F0xBC_x3FD54_MIN_VDDNB_MASK                           0xffffffff

/// D0F0xBC_x3FD54
typedef union {
  struct {                                                              ///<
    UINT32                                                MIN_VDDNB:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD54_STRUCT;

// **** D0F0xBC_x3FD58 Register Definition ****
// Address
#define D0F0xBC_x3FD58_ADDRESS                                  0x3fd58

// Type
#define D0F0xBC_x3FD58_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD58_HysteresisDown_OFFSET                    0
#define D0F0xBC_x3FD58_HysteresisDown_WIDTH                     8
#define D0F0xBC_x3FD58_HysteresisDown_MASK                      0xff
#define D0F0xBC_x3FD58_HysteresisUp_OFFSET                      8
#define D0F0xBC_x3FD58_HysteresisUp_WIDTH                       8
#define D0F0xBC_x3FD58_HysteresisUp_MASK                        0xff00
#define D0F0xBC_x3FD58_ResidencyCounter_OFFSET                  16
#define D0F0xBC_x3FD58_ResidencyCounter_WIDTH                   16
#define D0F0xBC_x3FD58_ResidencyCounter_MASK                    0xffff0000

/// D0F0xBC_x3FD58
typedef union {
  struct {                                                              ///<
    UINT32                                           HysteresisDown:8 ; ///<
    UINT32                                             HysteresisUp:8 ; ///<
    UINT32                                         ResidencyCounter:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD58_STRUCT;

// **** D0F0xBC_x3FD5C Register Definition ****
// Address
#define D0F0xBC_x3FD5C_ADDRESS                                  0x3fd5c

// Type
#define D0F0xBC_x3FD5C_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD5C_LCLK_FREQUENCY_OFFSET                    0
#define D0F0xBC_x3FD5C_LCLK_FREQUENCY_WIDTH                     32
#define D0F0xBC_x3FD5C_LCLK_FREQUENCY_MASK                      0xffffffff

/// D0F0xBC_x3FD5C
typedef union {
  struct {                                                              ///<
    UINT32                                           LCLK_FREQUENCY:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD5C_STRUCT;

// **** D0F0xBC_x3FD60 Register Definition ****
// Address
#define D0F0xBC_x3FD60_ADDRESS                                  0x3fd60

// Type
#define D0F0xBC_x3FD60_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD60_Reserved_15_0_OFFSET                     0
#define D0F0xBC_x3FD60_Reserved_15_0_WIDTH                      16
#define D0F0xBC_x3FD60_Reserved_15_0_MASK                       0xffff
#define D0F0xBC_x3FD60_EnabledForThrottle_OFFSET                16
#define D0F0xBC_x3FD60_EnabledForThrottle_WIDTH                 8
#define D0F0xBC_x3FD60_EnabledForThrottle_MASK                  0xff0000
#define D0F0xBC_x3FD60_ActivityThreshold_OFFSET                 24
#define D0F0xBC_x3FD60_ActivityThreshold_WIDTH                  8
#define D0F0xBC_x3FD60_ActivityThreshold_MASK                   0xff000000

/// D0F0xBC_x3FD60
typedef union {
  struct {                                                              ///<
    UINT32                                            Reserved_15_0:16; ///<
    UINT32                                       EnabledForThrottle:8 ; ///<
    UINT32                                        ActivityThreshold:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD60_STRUCT;

// **** D0F0xBC_x3FD64 Register Definition ****
// Address
#define D0F0xBC_x3FD64_ADDRESS                                  0x3fd64

// Type
#define D0F0xBC_x3FD64_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD64_LowVoltageReqThreshold_OFFSET            0
#define D0F0xBC_x3FD64_LowVoltageReqThreshold_WIDTH             8
#define D0F0xBC_x3FD64_LowVoltageReqThreshold_MASK              0xff
#define D0F0xBC_x3FD64_VID_OFFSET                               8
#define D0F0xBC_x3FD64_VID_WIDTH                                8
#define D0F0xBC_x3FD64_VID_MASK                                 0xff00
#define D0F0xBC_x3FD64_LclkDivider_OFFSET                       16
#define D0F0xBC_x3FD64_LclkDivider_WIDTH                        8
#define D0F0xBC_x3FD64_LclkDivider_MASK                         0xff0000
#define D0F0xBC_x3FD64_StateValid_OFFSET                        24
#define D0F0xBC_x3FD64_StateValid_WIDTH                         8
#define D0F0xBC_x3FD64_StateValid_MASK                          0xff000000

/// D0F0xBC_x3FD64
typedef union {
  struct {                                                              ///<
    UINT32                                   LowVoltageReqThreshold:8 ; ///<
    UINT32                                                      VID:8 ; ///<
    UINT32                                              LclkDivider:8 ; ///<
    UINT32                                               StateValid:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD64_STRUCT;

// **** D0F0xBC_x3FD68 Register Definition ****
// Address
#define D0F0xBC_x3FD68_ADDRESS                                  0x3fd68

// Type
#define D0F0xBC_x3FD68_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD68_MIN_VDDNB_OFFSET                         0
#define D0F0xBC_x3FD68_MIN_VDDNB_WIDTH                          32
#define D0F0xBC_x3FD68_MIN_VDDNB_MASK                           0xffffffff

/// D0F0xBC_x3FD68
typedef union {
  struct {                                                              ///<
    UINT32                                                MIN_VDDNB:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD68_STRUCT;

// **** D0F0xBC_x3FD6C Register Definition ****
// Address
#define D0F0xBC_x3FD6C_ADDRESS                                  0x3fd6c

// Type
#define D0F0xBC_x3FD6C_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD6C_HysteresisDown_OFFSET                    0
#define D0F0xBC_x3FD6C_HysteresisDown_WIDTH                     8
#define D0F0xBC_x3FD6C_HysteresisDown_MASK                      0xff
#define D0F0xBC_x3FD6C_HysteresisUp_OFFSET                      8
#define D0F0xBC_x3FD6C_HysteresisUp_WIDTH                       8
#define D0F0xBC_x3FD6C_HysteresisUp_MASK                        0xff00
#define D0F0xBC_x3FD6C_ResidencyCounter_OFFSET                  16
#define D0F0xBC_x3FD6C_ResidencyCounter_WIDTH                   16
#define D0F0xBC_x3FD6C_ResidencyCounter_MASK                    0xffff0000

/// D0F0xBC_x3FD6C
typedef union {
  struct {                                                              ///<
    UINT32                                           HysteresisDown:8 ; ///<
    UINT32                                             HysteresisUp:8 ; ///<
    UINT32                                         ResidencyCounter:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD6C_STRUCT;

// **** D0F0xBC_x3FD70 Register Definition ****
// Address
#define D0F0xBC_x3FD70_ADDRESS                                  0x3fd70

// Type
#define D0F0xBC_x3FD70_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD70_LCLK_FREQUENCY_OFFSET                    0
#define D0F0xBC_x3FD70_LCLK_FREQUENCY_WIDTH                     32
#define D0F0xBC_x3FD70_LCLK_FREQUENCY_MASK                      0xffffffff

/// D0F0xBC_x3FD70
typedef union {
  struct {                                                              ///<
    UINT32                                           LCLK_FREQUENCY:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD70_STRUCT;

// **** D0F0xBC_x3FD74 Register Definition ****
// Address
#define D0F0xBC_x3FD74_ADDRESS                                  0x3fd74

// Type
#define D0F0xBC_x3FD74_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD74_Reserved_15_0_OFFSET                     0
#define D0F0xBC_x3FD74_Reserved_15_0_WIDTH                      16
#define D0F0xBC_x3FD74_Reserved_15_0_MASK                       0xffff
#define D0F0xBC_x3FD74_EnabledForThrottle_OFFSET                16
#define D0F0xBC_x3FD74_EnabledForThrottle_WIDTH                 8
#define D0F0xBC_x3FD74_EnabledForThrottle_MASK                  0xff0000
#define D0F0xBC_x3FD74_ActivityThreshold_OFFSET                 24
#define D0F0xBC_x3FD74_ActivityThreshold_WIDTH                  8
#define D0F0xBC_x3FD74_ActivityThreshold_MASK                   0xff000000

/// D0F0xBC_x3FD74
typedef union {
  struct {                                                              ///<
    UINT32                                            Reserved_15_0:16; ///<
    UINT32                                       EnabledForThrottle:8 ; ///<
    UINT32                                        ActivityThreshold:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD74_STRUCT;

// **** D0F0xBC_x3FD78 Register Definition ****
// Address
#define D0F0xBC_x3FD78_ADDRESS                                  0x3fd78

// Type
#define D0F0xBC_x3FD78_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD78_LowVoltageReqThreshold_OFFSET            0
#define D0F0xBC_x3FD78_LowVoltageReqThreshold_WIDTH             8
#define D0F0xBC_x3FD78_LowVoltageReqThreshold_MASK              0xff
#define D0F0xBC_x3FD78_VID_OFFSET                               8
#define D0F0xBC_x3FD78_VID_WIDTH                                8
#define D0F0xBC_x3FD78_VID_MASK                                 0xff00
#define D0F0xBC_x3FD78_LclkDivider_OFFSET                       16
#define D0F0xBC_x3FD78_LclkDivider_WIDTH                        8
#define D0F0xBC_x3FD78_LclkDivider_MASK                         0xff0000
#define D0F0xBC_x3FD78_StateValid_OFFSET                        24
#define D0F0xBC_x3FD78_StateValid_WIDTH                         8
#define D0F0xBC_x3FD78_StateValid_MASK                          0xff000000

/// D0F0xBC_x3FD78
typedef union {
  struct {                                                              ///<
    UINT32                                   LowVoltageReqThreshold:8 ; ///<
    UINT32                                                      VID:8 ; ///<
    UINT32                                              LclkDivider:8 ; ///<
    UINT32                                               StateValid:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD78_STRUCT;

// **** D0F0xBC_x3FD7C Register Definition ****
// Address
#define D0F0xBC_x3FD7C_ADDRESS                                  0x3fd7c

// Type
#define D0F0xBC_x3FD7C_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD7C_MIN_VDDNB_OFFSET                         0
#define D0F0xBC_x3FD7C_MIN_VDDNB_WIDTH                          32
#define D0F0xBC_x3FD7C_MIN_VDDNB_MASK                           0xffffffff

/// D0F0xBC_x3FD7C
typedef union {
  struct {                                                              ///<
    UINT32                                                MIN_VDDNB:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD7C_STRUCT;

// **** D0F0xBC_x3FD80 Register Definition ****
// Address
#define D0F0xBC_x3FD80_ADDRESS                                  0x3fd80

// Type
#define D0F0xBC_x3FD80_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD80_HysteresisDown_OFFSET                    0
#define D0F0xBC_x3FD80_HysteresisDown_WIDTH                     8
#define D0F0xBC_x3FD80_HysteresisDown_MASK                      0xff
#define D0F0xBC_x3FD80_HysteresisUp_OFFSET                      8
#define D0F0xBC_x3FD80_HysteresisUp_WIDTH                       8
#define D0F0xBC_x3FD80_HysteresisUp_MASK                        0xff00
#define D0F0xBC_x3FD80_ResidencyCounter_OFFSET                  16
#define D0F0xBC_x3FD80_ResidencyCounter_WIDTH                   16
#define D0F0xBC_x3FD80_ResidencyCounter_MASK                    0xffff0000

/// D0F0xBC_x3FD80
typedef union {
  struct {                                                              ///<
    UINT32                                           HysteresisDown:8 ; ///<
    UINT32                                             HysteresisUp:8 ; ///<
    UINT32                                         ResidencyCounter:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD80_STRUCT;

// **** D0F0xBC_x3FD84 Register Definition ****
// Address
#define D0F0xBC_x3FD84_ADDRESS                                  0x3fd84

// Type
#define D0F0xBC_x3FD84_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD84_LCLK_FREQUENCY_OFFSET                    0
#define D0F0xBC_x3FD84_LCLK_FREQUENCY_WIDTH                     32
#define D0F0xBC_x3FD84_LCLK_FREQUENCY_MASK                      0xffffffff

/// D0F0xBC_x3FD84
typedef union {
  struct {                                                              ///<
    UINT32                                           LCLK_FREQUENCY:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD84_STRUCT;

// **** D0F0xBC_x3FD88 Register Definition ****
// Address
#define D0F0xBC_x3FD88_ADDRESS                                  0x3fd88

// Type
#define D0F0xBC_x3FD88_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD88_Reserved_15_0_OFFSET                     0
#define D0F0xBC_x3FD88_Reserved_15_0_WIDTH                      16
#define D0F0xBC_x3FD88_Reserved_15_0_MASK                       0xffff
#define D0F0xBC_x3FD88_EnabledForThrottle_OFFSET                16
#define D0F0xBC_x3FD88_EnabledForThrottle_WIDTH                 8
#define D0F0xBC_x3FD88_EnabledForThrottle_MASK                  0xff0000
#define D0F0xBC_x3FD88_ActivityThreshold_OFFSET                 24
#define D0F0xBC_x3FD88_ActivityThreshold_WIDTH                  8
#define D0F0xBC_x3FD88_ActivityThreshold_MASK                   0xff000000

/// D0F0xBC_x3FD88
typedef union {
  struct {                                                              ///<
    UINT32                                            Reserved_15_0:16; ///<
    UINT32                                       EnabledForThrottle:8 ; ///<
    UINT32                                        ActivityThreshold:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD88_STRUCT;

// **** D0F0xBC_x3FD8C Register Definition ****
// Address
#define D0F0xBC_x3FD8C_ADDRESS                                  0x3fd8c

// Type
#define D0F0xBC_x3FD8C_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD8C_LowVoltageReqThreshold_OFFSET            0
#define D0F0xBC_x3FD8C_LowVoltageReqThreshold_WIDTH             8
#define D0F0xBC_x3FD8C_LowVoltageReqThreshold_MASK              0xff
#define D0F0xBC_x3FD8C_VID_OFFSET                               8
#define D0F0xBC_x3FD8C_VID_WIDTH                                8
#define D0F0xBC_x3FD8C_VID_MASK                                 0xff00
#define D0F0xBC_x3FD8C_LclkDivider_OFFSET                       16
#define D0F0xBC_x3FD8C_LclkDivider_WIDTH                        8
#define D0F0xBC_x3FD8C_LclkDivider_MASK                         0xff0000
#define D0F0xBC_x3FD8C_StateValid_OFFSET                        24
#define D0F0xBC_x3FD8C_StateValid_WIDTH                         8
#define D0F0xBC_x3FD8C_StateValid_MASK                          0xff000000

/// D0F0xBC_x3FD8C
typedef union {
  struct {                                                              ///<
    UINT32                                   LowVoltageReqThreshold:8 ; ///<
    UINT32                                                      VID:8 ; ///<
    UINT32                                              LclkDivider:8 ; ///<
    UINT32                                               StateValid:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD8C_STRUCT;

// **** D0F0xBC_x3FD90 Register Definition ****
// Address
#define D0F0xBC_x3FD90_ADDRESS                                  0x3fd90

// Type
#define D0F0xBC_x3FD90_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD90_MIN_VDDNB_OFFSET                         0
#define D0F0xBC_x3FD90_MIN_VDDNB_WIDTH                          32
#define D0F0xBC_x3FD90_MIN_VDDNB_MASK                           0xffffffff

/// D0F0xBC_x3FD90
typedef union {
  struct {                                                              ///<
    UINT32                                                MIN_VDDNB:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD90_STRUCT;

// **** D0F0xBC_x3FD94 Register Definition ****
// Address
#define D0F0xBC_x3FD94_ADDRESS                                  0x3fd94

// Type
#define D0F0xBC_x3FD94_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD94_HysteresisDown_OFFSET                    0
#define D0F0xBC_x3FD94_HysteresisDown_WIDTH                     8
#define D0F0xBC_x3FD94_HysteresisDown_MASK                      0xff
#define D0F0xBC_x3FD94_HysteresisUp_OFFSET                      8
#define D0F0xBC_x3FD94_HysteresisUp_WIDTH                       8
#define D0F0xBC_x3FD94_HysteresisUp_MASK                        0xff00
#define D0F0xBC_x3FD94_ResidencyCounter_OFFSET                  16
#define D0F0xBC_x3FD94_ResidencyCounter_WIDTH                   16
#define D0F0xBC_x3FD94_ResidencyCounter_MASK                    0xffff0000

/// D0F0xBC_x3FD94
typedef union {
  struct {                                                              ///<
    UINT32                                           HysteresisDown:8 ; ///<
    UINT32                                             HysteresisUp:8 ; ///<
    UINT32                                         ResidencyCounter:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD94_STRUCT;

// **** D0F0xBC_x3FD98 Register Definition ****
// Address
#define D0F0xBC_x3FD98_ADDRESS                                  0x3fd98

// Type
#define D0F0xBC_x3FD98_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD98_LCLK_FREQUENCY_OFFSET                    0
#define D0F0xBC_x3FD98_LCLK_FREQUENCY_WIDTH                     32
#define D0F0xBC_x3FD98_LCLK_FREQUENCY_MASK                      0xffffffff

/// D0F0xBC_x3FD98
typedef union {
  struct {                                                              ///<
    UINT32                                           LCLK_FREQUENCY:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD98_STRUCT;

// **** D0F0xBC_x3FD9C Register Definition ****
// Address
#define D0F0xBC_x3FD9C_ADDRESS                                  0x3fd9c

// Type
#define D0F0xBC_x3FD9C_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FD9C_Reserved_15_0_OFFSET                     0
#define D0F0xBC_x3FD9C_Reserved_15_0_WIDTH                      16
#define D0F0xBC_x3FD9C_Reserved_15_0_MASK                       0xffff
#define D0F0xBC_x3FD9C_EnabledForThrottle_OFFSET                16
#define D0F0xBC_x3FD9C_EnabledForThrottle_WIDTH                 8
#define D0F0xBC_x3FD9C_EnabledForThrottle_MASK                  0xff0000
#define D0F0xBC_x3FD9C_ActivityThreshold_OFFSET                 24
#define D0F0xBC_x3FD9C_ActivityThreshold_WIDTH                  8
#define D0F0xBC_x3FD9C_ActivityThreshold_MASK                   0xff000000

/// D0F0xBC_x3FD9C
typedef union {
  struct {                                                              ///<
    UINT32                                            Reserved_15_0:16; ///<
    UINT32                                       EnabledForThrottle:8 ; ///<
    UINT32                                        ActivityThreshold:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FD9C_STRUCT;

// **** D0F0xBC_x3FDC0 Register Definition ****
// Address
#define D0F0xBC_x3FDC0_ADDRESS                                  0x3fdc0

// Type
#define D0F0xBC_x3FDC0_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FDC0_STATE_SHIFT_OFFSET                       0
#define D0F0xBC_x3FDC0_STATE_SHIFT_WIDTH                        32
#define D0F0xBC_x3FDC0_STATE_SHIFT_MASK                         0xffffffff

/// D0F0xBC_x3FDC0
typedef union {
  struct {                                                              ///<
    UINT32                                              STATE_SHIFT:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FDC0_STRUCT;

// **** D0F0xBC_x3FDC4 Register Definition ****
// Address
#define D0F0xBC_x3FDC4_ADDRESS                                  0x3fdc4

// Type
#define D0F0xBC_x3FDC4_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FDC4_LCLK_DPM_LEVEL_COUNT_OFFSET              0
#define D0F0xBC_x3FDC4_LCLK_DPM_LEVEL_COUNT_WIDTH               32
#define D0F0xBC_x3FDC4_LCLK_DPM_LEVEL_COUNT_MASK                0xffffffff

/// D0F0xBC_x3FDC4
typedef union {
  struct {                                                              ///<
    UINT32                                     LCLK_DPM_LEVEL_COUNT:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FDC4_STRUCT;

// **** D0F0xBC_x3FDC8 Register Definition ****
// Address
#define D0F0xBC_x3FDC8_ADDRESS                                  0x3fdc8

// Type
#define D0F0xBC_x3FDC8_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FDC8_Reserved_7_0_OFFSET                      0
#define D0F0xBC_x3FDC8_Reserved_7_0_WIDTH                       8
#define D0F0xBC_x3FDC8_Reserved_7_0_MASK                        0xff
#define D0F0xBC_x3FDC8_LclkDpmBootState_OFFSET                  8
#define D0F0xBC_x3FDC8_LclkDpmBootState_WIDTH                   8
#define D0F0xBC_x3FDC8_LclkDpmBootState_MASK                    0xff00
#define D0F0xBC_x3FDC8_VoltageChgEn_OFFSET                      16
#define D0F0xBC_x3FDC8_VoltageChgEn_WIDTH                       8
#define D0F0xBC_x3FDC8_VoltageChgEn_MASK                        0xff0000
#define D0F0xBC_x3FDC8_LclkDpmEn_OFFSET                         24
#define D0F0xBC_x3FDC8_LclkDpmEn_WIDTH                          8
#define D0F0xBC_x3FDC8_LclkDpmEn_MASK                           0xff000000

/// D0F0xBC_x3FDC8
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_7_0:8 ; ///<
    UINT32                                         LclkDpmBootState:8 ; ///<
    UINT32                                             VoltageChgEn:8 ; ///<
    UINT32                                                LclkDpmEn:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FDC8_STRUCT;

// **** D0F0xBC_x3FDCC Register Definition ****
// Address
#define D0F0xBC_x3FDCC_ADDRESS                                  0x3fdcc

// Type
#define D0F0xBC_x3FDCC_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FDCC_CURRENT_STATE_OFFSET                     0
#define D0F0xBC_x3FDCC_CURRENT_STATE_WIDTH                      8
#define D0F0xBC_x3FDCC_CURRENT_STATE_MASK                       0xff
#define D0F0xBC_x3FDCC_TARGET_STATE_OFFSET                      8
#define D0F0xBC_x3FDCC_TARGET_STATE_WIDTH                       8
#define D0F0xBC_x3FDCC_TARGET_STATE_MASK                        0xff00
#define D0F0xBC_x3FDCC_Reserved_OFFSET                          16
#define D0F0xBC_x3FDCC_Reserved_WIDTH                           16
#define D0F0xBC_x3FDCC_Reserved_MASK                            0xffff0000

/// D0F0xBC_x3FDCC
typedef union {
  struct {                                                              ///<
    UINT32                                            CURRENT_STATE:8 ; ///<
    UINT32                                             TARGET_STATE:8 ; ///<
    UINT32                                                 Reserved:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FDCC_STRUCT;

// **** D0F0xBC_x3FDD0 Register Definition ****
// Address
#define D0F0xBC_x3FDD0_ADDRESS                                  0x3fdd0

// Type
#define D0F0xBC_x3FDD0_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FDD0_LclkThermalThrottlingEn_OFFSET           0
#define D0F0xBC_x3FDD0_LclkThermalThrottlingEn_WIDTH            8
#define D0F0xBC_x3FDD0_LclkThermalThrottlingEn_MASK             0xff
#define D0F0xBC_x3FDD0_TemperatureSel_OFFSET                    8
#define D0F0xBC_x3FDD0_TemperatureSel_WIDTH                     8
#define D0F0xBC_x3FDD0_TemperatureSel_MASK                      0xff00
#define D0F0xBC_x3FDD0_LclkTtMode_OFFSET                        16
#define D0F0xBC_x3FDD0_LclkTtMode_WIDTH                         8
#define D0F0xBC_x3FDD0_LclkTtMode_MASK                          0xff0000
#define D0F0xBC_x3FDD0_TtHtcActive_OFFSET                       24
#define D0F0xBC_x3FDD0_TtHtcActive_WIDTH                        8
#define D0F0xBC_x3FDD0_TtHtcActive_MASK                         0xff000000

/// D0F0xBC_x3FDD0
typedef union {
  struct {                                                              ///<
    UINT32                                  LclkThermalThrottlingEn:8 ; ///<
    UINT32                                           TemperatureSel:8 ; ///<
    UINT32                                               LclkTtMode:8 ; ///<
    UINT32                                              TtHtcActive:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FDD0_STRUCT;

// **** D0F0xBC_x3FDD4 Register Definition ****
// Address
#define D0F0xBC_x3FDD4_ADDRESS                                  0x3fdd4

// Type
#define D0F0xBC_x3FDD4_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x3FDD4_LowThreshold_OFFSET                      0
#define D0F0xBC_x3FDD4_LowThreshold_WIDTH                       16
#define D0F0xBC_x3FDD4_LowThreshold_MASK                        0xffff
#define D0F0xBC_x3FDD4_HighThreshold_OFFSET                     16
#define D0F0xBC_x3FDD4_HighThreshold_WIDTH                      16
#define D0F0xBC_x3FDD4_HighThreshold_MASK                       0xffff0000

/// D0F0xBC_x3FDD4
typedef union {
  struct {                                                              ///<
    UINT32                                             LowThreshold:16; ///<
    UINT32                                            HighThreshold:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FDD4_STRUCT;

// **** D0F0xBC_xC0104000 Field Definition ****
// Address
#define D0F0xBC_xC0104000_ADDRESS                               0xC0104000

// Type
#define D0F0xBC_xC0104000_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0104000_Reserved0_31_OFFSET                   0
#define D0F0xBC_xC0104000_Reserved0_31_WIDTH                    32
#define D0F0xBC_xC0104000_Reserved0_31_MASK                     0xffffffff

/// D0F0xBC_xC0104000
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved0_31:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0104000_STRUCT;

// **** D0F0xBC_xC0104007 Field Definition ****
// Address
#define D0F0xBC_xC0104007_ADDRESS                               0xC0104007

// Type
#define D0F0xBC_xC0104007_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0104007_Reserved0_4_OFFSET                    0
#define D0F0xBC_xC0104007_Reserved0_4_WIDTH                     5
#define D0F0xBC_xC0104007_Reserved0_4_MASK                      0x0000001F
#define D0F0xBC_xC0104007_VddNbVid0_OFFSET                      5
#define D0F0xBC_xC0104007_VddNbVid0_WIDTH                       8
#define D0F0xBC_xC0104007_VddNbVid0_MASK                        0x00001FE0
#define D0F0xBC_xC0104007_Reserved13_31_OFFSET                  13
#define D0F0xBC_xC0104007_Reserved13_31_WIDTH                   19
#define D0F0xBC_xC0104007_Reserved13_31_MASK                    0xFFFFE000

/// D0F0xBC_xC0104007
typedef union {
  struct {                                                              ///<
    UINT32                                               Reserved0_4:5; ///<
    UINT32                                                 VddNbVid0:8; ///<
    UINT32                                            Reserved13_31:19; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0104007_STRUCT;

// **** D0F0xBC_xC0104008 Field Definition ****
// Address
#define D0F0xBC_xC0104008_ADDRESS                               0xC0104008

// Type
#define D0F0xBC_xC0104008_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0104008_Reserved0_4_OFFSET                    0
#define D0F0xBC_xC0104008_Reserved0_4_WIDTH                     5
#define D0F0xBC_xC0104008_Reserved0_4_MASK                      0x1f
#define D0F0xBC_xC0104008_VddNbVid1_OFFSET                      5
#define D0F0xBC_xC0104008_VddNbVid1_WIDTH                       8
#define D0F0xBC_xC0104008_VddNbVid1_MASK                        0x1fe0
#define D0F0xBC_xC0104008_VddNbVid2_OFFSET                      13
#define D0F0xBC_xC0104008_VddNbVid2_WIDTH                       8
#define D0F0xBC_xC0104008_VddNbVid2_MASK                        0x1fe000
#define D0F0xBC_xC0104008_VddNbVid3_OFFSET                      21
#define D0F0xBC_xC0104008_VddNbVid3_WIDTH                       8
#define D0F0xBC_xC0104008_VddNbVid3_MASK                        0x1fe00000
#define D0F0xBC_xC0104008_Reserved29_31_OFFSET                  29
#define D0F0xBC_xC0104008_Reserved29_31_WIDTH                   3
#define D0F0xBC_xC0104008_Reserved29_31_MASK                    0xe0000000

/// D0F0xBC_xC0104008
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_4:5 ; ///<
    UINT32                                                VddNbVid1:8 ; ///<
    UINT32                                                VddNbVid2:8 ; ///<
    UINT32                                                VddNbVid3:8 ; ///<
    UINT32                                            Reserved29_31:3 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0104008_STRUCT;

// **** D0F0xBC_xC010400C Field Definition ****
// Address
#define D0F0xBC_xC010400C_ADDRESS                               0xC010400c

// Type
#define D0F0xBC_xC010400C_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010400C_Reserved0_24_OFFSET                   0
#define D0F0xBC_xC010400C_Reserved0_24_WIDTH                    25
#define D0F0xBC_xC010400C_Reserved0_24_MASK                     0x1ffffff
#define D0F0xBC_xC010400C_MainPllOpFreqIdStartup_OFFSET         25
#define D0F0xBC_xC010400C_MainPllOpFreqIdStartup_WIDTH          6
#define D0F0xBC_xC010400C_MainPllOpFreqIdStartup_MASK           0x7e000000
#define D0F0xBC_xC010400C_Reserved31_31_OFFSET                  31
#define D0F0xBC_xC010400C_Reserved31_31_WIDTH                   1
#define D0F0xBC_xC010400C_Reserved31_31_MASK                    0x80000000

/// D0F0xBC_xC010400C
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved0_24:25; ///<
    UINT32                                   MainPllOpFreqIdStartup:6 ; ///<
    UINT32                                            Reserved31_31:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010400C_STRUCT;

// **** D0F0xBC_xC0107044 Field Definition ****
// Address
#define D0F0xBC_xC0107044_ADDRESS                               0xC0107044

// Type
#define D0F0xBC_xC0107044_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107044_Reserved00_15_OFFSET                  0
#define D0F0xBC_xC0107044_Reserved00_15_WIDTH                   16
#define D0F0xBC_xC0107044_Reserved00_15_MASK                    0xffff
#define D0F0xBC_xC0107044_NumBoostStates_OFFSET                 16
#define D0F0xBC_xC0107044_NumBoostStates_WIDTH                  3
#define D0F0xBC_xC0107044_NumBoostStates_MASK                   0x70000
#define D0F0xBC_xC0107044_Reserved19_31_OFFSET                  19
#define D0F0xBC_xC0107044_Reserved19_31_WIDTH                   13
#define D0F0xBC_xC0107044_Reserved19_31_MASK                    0xfff80000

/// D0F0xBC_xC0107044
typedef union {
  struct {                                                              ///<
    UINT32                                           Reserved00_15:16 ; ///<
    UINT32                                           NumBoostStates:3 ; ///<
    UINT32                                           Reserved19_31:13 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107044_STRUCT;

// **** D0F0xBC_xC010407C Field Definition ****
// Address
#define D0F0xBC_xC010407C_ADDRESS                               0xC010407c

// Type
#define D0F0xBC_xC010407C_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010407C_Reserved0_19_OFFSET                   0
#define D0F0xBC_xC010407C_Reserved0_19_WIDTH                    20
#define D0F0xBC_xC010407C_Reserved0_19_MASK                     0xfffff
#define D0F0xBC_xC010407C_VddNbVid4_OFFSET                      20
#define D0F0xBC_xC010407C_VddNbVid4_WIDTH                       8
#define D0F0xBC_xC010407C_VddNbVid4_MASK                        0xff00000
#define D0F0xBC_xC010407C_Reserved28_31_OFFSET                  28
#define D0F0xBC_xC010407C_Reserved28_31_WIDTH                   4
#define D0F0xBC_xC010407C_Reserved28_31_MASK                    0xf0000000

/// D0F0xBC_xC010407C
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved0_19:20; ///<
    UINT32                                                VddNbVid4:8 ; ///<
    UINT32                                            Reserved28_31:4 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010407C_STRUCT;

// **** D0F0xBC_xC0104080 Field Definition ****
// Address
#define D0F0xBC_xC0104080_ADDRESS                               0xC0104080

// Type
#define D0F0xBC_xC0104080_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0104080_Reserved0_20_OFFSET                   0
#define D0F0xBC_xC0104080_Reserved0_20_WIDTH                    21
#define D0F0xBC_xC0104080_Reserved0_20_MASK                     0x1fffff
#define D0F0xBC_xC0104080_VCEFlag0_OFFSET                       21
#define D0F0xBC_xC0104080_VCEFlag0_WIDTH                        8
#define D0F0xBC_xC0104080_VCEFlag0_MASK                         0x1fe00000
#define D0F0xBC_xC0104080_Reserved29_31_OFFSET                  29
#define D0F0xBC_xC0104080_Reserved29_31_WIDTH                   3
#define D0F0xBC_xC0104080_Reserved29_31_MASK                    0xe0000000

/// D0F0xBC_xC0104080
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved0_20:21; ///<
    UINT32                                                 VCEFlag0:8 ; ///<
    UINT32                                            Reserved29_31:3 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0104080_STRUCT;

// **** D0F0xBC_xC0104083 Field Definition ****
// Address
#define D0F0xBC_xC0104083_ADDRESS                               0xC0104083

// Type
#define D0F0xBC_xC0104083_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0104083_Reserved0_4_OFFSET                    0
#define D0F0xBC_xC0104083_Reserved0_4_WIDTH                     5
#define D0F0xBC_xC0104083_Reserved0_4_MASK                      0x1f
#define D0F0xBC_xC0104083_VCEFlag1_OFFSET                       5
#define D0F0xBC_xC0104083_VCEFlag1_WIDTH                        8
#define D0F0xBC_xC0104083_VCEFlag1_MASK                         0x1fe0
#define D0F0xBC_xC0104083_Reserved13_31_OFFSET                  13
#define D0F0xBC_xC0104083_Reserved13_31_WIDTH                   19
#define D0F0xBC_xC0104083_Reserved13_31_MASK                    0xffffe000

/// D0F0xBC_xC0104083
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_4:5 ; ///<
    UINT32                                                 VCEFlag1:8 ; ///<
    UINT32                                            Reserved13_31:19; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0104083_STRUCT;

// **** D0F0xBC_xC0104084 Field Definition ****
// Address
#define D0F0xBC_xC0104084_ADDRESS                               0xC0104084

// Type
#define D0F0xBC_xC0104084_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0104084_Reserved0_4_OFFSET                    0
#define D0F0xBC_xC0104084_Reserved0_4_WIDTH                     5
#define D0F0xBC_xC0104084_Reserved0_4_MASK                      0x1f
#define D0F0xBC_xC0104084_VCEFlag2_OFFSET                       5
#define D0F0xBC_xC0104084_VCEFlag2_WIDTH                        8
#define D0F0xBC_xC0104084_VCEFlag2_MASK                         0x1fe0
#define D0F0xBC_xC0104084_VCEFlag3_OFFSET                       13
#define D0F0xBC_xC0104084_VCEFlag3_WIDTH                        8
#define D0F0xBC_xC0104084_VCEFlag3_MASK                         0x1fe000
#define D0F0xBC_xC0104084_VCEFlag4_OFFSET                       21
#define D0F0xBC_xC0104084_VCEFlag4_WIDTH                        8
#define D0F0xBC_xC0104084_VCEFlag4_MASK                         0x1fe00000
#define D0F0xBC_xC0104084_ReqSclkSel0_OFFSET                    29
#define D0F0xBC_xC0104084_ReqSclkSel0_WIDTH                     3
#define D0F0xBC_xC0104084_ReqSclkSel0_MASK                      0xe0000000

/// D0F0xBC_xC0104084
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_4:5 ; ///<
    UINT32                                                 VCEFlag2:8 ; ///<
    UINT32                                                 VCEFlag3:8 ; ///<
    UINT32                                                 VCEFlag4:8 ; ///<
    UINT32                                              ReqSclkSel0:3 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0104084_STRUCT;

// **** D0F0xBC_xC0104088 Field Definition ****
// Address
#define D0F0xBC_xC0104088_ADDRESS                               0xC0104088

// Type
#define D0F0xBC_xC0104088_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0104088_ReqSclkSel1_OFFSET                    0
#define D0F0xBC_xC0104088_ReqSclkSel1_WIDTH                     3
#define D0F0xBC_xC0104088_ReqSclkSel1_MASK                      0x7
#define D0F0xBC_xC0104088_ReqSclkSel2_OFFSET                    3
#define D0F0xBC_xC0104088_ReqSclkSel2_WIDTH                     3
#define D0F0xBC_xC0104088_ReqSclkSel2_MASK                      0x38
#define D0F0xBC_xC0104088_ReqSclkSel3_OFFSET                    6
#define D0F0xBC_xC0104088_ReqSclkSel3_WIDTH                     3
#define D0F0xBC_xC0104088_ReqSclkSel3_MASK                      0x1c0
#define D0F0xBC_xC0104088_ReqSclkSel4_OFFSET                    9
#define D0F0xBC_xC0104088_ReqSclkSel4_WIDTH                     3
#define D0F0xBC_xC0104088_ReqSclkSel4_MASK                      0xe00
#define D0F0xBC_xC0104088_VCEMclk_OFFSET                        12
#define D0F0xBC_xC0104088_VCEMclk_WIDTH                         5
#define D0F0xBC_xC0104088_VCEMclk_MASK                          0x1f000
#define D0F0xBC_xC0104088_Reserved17_31_OFFSET                  17
#define D0F0xBC_xC0104088_Reserved17_31_WIDTH                   15
#define D0F0xBC_xC0104088_Reserved17_31_MASK                    0xfffe0000

/// D0F0xBC_xC0104088
typedef union {
  struct {                                                              ///<
    UINT32                                              ReqSclkSel1:3 ; ///<
    UINT32                                              ReqSclkSel2:3 ; ///<
    UINT32                                              ReqSclkSel3:3 ; ///<
    UINT32                                              ReqSclkSel4:3 ; ///<
    UINT32                                                  VCEMclk:5 ; ///<
    UINT32                                            Reserved17_31:15; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0104088_STRUCT;


// **** D0F0xBC_xC010408C Field Definition ****
// Address
#define D0F0xBC_xC010408C_ADDRESS                               0xC010408C

// Type
#define D0F0xBC_xC010408C_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010408C_Reserved0_1_OFFSET                    0
#define D0F0xBC_xC010408C_Reserved0_1_WIDTH                     2
#define D0F0xBC_xC010408C_Reserved0_1_MASK                      0x3
#define D0F0xBC_xC010408C_BapmDisable_OFFSET                    2
#define D0F0xBC_xC010408C_BapmDisable_WIDTH                     1
#define D0F0xBC_xC010408C_BapmDisable_MASK                      0x4
#define D0F0xBC_xC010408C_Reserved3_31_OFFSET                   3
#define D0F0xBC_xC010408C_Reserved3_31_WIDTH                    29
#define D0F0xBC_xC010408C_Reserved3_31_MASK                     0xfffffff8

/// D0F0xBC_xC010408C
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_1:2 ; ///<
    UINT32                                              BapmDisable:1 ; ///<
    UINT32                                             Reserved3_31:29; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010408C_STRUCT;

// **** D0F0xBC_xC01040A8 Field Definition ****
// Address
#define D0F0xBC_xC01040A8_ADDRESS                               0xC01040a8

// Type
#define D0F0xBC_xC01040A8_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC01040A8_Reserved0_15_OFFSET                   0
#define D0F0xBC_xC01040A8_Reserved0_15_WIDTH                    16
#define D0F0xBC_xC01040A8_Reserved0_15_MASK                     0xffff
#define D0F0xBC_xC01040A8_VddNbVidOffset0_OFFSET                16
#define D0F0xBC_xC01040A8_VddNbVidOffset0_WIDTH                 8
#define D0F0xBC_xC01040A8_VddNbVidOffset0_MASK                  0xff0000
#define D0F0xBC_xC01040A8_VddNbVidOffset1_OFFSET                24
#define D0F0xBC_xC01040A8_VddNbVidOffset1_WIDTH                 8
#define D0F0xBC_xC01040A8_VddNbVidOffset1_MASK                  0xff000000

/// D0F0xBC_xC01040A8
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved0_15:16; ///<
    UINT32                                          VddNbVidOffset0:8 ; ///<
    UINT32                                          VddNbVidOffset1:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC01040A8_STRUCT;

// **** D0F0xBC_xC01040AC Field Definition ****
// Address
#define D0F0xBC_xC01040AC_ADDRESS                               0xC01040ac

// Type
#define D0F0xBC_xC01040AC_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC01040AC_VddNbVidOffset2_OFFSET                0
#define D0F0xBC_xC01040AC_VddNbVidOffset2_WIDTH                 8
#define D0F0xBC_xC01040AC_VddNbVidOffset2_MASK                  0xff
#define D0F0xBC_xC01040AC_VddNbVidOffset3_OFFSET                8
#define D0F0xBC_xC01040AC_VddNbVidOffset3_WIDTH                 8
#define D0F0xBC_xC01040AC_VddNbVidOffset3_MASK                  0xff00
#define D0F0xBC_xC01040AC_VddNbVidOffset4_OFFSET                16
#define D0F0xBC_xC01040AC_VddNbVidOffset4_WIDTH                 8
#define D0F0xBC_xC01040AC_VddNbVidOffset4_MASK                  0xff0000
#define D0F0xBC_xC01040AC_Reserved24_31_OFFSET                  24
#define D0F0xBC_xC01040AC_Reserved24_31_WIDTH                   8
#define D0F0xBC_xC01040AC_Reserved24_31_MASK                    0xff000000

/// D0F0xBC_xC01040AC
typedef union {
  struct {                                                              ///<
    UINT32                                          VddNbVidOffset2:8 ; ///<
    UINT32                                          VddNbVidOffset3:8 ; ///<
    UINT32                                          VddNbVidOffset4:8 ; ///<
    UINT32                                            Reserved24_31:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC01040AC_STRUCT;

// **** D0F0xBC_xC0107064 Field Definition ****
// Address
#define D0F0xBC_xC0107064_ADDRESS                               0xC0107064

// Type
#define D0F0xBC_xC0107064_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107064_PowerplayTableRev_OFFSET              0
#define D0F0xBC_xC0107064_PowerplayTableRev_WIDTH               4
#define D0F0xBC_xC0107064_PowerplayTableRev_MASK                0xf
#define D0F0xBC_xC0107064_SClkThermDid_OFFSET                   4
#define D0F0xBC_xC0107064_SClkThermDid_WIDTH                    7
#define D0F0xBC_xC0107064_SClkThermDid_MASK                     0x7f0
#define D0F0xBC_xC0107064_PcieGen2Vid_OFFSET                    11
#define D0F0xBC_xC0107064_PcieGen2Vid_WIDTH                     3
#define D0F0xBC_xC0107064_PcieGen2Vid_MASK                      0x3800
#define D0F0xBC_xC0107064_Reserved14_16_OFFSET                  14
#define D0F0xBC_xC0107064_Reserved14_16_WIDTH                   3
#define D0F0xBC_xC0107064_Reserved14_16_MASK                    0x1c000
#define D0F0xBC_xC0107064_SClkDid0_OFFSET                       17
#define D0F0xBC_xC0107064_SClkDid0_WIDTH                        7
#define D0F0xBC_xC0107064_SClkDid0_MASK                         0xfe0000
#define D0F0xBC_xC0107064_SClkDid1_OFFSET                       24
#define D0F0xBC_xC0107064_SClkDid1_WIDTH                        7
#define D0F0xBC_xC0107064_SClkDid1_MASK                         0x7f000000
#define D0F0xBC_xC0107064_Reserved31_31_OFFSET                  31
#define D0F0xBC_xC0107064_Reserved31_31_WIDTH                   1
#define D0F0xBC_xC0107064_Reserved31_31_MASK                    0x80000000

/// D0F0xBC_xC0107064
typedef union {
  struct {                                                              ///<
    UINT32                                        PowerplayTableRev:4 ; ///<
    UINT32                                             SClkThermDid:7 ; ///<
    UINT32                                              PcieGen2Vid:3 ; ///<
    UINT32                                            Reserved14_16:3 ; ///<
    UINT32                                                 SClkDid0:7 ; ///<
    UINT32                                                 SClkDid1:7 ; ///<
    UINT32                                            Reserved31_31:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107064_STRUCT;

// **** D0F0xBC_xC0107067 Field Definition ****
// Address
#define D0F0xBC_xC0107067_ADDRESS                               0xC0107067

// Type
#define D0F0xBC_xC0107067_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107067_Reserved0_6_OFFSET                    0
#define D0F0xBC_xC0107067_Reserved0_6_WIDTH                     7
#define D0F0xBC_xC0107067_Reserved0_6_MASK                      0x7f
#define D0F0xBC_xC0107067_SClkDid2_OFFSET                       7
#define D0F0xBC_xC0107067_SClkDid2_WIDTH                        7
#define D0F0xBC_xC0107067_SClkDid2_MASK                         0x3f80
#define D0F0xBC_xC0107067_Reserved14_31_OFFSET                  14
#define D0F0xBC_xC0107067_Reserved14_31_WIDTH                   18
#define D0F0xBC_xC0107067_Reserved14_31_MASK                    0xffffc000

/// D0F0xBC_xC0107067
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_6:7 ; ///<
    UINT32                                                 SClkDid2:7 ; ///<
    UINT32                                            Reserved14_31:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107067_STRUCT;

// **** D0F0xBC_xC0107068 Field Definition ****
// Address
#define D0F0xBC_xC0107068_ADDRESS                               0xC0107068

// Type
#define D0F0xBC_xC0107068_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107068_Reserved0_5_OFFSET                    0
#define D0F0xBC_xC0107068_Reserved0_5_WIDTH                     6
#define D0F0xBC_xC0107068_Reserved0_5_MASK                      0x3f
#define D0F0xBC_xC0107068_SClkDid3_OFFSET                       6
#define D0F0xBC_xC0107068_SClkDid3_WIDTH                        7
#define D0F0xBC_xC0107068_SClkDid3_MASK                         0x1fc0
#define D0F0xBC_xC0107068_SClkDid4_OFFSET                       13
#define D0F0xBC_xC0107068_SClkDid4_WIDTH                        7
#define D0F0xBC_xC0107068_SClkDid4_MASK                         0xfe000
#define D0F0xBC_xC0107068_DispClkDid0_OFFSET                    20
#define D0F0xBC_xC0107068_DispClkDid0_WIDTH                     7
#define D0F0xBC_xC0107068_DispClkDid0_MASK                      0x7f00000
#define D0F0xBC_xC0107068_Reserved27_31_OFFSET                  27
#define D0F0xBC_xC0107068_Reserved27_31_WIDTH                   5
#define D0F0xBC_xC0107068_Reserved27_31_MASK                    0xf8000000

/// D0F0xBC_xC0107068
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_5:6 ; ///<
    UINT32                                                 SClkDid3:7 ; ///<
    UINT32                                                 SClkDid4:7 ; ///<
    UINT32                                              DispClkDid0:7 ; ///<
    UINT32                                            Reserved27_31:5 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107068_STRUCT;

// **** D0F0xBC_xC010706B Field Definition ****
// Address
#define D0F0xBC_xC010706B_ADDRESS                               0xC010706b

// Type
#define D0F0xBC_xC010706B_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010706B_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC010706B_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC010706B_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC010706B_DispClkDid1_OFFSET                    3
#define D0F0xBC_xC010706B_DispClkDid1_WIDTH                     7
#define D0F0xBC_xC010706B_DispClkDid1_MASK                      0x3f8
#define D0F0xBC_xC010706B_Reserved10_31_OFFSET                  10
#define D0F0xBC_xC010706B_Reserved10_31_WIDTH                   22
#define D0F0xBC_xC010706B_Reserved10_31_MASK                    0xfffffc00

/// D0F0xBC_xC010706B
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_2:3 ; ///<
    UINT32                                              DispClkDid1:7 ; ///<
    UINT32                                            Reserved10_31:22; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010706B_STRUCT;

// **** D0F0xBC_xC010706C Field Definition ****
// Address
#define D0F0xBC_xC010706C_ADDRESS                               0xC010706c

// Type
#define D0F0xBC_xC010706C_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010706C_Reserved0_1_OFFSET                    0
#define D0F0xBC_xC010706C_Reserved0_1_WIDTH                     2
#define D0F0xBC_xC010706C_Reserved0_1_MASK                      0x3
#define D0F0xBC_xC010706C_DispClkDid2_OFFSET                    2
#define D0F0xBC_xC010706C_DispClkDid2_WIDTH                     7
#define D0F0xBC_xC010706C_DispClkDid2_MASK                      0x1fc
#define D0F0xBC_xC010706C_DispClkDid3_OFFSET                    9
#define D0F0xBC_xC010706C_DispClkDid3_WIDTH                     7
#define D0F0xBC_xC010706C_DispClkDid3_MASK                      0xfe00
#define D0F0xBC_xC010706C_DispClkDid4_OFFSET                    16
#define D0F0xBC_xC010706C_DispClkDid4_WIDTH                     7
#define D0F0xBC_xC010706C_DispClkDid4_MASK                      0x7f0000
#define D0F0xBC_xC010706C_LClkDid0_OFFSET                       23
#define D0F0xBC_xC010706C_LClkDid0_WIDTH                        7
#define D0F0xBC_xC010706C_LClkDid0_MASK                         0x3f800000
#define D0F0xBC_xC010706C_Reserved30_31_OFFSET                  30
#define D0F0xBC_xC010706C_Reserved30_31_WIDTH                   2
#define D0F0xBC_xC010706C_Reserved30_31_MASK                    0xc0000000

/// D0F0xBC_xC010706C
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_1:2 ; ///<
    UINT32                                              DispClkDid2:7 ; ///<
    UINT32                                              DispClkDid3:7 ; ///<
    UINT32                                              DispClkDid4:7 ; ///<
    UINT32                                                 LClkDid0:7 ; ///<
    UINT32                                            Reserved30_31:2 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010706C_STRUCT;

// **** D0F0xBC_xC010706F Field Definition ****
// Address
#define D0F0xBC_xC010706F_ADDRESS                               0xC010706f

// Type
#define D0F0xBC_xC010706F_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010706F_Reserved0_5_OFFSET                    0
#define D0F0xBC_xC010706F_Reserved0_5_WIDTH                     6
#define D0F0xBC_xC010706F_Reserved0_5_MASK                      0x3f
#define D0F0xBC_xC010706F_LClkDid1_OFFSET                       6
#define D0F0xBC_xC010706F_LClkDid1_WIDTH                        7
#define D0F0xBC_xC010706F_LClkDid1_MASK                         0x1fc0
#define D0F0xBC_xC010706F_Reserved13_31_OFFSET                  13
#define D0F0xBC_xC010706F_Reserved13_31_WIDTH                   19
#define D0F0xBC_xC010706F_Reserved13_31_MASK                    0xffffe000

/// D0F0xBC_xC010706F
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_5:6 ; ///<
    UINT32                                                 LClkDid1:7 ; ///<
    UINT32                                            Reserved13_31:19; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010706F_STRUCT;

// **** D0F0xBC_xC0107070 Field Definition ****
// Address
#define D0F0xBC_xC0107070_ADDRESS                               0xC0107070

// Type
#define D0F0xBC_xC0107070_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107070_Reserved0_4_OFFSET                    0
#define D0F0xBC_xC0107070_Reserved0_4_WIDTH                     5
#define D0F0xBC_xC0107070_Reserved0_4_MASK                      0x1f
#define D0F0xBC_xC0107070_LClkDid2_OFFSET                       5
#define D0F0xBC_xC0107070_LClkDid2_WIDTH                        7
#define D0F0xBC_xC0107070_LClkDid2_MASK                         0xfe0
#define D0F0xBC_xC0107070_LClkDid3_OFFSET                       12
#define D0F0xBC_xC0107070_LClkDid3_WIDTH                        7
#define D0F0xBC_xC0107070_LClkDid3_MASK                         0x7f000
#define D0F0xBC_xC0107070_LClkDid4_OFFSET                       19
#define D0F0xBC_xC0107070_LClkDid4_WIDTH                        7
#define D0F0xBC_xC0107070_LClkDid4_MASK                         0x3f80000
#define D0F0xBC_xC0107070_Reserved26_31_OFFSET                  26
#define D0F0xBC_xC0107070_Reserved26_31_WIDTH                   6
#define D0F0xBC_xC0107070_Reserved26_31_MASK                    0xfc000000

/// D0F0xBC_xC0107070
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_4:5 ; ///<
    UINT32                                                 LClkDid2:7 ; ///<
    UINT32                                                 LClkDid3:7 ; ///<
    UINT32                                                 LClkDid4:7 ; ///<
    UINT32                                            Reserved26_31:6 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107070_STRUCT;


// **** D0F0xBC_xC0107073 Field Definition ****
// Address
#define D0F0xBC_xC0107073_ADDRESS                               0xC0107073

// Type
#define D0F0xBC_xC0107073_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107073_Reserved0_1_OFFSET                    0
#define D0F0xBC_xC0107073_Reserved0_1_WIDTH                     2
#define D0F0xBC_xC0107073_Reserved0_1_MASK                      0x3
#define D0F0xBC_xC0107073_DClkDid0_OFFSET                       2
#define D0F0xBC_xC0107073_DClkDid0_WIDTH                        7
#define D0F0xBC_xC0107073_DClkDid0_MASK                         0x1fc
#define D0F0xBC_xC0107073_Reserved9_31_OFFSET                   9
#define D0F0xBC_xC0107073_Reserved9_31_WIDTH                    23
#define D0F0xBC_xC0107073_Reserved9_31_MASK                     0xfffffe00

/// D0F0xBC_xC0107073
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_1:2 ; ///<
    UINT32                                                 DClkDid0:7 ; ///<
    UINT32                                             Reserved9_31:23; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107073_STRUCT;

// **** D0F0xBC_xC0107074 Field Definition ****
// Address
#define D0F0xBC_xC0107074_ADDRESS                               0xC0107074

// Type
#define D0F0xBC_xC0107074_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107074_Reserved0_0_OFFSET                    0
#define D0F0xBC_xC0107074_Reserved0_0_WIDTH                     1
#define D0F0xBC_xC0107074_Reserved0_0_MASK                      0x1
#define D0F0xBC_xC0107074_DClkDid1_OFFSET                       1
#define D0F0xBC_xC0107074_DClkDid1_WIDTH                        7
#define D0F0xBC_xC0107074_DClkDid1_MASK                         0xfe
#define D0F0xBC_xC0107074_DClkDid2_OFFSET                       8
#define D0F0xBC_xC0107074_DClkDid2_WIDTH                        7
#define D0F0xBC_xC0107074_DClkDid2_MASK                         0x7f00
#define D0F0xBC_xC0107074_DClkDid3_OFFSET                       15
#define D0F0xBC_xC0107074_DClkDid3_WIDTH                        7
#define D0F0xBC_xC0107074_DClkDid3_MASK                         0x3f8000
#define D0F0xBC_xC0107074_DClkDid4_OFFSET                       22
#define D0F0xBC_xC0107074_DClkDid4_WIDTH                        7
#define D0F0xBC_xC0107074_DClkDid4_MASK                         0x1fc00000
#define D0F0xBC_xC0107074_Reserved29_31_OFFSET                  29
#define D0F0xBC_xC0107074_Reserved29_31_WIDTH                   3
#define D0F0xBC_xC0107074_Reserved29_31_MASK                    0xe0000000

/// D0F0xBC_xC0107074
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_0:1 ; ///<
    UINT32                                                 DClkDid1:7 ; ///<
    UINT32                                                 DClkDid2:7 ; ///<
    UINT32                                                 DClkDid3:7 ; ///<
    UINT32                                                 DClkDid4:7 ; ///<
    UINT32                                            Reserved29_31:3 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107074_STRUCT;

// **** D0F0xBC_xC0107077 Field Definition ****
// Address
#define D0F0xBC_xC0107077_ADDRESS                               0xC0107077

// Type
#define D0F0xBC_xC0107077_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107077_Reserved0_4_OFFSET                    0
#define D0F0xBC_xC0107077_Reserved0_4_WIDTH                     5
#define D0F0xBC_xC0107077_Reserved0_4_MASK                      0x1f
#define D0F0xBC_xC0107077_VClkDid0_OFFSET                       5
#define D0F0xBC_xC0107077_VClkDid0_WIDTH                        7
#define D0F0xBC_xC0107077_VClkDid0_MASK                         0xfe0
#define D0F0xBC_xC0107077_Reserved12_31_OFFSET                  12
#define D0F0xBC_xC0107077_Reserved12_31_WIDTH                   20
#define D0F0xBC_xC0107077_Reserved12_31_MASK                    0xfffff000

/// D0F0xBC_xC0107077
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_4:5 ; ///<
    UINT32                                                 VClkDid0:7 ; ///<
    UINT32                                            Reserved12_31:20; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107077_STRUCT;

// **** D0F0xBC_xC0107078 Field Definition ****
// Address
#define D0F0xBC_xC0107078_ADDRESS                               0xC0107078

// Type
#define D0F0xBC_xC0107078_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107078_Reserved0_3_OFFSET                    0
#define D0F0xBC_xC0107078_Reserved0_3_WIDTH                     4
#define D0F0xBC_xC0107078_Reserved0_3_MASK                      0xf
#define D0F0xBC_xC0107078_VClkDid1_OFFSET                       4
#define D0F0xBC_xC0107078_VClkDid1_WIDTH                        7
#define D0F0xBC_xC0107078_VClkDid1_MASK                         0x7f0
#define D0F0xBC_xC0107078_VClkDid2_OFFSET                       11
#define D0F0xBC_xC0107078_VClkDid2_WIDTH                        7
#define D0F0xBC_xC0107078_VClkDid2_MASK                         0x3f800
#define D0F0xBC_xC0107078_VClkDid3_OFFSET                       18
#define D0F0xBC_xC0107078_VClkDid3_WIDTH                        7
#define D0F0xBC_xC0107078_VClkDid3_MASK                         0x1fc0000
#define D0F0xBC_xC0107078_VClkDid4_OFFSET                       25
#define D0F0xBC_xC0107078_VClkDid4_WIDTH                        7
#define D0F0xBC_xC0107078_VClkDid4_MASK                         0xfe000000

/// D0F0xBC_xC0107078
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_3:4 ; ///<
    UINT32                                                 VClkDid1:7 ; ///<
    UINT32                                                 VClkDid2:7 ; ///<
    UINT32                                                 VClkDid3:7 ; ///<
    UINT32                                                 VClkDid4:7 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107078_STRUCT;

// **** D0F0xBC_xC010707C Field Definition ****
// Address
#define D0F0xBC_xC010707C_ADDRESS                               0xC010707c

// Type
#define D0F0xBC_xC010707C_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010707C_EClkDid0_OFFSET                       0
#define D0F0xBC_xC010707C_EClkDid0_WIDTH                        7
#define D0F0xBC_xC010707C_EClkDid0_MASK                         0x7f
#define D0F0xBC_xC010707C_EClkDid1_OFFSET                       7
#define D0F0xBC_xC010707C_EClkDid1_WIDTH                        7
#define D0F0xBC_xC010707C_EClkDid1_MASK                         0x3f80
#define D0F0xBC_xC010707C_EClkDid2_OFFSET                       14
#define D0F0xBC_xC010707C_EClkDid2_WIDTH                        7
#define D0F0xBC_xC010707C_EClkDid2_MASK                         0x1fc000
#define D0F0xBC_xC010707C_EClkDid3_OFFSET                       21
#define D0F0xBC_xC010707C_EClkDid3_WIDTH                        7
#define D0F0xBC_xC010707C_EClkDid3_MASK                         0xfe00000
#define D0F0xBC_xC010707C_Reserved28_31_OFFSET                  28
#define D0F0xBC_xC010707C_Reserved28_31_WIDTH                   4
#define D0F0xBC_xC010707C_Reserved28_31_MASK                    0xf0000000

/// D0F0xBC_xC010707C
typedef union {
  struct {                                                              ///<
    UINT32                                                 EClkDid0:7 ; ///<
    UINT32                                                 EClkDid1:7 ; ///<
    UINT32                                                 EClkDid2:7 ; ///<
    UINT32                                                 EClkDid3:7 ; ///<
    UINT32                                            Reserved28_31:4 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010707C_STRUCT;

// **** D0F0xBC_xC010707F Field Definition ****
// Address
#define D0F0xBC_xC010707F_ADDRESS                               0xC010707f

// Type
#define D0F0xBC_xC010707F_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010707F_Reserved0_3_OFFSET                    0
#define D0F0xBC_xC010707F_Reserved0_3_WIDTH                     4
#define D0F0xBC_xC010707F_Reserved0_3_MASK                      0xf
#define D0F0xBC_xC010707F_EClkDid4_OFFSET                       4
#define D0F0xBC_xC010707F_EClkDid4_WIDTH                        7
#define D0F0xBC_xC010707F_EClkDid4_MASK                         0x7f0
#define D0F0xBC_xC010707F_Reserved11_31_OFFSET                  11
#define D0F0xBC_xC010707F_Reserved11_31_WIDTH                   21
#define D0F0xBC_xC010707F_Reserved11_31_MASK                    0xfffff800

/// D0F0xBC_xC010707F
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_3:4 ; ///<
    UINT32                                                 EClkDid4:7 ; ///<
    UINT32                                            Reserved11_31:21; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010707F_STRUCT;

// **** D0F0xBC_xC0107080 Field Definition ****
// Address
#define D0F0xBC_xC0107080_ADDRESS                               0xC0107080

// Type
#define D0F0xBC_xC0107080_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107080_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC0107080_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC0107080_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC0107080_PowerplayPerfFlag_OFFSET              3
#define D0F0xBC_xC0107080_PowerplayPerfFlag_WIDTH               5
#define D0F0xBC_xC0107080_PowerplayPerfFlag_MASK                0xf8
#define D0F0xBC_xC0107080_PowerplayPowerSavingsFlag_OFFSET      8
#define D0F0xBC_xC0107080_PowerplayPowerSavingsFlag_WIDTH       5
#define D0F0xBC_xC0107080_PowerplayPowerSavingsFlag_MASK        0x1f00
#define D0F0xBC_xC0107080_SamClkDid0_OFFSET                     13
#define D0F0xBC_xC0107080_SamClkDid0_WIDTH                      7
#define D0F0xBC_xC0107080_SamClkDid0_MASK                       0xfe000
#define D0F0xBC_xC0107080_SamClkDid1_OFFSET                     20
#define D0F0xBC_xC0107080_SamClkDid1_WIDTH                      7
#define D0F0xBC_xC0107080_SamClkDid1_MASK                       0x7f00000
#define D0F0xBC_xC0107080_Reserved27_31_OFFSET                  27
#define D0F0xBC_xC0107080_Reserved27_31_WIDTH                   5
#define D0F0xBC_xC0107080_Reserved27_31_MASK                    0xf8000000

/// D0F0xBC_xC0107080
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_2:3 ; ///<
    UINT32                                        PowerplayPerfFlag:5 ; ///<
    UINT32                                PowerplayPowerSavingsFlag:5 ; ///<
    UINT32                                               SamClkDid0:7 ; ///<
    UINT32                                               SamClkDid1:7 ; ///<
    UINT32                                            Reserved27_31:5 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107080_STRUCT;

// **** D0F0xBC_xC0107083 Field Definition ****
// Address
#define D0F0xBC_xC0107083_ADDRESS                               0xC0107083

// Type
#define D0F0xBC_xC0107083_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107083_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC0107083_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC0107083_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC0107083_SamClkDid2_OFFSET                     3
#define D0F0xBC_xC0107083_SamClkDid2_WIDTH                      7
#define D0F0xBC_xC0107083_SamClkDid2_MASK                       0x3f8
#define D0F0xBC_xC0107083_Reserved10_31_OFFSET                  10
#define D0F0xBC_xC0107083_Reserved10_31_WIDTH                   22
#define D0F0xBC_xC0107083_Reserved10_31_MASK                    0xfffffc00

/// D0F0xBC_xC0107083
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_2:3 ; ///<
    UINT32                                               SamClkDid2:7 ; ///<
    UINT32                                            Reserved10_31:22; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107083_STRUCT;

// **** D0F0xBC_xC0107084 Field Definition ****
// Address
#define D0F0xBC_xC0107084_ADDRESS                               0xC0107084

// Type
#define D0F0xBC_xC0107084_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107084_Reserved0_1_OFFSET                    0
#define D0F0xBC_xC0107084_Reserved0_1_WIDTH                     2
#define D0F0xBC_xC0107084_Reserved0_1_MASK                      0x3
#define D0F0xBC_xC0107084_SamClkDid3_OFFSET                     2
#define D0F0xBC_xC0107084_SamClkDid3_WIDTH                      7
#define D0F0xBC_xC0107084_SamClkDid3_MASK                       0x1fc
#define D0F0xBC_xC0107084_SamClkDid4_OFFSET                     9
#define D0F0xBC_xC0107084_SamClkDid4_WIDTH                      7
#define D0F0xBC_xC0107084_SamClkDid4_MASK                       0xfe00
#define D0F0xBC_xC0107084_Reserved16_31_OFFSET                  16
#define D0F0xBC_xC0107084_Reserved16_31_WIDTH                   16
#define D0F0xBC_xC0107084_Reserved16_31_MASK                    0xffff0000

/// D0F0xBC_xC0107084
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_1:2 ; ///<
    UINT32                                               SamClkDid3:7 ; ///<
    UINT32                                               SamClkDid4:7 ; ///<
    UINT32                                            Reserved16_31:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107084_STRUCT;

// **** D0F0xBC_xC0107097 Field Definition ****
// Address
#define D0F0xBC_xC0107097_ADDRESS                               0xC0107097

// Type
#define D0F0xBC_xC0107097_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107097_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC0107097_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC0107097_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC0107097_HtcTmpLmt_OFFSET                      3
#define D0F0xBC_xC0107097_HtcTmpLmt_WIDTH                       7
#define D0F0xBC_xC0107097_HtcTmpLmt_MASK                        0x3f8
#define D0F0xBC_xC0107097_Reserved10_31_OFFSET                  10
#define D0F0xBC_xC0107097_Reserved10_31_WIDTH                   22
#define D0F0xBC_xC0107097_Reserved10_31_MASK                    0xffff0000

/// D0F0xBC_xC0107097
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_2:3 ; ///<
    UINT32                                                HtcTmpLmt:7 ; ///<
    UINT32                                            Reserved10_31:22; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107097_STRUCT;

// **** D0F0xBC_xC00C0000 Register Definition ****
// Address
#define D0F0xBC_xC00C0000_ADDRESS                               0xc00c0000

// Type
#define D0F0xBC_xC00C0000_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC00C0000_Reserved0_0_OFFSET                    0
#define D0F0xBC_xC00C0000_Reserved0_0_WIDTH                     1
#define D0F0xBC_xC00C0000_Reserved0_0_MASK                      0x1
#define D0F0xBC_xC00C0000_GPU_DIS_OFFSET                        1
#define D0F0xBC_xC00C0000_GPU_DIS_WIDTH                         1
#define D0F0xBC_xC00C0000_GPU_DIS_MASK                          0x2
#define D0F0xBC_xC00C0000_Reserved2_31_OFFSET                   2
#define D0F0xBC_xC00C0000_Reserved2_31_WIDTH                    30
#define D0F0xBC_xC00C0000_Reserved2_31_MASK                     0xfffffffc

/// D0F0xBC_xC00C0000
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_0:1 ; ///<
    UINT32                                                  GPU_DIS:1 ; ///<
    UINT32                                             Reserved2_31:30; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC00C0000_STRUCT;

// **** D0F0xBC_x1F630 Register Definition ****
// Address
#define D0F0xBC_x1F630_ADDRESS                                  0x1f630

// Type
#define D0F0xBC_x1F630_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x1F630_RECONF_WAIT_OFFSET                       0
#define D0F0xBC_x1F630_RECONF_WAIT_WIDTH                        8
#define D0F0xBC_x1F630_RECONF_WAIT_MASK                         0xff
#define D0F0xBC_x1F630_RECONF_WRAPPER_OFFSET                    8
#define D0F0xBC_x1F630_RECONF_WRAPPER_WIDTH                     8
#define D0F0xBC_x1F630_RECONF_WRAPPER_MASK                      0x00ff00

/// D0F0xBC_x1F630
typedef union {
  struct {                                                              ///<
    UINT32                                              RECONF_WAIT:8;  ///<
    UINT32                                           RECONF_WRAPPER:8;  ///<
    UINT32                                                Reserved:16;  ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x1F630_STRUCT;

/// D0F0xBC_x3FA04
typedef union {
  struct {                                                            ///<
    UINT32                                           Bitfield_15_0:16; ///<
    UINT32                                          Bitfield_31_16:16; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_x3FA04_STRUCT;

// **** D0F0xE4_PHY_0004 Register Definition ****
// Address
#define D0F0xE4_PHY_0004_ADDRESS                                0x0004
// Type
#define D0F0xE4_PHY_0004_TYPE                                   TYPE_D0F0xE4

// Field Data
#define D0F0xE4_PHY_0004_Reserved_15_0_OFFSET                   0
#define D0F0xE4_PHY_0004_Reserved_15_0_WIDTH                    16
#define D0F0xE4_PHY_0004_Reserved_15_0_MASK                     0xFFFF
#define D0F0xE4_PHY_0004_CfgIdleDetTh_OFFSET                    16
#define D0F0xE4_PHY_0004_CfgIdleDetTh_WIDTH                     2
#define D0F0xE4_PHY_0004_CfgIdleDetTh_MASK                      0x30000
#define D0F0xE4_PHY_0004_Reserved_31_18_OFFSET                  18
#define D0F0xE4_PHY_0004_Reserved_31_18_WIDTH                   14
#define D0F0xE4_PHY_0004_Reserved_31_18_MASK                    0xFFFC0000

/// D0F0xE4_PHY_0004
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_15_0:16; ///<
    UINT32                                            CfgIdleDetTh:2 ; ///<
    UINT32                                          Reserved_31_18:14; ///<
  } Field;
  UINT32 Value;
} D0F0xE4_PHY_0004_STRUCT;

// **** D0F0xE4_PHY_4440 Register Definition ****
// Address
#define D0F0xE4_PHY_4440_ADDRESS                                0x4440
// Type
#define D0F0xE4_PHY_4440_TYPE                                   TYPE_D0F0xE4

// Field Data
#define D0F0xE4_PHY_4440_Reserved_12_0_OFFSET                   0
#define D0F0xE4_PHY_4440_Reserved_12_0_WIDTH                    13
#define D0F0xE4_PHY_4440_Reserved_12_0_MASK                     0x1FFF
#define D0F0xE4_PHY_4440_PllDbgRoIPFDResetCntrl_OFFSET          13
#define D0F0xE4_PHY_4440_PllDbgRoIPFDResetCntrl_WIDTH           2
#define D0F0xE4_PHY_4440_PllDbgRoIPFDResetCntrl_MASK            0x6000
#define D0F0xE4_PHY_4440_Reserved_31_15_OFFSET                  15
#define D0F0xE4_PHY_4440_Reserved_31_15_WIDTH                   17
#define D0F0xE4_PHY_4440_Reserved_31_15_MASK                    0xFFFF1000

/// D0F0xE4_PHY_4440
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_12_0:13; ///<
    UINT32                                  PllDbgRoIPFDResetCntrl:2 ; ///<
    UINT32                                          Reserved_31_15:17; ///<
  } Field;
  UINT32 Value;
} D0F0xE4_PHY_4440_STRUCT;

// **** D0F0xE4_PHY_4450 Register Definition ****
// Address
#define D0F0xE4_PHY_4450_ADDRESS                                0x4450
// Type
#define D0F0xE4_PHY_4450_TYPE                                   TYPE_D0F0xE4

// Field Data
#define D0F0xE4_PHY_4450_PllCfgROBWCntrlOvrdVal0_OFFSET         0
#define D0F0xE4_PHY_4450_PllCfgROBWCntrlOvrdVal0_WIDTH          8
#define D0F0xE4_PHY_4450_PllCfgROBWCntrlOvrdVal0_MASK           0xFF
#define D0F0xE4_PHY_4450_Reserved_29_8_OFFSET                   8
#define D0F0xE4_PHY_4450_Reserved_29_8_WIDTH                    22
#define D0F0xE4_PHY_4450_Reserved_29_8_MASK                     0x3FFFFF00
#define D0F0xE4_PHY_4450_PllCfgROVTOIBiasCntrlOvrdVal0_OFFSET   30
#define D0F0xE4_PHY_4450_PllCfgROVTOIBiasCntrlOvrdVal0_WIDTH    1
#define D0F0xE4_PHY_4450_PllCfgROVTOIBiasCntrlOvrdVal0_MASK     0x40000000
#define D0F0xE4_PHY_4450_Reserved_31_31_OFFSET                  31
#define D0F0xE4_PHY_4450_Reserved_31_31_WIDTH                   1
#define D0F0xE4_PHY_4450_Reserved_31_31_MASK                    0x80000000

/// D0F0xE4_PHY_4450
typedef union {
  struct {                                                            ///<
    UINT32                                 PllCfgROBWCntrlOvrdVal0:8 ; ///<
    UINT32                                           Reserved_29_8:22; ///<
    UINT32                           PllCfgROVTOIBiasCntrlOvrdVal0:1 ; ///<
    UINT32                                          Reserved_31_31:1 ; ///<
  } Field;
  UINT32 Value;
} D0F0xE4_PHY_4450_STRUCT;

// **** D0F0xE4_WRAP_0800 Register Definition ****
// Address
#define D0F0xE4_WRAP_0800_ADDRESS                               0x800
// Type
#define D0F0xE4_WRAP_0800_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_0800_HoldTraining_OFFSET                   0
#define D0F0xE4_WRAP_0800_HoldTraining_WIDTH                    1
#define D0F0xE4_WRAP_0800_HoldTraining_MASK                     0x1
#define D0F0xE4_WRAP_0800_Reserved_31_1_OFFSET                  1
#define D0F0xE4_WRAP_0800_Reserved_31_1_WIDTH                   31
#define D0F0xE4_WRAP_0800_Reserved_31_1_MASK                    0xFFFFFFFE

/// D0F0xE4_WRAP_0800
typedef union {
  struct {                                                            ///<
    UINT32                                            HoldTraining:1; ///<
    UINT32                                           Reserved_31_1:31; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_0800_STRUCT;

// **** D0F0xE4_WRAP_0805 Register Definition ****
// Address
#define D0F0xE4_WRAP_0805_ADDRESS                               0x805
// Type
#define D0F0xE4_WRAP_0805_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_0805_Reserved_23_0_OFFSET                   0
#define D0F0xE4_WRAP_0805_Reserved_23_0_WIDTH                    24
#define D0F0xE4_WRAP_0805_Reserved_23_0_MASK                     0x0FFFFFF
#define D0F0xE4_WRAP_0805_STRAP_BIF_INITIAL_N_FTS_A_OFFSET       24
#define D0F0xE4_WRAP_0805_STRAP_BIF_INITIAL_N_FTS_A_WIDTH        8
#define D0F0xE4_WRAP_0805_STRAP_BIF_INITIAL_N_FTS_A_MASK         0x0FF000000

/// D0F0xE4_WRAP_0805
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_23_0:24; ///<
    UINT32                               STRAP_BIF_INITIAL_N_FTS_A:8; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_0805_STRUCT;

// **** D0F0xE4_WRAP_0905 Register Definition ****
// Address
#define D0F0xE4_WRAP_0905_ADDRESS                               0x905
// Type
#define D0F0xE4_WRAP_0905_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_0905_Reserved_23_0_OFFSET                   0
#define D0F0xE4_WRAP_0905_Reserved_23_0_WIDTH                    24
#define D0F0xE4_WRAP_0905_Reserved_23_0_MASK                     0x0FFFFFF
#define D0F0xE4_WRAP_0905_STRAP_BIF_INITIAL_N_FTS_B_OFFSET       24
#define D0F0xE4_WRAP_0905_STRAP_BIF_INITIAL_N_FTS_B_WIDTH        8
#define D0F0xE4_WRAP_0905_STRAP_BIF_INITIAL_N_FTS_B_MASK         0x0FF000000

/// D0F0xE4_WRAP_0905
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_23_0:24; ///<
    UINT32                               STRAP_BIF_INITIAL_N_FTS_B:8; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_0905_STRUCT;

// **** D0F0xE4_WRAP_0A05 Register Definition ****
// Address
#define D0F0xE4_WRAP_0A05_ADDRESS                               0x0A05
// Type
#define D0F0xE4_WRAP_0A05_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_0A05_Reserved_23_0_OFFSET                   0
#define D0F0xE4_WRAP_0A05_Reserved_23_0_WIDTH                    24
#define D0F0xE4_WRAP_0A05_Reserved_23_0_MASK                     0x0FFFFFF
#define D0F0xE4_WRAP_0A05_STRAP_BIF_INITIAL_N_FTS_C_OFFSET       24
#define D0F0xE4_WRAP_0A05_STRAP_BIF_INITIAL_N_FTS_C_WIDTH        8
#define D0F0xE4_WRAP_0A05_STRAP_BIF_INITIAL_N_FTS_C_MASK         0x0FF000000

/// D0F0xE4_WRAP_0A05
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_23_0:24; ///<
    UINT32                               STRAP_BIF_INITIAL_N_FTS_C:8; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_0A05_STRUCT;

// **** D0F0xE4_WRAP_0B05 Register Definition ****
// Address
#define D0F0xE4_WRAP_0B05_ADDRESS                               0x0B05
// Type
#define D0F0xE4_WRAP_0B05_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_0B05_Reserved_23_0_OFFSET                   0
#define D0F0xE4_WRAP_0B05_Reserved_23_0_WIDTH                    24
#define D0F0xE4_WRAP_0B05_Reserved_23_0_MASK                     0x0FFFFFF
#define D0F0xE4_WRAP_0B05_STRAP_BIF_INITIAL_N_FTS_D_OFFSET       24
#define D0F0xE4_WRAP_0B05_STRAP_BIF_INITIAL_N_FTS_D_WIDTH        8
#define D0F0xE4_WRAP_0B05_STRAP_BIF_INITIAL_N_FTS_D_MASK         0x0FF000000

/// D0F0xE4_WRAP_0B05
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_23_0:24; ///<
    UINT32                               STRAP_BIF_INITIAL_N_FTS_D:8; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_0B05_STRUCT;

// **** D0F0xE4_WRAP_0C05 Register Definition ****
// Address
#define D0F0xE4_WRAP_0C05_ADDRESS                               0x0C05
// Type
#define D0F0xE4_WRAP_0C05_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_0C05_Reserved_23_0_OFFSET                   0
#define D0F0xE4_WRAP_0C05_Reserved_23_0_WIDTH                    24
#define D0F0xE4_WRAP_0C05_Reserved_23_0_MASK                     0x0FFFFFF
#define D0F0xE4_WRAP_0C05_STRAP_BIF_INITIAL_N_FTS_E_OFFSET       24
#define D0F0xE4_WRAP_0C05_STRAP_BIF_INITIAL_N_FTS_E_WIDTH        8
#define D0F0xE4_WRAP_0C05_STRAP_BIF_INITIAL_N_FTS_E_MASK         0x0FF000000

/// D0F0xE4_WRAP_0C05
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_23_0:24; ///<
    UINT32                               STRAP_BIF_INITIAL_N_FTS_E:8; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_0C05_STRUCT;

// **** D0F0xE4_WRAP_8011 Register Definition ****
// Address
#define D0F0xE4_WRAP_8011_ADDRESS                               0x8011
// Type
#define D0F0xE4_WRAP_8011_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_8011_TxclkDynGateLatency_OFFSET            0
#define D0F0xE4_WRAP_8011_TxclkDynGateLatency_WIDTH             6
#define D0F0xE4_WRAP_8011_TxclkDynGateLatency_MASK              0x3F
#define D0F0xE4_WRAP_8011_TxclkPermGateEven_OFFSET              6
#define D0F0xE4_WRAP_8011_TxclkPermGateEven_WIDTH               1
#define D0F0xE4_WRAP_8011_TxclkPermGateEven_MASK                0x40
#define D0F0xE4_WRAP_8011_TxclkDynGateEnable_OFFSET             7
#define D0F0xE4_WRAP_8011_TxclkDynGateEnable_WIDTH              1
#define D0F0xE4_WRAP_8011_TxclkDynGateEnable_MASK               0x80
#define D0F0xE4_WRAP_8011_TxclkPermStop_OFFSET                  8
#define D0F0xE4_WRAP_8011_TxclkPermStop_WIDTH                   1
#define D0F0xE4_WRAP_8011_TxclkPermStop_MASK                    0x100
#define D0F0xE4_WRAP_8011_TxclkRegsGateEnable_OFFSET            9
#define D0F0xE4_WRAP_8011_TxclkRegsGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8011_TxclkRegsGateEnable_MASK              0x200
#define D0F0xE4_WRAP_8011_TxclkRegsGateLatency_OFFSET           10
#define D0F0xE4_WRAP_8011_TxclkRegsGateLatency_WIDTH            6
#define D0F0xE4_WRAP_8011_TxclkRegsGateLatency_MASK             0xFC00
#define D0F0xE4_WRAP_8011_Reserved_16_16_OFFSET                 16
#define D0F0xE4_WRAP_8011_Reserved_16_16_WIDTH                  1
#define D0F0xE4_WRAP_8011_Reserved_16_16_MASK                   0x10000
#define D0F0xE4_WRAP_8011_TxclkPermGateLatency_OFFSET           17
#define D0F0xE4_WRAP_8011_TxclkPermGateLatency_WIDTH            6
#define D0F0xE4_WRAP_8011_TxclkPermGateLatency_MASK             0x7E0000
#define D0F0xE4_WRAP_8011_Bitfield_23_23_OFFSET                 23
#define D0F0xE4_WRAP_8011_Bitfield_23_23_WIDTH                  1
#define D0F0xE4_WRAP_8011_Bitfield_23_23_MASK                   0x800000
#define D0F0xE4_WRAP_8011_TxclkLcntGateEnable_OFFSET            24
#define D0F0xE4_WRAP_8011_TxclkLcntGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8011_TxclkLcntGateEnable_MASK              0x1000000
#define D0F0xE4_WRAP_8011_Reserved_25_25_OFFSET                 25
#define D0F0xE4_WRAP_8011_Reserved_25_25_WIDTH                  1
#define D0F0xE4_WRAP_8011_Reserved_25_25_MASK                   0x2000000
#define D0F0xE4_WRAP_8011_Reserved_31_26_OFFSET                 26
#define D0F0xE4_WRAP_8011_Reserved_31_26_WIDTH                  6
#define D0F0xE4_WRAP_8011_Reserved_31_26_MASK                   0xFC000000

/// D0F0xE4_WRAP_8011
typedef union {
  struct {                                                            ///<
    UINT32                                     TxclkDynGateLatency:6; ///<
    UINT32                                       TxclkPermGateEven:1; ///<
    UINT32                                      TxclkDynGateEnable:1; ///<
    UINT32                                           TxclkPermStop:1; ///<
    UINT32                                     TxclkRegsGateEnable:1; ///<
    UINT32                                    TxclkRegsGateLatency:6; ///<
    UINT32                                          Reserved_16_16:1; ///<
    UINT32                                    TxclkPermGateLatency:6; ///<
    UINT32                                          Bitfield_23_23:1; ///<
    UINT32                                     TxclkLcntGateEnable:1; ///<
    UINT32                                          Reserved_25_25:1; ///<
    UINT32                                          Reserved_31_26:6; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_8011_STRUCT;

// **** D0F0xE4_WRAP_8012 Register Definition ****
// Address
#define D0F0xE4_WRAP_8012_ADDRESS                               0x8012
// Type
#define D0F0xE4_WRAP_8012_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_8012_Pif1xIdleGateLatency_OFFSET           0
#define D0F0xE4_WRAP_8012_Pif1xIdleGateLatency_WIDTH            6
#define D0F0xE4_WRAP_8012_Pif1xIdleGateLatency_MASK             0x3F
#define D0F0xE4_WRAP_8012_Reserved_6_6_OFFSET                   6
#define D0F0xE4_WRAP_8012_Reserved_6_6_WIDTH                    1
#define D0F0xE4_WRAP_8012_Reserved_6_6_MASK                     0x40
#define D0F0xE4_WRAP_8012_Pif1xIdleGateEnable_OFFSET            7
#define D0F0xE4_WRAP_8012_Pif1xIdleGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8012_Pif1xIdleGateEnable_MASK              0x80
#define D0F0xE4_WRAP_8012_Pif1xIdleResumeLatency_OFFSET         8
#define D0F0xE4_WRAP_8012_Pif1xIdleResumeLatency_WIDTH          6
#define D0F0xE4_WRAP_8012_Pif1xIdleResumeLatency_MASK           0x3F00
#define D0F0xE4_WRAP_8012_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_8012_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_8012_Reserved_31_14_MASK                   0xFFFFC000

/// D0F0xE4_WRAP_8012
typedef union {
  struct {                                                            ///<
    UINT32                                    Pif1xIdleGateLatency:6; ///<
    UINT32                                            Reserved_6_6:1; ///<
    UINT32                                     Pif1xIdleGateEnable:1; ///<
    UINT32                                  Pif1xIdleResumeLatency:6; ///<
    UINT32                                          Reserved_31_14:18; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_8012_STRUCT;

// **** D0F0xE4_WRAP_8013 Register Definition ****
// Address
#define D0F0xE4_WRAP_8013_ADDRESS                               0x8013
// Type
#define D0F0xE4_WRAP_8013_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_8013_MasterPciePllA_OFFSET                 0
#define D0F0xE4_WRAP_8013_MasterPciePllA_WIDTH                  1
#define D0F0xE4_WRAP_8013_MasterPciePllA_MASK                   0x1
#define D0F0xE4_WRAP_8013_MasterPciePllB_OFFSET                 1
#define D0F0xE4_WRAP_8013_MasterPciePllB_WIDTH                  1
#define D0F0xE4_WRAP_8013_MasterPciePllB_MASK                   0x2
#define D0F0xE4_WRAP_8013_Reserved_2_2_OFFSET                   2
#define D0F0xE4_WRAP_8013_Reserved_2_2_WIDTH                    1
#define D0F0xE4_WRAP_8013_Reserved_2_2_MASK                     0x4
#define D0F0xE4_WRAP_8013_Reserved_3_3_OFFSET                   3
#define D0F0xE4_WRAP_8013_Reserved_3_3_WIDTH                    1
#define D0F0xE4_WRAP_8013_Reserved_3_3_MASK                     0x8
#define D0F0xE4_WRAP_8013_ClkDividerResetOverrideA_OFFSET       4
#define D0F0xE4_WRAP_8013_ClkDividerResetOverrideA_WIDTH        1
#define D0F0xE4_WRAP_8013_ClkDividerResetOverrideA_MASK         0x10
#define D0F0xE4_WRAP_8013_ClkDividerResetOverrideB_OFFSET       5
#define D0F0xE4_WRAP_8013_ClkDividerResetOverrideB_WIDTH        1
#define D0F0xE4_WRAP_8013_ClkDividerResetOverrideB_MASK         0x20
#define D0F0xE4_WRAP_8013_Reserved_6_6_OFFSET                   6
#define D0F0xE4_WRAP_8013_Reserved_6_6_WIDTH                    1
#define D0F0xE4_WRAP_8013_Reserved_6_6_MASK                     0x40
#define D0F0xE4_WRAP_8013_Reserved_7_7_OFFSET                   7
#define D0F0xE4_WRAP_8013_Reserved_7_7_WIDTH                    1
#define D0F0xE4_WRAP_8013_Reserved_7_7_MASK                     0x80
#define D0F0xE4_WRAP_8013_TxclkSelCoreOverride_OFFSET           8
#define D0F0xE4_WRAP_8013_TxclkSelCoreOverride_WIDTH            1
#define D0F0xE4_WRAP_8013_TxclkSelCoreOverride_MASK             0x100
#define D0F0xE4_WRAP_8013_TxclkSelPifAOverride_OFFSET           9
#define D0F0xE4_WRAP_8013_TxclkSelPifAOverride_WIDTH            1
#define D0F0xE4_WRAP_8013_TxclkSelPifAOverride_MASK             0x200
#define D0F0xE4_WRAP_8013_TxclkSelPifBOverride_OFFSET           10
#define D0F0xE4_WRAP_8013_TxclkSelPifBOverride_WIDTH            1
#define D0F0xE4_WRAP_8013_TxclkSelPifBOverride_MASK             0x400
#define D0F0xE4_WRAP_8013_Reserved_11_11_OFFSET                 11
#define D0F0xE4_WRAP_8013_Reserved_11_11_WIDTH                  1
#define D0F0xE4_WRAP_8013_Reserved_11_11_MASK                   0x800
#define D0F0xE4_WRAP_8013_Reserved_12_12_OFFSET                 12
#define D0F0xE4_WRAP_8013_Reserved_12_12_WIDTH                  1
#define D0F0xE4_WRAP_8013_Reserved_12_12_MASK                   0x1000
#define D0F0xE4_WRAP_8013_PhyRxIsoDis_OFFSET                    13
#define D0F0xE4_WRAP_8013_PhyRxIsoDis_WIDTH                     2
#define D0F0xE4_WRAP_8013_PhyRxIsoDis_MASK                      0x6000
#define D0F0xE4_WRAP_8013_Reserved_31_15_OFFSET                 15
#define D0F0xE4_WRAP_8013_Reserved_31_15_WIDTH                  17
#define D0F0xE4_WRAP_8013_Reserved_31_15_MASK                   0xFFFF8000

/// D0F0xE4_WRAP_8013
typedef union {
  struct {                                                            ///<
    UINT32                                          MasterPciePllA:1; ///<
    UINT32                                          MasterPciePllB:1; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                ClkDividerResetOverrideA:1; ///<
    UINT32                                ClkDividerResetOverrideB:1; ///<
    UINT32                                            Reserved_6_6:1; ///<
    UINT32                                            Reserved_7_7:1; ///<
    UINT32                                    TxclkSelCoreOverride:1; ///<
    UINT32                                    TxclkSelPifAOverride:1; ///<
    UINT32                                    TxclkSelPifBOverride:1; ///<
    UINT32                                          Reserved_11_11:1; ///<
    UINT32                                          Reserved_12_12:1; ///<
    UINT32                                             PhyRxIsoDis:2; ///<
    UINT32                                          Reserved_31_15:17; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_8013_STRUCT;

// **** D0F0xE4_WRAP_8014 Register Definition ****
// Address
#define D0F0xE4_WRAP_8014_ADDRESS                               0x8014
// Type
#define D0F0xE4_WRAP_8014_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_8014_TxclkPermGateEnable_OFFSET            0
#define D0F0xE4_WRAP_8014_TxclkPermGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8014_TxclkPermGateEnable_MASK              0x1
#define D0F0xE4_WRAP_8014_TxclkPrbsGateEnable_OFFSET            1
#define D0F0xE4_WRAP_8014_TxclkPrbsGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8014_TxclkPrbsGateEnable_MASK              0x2
#define D0F0xE4_WRAP_8014_Reserved_2_2_OFFSET                   2
#define D0F0xE4_WRAP_8014_Reserved_2_2_WIDTH                    1
#define D0F0xE4_WRAP_8014_Reserved_2_2_MASK                     0x4
#define D0F0xE4_WRAP_8014_Reserved_3_3_OFFSET                   3
#define D0F0xE4_WRAP_8014_Reserved_3_3_WIDTH                    1
#define D0F0xE4_WRAP_8014_Reserved_3_3_MASK                     0x8
#define D0F0xE4_WRAP_8014_Reserved_4_4_OFFSET                   4
#define D0F0xE4_WRAP_8014_Reserved_4_4_WIDTH                    1
#define D0F0xE4_WRAP_8014_Reserved_4_4_MASK                     0x10
#define D0F0xE4_WRAP_8014_Reserved_5_5_OFFSET                   5
#define D0F0xE4_WRAP_8014_Reserved_5_5_WIDTH                    1
#define D0F0xE4_WRAP_8014_Reserved_5_5_MASK                     0x20
#define D0F0xE4_WRAP_8014_Reserved_11_6_OFFSET                  6
#define D0F0xE4_WRAP_8014_Reserved_11_6_WIDTH                   6
#define D0F0xE4_WRAP_8014_Reserved_11_6_MASK                    0xFC0
#define D0F0xE4_WRAP_8014_PcieGatePifA1xEnable_OFFSET           12
#define D0F0xE4_WRAP_8014_PcieGatePifA1xEnable_WIDTH            1
#define D0F0xE4_WRAP_8014_PcieGatePifA1xEnable_MASK             0x1000
#define D0F0xE4_WRAP_8014_PcieGatePifB1xEnable_OFFSET           13
#define D0F0xE4_WRAP_8014_PcieGatePifB1xEnable_WIDTH            1
#define D0F0xE4_WRAP_8014_PcieGatePifB1xEnable_MASK             0x2000
#define D0F0xE4_WRAP_8014_Reserved_14_14_OFFSET                 14
#define D0F0xE4_WRAP_8014_Reserved_14_14_WIDTH                  1
#define D0F0xE4_WRAP_8014_Reserved_14_14_MASK                   0x4000
#define D0F0xE4_WRAP_8014_Reserved_15_15_OFFSET                 15
#define D0F0xE4_WRAP_8014_Reserved_15_15_WIDTH                  1
#define D0F0xE4_WRAP_8014_Reserved_15_15_MASK                   0x8000
#define D0F0xE4_WRAP_8014_Reserved_19_16_OFFSET                 16
#define D0F0xE4_WRAP_8014_Reserved_19_16_WIDTH                  4
#define D0F0xE4_WRAP_8014_Reserved_19_16_MASK                   0xF0000
#define D0F0xE4_WRAP_8014_TxclkPermGateOnlyWhenPllPwrDn_OFFSET  20
#define D0F0xE4_WRAP_8014_TxclkPermGateOnlyWhenPllPwrDn_WIDTH   1
#define D0F0xE4_WRAP_8014_TxclkPermGateOnlyWhenPllPwrDn_MASK    0x100000
#define D0F0xE4_WRAP_8014_Reserved_23_21_OFFSET                 21
#define D0F0xE4_WRAP_8014_Reserved_23_21_WIDTH                  3
#define D0F0xE4_WRAP_8014_Reserved_23_21_MASK                   0xE00000
#define D0F0xE4_WRAP_8014_Reserved_24_24_OFFSET                 24
#define D0F0xE4_WRAP_8014_Reserved_24_24_WIDTH                  1
#define D0F0xE4_WRAP_8014_Reserved_24_24_MASK                   0x1000000
#define D0F0xE4_WRAP_8014_Reserved_25_25_OFFSET                 25
#define D0F0xE4_WRAP_8014_Reserved_25_25_WIDTH                  1
#define D0F0xE4_WRAP_8014_Reserved_25_25_MASK                   0x2000000
#define D0F0xE4_WRAP_8014_Reserved_26_26_OFFSET                 26
#define D0F0xE4_WRAP_8014_Reserved_26_26_WIDTH                  1
#define D0F0xE4_WRAP_8014_Reserved_26_26_MASK                   0x4000000
#define D0F0xE4_WRAP_8014_Reserved_27_27_OFFSET                 27
#define D0F0xE4_WRAP_8014_Reserved_27_27_WIDTH                  1
#define D0F0xE4_WRAP_8014_Reserved_27_27_MASK                   0x8000000
#define D0F0xE4_WRAP_8014_SpareRegRw_OFFSET                     28
#define D0F0xE4_WRAP_8014_SpareRegRw_WIDTH                      4
#define D0F0xE4_WRAP_8014_SpareRegRw_MASK                       0xF0000000

/// D0F0xE4_WRAP_8014
typedef union {
  struct {                                                            ///<
    UINT32                                     TxclkPermGateEnable:1; ///<
    UINT32                                     TxclkPrbsGateEnable:1; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                            Reserved_4_4:1; ///<
    UINT32                                            Reserved_5_5:1; ///<
    UINT32                                           Reserved_11_6:6; ///<
    UINT32                                    PcieGatePifA1xEnable:1; ///<
    UINT32                                    PcieGatePifB1xEnable:1; ///<
    UINT32                                          Reserved_14_14:1; ///<
    UINT32                                          Reserved_15_15:1; ///<
    UINT32                                          Reserved_19_16:4; ///<
    UINT32                           TxclkPermGateOnlyWhenPllPwrDn:1; ///<
    UINT32                                          Reserved_23_21:3; ///<
    UINT32                                          Reserved_24_24:1; ///<
    UINT32                                          Reserved_25_25:1; ///<
    UINT32                                          Reserved_26_26:1; ///<
    UINT32                                          Reserved_27_27:1; ///<
    UINT32                                              SpareRegRw:4; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_8014_STRUCT;

// **** D0F0xE4_WRAP_8015 Register Definition ****
// Address
#define D0F0xE4_WRAP_8015_ADDRESS                               0x8015
// Type
#define D0F0xE4_WRAP_8015_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_8015_Bitfield_0_0_OFFSET                   0
#define D0F0xE4_WRAP_8015_Bitfield_0_0_WIDTH                    1
#define D0F0xE4_WRAP_8015_Bitfield_0_0_MASK                     0x1
#define D0F0xE4_WRAP_8015_Reserved_1_1_OFFSET                   1
#define D0F0xE4_WRAP_8015_Reserved_1_1_WIDTH                    1
#define D0F0xE4_WRAP_8015_Reserved_1_1_MASK                     0x2
#define D0F0xE4_WRAP_8015_Reserved_7_2_OFFSET                   2
#define D0F0xE4_WRAP_8015_Reserved_7_2_WIDTH                    6
#define D0F0xE4_WRAP_8015_Reserved_7_2_MASK                     0xFC
#define D0F0xE4_WRAP_8015_Reserved_8_8_OFFSET                   8
#define D0F0xE4_WRAP_8015_Reserved_8_8_WIDTH                    1
#define D0F0xE4_WRAP_8015_Reserved_8_8_MASK                     0x100
#define D0F0xE4_WRAP_8015_Bitfield_9_9_OFFSET                   9
#define D0F0xE4_WRAP_8015_Bitfield_9_9_WIDTH                    1
#define D0F0xE4_WRAP_8015_Bitfield_9_9_MASK                     0x200
#define D0F0xE4_WRAP_8015_Bitfield_10_10_OFFSET                 10
#define D0F0xE4_WRAP_8015_Bitfield_10_10_WIDTH                  1
#define D0F0xE4_WRAP_8015_Bitfield_10_10_MASK                   0x400
#define D0F0xE4_WRAP_8015_Bitfield_11_11_OFFSET                 11
#define D0F0xE4_WRAP_8015_Bitfield_11_11_WIDTH                  1
#define D0F0xE4_WRAP_8015_Bitfield_11_11_MASK                   0x800
#define D0F0xE4_WRAP_8015_Reserved_13_12_OFFSET                 12
#define D0F0xE4_WRAP_8015_Reserved_13_12_WIDTH                  2
#define D0F0xE4_WRAP_8015_Reserved_13_12_MASK                   0x3000
#define D0F0xE4_WRAP_8015_Bitfield_15_14_OFFSET                 14
#define D0F0xE4_WRAP_8015_Bitfield_15_14_WIDTH                  2
#define D0F0xE4_WRAP_8015_Bitfield_15_14_MASK                   0xC000
#define D0F0xE4_WRAP_8015_RefclkRegsGateLatency_OFFSET          16
#define D0F0xE4_WRAP_8015_RefclkRegsGateLatency_WIDTH           6
#define D0F0xE4_WRAP_8015_RefclkRegsGateLatency_MASK            0x3F0000
#define D0F0xE4_WRAP_8015_Reserved_22_22_OFFSET                 22
#define D0F0xE4_WRAP_8015_Reserved_22_22_WIDTH                  1
#define D0F0xE4_WRAP_8015_Reserved_22_22_MASK                   0x400000
#define D0F0xE4_WRAP_8015_RefclkRegsGateEnable_OFFSET           23
#define D0F0xE4_WRAP_8015_RefclkRegsGateEnable_WIDTH            1
#define D0F0xE4_WRAP_8015_RefclkRegsGateEnable_MASK             0x800000
#define D0F0xE4_WRAP_8015_Reserved_31_24_OFFSET                 24
#define D0F0xE4_WRAP_8015_Reserved_31_24_WIDTH                  8
#define D0F0xE4_WRAP_8015_Reserved_31_24_MASK                   0xFF000000

/// D0F0xE4_WRAP_8015
typedef union {
  struct {                                                            ///<
    UINT32                                            Bitfield_0_0:1; ///<
    UINT32                                            Reserved_1_1:1; ///<
    UINT32                                            Reserved_7_2:6; ///<
    UINT32                                            Reserved_8_8:1; ///<
    UINT32                                            Bitfield_9_9:1; ///<
    UINT32                                          Bitfield_10_10:1; ///<
    UINT32                                          Bitfield_11_11:1; ///<
    UINT32                                          Reserved_13_12:2; ///<
    UINT32                                          Bitfield_15_14:2; ///<
    UINT32                                   RefclkRegsGateLatency:6; ///<
    UINT32                                          Reserved_22_22:1; ///<
    UINT32                                    RefclkRegsGateEnable:1; ///<
    UINT32                                          Reserved_31_24:8; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_8015_STRUCT;

// **** D0F0xE4_WRAP_8016 Register Definition ****
// Address
#define D0F0xE4_WRAP_8016_ADDRESS                               0x8016
// Type
#define D0F0xE4_WRAP_8016_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_8016_CalibAckLatency_OFFSET                0
#define D0F0xE4_WRAP_8016_CalibAckLatency_WIDTH                 6
#define D0F0xE4_WRAP_8016_CalibAckLatency_MASK                  0x3F
#define D0F0xE4_WRAP_8016_Reserved_15_6_OFFSET                  6
#define D0F0xE4_WRAP_8016_Reserved_15_6_WIDTH                   10
#define D0F0xE4_WRAP_8016_Reserved_15_6_MASK                    0xFFC0
#define D0F0xE4_WRAP_8016_LclkDynGateLatency_OFFSET             16
#define D0F0xE4_WRAP_8016_LclkDynGateLatency_WIDTH              6
#define D0F0xE4_WRAP_8016_LclkDynGateLatency_MASK               0x3F0000
#define D0F0xE4_WRAP_8016_LclkGateFree_OFFSET                   22
#define D0F0xE4_WRAP_8016_LclkGateFree_WIDTH                    1
#define D0F0xE4_WRAP_8016_LclkGateFree_MASK                     0x400000
#define D0F0xE4_WRAP_8016_LclkDynGateEnable_OFFSET              23
#define D0F0xE4_WRAP_8016_LclkDynGateEnable_WIDTH               1
#define D0F0xE4_WRAP_8016_LclkDynGateEnable_MASK                0x800000
#define D0F0xE4_WRAP_8016_Reserved_31_24_OFFSET                 24
#define D0F0xE4_WRAP_8016_Reserved_31_24_WIDTH                  8
#define D0F0xE4_WRAP_8016_Reserved_31_24_MASK                   0xFF000000

/// D0F0xE4_WRAP_8016
typedef union {
  struct {                                                            ///<
    UINT32                                         CalibAckLatency:6; ///<
    UINT32                                           Reserved_15_6:10; ///<
    UINT32                                      LclkDynGateLatency:6; ///<
    UINT32                                            LclkGateFree:1; ///<
    UINT32                                       LclkDynGateEnable:1; ///<
    UINT32                                          Reserved_31_24:8; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_8016_STRUCT;

// **** D0F0xE4_WRAP_8029 Register Definition ****
// Address
#define D0F0xE4_WRAP_8029_ADDRESS                               0x8029
// Type
#define D0F0xE4_WRAP_8029_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_8029_LaneEnable_OFFSET                     0
#define D0F0xE4_WRAP_8029_LaneEnable_WIDTH                      16
#define D0F0xE4_WRAP_8029_LaneEnable_MASK                       0xFFFF
#define D0F0xE4_WRAP_8029_Reserved_31_16_OFFSET                 16
#define D0F0xE4_WRAP_8029_Reserved_31_16_WIDTH                  16
#define D0F0xE4_WRAP_8029_Reserved_31_16_MASK                   0xFFFF0000

/// D0F0xE4_WRAP_8029
typedef union {
  struct {                                                            ///<
    UINT32                                              LaneEnable:16; ///<
    UINT32                                          Reserved_31_16:16; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_8029_STRUCT;

// **** D0F0xE4_WRAP_8060 Register Definition ****
// Address
#define D0F0xE4_WRAP_8060_ADDRESS                               0x8060

// Type
#define D0F0xE4_WRAP_8060_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_8060_Reconfigure_OFFSET                    0
#define D0F0xE4_WRAP_8060_Reconfigure_WIDTH                     1
#define D0F0xE4_WRAP_8060_Reconfigure_MASK                      0x1
#define D0F0xE4_WRAP_8060_Reserved_1_1_OFFSET                   1
#define D0F0xE4_WRAP_8060_Reserved_1_1_WIDTH                    1
#define D0F0xE4_WRAP_8060_Reserved_1_1_MASK                     0x2
#define D0F0xE4_WRAP_8060_ResetComplete_OFFSET                  2
#define D0F0xE4_WRAP_8060_ResetComplete_WIDTH                   1
#define D0F0xE4_WRAP_8060_ResetComplete_MASK                    0x4
#define D0F0xE4_WRAP_8060_Reserved_15_3_OFFSET                  3
#define D0F0xE4_WRAP_8060_Reserved_15_3_WIDTH                   13
#define D0F0xE4_WRAP_8060_Reserved_15_3_MASK                    0xfff8
#define D0F0xE4_WRAP_8060_BifGlobalReset_OFFSET                 16
#define D0F0xE4_WRAP_8060_BifGlobalReset_WIDTH                  1
#define D0F0xE4_WRAP_8060_BifGlobalReset_MASK                   0x10000
#define D0F0xE4_WRAP_8060_BifCalibrationReset_OFFSET            17
#define D0F0xE4_WRAP_8060_BifCalibrationReset_WIDTH             1
#define D0F0xE4_WRAP_8060_BifCalibrationReset_MASK              0x20000
#define D0F0xE4_WRAP_8060_Reserved_31_18_OFFSET                 18
#define D0F0xE4_WRAP_8060_Reserved_31_18_WIDTH                  14
#define D0F0xE4_WRAP_8060_Reserved_31_18_MASK                   0xfffc0000

/// D0F0xE4_WRAP_8060
typedef union {
  struct {                                                              ///<
    UINT32                                              Reconfigure:1 ; ///<
    UINT32                                             Reserved_1_1:1 ; ///<
    UINT32                                            ResetComplete:1 ; ///<
    UINT32                                            Reserved_15_3:13; ///<
    UINT32                                           BifGlobalReset:1 ; ///<
    UINT32                                      BifCalibrationReset:1 ; ///<
    UINT32                                           Reserved_31_18:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_8060_STRUCT;

// **** D0F0xE4_WRAP_8062 Register Definition ****
// Address
#define D0F0xE4_WRAP_8062_ADDRESS                               0x8062

// Type
#define D0F0xE4_WRAP_8062_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_8062_ReconfigureEn_OFFSET                  0
#define D0F0xE4_WRAP_8062_ReconfigureEn_WIDTH                   1
#define D0F0xE4_WRAP_8062_ReconfigureEn_MASK                    0x1
#define D0F0xE4_WRAP_8062_Reserved_1_1_OFFSET                   1
#define D0F0xE4_WRAP_8062_Reserved_1_1_WIDTH                    1
#define D0F0xE4_WRAP_8062_Reserved_1_1_MASK                     0x2
#define D0F0xE4_WRAP_8062_ResetPeriod_OFFSET                    2
#define D0F0xE4_WRAP_8062_ResetPeriod_WIDTH                     3
#define D0F0xE4_WRAP_8062_ResetPeriod_MASK                      0x1c
#define D0F0xE4_WRAP_8062_Reserved_9_5_OFFSET                   5
#define D0F0xE4_WRAP_8062_Reserved_9_5_WIDTH                    5
#define D0F0xE4_WRAP_8062_Reserved_9_5_MASK                     0x3e0
#define D0F0xE4_WRAP_8062_BlockOnIdle_OFFSET                    10
#define D0F0xE4_WRAP_8062_BlockOnIdle_WIDTH                     1
#define D0F0xE4_WRAP_8062_BlockOnIdle_MASK                      0x400
#define D0F0xE4_WRAP_8062_ConfigXferMode_OFFSET                 11
#define D0F0xE4_WRAP_8062_ConfigXferMode_WIDTH                  1
#define D0F0xE4_WRAP_8062_ConfigXferMode_MASK                   0x800
#define D0F0xE4_WRAP_8062_Reserved_31_12_OFFSET                 12
#define D0F0xE4_WRAP_8062_Reserved_31_12_WIDTH                  20
#define D0F0xE4_WRAP_8062_Reserved_31_12_MASK                   0xfffff000

/// D0F0xE4_WRAP_8062
typedef union {
  struct {                                                              ///<
    UINT32                                            ReconfigureEn:1 ; ///<
    UINT32                                             Reserved_1_1:1 ; ///<
    UINT32                                              ResetPeriod:3 ; ///<
    UINT32                                             Reserved_9_5:5 ; ///<
    UINT32                                              BlockOnIdle:1 ; ///<
    UINT32                                           ConfigXferMode:1 ; ///<
    UINT32                                           Reserved_31_12:20; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_8062_STRUCT;

// **** D0F0xE4_PIF_0011 Register Definition ****
// Address
#define D0F0xE4_PIF_0011_ADDRESS                                0x11
// Type
#define D0F0xE4_PIF_0011_TYPE                                   TYPE_D0F0xE4

// Field Data
#define D0F0xE4_PIF_0011_X2Lane10_OFFSET                        0
#define D0F0xE4_PIF_0011_X2Lane10_WIDTH                         1
#define D0F0xE4_PIF_0011_X2Lane10_MASK                          0x1
#define D0F0xE4_PIF_0011_X2Lane32_OFFSET                        1
#define D0F0xE4_PIF_0011_X2Lane32_WIDTH                         1
#define D0F0xE4_PIF_0011_X2Lane32_MASK                          0x2
#define D0F0xE4_PIF_0011_X2Lane54_OFFSET                        2
#define D0F0xE4_PIF_0011_X2Lane54_WIDTH                         1
#define D0F0xE4_PIF_0011_X2Lane54_MASK                          0x4
#define D0F0xE4_PIF_0011_X2Lane76_OFFSET                        3
#define D0F0xE4_PIF_0011_X2Lane76_WIDTH                         1
#define D0F0xE4_PIF_0011_X2Lane76_MASK                          0x8
#define D0F0xE4_PIF_0011_X2Lane98_OFFSET                        4
#define D0F0xE4_PIF_0011_X2Lane98_WIDTH                         1
#define D0F0xE4_PIF_0011_X2Lane98_MASK                          0x10
#define D0F0xE4_PIF_0011_X2Lane1110_OFFSET                      5
#define D0F0xE4_PIF_0011_X2Lane1110_WIDTH                       1
#define D0F0xE4_PIF_0011_X2Lane1110_MASK                        0x20
#define D0F0xE4_PIF_0011_X2Lane1312_OFFSET                      6
#define D0F0xE4_PIF_0011_X2Lane1312_WIDTH                       1
#define D0F0xE4_PIF_0011_X2Lane1312_MASK                        0x40
#define D0F0xE4_PIF_0011_X2Lane1514_OFFSET                      7
#define D0F0xE4_PIF_0011_X2Lane1514_WIDTH                       1
#define D0F0xE4_PIF_0011_X2Lane1514_MASK                        0x80
#define D0F0xE4_PIF_0011_X4Lane30_OFFSET                        8
#define D0F0xE4_PIF_0011_X4Lane30_WIDTH                         1
#define D0F0xE4_PIF_0011_X4Lane30_MASK                          0x100
#define D0F0xE4_PIF_0011_X4Lane74_OFFSET                        9
#define D0F0xE4_PIF_0011_X4Lane74_WIDTH                         1
#define D0F0xE4_PIF_0011_X4Lane74_MASK                          0x200
#define D0F0xE4_PIF_0011_X4Lane118_OFFSET                       10
#define D0F0xE4_PIF_0011_X4Lane118_WIDTH                        1
#define D0F0xE4_PIF_0011_X4Lane118_MASK                         0x400
#define D0F0xE4_PIF_0011_X4Lane1512_OFFSET                      11
#define D0F0xE4_PIF_0011_X4Lane1512_WIDTH                       1
#define D0F0xE4_PIF_0011_X4Lane1512_MASK                        0x800
#define D0F0xE4_PIF_0011_Reserved_15_12_OFFSET                  12
#define D0F0xE4_PIF_0011_Reserved_15_12_WIDTH                   4
#define D0F0xE4_PIF_0011_Reserved_15_12_MASK                    0xF000
#define D0F0xE4_PIF_0011_X8Lane70_OFFSET                        16
#define D0F0xE4_PIF_0011_X8Lane70_WIDTH                         1
#define D0F0xE4_PIF_0011_X8Lane70_MASK                          0x10000
#define D0F0xE4_PIF_0011_X8Lane158_OFFSET                       17
#define D0F0xE4_PIF_0011_X8Lane158_WIDTH                        1
#define D0F0xE4_PIF_0011_X8Lane158_MASK                         0x20000
#define D0F0xE4_PIF_0011_Reserved_19_18_OFFSET                  18
#define D0F0xE4_PIF_0011_Reserved_19_18_WIDTH                   2
#define D0F0xE4_PIF_0011_Reserved_19_18_MASK                    0xC0000
#define D0F0xE4_PIF_0011_X16Lane150_OFFSET                      20
#define D0F0xE4_PIF_0011_X16Lane150_WIDTH                       1
#define D0F0xE4_PIF_0011_X16Lane150_MASK                        0x100000
#define D0F0xE4_PIF_0011_Reserved_24_21_OFFSET                  21
#define D0F0xE4_PIF_0011_Reserved_24_21_WIDTH                   4
#define D0F0xE4_PIF_0011_Reserved_24_21_MASK                    0x1E00000
#define D0F0xE4_PIF_0011_MultiPif_OFFSET                        25
#define D0F0xE4_PIF_0011_MultiPif_WIDTH                         1
#define D0F0xE4_PIF_0011_MultiPif_MASK                          0x2000000
#define D0F0xE4_PIF_0011_Reserved_31_26_OFFSET                  26
#define D0F0xE4_PIF_0011_Reserved_31_26_WIDTH                   6
#define D0F0xE4_PIF_0011_Reserved_31_26_MASK                    0xFC000000

/// D0F0xE4_PIF_0011
typedef union {
  struct {                                                            ///<
    UINT32                                                X2Lane10:1; ///<
    UINT32                                                X2Lane32:1; ///<
    UINT32                                                X2Lane54:1; ///<
    UINT32                                                X2Lane76:1; ///<
    UINT32                                                X2Lane98:1; ///<
    UINT32                                              X2Lane1110:1; ///<
    UINT32                                              X2Lane1312:1; ///<
    UINT32                                              X2Lane1514:1; ///<
    UINT32                                                X4Lane30:1; ///<
    UINT32                                                X4Lane74:1; ///<
    UINT32                                               X4Lane118:1; ///<
    UINT32                                              X4Lane1512:1; ///<
    UINT32                                          Reserved_15_12:4; ///<
    UINT32                                                X8Lane70:1; ///<
    UINT32                                               X8Lane158:1; ///<
    UINT32                                          Reserved_19_18:2; ///<
    UINT32                                              X16Lane150:1; ///<
    UINT32                                          Reserved_24_21:4; ///<
    UINT32                                                MultiPif:1; ///<
    UINT32                                          Reserved_31_26:6; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_PIF_0011_STRUCT;

// **** D0F0xE4_PIF_0012 Register Definition ****
// Address
#define D0F0xE4_PIF_0012_ADDRESS                                0x12
// Type
#define D0F0xE4_PIF_0012_TYPE                                   TYPE_D0F0xE4

// Field Data
#define D0F0xE4_PIF_0012_TxPowerStateInTxs2_OFFSET              0
#define D0F0xE4_PIF_0012_TxPowerStateInTxs2_WIDTH               3
#define D0F0xE4_PIF_0012_TxPowerStateInTxs2_MASK                0x7
#define D0F0xE4_PIF_0012_ForceRxEnInL0s_OFFSET                  3
#define D0F0xE4_PIF_0012_ForceRxEnInL0s_WIDTH                   1
#define D0F0xE4_PIF_0012_ForceRxEnInL0s_MASK                    0x8
#define D0F0xE4_PIF_0012_RxPowerStateInRxs2_OFFSET              4
#define D0F0xE4_PIF_0012_RxPowerStateInRxs2_WIDTH               3
#define D0F0xE4_PIF_0012_RxPowerStateInRxs2_MASK                0x70
#define D0F0xE4_PIF_0012_PllPowerStateInTxs2_OFFSET             7
#define D0F0xE4_PIF_0012_PllPowerStateInTxs2_WIDTH              3
#define D0F0xE4_PIF_0012_PllPowerStateInTxs2_MASK               0x380
#define D0F0xE4_PIF_0012_PllPowerStateInOff_OFFSET              10
#define D0F0xE4_PIF_0012_PllPowerStateInOff_WIDTH               3
#define D0F0xE4_PIF_0012_PllPowerStateInOff_MASK                0x1C00
#define D0F0xE4_PIF_0012_Reserved_15_13_OFFSET                  13
#define D0F0xE4_PIF_0012_Reserved_15_13_WIDTH                   3
#define D0F0xE4_PIF_0012_Reserved_15_13_MASK                    0xE000
#define D0F0xE4_PIF_0012_Tx2p5clkClockGatingEn_OFFSET           16
#define D0F0xE4_PIF_0012_Tx2p5clkClockGatingEn_WIDTH            1
#define D0F0xE4_PIF_0012_Tx2p5clkClockGatingEn_MASK             0x10000
#define D0F0xE4_PIF_0012_Reserved_23_17_OFFSET                  17
#define D0F0xE4_PIF_0012_Reserved_23_17_WIDTH                   7
#define D0F0xE4_PIF_0012_Reserved_23_17_MASK                    0xFE0000
#define D0F0xE4_PIF_0012_PllRampUpTime_OFFSET                   24
#define D0F0xE4_PIF_0012_PllRampUpTime_WIDTH                    3
#define D0F0xE4_PIF_0012_PllRampUpTime_MASK                     0x7000000
#define D0F0xE4_PIF_0012_Reserved_27_27_OFFSET                  27
#define D0F0xE4_PIF_0012_Reserved_27_27_WIDTH                   1
#define D0F0xE4_PIF_0012_Reserved_27_27_MASK                    0x8000000
#define D0F0xE4_PIF_0012_PllPwrOverrideEn_OFFSET                28
#define D0F0xE4_PIF_0012_PllPwrOverrideEn_WIDTH                 1
#define D0F0xE4_PIF_0012_PllPwrOverrideEn_MASK                  0x10000000
#define D0F0xE4_PIF_0012_PllPwrOverrideVal_OFFSET               29
#define D0F0xE4_PIF_0012_PllPwrOverrideVal_WIDTH                3
#define D0F0xE4_PIF_0012_PllPwrOverrideVal_MASK                 0xE0000000

/// D0F0xE4_PIF_0012
typedef union {
  struct {                                                            ///<
    UINT32                                      TxPowerStateInTxs2:3; ///<
    UINT32                                          ForceRxEnInL0s:1; ///<
    UINT32                                      RxPowerStateInRxs2:3; ///<
    UINT32                                     PllPowerStateInTxs2:3; ///<
    UINT32                                      PllPowerStateInOff:3; ///<
    UINT32                                          Reserved_15_13:3; ///<
    UINT32                                   Tx2p5clkClockGatingEn:1; ///<
    UINT32                                          Reserved_23_17:7; ///<
    UINT32                                           PllRampUpTime:3; ///<
    UINT32                                          Reserved_27_27:1; ///<
    UINT32                                        PllPwrOverrideEn:1; ///<
    UINT32                                       PllPwrOverrideVal:3; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_PIF_0012_STRUCT;

// **** D0F0xE4_CORE_0002 Register Definition ****
// Address
#define D0F0xE4_CORE_0002_ADDRESS                               0x0002
// Type
#define D0F0xE4_CORE_0002_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_0002_HwDebug_0_OFFSET                      0
#define D0F0xE4_CORE_0002_HwDebug_0_WIDTH                       1
#define D0F0xE4_CORE_0002_HwDebug_0_MASK                        0x1
#define D0F0xE4_CORE_0002_Reserved_31_1_OFFSET                  1
#define D0F0xE4_CORE_0002_Reserved_31_1_WIDTH                   31
#define D0F0xE4_CORE_0002_Reserved_31_1_MASK                    0xFFFFFFFE

/// D0F0xE4_CORE_0002
typedef union {
  struct {                                                            ///<
    UINT32                                               HwDebug_0:1; ///<
    UINT32                                           Reserved_31_1:31; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_0002_STRUCT;

// **** D0F0xE4_CORE_0010 Register Definition ****
// Address
#define D0F0xE4_CORE_0010_ADDRESS                               0x10
// Type
#define D0F0xE4_CORE_0010_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_0010_HwInitWrLock_OFFSET                   0
#define D0F0xE4_CORE_0010_HwInitWrLock_WIDTH                    1
#define D0F0xE4_CORE_0010_HwInitWrLock_MASK                     0x1
#define D0F0xE4_CORE_0010_Reserved_8_1_OFFSET                   1
#define D0F0xE4_CORE_0010_Reserved_8_1_WIDTH                    8
#define D0F0xE4_CORE_0010_Reserved_8_1_MASK                     0x1FE
#define D0F0xE4_CORE_0010_UmiNpMemWrite_OFFSET                  9
#define D0F0xE4_CORE_0010_UmiNpMemWrite_WIDTH                   1
#define D0F0xE4_CORE_0010_UmiNpMemWrite_MASK                    0x200
#define D0F0xE4_CORE_0010_RxUmiAdjPayloadSize_OFFSET            10
#define D0F0xE4_CORE_0010_RxUmiAdjPayloadSize_WIDTH             3
#define D0F0xE4_CORE_0010_RxUmiAdjPayloadSize_MASK              0x1C00
#define D0F0xE4_CORE_0010_Reserved_31_13_OFFSET                 13
#define D0F0xE4_CORE_0010_Reserved_31_13_WIDTH                  19
#define D0F0xE4_CORE_0010_Reserved_31_13_MASK                   0xFFFFE000

/// D0F0xE4_CORE_0010
typedef union {
  struct {                                                            ///<
    UINT32                                            HwInitWrLock:1; ///<
    UINT32                                            Reserved_8_1:8; ///<
    UINT32                                           UmiNpMemWrite:1; ///<
    UINT32                                     RxUmiAdjPayloadSize:3; ///<
    UINT32                                          Reserved_31_13:19; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_0010_STRUCT;

// **** D0F0xE4_CORE_0011 Register Definition ****
// Address
#define D0F0xE4_CORE_0011_ADDRESS                               0x11
// Type
#define D0F0xE4_CORE_0011_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_0011_DynClkLatency_OFFSET                  0
#define D0F0xE4_CORE_0011_DynClkLatency_WIDTH                   4
#define D0F0xE4_CORE_0011_DynClkLatency_MASK                    0xF
#define D0F0xE4_CORE_0011_Reserved_31_4_OFFSET                  4
#define D0F0xE4_CORE_0011_Reserved_31_4_WIDTH                   28
#define D0F0xE4_CORE_0011_Reserved_31_4_MASK                    0xFFFFFFF0

/// D0F0xE4_CORE_0011
typedef union {
  struct {                                                            ///<
    UINT32                                           DynClkLatency:4; ///<
    UINT32                                           Reserved_31_4:28; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_0011_STRUCT;

// **** D0F0xE4_CORE_001C Register Definition ****
// Address
#define D0F0xE4_CORE_001C_ADDRESS                               0x1C
// Type
#define D0F0xE4_CORE_001C_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_001C_TxArbRoundRobinEn_OFFSET              0
#define D0F0xE4_CORE_001C_TxArbRoundRobinEn_WIDTH               1
#define D0F0xE4_CORE_001C_TxArbRoundRobinEn_MASK                0x1
#define D0F0xE4_CORE_001C_TxArbSlvLimit_OFFSET                  1
#define D0F0xE4_CORE_001C_TxArbSlvLimit_WIDTH                   5
#define D0F0xE4_CORE_001C_TxArbSlvLimit_MASK                    0x3E
#define D0F0xE4_CORE_001C_TxArbMstLimit_OFFSET                  6
#define D0F0xE4_CORE_001C_TxArbMstLimit_WIDTH                   5
#define D0F0xE4_CORE_001C_TxArbMstLimit_MASK                    0x7C0
#define D0F0xE4_CORE_001C_Reserved_31_11_OFFSET                 11
#define D0F0xE4_CORE_001C_Reserved_31_11_WIDTH                  21
#define D0F0xE4_CORE_001C_Reserved_31_11_MASK                   0xFFFFF800

/// D0F0xE4_CORE_001C
typedef union {
  struct {                                                            ///<
    UINT32                                       TxArbRoundRobinEn:1; ///<
    UINT32                                           TxArbSlvLimit:5; ///<
    UINT32                                           TxArbMstLimit:5; ///<
    UINT32                                          Reserved_31_11:21; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_001C_STRUCT;

// **** D0F0xE4_CORE_0020 Register Definition ****
// Address
#define D0F0xE4_CORE_0020_ADDRESS                               0x0020
// Type
#define D0F0xE4_CORE_0020_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_0020_Reserved_7_0_OFFSET                   0
#define D0F0xE4_CORE_0020_Reserved_7_0_WIDTH                    8
#define D0F0xE4_CORE_0020_Reserved_7_0_MASK                     0xFF
#define D0F0xE4_CORE_0020_CiSlvOrderingDis_OFFSET               8
#define D0F0xE4_CORE_0020_CiSlvOrderingDis_WIDTH                1
#define D0F0xE4_CORE_0020_CiSlvOrderingDis_MASK                 0x100
#define D0F0xE4_CORE_0020_CiRcOrderingDis_OFFSET                9
#define D0F0xE4_CORE_0020_CiRcOrderingDis_WIDTH                 1
#define D0F0xE4_CORE_0020_CiRcOrderingDis_MASK                  0x200
#define D0F0xE4_CORE_0020_Reserved_31_10_OFFSET                 10
#define D0F0xE4_CORE_0020_Reserved_31_10_WIDTH                  22
#define D0F0xE4_CORE_0020_Reserved_31_10_MASK                   0xFFFFFC00

/// D0F0xE4_CORE_0020
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_7_0:8; ///<
    UINT32                                        CiSlvOrderingDis:1; ///<
    UINT32                                         CiRcOrderingDis:1; ///<
    UINT32                                          Reserved_31_10:22; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_0020_STRUCT;

// **** D0F0xE4_CORE_0040 Register Definition ****
// Address
#define D0F0xE4_CORE_0040_ADDRESS                               0x40
// Type
#define D0F0xE4_CORE_0040_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_0040_Reserved_13_0_OFFSET                  0
#define D0F0xE4_CORE_0040_Reserved_13_0_WIDTH                   14
#define D0F0xE4_CORE_0040_Reserved_13_0_MASK                    0x3FFF
#define D0F0xE4_CORE_0040_PElecIdleMode_OFFSET                  14
#define D0F0xE4_CORE_0040_PElecIdleMode_WIDTH                   2
#define D0F0xE4_CORE_0040_PElecIdleMode_MASK                    0xC000
#define D0F0xE4_CORE_0040_Reserved_31_16_OFFSET                 16
#define D0F0xE4_CORE_0040_Reserved_31_16_WIDTH                  16
#define D0F0xE4_CORE_0040_Reserved_31_16_MASK                   0xFFFF0000

/// D0F0xE4_CORE_0040
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_13_0:14; ///<
    UINT32                                           PElecIdleMode:2; ///<
    UINT32                                          Reserved_31_16:16; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_0040_STRUCT;

// **** D0F0xE4_CORE_00B0 Register Definition ****
// Address
#define D0F0xE4_CORE_00B0_ADDRESS                               0xB0
// Type
#define D0F0xE4_CORE_00B0_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_00B0_Reserved_1_0_OFFSET                   0
#define D0F0xE4_CORE_00B0_Reserved_1_0_WIDTH                    2
#define D0F0xE4_CORE_00B0_Reserved_1_0_MASK                     0x3
#define D0F0xE4_CORE_00B0_StrapF0MsiEn_OFFSET                   2
#define D0F0xE4_CORE_00B0_StrapF0MsiEn_WIDTH                    1
#define D0F0xE4_CORE_00B0_StrapF0MsiEn_MASK                     0x4
#define D0F0xE4_CORE_00B0_Reserved_4_3_OFFSET                   3
#define D0F0xE4_CORE_00B0_Reserved_4_3_WIDTH                    2
#define D0F0xE4_CORE_00B0_Reserved_4_3_MASK                     0x18
#define D0F0xE4_CORE_00B0_StrapF0AerEn_OFFSET                   5
#define D0F0xE4_CORE_00B0_StrapF0AerEn_WIDTH                    1
#define D0F0xE4_CORE_00B0_StrapF0AerEn_MASK                     0x20
#define D0F0xE4_CORE_00B0_Reserved_31_6_OFFSET                  6
#define D0F0xE4_CORE_00B0_Reserved_31_6_WIDTH                   26
#define D0F0xE4_CORE_00B0_Reserved_31_6_MASK                    0xFFFFFFC0

/// D0F0xE4_CORE_00B0
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_1_0:2; ///<
    UINT32                                            StrapF0MsiEn:1; ///<
    UINT32                                            Reserved_4_3:2; ///<
    UINT32                                            StrapF0AerEn:1; ///<
    UINT32                                           Reserved_31_6:26; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_00B0_STRUCT;

// **** D0F0xE4_CORE_00C1 Register Definition ****
// Address
#define D0F0xE4_CORE_00C1_ADDRESS                               0xC1
// Type
#define D0F0xE4_CORE_00C1_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_00C1_StrapLinkBwNotificationCapEn_OFFSET   0
#define D0F0xE4_CORE_00C1_StrapLinkBwNotificationCapEn_WIDTH    1
#define D0F0xE4_CORE_00C1_StrapLinkBwNotificationCapEn_MASK     0x1
#define D0F0xE4_CORE_00C1_StrapGen2Compliance_OFFSET            1
#define D0F0xE4_CORE_00C1_StrapGen2Compliance_WIDTH             1
#define D0F0xE4_CORE_00C1_StrapGen2Compliance_MASK              0x2
#define D0F0xE4_CORE_00C1_Reserved_31_2_OFFSET                  2
#define D0F0xE4_CORE_00C1_Reserved_31_2_WIDTH                   30
#define D0F0xE4_CORE_00C1_Reserved_31_2_MASK                    0xFFFFFFFC

/// D0F0xE4_CORE_00C1
typedef union {
  struct {                                                            ///<
    UINT32                            StrapLinkBwNotificationCapEn:1; ///<
    UINT32                                     StrapGen2Compliance:1; ///<
    UINT32                                           Reserved_31_2:30; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_00C1_STRUCT;

// **** DxFxxE4_x70 Register Definition ****
// Address
#define DxFxxE4_x70_ADDRESS                                     0x70
// Type
#define DxFxxE4_x70_TYPE                                        TYPE_D2F1xE4

// Field Data
#define DxFxxE4_x70_Reserved_15_0_OFFSET                        0
#define DxFxxE4_x70_Reserved_15_0_WIDTH                         16
#define DxFxxE4_x70_Reserved_15_0_MASK                          0xFFFF
#define DxFxxE4_x70_RxRcbCplTimeout_OFFSET                      16
#define DxFxxE4_x70_RxRcbCplTimeout_WIDTH                       3
#define DxFxxE4_x70_RxRcbCplTimeout_MASK                        0x70000
#define DxFxxE4_x70_RxRcbCplTimeoutMode_OFFSET                  19
#define DxFxxE4_x70_RxRcbCplTimeoutMode_WIDTH                   1
#define DxFxxE4_x70_RxRcbCplTimeoutMode_MASK                    0x80000
#define DxFxxE4_x70_Reserved_31_20_OFFSET                       20
#define DxFxxE4_x70_Reserved_31_20_WIDTH                        12
#define DxFxxE4_x70_Reserved_31_20_MASK                         0xFFF00000

/// DxFxxE4_x70
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_15_0:16; ///<
    UINT32                                         RxRcbCplTimeout:3; ///<
    UINT32                                     RxRcbCplTimeoutMode:1; ///<
    UINT32                                          Reserved_31_20:12; ///<

  } Field;

  UINT32 Value;
} DxFxxE4_x70_STRUCT;

// **** DxFxxE4_xA0 Register Definition ****
// Address
#define DxFxxE4_xA0_ADDRESS                                     0xA0
// Type
#define DxFxxE4_xA0_TYPE                                        TYPE_D2F1xE4

// Field Data
#define DxFxxE4_xA0_Reserved_3_0_OFFSET                         0
#define DxFxxE4_xA0_Reserved_3_0_WIDTH                          4
#define DxFxxE4_xA0_Reserved_3_0_MASK                           0xF
#define DxFxxE4_xA0_Lc16xClearTxPipe_OFFSET                     4
#define DxFxxE4_xA0_Lc16xClearTxPipe_WIDTH                      4
#define DxFxxE4_xA0_Lc16xClearTxPipe_MASK                       0xF0
#define DxFxxE4_xA0_LcL0sInactivity_OFFSET                      8
#define DxFxxE4_xA0_LcL0sInactivity_WIDTH                       4
#define DxFxxE4_xA0_LcL0sInactivity_MASK                        0xF00
#define DxFxxE4_xA0_LcL1Inactivity_OFFSET                       12
#define DxFxxE4_xA0_LcL1Inactivity_WIDTH                        4
#define DxFxxE4_xA0_LcL1Inactivity_MASK                         0xF000
#define DxFxxE4_xA0_Reserved_22_16_OFFSET                       16
#define DxFxxE4_xA0_Reserved_22_16_WIDTH                        7
#define DxFxxE4_xA0_Reserved_22_16_MASK                         0x7F0000
#define DxFxxE4_xA0_LcL1ImmediateAck_OFFSET                     23
#define DxFxxE4_xA0_LcL1ImmediateAck_WIDTH                      1
#define DxFxxE4_xA0_LcL1ImmediateAck_MASK                       0x800000
#define DxFxxE4_xA0_Reserved_31_24_OFFSET                       24
#define DxFxxE4_xA0_Reserved_31_24_WIDTH                        8
#define DxFxxE4_xA0_Reserved_31_24_MASK                         0xFF000000

/// DxFxxE4_xA0
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_3_0:4; ///<
    UINT32                                        Lc16xClearTxPipe:4; ///<
    UINT32                                         LcL0sInactivity:4; ///<
    UINT32                                          LcL1Inactivity:4; ///<
    UINT32                                          Reserved_22_16:7; ///<
    UINT32                                        LcL1ImmediateAck:1; ///<
    UINT32                                          Reserved_31_24:8; ///<

  } Field;

  UINT32 Value;
} DxFxxE4_xA0_STRUCT;

// **** DxFxxE4_xA1 Register Definition ****
// Address
#define DxFxxE4_xA1_ADDRESS                                     0xA1
// Type
#define DxFxxE4_xA1_TYPE                                        TYPE_D2F1xE4

// Field Data
#define DxFxxE4_xA1_Reserved_10_0_OFFSET                        0
#define DxFxxE4_xA1_Reserved_10_0_WIDTH                         11
#define DxFxxE4_xA1_Reserved_10_0_MASK                          0x7FF
#define DxFxxE4_xA1_LcDontGotoL0sifL1Armed_OFFSET               11
#define DxFxxE4_xA1_LcDontGotoL0sifL1Armed_WIDTH                1
#define DxFxxE4_xA1_LcDontGotoL0sifL1Armed_MASK                 0x800
#define DxFxxE4_xA1_Reserved_31_12_OFFSET                       12
#define DxFxxE4_xA1_Reserved_31_12_WIDTH                        20
#define DxFxxE4_xA1_Reserved_31_12_MASK                         0xFFFFF000

/// DxFxxE4_xA1
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_10_0:11; ///<
    UINT32                                  LcDontGotoL0sifL1Armed:1; ///<
    UINT32                                          Reserved_31_12:20; ///<

  } Field;

  UINT32 Value;
} DxFxxE4_xA1_STRUCT;

// **** DxFxxE4_xA3 Register Definition ****
// Address
#define DxFxxE4_xA3_ADDRESS                                     0xA3
// Type
#define DxFxxE4_xA3_TYPE                                        TYPE_D2F1xE4

// Field Data
#define DxFxxE4_xA3_Reserved_8_0_OFFSET                         0
#define DxFxxE4_xA3_Reserved_8_0_WIDTH                          9
#define DxFxxE4_xA3_Reserved_8_0_MASK                           0x1FF
#define DxFxxE4_xA3_LcXmitFtsBeforeRecovery_OFFSET              9
#define DxFxxE4_xA3_LcXmitFtsBeforeRecovery_WIDTH               1
#define DxFxxE4_xA3_LcXmitFtsBeforeRecovery_MASK                0x200
#define DxFxxE4_xA3_Reserved_31_10_OFFSET                       10
#define DxFxxE4_xA3_Reserved_31_10_WIDTH                        22
#define DxFxxE4_xA3_Reserved_31_10_MASK                         0xFFFFFC00

/// DxFxxE4_xA3
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_8_0:9; ///<
    UINT32                                 LcXmitFtsBeforeRecovery:1; ///<
    UINT32                                          Reserved_31_10:22; ///<

  } Field;

  UINT32 Value;
} DxFxxE4_xA3_STRUCT;

// **** DxFxxE4_xB1 Register Definition ****
// Address
#define DxFxxE4_xB1_ADDRESS                                     0xB1
// Type
#define DxFxxE4_xB1_TYPE                                        TYPE_D2F1xE4

// Field Data
#define DxFxxE4_xB1_Reserved_13_0_OFFSET                        0
#define DxFxxE4_xB1_Reserved_13_0_WIDTH                         14
#define DxFxxE4_xB1_Reserved_13_0_MASK                          0x3FFF
#define DxFxxE4_xB1_LcElecIdleMode_OFFSET                       14
#define DxFxxE4_xB1_LcElecIdleMode_WIDTH                        2
#define DxFxxE4_xB1_LcElecIdleMode_MASK                         0xc000
#define DxFxxE4_xB1_Reserved_18_16_OFFSET                       16
#define DxFxxE4_xB1_Reserved_18_16_WIDTH                        3
#define DxFxxE4_xB1_Reserved_18_16_MASK                         0x70000
#define DxFxxE4_xB1_LcDeassertRxEnInL0s_OFFSET                  19
#define DxFxxE4_xB1_LcDeassertRxEnInL0s_WIDTH                   1
#define DxFxxE4_xB1_LcDeassertRxEnInL0s_MASK                    0x80000
#define DxFxxE4_xB1_LcBlockElIdleinL0_OFFSET                    20
#define DxFxxE4_xB1_LcBlockElIdleinL0_WIDTH                     1
#define DxFxxE4_xB1_LcBlockElIdleinL0_MASK                      0x100000
#define DxFxxE4_xB1_Reserved_31_21_OFFSET                       21
#define DxFxxE4_xB1_Reserved_31_21_WIDTH                        11
#define DxFxxE4_xB1_Reserved_31_21_MASK                         0xFFE00000

/// DxFxxE4_xB1
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_13_0:14; ///<
    UINT32                                          LcElecIdleMode:2 ; ///<
    UINT32                                          Reserved_18_16:3 ; ///<
    UINT32                                     LcDeassertRxEnInL0s:1; ///<
    UINT32                                       LcBlockElIdleinL0:1; ///<
    UINT32                                          Reserved_31_21:11; ///<

  } Field;
  UINT32 Value;
} DxFxxE4_xB1_STRUCT;

// **** DxFxxE4_xC0 Register Definition ****
// Address
#define DxFxxE4_xC0_ADDRESS                                     0xC0
// Type
#define DxFxxE4_xC0_TYPE                                        TYPE_D2F1xE4

// Field Data
#define DxFxxE4_xC0_Reserved_12_0_OFFSET                        0
#define DxFxxE4_xC0_Reserved_12_0_WIDTH                         13
#define DxFxxE4_xC0_Reserved_12_0_MASK                          0x1FFF
#define DxFxxE4_xC0_StrapForceCompliance_OFFSET                 13
#define DxFxxE4_xC0_StrapForceCompliance_WIDTH                  1
#define DxFxxE4_xC0_StrapForceCompliance_MASK                   0x2000
#define DxFxxE4_xC0_Reserved_14_14_OFFSET                       14
#define DxFxxE4_xC0_Reserved_14_14_WIDTH                        1
#define DxFxxE4_xC0_Reserved_14_14_MASK                         0x4000
#define DxFxxE4_xC0_StrapAutoRcSpeedNegotiationDis_OFFSET       15
#define DxFxxE4_xC0_StrapAutoRcSpeedNegotiationDis_WIDTH        1
#define DxFxxE4_xC0_StrapAutoRcSpeedNegotiationDis_MASK         0x8000
#define DxFxxE4_xC0_StrapLaneNegotiation_OFFSET                 16
#define DxFxxE4_xC0_StrapLaneNegotiation_WIDTH                  3
#define DxFxxE4_xC0_StrapLaneNegotiation_MASK                   0x70000
#define DxFxxE4_xC0_Reserved_31_19_OFFSET                       19
#define DxFxxE4_xC0_Reserved_31_19_WIDTH                        13
#define DxFxxE4_xC0_Reserved_31_19_MASK                         0xfff80000

/// DxFxxE4_xC0
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_12_0:13; ///<
    UINT32                                    StrapForceCompliance:1; ///<
    UINT32                                          Reserved_14_14:1; ///<
    UINT32                          StrapAutoRcSpeedNegotiationDis:1; ///<
    UINT32                                     StrapLaneNegotiation:3 ; ///<
    UINT32                                           Reserved_31_19:13; ///<
  } Field;
  UINT32 Value;
} DxFxxE4_xC0_STRUCT;

// **** GMMx190 Register Definition ****
// Address
#define GMMx190_ADDRESS                                         0x190

// Type
#define GMMx190_TYPE                                            TYPE_GMM
// Field Data
#define GMMx190_ADDR_OFFSET                                     0
#define GMMx190_ADDR_WIDTH                                      8
#define GMMx190_ADDR_MASK                                       0xff
#define GMMx190_DSM_SEL_OFFSET                                  8
#define GMMx190_DSM_SEL_WIDTH                                   1
#define GMMx190_DSM_SEL_MASK                                    0x100
#define GMMx190_DRB_SEL_OFFSET                                  9
#define GMMx190_DRB_SEL_WIDTH                                   1
#define GMMx190_DRB_SEL_MASK                                    0x200
#define GMMx190_SPARE_BITS31_6_OFFSET                           10
#define GMMx190_SPARE_BITS31_6_WIDTH                            22
#define GMMx190_SPARE_BITS31_6_MASK                             0xfffffc00

/// GMMx190
typedef union {
  struct {                                                              ///<
    UINT32                                                     ADDR:8 ; ///<
    UINT32                                                  DSM_SEL:1 ; ///<
    UINT32                                                  DRB_SEL:1 ; ///<
    UINT32                                           SPARE_BITS31_6:22; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx190_STRUCT;

// **** GMMx194 Register Definition ****
// Address
#define GMMx194_ADDRESS                                         0x194

// Type
#define GMMx194_TYPE                                            TYPE_GMM
// Field Data
#define GMMx194_DBG_EN_OFFSET                                   0
#define GMMx194_DBG_EN_WIDTH                                    1
#define GMMx194_DBG_EN_MASK                                     0x1
#define GMMx194_DSM_SPR_WREN_D4_OFFSET                          1
#define GMMx194_DSM_SPR_WREN_D4_WIDTH                           1
#define GMMx194_DSM_SPR_WREN_D4_MASK                            0x2
#define GMMx194_DSM_SPR_WREN_D5_OFFSET                          2
#define GMMx194_DSM_SPR_WREN_D5_WIDTH                           1
#define GMMx194_DSM_SPR_WREN_D5_MASK                            0x4
#define GMMx194_DSM_SPR_RDEN_D5_OFFSET                          3
#define GMMx194_DSM_SPR_RDEN_D5_WIDTH                           1
#define GMMx194_DSM_SPR_RDEN_D5_MASK                            0x8
#define GMMx194_DRB_SPR_WREN_OFFSET                             4
#define GMMx194_DRB_SPR_WREN_WIDTH                              1
#define GMMx194_DRB_SPR_WREN_MASK                               0x10
#define GMMx194_DRB_SPR_RDEN_OFFSET                             5
#define GMMx194_DRB_SPR_RDEN_WIDTH                              1
#define GMMx194_DRB_SPR_RDEN_MASK                               0x20
#define GMMx194_GTSC_REFCLK_SEL_OFFSET                          6
#define GMMx194_GTSC_REFCLK_SEL_WIDTH                           1
#define GMMx194_GTSC_REFCLK_SEL_MASK                            0x40
#define GMMx194_GTSC_SCLK_SEL_OFFSET                            7
#define GMMx194_GTSC_SCLK_SEL_WIDTH                             1
#define GMMx194_GTSC_SCLK_SEL_MASK                              0x80
#define GMMx194_GTSC_INIT_EN_OFFSET                             8
#define GMMx194_GTSC_INIT_EN_WIDTH                              1
#define GMMx194_GTSC_INIT_EN_MASK                               0x100
#define GMMx194_DCW_EN_OFFSET                                   9
#define GMMx194_DCW_EN_WIDTH                                    1
#define GMMx194_DCW_EN_MASK                                     0x200
#define GMMx194_DCW_PACK_RATIO_OFFSET                           10
#define GMMx194_DCW_PACK_RATIO_WIDTH                            2
#define GMMx194_DCW_PACK_RATIO_MASK                             0xc00
#define GMMx194_SPARE_BITS31_3_OFFSET                           12
#define GMMx194_SPARE_BITS31_3_WIDTH                            20
#define GMMx194_SPARE_BITS31_3_MASK                             0xfffff000

/// GMMx194
typedef union {
  struct {                                                              ///<
    UINT32                                                   DBG_EN:1 ; ///<
    UINT32                                          DSM_SPR_WREN_D4:1 ; ///<
    UINT32                                          DSM_SPR_WREN_D5:1 ; ///<
    UINT32                                          DSM_SPR_RDEN_D5:1 ; ///<
    UINT32                                             DRB_SPR_WREN:1 ; ///<
    UINT32                                             DRB_SPR_RDEN:1 ; ///<
    UINT32                                          GTSC_REFCLK_SEL:1 ; ///<
    UINT32                                            GTSC_SCLK_SEL:1 ; ///<
    UINT32                                             GTSC_INIT_EN:1 ; ///<
    UINT32                                                   DCW_EN:1 ; ///<
    UINT32                                           DCW_PACK_RATIO:2 ; ///<
    UINT32                                           SPARE_BITS31_3:20; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx194_STRUCT;

// **** GMMx198 Register Definition ****
// Address
#define GMMx198_ADDRESS                                         0x198

// Type
#define GMMx198_TYPE                                            TYPE_GMM
// Field Data
#define GMMx198_DSM_SPR_DATA0_OFFSET                            0
#define GMMx198_DSM_SPR_DATA0_WIDTH                             32
#define GMMx198_DSM_SPR_DATA0_MASK                              0xffffffff


// **** GMMx1740 Register Definition ****
// Address
#define GMMx1740_ADDRESS                                        0x1740

// Type
#define GMMx1740_TYPE                                           TYPE_GMM
// Field Data
#define GMMx1740_BIOS_SCRATCH_7_OFFSET                          0
#define GMMx1740_BIOS_SCRATCH_7_WIDTH                           32
#define GMMx1740_BIOS_SCRATCH_7_MASK                            0xffffffff
// VBIOS post check bit
#define ATOM_S7_ASIC_INIT_COMPLETEb1                            0x200

/// GMMx1740
typedef union {
  struct {                                                              ///<
    UINT32                                           BIOS_SCRATCH_7:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx1740_STRUCT;

// **** GMMx2004 Register Definition ****
// Address
#define GMMx2004_ADDRESS                                        0x2004
// Type
#define GMMx2004_TYPE                                           TYPE_GMM

// Field Data
#define GMMx2004_Reserved_11_0_OFFSET                           0
#define GMMx2004_Reserved_11_0_WIDTH                            12
#define GMMx2004_Reserved_11_0_MASK                             0xFFF
#define GMMx2004_NOOFCHAN_OFFSET                                12
#define GMMx2004_NOOFCHAN_WIDTH                                 4
#define GMMx2004_NOOFCHAN_MASK                                  0xF000
#define GMMx2004_Reserved_31_16_OFFSET                          16
#define GMMx2004_Reserved_31_16_WIDTH                           16
#define GMMx2004_Reserved_31_16_MASK                            0xFFFF0000

/// GMMx2004
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_11_0:12; ///<
    UINT32                                                NOOFCHAN:4; ///<
    UINT32                                          Reserved_31_16:16; ///<

  } Field;

  UINT32 Value;
} GMMx2004_STRUCT;

// **** GMMx2008 Register Definition ****
// Address
#define GMMx2008_ADDRESS                                        0x2008
// Type
#define GMMx2008_TYPE                                           TYPE_GMM

// Field Data
#define GMMx2008_CHAN0_OFFSET                                   0
#define GMMx2008_CHAN0_WIDTH                                    3
#define GMMx2008_CHAN0_MASK                                     0x7
#define GMMx2008_CHAN1_OFFSET                                   3
#define GMMx2008_CHAN1_WIDTH                                    3
#define GMMx2008_CHAN1_MASK                                     0x38
#define GMMx2008_CHAN2_OFFSET                                   6
#define GMMx2008_CHAN2_WIDTH                                    3
#define GMMx2008_CHAN2_MASK                                     0x1C0
#define GMMx2008_CHAN3_OFFSET                                   9
#define GMMx2008_CHAN3_WIDTH                                    3
#define GMMx2008_CHAN3_MASK                                     0xE00
#define GMMx2008_Reserved_31_15_OFFSET                          12
#define GMMx2008_Reserved_31_15_WIDTH                           20
#define GMMx2008_Reserved_31_15_MASK                            0xFFFFF000

/// GMMx2008
typedef union {
  struct {                                                            ///<
    UINT32                                                   CHAN0:3; ///<
    UINT32                                                   CHAN1:3; ///<
    UINT32                                                   CHAN2:3; ///<
    UINT32                                                   CHAN3:3; ///<
    UINT32                                          Reserved_31_15:20; ///<

  } Field;

  UINT32 Value;
} GMMx2008_STRUCT;

// **** GMMx2024 Register Definition ****
// Address
#define GMMx2024_ADDRESS                                        0x2024
// Type
#define GMMx2024_TYPE                                           TYPE_GMM

// Field Data
#define GMMx2024_FbBase_OFFSET                                  0
#define GMMx2024_FbBase_WIDTH                                   16
#define GMMx2024_FbBase_MASK                                    0xFFFF
#define GMMx2024_FbTop_OFFSET                                   16
#define GMMx2024_FbTop_WIDTH                                    16
#define GMMx2024_FbTop_MASK                                     0xFFFF0000

/// GMMx2024
typedef union {
  struct {                                                            ///<
    UINT32                                                  FbBase:16; ///<
    UINT32                                                   FbTop:16; ///<

  } Field;

  UINT32 Value;
} GMMx2024_STRUCT;

// **** GMMx2108 Register Definition ****
// Address
#define GMMx2108_ADDRESS                                        0x2108

// Type
#define GMMx2108_TYPE                                           TYPE_GMM
// Field Data
#define GMMx2108_MAXBURST_OFFSET                                0
#define GMMx2108_MAXBURST_WIDTH                                 4
#define GMMx2108_MAXBURST_MASK                                  0xf
#define GMMx2108_LAZY_TIMER_OFFSET                              4
#define GMMx2108_LAZY_TIMER_WIDTH                               4
#define GMMx2108_LAZY_TIMER_MASK                                0xf0
#define GMMx2108_STALL_THRESHOLD_OFFSET                         8
#define GMMx2108_STALL_THRESHOLD_WIDTH                          8
#define GMMx2108_STALL_THRESHOLD_MASK                           0xff00
#define GMMx2108_STALL_MODE_OFFSET                              16
#define GMMx2108_STALL_MODE_WIDTH                               1
#define GMMx2108_STALL_MODE_MASK                                0x10000
#define GMMx2108_Reserved_OFFSET                                17
#define GMMx2108_Reserved_WIDTH                                 15
#define GMMx2108_Reserved_MASK                                  0xfffe0000

/// GMMx2108
typedef union {
  struct {                                                              ///<
    UINT32                                                 MAXBURST:4 ; ///<
    UINT32                                               LAZY_TIMER:4 ; ///<
    UINT32                                          STALL_THRESHOLD:8 ; ///<
    UINT32                                               STALL_MODE:1 ; ///<
    UINT32                                                 Reserved:15; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx2108_STRUCT;

// **** GMMx2198 Register Definition ****
// Address
#define GMMx2198_ADDRESS                                        0x2198

// Type
#define GMMx2198_TYPE                                           TYPE_GMM

// **** GMMx21FC Register Definition ****
// Address
#define GMMx21FC_ADDRESS                                        0x21fc

// Type
#define GMMx21FC_TYPE                                           TYPE_GMM

// **** GMMx3508 Register Definition ****
// Address
#define GMMx3508_ADDRESS                                        0x3508
// Type
#define GMMx3508_TYPE                                           TYPE_GMM

// Field Data
#define GMMx3508_RENG_EXECUTE_ON_PWR_UP_OFFSET                  0
#define GMMx3508_RENG_EXECUTE_ON_PWR_UP_WIDTH                   1
#define GMMx3508_RENG_EXECUTE_ON_PWR_UP_MASK                    0x1
#define GMMx3508_Reserved_31_1_OFFSET                           1
#define GMMx3508_Reserved_31_1_WIDTH                            31
#define GMMx3508_Reserved_31_1_MASK                             0xFFFFFFFE

/// GMMx3508
typedef union {
  struct {                                                            ///<
    UINT32                                  RENG_EXECUTE_ON_PWR_UP:1; ///<
    UINT32                                           Reserved_31_1:31; ///<

  } Field;

  UINT32 Value;
} GMMx3508_STRUCT;

// **** GMMx350C Register Definition ****
// Address
#define GMMx350C_ADDRESS                                        0x350C
// Type
#define GMMx350C_TYPE                                           TYPE_GMM

// Field Data
#define GMMx350C_Reserved_10_0_OFFSET                           0
#define GMMx350C_Reserved_10_0_WIDTH                            11
#define GMMx350C_Reserved_10_0_MASK                             0x7FF
#define GMMx350C_RENG_EXECUTE_ON_REG_UPDATE_OFFSET              11
#define GMMx350C_RENG_EXECUTE_ON_REG_UPDATE_WIDTH               1
#define GMMx350C_RENG_EXECUTE_ON_REG_UPDATE_MASK                0x800
#define GMMx350C_Reserved_15_12_OFFSET                          12
#define GMMx350C_Reserved_15_12_WIDTH                           4
#define GMMx350C_Reserved_15_12_MASK                            0xF000
#define GMMx350C_STCTRL_STUTTER_EN_OFFSET                       16
#define GMMx350C_STCTRL_STUTTER_EN_WIDTH                        1
#define GMMx350C_STCTRL_STUTTER_EN_MASK                         0x10000
#define GMMx350C_Reserved_23_17_OFFSET                          17
#define GMMx350C_Reserved_23_17_WIDTH                           7
#define GMMx350C_Reserved_23_17_MASK                            0x00FE0000
#define GMMx350C_STCTRL_IGNORE_PROTECTION_FAULT_OFFSET          24
#define GMMx350C_STCTRL_IGNORE_PROTECTION_FAULT_WIDTH           1
#define GMMx350C_STCTRL_IGNORE_PROTECTION_FAULT_MASK            0x01000000
#define GMMx350C_Reserved_31_25_OFFSET                          25
#define GMMx350C_Reserved_31_25_WIDTH                           7
#define GMMx350C_Reserved_31_25_MASK                            0xFE000000

/// GMMx350C
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_10_0:11; ///<
    UINT32                              RENG_EXECUTE_ON_REG_UPDATE:1; ///<
    UINT32                                          Reserved_15_12:4; ///<
    UINT32                                       STCTRL_STUTTER_EN:1; ///<
    UINT32                                          Reserved_31_17:15; ///<

  } Field;

  UINT32 Value;
} GMMx350C_STRUCT;

// **** GMMx3510 Register Definition ****
// Address
#define GMMx3510_ADDRESS                                        0x3510
// Type
#define GMMx3510_TYPE                                           TYPE_GMM

// Field Data
#define GMMx3510_RENG_MEM_POWER_CTRL_OVERRIDE0_OFFSET           0
#define GMMx3510_RENG_MEM_POWER_CTRL_OVERRIDE0_WIDTH            3
#define GMMx3510_RENG_MEM_POWER_CTRL_OVERRIDE0_MASK             0x7
#define GMMx3510_RENG_MEM_POWER_CTRL_OVERRIDE1_OFFSET           3
#define GMMx3510_RENG_MEM_POWER_CTRL_OVERRIDE1_WIDTH            3
#define GMMx3510_RENG_MEM_POWER_CTRL_OVERRIDE1_MASK             0x38
#define GMMx3510_STCTRL_NONDISP_IDLE_THRESHOLD_OFFSET           6
#define GMMx3510_STCTRL_NONDISP_IDLE_THRESHOLD_WIDTH            5
#define GMMx3510_STCTRL_NONDISP_IDLE_THRESHOLD_MASK             0x7c0
#define GMMx3510_RENG_SR_HOLD_THRESHOLD_OFFSET                  11
#define GMMx3510_RENG_SR_HOLD_THRESHOLD_WIDTH                   6
#define GMMx3510_RENG_SR_HOLD_THRESHOLD_MASK                    0x1f800
#define GMMx3510_STCTRL_LPT_TARGET_OFFSET                       17
#define GMMx3510_STCTRL_LPT_TARGET_WIDTH                        12
#define GMMx3510_STCTRL_LPT_TARGET_MASK                         0x1ffe0000
#define GMMx3510_STCTRL_IGNORE_ARB_BUSY_OFFSET                  29
#define GMMx3510_STCTRL_IGNORE_ARB_BUSY_WIDTH                   1
#define GMMx3510_STCTRL_IGNORE_ARB_BUSY_MASK                    0x20000000
#define GMMx3510_STCTRL_EXTEND_GMC_OFFLINE_OFFSET               30
#define GMMx3510_STCTRL_EXTEND_GMC_OFFLINE_WIDTH                1
#define GMMx3510_STCTRL_EXTEND_GMC_OFFLINE_MASK                 0x40000000
#define GMMx3510_STCTRL_TIMER_PULSE_OVERRIDE_OFFSET             31
#define GMMx3510_STCTRL_TIMER_PULSE_OVERRIDE_WIDTH              1
#define GMMx3510_STCTRL_TIMER_PULSE_OVERRIDE_MASK               0x80000000

/// GMMx3510
typedef union {
  struct {                                                              ///<
    UINT32                            RENG_MEM_POWER_CTRL_OVERRIDE0:3 ; ///<
    UINT32                            RENG_MEM_POWER_CTRL_OVERRIDE1:3 ; ///<
    UINT32                            STCTRL_NONDISP_IDLE_THRESHOLD:5 ; ///<
    UINT32                                   RENG_SR_HOLD_THRESHOLD:6 ; ///<
    UINT32                                        STCTRL_LPT_TARGET:12; ///<
    UINT32                                   STCTRL_IGNORE_ARB_BUSY:1 ; ///<
    UINT32                                STCTRL_EXTEND_GMC_OFFLINE:1 ; ///<
    UINT32                              STCTRL_TIMER_PULSE_OVERRIDE:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx3510_STRUCT;

// **** GMMx5490 Register Definition ****
// Address
#define GMMx5490_ADDRESS                                        0x5490
// Type
#define GMMx5490_TYPE                                           TYPE_GMM

// Field Data
#define GMMx5490_FB_READ_EN_OFFSET                              0
#define GMMx5490_FB_READ_EN_WIDTH                               1
#define GMMx5490_FB_READ_EN_MASK                                0x1
#define GMMx5490_FB_WRITE_EN_OFFSET                             1
#define GMMx5490_FB_WRITE_EN_WIDTH                              1
#define GMMx5490_FB_WRITE_EN_MASK                               0x2
#define GMMx5490_Reserved_31_2_OFFSET                           2
#define GMMx5490_Reserved_31_2_WIDTH                            30
#define GMMx5490_Reserved_31_2_MASK                             0xFFFFFFFC

/// GMMx5490
typedef union {
  struct {                                                            ///<
    UINT32                                              FB_READ_EN:1; ///<
    UINT32                                             FB_WRITE_EN:1; ///<
    UINT32                                           Reserved_31_2:30; ///<

  } Field;

  UINT32 Value;
} GMMx5490_STRUCT;

// **** GMMx5C6C Register Definition ****
// Address
#define GMMx5C6C_ADDRESS                                        0x5c6c

// Type
#define GMMx5C6C_TYPE                                           TYPE_GMM
// Field Data
#define GMMx5C6C_PLL_RESET_OFFSET                               0
#define GMMx5C6C_PLL_RESET_WIDTH                                1
#define GMMx5C6C_PLL_RESET_MASK                                 0x1
#define GMMx5C6C_PLL_POWER_DOWN_OFFSET                          1
#define GMMx5C6C_PLL_POWER_DOWN_WIDTH                           1
#define GMMx5C6C_PLL_POWER_DOWN_MASK                            0x2
#define GMMx5C6C_PLL_BYPASS_CAL_OFFSET                          2
#define GMMx5C6C_PLL_BYPASS_CAL_WIDTH                           1
#define GMMx5C6C_PLL_BYPASS_CAL_MASK                            0x4
#define GMMx5C6C_PLL_POST_DIV_SRC_OFFSET                        3
#define GMMx5C6C_PLL_POST_DIV_SRC_WIDTH                         1
#define GMMx5C6C_PLL_POST_DIV_SRC_MASK                          0x8
#define GMMx5C6C_PLL_VCOREF_OFFSET                              4
#define GMMx5C6C_PLL_VCOREF_WIDTH                               2
#define GMMx5C6C_PLL_VCOREF_MASK                                0x30
#define GMMx5C6C_PLL_PCIE_REFCLK_SEL_OFFSET                     6
#define GMMx5C6C_PLL_PCIE_REFCLK_SEL_WIDTH                      1
#define GMMx5C6C_PLL_PCIE_REFCLK_SEL_MASK                       0x40
#define GMMx5C6C_PLL_ANTIGLITCH_RESETB_OFFSET                   7
#define GMMx5C6C_PLL_ANTIGLITCH_RESETB_WIDTH                    1
#define GMMx5C6C_PLL_ANTIGLITCH_RESETB_MASK                     0x80
#define GMMx5C6C_PLL_CALREF_OFFSET                              8
#define GMMx5C6C_PLL_CALREF_WIDTH                               2
#define GMMx5C6C_PLL_CALREF_MASK                                0x300
#define GMMx5C6C_PLL_CAL_BYPASS_REFDIV_OFFSET                   10
#define GMMx5C6C_PLL_CAL_BYPASS_REFDIV_WIDTH                    1
#define GMMx5C6C_PLL_CAL_BYPASS_REFDIV_MASK                     0x400
#define GMMx5C6C_PLL_REFCLK_SEL_OFFSET                          11
#define GMMx5C6C_PLL_REFCLK_SEL_WIDTH                           2
#define GMMx5C6C_PLL_REFCLK_SEL_MASK                            0x1800
#define GMMx5C6C_PLL_ANTI_GLITCH_RESET_OFFSET                   13
#define GMMx5C6C_PLL_ANTI_GLITCH_RESET_WIDTH                    1
#define GMMx5C6C_PLL_ANTI_GLITCH_RESET_MASK                     0x2000
#define GMMx5C6C_PLL_XOCLK_DRV_R_EN_OFFSET                      14
#define GMMx5C6C_PLL_XOCLK_DRV_R_EN_WIDTH                       1
#define GMMx5C6C_PLL_XOCLK_DRV_R_EN_MASK                        0x4000
#define GMMx5C6C_Reserved_OFFSET                                15
#define GMMx5C6C_Reserved_WIDTH                                 1
#define GMMx5C6C_Reserved_MASK                                  0x8000
#define GMMx5C6C_PLL_REF_DIV_SRC_OFFSET                         16
#define GMMx5C6C_PLL_REF_DIV_SRC_WIDTH                          3
#define GMMx5C6C_PLL_REF_DIV_SRC_MASK                           0x70000
#define GMMx5C6C_PLL_LOCK_FREQ_SEL_OFFSET                       19
#define GMMx5C6C_PLL_LOCK_FREQ_SEL_WIDTH                        1
#define GMMx5C6C_PLL_LOCK_FREQ_SEL_MASK                         0x80000
#define GMMx5C6C_PLL_CALIB_DONE_OFFSET                          20
#define GMMx5C6C_PLL_CALIB_DONE_WIDTH                           1
#define GMMx5C6C_PLL_CALIB_DONE_MASK                            0x100000
#define GMMx5C6C_PLL_LOCKED_OFFSET                              21
#define GMMx5C6C_PLL_LOCKED_WIDTH                               1
#define GMMx5C6C_PLL_LOCKED_MASK                                0x200000
#define GMMx5C6C_Reserved22_23_OFFSET                           22
#define GMMx5C6C_Reserved22_23_WIDTH                            2
#define GMMx5C6C_Reserved22_23_MASK                             0xc00000
#define GMMx5C6C_PLL_TIMING_MODE_STATUS_OFFSET                  24
#define GMMx5C6C_PLL_TIMING_MODE_STATUS_WIDTH                   2
#define GMMx5C6C_PLL_TIMING_MODE_STATUS_MASK                    0x3000000
#define GMMx5C6C_PLL_DIG_SPARE_OFFSET                           26
#define GMMx5C6C_PLL_DIG_SPARE_WIDTH                            6
#define GMMx5C6C_PLL_DIG_SPARE_MASK                             0xfc000000

/// GMMx5C6C
typedef union {
  struct {                                                              ///<
    UINT32                                                PLL_RESET:1 ; ///<
    UINT32                                           PLL_POWER_DOWN:1 ; ///<
    UINT32                                           PLL_BYPASS_CAL:1 ; ///<
    UINT32                                         PLL_POST_DIV_SRC:1 ; ///<
    UINT32                                               PLL_VCOREF:2 ; ///<
    UINT32                                      PLL_PCIE_REFCLK_SEL:1 ; ///<
    UINT32                                    PLL_ANTIGLITCH_RESETB:1 ; ///<
    UINT32                                               PLL_CALREF:2 ; ///<
    UINT32                                    PLL_CAL_BYPASS_REFDIV:1 ; ///<
    UINT32                                           PLL_REFCLK_SEL:2 ; ///<
    UINT32                                    PLL_ANTI_GLITCH_RESET:1 ; ///<
    UINT32                                       PLL_XOCLK_DRV_R_EN:1 ; ///<
    UINT32                                                 Reserved:1 ; ///<
    UINT32                                          PLL_REF_DIV_SRC:3 ; ///<
    UINT32                                        PLL_LOCK_FREQ_SEL:1 ; ///<
    UINT32                                           PLL_CALIB_DONE:1 ; ///<
    UINT32                                               PLL_LOCKED:1 ; ///<
    UINT32                                            Reserved22_23:2 ; ///<
    UINT32                                   PLL_TIMING_MODE_STATUS:2 ; ///<
    UINT32                                            PLL_DIG_SPARE:6 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx5C6C_STRUCT;

// **** GMMx5C90 Register Definition ****
// Address
#define GMMx5C90_ADDRESS                                        0x5c90

// Type
#define GMMx5C90_TYPE                                           TYPE_GMM
// Field Data
#define GMMx5C90_PLL_XOR_LOCK_OFFSET                            0
#define GMMx5C90_PLL_XOR_LOCK_WIDTH                             1
#define GMMx5C90_PLL_XOR_LOCK_MASK                              0x1
#define GMMx5C90_PLL_XOR_LOCK_READBACK_OFFSET                   1
#define GMMx5C90_PLL_XOR_LOCK_READBACK_WIDTH                    1
#define GMMx5C90_PLL_XOR_LOCK_READBACK_MASK                     0x2
#define GMMx5C90_Reserved_OFFSET                                2
#define GMMx5C90_Reserved_WIDTH                                 6
#define GMMx5C90_Reserved_MASK                                  0xfc
#define GMMx5C90_PLL_SPARE_OFFSET                               8
#define GMMx5C90_PLL_SPARE_WIDTH                                6
#define GMMx5C90_PLL_SPARE_MASK                                 0x3f00
#define GMMx5C90_Reserved14_31_OFFSET                           14
#define GMMx5C90_Reserved14_31_WIDTH                            18
#define GMMx5C90_Reserved14_31_MASK                             0xffffc000

/// GMMx5C90
typedef union {
  struct {                                                              ///<
    UINT32                                             PLL_XOR_LOCK:1 ; ///<
    UINT32                                    PLL_XOR_LOCK_READBACK:1 ; ///<
    UINT32                                                 Reserved:6 ; ///<
    UINT32                                                PLL_SPARE:6 ; ///<
    UINT32                                            Reserved14_31:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx5C90_STRUCT;

// **** GMMx5F50 Register Definition ****
// Address
#define GMMx5F50_ADDRESS                                        0x5F50
// Type
#define GMMx5F50_TYPE                                           TYPE_GMM

// Field Data
#define GMMx5F50_PORT_CONNECTIVITY_OFFSET                       0
#define GMMx5F50_PORT_CONNECTIVITY_WIDTH                        3
#define GMMx5F50_PORT_CONNECTIVITY_MASK                         0x7
#define GMMx5F50_Reserved_3_3_OFFSET                            3
#define GMMx5F50_Reserved_3_3_WIDTH                             1
#define GMMx5F50_Reserved_3_3_MASK                              0x08
#define GMMx5F50_PORT_CONNECTIVITY_OVERRIDE_ENABLE_OFFSET       4
#define GMMx5F50_PORT_CONNECTIVITY_OVERRIDE_ENABLE_WIDTH        1
#define GMMx5F50_PORT_CONNECTIVITY_OVERRIDE_ENABLE_MASK         0x010
#define GMMx5F50_Reserved_31_3_OFFSET                           5
#define GMMx5F50_Reserved_31_3_WIDTH                            27
#define GMMx5F50_Reserved_31_3_MASK                             0xFFFFFFE0

/// GMMx5490
typedef union {
  struct {                                                            ///<
    UINT32                                       PORT_CONNECTIVITY:3; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                       PORT_CONNECTIVITY_OVERRIDE_ENABLE:1; ///<
    UINT32                                           Reserved_31_5:27; ///<

  } Field;

  UINT32 Value;
} GMMx5F50_STRUCT;

// **** GMMx6464 Register Definition ****
// Address
#define GMMx6464_ADDRESS                                        0x6464
// Type
#define GMMx6464_TYPE                                           TYPE_GMM

// Field Data
#define GMMx6464_LVTMA_PWRSEQ_EN_OFFSET                         0
#define GMMx6464_LVTMA_PWRSEQ_EN_WIDTH                          1
#define GMMx6464_LVTMA_PWRSEQ_EN_MASK                           0x1
#define GMMx6464_Reserved_3_1_OFFSET                            1
#define GMMx6464_Reserved_3_1_WIDTH                             3
#define GMMx6464_Reserved_3_1_MASK                              0xE
#define GMMx6464_LVTMA_PWRSEQ_TARGET_STATE_OFFSET               4
#define GMMx6464_LVTMA_PWRSEQ_TARGET_STATE_WIDTH                1
#define GMMx6464_LVTMA_PWRSEQ_TARGET_STATE_MASK                 0x10
#define GMMx6464_Reserved_24_5_OFFSET                           5
#define GMMx6464_Reserved_24_5_WIDTH                            20
#define GMMx6464_Reserved_24_5_MASK                             0x1FFFFE0
#define GMMx6464_LVTMA_BLON_OVRD_OFFSET                         25
#define GMMx6464_LVTMA_BLON_OVRD_WIDTH                          1
#define GMMx6464_LVTMA_BLON_OVRD_MASK                           0x2000000
#define GMMx6464_Reserved_26_26_OFFSET                          26
#define GMMx6464_Reserved_26_26_WIDTH                           1
#define GMMx6464_Reserved_26_26_MASK                            0x4000000
#define GMMx6464_Reserved_31_27_OFFSET                          27
#define GMMx6464_Reserved_31_27_WIDTH                           5
#define GMMx6464_Reserved_31_27_MASK                            0xF8000000

/// GMMx6464
typedef union {
  struct {                                                            ///<
    UINT32                                         LVTMA_PWRSEQ_EN:1; ///<
    UINT32                                            Reserved_3_1:3; ///<
    UINT32                               LVTMA_PWRSEQ_TARGET_STATE:1; ///<
    UINT32                                           Reserved_24_5:20; ///<
    UINT32                                         LVTMA_BLON_OVRD:1; ///<
    UINT32                                          Reserved_26_26:1; ///<
    UINT32                                          Reserved_31_27:5; ///<

  } Field;

  UINT32 Value;
} GMMx6464_STRUCT;

// **** GMMx6CC8 Register Definition ****
// Address
#define GMMx6CC8_ADDRESS                                        0x6cc8

// Type
#define GMMx6CC8_TYPE                                           TYPE_GMM
// Field Data
#define GMMx6CC8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_OFFSET 0
#define GMMx6CC8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_WIDTH 2
#define GMMx6CC8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_MASK  0x3
#define GMMx6CC8_Reserved_OFFSET                                2
#define GMMx6CC8_Reserved_WIDTH                                 6
#define GMMx6CC8_Reserved_MASK                                  0xfc
#define GMMx6CC8_URGENCY_WATERMARK_MASK_OFFSET                  8
#define GMMx6CC8_URGENCY_WATERMARK_MASK_WIDTH                   2
#define GMMx6CC8_URGENCY_WATERMARK_MASK_MASK                    0x300
#define GMMx6CC8_Reserved10_15_OFFSET                           10
#define GMMx6CC8_Reserved10_15_WIDTH                            6
#define GMMx6CC8_Reserved10_15_MASK                             0xfc00
#define GMMx6CC8_NB_PSTATE_CHANGE_WATERMARK_MASK_OFFSET         16
#define GMMx6CC8_NB_PSTATE_CHANGE_WATERMARK_MASK_WIDTH          2
#define GMMx6CC8_NB_PSTATE_CHANGE_WATERMARK_MASK_MASK           0x30000
#define GMMx6CC8_Reserved18_31_OFFSET                           18
#define GMMx6CC8_Reserved18_31_WIDTH                            14
#define GMMx6CC8_Reserved18_31_MASK                             0xfffc0000

/// GMMx6CC8
typedef union {
  struct {                                                              ///<
    UINT32                 STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK:2 ; ///<
    UINT32                                                 Reserved:6 ; ///<
    UINT32                                   URGENCY_WATERMARK_MASK:2 ; ///<
    UINT32                                            Reserved10_15:6 ; ///<
    UINT32                          NB_PSTATE_CHANGE_WATERMARK_MASK:2 ; ///<
    UINT32                                            Reserved18_31:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx6CC8_STRUCT;

// **** GMMx6CD4 Register Definition ****
// Address
#define GMMx6CD4_ADDRESS                                        0x6cd4

// Type
#define GMMx6CD4_TYPE                                           TYPE_GMM
// Field Data
#define GMMx6CD4_STUTTER_ENABLE_OFFSET                          0
#define GMMx6CD4_STUTTER_ENABLE_WIDTH                           1
#define GMMx6CD4_STUTTER_ENABLE_MASK                            0x1
#define GMMx6CD4_Reserved_OFFSET                                1
#define GMMx6CD4_Reserved_WIDTH                                 3
#define GMMx6CD4_Reserved_MASK                                  0xe
#define GMMx6CD4_STUTTER_IGNORE_CURSOR_OFFSET                   4
#define GMMx6CD4_STUTTER_IGNORE_CURSOR_WIDTH                    1
#define GMMx6CD4_STUTTER_IGNORE_CURSOR_MASK                     0x10
#define GMMx6CD4_STUTTER_IGNORE_ICON_OFFSET                     5
#define GMMx6CD4_STUTTER_IGNORE_ICON_WIDTH                      1
#define GMMx6CD4_STUTTER_IGNORE_ICON_MASK                       0x20
#define GMMx6CD4_STUTTER_IGNORE_VGA_OFFSET                      6
#define GMMx6CD4_STUTTER_IGNORE_VGA_WIDTH                       1
#define GMMx6CD4_STUTTER_IGNORE_VGA_MASK                        0x40
#define GMMx6CD4_STUTTER_IGNORE_FBC_OFFSET                      7
#define GMMx6CD4_STUTTER_IGNORE_FBC_WIDTH                       1
#define GMMx6CD4_STUTTER_IGNORE_FBC_MASK                        0x80
#define GMMx6CD4_STUTTER_WM_HIGH_FORCE_ON_OFFSET                8
#define GMMx6CD4_STUTTER_WM_HIGH_FORCE_ON_WIDTH                 1
#define GMMx6CD4_STUTTER_WM_HIGH_FORCE_ON_MASK                  0x100
#define GMMx6CD4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_OFFSET         9
#define GMMx6CD4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_WIDTH          1
#define GMMx6CD4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_MASK           0x200
#define GMMx6CD4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_OFFSET      10
#define GMMx6CD4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_WIDTH       1
#define GMMx6CD4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_MASK        0x400
#define GMMx6CD4_STUTTER_SELF_REFRESH_FORCE_ON_OFFSET           11
#define GMMx6CD4_STUTTER_SELF_REFRESH_FORCE_ON_WIDTH            1
#define GMMx6CD4_STUTTER_SELF_REFRESH_FORCE_ON_MASK             0x800
#define GMMx6CD4_Reserved12_15_OFFSET                           12
#define GMMx6CD4_Reserved12_15_WIDTH                            4
#define GMMx6CD4_Reserved12_15_MASK                             0xf000
#define GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET     16
#define GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_WIDTH      16
#define GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK       0xffff0000

/// GMMx6CD4
typedef union {
  struct {                                                              ///<
    UINT32                                           STUTTER_ENABLE:1 ; ///<
    UINT32                                                 Reserved:3 ; ///<
    UINT32                                    STUTTER_IGNORE_CURSOR:1 ; ///<
    UINT32                                      STUTTER_IGNORE_ICON:1 ; ///<
    UINT32                                       STUTTER_IGNORE_VGA:1 ; ///<
    UINT32                                       STUTTER_IGNORE_FBC:1 ; ///<
    UINT32                                 STUTTER_WM_HIGH_FORCE_ON:1 ; ///<
    UINT32                          STUTTER_WM_HIGH_EXCLUDES_VBLANK:1 ; ///<
    UINT32                       STUTTER_URGENT_IN_NOT_SELF_REFRESH:1 ; ///<
    UINT32                            STUTTER_SELF_REFRESH_FORCE_ON:1 ; ///<
    UINT32                                            Reserved12_15:4 ; ///<
    UINT32                      STUTTER_EXIT_SELF_REFRESH_WATERMARK:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx6CD4_STRUCT;

// **** GMMx6CD8 Register Definition ****
// Address
#define GMMx6CD8_ADDRESS                                        0x6cd8

// Type
#define GMMx6CD8_TYPE                                           TYPE_GMM
// Field Data
#define GMMx6CD8_NB_PSTATE_CHANGE_ENABLE_OFFSET                 0
#define GMMx6CD8_NB_PSTATE_CHANGE_ENABLE_WIDTH                  1
#define GMMx6CD8_NB_PSTATE_CHANGE_ENABLE_MASK                   0x1
#define GMMx6CD8_Reserved_3_1_OFFSET                            1
#define GMMx6CD8_Reserved_3_1_WIDTH                             3
#define GMMx6CD8_Reserved_3_1_MASK                              0xe
#define GMMx6CD8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_OFFSET  4
#define GMMx6CD8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_WIDTH   1
#define GMMx6CD8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_MASK    0x10
#define GMMx6CD8_Reserved_7_5_OFFSET                            5
#define GMMx6CD8_Reserved_7_5_WIDTH                             3
#define GMMx6CD8_Reserved_7_5_MASK                              0xe0
#define GMMx6CD8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_OFFSET 8
#define GMMx6CD8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_WIDTH 1
#define GMMx6CD8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_MASK 0x100
#define GMMx6CD8_NB_PSTATE_CHANGE_FORCE_ON_OFFSET               9
#define GMMx6CD8_NB_PSTATE_CHANGE_FORCE_ON_WIDTH                1
#define GMMx6CD8_NB_PSTATE_CHANGE_FORCE_ON_MASK                 0x200
#define GMMx6CD8_NB_PSTATE_ALLOW_FOR_URGENT_OFFSET              10
#define GMMx6CD8_NB_PSTATE_ALLOW_FOR_URGENT_WIDTH               1
#define GMMx6CD8_NB_PSTATE_ALLOW_FOR_URGENT_MASK                0x400
#define GMMx6CD8_Reserved_15_11_OFFSET                          11
#define GMMx6CD8_Reserved_15_11_WIDTH                           5
#define GMMx6CD8_Reserved_15_11_MASK                            0xf800
#define GMMx6CD8_NB_PSTATE_CHANGE_WATERMARK_OFFSET              16
#define GMMx6CD8_NB_PSTATE_CHANGE_WATERMARK_WIDTH               16
#define GMMx6CD8_NB_PSTATE_CHANGE_WATERMARK_MASK                0xffff0000

/// GMMx6CD8
typedef union {
  struct {                                                              ///<
    UINT32                                  NB_PSTATE_CHANGE_ENABLE:1 ; ///<
    UINT32                                             Reserved_3_1:3 ; ///<
    UINT32                   NB_PSTATE_CHANGE_URGENT_DURING_REQUEST:1 ; ///<
    UINT32                                             Reserved_7_5:3 ; ///<
    UINT32         NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST:1 ; ///<
    UINT32                                NB_PSTATE_CHANGE_FORCE_ON:1 ; ///<
    UINT32                               NB_PSTATE_ALLOW_FOR_URGENT:1 ; ///<
    UINT32                                           Reserved_15_11:5 ; ///<
    UINT32                               NB_PSTATE_CHANGE_WATERMARK:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx6CD8_STRUCT;

// **** GMMx78C8 Register Definition ****
// Address
#define GMMx78C8_ADDRESS                                        0x78C8

// Type
#define GMMx78C8_TYPE                                           TYPE_GMM
// Field Data
#define GMMx78C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_OFFSET 0
#define GMMx78C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_WIDTH 2
#define GMMx78C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_MASK  0x3
#define GMMx78C8_Reserved_OFFSET                                2
#define GMMx78C8_Reserved_WIDTH                                 6
#define GMMx78C8_Reserved_MASK                                  0xfc
#define GMMx78C8_URGENCY_WATERMARK_MASK_OFFSET                  8
#define GMMx78C8_URGENCY_WATERMARK_MASK_WIDTH                   2
#define GMMx78C8_URGENCY_WATERMARK_MASK_MASK                    0x300
#define GMMx78C8_Reserved10_15_OFFSET                           10
#define GMMx78C8_Reserved10_15_WIDTH                            6
#define GMMx78C8_Reserved10_15_MASK                             0xfc00
#define GMMx78C8_NB_PSTATE_CHANGE_WATERMARK_MASK_OFFSET         16
#define GMMx78C8_NB_PSTATE_CHANGE_WATERMARK_MASK_WIDTH          2
#define GMMx78C8_NB_PSTATE_CHANGE_WATERMARK_MASK_MASK           0x30000
#define GMMx78C8_Reserved18_31_OFFSET                           18
#define GMMx78C8_Reserved18_31_WIDTH                            14
#define GMMx78C8_Reserved18_31_MASK                             0xfffc0000

/// GMMx78C8
typedef union {
  struct {                                                              ///<
    UINT32                 STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK:2 ; ///<
    UINT32                                                 Reserved:6 ; ///<
    UINT32                                   URGENCY_WATERMARK_MASK:2 ; ///<
    UINT32                                            Reserved10_15:6 ; ///<
    UINT32                          NB_PSTATE_CHANGE_WATERMARK_MASK:2 ; ///<
    UINT32                                            Reserved18_31:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx78C8_STRUCT;

// **** GMMx78D4 Register Definition ****
// Address
#define GMMx78D4_ADDRESS                                        0x78d4

// Type
#define GMMx78D4_TYPE                                           TYPE_GMM
// Field Data
#define GMMx78D4_STUTTER_ENABLE_OFFSET                          0
#define GMMx78D4_STUTTER_ENABLE_WIDTH                           1
#define GMMx78D4_STUTTER_ENABLE_MASK                            0x1
#define GMMx78D4_Reserved_OFFSET                                1
#define GMMx78D4_Reserved_WIDTH                                 3
#define GMMx78D4_Reserved_MASK                                  0xe
#define GMMx78D4_STUTTER_IGNORE_CURSOR_OFFSET                   4
#define GMMx78D4_STUTTER_IGNORE_CURSOR_WIDTH                    1
#define GMMx78D4_STUTTER_IGNORE_CURSOR_MASK                     0x10
#define GMMx78D4_STUTTER_IGNORE_ICON_OFFSET                     5
#define GMMx78D4_STUTTER_IGNORE_ICON_WIDTH                      1
#define GMMx78D4_STUTTER_IGNORE_ICON_MASK                       0x20
#define GMMx78D4_STUTTER_IGNORE_VGA_OFFSET                      6
#define GMMx78D4_STUTTER_IGNORE_VGA_WIDTH                       1
#define GMMx78D4_STUTTER_IGNORE_VGA_MASK                        0x40
#define GMMx78D4_STUTTER_IGNORE_FBC_OFFSET                      7
#define GMMx78D4_STUTTER_IGNORE_FBC_WIDTH                       1
#define GMMx78D4_STUTTER_IGNORE_FBC_MASK                        0x80
#define GMMx78D4_STUTTER_WM_HIGH_FORCE_ON_OFFSET                8
#define GMMx78D4_STUTTER_WM_HIGH_FORCE_ON_WIDTH                 1
#define GMMx78D4_STUTTER_WM_HIGH_FORCE_ON_MASK                  0x100
#define GMMx78D4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_OFFSET         9
#define GMMx78D4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_WIDTH          1
#define GMMx78D4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_MASK           0x200
#define GMMx78D4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_OFFSET      10
#define GMMx78D4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_WIDTH       1
#define GMMx78D4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_MASK        0x400
#define GMMx78D4_STUTTER_SELF_REFRESH_FORCE_ON_OFFSET           11
#define GMMx78D4_STUTTER_SELF_REFRESH_FORCE_ON_WIDTH            1
#define GMMx78D4_STUTTER_SELF_REFRESH_FORCE_ON_MASK             0x800
#define GMMx78D4_Reserved12_15_OFFSET                           12
#define GMMx78D4_Reserved12_15_WIDTH                            4
#define GMMx78D4_Reserved12_15_MASK                             0xf000
#define GMMx78D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET     16
#define GMMx78D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_WIDTH      16
#define GMMx78D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK       0xffff0000

/// GMMx78D4
typedef union {
  struct {                                                              ///<
    UINT32                                           STUTTER_ENABLE:1 ; ///<
    UINT32                                                 Reserved:3 ; ///<
    UINT32                                    STUTTER_IGNORE_CURSOR:1 ; ///<
    UINT32                                      STUTTER_IGNORE_ICON:1 ; ///<
    UINT32                                       STUTTER_IGNORE_VGA:1 ; ///<
    UINT32                                       STUTTER_IGNORE_FBC:1 ; ///<
    UINT32                                 STUTTER_WM_HIGH_FORCE_ON:1 ; ///<
    UINT32                          STUTTER_WM_HIGH_EXCLUDES_VBLANK:1 ; ///<
    UINT32                       STUTTER_URGENT_IN_NOT_SELF_REFRESH:1 ; ///<
    UINT32                            STUTTER_SELF_REFRESH_FORCE_ON:1 ; ///<
    UINT32                                            Reserved12_15:4 ; ///<
    UINT32                      STUTTER_EXIT_SELF_REFRESH_WATERMARK:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx78D4_STRUCT;

// **** GMMx78D8 Register Definition ****
// Address
#define GMMx78D8_ADDRESS                                        0x78d8

// Type
#define GMMx78D8_TYPE                                           TYPE_GMM
// Field Data
#define GMMx78D8_NB_PSTATE_CHANGE_ENABLE_OFFSET                 0
#define GMMx78D8_NB_PSTATE_CHANGE_ENABLE_WIDTH                  1
#define GMMx78D8_NB_PSTATE_CHANGE_ENABLE_MASK                   0x1
#define GMMx78D8_Reserved_3_1_OFFSET                            1
#define GMMx78D8_Reserved_3_1_WIDTH                             3
#define GMMx78D8_Reserved_3_1_MASK                              0xe
#define GMMx78D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_OFFSET  4
#define GMMx78D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_WIDTH   1
#define GMMx78D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_MASK    0x10
#define GMMx78D8_Reserved_7_5_OFFSET                            5
#define GMMx78D8_Reserved_7_5_WIDTH                             3
#define GMMx78D8_Reserved_7_5_MASK                              0xe0
#define GMMx78D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_OFFSET 8
#define GMMx78D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_WIDTH 1
#define GMMx78D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_MASK 0x100
#define GMMx78D8_NB_PSTATE_CHANGE_FORCE_ON_OFFSET               9
#define GMMx78D8_NB_PSTATE_CHANGE_FORCE_ON_WIDTH                1
#define GMMx78D8_NB_PSTATE_CHANGE_FORCE_ON_MASK                 0x200
#define GMMx78D8_NB_PSTATE_ALLOW_FOR_URGENT_OFFSET              10
#define GMMx78D8_NB_PSTATE_ALLOW_FOR_URGENT_WIDTH               1
#define GMMx78D8_NB_PSTATE_ALLOW_FOR_URGENT_MASK                0x400
#define GMMx78D8_Reserved_15_11_OFFSET                          11
#define GMMx78D8_Reserved_15_11_WIDTH                           5
#define GMMx78D8_Reserved_15_11_MASK                            0xf800
#define GMMx78D8_NB_PSTATE_CHANGE_WATERMARK_OFFSET              16
#define GMMx78D8_NB_PSTATE_CHANGE_WATERMARK_WIDTH               16
#define GMMx78D8_NB_PSTATE_CHANGE_WATERMARK_MASK                0xffff0000

/// GMMx78D8
typedef union {
  struct {                                                              ///<
    UINT32                                  NB_PSTATE_CHANGE_ENABLE:1 ; ///<
    UINT32                                             Reserved_3_1:3 ; ///<
    UINT32                   NB_PSTATE_CHANGE_URGENT_DURING_REQUEST:1 ; ///<
    UINT32                                             Reserved_7_5:3 ; ///<
    UINT32         NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST:1 ; ///<
    UINT32                                NB_PSTATE_CHANGE_FORCE_ON:1 ; ///<
    UINT32                               NB_PSTATE_ALLOW_FOR_URGENT:1 ; ///<
    UINT32                                           Reserved_15_11:5 ; ///<
    UINT32                               NB_PSTATE_CHANGE_WATERMARK:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx78D8_STRUCT;

// **** D0F0xBC_x80000000 Register Definition ****
// Address
#define D0F0xBC_x80000000_ADDRESS                               0x80000000
// Type
#define D0F0xBC_x80000000_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_x80000000_smu_rst_reg_OFFSET                    0
#define D0F0xBC_x80000000_smu_rst_reg_WIDTH                     1
#define D0F0xBC_x80000000_smu_rst_reg_MASK                      0x1
#define D0F0xBC_x80000000_srbm_soft_rst_override_OFFSET         1
#define D0F0xBC_x80000000_srbm_soft_rst_override_WIDTH          1
#define D0F0xBC_x80000000_srbm_soft_rst_override_MASK           0x2
#define D0F0xBC_x80000000_Reserved_29_2_OFFSET                  2
#define D0F0xBC_x80000000_Reserved_29_2_WIDTH                   28
#define D0F0xBC_x80000000_Reserved_29_2_MASK                    0x3FFFFFFC
#define D0F0xBC_x80000000_RegReset_OFFSET                       30
#define D0F0xBC_x80000000_RegReset_WIDTH                        1
#define D0F0xBC_x80000000_RegReset_MASK                         0x40000000
#define D0F0xBC_x80000000_Reserved_31_31_OFFSET                 31
#define D0F0xBC_x80000000_Reserved_31_31_WIDTH                  1
#define D0F0xBC_x80000000_Reserved_31_31_MASK                   0x80000000

/// D0F0xBC_x80000000
typedef union {
  struct {                                                            ///<
    UINT32                                             smu_rst_reg:1; ///<
    UINT32                                  srbm_soft_rst_override:1; ///<
    UINT32                                           Reserved_29_2:28; ///<
    UINT32                                                RegReset:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_x80000000_STRUCT;

// **** D0F0xBC_x80000004 Register Definition ****
// Address
#define D0F0xBC_x80000004_ADDRESS                               0x80000004
// Type
#define D0F0xBC_x80000004_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_x80000004_smu_ck_disable_OFFSET                 0
#define D0F0xBC_x80000004_smu_ck_disable_WIDTH                  1
#define D0F0xBC_x80000004_smu_ck_disable_MASK                   0x1
#define D0F0xBC_x80000004_smu_auto_cg_en_OFFSET                 1
#define D0F0xBC_x80000004_smu_auto_cg_en_WIDTH                  1
#define D0F0xBC_x80000004_smu_auto_cg_en_MASK                   0x2
#define D0F0xBC_x80000004_Reserved_7_2_OFFSET                   2
#define D0F0xBC_x80000004_Reserved_7_2_WIDTH                    6
#define D0F0xBC_x80000004_Reserved_7_2_MASK                     0xFC
#define D0F0xBC_x80000004_smu_auto_cg_timeout_OFFSET            8
#define D0F0xBC_x80000004_smu_auto_cg_timeout_WIDTH             16
#define D0F0xBC_x80000004_smu_auto_cg_timeout_MASK              0xFFFF00
#define D0F0xBC_x80000004_smu_cken_OFFSET                       24
#define D0F0xBC_x80000004_smu_cken_WIDTH                        1
#define D0F0xBC_x80000004_smu_cken_MASK                         0x1000000
#define D0F0xBC_x80000004_Reserved_31_25_OFFSET                 25
#define D0F0xBC_x80000004_Reserved_31_25_WIDTH                  7
#define D0F0xBC_x80000004_Reserved_31_25_MASK                   0xFE000000

/// D0F0xBC_x80000004
typedef union {
  struct {                                                            ///<
    UINT32                                          smu_ck_disable:1; ///<
    UINT32                                          smu_auto_cg_en:1; ///<
    UINT32                                            Reserved_7_2:6; ///<
    UINT32                                     smu_auto_cg_timeout:16; ///<
    UINT32                                                smu_cken:1; ///<
    UINT32                                          Reserved_31_25:7; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_x80000004_STRUCT;

// **** D0F0xBC_xC0500000 Register Definition ****
// Address
#define D0F0xBC_xC0500000_ADDRESS                               0xC0500000
// Type
#define D0F0xBC_xC0500000_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC0500000_GckFuseProg_OFFSET                    0
#define D0F0xBC_xC0500000_GckFuseProg_WIDTH                     1
#define D0F0xBC_xC0500000_GckFuseProg_MASK                      0x1
#define D0F0xBC_xC0500000_MainPllOpFreqIdStartup_OFFSET         1
#define D0F0xBC_xC0500000_MainPllOpFreqIdStartup_WIDTH          6
#define D0F0xBC_xC0500000_MainPllOpFreqIdStartup_MASK           0x7E
#define D0F0xBC_xC0500000_MainPllOpFreqIdMax_OFFSET             7
#define D0F0xBC_xC0500000_MainPllOpFreqIdMax_WIDTH              6
#define D0F0xBC_xC0500000_MainPllOpFreqIdMax_MASK               0x1F80
#define D0F0xBC_xC0500000_MainPllRefAdj_OFFSET                  13
#define D0F0xBC_xC0500000_MainPllRefAdj_WIDTH                   5
#define D0F0xBC_xC0500000_MainPllRefAdj_MASK                    0x3E000
#define D0F0xBC_xC0500000_PllMiscFuseCtl_OFFSET                 18
#define D0F0xBC_xC0500000_PllMiscFuseCtl_WIDTH                  4
#define D0F0xBC_xC0500000_PllMiscFuseCtl_MASK                   0x3C0000
#define D0F0xBC_xC0500000_Reserved_31_22_OFFSET                 22
#define D0F0xBC_xC0500000_Reserved_31_22_WIDTH                  10
#define D0F0xBC_xC0500000_Reserved_31_22_MASK                   0xFFC00000

/// D0F0xBC_xC0500000
typedef union {
  struct {                                                            ///<
    UINT32                                             GckFuseProg:1; ///<
    UINT32                                  MainPllOpFreqIdStartup:6; ///<
    UINT32                                      MainPllOpFreqIdMax:6; ///<
    UINT32                                           MainPllRefAdj:5; ///<
    UINT32                                          PllMiscFuseCtl:4; ///<
    UINT32                                          Reserved_31_22:10; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xC0500000_STRUCT;

// **** D0F0xBC_xC0200000 Register Definition ****
// Address
#define D0F0xBC_xC0200000_ADDRESS                               0xC0200000
// Type
#define D0F0xBC_xC0200000_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC0200000_GLOBAL_PWRMGT_EN_OFFSET               0
#define D0F0xBC_xC0200000_GLOBAL_PWRMGT_EN_WIDTH                1
#define D0F0xBC_xC0200000_GLOBAL_PWRMGT_EN_MASK                 0x1
#define D0F0xBC_xC0200000_STATIC_PM_EN_OFFSET                   1
#define D0F0xBC_xC0200000_STATIC_PM_EN_WIDTH                    1
#define D0F0xBC_xC0200000_STATIC_PM_EN_MASK                     0x2
#define D0F0xBC_xC0200000_THERMAL_PROTECTION_DIS_OFFSET         2
#define D0F0xBC_xC0200000_THERMAL_PROTECTION_DIS_WIDTH          1
#define D0F0xBC_xC0200000_THERMAL_PROTECTION_DIS_MASK           0x4
#define D0F0xBC_xC0200000_THERMAL_PROTECTION_TYPE_OFFSET        3
#define D0F0xBC_xC0200000_THERMAL_PROTECTION_TYPE_WIDTH         1
#define D0F0xBC_xC0200000_THERMAL_PROTECTION_TYPE_MASK          0x8
#define D0F0xBC_xC0200000_Reserved_5_4_OFFSET                   4
#define D0F0xBC_xC0200000_Reserved_5_4_WIDTH                    2
#define D0F0xBC_xC0200000_Reserved_5_4_MASK                     0x30
#define D0F0xBC_xC0200000_SW_SMIO_INDEX_OFFSET                  6
#define D0F0xBC_xC0200000_SW_SMIO_INDEX_WIDTH                   1
#define D0F0xBC_xC0200000_SW_SMIO_INDEX_MASK                    0x40
#define D0F0xBC_xC0200000_Reserved_7_7_OFFSET                   7
#define D0F0xBC_xC0200000_Reserved_7_7_WIDTH                    1
#define D0F0xBC_xC0200000_Reserved_7_7_MASK                     0x80
#define D0F0xBC_xC0200000_LOW_VOLT_D2_ACPI_OFFSET               8
#define D0F0xBC_xC0200000_LOW_VOLT_D2_ACPI_WIDTH                1
#define D0F0xBC_xC0200000_LOW_VOLT_D2_ACPI_MASK                 0x100
#define D0F0xBC_xC0200000_LOW_VOLT_D3_ACPI_OFFSET               9
#define D0F0xBC_xC0200000_LOW_VOLT_D3_ACPI_WIDTH                1
#define D0F0xBC_xC0200000_LOW_VOLT_D3_ACPI_MASK                 0x200
#define D0F0xBC_xC0200000_VOLT_PWRMGT_EN_OFFSET                 10
#define D0F0xBC_xC0200000_VOLT_PWRMGT_EN_WIDTH                  1
#define D0F0xBC_xC0200000_VOLT_PWRMGT_EN_MASK                   0x400
#define D0F0xBC_xC0200000_SPARE11_OFFSET                        11
#define D0F0xBC_xC0200000_SPARE11_WIDTH                         1
#define D0F0xBC_xC0200000_SPARE11_MASK                          0x800
#define D0F0xBC_xC0200000_Reserved_13_12_OFFSET                 12
#define D0F0xBC_xC0200000_Reserved_13_12_WIDTH                  2
#define D0F0xBC_xC0200000_Reserved_13_12_MASK                   0x3000
#define D0F0xBC_xC0200000_GPU_COUNTER_ACPI_OFFSET               14
#define D0F0xBC_xC0200000_GPU_COUNTER_ACPI_WIDTH                1
#define D0F0xBC_xC0200000_GPU_COUNTER_ACPI_MASK                 0x4000
#define D0F0xBC_xC0200000_GPU_COUNTER_CLK_OFFSET                15
#define D0F0xBC_xC0200000_GPU_COUNTER_CLK_WIDTH                 1
#define D0F0xBC_xC0200000_GPU_COUNTER_CLK_MASK                  0x8000
#define D0F0xBC_xC0200000_GPU_COUNTER_OFF_OFFSET                16
#define D0F0xBC_xC0200000_GPU_COUNTER_OFF_WIDTH                 1
#define D0F0xBC_xC0200000_GPU_COUNTER_OFF_MASK                  0x10000
#define D0F0xBC_xC0200000_GPU_COUNTER_INTF_OFF_OFFSET           17
#define D0F0xBC_xC0200000_GPU_COUNTER_INTF_OFF_WIDTH            1
#define D0F0xBC_xC0200000_GPU_COUNTER_INTF_OFF_MASK             0x20000
#define D0F0xBC_xC0200000_SPARE18_OFFSET                        18
#define D0F0xBC_xC0200000_SPARE18_WIDTH                         1
#define D0F0xBC_xC0200000_SPARE18_MASK                          0x40000
#define D0F0xBC_xC0200000_ACPI_D3_VID_OFFSET                    19
#define D0F0xBC_xC0200000_ACPI_D3_VID_WIDTH                     2
#define D0F0xBC_xC0200000_ACPI_D3_VID_MASK                      0x180000
#define D0F0xBC_xC0200000_Reserved_22_21_OFFSET                 21
#define D0F0xBC_xC0200000_Reserved_22_21_WIDTH                  2
#define D0F0xBC_xC0200000_Reserved_22_21_MASK                   0x600000
#define D0F0xBC_xC0200000_DYN_SPREAD_SPECTRUM_EN_OFFSET         23
#define D0F0xBC_xC0200000_DYN_SPREAD_SPECTRUM_EN_WIDTH          1
#define D0F0xBC_xC0200000_DYN_SPREAD_SPECTRUM_EN_MASK           0x800000
#define D0F0xBC_xC0200000_Reserved_26_24_OFFSET                 24
#define D0F0xBC_xC0200000_Reserved_26_24_WIDTH                  3
#define D0F0xBC_xC0200000_Reserved_26_24_MASK                   0x7000000
#define D0F0xBC_xC0200000_SPARE27_OFFSET                        27
#define D0F0xBC_xC0200000_SPARE27_WIDTH                         1
#define D0F0xBC_xC0200000_SPARE27_MASK                          0x8000000
#define D0F0xBC_xC0200000_SPARE_OFFSET                          28
#define D0F0xBC_xC0200000_SPARE_WIDTH                           4
#define D0F0xBC_xC0200000_SPARE_MASK                            0xF0000000

/// D0F0xBC_xC0200000
typedef union {
  struct {                                                            ///<
    UINT32                                        GLOBAL_PWRMGT_EN:1; ///<
    UINT32                                            STATIC_PM_EN:1; ///<
    UINT32                                  THERMAL_PROTECTION_DIS:1; ///<
    UINT32                                 THERMAL_PROTECTION_TYPE:1; ///<
    UINT32                                            Reserved_5_4:2; ///<
    UINT32                                           SW_SMIO_INDEX:1; ///<
    UINT32                                            Reserved_7_7:1; ///<
    UINT32                                        LOW_VOLT_D2_ACPI:1; ///<
    UINT32                                        LOW_VOLT_D3_ACPI:1; ///<
    UINT32                                          VOLT_PWRMGT_EN:1; ///<
    UINT32                                                 SPARE11:1; ///<
    UINT32                                          Reserved_13_12:2; ///<
    UINT32                                        GPU_COUNTER_ACPI:1; ///<
    UINT32                                         GPU_COUNTER_CLK:1; ///<
    UINT32                                         GPU_COUNTER_OFF:1; ///<
    UINT32                                    GPU_COUNTER_INTF_OFF:1; ///<
    UINT32                                                 SPARE18:1; ///<
    UINT32                                             ACPI_D3_VID:2; ///<
    UINT32                                          Reserved_22_21:2; ///<
    UINT32                                  DYN_SPREAD_SPECTRUM_EN:1; ///<
    UINT32                                          Reserved_26_24:3; ///<
    UINT32                                                 SPARE27:1; ///<
    UINT32                                                   SPARE:4; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xC0200000_STRUCT;

// **** D0F0xBC_xC020008C Register Definition ****
// Address
#define D0F0xBC_xC020008C_ADDRESS                               0xc020008c

// Type
#define D0F0xBC_xC020008C_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC020008C_DIV_ID_OFFSET                         0
#define D0F0xBC_xC020008C_DIV_ID_WIDTH                          3
#define D0F0xBC_xC020008C_DIV_ID_MASK                           0x7
#define D0F0xBC_xC020008C_RAMP_DIS_OFFSET                       3
#define D0F0xBC_xC020008C_RAMP_DIS_WIDTH                        1
#define D0F0xBC_xC020008C_RAMP_DIS_MASK                         0x8
#define D0F0xBC_xC020008C_HYSTERESIS_OFFSET                     4
#define D0F0xBC_xC020008C_HYSTERESIS_WIDTH                      12
#define D0F0xBC_xC020008C_HYSTERESIS_MASK                       0xfff0
#define D0F0xBC_xC020008C_RESERVED_OFFSET                       16
#define D0F0xBC_xC020008C_RESERVED_WIDTH                        15
#define D0F0xBC_xC020008C_RESERVED_MASK                         0x7fff0000
#define D0F0xBC_xC020008C_ENABLE_DS_OFFSET                      31
#define D0F0xBC_xC020008C_ENABLE_DS_WIDTH                       1
#define D0F0xBC_xC020008C_ENABLE_DS_MASK                        0x80000000

/// D0F0xBC_xC020008C
typedef union {
  struct {                                                              ///<
    UINT32                                                   DIV_ID:3 ; ///<
    UINT32                                                 RAMP_DIS:1 ; ///<
    UINT32                                               HYSTERESIS:12; ///<
    UINT32                                                 RESERVED:15; ///<
    UINT32                                                ENABLE_DS:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC020008C_STRUCT;

// **** D0F0xBC_xC0200110 Register Definition ****
// Address
#define D0F0xBC_xC0200110_ADDRESS                               0xc0200110

// Type
#define D0F0xBC_xC0200110_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC0200110_ActivityCntRst_OFFSET                 0
#define D0F0xBC_xC0200110_ActivityCntRst_WIDTH                  1
#define D0F0xBC_xC0200110_ActivityCntRst_MASK                   0x1
#define D0F0xBC_xC0200110_PeriodCntRst_OFFSET                   1
#define D0F0xBC_xC0200110_PeriodCntRst_WIDTH                    1
#define D0F0xBC_xC0200110_PeriodCntRst_MASK                     0x2
#define D0F0xBC_xC0200110_Reserved_2_2_OFFSET                   2
#define D0F0xBC_xC0200110_Reserved_2_2_WIDTH                    1
#define D0F0xBC_xC0200110_Reserved_2_2_MASK                     0x4
#define D0F0xBC_xC0200110_BusyCntSel_OFFSET                     3
#define D0F0xBC_xC0200110_BusyCntSel_WIDTH                      2
#define D0F0xBC_xC0200110_BusyCntSel_MASK                       0x18
#define D0F0xBC_xC0200110_Reserved_7_5_OFFSET                   5
#define D0F0xBC_xC0200110_Reserved_7_5_WIDTH                    3
#define D0F0xBC_xC0200110_Reserved_7_5_MASK                     0xe0
#define D0F0xBC_xC0200110_EnBifCnt_OFFSET                       8
#define D0F0xBC_xC0200110_EnBifCnt_WIDTH                        1
#define D0F0xBC_xC0200110_EnBifCnt_MASK                         0x100
#define D0F0xBC_xC0200110_EnOrbUsCnt_OFFSET                     9
#define D0F0xBC_xC0200110_EnOrbUsCnt_WIDTH                      1
#define D0F0xBC_xC0200110_EnOrbUsCnt_MASK                       0x200
#define D0F0xBC_xC0200110_EnOrbDsCnt_OFFSET                     10
#define D0F0xBC_xC0200110_EnOrbDsCnt_WIDTH                      1
#define D0F0xBC_xC0200110_EnOrbDsCnt_MASK                       0x400
#define D0F0xBC_xC0200110_Reserved_31_11_OFFSET                 11
#define D0F0xBC_xC0200110_Reserved_31_11_WIDTH                  21
#define D0F0xBC_xC0200110_Reserved_31_11_MASK                   0xfffff800

/// D0F0xBC_xC0200110
typedef union {
  struct {                                                              ///<
    UINT32                                           ActivityCntRst:1 ; ///<
    UINT32                                             PeriodCntRst:1 ; ///<
    UINT32                                             Reserved_2_2:1 ; ///<
    UINT32                                               BusyCntSel:2 ; ///<
    UINT32                                             Reserved_7_5:3 ; ///<
    UINT32                                                 EnBifCnt:1 ; ///<
    UINT32                                               EnOrbUsCnt:1 ; ///<
    UINT32                                               EnOrbDsCnt:1 ; ///<
    UINT32                                           Reserved_31_11:21; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0200110_STRUCT;

// **** D0F0xBC_xC0200200 Register Definition ****
// Address
#define D0F0xBC_xC0200200_ADDRESS                               0xc0200200

// Type
#define D0F0xBC_xC0200200_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC0200200_Reserved_0_OFFSET                     0
#define D0F0xBC_xC0200200_Reserved_0_WIDTH                      1
#define D0F0xBC_xC0200200_Reserved_0_MASK                       0x1
#define D0F0xBC_xC0200200_Reserved_31_1_OFFSET                  1
#define D0F0xBC_xC0200200_Reserved_31_1_WIDTH                   31
#define D0F0xBC_xC0200200_Reserved_31_1_MASK                    0xfffffffe

/// D0F0xBC_xC020008C
typedef union {
  struct {                                                              ///<
    UINT32                                               Reserved_0:1 ; ///<
    UINT32                                           Reserved_31_1:31 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0200200_STRUCT;

// **** D0F0xBC_xC0200310 Register Definition ****
// Address
#define D0F0xBC_xC0200310_ADDRESS                               0xc0200310

// Type
#define D0F0xBC_xC0200310_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC0200310_RFE_BUSY_MASK_OFFSET                  0
#define D0F0xBC_xC0200310_RFE_BUSY_MASK_WIDTH                   1
#define D0F0xBC_xC0200310_RFE_BUSY_MASK_MASK                    0x1
#define D0F0xBC_xC0200310_BIF_CG_LCLK_BUSY_MASK_OFFSET          1
#define D0F0xBC_xC0200310_BIF_CG_LCLK_BUSY_MASK_WIDTH           1
#define D0F0xBC_xC0200310_BIF_CG_LCLK_BUSY_MASK_MASK            0x2
#define D0F0xBC_xC0200310_L1IMU_SMU_IDLE_MASK_OFFSET            2
#define D0F0xBC_xC0200310_L1IMU_SMU_IDLE_MASK_WIDTH             1
#define D0F0xBC_xC0200310_L1IMU_SMU_IDLE_MASK_MASK              0x4
#define D0F0xBC_xC0200310_RESERVED_BIT3_OFFSET                  3
#define D0F0xBC_xC0200310_RESERVED_BIT3_WIDTH                   1
#define D0F0xBC_xC0200310_RESERVED_BIT3_MASK                    0x8
#define D0F0xBC_xC0200310_SCLK_RUNNING_MASK_OFFSET              4
#define D0F0xBC_xC0200310_SCLK_RUNNING_MASK_WIDTH               1
#define D0F0xBC_xC0200310_SCLK_RUNNING_MASK_MASK                0x10
#define D0F0xBC_xC0200310_SMU_BUSY_MASK_OFFSET                  5
#define D0F0xBC_xC0200310_SMU_BUSY_MASK_WIDTH                   1
#define D0F0xBC_xC0200310_SMU_BUSY_MASK_MASK                    0x20
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE1_MASK_OFFSET           6
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE1_MASK_WIDTH            1
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE1_MASK_MASK             0x40
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE2_MASK_OFFSET           7
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE2_MASK_WIDTH            1
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE2_MASK_MASK             0x80
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE3_MASK_OFFSET           8
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE3_MASK_WIDTH            1
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE3_MASK_MASK             0x100
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE4_MASK_OFFSET           9
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE4_MASK_WIDTH            1
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE4_MASK_MASK             0x200
#define D0F0xBC_xC0200310_L1IMUGPP_IDLE_MASK_OFFSET             10
#define D0F0xBC_xC0200310_L1IMUGPP_IDLE_MASK_WIDTH              1
#define D0F0xBC_xC0200310_L1IMUGPP_IDLE_MASK_MASK               0x400
#define D0F0xBC_xC0200310_L1IMUGPPSB_IDLE_MASK_OFFSET           11
#define D0F0xBC_xC0200310_L1IMUGPPSB_IDLE_MASK_WIDTH            1
#define D0F0xBC_xC0200310_L1IMUGPPSB_IDLE_MASK_MASK             0x800
#define D0F0xBC_xC0200310_L1IMUBIF_IDLE_MASK_OFFSET             12
#define D0F0xBC_xC0200310_L1IMUBIF_IDLE_MASK_WIDTH              1
#define D0F0xBC_xC0200310_L1IMUBIF_IDLE_MASK_MASK               0x1000
#define D0F0xBC_xC0200310_L1IMUINTGEN_IDLE_MASK_OFFSET          13
#define D0F0xBC_xC0200310_L1IMUINTGEN_IDLE_MASK_WIDTH           1
#define D0F0xBC_xC0200310_L1IMUINTGEN_IDLE_MASK_MASK            0x2000
#define D0F0xBC_xC0200310_L2IMU_IDLE_MASK_OFFSET                14
#define D0F0xBC_xC0200310_L2IMU_IDLE_MASK_WIDTH                 1
#define D0F0xBC_xC0200310_L2IMU_IDLE_MASK_MASK                  0x4000
#define D0F0xBC_xC0200310_ORB_IDLE_MASK_OFFSET                  15
#define D0F0xBC_xC0200310_ORB_IDLE_MASK_WIDTH                   1
#define D0F0xBC_xC0200310_ORB_IDLE_MASK_MASK                    0x8000
#define D0F0xBC_xC0200310_ON_INB_WAKE_MASK_OFFSET               16
#define D0F0xBC_xC0200310_ON_INB_WAKE_MASK_WIDTH                1
#define D0F0xBC_xC0200310_ON_INB_WAKE_MASK_MASK                 0x10000
#define D0F0xBC_xC0200310_ON_INB_WAKE_ACK_MASK_OFFSET           17
#define D0F0xBC_xC0200310_ON_INB_WAKE_ACK_MASK_WIDTH            1
#define D0F0xBC_xC0200310_ON_INB_WAKE_ACK_MASK_MASK             0x20000
#define D0F0xBC_xC0200310_ON_OUTB_WAKE_MASK_OFFSET              18
#define D0F0xBC_xC0200310_ON_OUTB_WAKE_MASK_WIDTH               1
#define D0F0xBC_xC0200310_ON_OUTB_WAKE_MASK_MASK                0x40000
#define D0F0xBC_xC0200310_ON_OUTB_WAKE_ACK_MASK_OFFSET          19
#define D0F0xBC_xC0200310_ON_OUTB_WAKE_ACK_MASK_WIDTH           1
#define D0F0xBC_xC0200310_ON_OUTB_WAKE_ACK_MASK_MASK            0x80000
#define D0F0xBC_xC0200310_DMAACTIVE_MASK_OFFSET                 20
#define D0F0xBC_xC0200310_DMAACTIVE_MASK_WIDTH                  1
#define D0F0xBC_xC0200310_DMAACTIVE_MASK_MASK                   0x100000
#define D0F0xBC_xC0200310_RLC_SMU_GFXCLK_OFF_MASK_OFFSET        21
#define D0F0xBC_xC0200310_RLC_SMU_GFXCLK_OFF_MASK_WIDTH         1
#define D0F0xBC_xC0200310_RLC_SMU_GFXCLK_OFF_MASK_MASK          0x200000
#define D0F0xBC_xC0200310_RESERVED_OFFSET                       22
#define D0F0xBC_xC0200310_RESERVED_WIDTH                        10
#define D0F0xBC_xC0200310_RESERVED_MASK                         0xffc00000

/// D0F0xBC_xC0200310
typedef union {
  struct {                                                              ///<
    UINT32                                            RFE_BUSY_MASK:1 ; ///<
    UINT32                                    BIF_CG_LCLK_BUSY_MASK:1 ; ///<
    UINT32                                      L1IMU_SMU_IDLE_MASK:1 ; ///<
    UINT32                                            RESERVED_BIT3:1 ; ///<
    UINT32                                        SCLK_RUNNING_MASK:1 ; ///<
    UINT32                                            SMU_BUSY_MASK:1 ; ///<
    UINT32                                     PCIE_LCLK_IDLE1_MASK:1 ; ///<
    UINT32                                     PCIE_LCLK_IDLE2_MASK:1 ; ///<
    UINT32                                     PCIE_LCLK_IDLE3_MASK:1 ; ///<
    UINT32                                     PCIE_LCLK_IDLE4_MASK:1 ; ///<
    UINT32                                       L1IMUGPP_IDLE_MASK:1 ; ///<
    UINT32                                     L1IMUGPPSB_IDLE_MASK:1 ; ///<
    UINT32                                       L1IMUBIF_IDLE_MASK:1 ; ///<
    UINT32                                    L1IMUINTGEN_IDLE_MASK:1 ; ///<
    UINT32                                          L2IMU_IDLE_MASK:1 ; ///<
    UINT32                                            ORB_IDLE_MASK:1 ; ///<
    UINT32                                         ON_INB_WAKE_MASK:1 ; ///<
    UINT32                                     ON_INB_WAKE_ACK_MASK:1 ; ///<
    UINT32                                        ON_OUTB_WAKE_MASK:1 ; ///<
    UINT32                                    ON_OUTB_WAKE_ACK_MASK:1 ; ///<
    UINT32                                           DMAACTIVE_MASK:1 ; ///<
    UINT32                                  RLC_SMU_GFXCLK_OFF_MASK:1 ; ///<
    UINT32                                                 RESERVED:10; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0200310_STRUCT;

// **** D0F0xD4_x01301486 Register Definition ****
// Address
#define D0F0xD4_x01301486_ADDRESS                               0x1301486
// Type
#define D0F0xD4_x01301486_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x01301486_Reserved_1_0_OFFSET                   0
#define D0F0xD4_x01301486_Reserved_1_0_WIDTH                    2
#define D0F0xD4_x01301486_Reserved_1_0_MASK                     0x3
#define D0F0xD4_x01301486_LinkTrainEn_OFFSET                    2
#define D0F0xD4_x01301486_LinkTrainEn_WIDTH                     1
#define D0F0xD4_x01301486_LinkTrainEn_MASK                      0x4
#define D0F0xD4_x01301486_Reserved_3_3_OFFSET                   3
#define D0F0xD4_x01301486_Reserved_3_3_WIDTH                    1
#define D0F0xD4_x01301486_Reserved_3_3_MASK                     0x8
#define D0F0xD4_x01301486_Reserved_7_4_OFFSET                   4
#define D0F0xD4_x01301486_Reserved_7_4_WIDTH                    4
#define D0F0xD4_x01301486_Reserved_7_4_MASK                     0xF0
#define D0F0xD4_x01301486_Reserved_8_8_OFFSET                   8
#define D0F0xD4_x01301486_Reserved_8_8_WIDTH                    1
#define D0F0xD4_x01301486_Reserved_8_8_MASK                     0x100
#define D0F0xD4_x01301486_Reserved_31_9_OFFSET                  9
#define D0F0xD4_x01301486_Reserved_31_9_WIDTH                   23
#define D0F0xD4_x01301486_Reserved_31_9_MASK                    0xFFFFFE00

/// D0F0xD4_x01301486
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_1_0:2; ///<
    UINT32                                             LinkTrainEn:1; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                            Reserved_7_4:4; ///<
    UINT32                                            Reserved_8_8:1; ///<
    UINT32                                           Reserved_31_9:23; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x01301486_STRUCT;

// **** D0F0xD4_x013014AC Register Definition ****
// Address
#define D0F0xD4_x013014AC_ADDRESS                               0x13014AC
// Type
#define D0F0xD4_x013014AC_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x013014AC_Reserved_15_0_OFFSET                  0
#define D0F0xD4_x013014AC_Reserved_15_0_WIDTH                   16
#define D0F0xD4_x013014AC_Reserved_15_0_MASK                    0xFFFF
#define D0F0xD4_x013014AC_StrapBifAudioEnPin_OFFSET             16
#define D0F0xD4_x013014AC_StrapBifAudioEnPin_WIDTH              1
#define D0F0xD4_x013014AC_StrapBifAudioEnPin_MASK               0x10000
#define D0F0xD4_x013014AC_Reserved_17_17_OFFSET                 17
#define D0F0xD4_x013014AC_Reserved_17_17_WIDTH                  1
#define D0F0xD4_x013014AC_Reserved_17_17_MASK                   0x20000
#define D0F0xD4_x013014AC_Reserved_31_18_OFFSET                 18
#define D0F0xD4_x013014AC_Reserved_31_18_WIDTH                  14
#define D0F0xD4_x013014AC_Reserved_31_18_MASK                   0xFFFC0000

/// D0F0xD4_x013014AC
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_15_0:16; ///<
    UINT32                                      StrapBifAudioEnPin:1; ///<
    UINT32                                          Reserved_17_17:1; ///<
    UINT32                                          Reserved_31_18:14; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x013014AC_STRUCT;

// **** D0F0xD4_x013014B6 Register Definition ****
// Address
#define D0F0xD4_x013014B6_ADDRESS                               0x13014B6
// Type
#define D0F0xD4_x013014B6_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x013014B6_Reserved_7_0_OFFSET                   0
#define D0F0xD4_x013014B6_Reserved_7_0_WIDTH                    8
#define D0F0xD4_x013014B6_Reserved_7_0_MASK                     0xFF
#define D0F0xD4_x013014B6_StrapBifF0LegacyDeviceTypeDis_OFFSET  8
#define D0F0xD4_x013014B6_StrapBifF0LegacyDeviceTypeDis_WIDTH   1
#define D0F0xD4_x013014B6_StrapBifF0LegacyDeviceTypeDis_MASK    0x100
#define D0F0xD4_x013014B6_Reserved_9_9_OFFSET                   9
#define D0F0xD4_x013014B6_Reserved_9_9_WIDTH                    1
#define D0F0xD4_x013014B6_Reserved_9_9_MASK                     0x200
#define D0F0xD4_x013014B6_StrapBifAzLegacyDeviceTypeDis_OFFSET  10
#define D0F0xD4_x013014B6_StrapBifAzLegacyDeviceTypeDis_WIDTH   1
#define D0F0xD4_x013014B6_StrapBifAzLegacyDeviceTypeDis_MASK    0x400
#define D0F0xD4_x013014B6_Reserved_12_11_OFFSET                 11
#define D0F0xD4_x013014B6_Reserved_12_11_WIDTH                  2
#define D0F0xD4_x013014B6_Reserved_12_11_MASK                   0x1800
#define D0F0xD4_x013014B6_StrapBifAudioEn_OFFSET                13
#define D0F0xD4_x013014B6_StrapBifAudioEn_WIDTH                 1
#define D0F0xD4_x013014B6_StrapBifAudioEn_MASK                  0x2000
#define D0F0xD4_x013014B6_Reserved_31_14_OFFSET                 14
#define D0F0xD4_x013014B6_Reserved_31_14_WIDTH                  18
#define D0F0xD4_x013014B6_Reserved_31_14_MASK                   0xFFFFC000

/// D0F0xD4_x013014B6
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_7_0:8; ///<
    UINT32                           StrapBifF0LegacyDeviceTypeDis:1; ///<
    UINT32                                            Reserved_9_9:1; ///<
    UINT32                           StrapBifAzLegacyDeviceTypeDis:1; ///<
    UINT32                                          Reserved_12_11:2; ///<
    UINT32                                         StrapBifAudioEn:1; ///<
    UINT32                                          Reserved_31_14:18; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x013014B6_STRUCT;

// **** D0F0xD4_x013014B8 Register Definition ****
// Address
#define D0F0xD4_x013014B8_ADDRESS                               0x13014B8
// Type
#define D0F0xD4_x013014B8_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x013014B8_Reserved_13_0_OFFSET                           0
#define D0F0xD4_x013014B8_Reserved_13_0_WIDTH                            14
#define D0F0xD4_x013014B8_Reserved_13_0_MASK                             0x03FFF
#define D0F0xD4_x013014B8_STRAP_BIF_E2E_PREFIX_EN_A_OFFSET               14
#define D0F0xD4_x013014B8_STRAP_BIF_E2E_PREFIX_EN_A_WIDTH                1
#define D0F0xD4_x013014B8_STRAP_BIF_E2E_PREFIX_EN_A_MASK                 0x04000
#define D0F0xD4_x013014B8_STRAP_BIF_EXTENDED_FMT_SUPPORTED_A_OFFSET      15
#define D0F0xD4_x013014B8_STRAP_BIF_EXTENDED_FMT_SUPPORTED_A_WIDTH       1
#define D0F0xD4_x013014B8_STRAP_BIF_EXTENDED_FMT_SUPPORTED_A_MASK        0x08000
#define D0F0xD4_x013014B8_STRAP_BIF_PASID_PREFIX_SUPPORTED_OFFSET        16
#define D0F0xD4_x013014B8_STRAP_BIF_PASID_PREFIX_SUPPORTED_WIDTH         1
#define D0F0xD4_x013014B8_STRAP_BIF_PASID_PREFIX_SUPPORTED_MASK          0x010000
#define D0F0xD4_x013014B8_STRAP_BIF_MAX_PASID_WIDTH_OFFSET               17
#define D0F0xD4_x013014B8_STRAP_BIF_MAX_PASID_WIDTH_WIDTH                5
#define D0F0xD4_x013014B8_STRAP_BIF_MAX_PASID_WIDTH_MASK                 0x03E0000
#define D0F0xD4_x013014B8_STRAP_BIF_F0_ATS_EN_OFFSET                     22
#define D0F0xD4_x013014B8_STRAP_BIF_F0_ATS_EN_WIDTH                      1
#define D0F0xD4_x013014B8_STRAP_BIF_F0_ATS_EN_MASK                       0x0400000
#define D0F0xD4_x013014B8_Reserved_31_23_OFFSET                          23
#define D0F0xD4_x013014B8_Reserved_31_23_WIDTH                           9
#define D0F0xD4_x013014B8_Reserved_31_23_MASK                            0xFF800000

/// D0F0xD4_x013014B8
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_13_0:14; ///<
    UINT32                               STRAP_BIF_E2E_PREFIX_EN_A:1; ///<
    UINT32                      STRAP_BIF_EXTENDED_FMT_SUPPORTED_A:1; ///<
    UINT32                        STRAP_BIF_PASID_PREFIX_SUPPORTED:1; ///<
    UINT32                               STRAP_BIF_MAX_PASID_WIDTH:5; ///<
    UINT32                                     STRAP_BIF_F0_ATS_EN:1; ///<
    UINT32                                          Reserved_31_23:9; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x013014B8_STRUCT;

// **** D0F0xD4_x013014BA Register Definition ****
// Address
#define D0F0xD4_x013014BA_ADDRESS                               0x13014BA
// Type
#define D0F0xD4_x013014BA_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x013014BA_Reserved_25_0_OFFSET                          0
#define D0F0xD4_x013014BA_Reserved_25_0_WIDTH                           26
#define D0F0xD4_x013014BA_Reserved_25_0_MASK                            0x03FFFFFF
#define D0F0xD4_x013014BA_STRAP_BIF_F0_PAGE_REQ_EN_OFFSET               26
#define D0F0xD4_x013014BA_STRAP_BIF_F0_PAGE_REQ_EN_WIDTH                1
#define D0F0xD4_x013014BA_STRAP_BIF_F0_PAGE_REQ_EN_MASK                 0x04000000
#define D0F0xD4_x013014BA_STRAP_BIF_F0_PASID_EN_OFFSET                  27
#define D0F0xD4_x013014BA_STRAP_BIF_F0_PASID_EN_WIDTH                   1
#define D0F0xD4_x013014BA_STRAP_BIF_F0_PASID_EN_MASK                    0x08000000
#define D0F0xD4_x013014BA_STRAP_BIF_PASID_EXE_PERMISSION_SUP_OFFSET     28
#define D0F0xD4_x013014BA_STRAP_BIF_PASID_EXE_PERMISSION_SUP_WIDTH      1
#define D0F0xD4_x013014BA_STRAP_BIF_PASID_EXE_PERMISSION_SUP_MASK       0x010000000
#define D0F0xD4_x013014BA_STRAP_BIF_PASID_PRIV_MODE_SUP_OFFSET          29
#define D0F0xD4_x013014BA_STRAP_BIF_PASID_PRIV_MODE_SUP_WIDTH           1
#define D0F0xD4_x013014BA_STRAP_BIF_PASID_PRIV_MODE_SUP_MASK            0x020000000
#define D0F0xD4_x013014BA_STRAP_BIF_PASID_GLOBAL_INVALID_SUP_OFFSET     30
#define D0F0xD4_x013014BA_STRAP_BIF_PASID_GLOBAL_INVALID_SUP_WIDTH      1
#define D0F0xD4_x013014BA_STRAP_BIF_PASID_GLOBAL_INVALID_SUP_MASK       0x020000000
#define D0F0xD4_x013014BA_Reserved_31_31_OFFSET                         31
#define D0F0xD4_x013014BA_Reserved_31_31_WIDTH                          1
#define D0F0xD4_x013014BA_Reserved_31_31_MASK                           0x80000000

/// D0F0xD4_x013014BA
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_25_0:26; ///<
    UINT32                                STRAP_BIF_F0_PAGE_REQ_EN:1; ///<
    UINT32                                   STRAP_BIF_F0_PASID_EN:1; ///<
    UINT32                      STRAP_BIF_PASID_EXE_PERMISSION_SUP:1; ///<
    UINT32                           STRAP_BIF_PASID_PRIV_MODE_SUP:1; ///<
    UINT32                      STRAP_BIF_PASID_GLOBAL_INVALID_SUP:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x013014BA_STRUCT;

// **** D0F0xD4_x013014BE Register Definition ****
// Address
#define D0F0xD4_x013014BE_ADDRESS                                     0x013014BE

// Type
#define D0F0xD4_x013014BE_TYPE                                        TYPE_D0F0xD4
// Field Data
#define D0F0xD4_x013014BE_Reserved_19_0_OFFSET                        0
#define D0F0xD4_x013014BE_Reserved_19_0_WIDTH                         20
#define D0F0xD4_x013014BE_Reserved_19_0_MASK                          0xfffff
#define D0F0xD4_x013014BE_StrapBifSymalignHwDebug_OFFSET              20
#define D0F0xD4_x013014BE_StrapBifSymalignHwDebug_WIDTH               1
#define D0F0xD4_x013014BE_StrapBifSymalignHwDebug_MASK                0x100000
#define D0F0xD4_x013014BE_Reserved_31_21_OFFSET                       21
#define D0F0xD4_x013014BE_Reserved_31_21_WIDTH                        11
#define D0F0xD4_x013014BE_Reserved_31_21_MASK                         0xffe00000

/// D0F0xD4_x013014BE
typedef union {
  struct {                                                              ///<
    UINT32                                            Reserved_19_0:20; ///<
    UINT32                                  StrapBifSymalignHwDebug:1 ; ///<
    UINT32                                           Reserved_31_21:11; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xD4_x013014BE_STRUCT;


// **** D0F0xD4_x010914E1 Register Definition ****
// Address
#define D0F0xD4_x010914E1_ADDRESS                               0x10914E1
// Type
#define D0F0xD4_x010914E1_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x010914E1_Reserved_0_0_OFFSET                   0
#define D0F0xD4_x010914E1_Reserved_0_0_WIDTH                    1
#define D0F0xD4_x010914E1_Reserved_0_0_MASK                     0x1
#define D0F0xD4_x010914E1_StrapBifRegApSize_OFFSET              1
#define D0F0xD4_x010914E1_StrapBifRegApSize_WIDTH               2
#define D0F0xD4_x010914E1_StrapBifRegApSize_MASK                0x6
#define D0F0xD4_x010914E1_StrapBifMemApSize_OFFSET              3
#define D0F0xD4_x010914E1_StrapBifMemApSize_WIDTH               3
#define D0F0xD4_x010914E1_StrapBifMemApSize_MASK                0x38
#define D0F0xD4_x010914E1_Reserved_11_6_OFFSET                  6
#define D0F0xD4_x010914E1_Reserved_11_6_WIDTH                   6
#define D0F0xD4_x010914E1_Reserved_11_6_MASK                    0xFC0
#define D0F0xD4_x010914E1_StrapBifDoorbellBarDis_OFFSET         12
#define D0F0xD4_x010914E1_StrapBifDoorbellBarDis_WIDTH          1
#define D0F0xD4_x010914E1_StrapBifDoorbellBarDis_MASK           0x1000
#define D0F0xD4_x010914E1_Bitfield_13_13_OFFSET                 13
#define D0F0xD4_x010914E1_Bitfield_13_13_WIDTH                  1
#define D0F0xD4_x010914E1_Bitfield_13_13_MASK                   0x2000
#define D0F0xD4_x010914E1_Bitfield_15_14_OFFSET                 14
#define D0F0xD4_x010914E1_Bitfield_15_14_WIDTH                  2
#define D0F0xD4_x010914E1_Bitfield_15_14_MASK                   0xC000
#define D0F0xD4_x010914E1_Reserved_31_13_OFFSET                 16
#define D0F0xD4_x010914E1_Reserved_31_13_WIDTH                  16
#define D0F0xD4_x010914E1_Reserved_31_13_MASK                   0xFFFF0000

/// D0F0xD4_x010914E1
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                       StrapBifRegApSize:2; ///<
    UINT32                                       StrapBifMemApSize:3; ///<
    UINT32                                           Reserved_11_6:6; ///<
    UINT32                                  StrapBifDoorbellBarDis:1; ///<
    UINT32                                          Bitfield_13_13:1; ///<
    UINT32                                          Bitfield_15_14:2; ///<
    UINT32                                          Reserved_31_13:16; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x010914E1_STRUCT;

// **** D0F0xD4_x010914E2 Register Definition ****
// Address
#define D0F0xD4_x010914E2_ADDRESS                               0x10914E2
// Type
#define D0F0xD4_x010914E2_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x010914E2_Reserved_0_0_OFFSET                   0
#define D0F0xD4_x010914E2_Reserved_0_0_WIDTH                    1
#define D0F0xD4_x010914E2_Reserved_0_0_MASK                     0x1
#define D0F0xD4_x010914E2_StrapBifIoBarDis_OFFSET               1
#define D0F0xD4_x010914E2_StrapBifIoBarDis_WIDTH                1
#define D0F0xD4_x010914E2_StrapBifIoBarDis_MASK                 0x2
#define D0F0xD4_x010914E2_Reserved_2_2_OFFSET                   2
#define D0F0xD4_x010914E2_Reserved_2_2_WIDTH                    1
#define D0F0xD4_x010914E2_Reserved_2_2_MASK                     0x4
#define D0F0xD4_x010914E2_StrapBifF064BarDisA_OFFSET            3
#define D0F0xD4_x010914E2_StrapBifF064BarDisA_WIDTH             1
#define D0F0xD4_x010914E2_StrapBifF064BarDisA_MASK              0x8
#define D0F0xD4_x010914E2_Reserved_7_4_OFFSET                   4
#define D0F0xD4_x010914E2_Reserved_7_4_WIDTH                    4
#define D0F0xD4_x010914E2_Reserved_7_4_MASK                     0xF0
#define D0F0xD4_x010914E2_StrapBifF0LegacyDeviceTypeDis_OFFSET  8
#define D0F0xD4_x010914E2_StrapBifF0LegacyDeviceTypeDis_WIDTH   1
#define D0F0xD4_x010914E2_StrapBifF0LegacyDeviceTypeDis_MASK    0x100
#define D0F0xD4_x010914E2_Reserved_9_9_OFFSET                   9
#define D0F0xD4_x010914E2_Reserved_9_9_WIDTH                    1
#define D0F0xD4_x010914E2_Reserved_9_9_MASK                     0x200
#define D0F0xD4_x010914E2_StrapBifAzLegacyDeviceTypeDis_OFFSET  10
#define D0F0xD4_x010914E2_StrapBifAzLegacyDeviceTypeDis_WIDTH   1
#define D0F0xD4_x010914E2_StrapBifAzLegacyDeviceTypeDis_MASK    0x400
#define D0F0xD4_x010914E2_Reserved_12_11_OFFSET                 11
#define D0F0xD4_x010914E2_Reserved_12_11_WIDTH                  2
#define D0F0xD4_x010914E2_Reserved_12_11_MASK                   0x1800
#define D0F0xD4_x010914E2_StrapBifAudioEn_OFFSET                13
#define D0F0xD4_x010914E2_StrapBifAudioEn_WIDTH                 1
#define D0F0xD4_x010914E2_StrapBifAudioEn_MASK                  0x2000
#define D0F0xD4_x010914E2_Reserved_31_14_OFFSET                 14
#define D0F0xD4_x010914E2_Reserved_31_14_WIDTH                  18
#define D0F0xD4_x010914E2_Reserved_31_14_MASK                   0xFFFFC000

/// D0F0xD4_x010914E2
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                        StrapBifIoBarDis:1; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                     StrapBifF064BarDisA:1; ///<
    UINT32                                            Reserved_7_4:4; ///<
    UINT32                           StrapBifF0LegacyDeviceTypeDis:1; ///<
    UINT32                                            Reserved_9_9:1; ///<
    UINT32                           StrapBifAzLegacyDeviceTypeDis:1; ///<
    UINT32                                          Reserved_12_11:2; ///<
    UINT32                                         StrapBifAudioEn:1; ///<
    UINT32                                          Reserved_31_14:18; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x010914E2_STRUCT;

// **** D0F0xD4_x01091507 Register Definition ****
// Address
#define D0F0xD4_x01091507_ADDRESS                               0x1091507
// Type
#define D0F0xD4_x01091507_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x01091507_Reserved_4_0_OFFSET                   0
#define D0F0xD4_x01091507_Reserved_4_0_WIDTH                    5
#define D0F0xD4_x01091507_Reserved_4_0_MASK                     0x1F
#define D0F0xD4_x01091507_StrapBifMemApSizePin_OFFSET           5
#define D0F0xD4_x01091507_StrapBifMemApSizePin_WIDTH            3
#define D0F0xD4_x01091507_StrapBifMemApSizePin_MASK             0xE0
#define D0F0xD4_x01091507_Reserved_15_8_OFFSET                  8
#define D0F0xD4_x01091507_Reserved_15_8_WIDTH                   8
#define D0F0xD4_x01091507_Reserved_15_8_MASK                    0xFF00
#define D0F0xD4_x01091507_StrapBifAudioEnPin_OFFSET             16
#define D0F0xD4_x01091507_StrapBifAudioEnPin_WIDTH              1
#define D0F0xD4_x01091507_StrapBifAudioEnPin_MASK               0x10000
#define D0F0xD4_x01091507_Reserved_31_17_OFFSET                 17
#define D0F0xD4_x01091507_Reserved_31_17_WIDTH                  15
#define D0F0xD4_x01091507_Reserved_31_17_MASK                   0xFFFE0000

/// D0F0xD4_x01091507
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_4_0:5; ///<
    UINT32                                    StrapBifMemApSizePin:3; ///<
    UINT32                                           Reserved_15_8:8; ///<
    UINT32                                      StrapBifAudioEnPin:1; ///<
    UINT32                                          Reserved_31_17:15; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x01091507_STRUCT;

// **** D0F0xD4_x01091518 Register Definition ****
// Address
#define D0F0xD4_x01091518_ADDRESS                               0x1091518
// Type
#define D0F0xD4_x01091518_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x01091518_LinkTrainEn_OFFSET                    0
#define D0F0xD4_x01091518_LinkTrainEn_WIDTH                     1
#define D0F0xD4_x01091518_LinkTrainEn_MASK                      0x1
#define D0F0xD4_x01091518_Reserved_31_1_OFFSET                  1
#define D0F0xD4_x01091518_Reserved_31_1_WIDTH                   31
#define D0F0xD4_x01091518_Reserved_31_1_MASK                    0xFFFFFFFE

/// D0F0xD4_x01091518
typedef union {
  struct {                                                            ///<
    UINT32                                             LinkTrainEn:1; ///<
    UINT32                                           Reserved_31_1:31; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x01091518_STRUCT;

// **** D0F0xBC_xC2100000 Register Definition ****
// Address
#define D0F0xBC_xC2100000_ADDRESS                               0xC2100000
// Type
#define D0F0xBC_xC2100000_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC2100000_IntToggle_OFFSET                      0
#define D0F0xBC_xC2100000_IntToggle_WIDTH                       1
#define D0F0xBC_xC2100000_IntToggle_MASK                        0x1
#define D0F0xBC_xC2100000_ServiceIndex_OFFSET                   1
#define D0F0xBC_xC2100000_ServiceIndex_WIDTH                    16
#define D0F0xBC_xC2100000_ServiceIndex_MASK                     0x1FFFE
#define D0F0xBC_xC2100000_Reserved_31_17_OFFSET                 17
#define D0F0xBC_xC2100000_Reserved_31_17_WIDTH                  15
#define D0F0xBC_xC2100000_Reserved_31_17_MASK                   0xFFFE0000

/// D0F0xBC_xC2100000
typedef union {
  struct {                                                            ///<
    UINT32                                               IntToggle:1; ///<
    UINT32                                            ServiceIndex:16; ///<
    UINT32                                          Reserved_31_17:15; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xC2100000_STRUCT;

// **** D0F0xBC_xC2100004 Register Definition ****
// Address
#define D0F0xBC_xC2100004_ADDRESS                               0xC2100004
// Type
#define D0F0xBC_xC2100004_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC2100004_IntAck_OFFSET                         0
#define D0F0xBC_xC2100004_IntAck_WIDTH                          1
#define D0F0xBC_xC2100004_IntAck_MASK                           0x1
#define D0F0xBC_xC2100004_IntDone_OFFSET                        1
#define D0F0xBC_xC2100004_IntDone_WIDTH                         1
#define D0F0xBC_xC2100004_IntDone_MASK                          0x2
#define D0F0xBC_xC2100004_Reserved_31_2_OFFSET                  2
#define D0F0xBC_xC2100004_Reserved_31_2_WIDTH                   30
#define D0F0xBC_xC2100004_Reserved_31_2_MASK                    0xFFFFFFFC

/// D0F0xBC_xC2100004
typedef union {
  struct {                                                            ///<
    UINT32                                                  IntAck:1; ///<
    UINT32                                                 IntDone:1; ///<
    UINT32                                           Reserved_31_2:30; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xC2100004_STRUCT;

// **** D0F0xBC_xE0003088 Register Definition ****
// Address
#define D0F0xBC_xE0003088_ADDRESS                               0xE0003088
// Type
#define D0F0xBC_xE0003088_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xE0003088_SMU_AUTH_DONE_OFFSET                  0
#define D0F0xBC_xE0003088_SMU_AUTH_DONE_WIDTH                   1
#define D0F0xBC_xE0003088_SMU_AUTH_DONE_MASK                    0x1
#define D0F0xBC_xE0003088_SMU_AUTH_PASS_OFFSET                  1
#define D0F0xBC_xE0003088_SMU_AUTH_PASS_WIDTH                   1
#define D0F0xBC_xE0003088_SMU_AUTH_PASS_MASK                    0x2
#define D0F0xBC_xE0003088_Reserved_31_2_OFFSET                  2
#define D0F0xBC_xE0003088_Reserved_31_2_WIDTH                   30
#define D0F0xBC_xE0003088_Reserved_31_2_MASK                    0xFFFFFFFC

/// D0F0xBC_xE0003088
typedef union {
  struct {                                                            ///<
    UINT32                                           SMU_AUTH_DONE:1; ///<
    UINT32                                           SMU_AUTH_PASS:1; ///<
    UINT32                                           Reserved_31_2:30; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xE0003088_STRUCT;

// **** D0F0xBC_xE00030A4 Register Definition ****
// Address
#define D0F0xBC_xE00030A4_ADDRESS                               0xE00030A4
// Type
#define D0F0xBC_xE00030A4_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xE00030A4_SMU_AUTH_IN_PROG_OFFSET               0
#define D0F0xBC_xE00030A4_SMU_AUTH_IN_PROG_WIDTH                1
#define D0F0xBC_xE00030A4_SMU_AUTH_IN_PROG_MASK                 0x1
#define D0F0xBC_xE00030A4_SMU_KEY_RD_DONE_OFFSET                1
#define D0F0xBC_xE00030A4_SMU_KEY_RD_DONE_WIDTH                 2
#define D0F0xBC_xE00030A4_SMU_KEY_RD_DONE_MASK                  0x6
#define D0F0xBC_xE00030A4_SMU_SRAM_RD_BLOCK_EN_OFFSET           3
#define D0F0xBC_xE00030A4_SMU_SRAM_RD_BLOCK_EN_WIDTH            1
#define D0F0xBC_xE00030A4_SMU_SRAM_RD_BLOCK_EN_MASK             0x8
#define D0F0xBC_xE00030A4_SMU_SRAM_WR_BLOCK_EN_OFFSET           4
#define D0F0xBC_xE00030A4_SMU_SRAM_WR_BLOCK_EN_WIDTH            1
#define D0F0xBC_xE00030A4_SMU_SRAM_WR_BLOCK_EN_MASK             0x10
#define D0F0xBC_xE00030A4_Reserved_7_5_OFFSET                   5
#define D0F0xBC_xE00030A4_Reserved_7_5_WIDTH                    3
#define D0F0xBC_xE00030A4_Reserved_7_5_MASK                     0xE0
#define D0F0xBC_xE00030A4_SMU_AUTH_counter_OFFSET               8
#define D0F0xBC_xE00030A4_SMU_AUTH_counter_WIDTH                4
#define D0F0xBC_xE00030A4_SMU_AUTH_counter_MASK                 0xF00
#define D0F0xBC_xE00030A4_Reserved_15_12_OFFSET                 12
#define D0F0xBC_xE00030A4_Reserved_15_12_WIDTH                  4
#define D0F0xBC_xE00030A4_Reserved_15_12_MASK                   0xF000
#define D0F0xBC_xE00030A4_SMU_PROTECTED_MODE_OFFSET             16
#define D0F0xBC_xE00030A4_SMU_PROTECTED_MODE_WIDTH              1
#define D0F0xBC_xE00030A4_SMU_PROTECTED_MODE_MASK               0x10000
#define D0F0xBC_xE00030A4_SMU_KEY_SEL_OFFSET                    17
#define D0F0xBC_xE00030A4_SMU_KEY_SEL_WIDTH                     1
#define D0F0xBC_xE00030A4_SMU_KEY_SEL_MASK                      0x20000
#define D0F0xBC_xE00030A4_Reserved_31_18_OFFSET                 18
#define D0F0xBC_xE00030A4_Reserved_31_18_WIDTH                  14
#define D0F0xBC_xE00030A4_Reserved_31_18_MASK                   0xFFFC0000

/// D0F0xBC_xE00030A4
typedef union {
  struct {                                                            ///<
    UINT32                                        SMU_AUTH_IN_PROG:1; ///<
    UINT32                                         SMU_KEY_RD_DONE:2; ///<
    UINT32                                    SMU_SRAM_RD_BLOCK_EN:1; ///<
    UINT32                                    SMU_SRAM_WR_BLOCK_EN:1; ///<
    UINT32                                            Reserved_7_5:3; ///<
    UINT32                                        SMU_AUTH_counter:4; ///<
    UINT32                                          Reserved_15_12:4; ///<
    UINT32                                      SMU_PROTECTED_MODE:1; ///<
    UINT32                                             SMU_KEY_SEL:1; ///<
    UINT32                                          Reserved_31_18:14; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xE00030A4_STRUCT;

// **** D0F0xBC_xC0000004 Register Definition ****
// Address
#define D0F0xBC_xC0000004_ADDRESS                               0xC0000004
// Type
#define D0F0xBC_xC0000004_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC0000004_Reserved_6_0_OFFSET                   0
#define D0F0xBC_xC0000004_Reserved_6_0_WIDTH                    7
#define D0F0xBC_xC0000004_Reserved_6_0_MASK                     0x7F
#define D0F0xBC_xC0000004_BootSeqDone_OFFSET                    7
#define D0F0xBC_xC0000004_BootSeqDone_WIDTH                     1
#define D0F0xBC_xC0000004_BootSeqDone_MASK                      0x80
#define D0F0xBC_xC0000004_Reserved_25_8_OFFSET                  8
#define D0F0xBC_xC0000004_Reserved_25_8_WIDTH                   18
#define D0F0xBC_xC0000004_Reserved_25_8_MASK                    0x3FFFF00
#define D0F0xBC_xC0000004_Reserved_31_26_OFFSET                 26
#define D0F0xBC_xC0000004_Reserved_31_26_WIDTH                  6
#define D0F0xBC_xC0000004_Reserved_31_26_MASK                   0xFC000000

/// D0F0xBC_xC0000004
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_6_0:7; ///<
    UINT32                                             BootSeqDone:1; ///<
    UINT32                                           Reserved_25_8:18; ///<
    UINT32                                          Reserved_31_26:6; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xC0000004_STRUCT;

// **** D0F0xBC_xC0000024 Register Definition ****
// Address
#define D0F0xBC_xC0000024_ADDRESS                               0xC0000024
// Type
#define D0F0xBC_xC0000024_TYPE                                     TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC0000024_InterruptsEnabled_OFFSET                 0
#define D0F0xBC_xC0000024_InterruptsEnabled_WIDTH                  1
#define D0F0xBC_xC0000024_InterruptsEnabled_MASK                   0x1
#define D0F0xBC_xC0000024_Reserved_23_1_OFFSET                     1
#define D0F0xBC_xC0000024_Reserved_23_1_WIDTH                      23
#define D0F0xBC_xC0000024_Reserved_23_1_MASK                       0xFFFFFE
#define D0F0xBC_xC0000024_TestCount_OFFSET                         24
#define D0F0xBC_xC0000024_TestCount_WIDTH                          8
#define D0F0xBC_xC0000024_TestCount_MASK                           0xFF000000

/// D0F0xBC_xC0000024
typedef union {
  struct {                                                            ///<
    UINT32                                       InterruptsEnabled:1; ///<
    UINT32                                           Reserved_23_1:23; ///<
    UINT32                                               TestCount:8; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xC0000024_STRUCT;

// **** D0F0xFC_x00 Register Definition ****
// Address
#define D0F0xFC_x00_ADDRESS                                     0x0
// Type
#define D0F0xFC_x00_TYPE                                        TYPE_D0F0xFC

// Field Data
#define D0F0xFC_x00_IoapicEnable_OFFSET                         0
#define D0F0xFC_x00_IoapicEnable_WIDTH                          1
#define D0F0xFC_x00_IoapicEnable_MASK                           0x1
#define D0F0xFC_x00_Reserved_1_1_OFFSET                         1
#define D0F0xFC_x00_Reserved_1_1_WIDTH                          1
#define D0F0xFC_x00_Reserved_1_1_MASK                           0x2
#define D0F0xFC_x00_IoapicIdExtEn_OFFSET                        2
#define D0F0xFC_x00_IoapicIdExtEn_WIDTH                         1
#define D0F0xFC_x00_IoapicIdExtEn_MASK                          0x4
#define D0F0xFC_x00_Reserved_3_3_OFFSET                         3
#define D0F0xFC_x00_Reserved_3_3_WIDTH                          1
#define D0F0xFC_x00_Reserved_3_3_MASK                           0x8
#define D0F0xFC_x00_IoapicSbFeatureEn_OFFSET                    4
#define D0F0xFC_x00_IoapicSbFeatureEn_WIDTH                     1
#define D0F0xFC_x00_IoapicSbFeatureEn_MASK                      0x10
#define D0F0xFC_x00_Reserved_31_5_OFFSET                        5
#define D0F0xFC_x00_Reserved_31_5_WIDTH                         27
#define D0F0xFC_x00_Reserved_31_5_MASK                          0xFFFFFFE0

/// D0F0xFC_x00
typedef union {
  struct {                                                            ///<
    UINT32                                            IoapicEnable:1; ///<
    UINT32                                            Reserved_1_1:1; ///<
    UINT32                                           IoapicIdExtEn:1; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                       IoapicSbFeatureEn:1; ///<
    UINT32                                           Reserved_31_5:27; ///<

  } Field;

  UINT32 Value;
} D0F0xFC_x00_STRUCT;

#define D0F2xFC_x07_L1_ADDRESS(Sel)                             ((Sel << 16) | 0x7)
// Type
#define D0F2xFC_x07_L1_TYPE                                     TYPE_D0F2xFC

// Field Data
#define D0F2xFC_x07_L1_PhantomFuncEn_OFFSET                     0
#define D0F2xFC_x07_L1_PhantomFuncEn_WIDTH                      1
#define D0F2xFC_x07_L1_PhantomFuncEn_MASK                       0x1
#define D0F2xFC_x07_L1_Reserved_10_1_OFFSET                     1
#define D0F2xFC_x07_L1_Reserved_10_1_WIDTH                      10
#define D0F2xFC_x07_L1_Reserved_10_1_MASK                       0x7FE
#define D0F2xFC_x07_L1_SpecReqFilterEn_OFFSET                   11
#define D0F2xFC_x07_L1_SpecReqFilterEn_WIDTH                    1
#define D0F2xFC_x07_L1_SpecReqFilterEn_MASK                     0x800
#define D0F2xFC_x07_L1_AtsSeqNumEn_OFFSET                       12
#define D0F2xFC_x07_L1_AtsSeqNumEn_WIDTH                        1
#define D0F2xFC_x07_L1_AtsSeqNumEn_MASK                         0x1000
#define D0F2xFC_x07_L1_Reserved_13_13_OFFSET                    13
#define D0F2xFC_x07_L1_Reserved_13_13_WIDTH                     1
#define D0F2xFC_x07_L1_Reserved_13_13_MASK                      0x2000
#define D0F2xFC_x07_L1_AtsPhysPageOverlapDis_OFFSET             14
#define D0F2xFC_x07_L1_AtsPhysPageOverlapDis_WIDTH              1
#define D0F2xFC_x07_L1_AtsPhysPageOverlapDis_MASK               0x4000
#define D0F2xFC_x07_L1_Reserved_16_15_OFFSET                    15
#define D0F2xFC_x07_L1_Reserved_16_15_WIDTH                     2
#define D0F2xFC_x07_L1_Reserved_16_15_MASK                      0x18000
#define D0F2xFC_x07_L1_L1NwEn_OFFSET                            17
#define D0F2xFC_x07_L1_L1NwEn_WIDTH                             1
#define D0F2xFC_x07_L1_L1NwEn_MASK                              0x20000
#define D0F2xFC_x07_L1_Reserved_31_18_OFFSET                    18
#define D0F2xFC_x07_L1_Reserved_31_18_WIDTH                     14
#define D0F2xFC_x07_L1_Reserved_31_18_MASK                      0xFFFC0000

/// D0F2xFC_x07_L1
typedef union {
  struct {                                                            ///<
    UINT32                                           PhantomFuncEn:1; ///<
    UINT32                                           Reserved_10_1:10; ///<
    UINT32                                         SpecReqFilterEn:1; ///<
    UINT32                                             AtsSeqNumEn:1; ///<
    UINT32                                          Reserved_13_13:1; ///<
    UINT32                                   AtsPhysPageOverlapDis:1; ///<
    UINT32                                          Reserved_16_15:2; ///<
    UINT32                                                  L1NwEn:1; ///<
    UINT32                                          Reserved_31_18:14; ///<

  } Field;

  UINT32 Value;
} D0F2xFC_x07_L1_STRUCT;

// **** D0F2xFC_x0C_L1 Register Definition ****
// Address
#define D0F2xFC_x0C_L1_ADDRESS(Sel)                             ((Sel << 16) | 0xC)
// Type
#define D0F2xFC_x0C_L1_TYPE                                     TYPE_D0F2xFC

// Field Data
#define D0F2xFC_x0C_L1_UnfilterDis_OFFSET                       0
#define D0F2xFC_x0C_L1_UnfilterDis_WIDTH                        1
#define D0F2xFC_x0C_L1_UnfilterDis_MASK                         0x1
#define D0F2xFC_x0C_L1_FragmentDis_OFFSET                       1
#define D0F2xFC_x0C_L1_FragmentDis_WIDTH                        1
#define D0F2xFC_x0C_L1_FragmentDis_MASK                         0x2
#define D0F2xFC_x0C_L1_CacheirOnly_OFFSET                       2
#define D0F2xFC_x0C_L1_CacheirOnly_WIDTH                        1
#define D0F2xFC_x0C_L1_CacheirOnly_MASK                         0x4
#define D0F2xFC_x0C_L1_CacheiwOnly_OFFSET                       3
#define D0F2xFC_x0C_L1_CacheiwOnly_WIDTH                        1
#define D0F2xFC_x0C_L1_CacheiwOnly_MASK                         0x8
#define D0F2xFC_x0C_L1_Reserved_4_4_OFFSET                      4
#define D0F2xFC_x0C_L1_Reserved_4_4_WIDTH                       1
#define D0F2xFC_x0C_L1_Reserved_4_4_MASK                        0x10
#define D0F2xFC_x0C_L1_ReplacementSel_OFFSET                    5
#define D0F2xFC_x0C_L1_ReplacementSel_WIDTH                     1
#define D0F2xFC_x0C_L1_ReplacementSel_MASK                      0x20
#define D0F2xFC_x0C_L1_Reserved_7_6_OFFSET                      6
#define D0F2xFC_x0C_L1_Reserved_7_6_WIDTH                       2
#define D0F2xFC_x0C_L1_Reserved_7_6_MASK                        0xC0
#define D0F2xFC_x0C_L1_L2Credits_OFFSET                         8
#define D0F2xFC_x0C_L1_L2Credits_WIDTH                          6
#define D0F2xFC_x0C_L1_L2Credits_MASK                           0x3F00
#define D0F2xFC_x0C_L1_Reserved_19_14_OFFSET                    14
#define D0F2xFC_x0C_L1_Reserved_19_14_WIDTH                     6
#define D0F2xFC_x0C_L1_Reserved_19_14_MASK                      0xFC000
#define D0F2xFC_x0C_L1_L1Banks_OFFSET                           20
#define D0F2xFC_x0C_L1_L1Banks_WIDTH                            2
#define D0F2xFC_x0C_L1_L1Banks_MASK                             0x300000
#define D0F2xFC_x0C_L1_Reserved_23_22_OFFSET                    22
#define D0F2xFC_x0C_L1_Reserved_23_22_WIDTH                     2
#define D0F2xFC_x0C_L1_Reserved_23_22_MASK                      0xC00000
#define D0F2xFC_x0C_L1_L1Entries_OFFSET                         24
#define D0F2xFC_x0C_L1_L1Entries_WIDTH                          4
#define D0F2xFC_x0C_L1_L1Entries_MASK                           0xF000000
#define D0F2xFC_x0C_L1_L1VirtOrderQueues_OFFSET                 28
#define D0F2xFC_x0C_L1_L1VirtOrderQueues_WIDTH                  3
#define D0F2xFC_x0C_L1_L1VirtOrderQueues_MASK                   0x70000000
#define D0F2xFC_x0C_L1_Reserved_31_31_OFFSET                    31
#define D0F2xFC_x0C_L1_Reserved_31_31_WIDTH                     1
#define D0F2xFC_x0C_L1_Reserved_31_31_MASK                      0x80000000

/// D0F2xFC_x0C_L1
typedef union {
  struct {                                                            ///<
    UINT32                                             UnfilterDis:1; ///<
    UINT32                                             FragmentDis:1; ///<
    UINT32                                             CacheirOnly:1; ///<
    UINT32                                             CacheiwOnly:1; ///<
    UINT32                                            Reserved_4_4:1; ///<
    UINT32                                          ReplacementSel:1; ///<
    UINT32                                            Reserved_7_6:2; ///<
    UINT32                                               L2Credits:6; ///<
    UINT32                                          Reserved_19_14:6; ///<
    UINT32                                                 L1Banks:2; ///<
    UINT32                                          Reserved_23_22:2; ///<
    UINT32                                               L1Entries:4; ///<
    UINT32                                       L1VirtOrderQueues:3; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F2xFC_x0C_L1_STRUCT;

// **** D0F2xFC_x0D_L1 Register Definition ****
// Address
#define D0F2xFC_x0D_L1_ADDRESS(Sel)                             ((Sel << 16) | 0xD)
// Type
#define D0F2xFC_x0D_L1_TYPE                                     TYPE_D0F2xFC

// Field Data
#define D0F2xFC_x0D_L1_VOQPortBits_OFFSET                       0
#define D0F2xFC_x0D_L1_VOQPortBits_WIDTH                        3
#define D0F2xFC_x0D_L1_VOQPortBits_MASK                         0x7
#define D0F2xFC_x0D_L1_Reserved_3_3_OFFSET                      3
#define D0F2xFC_x0D_L1_Reserved_3_3_WIDTH                       1
#define D0F2xFC_x0D_L1_Reserved_3_3_MASK                        0x8
#define D0F2xFC_x0D_L1_VOQFuncBits_OFFSET                       4
#define D0F2xFC_x0D_L1_VOQFuncBits_WIDTH                        3
#define D0F2xFC_x0D_L1_VOQFuncBits_MASK                         0x70
#define D0F2xFC_x0D_L1_Reserved_7_7_OFFSET                      7
#define D0F2xFC_x0D_L1_Reserved_7_7_WIDTH                       1
#define D0F2xFC_x0D_L1_Reserved_7_7_MASK                        0x80
#define D0F2xFC_x0D_L1_VOQXorMode_OFFSET                        8
#define D0F2xFC_x0D_L1_VOQXorMode_WIDTH                         1
#define D0F2xFC_x0D_L1_VOQXorMode_MASK                          0x100
#define D0F2xFC_x0D_L1_CacheByPass_OFFSET                       9
#define D0F2xFC_x0D_L1_CacheByPass_WIDTH                        1
#define D0F2xFC_x0D_L1_CacheByPass_MASK                         0x200
#define D0F2xFC_x0D_L1_L1CacheParityEn_OFFSET                   10
#define D0F2xFC_x0D_L1_L1CacheParityEn_WIDTH                    1
#define D0F2xFC_x0D_L1_L1CacheParityEn_MASK                     0x400
#define D0F2xFC_x0D_L1_L1ParityEn_OFFSET                        11
#define D0F2xFC_x0D_L1_L1ParityEn_WIDTH                         1
#define D0F2xFC_x0D_L1_L1ParityEn_MASK                          0x800
#define D0F2xFC_x0D_L1_L1DTEDis_OFFSET                          12
#define D0F2xFC_x0D_L1_L1DTEDis_WIDTH                           1
#define D0F2xFC_x0D_L1_L1DTEDis_MASK                            0x1000
#define D0F2xFC_x0D_L1_BlockL1Dis_OFFSET                        13
#define D0F2xFC_x0D_L1_BlockL1Dis_WIDTH                         1
#define D0F2xFC_x0D_L1_BlockL1Dis_MASK                          0x2000
#define D0F2xFC_x0D_L1_WqEntrydis_OFFSET                        14
#define D0F2xFC_x0D_L1_WqEntrydis_WIDTH                         5
#define D0F2xFC_x0D_L1_WqEntrydis_MASK                          0x7C000
#define D0F2xFC_x0D_L1_AtsNobufferInsert_OFFSET                 19
#define D0F2xFC_x0D_L1_AtsNobufferInsert_WIDTH                  1
#define D0F2xFC_x0D_L1_AtsNobufferInsert_MASK                   0x80000
#define D0F2xFC_x0D_L1_SndFilterDis_OFFSET                      20
#define D0F2xFC_x0D_L1_SndFilterDis_WIDTH                       1
#define D0F2xFC_x0D_L1_SndFilterDis_MASK                        0x100000
#define D0F2xFC_x0D_L1_L1orderEn_OFFSET                         21
#define D0F2xFC_x0D_L1_L1orderEn_WIDTH                          1
#define D0F2xFC_x0D_L1_L1orderEn_MASK                           0x200000
#define D0F2xFC_x0D_L1_L1CacheInvAllEn_OFFSET                   22
#define D0F2xFC_x0D_L1_L1CacheInvAllEn_WIDTH                    1
#define D0F2xFC_x0D_L1_L1CacheInvAllEn_MASK                     0x400000
#define D0F2xFC_x0D_L1_SelectTimeoutPulse_OFFSET                23
#define D0F2xFC_x0D_L1_SelectTimeoutPulse_WIDTH                 3
#define D0F2xFC_x0D_L1_SelectTimeoutPulse_MASK                  0x3800000
#define D0F2xFC_x0D_L1_L1CacheSelReqid_OFFSET                   26
#define D0F2xFC_x0D_L1_L1CacheSelReqid_WIDTH                    1
#define D0F2xFC_x0D_L1_L1CacheSelReqid_MASK                     0x4000000
#define D0F2xFC_x0D_L1_L1CacheSelInterleave_OFFSET              27
#define D0F2xFC_x0D_L1_L1CacheSelInterleave_WIDTH               1
#define D0F2xFC_x0D_L1_L1CacheSelInterleave_MASK                0x8000000
#define D0F2xFC_x0D_L1_PretransNovaFilteren_OFFSET              28
#define D0F2xFC_x0D_L1_PretransNovaFilteren_WIDTH               1
#define D0F2xFC_x0D_L1_PretransNovaFilteren_MASK                0x10000000
#define D0F2xFC_x0D_L1_Untrans2mFilteren_OFFSET                 29
#define D0F2xFC_x0D_L1_Untrans2mFilteren_WIDTH                  1
#define D0F2xFC_x0D_L1_Untrans2mFilteren_MASK                   0x20000000
#define D0F2xFC_x0D_L1_L1DebugCntrMode_OFFSET                   30
#define D0F2xFC_x0D_L1_L1DebugCntrMode_WIDTH                    1
#define D0F2xFC_x0D_L1_L1DebugCntrMode_MASK                     0x40000000
#define D0F2xFC_x0D_L1_Reserved_31_31_OFFSET                    31
#define D0F2xFC_x0D_L1_Reserved_31_31_WIDTH                     1
#define D0F2xFC_x0D_L1_Reserved_31_31_MASK                      0x80000000

/// D0F2xFC_x0D_L1
typedef union {
  struct {                                                            ///<
    UINT32                                             VOQPortBits:3; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                             VOQFuncBits:3; ///<
    UINT32                                            Reserved_7_7:1; ///<
    UINT32                                              VOQXorMode:1; ///<
    UINT32                                             CacheByPass:1; ///<
    UINT32                                         L1CacheParityEn:1; ///<
    UINT32                                              L1ParityEn:1; ///<
    UINT32                                                L1DTEDis:1; ///<
    UINT32                                              BlockL1Dis:1; ///<
    UINT32                                              WqEntrydis:5; ///<
    UINT32                                       AtsNobufferInsert:1; ///<
    UINT32                                            SndFilterDis:1; ///<
    UINT32                                               L1orderEn:1; ///<
    UINT32                                         L1CacheInvAllEn:1; ///<
    UINT32                                      SelectTimeoutPulse:3; ///<
    UINT32                                         L1CacheSelReqid:1; ///<
    UINT32                                    L1CacheSelInterleave:1; ///<
    UINT32                                    PretransNovaFilteren:1; ///<
    UINT32                                       Untrans2mFilteren:1; ///<
    UINT32                                         L1DebugCntrMode:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F2xFC_x0D_L1_STRUCT;

// **** D0F0xFC_x0F Register Definition ****
// Address
#define D0F0xFC_x0F_ADDRESS                                     0x0F
// Type
#define D0F0xFC_x0F_TYPE                                        TYPE_D0F0xFC

// Field Data
#define D0F0xFC_x0F_GBIFExtIntrGrp_OFFSET                         0
#define D0F0xFC_x0F_GBIFExtIntrGrp_WIDTH                          3
#define D0F0xFC_x0F_GBIFExtIntrGrp_MASK                           0x7
#define D0F0xFC_x0F_Reserved_3_3_OFFSET                           3
#define D0F0xFC_x0F_Reserved_3_3_WIDTH                            1
#define D0F0xFC_x0F_Reserved_3_3_MASK                             0x8
#define D0F0xFC_x0F_GBIFExtIntrSwz_OFFSET                         4
#define D0F0xFC_x0F_GBIFExtIntrSwz_WIDTH                          2
#define D0F0xFC_x0F_GBIFExtIntrSwz_MASK                           0x30
#define D0F0xFC_x0F_Reserved_31_6_OFFSET                          6
#define D0F0xFC_x0F_Reserved_31_6_WIDTH                           26
#define D0F0xFC_x0F_Reserved_31_6_MASK                            0xFFFFFFC0

/// D0F0xFC_xOF
typedef union {
  struct {                                                            ///<
    UINT32                                            GBIFExtIntrGrp:3; ///<
    UINT32                                              Reserved_3_3:1; ///<
    UINT32                                            GBIFExtIntrSwz:2; ///<
    UINT32                                             Reserved_31_6:26; ///<
  } Field;

  UINT32 Value;
} D0F0xFC_x0F_STRUCT;

// **** D0F0xFC_x10 Register Definition ****
// Address
#define D0F0xFC_x10_ADDRESS                                     0x10
// Type
#define D0F0xFC_x10_TYPE                                        TYPE_D0F0xFC

// Field Data
#define D0F0xFC_x10_BrExtIntrGrp_OFFSET                         0
#define D0F0xFC_x10_BrExtIntrGrp_WIDTH                          3
#define D0F0xFC_x10_BrExtIntrGrp_MASK                           0x7
#define D0F0xFC_x10_Reserved_3_3_OFFSET                         3
#define D0F0xFC_x10_Reserved_3_3_WIDTH                          1
#define D0F0xFC_x10_Reserved_3_3_MASK                           0x8
#define D0F0xFC_x10_BrExtIntrSwz_OFFSET                         4
#define D0F0xFC_x10_BrExtIntrSwz_WIDTH                          2
#define D0F0xFC_x10_BrExtIntrSwz_MASK                           0x30
#define D0F0xFC_x10_Reserved_15_6_OFFSET                        6
#define D0F0xFC_x10_Reserved_15_6_WIDTH                         10
#define D0F0xFC_x10_Reserved_15_6_MASK                          0xFFC0
#define D0F0xFC_x10_BrIntIntrMap_OFFSET                         16
#define D0F0xFC_x10_BrIntIntrMap_WIDTH                          5
#define D0F0xFC_x10_BrIntIntrMap_MASK                           0x1F0000
#define D0F0xFC_x10_Reserved_31_21_OFFSET                       21
#define D0F0xFC_x10_Reserved_31_21_WIDTH                        11
#define D0F0xFC_x10_Reserved_31_21_MASK                         0xFFE00000

/// D0F0xFC_x10
typedef union {
  struct {                                                            ///<
    UINT32                                            BrExtIntrGrp:3; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                            BrExtIntrSwz:2; ///<
    UINT32                                           Reserved_15_6:10; ///<
    UINT32                                            BrIntIntrMap:5; ///<
    UINT32                                          Reserved_31_21:11; ///<

  } Field;

  UINT32 Value;
} D0F0xFC_x10_STRUCT;

// **** D0F2xFC_x32_L1 Register Definition ****
// Address
#define D0F2xFC_x32_L1_ADDRESS(Sel)                             ((Sel << 16) | 0x32)
// Type
#define D0F2xFC_x32_L1_TYPE                                     TYPE_D0F2xFC

// Field Data
#define D0F2xFC_x32_L1_AtsMultipleRespEn_OFFSET                 0
#define D0F2xFC_x32_L1_AtsMultipleRespEn_WIDTH                  1
#define D0F2xFC_x32_L1_AtsMultipleRespEn_MASK                   0x1
#define D0F2xFC_x32_L1_AtsMultipleL1toL2En_OFFSET               1
#define D0F2xFC_x32_L1_AtsMultipleL1toL2En_WIDTH                1
#define D0F2xFC_x32_L1_AtsMultipleL1toL2En_MASK                 0x2
#define D0F2xFC_x32_L1_TimeoutPulseExtEn_OFFSET                 2
#define D0F2xFC_x32_L1_TimeoutPulseExtEn_WIDTH                  1
#define D0F2xFC_x32_L1_TimeoutPulseExtEn_MASK                   0x4
#define D0F2xFC_x32_L1_TlpprefixerrEn_OFFSET                    3
#define D0F2xFC_x32_L1_TlpprefixerrEn_WIDTH                     1
#define D0F2xFC_x32_L1_TlpprefixerrEn_MASK                      0x8
#define D0F2xFC_x32_L1_DmaBufCredits_OFFSET                     4
#define D0F2xFC_x32_L1_DmaBufCredits_WIDTH                      6
#define D0F2xFC_x32_L1_DmaBufCredits_MASK                       0x3F0
#define D0F2xFC_x32_L1_DmaBufMaxNPCred_OFFSET                   10
#define D0F2xFC_x32_L1_DmaBufMaxNPCred_WIDTH                    6
#define D0F2xFC_x32_L1_DmaBufMaxNPCred_MASK                     0xFC00
#define D0F2xFC_x32_L1_DmaNpHaltDis_OFFSET                      16
#define D0F2xFC_x32_L1_DmaNpHaltDis_WIDTH                       1
#define D0F2xFC_x32_L1_DmaNpHaltDis_MASK                        0x10000
#define D0F2xFC_x32_L1_Reserved_31_17_OFFSET                    17
#define D0F2xFC_x32_L1_Reserved_31_17_WIDTH                     15
#define D0F2xFC_x32_L1_Reserved_31_17_MASK                      0xFFFE0000

/// D0F2xFC_x32_L1
typedef union {
  struct {                                                            ///<
    UINT32                                       AtsMultipleRespEn:1; ///<
    UINT32                                     AtsMultipleL1toL2En:1; ///<
    UINT32                                       TimeoutPulseExtEn:1; ///<
    UINT32                                          TlpprefixerrEn:1; ///<
    UINT32                                           DmaBufCredits:6; ///<
    UINT32                                         DmaBufMaxNPCred:6; ///<
    UINT32                                            DmaNpHaltDis:1; ///<
    UINT32                                          Reserved_31_17:15; ///<

  } Field;

  UINT32 Value;
} D0F2xFC_x32_L1_STRUCT;

// **** D0F2xFC_x33_L1 Register Definition ****
// Address
#define D0F2xFC_x33_L1_ADDRESS(Sel)                             ((Sel << 16) | 0x33)
// Type
#define D0F2xFC_x33_L1_TYPE                                     TYPE_D0F2xFC

// Field Data
#define D0F2xFC_x33_L1_L1ClkgateLen_OFFSET                      0
#define D0F2xFC_x33_L1_L1ClkgateLen_WIDTH                       2
#define D0F2xFC_x33_L1_L1ClkgateLen_MASK                        0x3
#define D0F2xFC_x33_L1_Reserved_3_2_OFFSET                      2
#define D0F2xFC_x33_L1_Reserved_3_2_WIDTH                       2
#define D0F2xFC_x33_L1_Reserved_3_2_MASK                        0xC
#define D0F2xFC_x33_L1_L1DmaClkgateEn_OFFSET                    4
#define D0F2xFC_x33_L1_L1DmaClkgateEn_WIDTH                     1
#define D0F2xFC_x33_L1_L1DmaClkgateEn_MASK                      0x10
#define D0F2xFC_x33_L1_L1CacheClkgateEn_OFFSET                  5
#define D0F2xFC_x33_L1_L1CacheClkgateEn_WIDTH                   1
#define D0F2xFC_x33_L1_L1CacheClkgateEn_MASK                    0x20
#define D0F2xFC_x33_L1_L1CpslvClkgateEn_OFFSET                  6
#define D0F2xFC_x33_L1_L1CpslvClkgateEn_WIDTH                   1
#define D0F2xFC_x33_L1_L1CpslvClkgateEn_MASK                    0x40
#define D0F2xFC_x33_L1_L1DmaInputClkgateEn_OFFSET               7
#define D0F2xFC_x33_L1_L1DmaInputClkgateEn_WIDTH                1
#define D0F2xFC_x33_L1_L1DmaInputClkgateEn_MASK                 0x80
#define D0F2xFC_x33_L1_L1PerfClkgateEn_OFFSET                   8
#define D0F2xFC_x33_L1_L1PerfClkgateEn_WIDTH                    1
#define D0F2xFC_x33_L1_L1PerfClkgateEn_MASK                     0x100
#define D0F2xFC_x33_L1_L1MemoryClkgateEn_OFFSET                 9
#define D0F2xFC_x33_L1_L1MemoryClkgateEn_WIDTH                  1
#define D0F2xFC_x33_L1_L1MemoryClkgateEn_MASK                   0x200
#define D0F2xFC_x33_L1_L1RegClkgateEn_OFFSET                    10
#define D0F2xFC_x33_L1_L1RegClkgateEn_WIDTH                     1
#define D0F2xFC_x33_L1_L1RegClkgateEn_MASK                      0x400
#define D0F2xFC_x33_L1_L1HostreqClkgateEn_OFFSET                11
#define D0F2xFC_x33_L1_L1HostreqClkgateEn_WIDTH                 1
#define D0F2xFC_x33_L1_L1HostreqClkgateEn_MASK                  0x800
#define D0F2xFC_x33_L1_Reserved_30_12_OFFSET                    12
#define D0F2xFC_x33_L1_Reserved_30_12_WIDTH                     19
#define D0F2xFC_x33_L1_Reserved_30_12_MASK                      0x7FFFF000
#define D0F2xFC_x33_L1_L1L2ClkgateEn_OFFSET                     31
#define D0F2xFC_x33_L1_L1L2ClkgateEn_WIDTH                      1
#define D0F2xFC_x33_L1_L1L2ClkgateEn_MASK                       0x80000000

/// D0F2xFC_x33_L1
typedef union {
  struct {                                                            ///<
    UINT32                                            L1ClkgateLen:2; ///<
    UINT32                                            Reserved_3_2:2; ///<
    UINT32                                          L1DmaClkgateEn:1; ///<
    UINT32                                        L1CacheClkgateEn:1; ///<
    UINT32                                        L1CpslvClkgateEn:1; ///<
    UINT32                                     L1DmaInputClkgateEn:1; ///<
    UINT32                                         L1PerfClkgateEn:1; ///<
    UINT32                                       L1MemoryClkgateEn:1; ///<
    UINT32                                          L1RegClkgateEn:1; ///<
    UINT32                                      L1HostreqClkgateEn:1; ///<
    UINT32                                          Reserved_30_12:19; ///<
    UINT32                                           L1L2ClkgateEn:1; ///<

  } Field;

  UINT32 Value;
} D0F2xFC_x33_L1_STRUCT;

// **** D0F2xF4_x10 Register Definition ****
// Address
#define D0F2xF4_x10_ADDRESS                                     0x10
// Type
#define D0F2xF4_x10_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x10_DTCReplacementSel_OFFSET                    0
#define D0F2xF4_x10_DTCReplacementSel_WIDTH                     2
#define D0F2xF4_x10_DTCReplacementSel_MASK                      0x3
#define D0F2xF4_x10_Reserved_2_2_OFFSET                         2
#define D0F2xF4_x10_Reserved_2_2_WIDTH                          1
#define D0F2xF4_x10_Reserved_2_2_MASK                           0x4
#define D0F2xF4_x10_DTCLRUUpdatePri_OFFSET                      3
#define D0F2xF4_x10_DTCLRUUpdatePri_WIDTH                       1
#define D0F2xF4_x10_DTCLRUUpdatePri_MASK                        0x8
#define D0F2xF4_x10_DTCParityEn_OFFSET                          4
#define D0F2xF4_x10_DTCParityEn_WIDTH                           1
#define D0F2xF4_x10_DTCParityEn_MASK                            0x10
#define D0F2xF4_x10_Reserved_7_5_OFFSET                         5
#define D0F2xF4_x10_Reserved_7_5_WIDTH                          3
#define D0F2xF4_x10_Reserved_7_5_MASK                           0xE0
#define D0F2xF4_x10_DTCInvalidationSel_OFFSET                   8
#define D0F2xF4_x10_DTCInvalidationSel_WIDTH                    2
#define D0F2xF4_x10_DTCInvalidationSel_MASK                     0x300
#define D0F2xF4_x10_DTCSoftInvalidate_OFFSET                    10
#define D0F2xF4_x10_DTCSoftInvalidate_WIDTH                     1
#define D0F2xF4_x10_DTCSoftInvalidate_MASK                      0x400
#define D0F2xF4_x10_Reserved_12_11_OFFSET                       11
#define D0F2xF4_x10_Reserved_12_11_WIDTH                        2
#define D0F2xF4_x10_Reserved_12_11_MASK                         0x1800
#define D0F2xF4_x10_DTCBypass_OFFSET                            13
#define D0F2xF4_x10_DTCBypass_WIDTH                             1
#define D0F2xF4_x10_DTCBypass_MASK                              0x2000
#define D0F2xF4_x10_Reserved_14_14_OFFSET                       14
#define D0F2xF4_x10_Reserved_14_14_WIDTH                        1
#define D0F2xF4_x10_Reserved_14_14_MASK                         0x4000
#define D0F2xF4_x10_DTCParitySupport_OFFSET                     15
#define D0F2xF4_x10_DTCParitySupport_WIDTH                      1
#define D0F2xF4_x10_DTCParitySupport_MASK                       0x8000
#define D0F2xF4_x10_DTCWays_OFFSET                              16
#define D0F2xF4_x10_DTCWays_WIDTH                               8
#define D0F2xF4_x10_DTCWays_MASK                                0xFF0000
#define D0F2xF4_x10_Reserved_27_24_OFFSET                       24
#define D0F2xF4_x10_Reserved_27_24_WIDTH                        4
#define D0F2xF4_x10_Reserved_27_24_MASK                         0xF000000
#define D0F2xF4_x10_DTCEntries_OFFSET                           28
#define D0F2xF4_x10_DTCEntries_WIDTH                            4
#define D0F2xF4_x10_DTCEntries_MASK                             0xF0000000

/// D0F2xF4_x10
typedef union {
  struct {                                                            ///<
    UINT32                                       DTCReplacementSel:2; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                         DTCLRUUpdatePri:1; ///<
    UINT32                                             DTCParityEn:1; ///<
    UINT32                                            Reserved_7_5:3; ///<
    UINT32                                      DTCInvalidationSel:2; ///<
    UINT32                                       DTCSoftInvalidate:1; ///<
    UINT32                                          Reserved_12_11:2; ///<
    UINT32                                               DTCBypass:1; ///<
    UINT32                                          Reserved_14_14:1; ///<
    UINT32                                        DTCParitySupport:1; ///<
    UINT32                                                 DTCWays:8; ///<
    UINT32                                          Reserved_27_24:4; ///<
    UINT32                                              DTCEntries:4; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x10_STRUCT;

// **** D0F2xF4_x14 Register Definition ****
// Address
#define D0F2xF4_x14_ADDRESS                                     0x14
// Type
#define D0F2xF4_x14_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x14_ITCReplacementSel_OFFSET                    0
#define D0F2xF4_x14_ITCReplacementSel_WIDTH                     2
#define D0F2xF4_x14_ITCReplacementSel_MASK                      0x3
#define D0F2xF4_x14_Reserved_2_2_OFFSET                         2
#define D0F2xF4_x14_Reserved_2_2_WIDTH                          1
#define D0F2xF4_x14_Reserved_2_2_MASK                           0x4
#define D0F2xF4_x14_ITCLRUUpdatePri_OFFSET                      3
#define D0F2xF4_x14_ITCLRUUpdatePri_WIDTH                       1
#define D0F2xF4_x14_ITCLRUUpdatePri_MASK                        0x8
#define D0F2xF4_x14_ITCParityEn_OFFSET                          4
#define D0F2xF4_x14_ITCParityEn_WIDTH                           1
#define D0F2xF4_x14_ITCParityEn_MASK                            0x10
#define D0F2xF4_x14_Reserved_7_5_OFFSET                         5
#define D0F2xF4_x14_Reserved_7_5_WIDTH                          3
#define D0F2xF4_x14_Reserved_7_5_MASK                           0xE0
#define D0F2xF4_x14_ITCInvalidationSel_OFFSET                   8
#define D0F2xF4_x14_ITCInvalidationSel_WIDTH                    2
#define D0F2xF4_x14_ITCInvalidationSel_MASK                     0x300
#define D0F2xF4_x14_ITCSoftInvalidate_OFFSET                    10
#define D0F2xF4_x14_ITCSoftInvalidate_WIDTH                     1
#define D0F2xF4_x14_ITCSoftInvalidate_MASK                      0x400
#define D0F2xF4_x14_Reserved_12_11_OFFSET                       11
#define D0F2xF4_x14_Reserved_12_11_WIDTH                        2
#define D0F2xF4_x14_Reserved_12_11_MASK                         0x1800
#define D0F2xF4_x14_ITCBypass_OFFSET                            13
#define D0F2xF4_x14_ITCBypass_WIDTH                             1
#define D0F2xF4_x14_ITCBypass_MASK                              0x2000
#define D0F2xF4_x14_Reserved_14_14_OFFSET                       14
#define D0F2xF4_x14_Reserved_14_14_WIDTH                        1
#define D0F2xF4_x14_Reserved_14_14_MASK                         0x4000
#define D0F2xF4_x14_ITCParitySupport_OFFSET                     15
#define D0F2xF4_x14_ITCParitySupport_WIDTH                      1
#define D0F2xF4_x14_ITCParitySupport_MASK                       0x8000
#define D0F2xF4_x14_ITCWays_OFFSET                              16
#define D0F2xF4_x14_ITCWays_WIDTH                               8
#define D0F2xF4_x14_ITCWays_MASK                                0xFF0000
#define D0F2xF4_x14_Reserved_27_24_OFFSET                       24
#define D0F2xF4_x14_Reserved_27_24_WIDTH                        4
#define D0F2xF4_x14_Reserved_27_24_MASK                         0xF000000
#define D0F2xF4_x14_ITCEntries_OFFSET                           28
#define D0F2xF4_x14_ITCEntries_WIDTH                            4
#define D0F2xF4_x14_ITCEntries_MASK                             0xF0000000

/// D0F2xF4_x14
typedef union {
  struct {                                                            ///<
    UINT32                                       ITCReplacementSel:2; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                         ITCLRUUpdatePri:1; ///<
    UINT32                                             ITCParityEn:1; ///<
    UINT32                                            Reserved_7_5:3; ///<
    UINT32                                      ITCInvalidationSel:2; ///<
    UINT32                                       ITCSoftInvalidate:1; ///<
    UINT32                                          Reserved_12_11:2; ///<
    UINT32                                               ITCBypass:1; ///<
    UINT32                                          Reserved_14_14:1; ///<
    UINT32                                        ITCParitySupport:1; ///<
    UINT32                                                 ITCWays:8; ///<
    UINT32                                          Reserved_27_24:4; ///<
    UINT32                                              ITCEntries:4; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x14_STRUCT;

// **** D0F2xF4_x18 Register Definition ****
// Address
#define D0F2xF4_x18_ADDRESS                                     0x18
// Type
#define D0F2xF4_x18_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x18_PTCAReplacementSel_OFFSET                   0
#define D0F2xF4_x18_PTCAReplacementSel_WIDTH                    2
#define D0F2xF4_x18_PTCAReplacementSel_MASK                     0x3
#define D0F2xF4_x18_Reserved_2_2_OFFSET                         2
#define D0F2xF4_x18_Reserved_2_2_WIDTH                          1
#define D0F2xF4_x18_Reserved_2_2_MASK                           0x4
#define D0F2xF4_x18_PTCALRUUpdatePri_OFFSET                     3
#define D0F2xF4_x18_PTCALRUUpdatePri_WIDTH                      1
#define D0F2xF4_x18_PTCALRUUpdatePri_MASK                       0x8
#define D0F2xF4_x18_PTCAParityEn_OFFSET                         4
#define D0F2xF4_x18_PTCAParityEn_WIDTH                          1
#define D0F2xF4_x18_PTCAParityEn_MASK                           0x10
#define D0F2xF4_x18_Reserved_7_5_OFFSET                         5
#define D0F2xF4_x18_Reserved_7_5_WIDTH                          3
#define D0F2xF4_x18_Reserved_7_5_MASK                           0xE0
#define D0F2xF4_x18_PTCAInvalidationSel_OFFSET                  8
#define D0F2xF4_x18_PTCAInvalidationSel_WIDTH                   2
#define D0F2xF4_x18_PTCAInvalidationSel_MASK                    0x300
#define D0F2xF4_x18_PTCASoftInvalidate_OFFSET                   10
#define D0F2xF4_x18_PTCASoftInvalidate_WIDTH                    1
#define D0F2xF4_x18_PTCASoftInvalidate_MASK                     0x400
#define D0F2xF4_x18_PTCA2MMode_OFFSET                           11
#define D0F2xF4_x18_PTCA2MMode_WIDTH                            1
#define D0F2xF4_x18_PTCA2MMode_MASK                             0x800
#define D0F2xF4_x18_Reserved_12_12_OFFSET                       12
#define D0F2xF4_x18_Reserved_12_12_WIDTH                        1
#define D0F2xF4_x18_Reserved_12_12_MASK                         0x1000
#define D0F2xF4_x18_PTCABypass_OFFSET                           13
#define D0F2xF4_x18_PTCABypass_WIDTH                            1
#define D0F2xF4_x18_PTCABypass_MASK                             0x2000
#define D0F2xF4_x18_Reserved_14_14_OFFSET                       14
#define D0F2xF4_x18_Reserved_14_14_WIDTH                        1
#define D0F2xF4_x18_Reserved_14_14_MASK                         0x4000
#define D0F2xF4_x18_PTCAParitySupport_OFFSET                    15
#define D0F2xF4_x18_PTCAParitySupport_WIDTH                     1
#define D0F2xF4_x18_PTCAParitySupport_MASK                      0x8000
#define D0F2xF4_x18_PTCAWays_OFFSET                             16
#define D0F2xF4_x18_PTCAWays_WIDTH                              8
#define D0F2xF4_x18_PTCAWays_MASK                               0xFF0000
#define D0F2xF4_x18_Reserved_27_24_OFFSET                       24
#define D0F2xF4_x18_Reserved_27_24_WIDTH                        4
#define D0F2xF4_x18_Reserved_27_24_MASK                         0xF000000
#define D0F2xF4_x18_PTCAEntries_OFFSET                          28
#define D0F2xF4_x18_PTCAEntries_WIDTH                           4
#define D0F2xF4_x18_PTCAEntries_MASK                            0xF0000000

/// D0F2xF4_x18
typedef union {
  struct {                                                            ///<
    UINT32                                      PTCAReplacementSel:2; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                        PTCALRUUpdatePri:1; ///<
    UINT32                                            PTCAParityEn:1; ///<
    UINT32                                            Reserved_7_5:3; ///<
    UINT32                                     PTCAInvalidationSel:2; ///<
    UINT32                                      PTCASoftInvalidate:1; ///<
    UINT32                                              PTCA2MMode:1; ///<
    UINT32                                          Reserved_12_12:1; ///<
    UINT32                                              PTCABypass:1; ///<
    UINT32                                          Reserved_14_14:1; ///<
    UINT32                                       PTCAParitySupport:1; ///<
    UINT32                                                PTCAWays:8; ///<
    UINT32                                          Reserved_27_24:4; ///<
    UINT32                                             PTCAEntries:4; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x18_STRUCT;

// **** D0F2xF4_x30 Register Definition ****
// Address
#define D0F2xF4_x30_ADDRESS                                     0x30
// Type
#define D0F2xF4_x30_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x30_ERRRuleLock1_OFFSET                         0
#define D0F2xF4_x30_ERRRuleLock1_WIDTH                          1
#define D0F2xF4_x30_ERRRuleLock1_MASK                           0x1
#define D0F2xF4_x30_Reserved_3_1_OFFSET                         1
#define D0F2xF4_x30_Reserved_3_1_WIDTH                          3
#define D0F2xF4_x30_Reserved_3_1_MASK                           0xE
#define D0F2xF4_x30_ERRRuleDisable3_OFFSET                      4
#define D0F2xF4_x30_ERRRuleDisable3_WIDTH                       28
#define D0F2xF4_x30_ERRRuleDisable3_MASK                        0xFFFFFFF0

/// D0F2xF4_x30
typedef union {
  struct {                                                            ///<
    UINT32                                            ERRRuleLock1:1; ///<
    UINT32                                            Reserved_3_1:3; ///<
    UINT32                                         ERRRuleDisable3:28; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x30_STRUCT;

// **** D0F2xF4_x33 Register Definition ****
// Address
#define D0F2xF4_x33_ADDRESS                                     0x33
// Type
#define D0F2xF4_x33_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x33_CKGateL2ARegsDisable_OFFSET                 0
#define D0F2xF4_x33_CKGateL2ARegsDisable_WIDTH                  1
#define D0F2xF4_x33_CKGateL2ARegsDisable_MASK                   0x1
#define D0F2xF4_x33_CKGateL2ADynamicDisable_OFFSET              1
#define D0F2xF4_x33_CKGateL2ADynamicDisable_WIDTH               1
#define D0F2xF4_x33_CKGateL2ADynamicDisable_MASK                0x2
#define D0F2xF4_x33_CKGateL2ACacheDisable_OFFSET                2
#define D0F2xF4_x33_CKGateL2ACacheDisable_WIDTH                 1
#define D0F2xF4_x33_CKGateL2ACacheDisable_MASK                  0x4
#define D0F2xF4_x33_CKGateL2ASpare_OFFSET                       3
#define D0F2xF4_x33_CKGateL2ASpare_WIDTH                        1
#define D0F2xF4_x33_CKGateL2ASpare_MASK                         0x8
#define D0F2xF4_x33_CKGateL2ALength_OFFSET                      4
#define D0F2xF4_x33_CKGateL2ALength_WIDTH                       2
#define D0F2xF4_x33_CKGateL2ALength_MASK                        0x30
#define D0F2xF4_x33_CKGateL2AStop_OFFSET                        6
#define D0F2xF4_x33_CKGateL2AStop_WIDTH                         2
#define D0F2xF4_x33_CKGateL2AStop_MASK                          0xC0
#define D0F2xF4_x33_Reserved_31_8_OFFSET                        8
#define D0F2xF4_x33_Reserved_31_8_WIDTH                         24
#define D0F2xF4_x33_Reserved_31_8_MASK                          0xFFFFFF00

/// D0F2xF4_x33
typedef union {
  struct {                                                            ///<
    UINT32                                    CKGateL2ARegsDisable:1; ///<
    UINT32                                 CKGateL2ADynamicDisable:1; ///<
    UINT32                                   CKGateL2ACacheDisable:1; ///<
    UINT32                                          CKGateL2ASpare:1; ///<
    UINT32                                         CKGateL2ALength:2; ///<
    UINT32                                           CKGateL2AStop:2; ///<
    UINT32                                           Reserved_31_8:24; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x33_STRUCT;

// **** D0F2xF4_x34 Register Definition ****
// Address
#define D0F2xF4_x34_ADDRESS                                     0x34
// Type
#define D0F2xF4_x34_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x34_L2aregGstPgsize_OFFSET                      0
#define D0F2xF4_x34_L2aregGstPgsize_WIDTH                       2
#define D0F2xF4_x34_L2aregGstPgsize_MASK                        0x3
#define D0F2xF4_x34_L2aregHostPgsize_OFFSET                     2
#define D0F2xF4_x34_L2aregHostPgsize_WIDTH                      2
#define D0F2xF4_x34_L2aregHostPgsize_MASK                       0xC
#define D0F2xF4_x34_Reserved_31_4_OFFSET                        4
#define D0F2xF4_x34_Reserved_31_4_WIDTH                         28
#define D0F2xF4_x34_Reserved_31_4_MASK                          0xFFFFFFF0

/// D0F2xF4_x34
typedef union {
  struct {                                                            ///<
    UINT32                                         L2aregGstPgsize:2; ///<
    UINT32                                        L2aregHostPgsize:2; ///<
    UINT32                                           Reserved_31_4:28; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x34_STRUCT;

// **** D0F2xF4_x3B Register Definition ****
// Address
#define D0F2xF4_x3B_ADDRESS                                     0x3b

// Type
#define D0F2xF4_x3B_TYPE                                        TYPE_D0F2xF4
// Field Data
#define D0F2xF4_x3B_FSM_ADDR_OFFSET                             0
#define D0F2xF4_x3B_FSM_ADDR_WIDTH                              8
#define D0F2xF4_x3B_FSM_ADDR_MASK                               0xff
#define D0F2xF4_x3B_Power_Down_OFFSET                           8
#define D0F2xF4_x3B_Power_Down_WIDTH                            1
#define D0F2xF4_x3B_Power_Down_MASK                             0x100
#define D0F2xF4_x3B_Power_Up_OFFSET                             9
#define D0F2xF4_x3B_Power_Up_WIDTH                              1
#define D0F2xF4_x3B_Power_Up_MASK                               0x200
#define D0F2xF4_x3B_P1_Select_OFFSET                            10
#define D0F2xF4_x3B_P1_Select_WIDTH                             1
#define D0F2xF4_x3B_P1_Select_MASK                              0x400
#define D0F2xF4_x3B_Reserved_OFFSET                             11
#define D0F2xF4_x3B_Reserved_WIDTH                              1
#define D0F2xF4_x3B_Reserved_MASK                               0x800
#define D0F2xF4_x3B_PG_Write_OFFSET                             12
#define D0F2xF4_x3B_PG_Write_WIDTH                              1
#define D0F2xF4_x3B_PG_Write_MASK                               0x1000
#define D0F2xF4_x3B_PG_Read_OFFSET                              13
#define D0F2xF4_x3B_PG_Read_WIDTH                               1
#define D0F2xF4_x3B_PG_Read_MASK                                0x2000
#define D0F2xF4_x3B_Reserved14_26_OFFSET                        14
#define D0F2xF4_x3B_Reserved14_26_WIDTH                         13
#define D0F2xF4_x3B_Reserved14_26_MASK                          0x7ffc000
#define D0F2xF4_x3B_SRBM_override_OFFSET                        27
#define D0F2xF4_x3B_SRBM_override_WIDTH                         1
#define D0F2xF4_x3B_SRBM_override_MASK                          0x8000000
#define D0F2xF4_x3B_REG_ADDR_OFFSET                             28
#define D0F2xF4_x3B_REG_ADDR_WIDTH                              4
#define D0F2xF4_x3B_REG_ADDR_MASK                               0xf0000000

/// D0F2xF4_x3B
typedef union {
  struct {                                                              ///<
    UINT32                                                 FSM_ADDR:8 ; ///<
    UINT32                                               Power_Down:1 ; ///<
    UINT32                                                 Power_Up:1 ; ///<
    UINT32                                                P1_Select:1 ; ///<
    UINT32                                                 Reserved:1 ; ///<
    UINT32                                                 PG_Write:1 ; ///<
    UINT32                                                  PG_Read:1 ; ///<
    UINT32                                            Reserved14_26:13; ///<
    UINT32                                            SRBM_override:1 ; ///<
    UINT32                                                 REG_ADDR:4 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F2xF4_x3B_STRUCT;

// **** D0F2xF4_x4C Register Definition ****
// Address
#define D0F2xF4_x4C_ADDRESS                                     0x4C
// Type
#define D0F2xF4_x4C_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x4C_QueueArbFBPri_OFFSET                        0
#define D0F2xF4_x4C_QueueArbFBPri_WIDTH                         1
#define D0F2xF4_x4C_QueueArbFBPri_MASK                          0x1
#define D0F2xF4_x4C_PTCAddrTransReqUpdate_OFFSET                1
#define D0F2xF4_x4C_PTCAddrTransReqUpdate_WIDTH                 1
#define D0F2xF4_x4C_PTCAddrTransReqUpdate_MASK                  0x2
#define D0F2xF4_x4C_FC1Dis_OFFSET                               2
#define D0F2xF4_x4C_FC1Dis_WIDTH                                1
#define D0F2xF4_x4C_FC1Dis_MASK                                 0x4
#define D0F2xF4_x4C_DTCUpdateVOneIVZero_OFFSET                  3
#define D0F2xF4_x4C_DTCUpdateVOneIVZero_WIDTH                   1
#define D0F2xF4_x4C_DTCUpdateVOneIVZero_MASK                    0x8
#define D0F2xF4_x4C_DTCUpdateVZeroIVOne_OFFSET                  4
#define D0F2xF4_x4C_DTCUpdateVZeroIVOne_WIDTH                   1
#define D0F2xF4_x4C_DTCUpdateVZeroIVOne_MASK                    0x10
#define D0F2xF4_x4C_FC2Dis_OFFSET                               5
#define D0F2xF4_x4C_FC2Dis_WIDTH                                1
#define D0F2xF4_x4C_FC2Dis_MASK                                 0x20
#define D0F2xF4_x4C_FC3Dis_OFFSET                               6
#define D0F2xF4_x4C_FC3Dis_WIDTH                                1
#define D0F2xF4_x4C_FC3Dis_MASK                                 0x40
#define D0F2xF4_x4C_FC2AltMode_OFFSET                           7
#define D0F2xF4_x4C_FC2AltMode_WIDTH                            1
#define D0F2xF4_x4C_FC2AltMode_MASK                             0x80
#define D0F2xF4_x4C_GstPartialPtcCntrl_OFFSET                   8
#define D0F2xF4_x4C_GstPartialPtcCntrl_WIDTH                    2
#define D0F2xF4_x4C_GstPartialPtcCntrl_MASK                     0x300
#define D0F2xF4_x4C_Reserved_31_10_OFFSET                       10
#define D0F2xF4_x4C_Reserved_31_10_WIDTH                        22
#define D0F2xF4_x4C_Reserved_31_10_MASK                         0xFFFFFC00

/// D0F2xF4_x4C
typedef union {
  struct {                                                            ///<
    UINT32                                           QueueArbFBPri:1; ///<
    UINT32                                   PTCAddrTransReqUpdate:1; ///<
    UINT32                                                  FC1Dis:1; ///<
    UINT32                                     DTCUpdateVOneIVZero:1; ///<
    UINT32                                     DTCUpdateVZeroIVOne:1; ///<
    UINT32                                                  FC2Dis:1; ///<
    UINT32                                                  FC3Dis:1; ///<
    UINT32                                              FC2AltMode:1; ///<
    UINT32                                      GstPartialPtcCntrl:2; ///<
    UINT32                                          Reserved_31_10:22; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x4C_STRUCT;

// **** D0F2xF4_x50 Register Definition ****
// Address
#define D0F2xF4_x50_ADDRESS                                     0x50
// Type
#define D0F2xF4_x50_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x50_PDCReplacementSel_OFFSET                    0
#define D0F2xF4_x50_PDCReplacementSel_WIDTH                     2
#define D0F2xF4_x50_PDCReplacementSel_MASK                      0x3
#define D0F2xF4_x50_Reserved_2_2_OFFSET                         2
#define D0F2xF4_x50_Reserved_2_2_WIDTH                          1
#define D0F2xF4_x50_Reserved_2_2_MASK                           0x4
#define D0F2xF4_x50_PDCLRUUpdatePri_OFFSET                      3
#define D0F2xF4_x50_PDCLRUUpdatePri_WIDTH                       1
#define D0F2xF4_x50_PDCLRUUpdatePri_MASK                        0x8
#define D0F2xF4_x50_PDCParityEn_OFFSET                          4
#define D0F2xF4_x50_PDCParityEn_WIDTH                           1
#define D0F2xF4_x50_PDCParityEn_MASK                            0x10
#define D0F2xF4_x50_Reserved_7_5_OFFSET                         5
#define D0F2xF4_x50_Reserved_7_5_WIDTH                          3
#define D0F2xF4_x50_Reserved_7_5_MASK                           0xE0
#define D0F2xF4_x50_PDCInvalidationSel_OFFSET                   8
#define D0F2xF4_x50_PDCInvalidationSel_WIDTH                    2
#define D0F2xF4_x50_PDCInvalidationSel_MASK                     0x300
#define D0F2xF4_x50_PDCSoftInvalidate_OFFSET                    10
#define D0F2xF4_x50_PDCSoftInvalidate_WIDTH                     1
#define D0F2xF4_x50_PDCSoftInvalidate_MASK                      0x400
#define D0F2xF4_x50_Reserved_11_11_OFFSET                       11
#define D0F2xF4_x50_Reserved_11_11_WIDTH                        1
#define D0F2xF4_x50_Reserved_11_11_MASK                         0x800
#define D0F2xF4_x50_PDCSearchDirection_OFFSET                   12
#define D0F2xF4_x50_PDCSearchDirection_WIDTH                    1
#define D0F2xF4_x50_PDCSearchDirection_MASK                     0x1000
#define D0F2xF4_x50_PDCBypass_OFFSET                            13
#define D0F2xF4_x50_PDCBypass_WIDTH                             1
#define D0F2xF4_x50_PDCBypass_MASK                              0x2000
#define D0F2xF4_x50_Reserved_14_14_OFFSET                       14
#define D0F2xF4_x50_Reserved_14_14_WIDTH                        1
#define D0F2xF4_x50_Reserved_14_14_MASK                         0x4000
#define D0F2xF4_x50_PDCParitySupport_OFFSET                     15
#define D0F2xF4_x50_PDCParitySupport_WIDTH                      1
#define D0F2xF4_x50_PDCParitySupport_MASK                       0x8000
#define D0F2xF4_x50_PDCWays_OFFSET                              16
#define D0F2xF4_x50_PDCWays_WIDTH                               8
#define D0F2xF4_x50_PDCWays_MASK                                0xFF0000
#define D0F2xF4_x50_Reserved_27_24_OFFSET                       24
#define D0F2xF4_x50_Reserved_27_24_WIDTH                        4
#define D0F2xF4_x50_Reserved_27_24_MASK                         0xF000000
#define D0F2xF4_x50_PDCEntries_OFFSET                           28
#define D0F2xF4_x50_PDCEntries_WIDTH                            4
#define D0F2xF4_x50_PDCEntries_MASK                             0xF0000000

/// D0F2xF4_x50
typedef union {
  struct {                                                            ///<
    UINT32                                       PDCReplacementSel:2; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                         PDCLRUUpdatePri:1; ///<
    UINT32                                             PDCParityEn:1; ///<
    UINT32                                            Reserved_7_5:3; ///<
    UINT32                                      PDCInvalidationSel:2; ///<
    UINT32                                       PDCSoftInvalidate:1; ///<
    UINT32                                          Reserved_11_11:1; ///<
    UINT32                                      PDCSearchDirection:1; ///<
    UINT32                                               PDCBypass:1; ///<
    UINT32                                          Reserved_14_14:1; ///<
    UINT32                                        PDCParitySupport:1; ///<
    UINT32                                                 PDCWays:8; ///<
    UINT32                                          Reserved_27_24:4; ///<
    UINT32                                              PDCEntries:4; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x50_STRUCT;

// **** D0F2xF4_x57 Register Definition ****
// Address
#define D0F2xF4_x57_ADDRESS                                     0x57
// Type
#define D0F2xF4_x57_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x57_L1ImuPcieGfxDis_OFFSET                      0
#define D0F2xF4_x57_L1ImuPcieGfxDis_WIDTH                       1
#define D0F2xF4_x57_L1ImuPcieGfxDis_MASK                        0x1
#define D0F2xF4_x57_CPL1Off_1_OFFSET                            1
#define D0F2xF4_x57_CPL1Off_1_WIDTH                             1
#define D0F2xF4_x57_CPL1Off_1_MASK                              0x2
#define D0F2xF4_x57_L1ImuIntGfxDis_OFFSET                       2
#define D0F2xF4_x57_L1ImuIntGfxDis_WIDTH                        1
#define D0F2xF4_x57_L1ImuIntGfxDis_MASK                         0x4
#define D0F2xF4_x57_CPL1Off_15_3_OFFSET                         3
#define D0F2xF4_x57_CPL1Off_15_3_WIDTH                          13
#define D0F2xF4_x57_CPL1Off_15_3_MASK                           0xFFF8
#define D0F2xF4_x57_Reserved_31_16_OFFSET                       16
#define D0F2xF4_x57_Reserved_31_16_WIDTH                        16
#define D0F2xF4_x57_Reserved_31_16_MASK                         0xFFFF0000

/// D0F2xF4_x57
typedef union {
  struct {                                                            ///<
    UINT32                                         L1ImuPcieGfxDis:1; ///<
    UINT32                                               CPL1Off_1:1; ///<
    UINT32                                          L1ImuIntGfxDis:1; ///<
    UINT32                                            CPL1Off_15_3:13; ///<
    UINT32                                          Reserved_31_16:16; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x57_STRUCT;

// **** D0F2xF4_x80 Register Definition ****
// Address
#define D0F2xF4_x80_ADDRESS                                     0x80
// Type
#define D0F2xF4_x80_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x80_ERRRuleLock0_OFFSET                         0
#define D0F2xF4_x80_ERRRuleLock0_WIDTH                          1
#define D0F2xF4_x80_ERRRuleLock0_MASK                           0x1
#define D0F2xF4_x80_Reserved_3_1_OFFSET                         1
#define D0F2xF4_x80_Reserved_3_1_WIDTH                          3
#define D0F2xF4_x80_Reserved_3_1_MASK                           0xE
#define D0F2xF4_x80_ERRRuleDisable0_OFFSET                      4
#define D0F2xF4_x80_ERRRuleDisable0_WIDTH                       28
#define D0F2xF4_x80_ERRRuleDisable0_MASK                        0xFFFFFFF0

/// D0F2xF4_x80
typedef union {
  struct {                                                            ///<
    UINT32                                            ERRRuleLock0:1; ///<
    UINT32                                            Reserved_3_1:3; ///<
    UINT32                                         ERRRuleDisable0:28; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x80_STRUCT;

// **** D0F2xF4_x90 Register Definition ****
// Address
#define D0F2xF4_x90_ADDRESS                                     0x90
// Type
#define D0F2xF4_x90_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x90_CKGateL2BRegsDisable_OFFSET                 0
#define D0F2xF4_x90_CKGateL2BRegsDisable_WIDTH                  1
#define D0F2xF4_x90_CKGateL2BRegsDisable_MASK                   0x1
#define D0F2xF4_x90_CKGateL2BDynamicDisable_OFFSET              1
#define D0F2xF4_x90_CKGateL2BDynamicDisable_WIDTH               1
#define D0F2xF4_x90_CKGateL2BDynamicDisable_MASK                0x2
#define D0F2xF4_x90_CKGateL2BMiscDisable_OFFSET                 2
#define D0F2xF4_x90_CKGateL2BMiscDisable_WIDTH                  1
#define D0F2xF4_x90_CKGateL2BMiscDisable_MASK                   0x4
#define D0F2xF4_x90_CKGateL2BCacheDisable_OFFSET                3
#define D0F2xF4_x90_CKGateL2BCacheDisable_WIDTH                 1
#define D0F2xF4_x90_CKGateL2BCacheDisable_MASK                  0x8
#define D0F2xF4_x90_CKGateL2BLength_OFFSET                      4
#define D0F2xF4_x90_CKGateL2BLength_WIDTH                       2
#define D0F2xF4_x90_CKGateL2BLength_MASK                        0x30
#define D0F2xF4_x90_CKGateL2BStop_OFFSET                        6
#define D0F2xF4_x90_CKGateL2BStop_WIDTH                         2
#define D0F2xF4_x90_CKGateL2BStop_MASK                          0xC0
#define D0F2xF4_x90_Reserved_31_8_OFFSET                        8
#define D0F2xF4_x90_Reserved_31_8_WIDTH                         24
#define D0F2xF4_x90_Reserved_31_8_MASK                          0xFFFFFF00

/// D0F2xF4_x90
typedef union {
  struct {                                                            ///<
    UINT32                                    CKGateL2BRegsDisable:1; ///<
    UINT32                                 CKGateL2BDynamicDisable:1; ///<
    UINT32                                    CKGateL2BMiscDisable:1; ///<
    UINT32                                   CKGateL2BCacheDisable:1; ///<
    UINT32                                         CKGateL2BLength:2; ///<
    UINT32                                           CKGateL2BStop:2; ///<
    UINT32                                           Reserved_31_8:24; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x90_STRUCT;

// **** D0F2xF4_x94 Register Definition ****
// Address
#define D0F2xF4_x94_ADDRESS                                     0x94
// Type
#define D0F2xF4_x94_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x94_L2bregGstPgsize_OFFSET                      0
#define D0F2xF4_x94_L2bregGstPgsize_WIDTH                       2
#define D0F2xF4_x94_L2bregGstPgsize_MASK                        0x3
#define D0F2xF4_x94_L2bregHostPgsize_OFFSET                     2
#define D0F2xF4_x94_L2bregHostPgsize_WIDTH                      2
#define D0F2xF4_x94_L2bregHostPgsize_MASK                       0xC
#define D0F2xF4_x94_Reserved_31_4_OFFSET                        4
#define D0F2xF4_x94_Reserved_31_4_WIDTH                         28
#define D0F2xF4_x94_Reserved_31_4_MASK                          0xFFFFFFF0

/// D0F2xF4_x94
typedef union {
  struct {                                                            ///<
    UINT32                                         L2bregGstPgsize:2; ///<
    UINT32                                        L2bregHostPgsize:2; ///<
    UINT32                                           Reserved_31_4:28; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x94_STRUCT;

// **** D0F2x80 Register Definition ****
// Address
#define D0F2x80_ADDRESS                                         0x80

// Type
#define D0F2x80_TYPE                                            TYPE_D0F2
// Field Data
#define D0F2x80_PrefSupW_OFFSET                                 0
#define D0F2x80_PrefSupW_WIDTH                                  1
#define D0F2x80_PrefSupW_MASK                                   0x1
#define D0F2x80_PprSupW_OFFSET                                  1
#define D0F2x80_PprSupW_WIDTH                                   1
#define D0F2x80_PprSupW_MASK                                    0x2
#define D0F2x80_Reserved_2_2_OFFSET                             2
#define D0F2x80_Reserved_2_2_WIDTH                              1
#define D0F2x80_Reserved_2_2_MASK                               0x4
#define D0F2x80_NxSupW_OFFSET                                   3
#define D0F2x80_NxSupW_WIDTH                                    1
#define D0F2x80_NxSupW_MASK                                     0x8
#define D0F2x80_GtSupW_OFFSET                                   4
#define D0F2x80_GtSupW_WIDTH                                    1
#define D0F2x80_GtSupW_MASK                                     0x10
#define D0F2x80_Reserved_5_5_OFFSET                             5
#define D0F2x80_Reserved_5_5_WIDTH                              1
#define D0F2x80_Reserved_5_5_MASK                               0x20
#define D0F2x80_IaSupW_OFFSET                                   6
#define D0F2x80_IaSupW_WIDTH                                    1
#define D0F2x80_IaSupW_MASK                                     0x40
#define D0F2x80_Reserved_7_7_OFFSET                             7
#define D0F2x80_Reserved_7_7_WIDTH                              1
#define D0F2x80_Reserved_7_7_MASK                               0x80
#define D0F2x80_Reserved_8_8_OFFSET                             8
#define D0F2x80_Reserved_8_8_WIDTH                              1
#define D0F2x80_Reserved_8_8_MASK                               0x100
#define D0F2x80_PcSupW_OFFSET                                   9
#define D0F2x80_PcSupW_WIDTH                                    1
#define D0F2x80_PcSupW_MASK                                     0x200
#define D0F2x80_HatsW_OFFSET                                    10
#define D0F2x80_HatsW_WIDTH                                     2
#define D0F2x80_HatsW_MASK                                      0xc00
#define D0F2x80_Reserved_31_12_OFFSET                           12
#define D0F2x80_Reserved_31_12_WIDTH                            20
#define D0F2x80_Reserved_31_12_MASK                             0xfffff000

/// D0F2x80
typedef union {
  struct {                                                              ///<
    UINT32                                                 PrefSupW:1 ; ///<
    UINT32                                                  PprSupW:1 ; ///<
    UINT32                                             Reserved_2_2:1 ; ///<
    UINT32                                                   NxSupW:1 ; ///<
    UINT32                                                   GtSupW:1 ; ///<
    UINT32                                             Reserved_5_5:1 ; ///<
    UINT32                                                   IaSupW:1 ; ///<
    UINT32                                             Reserved_7_7:1 ; ///<
    UINT32                                             Reserved_8_8:1 ; ///<
    UINT32                                                   PcSupW:1 ; ///<
    UINT32                                                    HatsW:2 ; ///<
    UINT32                                           Reserved_31_12:20; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F2x80_STRUCT;

// **** D0F0x90 Register Definition ****
// Address
#define D0F0x90_ADDRESS                                         0x90
// Type
#define D0F0x90_TYPE                                            TYPE_D0F0

// **** D0F0x94 Register Definition ****
// Address
#define D0F0x94_ADDRESS                                         0x94
// Type
#define D0F0x94_TYPE                                            TYPE_D0F0

// **** D0F2xFC_x09_L1 Register Definition ****
// Address
#define D0F2xFC_x09_L1_ADDRESS(Sel)                             ((Sel << 16) | 0x9)
// Type
#define D0F2xFC_x09_L1_TYPE                                     TYPE_D0F2xFC

// **** D18F1xF0 Register Definition ****
// Address
#define D18F1xF0_ADDRESS                                        0xF0
// Type
#define D18F1xF0_TYPE                                           TYPE_D18F1

// **** D18F1x200 Register Definition ****
// Address
#define D18F1x200_ADDRESS                                       0x200
// Type
#define D18F1x200_TYPE                                          TYPE_D18F1

// **** D18F1x204 Register Definition ****
// Address
#define D18F1x204_ADDRESS                                       0x204
// Type
#define D18F1x204_TYPE                                          TYPE_D18F1

// **** D18F1x240 Register Definition ****
// Address
#define D18F1x240_ADDRESS                                       0x240
// Type
#define D18F1x240_TYPE                                          TYPE_D18F1

// **** D18F2x40_dct0 Register Definition ****
// Address
#define D18F2x40_dct0_ADDRESS                                   0x40
// Type
#define D18F2x40_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x44_dct0 Register Definition ****
// Address
#define D18F2x44_dct0_ADDRESS                                   0x44
// Type
#define D18F2x44_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x48_dct0 Register Definition ****
// Address
#define D18F2x48_dct0_ADDRESS                                   0x48
// Type
#define D18F2x48_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x4C_dct0 Register Definition ****
// Address
#define D18F2x4C_dct0_ADDRESS                                   0x4C
// Type
#define D18F2x4C_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x60_dct0 Register Definition ****
// Address
#define D18F2x60_dct0_ADDRESS                                   0x60
// Type
#define D18F2x60_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x64_dct0 Register Definition ****
// Address
#define D18F2x64_dct0_ADDRESS                                   0x64
// Type
#define D18F2x64_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x80_dct0 Register Definition ****
// Address
#define D18F2x80_dct0_ADDRESS                                   0x80
// Type
#define D18F2x80_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2xA8_dct0 Register Definition ****
// Address
#define D18F2xA8_dct0_ADDRESS                                   0xA8
// Type
#define D18F2xA8_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x78_dct0 Register Definition ****
// Address
#define D18F2x78_dct0_ADDRESS                                   0x78
// Type
#define D18F2x78_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x110 Register Definition ****
// Address
#define D18F2x110_ADDRESS                                       0x110
// Type
#define D18F2x110_TYPE                                          TYPE_D18F2

// **** D18F2x114 Register Definition ****
// Address
#define D18F2x114_ADDRESS                                       0x114
// Type
#define D18F2x114_TYPE                                          TYPE_D18F2

// **** DxFxx18 Register Definition ****
// Address
#define DxFxx18_ADDRESS                                         0x18
// Type
#define DxFxx18_TYPE                                            TYPE_D2F1

// **** DxFxx20 Register Definition ****
// Address
#define DxFxx20_ADDRESS                                         0x20
// Type
#define DxFxx20_TYPE                                            TYPE_D2F1

// **** DxFxx24 Register Definition ****
// Address
#define DxFxx24_ADDRESS                                         0x24
// Type
#define DxFxx24_TYPE                                            TYPE_D2F1

// **** D0F0x60 Register Definition ****
// Address
#define D0F0x60_ADDRESS                                         0x60
// Type
#define D0F0x60_TYPE                                            TYPE_D0F0

// **** D0F0xB8 Register Definition ****
// Address
#define D0F0xB8_ADDRESS                                         0xB8
// Type
#define D0F0xB8_TYPE                                            TYPE_D0F0

// **** D0F0xE0 Register Definition ****
// Address
#define D0F0xE0_ADDRESS                                         0xE0
// Type
#define D0F0xE0_TYPE                                            TYPE_D0F0

// **** D0F2xF0 Register Definition ****
// Address
#define D0F2xF0_ADDRESS                                         0xF0
// Type
#define D0F2xF0_TYPE                                            TYPE_D0F2

// **** D0F2xF4 Register Definition ****
// Address
#define D0F2xF4_ADDRESS                                         0xF4
// Type
#define D0F2xF4_TYPE                                            TYPE_D0F2

// **** D0F2xF8 Register Definition ****
// Address
#define D0F2xF8_ADDRESS                                         0xF8
// Type
#define D0F2xF8_TYPE                                            TYPE_D0F2

// **** D0F0x64_x1F Register Definition ****
// Address
#define D0F0x64_x1F_ADDRESS                                     0x1F
// Type
#define D0F0x64_x1F_TYPE                                        TYPE_D0F0x64

// **** D0F0xE4_PIF_0017 Register Definition ****
// Address
#define D0F0xE4_PIF_0017_ADDRESS                                0x17
// Type
#define D0F0xE4_PIF_0017_TYPE                                   TYPE_D0F0xE4

// **** DxFxxE4_x01 Register Definition ****
// Address
#define DxFxxE4_x01_ADDRESS                                     0x1
// Type
#define DxFxxE4_x01_TYPE                                        TYPE_D2F1xE4

// **** D0F2xF4_x49 Register Definition ****
// Address
#define D0F2xF4_x49_ADDRESS                                     0x49
// Type
#define D0F2xF4_x49_TYPE                                        TYPE_D0F2xF4

// **** GMMx15C0 Register Definition ****
// Address
#define GMMx15C0_ADDRESS                                        0x15C0
// Type
#define GMMx15C0_TYPE                                           TYPE_GMM

// **** GMMx2014 Register Definition ****
// Address
#define GMMx2014_ADDRESS                                        0x2014
// Type
#define GMMx2014_TYPE                                           TYPE_GMM

// **** GMMx2018 Register Definition ****
// Address
#define GMMx2018_ADDRESS                                        0x2018
// Type
#define GMMx2018_TYPE                                           TYPE_GMM

// **** GMMx201C Register Definition ****
// Address
#define GMMx201C_ADDRESS                                        0x201C
// Type
#define GMMx201C_TYPE                                           TYPE_GMM

// **** GMMx2020 Register Definition ****
// Address
#define GMMx2020_ADDRESS                                        0x2020
// Type
#define GMMx2020_TYPE                                           TYPE_GMM

// **** GMMx2068 Register Definition ****
// Address
#define GMMx2068_ADDRESS                                        0x2068
// Type
#define GMMx2068_TYPE                                           TYPE_GMM

// **** GMMx206C Register Definition ****
// Address
#define GMMx206C_ADDRESS                                        0x206C
// Type
#define GMMx206C_TYPE                                           TYPE_GMM

// **** GMMx20AC Register Definition ****
// Address
#define GMMx20AC_ADDRESS                                        0x20AC
// Type
#define GMMx20AC_TYPE                                           TYPE_GMM

// **** GMMx20B8 Register Definition ****
// Address
#define GMMx20B8_ADDRESS                                        0x20B8
// Type
#define GMMx20B8_TYPE                                           TYPE_GMM

// **** GMMx20BC Register Definition ****
// Address
#define GMMx20BC_ADDRESS                                        0x20BC
// Type
#define GMMx20BC_TYPE                                           TYPE_GMM

// **** GMMx20C0 Register Definition ****
// Address
#define GMMx20C0_ADDRESS                                        0x20C0
// Type
#define GMMx20C0_TYPE                                           TYPE_GMM

// **** GMMx20EC Register Definition ****
// Address
#define GMMx20EC_ADDRESS                                        0x20EC
// Type
#define GMMx20EC_TYPE                                           TYPE_GMM

// **** GMMx2114 Register Definition ****
// Address
#define GMMx2114_ADDRESS                                        0x2114
// Type
#define GMMx2114_TYPE                                           TYPE_GMM

// **** GMMx2144 Register Definition ****
// Address
#define GMMx2144_ADDRESS                                        0x2144
// Type
#define GMMx2144_TYPE                                           TYPE_GMM

// **** GMMx217C Register Definition ****
// Address
#define GMMx217C_ADDRESS                                        0x217C
// Type
#define GMMx217C_TYPE                                           TYPE_GMM

// **** GMMx2184 Register Definition ****
// Address
#define GMMx2184_ADDRESS                                        0x2184
// Type
#define GMMx2184_TYPE                                           TYPE_GMM

// **** GMMx218C Register Definition ****
// Address
#define GMMx218C_ADDRESS                                        0x218C
// Type
#define GMMx218C_TYPE                                           TYPE_GMM

// **** GMMx21C0 Register Definition ****
// Address
#define GMMx21C0_ADDRESS                                        0x21C0
// Type
#define GMMx21C0_TYPE                                           TYPE_GMM

// **** GMMx21E0 Register Definition ****
// Address
#define GMMx21E0_ADDRESS                                        0x21E0
// Type
#define GMMx21E0_TYPE                                           TYPE_GMM

// **** GMMx2214 Register Definition ****
// Address
#define GMMx2214_ADDRESS                                        0x2214
// Type
#define GMMx2214_TYPE                                           TYPE_GMM

// **** GMMx2220 Register Definition ****
// Address
#define GMMx2220_ADDRESS                                        0x2220
// Type
#define GMMx2220_TYPE                                           TYPE_GMM

// **** GMMx2478 Register Definition ****
// Address
#define GMMx2478_ADDRESS                                        0x2478
// Type
#define GMMx2478_TYPE                                           TYPE_GMM

// **** GMMx253C Register Definition ****
// Address
#define GMMx253C_ADDRESS                                        0x253C
// Type
#define GMMx253C_TYPE                                           TYPE_GMM

// **** GMMx2544 Register Definition ****
// Address
#define GMMx2544_ADDRESS                                        0x2544
// Type
#define GMMx2544_TYPE                                           TYPE_GMM

// **** GMMx2550 Register Definition ****
// Address
#define GMMx2550_ADDRESS                                        0x2550
// Type
#define GMMx2550_TYPE                                           TYPE_GMM

// **** GMMx2558 Register Definition ****
// Address
#define GMMx2558_ADDRESS                                        0x2558
// Type
#define GMMx2558_TYPE                                           TYPE_GMM

// **** GMMx25C8 Register Definition ****
// Address
#define GMMx25C8_ADDRESS                                        0x25C8
// Type
#define GMMx25C8_TYPE                                           TYPE_GMM

// **** GMMx25CC Register Definition ****
// Address
#define GMMx25CC_ADDRESS                                        0x25CC
// Type
#define GMMx25CC_TYPE                                           TYPE_GMM

// **** GMMx25E0 Register Definition ****
// Address
#define GMMx25E0_ADDRESS                                        0x25E0
// Type
#define GMMx25E0_TYPE                                           TYPE_GMM

// **** GMMx25E4 Register Definition ****
// Address
#define GMMx25E4_ADDRESS                                        0x25E4
// Type
#define GMMx25E4_TYPE                                           TYPE_GMM

// **** GMMx25E8 Register Definition ****
// Address
#define GMMx25E8_ADDRESS                                        0x25E8
// Type
#define GMMx25E8_TYPE                                           TYPE_GMM

// **** GMMx2628 Register Definition ****
// Address
#define GMMx2628_ADDRESS                                        0x2628
// Type
#define GMMx2628_TYPE                                           TYPE_GMM

// **** GMMx262C Register Definition ****
// Address
#define GMMx262C_ADDRESS                                        0x262C
// Type
#define GMMx262C_TYPE                                           TYPE_GMM

// **** GMMx2648 Register Definition ****
// Address
#define GMMx2648_ADDRESS                                        0x2648
// Type
#define GMMx2648_TYPE                                           TYPE_GMM

// **** GMMx264C Register Definition ****
// Address
#define GMMx264C_ADDRESS                                        0x264C
// Type
#define GMMx264C_TYPE                                           TYPE_GMM

// **** GMMx2650 Register Definition ****
// Address
#define GMMx2650_ADDRESS                                        0x2650
// Type
#define GMMx2650_TYPE                                           TYPE_GMM

// **** GMMx2768 Register Definition ****
// Address
#define GMMx2768_ADDRESS                                        0x2768
// Type
#define GMMx2768_TYPE                                           TYPE_GMM

// **** GMMx276C Register Definition ****
// Address
#define GMMx276C_ADDRESS                                        0x276C
// Type
#define GMMx276C_TYPE                                           TYPE_GMM

// **** GMMx277C Register Definition ****
// Address
#define GMMx277C_ADDRESS                                        0x277C
// Type
#define GMMx277C_TYPE                                           TYPE_GMM

// **** GMMx278C Register Definition ****
// Address
#define GMMx278C_ADDRESS                                        0x278C
// Type
#define GMMx278C_TYPE                                           TYPE_GMM

// **** GMMx2790 Register Definition ****
// Address
#define GMMx2790_ADDRESS                                        0x2790
// Type
#define GMMx2790_TYPE                                           TYPE_GMM

// **** GMMx2794 Register Definition ****
// Address
#define GMMx2794_ADDRESS                                        0x2794
// Type
#define GMMx2794_TYPE                                           TYPE_GMM

// **** GMMx2798 Register Definition ****
// Address
#define GMMx2798_ADDRESS                                        0x2798
// Type
#define GMMx2798_TYPE                                           TYPE_GMM

// **** GMMx27A4 Register Definition ****
// Address
#define GMMx27A4_ADDRESS                                        0x27A4
// Type
#define GMMx27A4_TYPE                                           TYPE_GMM

// **** GMMx27A8 Register Definition ****
// Address
#define GMMx27A8_ADDRESS                                        0x27A8
// Type
#define GMMx27A8_TYPE                                           TYPE_GMM

// **** GMMx27C0 Register Definition ****
// Address
#define GMMx27C0_ADDRESS                                        0x27C0
// Type
#define GMMx27C0_TYPE                                           TYPE_GMM

// **** GMMx27C4 Register Definition ****
// Address
#define GMMx27C4_ADDRESS                                        0x27C4
// Type
#define GMMx27C4_TYPE                                           TYPE_GMM

// **** GMMx27D0 Register Definition ****
// Address
#define GMMx27D0_ADDRESS                                        0x27D0
// Type
#define GMMx27D0_TYPE                                           TYPE_GMM

// **** GMMx2814 Register Definition ****
// Address
#define GMMx2814_ADDRESS                                        0x2814
// Type
#define GMMx2814_TYPE                                           TYPE_GMM

// **** GMMx281C Register Definition ****
// Address
#define GMMx281C_ADDRESS                                        0x281C
// Type
#define GMMx281C_TYPE                                           TYPE_GMM

// **** GMMx2824 Register Definition ****
// Address
#define GMMx2824_ADDRESS                                        0x2824
// Type
#define GMMx2824_TYPE                                           TYPE_GMM

// **** GMMx282C Register Definition ****
// Address
#define GMMx282C_ADDRESS                                        0x282C
// Type
#define GMMx282C_TYPE                                           TYPE_GMM

// **** GMMx2834 Register Definition ****
// Address
#define GMMx2834_ADDRESS                                        0x2834
// Type
#define GMMx2834_TYPE                                           TYPE_GMM

// **** GMMx283C Register Definition ****
// Address
#define GMMx283C_ADDRESS                                        0x283C
// Type
#define GMMx283C_TYPE                                           TYPE_GMM

// **** GMMx2844 Register Definition ****
// Address
#define GMMx2844_ADDRESS                                        0x2844
// Type
#define GMMx2844_TYPE                                           TYPE_GMM

// **** GMMx284C Register Definition ****
// Address
#define GMMx284C_ADDRESS                                        0x284C
// Type
#define GMMx284C_TYPE                                           TYPE_GMM

// **** GMMx2854 Register Definition ****
// Address
#define GMMx2854_ADDRESS                                        0x2854
// Type
#define GMMx2854_TYPE                                           TYPE_GMM

// **** GMMx285C Register Definition ****
// Address
#define GMMx285C_ADDRESS                                        0x285C
// Type
#define GMMx285C_TYPE                                           TYPE_GMM

// **** GMMx2864 Register Definition ****
// Address
#define GMMx2864_ADDRESS                                        0x2864
// Type
#define GMMx2864_TYPE                                           TYPE_GMM

// **** GMMx2868 Register Definition ****
// Address
#define GMMx2868_ADDRESS                                        0x2868
// Type
#define GMMx2868_TYPE                                           TYPE_GMM

// **** GMMx286C Register Definition ****
// Address
#define GMMx286C_ADDRESS                                        0x286C
// Type
#define GMMx286C_TYPE                                           TYPE_GMM

// **** GMMx2870 Register Definition ****
// Address
#define GMMx2870_ADDRESS                                        0x2870
// Type
#define GMMx2870_TYPE                                           TYPE_GMM

// **** GMMx2874 Register Definition ****
// Address
#define GMMx2874_ADDRESS                                        0x2874
// Type
#define GMMx2874_TYPE                                           TYPE_GMM

// **** GMMx2878 Register Definition ****
// Address
#define GMMx2878_ADDRESS                                        0x2878
// Type
#define GMMx2878_TYPE                                           TYPE_GMM

// **** GMMx287C Register Definition ****
// Address
#define GMMx287C_ADDRESS                                        0x287C
// Type
#define GMMx287C_TYPE                                           TYPE_GMM

// **** GMMx2880 Register Definition ****
// Address
#define GMMx2880_ADDRESS                                        0x2880
// Type
#define GMMx2880_TYPE                                           TYPE_GMM

// **** GMMx2C04 Register Definition ****
// Address
#define GMMx2C04_ADDRESS                                        0x2C04
// Type
#define GMMx2C04_TYPE                                           TYPE_GMM

// **** GMMx3500 Register Definition ****
// Address
#define GMMx3500_ADDRESS                                        0x3500
// Type
#define GMMx3500_TYPE                                           TYPE_GMM

// **** GMMx3504 Register Definition ****
// Address
#define GMMx3504_ADDRESS                                        0x3504
// Type
#define GMMx3504_TYPE                                           TYPE_GMM

// **** GMMx3538 Register Definition ****
// Address
#define GMMx3538_ADDRESS                                        0x3538
// Type
#define GMMx3538_TYPE                                           TYPE_GMM

// **** GMMx353C Register Definition ****
// Address
#define GMMx353C_ADDRESS                                        0x353C
// Type
#define GMMx353C_TYPE                                           TYPE_GMM

// **** GMMx3544 Register Definition ****
// Address
#define GMMx3544_ADDRESS                                        0x3544
// Type
#define GMMx3544_TYPE                                           TYPE_GMM

// **** GMMx3700 Register Definition ****
// Address
#define GMMx3700_ADDRESS                                        0x3700
// Type
#define GMMx3700_TYPE                                           TYPE_GMM

// **** GMMx3704 Register Definition ****
// Address
#define GMMx3704_ADDRESS                                        0x3704
// Type
#define GMMx3704_TYPE                                           TYPE_GMM

// **** GMMx3748 Register Definition ****
// Address
#define GMMx3748_ADDRESS                                        0x3748
// Type
#define GMMx3748_TYPE                                           TYPE_GMM

// **** GMMx3750 Register Definition ****
// Address
#define GMMx3750_ADDRESS                                        0x3750
// Type
#define GMMx3750_TYPE                                           TYPE_GMM

// **** GMMx3754 Register Definition ****
// Address
#define GMMx3754_ADDRESS                                        0x3754
// Type
#define GMMx3754_TYPE                                           TYPE_GMM

// **** GMMx3758 Register Definition ****
// Address
#define GMMx3758_ADDRESS                                        0x3758
// Type
#define GMMx3758_TYPE                                           TYPE_GMM

// **** GMMx375C Register Definition ****
// Address
#define GMMx375C_ADDRESS                                        0x375C
// Type
#define GMMx375C_TYPE                                           TYPE_GMM

// **** GMMx5428 Register Definition ****
// Address
#define GMMx5428_ADDRESS                                        0x5428
// Type
#define GMMx5428_TYPE                                           TYPE_GMM

// **** GMMx22000 Register Definition ****
// Address
#define GMMx22000_ADDRESS                                       0x22000
// Type
#define GMMx22000_TYPE                                          TYPE_GMM

// **** GMMx22004 Register Definition ****
// Address
#define GMMx22004_ADDRESS                                       0x22004
// Type
#define GMMx22004_TYPE                                          TYPE_GMM

// **** GMMx22008 Register Definition ****
// Address
#define GMMx22008_ADDRESS                                       0x22008
// Type
#define GMMx22008_TYPE                                          TYPE_GMM

// **** GMMx2200C Register Definition ****
// Address
#define GMMx2200C_ADDRESS                                       0x2200C
// Type
#define GMMx2200C_TYPE                                          TYPE_GMM

// **** GMMx220B0 Register Definition ****
// Address
#define GMMx220B0_ADDRESS                                       0x220B0
// Type
#define GMMx220B0_TYPE                                          TYPE_GMM

// **** D0F0xE4_WRAP_8021 Register Definition ****
// Address
#define D0F0xE4_WRAP_8021_ADDRESS                               0x8021
// Type
#define D0F0xE4_WRAP_8021_TYPE                                  TYPE_D0F0xE4

// **** D0F0xE4_WRAP_8022 Register Definition ****
// Address
#define D0F0xE4_WRAP_8022_ADDRESS                               0x8022
// Type
#define D0F0xE4_WRAP_8022_TYPE                                  TYPE_D0F0xE4

// **** D0F0xE4_WRAP_8025 Register Definition ****
// Address
#define D0F0xE4_WRAP_8025_ADDRESS                               0x8025
// Type
#define D0F0xE4_WRAP_8025_TYPE                                  TYPE_D0F0xE4

// **** D0F0xE4_WRAP_8026 Register Definition ****
// Address
#define D0F0xE4_WRAP_8026_ADDRESS                               0x8026
// Type
#define D0F0xE4_WRAP_8026_TYPE                                  TYPE_D0F0xE4

// **** D0F0xE4_PHY_8004 Register Definition ****
// Address
#define D0F0xE4_PHY_8004_ADDRESS                               0x8004
// Type
#define D0F0xE4_PHY_8004_TYPE                                  TYPE_D0F0xE4

// **** D0F0xD0 Register Definition ****
// Address
#define D0F0xD0_ADDRESS                                         0xD0
// Type
#define D0F0xD0_TYPE                                            TYPE_D0F0

// **** D0F0xF8 Register Definition ****
// Address
#define D0F0xF8_ADDRESS                                         0xF8
// Type
#define D0F0xF8_TYPE                                            TYPE_D0F0

// **** D0F0x64_x19 Register Definition ****
// Address
#define D0F0x64_x19_ADDRESS                                     0x19
// Type
#define D0F0x64_x19_TYPE                                        TYPE_D0F0x64

// **** D0F0x64_x1A Register Definition ****
// Address
#define D0F0x64_x1A_ADDRESS                                     0x1A
// Type
#define D0F0x64_x1A_TYPE                                        TYPE_D0F0x64

// **** D0F0x64_x4E Register Definition ****
// Address
#define D0F0x64_x4E_ADDRESS                                     0x4E
// Type
#define D0F0x64_x4E_TYPE                                        TYPE_D0F0x64

// **** D0F0xBC_x20000 Register Definition ****
// Address
#define D0F0xBC_x20000_ADDRESS                                  0x20000
// Type
#define D0F0xBC_x20000_TYPE                                     TYPE_D0F0xBC

// **** D0F0xBC_x0 Register Definition ****
// Address
#define D0F0xBC_x0_ADDRESS                                      0x0
// Type
#define D0F0xBC_x0_TYPE                                         TYPE_D0F0xBC

// **** D0F0xBC_x3F834 Register Definition ****
// Address
#define D0F0xBC_x3F834_ADDRESS                                  0x3F834
// Type
#define D0F0xBC_x3F834_TYPE                                     TYPE_D0F0xBC

// **** D0F0xBC_x3F838 Register Definition ****
// Address
#define D0F0xBC_x3F838_ADDRESS                                  0x3F838
// Type
#define D0F0xBC_x3F838_TYPE                                     TYPE_D0F0xBC

// **** D0F0xBC_x80008000 Register Definition ****
// Address
#define D0F0xBC_x80008000_ADDRESS                               0x80008000
// Type
#define D0F0xBC_x80008000_TYPE                                  TYPE_D0F0xBC

// **** D0F0xBC_x800000A4 Register Definition ****
// Address
#define D0F0xBC_x800000A4_ADDRESS                               0x800000A4
// Type
#define D0F0xBC_x800000A4_TYPE                                  TYPE_D0F0xBC

// **** D0F0xBC_xC210003C Register Definition ****
// Address
#define D0F0xBC_xC210003C_ADDRESS                               0xC210003C
// Type
#define D0F0xBC_xC210003C_TYPE                                  TYPE_D0F0xBC

// **** D0F0xBC_xC00C002C Register Definition ****
// Address
#define D0F0xBC_xC00C002C_ADDRESS                               0xC00C002C
// Type
#define D0F0xBC_xC00C002C_TYPE                                  TYPE_D0F0xBC

// **** D0F2x50 Register Definition ****
// Address
#define D0F2x50_ADDRESS                                         0x50
// Type
#define D0F2x50_TYPE                                            TYPE_D0F2

// **** D0F2xF4_x47 Register Definition ****
// Address
#define D0F2xF4_x22_ADDRESS                                     0x22
// Type
#define D0F2xF4_x22_TYPE                                        TYPE_D0F2xF4

// **** D0F2xF4_x47 Register Definition ****
// Address
#define D0F2xF4_x47_ADDRESS                                     0x47
// Type
#define D0F2xF4_x47_TYPE                                        TYPE_D0F2xF4

// **** D0F2xF4_x53 Register Definition ****
// Address
#define D0F2xF4_x53_ADDRESS                                     0x53
// Type
#define D0F2xF4_x53_TYPE                                        TYPE_D0F2xF4

// **** D0F2xF4_x56 Register Definition ****
// Address
#define D0F2xF4_x56_ADDRESS                                     0x56
// Type
#define D0F2xF4_x56_TYPE                                        TYPE_D0F2xF4

// **** D0F0xD4_x010914C3 Register Definition ****
// Address
#define D0F0xD4_x010914C3_ADDRESS                               0x010914C3
// Type
#define D0F0xD4_x010914C3_TYPE                                  TYPE_D0F0xD4

// **** D0F0xBC_xC01040D0 Register Definition ****
// Address
#define D0F0xBC_xC01040D0_ADDRESS                               0xC01040D0
// Type
#define D0F0xBC_xC01040D0_TYPE                                  TYPE_D0F0xBC

// **** D18F2x40_dct0 Register Definition ****
// Address
#define D18F2x40_dct0_ADDRESS                                   0x40
// Type
#define D18F2x40_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x9C_x0D0F_001E_dct0_mp0 Register Definition ****
// Address
#define D18F2x9C_x0D0F_001E_dct0_mp0_ADDRESS                    0x0D0F001E
// Type
#define D18F2x9C_x0D0F_001E_dct0_mp0_TYPE                       TYPE_D18F2x9C_dct0_mp0

// **** D18F2x9C_x0D0F_001F_dct0_mp0 Register Definition ****
// Address
#define D18F2x9C_x0D0F_001F_dct0_mp0_ADDRESS                    0x0D0F001F
// Type
#define D18F2x9C_x0D0F_001F_dct0_mp0_TYPE                       TYPE_D18F2x9C_dct0_mp0

// **** D18F2x9C_x0D0F_001F_dct0_mp1 Register Definition ****
// Address
#define D18F2x9C_x0D0F_001F_dct0_mp1_ADDRESS                    0x0D0F001F
// Type
#define D18F2x9C_x0D0F_001F_dct0_mp1_TYPE                       TYPE_D18F2x9C_dct0_mp1

// **** D18F2x9C_dct0 Register Definition ****
// Address
#define D18F2x9C_dct0_ADDRESS                                   0x9c
// Type
#define D18F2x9C_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D8F0xE8 Register Definition ****
// Address
#define D8F0xE8_ADDRESS                                         0xE8

// **** D8F0xEC_x4A Register Definition ****
// Address
#define D8F0xEC_x4A_ADDRESS                                     0x4A
// Type
#define D8F0xEC_x4A_TYPE                                        TYPE_D8F0xEC

// Field Data
#define D8F0xEC_x4A_Reserved_23_0_OFFSET                         0
#define D8F0xEC_x4A_Reserved_23_0_WIDTH                          24
#define D8F0xEC_x4A_Reserved_23_0_MASK                           0xFFFFFF
#define D8F0xEC_x4A_Reserved_30_24_OFFSET                        24
#define D8F0xEC_x4A_Reserved_30_24_WIDTH                         7
#define D8F0xEC_x4A_Reserved_30_24_MASK                          0x7F000000
#define D8F0xEC_x4A_Reserved_31_31_OFFSET                        31
#define D8F0xEC_x4A_Reserved_31_31_WIDTH                         1
#define D8F0xEC_x4A_Reserved_31_31_MASK                          0x80000000

/// D8F0xEC_x4A
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_23_0:24; ///<
    UINT32                                          Reserved_30_24:7 ; ///<
    UINT32                                          Reserved_31_31:1 ; ///<
  } Field;
  UINT32 Value;
} D8F0xEC_x4A_STRUCT;

// **** D8F0xEC_x4C Register Definition ****
// Address
#define D8F0xEC_x4C_ADDRESS                                     0x4C
// Type
#define D8F0xEC_x4C_TYPE                                        TYPE_D8F0xEC

// Field Data
#define D8F0xEC_x4C_Reserved_23_0_OFFSET                         0
#define D8F0xEC_x4C_Reserved_23_0_WIDTH                          24
#define D8F0xEC_x4C_Reserved_23_0_MASK                           0xFFFFFF
#define D8F0xEC_x4C_Reserved_30_24_OFFSET                        24
#define D8F0xEC_x4C_Reserved_30_24_WIDTH                         7
#define D8F0xEC_x4C_Reserved_30_24_MASK                          0x7F000000
#define D8F0xEC_x4C_Reserved_31_31_OFFSET                        31
#define D8F0xEC_x4C_Reserved_31_31_WIDTH                         1
#define D8F0xEC_x4C_Reserved_31_31_MASK                          0x80000000

/// D8F0xEC_x4C
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_23_0:24; ///<
    UINT32                                          Reserved_30_24:7 ; ///<
    UINT32                                          Reserved_31_31:1 ; ///<
  } Field;
  UINT32 Value;
} D8F0xEC_x4C_STRUCT;

#endif
