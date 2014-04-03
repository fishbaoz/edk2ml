/* $NoKeywords:$ */
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
#ifndef _CCP_REGISTERS_H_
#define _CCP_REGISTERS_H_

#define MAKE_ADDR_LO( SecMem, AddrInHi )   ((UINT32) ((SecMem << 16) | ((AddrInHi) & 0x000000FFul)))

#define CCP_X86_KEY_ID_START        77            // first SW key ID in CCP key storage
#define CCP_X86_KEY_ID_END          127           // total 64 keys (first 4 are efused HW keys)

//
// CCP registers
//
#define CCP_MMIO_OFFSET                     0x20000ul
// PspCcpCmd_Queue_Mask
#define CCPCMD_QUEUE_MASK                   0x00000000ul

// PspCcpCmd_Queue_Priority
#define CCPCMD_QUEUE_PRIORITY               0x00000004ul

// PspCcpTrng_Out
#define CCPTRNG_OUT                         0x0000000Cul

// PspCcpCrypto_Interrupt_Enable
#define CCPCRYPTO_INTERRUPT_ENABLE          0x00000040ul

// PspCcpCmd_Timeout_Q0
#define CCPCMD_TIMEOUT_Q0                   0x00000070ul

// PspCcpCmd_Timeout_Q1
#define CCPCMD_TIMEOUT_Q1                   0x00000078ul

// PspCcpCmd_Timeout_Q2
#define CCPCMD_TIMEOUT_Q2                   0x00000080ul

// PspCcpCmd_Timeout_Q3
#define CCPCMD_TIMEOUT_Q3                   0x00000088ul

// PspCcpCmd_Timeout_Q4
#define CCPCMD_TIMEOUT_Q4                   0x00000090ul

// PspCcpCmd_Timeout_Granularity_Q0
#define CCPCMD_TIMEOUT_GRANULARITY_Q0       0x00000074ul

// PspCcpCmd_Timeout_Granularity_Q1
#define CCPCMD_TIMEOUT_GRANULARITY_Q1       0x0000007Cul

// PspCcpCmd_Timeout_Granularity_Q2
#define CCPCMD_TIMEOUT_GRANULARITY_Q2       0x00000084ul

// PspCcpCmd_Timeout_Granularity_Q3
#define CCPCMD_TIMEOUT_GRANULARITY_Q3       0x0000008Cul

// PspCcpCmd_Timeout_Granularity_Q4
#define CCPCMD_TIMEOUT_GRANULARITY_Q4       0x00000094ul

// PspCcpDelete_Cmd_Queue_Job
#define CCPDELETE_CMD_QUEUE_JOB             0x00000124ul

// PspCcp_Cpu_Cmd_Req0
#define CCP_CPU_CMD_REQ0                    0x00000180ul

// PspCcp_Cpu_Cmd_Req1
#define CCP_CPU_CMD_REQ1                    0x00000184ul

// PspCcp_Cpu_Cmd_Req2
#define CCP_CPU_CMD_REQ2                    0x00000188ul

// PspCcp_Cpu_Cmd_Req3
#define CCP_CPU_CMD_REQ3                    0x0000018Cul

// PspCcp_Cpu_Cmd_Req4
#define CCP_CPU_CMD_REQ4                    0x00000190ul

// PspCcp_Cpu_Cmd_Req5
#define CCP_CPU_CMD_REQ5                    0x00000194ul

// PspCcp_Cpu_Cmd_Req6
#define CCP_CPU_CMD_REQ6                    0x00000198ul

// PspCcp_Cpu_Cmd_Req7
#define CCP_CPU_CMD_REQ7                    0x0000019Cul

// PspCcp_Cpu_Cmd_Req8
#define CCP_CPU_CMD_REQ8                    0x000001A0ul

// PspCcp_Cpu_Cmd_Req9
#define CCP_CPU_CMD_REQ9                    0x000001A4ul

