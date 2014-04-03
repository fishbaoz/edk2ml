//* $NoKeywords:$ */
/**
 * @file
 *
 * CCP SHA Related Functions
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 */
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
 *
 ***************************************************************************/
#include "Tiano.h"
#include "EfiCommonLib.h"
#include "EfiDriverLib.h"
#include "Filecode.h"
#include "PspBaseLib.h"
#include "CcpReg.h"
#include "CcpShaLib.h"

#define FILECODE UEFI_PSP_LIBRARY_CCPSHALIB_CCPSHALIB_FILECODE

#define TIMEOUTLIMIT        2000000
  // DMA PASSTHROUGH MODE
#define DMA_BITWISE_NO_OP       0x00000000ul
#define DMA_BITWISE_AND_256     0x00000001ul
#define DMA_BITWISE_OR_256      0x00000002ul
#define DMA_BITWISE_XOR_256     0x00000003ul
#define DMA_BITWISE_LOAD_256    0x00000004ul

#define DMA_BYTESWAP_NO_OP       0x00000000ul
#define DMA_BYTESWAP_8           0x00000001ul
#define DMA_BYTESWAP_32          0x00000002ul

#define GETCCPError(x)      (x & 0x3Ful);
#define GETCCPJobStatus(x)  ( (x >> 22) & 0x7ul );
#define GETCCPCmdDepth(x)   ( (x >> 12) & 0xFul );

#define CCP_AES_EOM_SHIFT               (1)
#define CCP_AES_KEY_ID_SHIFT            (2)

// S/W limitation on number of outstanding CCP Job.
#define CCP_NUM_JOB_QUEUES          5

// Maximum number of jobs allowed in CCP queue.
#define CCP_QUEUE_DEPTH             8

//
// SW maintained context of CCP queue
//
typedef struct _CCP_JOB_CONTEXT {
  UINT32        Status;                       ///< status of all jobs in the queue
  UINT32        KsbId;                        ///< KSB used
} CCP_JOB_CONTEXT;

typedef struct _CCP_QUEUE_CONTEXT {
  UINT32          QueueId;                      ///< ID for Queue
  CCP_JOB_CONTEXT JobCtx [CCP_QUEUE_DEPTH];     ///< Queue Job list
  UINT32          NextJobId;                    ///< this ID will be used for next job
  UINT32          NextKsbId;                    ///< KSB ID will be used for next job
  UINT32          NumPendingJobs;               ///< number of pending jobs in the queue
} CCP_QUEUE_CONTEXT;


typedef enum _CCP_MEMORY_MODE {
  MEMORY_MODE_DRAM = 0,
  MEMORY_MODE_KSB  = 1,
  MEMORY_MODE_SRAM = 2
} CCP_MEMORY_MODE;


CCP_QUEUE_CONTEXT *gCcpQueueCtx = NULL;
STATIC UINT32 mQueueStatusRegAddr [CCP_NUM_JOB_QUEUES] =
{
  CCPCMD_STATUS_Q0,
  CCPCMD_STATUS_Q1,
  CCPCMD_STATUS_Q2,
  CCPCMD_STATUS_Q3,
  CCPCMD_STATUS_Q4
};

VOID
WriteCcpReg (
  IN       UINT32 RegOff,
  IN       UINT32 Value
  )
{
  EFI_PHYSICAL_ADDRESS PspBar1Addr;
  /// Get PSP Bar1 Address
  GetPspBar1Addr (&PspBar1Addr);
  PspLibMemWrite (AccessWidth32, PspBar1Addr + CCP_MMIO_OFFSET + RegOff, &Value);
}

VOID
ReadCcpReg (
  IN       UINT32 RegOff,
  IN OUT   UINT32 *pValue
  )
{
  EFI_PHYSICAL_ADDRESS PspBar1Addr;
  /// Get PSP Bar1 Address
  GetPspBar1Addr (&PspBar1Addr);
  PspLibMemRead (AccessWidth32, PspBar1Addr + CCP_MMIO_OFFSET + RegOff, pValue);
}

