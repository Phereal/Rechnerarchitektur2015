/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

/* This file is designed for use with ISim build 0xfbc00daa */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "/home/steffen/Dokumente/Rechnerarchitektur2015-master/Uebung 2/memory/memory.vhd";



static void work_a_3392787015_2882221765_p_0(char *t0)
{
    char *t1;

LAB0:    t1 = (t0 + 7656);
    *((int *)t1) = 1;

LAB1:    return;
}

static void work_a_3392787015_2882221765_p_1(char *t0)
{
    char *t1;

LAB0:    t1 = (t0 + 7672);
    *((int *)t1) = 1;

LAB1:    return;
}

static void work_a_3392787015_2882221765_p_2(char *t0)
{
    char *t1;

LAB0:    t1 = (t0 + 7688);
    *((int *)t1) = 1;

LAB1:    return;
}

static void work_a_3392787015_2882221765_p_3(char *t0)
{
    char *t1;

LAB0:    t1 = (t0 + 7704);
    *((int *)t1) = 1;

LAB1:    return;
}

static void work_a_3392787015_2882221765_p_4(char *t0)
{
    char *t1;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;

LAB0:    xsi_set_current_line(178, ng0);
    t1 = (t0 + 14720);
    t3 = (t0 + 7800);
    t4 = (t3 + 56U);
    t5 = *((char **)t4);
    t6 = (t5 + 56U);
    t7 = *((char **)t6);
    memcpy(t7, t1, 8U);
    xsi_driver_first_trans_fast_port(t3);
    t1 = (t0 + 7720);
    *((int *)t1) = 1;

LAB1:    return;
}


extern void work_a_3392787015_2882221765_init()
{
	static char *pe[] = {(void *)work_a_3392787015_2882221765_p_0,(void *)work_a_3392787015_2882221765_p_1,(void *)work_a_3392787015_2882221765_p_2,(void *)work_a_3392787015_2882221765_p_3,(void *)work_a_3392787015_2882221765_p_4};
	xsi_register_didat("work_a_3392787015_2882221765", "isim/memoryTB_isim_beh.exe.sim/work/a_3392787015_2882221765.didat");
	xsi_register_executes(pe);
}