// PspCcp_Cpu_Cmd_Req10
#define CCP_CPU_CMD_REQ10                   0x000001A8ul

// PspCcp_Cpu_Cmd_Req11
#define CCP_CPU_CMD_REQ11                   0x000001ACul

// PspCcp_Cpu_Cmd_Req12
#define CCP_CPU_CMD_REQ12                   0x000001B0ul

// PspCcp_Cpu_Cmd_Req13
#define CCP_CPU_CMD_REQ13                   0x000001B4ul

// PspCcp_Cpu_Cmd_Req14
#define CCP_CPU_CMD_REQ14                   0x000001B8ul

// PspCcpCrypto_Interrupt_Status
#define CCPCRYPTO_INTERRUPT_STATUS          0x00000200ul

// PspCcpCrypto_Command_Status
#define CCPCRYPTO_COMMAND_STATUS            0x00000204ul

// PspCcpCmd_Status_Q0
#define CCPCMD_STATUS_Q0                    0x00000210ul

// PspCcpCmd_Status_Q1
#define CCPCMD_STATUS_Q1                    0x00000230ul

// PspCcpCmd_Status_Q2
#define CCPCMD_STATUS_Q2                    0x00000250ul

// PspCcpCmd_Status_Q3
#define CCPCMD_STATUS_Q3                    0x00000270ul

// PspCcpCmd_Status_Q4
#define CCPCMD_STATUS_Q4                    0x00000290ul

// PspCcpCmd_Int_Status_Q0
#define CCPCMD_INT_STATUS_Q0                0x00000214ul

// PspCcpCmd_Int_Status_Q1
#define CCPCMD_INT_STATUS_Q1                0x00000234ul

// PspCcpCmd_Int_Status_Q2
#define CCPCMD_INT_STATUS_Q2                0x00000254ul

// PspCcpCmd_Int_Status_Q3
#define CCPCMD_INT_STATUS_Q3                0x00000274ul

// PspCcpCmd_Int_Status_Q4
#define CCPCMD_INT_STATUS_Q4                0x00000294ul

// PspCcpCmd_Dma_Status_Q0
#define CCPCMD_DMA_STATUS_Q0                0x00000218ul

// PspCcpCmd_Dma_Status_Q1
#define CCPCMD_DMA_STATUS_Q1                0x00000238ul

// PspCcpCmd_Dma_Status_Q2
#define CCPCMD_DMA_STATUS_Q2                0x00000258ul

// PspCcpCmd_Dma_Status_Q3
#define CCPCMD_DMA_STATUS_Q3                0x00000278ul

// PspCcpCmd_Dma_Status_Q4
#define CCPCMD_DMA_STATUS_Q4                0x00000298ul

// PspCcpCmd_Read_Status_Q0
#define CCPCMD_READ_STATUS_Q0               0x0000021Cul

// PspCcpCmd_Read_Status_Q1
#define CCPCMD_READ_STATUS_Q1               0x0000023Cul

// PspCcpCmd_Read_Status_Q2
#define CCPCMD_READ_STATUS_Q2               0x0000025Cul

// PspCcpCmd_Read_Status_Q3
#define CCPCMD_READ_STATUS_Q3               0x0000027Cul

// PspCcpCmd_Read_Status_Q4
#define CCPCMD_READ_STATUS_Q4               0x0000029Cul

// PspCcpCmd_Write_Status_Q0
#define CCPCMD_WRITE_STATUS_Q0              0x00000220ul

// PspCcpCmd_Write_Status_Q1
#define CCPCMD_WRITE_STATUS_Q1              0x00000240ul

// PspCcpCmd_Write_Status_Q2
#define CCPCMD_WRITE_STATUS_Q2              0x00000260ul

// PspCcpCmd_Write_Status_Q3
#define CCPCMD_WRITE_STATUS_Q3              0x00000280ul

// PspCcpCmd_Write_Status_Q4
#define CCPCMD_WRITE_STATUS_Q4              0x000002A0ul

