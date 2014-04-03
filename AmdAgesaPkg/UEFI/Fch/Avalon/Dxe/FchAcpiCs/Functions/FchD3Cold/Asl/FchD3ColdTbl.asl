#define FchInitMidUsbEhci1                        F000
#define FchInitMidUsbEhci2                        F001
#define FchInitMidUsbEhci3                        F002
#define FchAcpiEnableEhciGlobalClockGating        F003
#define FchUserDefineEhciSsid                     F004
#define FchUsbMsiEnable                           F005
#define FchEhci1PhyPort0                          F006
#define FchEhci1PhyPort1                          F007
#define FchEhci1PhyPort2                          F008
#define FchEhci1PhyPort3                          F009
#define FchEhci2PhyPort0                          F00A
#define FchEhci2PhyPort1                          F00B
#define FchEhci2PhyPort2                          F00C
#define FchEhci2PhyPort3                          F00D
#define FchEhci3PhyPort0                          F00E
#define FchEhci3PhyPort1                          F00F
#define FchEhci3PhyPort2                          F010
#define FchEhci3PhyPort3                          F011
#define FchEhci1Enable                            F012
#define FchEhci2Enable                            F013
#define FchEhci3Enable                            F014
#define FchXhci0PhyPort0                          F015
#define FchXhci0PhyPort1                          F016

// Sub-Procedure
#define FchEhci1PhySetting                        F020
#define FchCheckUtmiB17ActiveEhci1                F021
#define FchHsslewSettingEhci1                     F022
#define FchEhci2PhySetting                        F023
#define FchCheckUtmiB17ActiveEhci2                F024
#define FchHsslewSettingEhci2                     F025
#define FchEhci3PhySetting                        F026
#define FchCheckUtmiB17ActiveEhci3                F027
#define FchHsslewSettingEhci3                     F028

// BAR define
#define FchEhci1BarSpace                          F080
#define FchEhci2BarSpace                          F081
#define FchEhci3BarSpace                          F082
#define FchEhci1Bar                               F083
#define FchEhci2Bar                               F084
#define FchEhci3Bar                               F085
#define FchEhciPhyBar1                            F086
#define FchEhciUtmiActBar1                        F087
#define FchEhciHsslewBar1                         F088
#define FchEhciPhyBar2                            F089
#define FchEhciUtmiActBar2                        F08A
#define FchEhciHsslewBar2                         F08B
#define FchEhciPhyBar3                            F08C
#define FchEhciUtmiActBar3                        F08D
#define FchEhciHsslewBar3                         F08E
#define FchXhciEnable                             F08F

// PCI Registers
#define FchEhciSsidRegister1                      F100
#define FchEhciMiscControl1                       F101
#define FchEhciSpare1                             F102
#define FchEhciSsidRegister2                      F103
#define FchEhciMiscControl2                       F104
#define FchEhciSpare2                             F105
#define FchEhciSsidRegister3                      F106
#define FchEhciMiscControl3                       F107
#define FchEhciSpare3                             F108

// BAR Registers
#define FchEhciPacketBufferThreshold1             F200
#define FchEhciPhyStatus1                         F201
#define FchEhciSimControl1                        F202
#define FchEhciUtmiControl1                       F203
#define FchEhciEorMiscControl1                    F204
#define FchEhciCommonPhyCal1                      F205
#define FchEhciPhyControlReg1_1                   F206
#define FchEhciPhyControlReg1_2                   F207
#define FchEhciPhyControlReg1_3                   F208
#define FchEhciPhyControl1                        F209      // Same as FchEhciUtmiControl1
#define FchEhciUtmiActControl1                    F20A      // Same as FchEhciUtmiControl1
#define FchEhciHsslewControl1                     F20B      // Same as FchEhciUtmiControl1

#define FchEhciPacketBufferThreshold2             F300
#define FchEhciPhyStatus2                         F301
#define FchEhciSimControl2                        F302
#define FchEhciUtmiControl2                       F303
#define FchEhciEorMiscControl2                    F304
#define FchEhciCommonPhyCal2                      F305
#define FchEhciPhyControlReg2_1                   F306
#define FchEhciPhyControlReg2_2                   F307
#define FchEhciPhyControlReg2_3                   F308
#define FchEhciPhyControl2                        F309      // Same as FchEhciUtmiControl1
#define FchEhciUtmiActControl2                    F30A      // Same as FchEhciUtmiControl1
#define FchEhciHsslewControl2                     F30B      // Same as FchEhciUtmiControl1

#define FchEhciPacketBufferThreshold3             F400
#define FchEhciPhyStatus3                         F401
#define FchEhciSimControl3                        F402
#define FchEhciUtmiControl3                       F403
#define FchEhciEorMiscControl3                    F404
#define FchEhciCommonPhyCal3                      F405
#define FchEhciPhyControlReg3_1                   F406
#define FchEhciPhyControlReg3_2                   F407
#define FchEhciPhyControlReg3_3                   F408
#define FchEhciPhyControl3                        F409      // Same as FchEhciUtmiControl1
#define FchEhciUtmiActControl3                    F40A      // Same as FchEhciUtmiControl1
#define FchEhciHsslewControl3                     F40B      // Same as FchEhciUtmiControl1


#define DeviceEnterD3Hot                          FA00
#define DeviceEnterD3Cold                         FA01
#define DeviceEnterD0                             FA02
#define FchS0I3ReadyCheck                         FA03
#define GetTargetedDeviceState                    FA04

#define FchAOAC                                   FCSR
#define FchDeviceControlBase                      FC40
#define FchDeviceControl                          FDCR
#define TargetedDeviceState                       FD00
#define DeviceState                               FD02
#define PwrOnDev                                  FD03
#define SwPwrOnRstB                               FD04
#define SwRefClkOk                                FD05
#define SwRstB                                    FD06
#define IsSwControl                               FD07
#define FchS0I3Ready                              FC94
#define FchS0I3Ready01                            FD10


