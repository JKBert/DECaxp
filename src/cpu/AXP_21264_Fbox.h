/*
 * Copyright (C) Jonathan D. Belanger 2017.
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
 *	This header file contains the function definitions implemented in the
 *	AXP_21264_Fbox.c module.
 *
 * Revision History:
 *
 *	V01.000		19-Jun-2017	Jonathan D. Belanger
 *	Initially written.
 */
#ifndef _AXP_21264_FBOX_DEFS_
#define _AXP_21264_FBOX_DEFS_

#include "AXP_21264_CPU.h"

/*
 * FP Operate Function Field Format
 * 	The formatted function field for FP operations is only done for Opcodes
 * 	0x14 and 0x16.
 */
typedef struct
{
	 u32	fnc : 4;
	 u32	src : 2;
	 u32	rnd : 2;
	 u32	trp : 3;
	 u32	res : 21;
} AXP_FP_FUNC;

#endif /* _AXP_21264_FBOX_DEFS_ */