// PspCcpAES_Context0W0
#define CCPAES_CONTEXT0W0                   0x00000400ul

// PspCcpAES_Context1W0
#define CCPAES_CONTEXT1W0                   0x00000420ul

// PspCcpAES_Context2W0
#define CCPAES_CONTEXT2W0                   0x00000440ul

// PspCcpAES_Context3W0
#define CCPAES_CONTEXT3W0                   0x00000460ul

// PspCcpAES_Context4W0
#define CCPAES_CONTEXT4W0                   0x00000480ul

// PspCcpAES_Context0W1
#define CCPAES_CONTEXT0W1                   0x00000404ul

// PspCcpAES_Context1W1
#define CCPAES_CONTEXT1W1                   0x00000424ul

// PspCcpAES_Context2W1
#define CCPAES_CONTEXT2W1                   0x00000444ul

// PspCcpAES_Context3W1
#define CCPAES_CONTEXT3W1                   0x00000464ul

// PspCcpAES_Context4W1
#define CCPAES_CONTEXT4W1                   0x00000484ul

// PspCcpAES_Context0W2
#define CCPAES_CONTEXT0W2                   0x00000408ul

// PspCcpAES_Context1W2
#define CCPAES_CONTEXT1W2                   0x00000428ul

// PspCcpAES_Context2W2
#define CCPAES_CONTEXT2W2                   0x00000448ul

// PspCcpAES_Context3W2
#define CCPAES_CONTEXT3W2                   0x00000468ul

// PspCcpAES_Context4W2
#define CCPAES_CONTEXT4W2                   0x00000488ul

// PspCcpAES_Context0W3
#define CCPAES_CONTEXT0W3                   0x0000040Cul

// PspCcpAES_Context1W3
#define CCPAES_CONTEXT1W3                   0x0000042Cul

// PspCcpAES_Context2W3
#define CCPAES_CONTEXT2W3                   0x0000044Cul

// PspCcpAES_Context3W3
#define CCPAES_CONTEXT3W3                   0x0000046Cul

// PspCcpAES_Context4W3
#define CCPAES_CONTEXT4W3                   0x0000048Cul

// PspCcpConfig0
#define CCPCONFIG0                          0x00000800ul

// PspCcpEflc_Status
#define CCPEFLC_STATUS                      0x00000804ul

// PspCcpTrng_Control
#define CCPTRNG_CONTROL                     0x00000808ul

// PspCcpAesMaskWrite
#define CCPAESMASKWRITE                     0x00000810ul

// PspCcpDecompression_Limit_Lo
#define CCPDECOMPRESSION_LIMIT_LO           0x00000820ul

// PspCcpDecompression_Limit_Hi
#define CCPDECOMPRESSION_LIMIT_HI           0x00000824ul

// PspCcpDecompression_Status_Lo
#define CCPDECOMPRESSION_STATUS_LO          0x00000828ul

// PspCcpDecompression_Status_Hi
#define CCPDECOMPRESSION_STATUS_HI          0x0000082Cul

// PspCcpLoad_Sw_Crypto_Key0
#define CCPLOAD_SW_CRYPTO_KEY0              0x00000900ul

// PspCcpLoad_Sw_Crypto_Key1
#define CCPLOAD_SW_CRYPTO_KEY1              0x00000904ul

// PspCcpLoad_Sw_Crypto_Key2
#define CCPLOAD_SW_CRYPTO_KEY2              0x00000908ul

// PspCcpLoad_Sw_Crypto_Key3
#define CCPLOAD_SW_CRYPTO_KEY3              0x0000090Cul

// PspCcpLoad_Sw_Crypto_Key4
#define CCPLOAD_SW_CRYPTO_KEY4              0x00000910ul

// PspCcpLoad_Sw_Crypto_Key5
#define CCPLOAD_SW_CRYPTO_KEY5              0x00000914ul

// PspCcpLoad_Sw_Crypto_Key6
#define CCPLOAD_SW_CRYPTO_KEY6              0x00000918ul