UINT32
GetNextJobId (
  )
{
  UINT32 JobId;

  JobId = gCcpQueueCtx->NextJobId;
  gCcpQueueCtx->JobCtx[JobId].Status = 0;
  gCcpQueueCtx->JobCtx[JobId].KsbId = gCcpQueueCtx->NextKsbId;

  gCcpQueueCtx->NextKsbId++;
  if (gCcpQueueCtx->NextKsbId >= CCP_X86_KEY_ID_END) {
    gCcpQueueCtx->NextKsbId = CCP_X86_KEY_ID_START;
  }

  gCcpQueueCtx->NumPendingJobs++;
  gCcpQueueCtx->NextJobId++;

  if (gCcpQueueCtx->NextJobId >= CCP_QUEUE_DEPTH) {
    gCcpQueueCtx->NextJobId = 0;
  }

  return JobId;
}

EFI_STATUS
PCCP_DeleteJob (
  IN       UINT32 QueueId,
  IN       UINT32 JobId
  )
{
  VOLATILE UINT32       QueueStatus;
  UINT32                JStatus;

  // Checking input
  if ((QueueId >= CCP_NUM_JOB_QUEUES) ||
     (JobId >= CCP_QUEUE_DEPTH)) {
    return EFI_INVALID_PARAMETER;
  }

  // Delete the job from queue.
  WriteCcpReg (CCPDELETE_CMD_QUEUE_JOB,
                MAKE_DELETE_JOB_REG (QueueId, JobId));

  // Wait until Job deletion finishes.
  do
  {
    ReadCcpReg (mQueueStatusRegAddr[QueueId], (UINT32 *)&QueueStatus );
    JStatus = GETCCPJobStatus (QueueStatus);
  } while (JStatus == JSTATUS_ERROR_DELETE_PROCESS);

  return EFI_SUCCESS;
}

EFI_STATUS
WaitForJobDoneOrReturnJobId (
  IN       UINT32  QueueId,
  IN       UINT32  JobId
  )
{
  EFI_STATUS   Status;
  UINT32       QueueStatus;
  UINT32       CcpError;
  UINT32       CcpJobStatus;
  UINT32       LoopCounter;

  QueueStatus  = 0;
  CcpError     = 0;
  CcpJobStatus = 0;
  LoopCounter  = 0;
  Status = EFI_SUCCESS;

  if (QueueId > CCP_NUM_JOB_QUEUES) {
    return EFI_INVALID_PARAMETER;
  }

  do
  {
    ReadCcpReg (mQueueStatusRegAddr [QueueId], (UINT32 *)&QueueStatus);

    CcpError = GETCCPError (QueueStatus);
    CcpJobStatus = GETCCPJobStatus (QueueStatus);

    // This is a special case for CMAC where SOC to copy the output.
    if (CcpJobStatus == 4) {
      CcpJobStatus = 0;
    }

    LoopCounter++;

  } while (((CcpJobStatus != 0) || (CcpError != 0)) &&  (LoopCounter < TIMEOUTLIMIT));

  if (LoopCounter >= TIMEOUTLIMIT) {
    Status = EFI_TIMEOUT;
    DEBUG ((EFI_D_INFO, "CCP CMD Q%d J%d TimeOut\n", QueueId, JobId));
  }

  if ((CcpJobStatus != 0) || (CcpError != 0)) {
    Status = EFI_DEVICE_ERROR;
    DEBUG ((EFI_D_INFO, "CCP CMD Q%d.Sts[%x] J%d.Sts[%x] \n", QueueId, CcpError, JobId, CcpJobStatus));
  }
  // Delete job when it is error
  if (EFI_ERROR (Status)) {
    PCCP_DeleteJob (QueueId, JobId);
  }
  return Status;
}