#define DeviceControlClkGen                       00
#define DeviceControlAB                           01
#define DeviceControlAcpiS0                       02
#define DeviceControlAcpiS5                       03
#define DeviceControlLPC                          04
#define DeviceControlAZ                           14
#define DeviceControlSata                         15
#define DeviceControlEhci1                        18
#define DeviceControlEhci2                        19
#define DeviceControlEhci3                        20
#define DeviceControlXhci                         23
#define DeviceControlSd                           24



DefinitionBlock (
	 "FchD3ColdTbl.aml",   // Output Filename
	 "SSDT",      // Signature
	  0x01,       // DSDT Compliance Revision
	 "AMDFCH",    // OEMID
	 "FCHCSD3",   // TABLE ID
	 0x1000       // OEM Revision
  )
{
  External(IO80, IntObj)
  External(\_SB.USBB, IntObj)
  External(\_SB.SABB, IntObj)
  External(\_SB.SDBB, IntObj)
  External(\_SB.GPO0, IntObj)
  External(\_SB.PCI0, DeviceObj)
  External(\_SB.PCI0.EHC1, DeviceObj)
  External(\_SB.PCI0.EHC2, DeviceObj)
  External(\_SB.PCI0.EHC3, DeviceObj)
  External(\_SB.PCI0.XHC0, DeviceObj)
  External(\_SB.PCI0.XHC1, DeviceObj)
  External(\_SB.PCI0.SATA, DeviceObj)
  External(\_SB.PCI0.SDIO, DeviceObj)
  External(\_SB.SMBA, DeviceObj)
  External(\_SB.STHT, MethodObj)

  OperationRegion (SMIP, SystemIO, 0xB0, 0x2)
  Field (SMIP, WordAcc, NoLock, Preserve)
  {
      SMPC, 16
//      SMPD, 8
  }

  OperationRegion( FchAOAC, SystemMemory, 0xFED81E00, 0x100)
  Field( FchAOAC, ByteAcc, NoLock, Preserve) {
    //Offset (0x40),
    //FchDeviceControlBase, 8,
    Offset (0x94),
    FchS0I3Ready01,       1,
    }
  
  Method (GetTargetedDeviceState, 1) {
    Store ( 0xfed81e40, Local0 )
    ShiftLeft(Arg0, 1, Local1)
    Add(Local0, Local1, Local0)
      //Store ( 0xfed81e01, Local0 ) //for test on KB
    OperationRegion( FchDeviceControl, SystemMemory, Local0, 0x2)
    Field( FchDeviceControl, ByteAcc, NoLock, Preserve) {
      TargetedDeviceState,  2,
    }

    Store(TargetedDeviceState, Local0)
    Return(Local0)
  }

  Method (DeviceEnterD0, 1) {
    Store ( 0xfed81e40, Local0 )
    ShiftLeft(Arg0, 1, Local1)
    Add(Local0, Local1, Local0)
      //Store ( 0xfed81e01, Local0 ) //for test on KB
    OperationRegion( FchDeviceControl, SystemMemory, Local0, 0x2)
    Field( FchDeviceControl, ByteAcc, NoLock, Preserve) {
      TargetedDeviceState,  2,
    }

    Store(1, TargetedDeviceState)
  }
  
  Method (DeviceEnterD3Hot, 1) {
    Store ( 0xfed81e40, Local0 )
    ShiftLeft(Arg0, 1, Local1)
    Add(Local0, Local1, Local0)
      //Store ( 0xfed81e01, Local0 ) //for test on KB
    OperationRegion( FchDeviceControl, SystemMemory, Local0, 0x2)
    Field( FchDeviceControl, ByteAcc, NoLock, Preserve) {
      TargetedDeviceState,  2,
    }

    Store(2, TargetedDeviceState)
  }
  
  Method (DeviceEnterD3Cold, 1) {
    Store ( 0xfed81e40, Local0 )
    ShiftLeft(Arg0, 1, Local1)
    Add(Local0, Local1, Local0)
      //Store ( 0xfed81e01, Local0 ) //for test on KB
    OperationRegion( FchDeviceControl, SystemMemory, Local0, 0x2)
    Field( FchDeviceControl, ByteAcc, NoLock, Preserve) {
      TargetedDeviceState,  2,
      DeviceState,          1,
      PwrOnDev,             1,
      SwPwrOnRstB,          1,
      SwRefClkOk,           1,
      SwRstB,               1,
      IsSwControl,          1,
    }
    
    Store(0, PwrOnDev) 
    Store(3, TargetedDeviceState)  
  }
  
  Method (FchS0I3ReadyCheck, 0) {
    If (LEqual(GetTargetedDeviceState(DeviceControlSata), 3)) {
      If (LEqual(GetTargetedDeviceState(DeviceControlEhci1), 3)) {
        If (LEqual(GetTargetedDeviceState(DeviceControlEhci2), 3)) {
          If (LEqual(GetTargetedDeviceState(DeviceControlEhci3), 3)) {
            If (LEqual(GetTargetedDeviceState(DeviceControlXhci), 3)) {
              If (LEqual(GetTargetedDeviceState(DeviceControlSd), 3)) {
                Store(1, FchS0I3Ready01)
                \_SB.STHT ()  //+++ to call platform method for over-all flag
                Return
              }
            }
          }
        }
      }
    }
    Store(0, FchS0I3Ready01)
    \_SB.STHT ()  //+++to call platform method for over-all flag
  }
  
  #include "Ehci.asi"
  #include "Xhci.asi"
  #include "Sata.asi"
  #include "Sd.asi"
}