// PspCcpLoad_Sw_Crypto_Key7
#define CCPLOAD_SW_CRYPTO_KEY7              0x0000091Cul

// PspCcpUpdate_Sw_Crypto_Key
#define CCPUPDATE_SW_CRYPTO_KEY             0x00000920ul

// PspCcpCmd_Req0
#define CCPCMD_REQ0                         0x00000980ul

// PspCcpCmd_Req1
#define CCPCMD_REQ1                         0x00000984ul

// PspCcpCmd_Req2
#define CCPCMD_REQ2                         0x00000988ul

// PspCcpCmd_Req3
#define CCPCMD_REQ3                         0x0000098Cul

// PspCcpCmd_Req4
#define CCPCMD_REQ4                         0x00000990ul

// PspCcpCmd_Req5
#define CCPCMD_REQ5                         0x00000994ul

// PspCcpCmd_Req6
#define CCPCMD_REQ6                         0x00000998ul

// PspCcpCmd_Req7
#define CCPCMD_REQ7                         0x0000099Cul

// PspCcpCmd_Req8
#define CCPCMD_REQ8                         0x000009A0ul

// PspCcpCmd_Req9
#define CCPCMD_REQ9                         0x000009A4ul

// PspCcpCmd_Req10
#define CCPCMD_REQ10                        0x000009A8ul

// PspCcpCmd_Req11
#define CCPCMD_REQ11                        0x000009ACul

// PspCcpCmd_Req12
#define CCPCMD_REQ12                        0x000009B0ul

// PspCcpCmd_Req13
#define CCPCMD_REQ13                        0x000009B4ul

// PspCcpCmd_Req14
#define CCPCMD_REQ14                        0x000009B8ul


//MEMORY MODE REGISTER
#define MMODE_EXTERNAL_MEMORY_DRAM      0
#define MMODE_INTERNAL_KEY_STORAGE_BLK  1
#define MMODE_EXTERNAL_MEMORY_SRAM      2

// CMD_STATUS register definitions
//
#define CMD_STATUS_ERROR_SHIFT      (0)
#define CMD_STATUS_PWRITES_SHIFT    (7)
#define CMD_STATUS_PREADS_SHIFT     (13)
#define CMD_STATUS_DSTATUS_SHIFT    (19)
#define CMD_STATUS_JOBID_SHIFT      (21)
#define CMD_STATUS_DEPTH_SHIFT      (27)
#define CMD_STATUS_JSTATUS_SHIFT    (30)

#define CMD_STATUS_ERROR_MASK       0x0000007Ful
#define CMD_STATUS_PWRITES_MASK     0x00001F80ul
#define CMD_STATUS_PREADS_MASK      0x0007E000ul
#define CMD_STATUS_DSTATUS_MASK     0x00180000ul
#define CMD_STATUS_JOBID_MASK       0x07E00000ul
#define CMD_STATUS_DEPTH_MASK       0x38000000ul
#define CMD_STATUS_JSTATUS_MASK     0xC0000000ul

// CMD_INT_STATUS register definitions
//
#define CMD_INT_STATUS_EVENT_CNT_SHIFT  (0)
#define CMD_INT_STATUS_JOBID_SHIFT      (7)

#define CMD_INT_STATUS_EVENT_CNT_MASK   0x0000007Ful
#define CMD_INT_STATUS_JOBID_MASK       0x00001F80ul

// CMD_STATUS_Qx register definitions
//
#define CMD_STATUS_Q_ERROR_SHIFT        (0)
#define CMD_STATUS_Q_KEY_DEPTH_SHIFT    (7)
#define CMD_STATUS_Q_CMD_DEPTH_SHIFT    (12)
#define CMD_STATUS_Q_JOBID_SHIFT        (16)
#define CMD_STATUS_Q_JSTATUS_SHIFT      (22)