EFI_STATUS
CcpDMACopy (
  IN       CONST VOID       *pSrc,
  IN OUT   VOID             *pDest,
  IN       CCP_MEMORY_MODE  SrcMemMode,
  IN       CCP_MEMORY_MODE  DestMemMode,
  IN       UINT32           KeyId,
  IN       UINT32           KSBOffset,
  IN       UINT32           ByteSwap,
  IN       UINT32           ByteWise,
  IN       UINT32           Len,
  IN       UINT32           QueueId
  )
{
  EFI_STATUS           Status;
  UINT64               SrcPhyAddr;
  UINT64               DestPhyAddr;
  UINT32               Wfw;
  UINT32               Ionc;
  UINT32               Sonc;
  UINT32               JobId;

  Status          = EFI_SUCCESS;
  if (QueueId >= CCP_NUM_JOB_QUEUES) {
    return EFI_INVALID_PARAMETER;
  }


  SrcPhyAddr = ((UINT64) pSrc);
  DestPhyAddr = ((UINT64) pDest);

  JobId = GetNextJobId ();

  // Rsvc[7:0] BitWise[2:0] ByteSwap[1:0]
  // 000 0000 0101 0000 0000 000 00 000000 0 0
  // engine[3:0] = 5 (pass-through); 7<92>b0, 4<92>h6, 13<92>b0, 6<92>b0, EOM[0], 1<92>b0

  WriteCcpReg (CCP_CPU_CMD_REQ1, DMA_MAKE_CMD_REQ1 (1,  ByteWise, ByteSwap));

  // LenIN[31:0] - 1
  WriteCcpReg ( CCP_CPU_CMD_REQ2, Len - 1 );                     // LenIN[31:0] - 1
  // AddrIN[31:0]

  if (SrcMemMode == MEMORY_MODE_DRAM ||
      SrcMemMode == MEMORY_MODE_SRAM) {
    WriteCcpReg ( CCP_CPU_CMD_REQ3, (UINT32) SrcPhyAddr );          // AddrIN[31:0]
    WriteCcpReg ( CCP_CPU_CMD_REQ4, (KeyId << 18 | MAKE_ADDR_LO ( SrcMemMode, SrcPhyAddr >> 32 ) ));
  } else {
    WriteCcpReg ( CCP_CPU_CMD_REQ3, KeyId * 32);
    WriteCcpReg ( CCP_CPU_CMD_REQ4,  (KeyId << 18 | MAKE_ADDR_LO (SrcMemMode, 0)));
  }

  // AddrOUT[31:0]
  if (DestMemMode == MEMORY_MODE_DRAM ||
      DestMemMode == MEMORY_MODE_SRAM) {
     WriteCcpReg ( CCP_CPU_CMD_REQ5, (UINT32) DestPhyAddr );         // AddrOUT[31:0]
     WriteCcpReg ( CCP_CPU_CMD_REQ6, MAKE_ADDR_LO ( DestMemMode, DestPhyAddr >> 32 ) );
  } else {
    WriteCcpReg (CCP_CPU_CMD_REQ5, KeyId * 32 + KSBOffset);
    WriteCcpReg (CCP_CPU_CMD_REQ6, MAKE_ADDR_LO ( DestMemMode, 0 ));
  }

  Wfw     = 1;    // make sure that all DMA writes are commited before job completed
  Sonc    = 0;    // do not stop on completion
  Ionc    = 0;    // interrupt on completion is required

  WriteCcpReg ( CCP_CPU_CMD_REQ0,            // 20<92>h0, CMDQ[2:0], JOBID[5:0], WFW[0], IONC[0], SONC[0]
                MAKE_CMD_REQ0 ( QueueId, JobId, Wfw, Ionc, Sonc ) );


  // Now wait for job completion or just return job ID to the caller.
  Status = WaitForJobDoneOrReturnJobId ( QueueId, JobId);

  gCcpQueueCtx->NumPendingJobs--;

  return Status;
}


