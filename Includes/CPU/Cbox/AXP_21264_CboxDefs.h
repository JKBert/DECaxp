/*
 * Copyright (C) Jonathan D. Belanger 2017-2018.
 * All Rights Reserved.
 *
 * This software is furnished under a license and may be used and copied only
 * in accordance with the terms of such license and with the inclusion of the
 * above copyright notice.  This software or any other copies thereof may not
 * be provided or otherwise made available to any other person.  No title to
 * and ownership of the software is hereby transferred.
 *
 * The information in this software is subject to change without notice and
 * should not be construed as a commitment by the author or co-authors.
 *
 * The author and any co-authors assume no responsibility for the use or
 * reliability of this software.
 *
 * Description:
 *
 *  This header file contains the function definitions implemented in the
 *  AXP_21264_Cbox.c module.
 *
 * Revision History:
 *
 *  V01.000 14-May-2017 Jonathan D. Belanger
 *  Initially written.
 *
 *  V01.001 12-Oct-2017 Jonathan D. Belanger
 *  Change the CTAG structure to contain the DTAG index for the corresponding
 *  entry.  This should make keeping these 2 arrays, along with the Dcache
 *  itself, in synch.  NOTE: The set information is the same for all.
 *
 *  V01.002 14-Oct-2017 Jonathan D. Belanger
 *  Started defining the CPU to System and System to CPU messages.
 *
 *  V01.003 22-Oct-2017 Jonathan D. Belanger
 *  Differentiated between messages that come over the SysAddIn_L[14:0] pins
 *  and SysAddOut_L[14:0] pins.  Also defined a number of the remaining
 *  structures needed by the Cbox.
 *
 *  V01.004 06-Nov-2017 Jonathan D. Belanger
 *  Split this fine into 2 separate files.  One to include the definitions for
 *  the Cbox CSRs and one to be included by AXP_21264_Cbox.c so that it will
 *  compile cleanly.  This file is the one with the CSRs define within it.
 *
 *  V01.005 16-Nov-2017 Jonathan D. Belanger
 *  Added code to manage the Bcache.  We still need to figure out how to do
 *  the write-though to memory when updating a location in the Bcache.
 *
 *  V01.006 22-Dec-2017 Jonathan D. Belanger
 *  We need a merge length a store length in each IOWB.  The store length is
 *  used to determine how big the data was for the store that allocated the
 *  IOWB.  This is used to make sure we merge like sizes only.
 *
 *  V01.007 31-Mar-2018 Jonathan D. Belanger
 *  Move the definitions out of this header file that need to be common between
 *  the CPU and System emulations.  This is the requests and responses sent
 *  between the 2 emulations.  These definitions have to be maintained so that
 *  they are identical, except in name.  This way the CPU does not have to
 *  include all the System header files and the System the CPU header files.
 */
#ifndef _AXP_21264_CBOX_DEFS_
#define _AXP_21264_CBOX_DEFS_

#include "CommonUtilities/AXP_Utility.h"
#include "CommonUtilities/AXP_Configure.h"
#include "CPU/Cbox/SystemInterface/AXP_21264_21274_Common.h"

/*
 * HRM Tables 5-23 and 5-24
 * CBox CSRs
 */