#define CMD_STATUS_Q_ERROR_MASK         0x0000007Ful
#define CMD_STATUS_Q_KEY_DEPTH_MASK     0x00000F80ul
#define CMD_STATUS_Q_CMD_DEPTH_MASK     0x0000F000ul
#define CMD_STATUS_Q_JOBID_MASK         0x003F0000ul
#define CMD_STATUS_Q_JSTATUS_MASK       0x01C00000ul

// Definitions for the JStatus field of CMD_STATUS_Qx register
//
#define JSTATUS_IDLE                    0x00
#define JSTATUS_INIT                    0x01
#define JSTATUS_PENDING                 0x02
#define JSTATUS_ACTIVE                  0x03
#define JSTATUS_SUSPENDED               0x04
#define JSTATUS_ERROR_DELETE_PROCESS    0x05
#define JSTATUS_KEY_DELETE              0x06

// CMD_UPDATE_SW_CRYPTO_KEY register definitions
//
#define CMD_UPDATE_KEY_FIELD_SHIFT      (0)
#define CMD_UPDATE_KEY_ID_SHIFT         (3)

#define CMD_UPDATE_KEY_FIELD_MASK       0x00000007ul
#define CMD_UPDATE_KEY_ID_MASK          0x000001F8ul


// CCP error types
// Command Errors
#define CCP_ERR_NONE                               0x00000000ul
#define CCP_ERR_ILLEGAL_ENGINE                     0x00000001ul
#define CCP_ERR_ILLEGAL_KEY_ID                     0x00000002ul
#define CCP_ERR_ILLEGAL_FUNCTION_TYPE              0x00000003ul
#define CCP_ERR_ILLEGAL_FUNCTION_MODE              0x00000004ul
#define CCP_ERR_ILLEGAL_FUNCTION_ENCRYPT           0x00000005ul
#define CCP_ERR_ILLEGAL_FUNCTION_SIZE              0x00000006ul
#define CCP_ERR_ZLIB_WFW_WITHOUT_EOM               0x00000007ul
#define CCP_ERR_ILLEGAL_FUNCTION_RSVD              0x00000008ul
#define CCP_ERR_ILLEGAL_BUFFER_LENGTH              0x00000009ul
#define CCP_ERR_BUF_ADR_OVERLAP                    0x0000000Aul
#define CCP_ERR_ILLEGAL_MEM_ADDR                   0x0000000Bul
#define CCP_ERR_ILLEGAL_MEM_SEL                    0x0000000Cul
#define CCP_ERR_ILLEGAL_CONTEXT_ID                 0x0000000Dul
#define CCP_ERR_ZLIB_ILLEGAL_MULTI_QUEUE           0x00000010ul
#define CCP_ERR_ZLIB_ILLEGAL_JOBID_CHANGE          0x00000011ul
#define CCP_ERR_CMD_TIMEOUT                        0x00000012ul
#define CCP_ERR_IDMA0_AXI_SLAVE_FAULT              0x00000013ul
#define CCP_ERR_IDMA1_AXI_SLAVE_FAULT              0x00000016ul
#define CCP_ERR_ZLIBVHB_AXI_SLAVE_FAULT            0x00000019ul
#define CCP_ERR_ZLIB_UNEXPECTED_EOM                0x0000001Cul
#define CCP_ERR_ZLIB_EXTRA_DATA                    0x0000001Dul
#define CCP_ERR_ZLIB_BTYPE                         0x0000001Eul
#define CCP_ERR_ZLIB_UNDEFINED_SYMBOL              0x0000001Ful
#define CCP_ERR_ZLIB_UNDEFINED_DISTANCE_SYMBOL     0x00000020ul
#define CCP_ERR_ZLIB_CODE_LENGHT_SYMBOL            0x00000021ul
#define CCP_ERR_ZLIB_VHB_ILLEGAL_FETCH             0x00000022ul
#define CCP_ERR_ZLIB_UNCOMPRESSED_LEN              0x00000023ul
#define CCP_ERR_ZLIB_DECOMPRESS_LIMIT_REACHED      0x00000024ul
#define CCP_ERR_ZLIB_CHECKSUM_MISMATCH             0x00000025ul
#define CCP_ERR_ZLIB_ODMA_AXI_SLAVE_FAULT          0x00000026ul
#define CCP_ERR_ZLIB_AXI_SALVE_FAULT               0x00000029ul


