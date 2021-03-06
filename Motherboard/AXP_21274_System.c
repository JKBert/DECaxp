/*
 * Copyright (C) Jonathan D. Belanger 2018-2019.
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
 *  This source file the System interface functions.
 *
 * Revision History:
 *
 *  V01.000 21-JAN-2018	Jonathan D. Belanger
 *  Initially written.
 */
#include "CommonUtilities/AXP_Configure.h"
#include "CommonUtilities/AXP_Utility.h"
#include "CommonUtilities/AXP_Blocks.h"
#include "Motherboard/AXP_21274_System.h"
#include "Motherboard/Cchip/AXP_21274_Cchip.h"
#include "Motherboard/Dchip/AXP_21274_Dchip.h"
#include "Motherboard/Pchip/AXP_21274_Pchip.h"
#include "Motherboard/AXP_21274_InitRoutines.h"

AXP_21274_SYSTEM *AXP_21274_AllocateSystem(void)
{
    AXP_21274_SYSTEM *sys;
    void *cpu[AXP_21274_MAX_CPUS];
    int pthreadRet;
    int ii;
    bool qRet = true;

    sys = AXP_Allocate_Block(AXP_21274_SYS_BLK);
    if (sys != NULL)
    {
        pthreadRet = pthread_mutex_init(&sys->cChipMutex, NULL);
        if (pthreadRet == 0)
        {
            pthreadRet = pthread_mutex_init(&sys->memMutex, NULL);
        }
        if (pthreadRet == 0)
        {
            pthreadRet = pthread_mutex_init(&sys->p0.mutex, NULL);
        }
        if (pthreadRet == 0)
        {
            pthreadRet = pthread_mutex_init(&sys->p1.mutex, NULL);
        }
        if (pthreadRet == 0)
        {
            pthreadRet = pthread_cond_init(&sys->cChipCond, NULL);
        }
        if (pthreadRet == 0)
        {
            pthreadRet = pthread_cond_init(&sys->p0.cond, NULL);
        }
        if (pthreadRet == 0)
        {
            pthreadRet = pthread_cond_init(&sys->p1.cond, NULL);
        }

        /*
        * Let's go allocate all the CPUs configured to this emulation
        */
        if (pthreadRet == 0)
        {
            sys->cpuCount = AXP_ConfigGet_CPUCount();
            for (ii = 0; ii < AXP_21274_MAX_CPUS; ii++)
            {

                /*
                 * Go and allocate a CPU.
                 */
                if (ii < sys->cpuCount)
                {
                    cpu[ii] = AXP_21264_AllocateCPU(ii);
                    if (cpu[ii] != NULL)
                    {

                        /*
                         * Use the CPU ID as an entry into the CPU array to
                         * initialize the information needed for the System to
                         * be able to communicate with the CPU.
                         */
                        AXP_21264_Save_SystemInterfaces(cpu[ii],
                                                        &sys->cpu[ii].mutex,
                                                        &sys->cpu[ii].cond,
                                                        (void **) &sys->cpu[ii].pq,
                                                        &sys->cpu[ii].pqTop,
                                                        &sys->cpu[ii].pqBottom,
                                                        &sys->cpu[ii].irq_H,
                                                        &sys->cChipMutex,
                                                        &sys->cChipCond,
                                                        &sys->skidBufferQ);
                    }
                    else
                    {
                        qRet = false;
                    }
                }
                else
                {
                    cpu[ii] = NULL;
                }
            }
        }

        /*
         * If things are still going well, go get the size and number of memory
         * arrays and then allocate the memory accordingly.
         */
        if (qRet == true)
        {
            AXP_ConfigGet_DarrayInfo(&sys->arrayCount, &sys->arraySizes);

            /*
             * Now that we know the sizes, go and allocate the individual
             * arrays.  Each array contains a contiguous memory address space.
             */
            for (ii = 0; ii < AXP_21274_MAX_ARRAYS; ii++)
            {
                if (ii < sys->arrayCount)
                {
                    sys->array[ii] = AXP_Allocate_Block(-sys->arraySizes,
                                                        sys->array[ii]);
                }
                else
                {
                    sys->array[ii] = NULL;
                }
            }
        }

        /*
         * If we are, thus far, successful, time to initialize the rest of the
         * system and then create the System threads.
         */
        if (qRet == true)
        {

            /*
             * First, go initialize the rest of the system.
             */
            AXP_21274_CchipInit(sys);
            AXP_21274_DchipInit(sys);
            AXP_21274_PchipInit(&sys->p0, 0);
            AXP_21274_PchipInit(&sys->p1, 1);
            pthreadRet = pthread_create(&sys->cChipThreadID,
                                        NULL,
                                        AXP_21274_CchipMain,
                                        sys);
            if (pthreadRet == 0)
            {
                pthreadRet = pthread_create(&sys->p0.threadID,
                                            NULL,
                                            AXP_21274_PchipMain,
                                            sys);
            }
            if (pthreadRet == 0)
            {
                pthreadRet = pthread_create(&sys->p1.threadID,
                                            NULL,
                                            AXP_21274_PchipMain,
                                            sys);
            }
        }
    }

    /*
     * If something failed, then deallocate everything.
     */
    if ((sys != NULL) &&
        ((pthreadRet != 0) ||
         (qRet != true) ||
         (sys->cpuCount == 0) ||
         (sys->arrayCount == 0)))
    {
        for (ii = 0; ii < AXP_21274_MAX_CPUS; ii++)
        {
            if (cpu[ii] != NULL)
            {
                AXP_Deallocate_Block(cpu[ii]);
            }
        }
        AXP_Deallocate_Block(sys);
        sys = NULL;
    }

    /*
     * Return what we allocated back to the caller
     */
    return (sys);
}