typedef struct
{
    u64 BcBankEnable :1;
    u64 BcBurstModeEnable :1;
    u64 BcCleanVictim :1;
    u64 BcClkfwdEnable :1;
    u64 BcClockOut :1;
    u64 BcDdmFallEn :1;
    u64 BcDdmfEnable :1;
    u64 BcDdmrEnable :1;
    u64 BcDdmRiseEn :1;
    u64 BcEnable :1;
    u64 BcFrmClk :1;
    u64 BcLateWriteUpper :1;
    u64 BcPentiumMode :1;
    u64 BcRdRdBubble :1;
    u64 BcRdvictim :1;
    u64 BcSjBankEnable :1;
    u64 BcTagDdmFallEn :1;
    u64 BcTagDdmRiseEn :1;
    u64 BcWrWrBubble :1;
    u64 ThirtyTwoByteIo :1;
    u64 DupTagEnable :1;
    u64 EnableEvict :1;
    u64 EnableProbeCheck :1;
    u64 EnableStcCommand :1;
    u64 FastModeDisable :1;
    u64 InitMode :1;
    u64 JitterCmd :1;
    u64 MboxBcPrbStall :1;
    u64 PrbTagOnly :1;
    u64 RdvicAckInhibit :1;
    u64 SkewedFillMode :1;
    u64 SpecReadEnable :1;
    u64 StcEnable :1;
    u64 SysbusFormat :1;
    u64 SysbusMbEnable :1;
    u64 SysClkfwdEnable :1;
    u64 SysDdmFallEn :1;
    u64 SysDdmfEnable :1;
    u64 SysDdmrEnable :1;
    u64 SysDdmRdFallEn :1;
    u64 SysDdmRdRiseEn :1;
    u64 SysDdmRiseEn :1;
    u64 BcClkDelay :2;
    u64 BcCpuClkDelay :2;
    u64 BcCpuLateWriteNum :2;
    u64 BcRcvMuxCntPreset :2;
    u64 CfrFrmclkDelay :2;
    u64 DataValidDly :2;
    u64 InvalToDirty :2;
    u64 InvalToDirtyEnable :2;
    u64 SysBusSize :2;
    u64 SysClkDelay :2;
    u64 res_1 :2; /* Quadword align */
    u64 SysCpuClkDelay :2;
    u64 SysRcvMuxCntPreset :2;
    u64 SysRcvMuxPreset :2;
    u64 BcLateWriteNum :3;
    u64 CfrEv6clkDelay :3;
    u64 SetDirtyEnable :3;
    u64 SysbusVicLimit :3;
    u64 BcBphaseLdVector :4;
    u64 BcSize :4;
    u64 BcWrRdBubbles :4;
    u64 BcWrtSts :4;
    u64 CfrGclkDelay :4;
    u64 MbCnt :4;
    u64 SysBphaseLdVector :4;
    u64 SysdcDelay :4;
    u64 SysbusAckLimit :5;
    u64 SysClkRatio :5;
    u64 res_2 :4; /* Quadword align */
    u32 SysFrameLdVector :5;
    u32 BcRdWrBubbles :6;
    u32 res_3 :21; /* Longword Align */
    u32 BcLatTagPattern :24;
    u32 res_4 :8; /* Quadword align */
    u8 BcFdbkEn;
    u8 DcvicThreshold;
    u8 SysFdbkEn;
    u8 res_5; /* Longword align */
    u16 BcClkLdVector;
    u16 SysClkLdVector;
    u32 BcLatDataPattern;
    u32 res_6; /* Quadword align */
} AXP_21264_CBOX_CSRS;

#define AXP_21264_CBOX_CSR_CNT 78

/*
 * MB definitions.
 */
#define AXP_21264_1MB   (1 * ONE_M)
#define AXP_21264_2MB   (2 * ONE_M)
#define AXP_21264_4MB   (4 * ONE_M)
#define AXP_21264_8MB   (8 * ONE_M)
#define AXP_21264_16MB  (16 * ONE_M)

typedef enum
{
    BcBankEnable,
    BcBurstModeEnable,
    BcCleanVictim,
    BcClkfwdEnable,
    BcClockOut,
    BcDdmFallEn,
    BcDdmfEnable,
    BcDdmrEnable,
    BcDdmRiseEn,
    BcEnable,
    BcFrmClk,
    BcLateWriteUpper,
    BcPentiumMode,
    BcRdRdBubble,
    BcRdvictim,
    BcSjBankEnable,
    BcTagDdmFallEn,
    BcTagDdmRiseEn,
    BcWrWrBubble,
    ThirtyTwoByteIo,
    DupTagEnable,
    EnableEvict,
    EnableProbeCheck,
    EnableStcCommand,
    FastModeDisable,
    InitMode,
    JitterCmd,
    MboxBcPrbStall,
    PrbTagOnly,
    RdvicAckInhibit,
    SkewedFillMode,
    SpecReadEnable,
    StcEnable,
    SysbusFormat,
    SysbusMbEnable,
    SysClkfwdEnable,
    SysDdmFallEn,
    SysDdmfEnable,
    SysDdmrEnable,
    SysDdmRdFallEn,
    SysDdmRdRiseEn,
    SysDdmRiseEn,
    BcClkDelay,
    BcCpuClkDelay,
    BcCpuLateWriteNum,
    BcRcvMuxCntPreset,
    CfrFrmclkDelay,
    DataValidDly,
    InvalToDirty1,
    InvalToDirtyEnable,
    SysBusSize,
    SysClkDelay,
    SysCpuClkDelay,
    SysRcvMuxCntPreset,
    SysRcvMuxPreset,
    BcLateWriteNum,
    CfrEv6clkDelay,
    SetDirtyEnable,
    SysbusVicLimit,
    BcBphaseLdVector,
    BcSize,
    BcWrRdBubbles,
    BcWrtSts,
    CfrGclkDelay,
    MbCnt,
    SysBphaseLdVector,
    SysdcDelay,
    SysbusAckLimit,
    SysClkRatio,
    SysFrameLdVector,
    BcRdWrBubbles,
    BcLatTagPattern,
    BcFdbkEn,
    DcvicThreshold,
    SysFdbkEn,
    BcClkLdVector,
    SysClkLdVector,
    BcLatDataPattern,
    LastCSR
} AXP_21264_CBOX_CSR_VAL;