// Job status values
#define CCP_JOB_COMPLETED           0
#define CCP_JOB_PENDING             1
#define CCP_JOB_ACTIVE              2
#define CCP_JOB_DELETED             3

#define CCP_JOB_HW_STATUS_MASK      0x0000000Ful
#define CCP_JOB_TYPE_MASK           0x000000F0ul
#define CCP_JOB_OUT_SIZE_MASK       0x00FFFF00ul
#define CCP_JOB_ERROR_MASK          0xFF000000ul

#define CCP_JOB_TYPE_SHIFT          (4)
#define CCP_JOB_OUT_SIZE_SHIFT      (8)
#define CCP_JOB_ERROR_SHIFT         (24)

#define CCP_JOB_TYPE_DMA            0       // DMA job
#define CCP_JOB_TYPE_AES_CTX        1       // AES job with SONC = 1
#define CCP_JOB_TYPE_AES_LAST       2       // AES job with SONC = 0
#define CCP_JOB_TYPE_SHA_1          5       // SHA-1 job
#define CCP_JOB_TYPE_SHA_224        6       // SHA-224 job
#define CCP_JOB_TYPE_SHA_256        7       // SHA-256 job
#define CCP_JOB_TYPE_RSA            8       // RSA job
#define CCP_JOB_TYPE_ECC            9       // ECC job
#define CCP_JOB_TYPE_TRNG           10      // TRNG job


#define MAKE_CMD_REQ0( CmdQueue, JobId, Wfw, Ionc, Sonc )   \
\
    ( (CmdQueue & 0x7) << 9 ) |     \
    ( (JobId & 0x3F) << 3 )   |     \
    ( (Wfw & 0x1) << 2 )      |     \
    ( (Ionc & 0x1) << 1 )     |     \
    ( Sonc & 0x1 )

// Reserved[22:0] Active[0] Queue[2:0] JobId[5:0]
#define MAKE_DELETE_JOB_REG( Queue, JobId )     ((Queue & 0x07) << 6) | (JobId & 0x3F)


// SHA function IDs
//
#define SHA_1_FUNC_ID                   0xC00
#define SHA_224_FUNC_ID                 0x1400
#define SHA_256_FUNC_ID                 0x1C00
#define SHA_256_NO_WRITE_FUNC_ID        0x1800

#define CCP_SHA_FUNC_ID_SHIFT           (10)

// DMA function IDs
#define CCP_DMA_FUNC_ID_SHIFT           (10)
#define CCP_DMA_BYTESWAP_SHIFT          (12)
#define CCP_DMA_EOM_SHIFT               (1)

// DMA Func[12:0]
// Rsvd[7:0], BitWise[2:0], ByteSwap[1:0]
#define DMA_MAKE_CMD_REQ1(Eom, BitWise, ByteSwap )  \
    ( \
      0x2800000ul |                                 \
      ( ByteSwap << CCP_DMA_FUNC_ID_SHIFT ) |     \
      ( BitWise << CCP_DMA_BYTESWAP_SHIFT)  |     \
      ( Eom << CCP_DMA_EOM_SHIFT) \
    )



// FUNC[12:0], Key_Id[5:0], EOM[0], INIT[0]
#define SHA_MAKE_CMD_REQ1( Func, KeyId, EOM, INIT )           \
    0x1800000ul |                            \
    ( KeyId << CCP_AES_KEY_ID_SHIFT )  |   \
    ( EOM   << CCP_AES_EOM_SHIFT)      |   \
    ( Func << CCP_SHA_FUNC_ID_SHIFT )  |   \
    (INIT)




#endif        // #ifndef _CCP_H