EFI_STATUS
CcpShaKey (
  IN       UINT32 mode,
  IN       UINT32 keyId,
  IN       UINT32 *pIv,
  IN       UINT32 QueueId
  )
{
  UINT32      context[8];
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT32      *tmpAddr;

  if (pIv != NULL) {
    tmpAddr = pIv;
  } else {
    tmpAddr = &context[0];
  }

  if (mode == MODE_SHA_1) {
    context[7] = 0x67452301;
    context[6] = 0xefcdab89;
    context[5] = 0x98badcfe;
    context[4] = 0x10325476;
    context[3] = 0xc3d2e1f0;
    context[2] = 0x0;
    context[1] = 0x0;
    context[0] = 0x0;
  } else if (mode == MODE_SHA_224) {
    context[7] = 0xc1059ed8;
    context[6] = 0x367cd507;
    context[5] = 0x3070dd17;
    context[4] = 0xf70e5939;
    context[3] = 0xffc00b31;
    context[2] = 0x68581511;
    context[1] = 0x64f98fa7;
    context[0] = 0xbefa4fa4;
  } else if (mode == MODE_SHA_256) {
    context[7] = 0x6a09e667;
    context[6] = 0xbb67ae85;
    context[5] = 0x3c6ef372;
    context[4] = 0xa54ff53a;
    context[3] = 0x510e527f;
    context[2] = 0x9b05688c;
    context[1] = 0x1f83d9ab;
    context[0] = 0x5be0cd19;
  } else {
    EfiSetMem (context, 0, sizeof (context));
  }

 Status = CcpDMACopy ( (UINT32 *)tmpAddr,
                      (UINT32 *)tmpAddr,
                      MEMORY_MODE_DRAM,
                      MEMORY_MODE_KSB,
                      keyId,
                      0,
                      DMA_BYTESWAP_NO_OP,
                      DMA_BITWISE_NO_OP,
                      32,
                      QueueId);

  return Status;
}