typedef struct
{
    const char *name;
    const AXP_21264_CBOX_CSR_VAL values;
} AXP_21264_CBOX_CSR_NAMES;
/*
 * HRM Table 4-15
 * INVALID_TO_DIRTY_ENABLE values.
 */
#define AXP_WH64_CVT_RDMODX00       0    /* b'x0' */
#define AXP_WH64_CVT_RDMODX10       2    /* b'x0' */
#define AXP_WH64_ENA_INT_ACK        1    /* b'01' */
#define AXP_WH64_ENA_INT_INV2DIR    3    /* b'11' */

/*
 * HRM Table 4-16
 * SET_DIRTY_ENABLE values.
 *
 * This is a mask with the following meaning (when set):
 *       bit 0:    Only Clean Blocks generate external ACK - CleanToDirty
 *       bit 1:    Only Clean/Shared Blocks generate external ACK - SharedToDirty
 *       bit 2:    Only Dirty/Shared Blocks generate external ACK - SharedToDirty
 */
#define AXP_ACK_INTERNALLY          0    /* b'000' Uniprocessor */
#define AXP_CLEAN_ACK_ONLY          1    /* b'001' CleanToDirty */
#define AXP_CLEAN_SHARED_ACK_ONLY   2    /* b'010' SharedToDirty */
#define AXP_DIRTY_SHARED_ACK_ONLY   4    /* b'100' SharedToDirty */

/*
 * HRM Table 4-42
 * BC_SIZE value when BC_ENABLE is set.
 *
 * An easy calculation is the value of BC_SIZE + 1 in MB.
 */
#define    AXP_BCACHE_1MB           0                    /* b000000 */
#define    AXP_BCACHE_2MB           1                    /* b000001 */
#define    AXP_BCACHE_4MB           3                    /* b000011 */
#define    AXP_BCACHE_8MB           7                    /* b000111 */
#define    AXP_BCACHE_16MB          15                   /* b001111 */

/*
 * Mask bits for the offset, index, and tag information for the Bcache.  The
 * size of the index bits is dependent upon the size of the cache.  The offset
 * and tag bits are always the same.
 */
#define AXP_BCACHE_OFF_BITS     0x000000000000003fll    /* bits  5 -  0 */
#define AXP_BCACHE_TAG_BITS     0x00000fffffffffffll    /* bits 63 - 20 */
#define AXP_BCACHE_IDX_FILL     0x0000000000003fffll
#define AXP_BCACHE_IDX_SHIFT    6
#define AXP_BCACHE_TAG_SHIFT    20

/*
 * HRM Section 4.7.7
 * There are 2 commands to be sent from the System to the CPU.  These are
 * they.
 */
#define AXP_SYSTEM_DATA_TRANSFER    false
#define AXP_SYSTEM_PROBE            true

/*
 * HRM Tables 4-21 and 4-22
 * Probe Command
 */
#define AXP_21264_DM_NOP            0    /* b'00' */
#define AXP_21264_DM_RDHIT          1    /* b'01' */
#define AXP_21264_DM_RDDIRTY        2    /* b'10' */
#define AXP_21264_DM_RDANY          3    /* b'00' */

#define AXP_21264_NS_NOP            0    /* b'000' */
#define AXP_21264_NS_CLEAN          1    /* b'001' */
#define AXP_21264_NS_CLEAN_SHARED   2    /* b'010' */
#define AXP_21264_NS_TRANS3         3    /* b'011' */
#define AXP_21264_NS_DIRTY_SHARED   4    /* b'100' */
#define AXP_21264_NS_INVALID        5    /* b'101' */
#define AXP_21264_NS_TRANS1         6    /* b'110' */
#define AXP_21264_NS_RES            7    /* b'111' */

