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
 *	This header file contains useful definitions to be used throughout the
 *	Digital Alpha AXP emulation software.
 *
 *	Revision History:
 *
 *	V01.000		10-May-2017	Jonathan D. Belanger
 *	Initially written.
 *
 *	V01.001		14-May-2017	Jonathan D. Belanger
 *	Added includes for a number of standard header files.
 *
 *	V01.002		17-Jun-2017	Jonathan D. Belanger
 *	Added counted queues and counted queue entries.
 */
#ifndef _AXP_UTIL_DEFS_
#define _AXP_UTIL_DEFS_

/*
 * Includes used throughout the code.
 */
//#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Define some regularly utilized definitions.
 */
#define ONE_K				1024
#define FOUR_K				4096
#define EIGHT_K				8192

/*
 * Define some standard data types.
 */
typedef unsigned char		u8;		/* 1 byte (8 bits) in length */
typedef unsigned short		u16;	/* 2 bytes (16 bits) in length */
typedef unsigned int		u32;	/* 4 bytes (32 bits) in length */
typedef unsigned long long	u64;	/* 8 bytes (64 bits) in length */
typedef char				i8;		/* 1 byte (8 bits) in length */
typedef short				i16;	/* 2 bytes (16 bits) in length */
typedef int					i32;	/* 4 bytes (32 bits) in length */
typedef long long			i64;	/* 8 bytes (64 bits) in length */

/*
 * Define a basic queue.  This will be used to define a number of other queue
 * types.
 */
typedef struct
{
	void 			*flink;
	void 			*blink;
} AXP_QUEUE_HDR;

/*
 * Macros to use with the basic queue.
 */
#define AXP_INIT_QUE(queue)		queue.flink = queue.blink = (void *) &queue.flink
#define AXP_INIT_QUEP(queue)	queue->flink = queue->blink = (void *) &queue->flink
#define AXP_QUE_EMPTY(queue)	(queue.flink == (void *) &queue.flink)
#define AXP_QUEP_EMPTY(queue)	(queue->flink == (void *) &queue->flink)

/*
 * A counted queue.  If maximum is specified as zero at initialization, then
 * the number of entries in the queue has no limit.
 */
typedef struct
{
	AXP_QUEUE_HDR	header;
	u32				count;
	u32				max;
} AXP_COUNTED_QUEUE;

typedef struct
{
	AXP_QUEUE_HDR		header;
	AXP_COUNTED_QUEUE	*parent;
} AXP_CQUE_ENTRY;;

#define AXP_INIT_CQUE(queue, maximum)	\
	AXP_INIT_QUE(queue.header);			\
	queue.max = maximum;				\
	queue.count = 0
#define AXP_INIT_CQUEP(queue, maximum)	\
	AXP_INIT_QUE(queue->header);		\
	queue->max = maximum;				\
	queue->count = 0
#define AXP_INIT_CQENTRY(queue, prent)	\
		AXP_INIT_QUE(queue.header);		\
		queue.parent = &prent;
#define AXP_CQUE_EMPTY(queue)	(queue.count == 0)
#define AXP_CQUEP_EMPTY(queue)	(queue->count == 0)
#define AXP_CQUE_FULL(queue)	((queue.maximum !=0 ? (queue.count == queue.maximum) ? false)
#define AXP_CQUEP_FULL(queue)	((queue->maximum !=0 ? (queue->count == queue->maximum) ? false)
#define AXP_CQUE_COUNT(queue)	(queue.count)
#define AXP_CQUEP_COUNT(queue)	(queue->count)

/*
 * Prototype Definitions
 */
void AXP_LRUAdd(AXP_QUEUE_HDR *lruQ, AXP_QUEUE_HDR *entry);
void AXP_LRURemove(AXP_QUEUE_HDR *entry);
AXP_QUEUE_HDR *AXP_LRUReturn(AXP_QUEUE_HDR *lruQ);
i32 AXP_InsertCountedQueue(AXP_QUEUE_HDR *, AXP_CQUE_ENTRY *);
i32 AXP_RemoveCountedQueue(AXP_CQUE_ENTRY *);

#endif /* _AXP_UTIL_DEFS_ */