EFI_STATUS
CcpShaWorker (
  IN       CONST VOID         *pMsg,
  IN       UINT32             MsgLen,
  IN       UINT32             Mode,
  IN       UINT32             EntireMessageLen,
  IN       UINT32             Eom,
  IN       UINT32             Init,
  IN       CCP_HASH_OUTPUT    HashOut
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  EFI_PHYSICAL_ADDRESS  MsgPhyAddr = 0;
  UINT32                Wfw;
  UINT32                Ionc;
  UINT32                Sonc;

  // This needs to be added to the parameter.
  UINT32                MsgSrcMem;
  UINT32                JobId;
  UINT32                KeyId;
  UINT32                QueueId;
  UINT32                Func;
  UINT32                JobType;
  UINT64                MsgLenInBits;

  MsgPhyAddr = ((UINT64) pMsg);
  KeyId      = 0;
  MsgSrcMem = MEMORY_MODE_DRAM;

  ASSERT ((gCcpQueueCtx != NULL) && (HashOut.OutPut != NULL));

  if ((HashOut.OutPut == NULL) || (pMsg == NULL) || (EntireMessageLen == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if ( gCcpQueueCtx->NumPendingJobs >= CCP_QUEUE_DEPTH ) {
    return EFI_OUT_OF_RESOURCES;
  }

  MsgLenInBits = (EntireMessageLen) * 8;

  do
  {
    if ( Mode == MODE_SHA_1 ) {
      // Comments out SHA_1_INIT_STATE
      // Using KSB.
      Func = SHA_1_FUNC_ID;
      JobType = CCP_JOB_TYPE_SHA_1;
    } else if ( Mode == MODE_SHA_224 ) {
      Func = SHA_224_FUNC_ID;
      JobType = CCP_JOB_TYPE_SHA_224;
    } else if ( Mode == MODE_SHA_256 ) {
      // Need to find out whether no write affects the save and
      // restore state. Since for now, SHA after restore does
      // not resume to previous state.
      Func = SHA_256_FUNC_ID;
      JobType = CCP_JOB_TYPE_SHA_256;
    } else {
      Status = EFI_UNSUPPORTED;
      break;
    }

    // KSB ID for this Job will also be init.
    QueueId = gCcpQueueCtx->QueueId;
    JobId = GetNextJobId ();
    KeyId = gCcpQueueCtx->JobCtx[JobId].KsbId;
    //
    // We are setting the default IV only if Init == 1.
    //
    if (Init == 1) {
      Status = CcpShaKey (Mode, KeyId, NULL, QueueId);
      if (Status != 0) {
        break;
      }
    } else {
    //
    // We are setting it based on previous IV value.
    //
    // Only load the ShaCtx if it is specify as a algorithm
    // Otherwise, will use value for KSB.
      Status = CcpShaKey (Mode, KeyId, HashOut.OutPut, QueueId);
      if (Status != 0) {
        break;
      }
    }

    // Force it to 1 on init
    WriteCcpReg ( CCP_CPU_CMD_REQ1, SHA_MAKE_CMD_REQ1 ( Func, KeyId, Eom, 1) );

    WriteCcpReg ( CCP_CPU_CMD_REQ2, MsgLen - 1 );

    WriteCcpReg ( CCP_CPU_CMD_REQ3, (UINT32) MsgPhyAddr );

    // Insert KeyId to CmdReq4 for testing.
    WriteCcpReg ( CCP_CPU_CMD_REQ4, (KeyId << 18 | MAKE_ADDR_LO ( MsgSrcMem, MsgPhyAddr >> 32 )) );

    if (Eom == 1) {
      // The bit length of the Message.
      WriteCcpReg ( CCP_CPU_CMD_REQ5, (UINT32) (MsgLenInBits)  );

      WriteCcpReg ( CCP_CPU_CMD_REQ6, (UINT32) (MsgLenInBits >> 32)  );
    } else {
      WriteCcpReg ( CCP_CPU_CMD_REQ5, 0);

      WriteCcpReg ( CCP_CPU_CMD_REQ6, 0);
    }

    Wfw     = 1;    // make sure that all DMA writes are committed before job completed
    Sonc    = 0;    // stop on completion for SHA jobs
    Ionc    = 0;    // interrupt on completion is required

    WriteCcpReg ( CCP_CPU_CMD_REQ0, MAKE_CMD_REQ0 ( QueueId, JobId, Wfw, Ionc, Sonc ) );

    gCcpQueueCtx->JobCtx[JobId].Status  = (JobType << CCP_JOB_TYPE_SHIFT) + CCP_JOB_ACTIVE;

    // Now wait for job completion or just return job ID to the caller.
    Status = WaitForJobDoneOrReturnJobId ( QueueId, JobId );
  } while (0);


  // Save up the SHA Context
  Status = CcpDMACopy ( (UINT32 *)0,
                         HashOut.OutPut,
                         MEMORY_MODE_KSB,
                         MEMORY_MODE_DRAM,
                         KeyId,
                         0,
                         DMA_BYTESWAP_32,
                         DMA_BITWISE_NO_OP,
                         32,
                         3);


  gCcpQueueCtx->NumPendingJobs--;

  return Status;
}

EFI_STATUS
CcpSha (
  IN       UINT32             ShaMode,
  IN       CONST UINT8        *Message,
  IN       UINT32             MessageSize,
  IN       CCP_HASH_OUTPUT    HashOut
  )
{
  UINT32 Init;
  UINT32 Eom;
  UINT32 QueueMask;
  UINT32 QueueId;

  //Init CCP Context
  if (gCcpQueueCtx == NULL) {
    gCcpQueueCtx = EfiLibAllocateRuntimeZeroPool (sizeof (CCP_QUEUE_CONTEXT));
    ReadCcpReg (CCPCMD_QUEUE_MASK , &QueueMask);
    QueueMask &= 0x1F;
    ASSERT (QueueMask);
    //Considering BIOS is running in single thread, Use MSB of QMR as the contant QID.
    QueueId = 4;
    while (1) {
      if ((1 << QueueId--) & QueueMask) {
        break;
      }
    }

    gCcpQueueCtx->QueueId = QueueId;
    gCcpQueueCtx->NextKsbId = CCP_X86_KEY_ID_START;
  }


  Init = 1;            //Set Init to 0, if Extend set to False;
  Eom = 1;

  CcpShaWorker (
            Message,
            MessageSize,
            ShaMode,
            MessageSize,
            Eom,
            Init,
            HashOut
            );

  return EFI_SUCCESS;
}