#define AXP_21264_GET_PROBE_DM(probe)   (((probe) & 0x18) >> 3)
#define AXP_21264_GET_PROBE_NS(probe)   ((probe) & 0x07)
#define AXP_21264_SET_PROBE(dm, ns)     ((((dm) & 0x03) << 3) || ((ns) & 0x07))

#define AXP_21264_SIZE_LONG         32
#define AXP_21264_SIZE_QUAD         64

/*
 * sysData size in quadwords
 *
 * NOTE:    This definition must match with the AXP_21274_DATA_SIZE definition
 *          in the system/AXP_21274_to_CPU.h and the AXP_21264_DATA_SIZE
 *          definition in the cpu/AXP_21264_CboxDefs.h.
 */
#define AXP_21264_DATA_SIZE         8

/*
 * HRM 2.1.4.1
 * This structure is the definition for one of the Victim Address File (VAF)
 * and Victim Data File (VDF).  Together these structures form an 8-entry
 * victim buffer used for holding:
 *
 *       Dcache blocks to be written to the Bcache
 *       Istream cache blocks from memory to the Bcache
 *       Bcache blocks to be written to memory
 *       Cache blocks sent to the system in response to probe commands
 */
typedef enum
{
    toBcache,
    toMemory,
    probeResponse
} AXP_21264_VDB_TYPE;

typedef struct
{
    u64 sysData[AXP_21264_DATA_SIZE];
    u64 pa;
    AXP_21264_VDB_TYPE type;
    bool validVictim;
    bool validProbe;
    bool processed;
    bool valid;
    bool marked;
} AXP_21264_CBOX_VIC_BUF;

/*
 * In HRM Section 2, the MAF is documented as being in the Mbox/Memory
 * Reference Unit.  In HRM Section 4.1.1.1, it states the following:
 *
 *      The Cbox contains an 8-entry miss buffer (MAF) and an 8-entry victim
 *      buffer (VAF).
 */
typedef enum
{
    MAFNotInUse,
    LDx,
    STx,
    STx_C,
    STxChangeToDirty,
    STxCChangeToDirty,
    WH64,
    ECB,
    Istream,
    MemoryBarrier
} AXP_CBOX_MAF_TYPE;

#define AXP_21264_MBOX_MAX        8
typedef struct
{
    AXP_CBOX_MAF_TYPE type;
    u64 pa;
    u64 mask;
    i8 lqSqEntry[AXP_21264_MBOX_MAX];
    int dataLen;
    int bufLen;
    bool valid;
    bool complete; /* cleared by Mbox, set by Cbox */
    bool shared;
    bool ioReq;
} AXP_21264_CBOX_MAF;

/*
 * HRM 2.12
 * Each IOWB entry has the following:
 *       64 bytes of data
 *       physical address
 *       control logic    (TBD)
 */
typedef struct
{
    u64 sysData[AXP_21264_DATA_SIZE];
    u64 pa;
    int bufLen;
    bool processed;
    bool valid;
    u8 mask;
    u8 storeLen;
    i8 lqSqEntry[AXP_21264_MBOX_MAX];
} AXP_21264_CBOX_IOWB;

#define AXP_IOWB_ID_MASK        0x08
#define AXP_21264_IOWB_ID(id)   (((id) & AXP_IOWB_ID_MASK) == AXP_IOWB_ID_MASK)
#define AXP_MASK_ID(id)         ((id) & ~AXP_IOWB_ID_MASK)

/*
 * This structure is the definition for one Cbox copy of Dcache Tag Array.  A
 * block contains the following:
 *
 *      Physical tag bits
 *      Index and set into corresponding DTAG array (which is the same as the
 *          Dcache)
 *      Valid bit - the CTAG array entry is in use.
 */
typedef struct
{
    u64 physTag;
    u32 dtagIndex;
    bool valid;
    bool dirty;
    bool shared;
} AXP_21264_CBOX_CTAG;

/*
 * Bcache definitions
 */
#define AXP_BCACHE_BLOCK_SIZE    64
typedef u8 AXP_21264_BCACHE_BLK[AXP_BCACHE_BLOCK_SIZE];
typedef struct
{
    u64 tag;
    u64 pa;
    bool valid;
    bool dirty;
    bool shared;
} AXP_21264_BCACHE_TAG;

#endif /* _AXP_21264_CBOX_DEFS_ */
