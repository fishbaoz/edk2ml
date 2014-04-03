/* $NoKeywords:$ */
/**
 * @file
 *
 * SMU Bios Table
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
*****************************************************************************
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
#ifndef _GNBSMUBIOSTABLEML_H_
#define _GNBSMUBIOSTABLEML_H_

#define SMU7_MAX_LEVELS_GIO    8

#pragma pack (push, 1)

/// SMU BIOS Table structures

/// SMU GIO Level
typedef struct {
  UINT8        VoltageDownHyst;             ///< VoltageDownHyst
  UINT8        Vid;                         ///< Vid code for MinVddNb
  UINT8        LclkDid;                     ///< LclkDid
  UINT8        EnabledForActivity;          ///< EnabledForActivity

  UINT32       MinVddNb;                    ///< MinVddNb

  UINT8        DownHyst;                    ///< DownHyst
  UINT8        UpHyst;                      ///< UpHyst
  UINT16       ResidencyCounter;            ///< ResidencyCounter

  UINT32       LclkFrequency;               ///< LclkFrequency

  UINT8        Padding;                     ///< Padding
  UINT8        ClkBypassCntl;               ///< ClkBypassCntl
  UINT8        EnabledForThrottle;          ///< EnabledForThrottle
  UINT8        ActivityLevel;               ///< ActivityLevel
} SMU7_FUSIONGIOLEVEL;

/// SMU PID Controller
typedef struct {
  UINT32        Ki;                              ///< Ki
  INT32         LFWindupUpperLim;                ///< LFWindupUpperLim
  INT32         LFWindupLowerLim;                ///< LFWindupLowerLim
  UINT32        StatePrecision;                  ///< StatePrecision

  UINT32        LfPrecision;                     ///< LfPrecision
  UINT32        LfOffset;                        ///< LfOffset
  UINT32        MaxState;                        ///< MaxState
  UINT32        MaxLfFraction;                   ///< MaxLfFraction
  UINT32        StateShift;                      ///< StateShift
} SMU7_PIDCONTROLLER;

/// SMU GIO Dpm Table
typedef struct {
  SMU7_FUSIONGIOLEVEL        GIOLevel [SMU7_MAX_LEVELS_GIO];     ///<
  SMU7_PIDCONTROLLER         GIOPIDController;                   ///< GIOPIDController
  UINT32        GIOLevelCount;                              ///< GIOLevelCount
  UINT8         Padding;                                    ///< Padding
  UINT8         GIOBootLevel;                               ///< GIOBootLevel
  UINT8         GIOVoltageChangeEnable;                     ///< GIOVoltageChangeEnable
  UINT8         Enable;                                     ///< Enable
  UINT32        Reserved;                                   ///< Reserved
  UINT8         ThermThrottleEnable;                        ///< ThermThrottleEnable
  UINT8         ThermThrottleTempSelect;                    ///< ThermThrottleTempSelect
  UINT8         Reserved1;                                  ///< Reserved1
  UINT8         ThrottleOnHtc;                              ///< ThrottleOnHtc
  UINT16        TemperatureLimitLow;                        ///< TemperatureLimitLow
  UINT16        TemperatureLimitHigh;                       ///< TemperatureLimitHigh
} SMU7_FUSIONGIODPMTABLE;

/// SMU Port 80 Table
typedef struct {
  UINT32        MmioAddress;                ///< MmioAddress      //DEF=0xFED80E44;
  UINT32        MemoryBaseHi;               ///< MemoryBaseHi
  UINT32        MemoryBaseLo;               ///< MemoryBaseLo
  UINT16        MemoryPosition;             ///< MemoryPosition
  UINT16        MemoryBufferSize;           ///< MemoryBufferSize
  UINT8         EnableDramShadow;           ///< EnableDramShadow
  UINT8         EnableCsrShadow;            ///< EnableCsrShadow
  UINT16        PollingInterval;            ///< PollingInterval  //DEF=1
} SMU7_PORT80MONITORTABLE;

/// SMU Bios Table
typedef struct {
  UINT16        TdcVrmLimit_IddNb;              ///< 10mA units
  UINT16        TdcVrmLimit_Idd;                ///< 10mA units

  UINT16        IddNbspike_OCP;                 ///< max IDDNB current for SVI2 current calculations
  UINT16        Iddspike_OCP;                   ///< max IDD current for SVI2 current calculations

  UINT8         PcieLanesActiveMap;             ///<
  UINT8         DimmsConfig;                    ///<
  UINT8         Dimms;                          ///< number of DIMMs connected
  UINT8         SVI1;                           ///< set to 1 if ysstem is SVI1

  UINT16        Vddio;                          ///< voltage Q1.15
  UINT16        Vddr;                           ///< voltages Q1.15

  UINT16        MemorySpeedLow;                 ///< MemorySpeedLow (MHz)
  UINT16        MemorySpeedHigh;                ///< MemorySpeedHigh (MHz)

  UINT32        TdpDcUndocked;                  ///<
  UINT32        TdpAcUndocked;                  ///<
  UINT32        TdpDcDocked;                    ///<
  UINT32        TdpAcDocked;                    ///<

  UINT8         StapmEnableAcDocked;            ///<
  UINT8         StapmEnableDcDocked;            ///<
  UINT8         StapmEnableAcUndocked;          ///<
  UINT8         StapmEnableDcUndocked;          ///<

  UINT32        StapmPowerDcUndocked;           ///<
  UINT32        StapmPowerAcUndocked;           ///<
  UINT32        StapmPowerDcDocked;             ///<
  UINT32        StapmPowerAcDocked;             ///<

  UINT8         Reserved2;                      ///<
  UINT8         LowPowerRecMode_mp1;            ///<
  UINT8         LowPowerRecMode_mp0;            ///<
  UINT8         DllBypassMode;                  ///<

  UINT32        NumGfxCUsEnabled;               ///<
  UINT32        BatteryBoostTune;               ///<
  UINT32        TspAcDocked;                    ///<
  UINT32        TspDcDocked;                    ///<
  UINT32        TspAcUndocked;                  ///<
  UINT32        TspDcUndocked;                  ///<
  UINT32        Reserved[2];                    ///<

  UINT16        spare;                          ///<
  UINT16        TemperatureLimit;               ///<

  SMU7_PORT80MONITORTABLE      Port80MonitorTable;   ///< SMU Port 80 Table
  SMU7_FUSIONGIODPMTABLE       GioDpmTable;          ///< Gio Dpm Table
} SMU7_BIOSTABLE;

#pragma pack (pop)


VOID
GnbInitSmuBiosTableML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  );

AGESA_STATUS
GnbWriteSmuBiosTableML (
  IN       AMD_CONFIG_PARAMS        *StdHeader
  );

VOID
GnbSmuBiosTableGetSystemConfigInfoML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  );

VOID
GnbSmuBiosTableGetDimmInfoML (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  );

VOID
GnbSmuBiosTableGetPcieInfoML (
  IN      AMD_CONFIG_PARAMS               *StdHeader,
  IN      PCIe_PLATFORM_CONFIG            *Pcie
  );

#endif
