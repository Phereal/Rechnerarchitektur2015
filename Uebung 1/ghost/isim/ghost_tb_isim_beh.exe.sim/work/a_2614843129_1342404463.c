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
static const char *ng0 = "/home/stefan/Rechnerarchitektur2015/ghost/ghost.vhd";
extern char *IEEE_P_2592010699;

unsigned char ieee_p_2592010699_sub_2763492388968962707_503743352(char *, char *, unsigned int , unsigned int );


static void work_a_2614843129_1342404463_p_0(char *t0)
{
    char *t1;
    unsigned char t2;
    char *t3;
    char *t4;
    unsigned char t5;
    unsigned char t6;
    char *t7;
    int t8;
    int t9;
    char *t10;
    char *t11;
    char *t12;
    char *t13;
    unsigned char t14;
    unsigned char t15;
    unsigned char t16;

LAB0:    xsi_set_current_line(31, ng0);
    t1 = (t0 + 992U);
    t2 = ieee_p_2592010699_sub_2763492388968962707_503743352(IEEE_P_2592010699, t1, 0U, 0U);
    if (t2 != 0)
        goto LAB2;

LAB4:
LAB3:    t1 = (t0 + 3872);
    *((int *)t1) = 1;

LAB1:    return;
LAB2:    xsi_set_current_line(33, ng0);
    t3 = (t0 + 1192U);
    t4 = *((char **)t3);
    t5 = *((unsigned char *)t4);
    t6 = (t5 == (unsigned char)3);
    if (t6 != 0)
        goto LAB5;

LAB7:
LAB6:    xsi_set_current_line(50, ng0);
    t1 = (t0 + 1192U);
    t3 = *((char **)t1);
    t2 = *((unsigned char *)t3);
    t5 = (t2 == (unsigned char)2);
    if (t5 != 0)
        goto LAB23;

LAB25:
LAB24:    goto LAB3;

LAB5:    xsi_set_current_line(34, ng0);
    t3 = (t0 + 1992U);
    t7 = *((char **)t3);
    t8 = *((int *)t7);
    t9 = (t8 + 1);
    t3 = (t0 + 3968);
    t10 = (t3 + 56U);
    t11 = *((char **)t10);
    t12 = (t11 + 56U);
    t13 = *((char **)t12);
    *((int *)t13) = t9;
    xsi_driver_first_trans_fast(t3);
    xsi_set_current_line(37, ng0);
    t1 = (t0 + 1672U);
    t3 = *((char **)t1);
    t5 = *((unsigned char *)t3);
    t6 = (t5 == (unsigned char)2);
    if (t6 == 1)
        goto LAB11;

LAB12:    t2 = (unsigned char)0;

LAB13:    if (t2 != 0)
        goto LAB8;

LAB10:
LAB9:    xsi_set_current_line(43, ng0);
    t1 = (t0 + 1672U);
    t3 = *((char **)t1);
    t5 = *((unsigned char *)t3);
    t6 = (t5 == (unsigned char)3);
    if (t6 == 1)
        goto LAB17;

LAB18:    t2 = (unsigned char)0;

LAB19:    if (t2 != 0)
        goto LAB14;

LAB16:
LAB15:    goto LAB6;

LAB8:    xsi_set_current_line(38, ng0);
    t1 = (t0 + 3968);
    t7 = (t1 + 56U);
    t10 = *((char **)t7);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((int *)t12) = 1;
    xsi_driver_first_trans_fast(t1);
    xsi_set_current_line(39, ng0);
    t1 = (t0 + 4032);
    t3 = (t1 + 56U);
    t4 = *((char **)t3);
    t7 = (t4 + 56U);
    t10 = *((char **)t7);
    *((unsigned char *)t10) = (unsigned char)3;
    xsi_driver_first_trans_fast(t1);
    goto LAB9;

LAB11:    t1 = (t0 + 1832U);
    t4 = *((char **)t1);
    t14 = *((unsigned char *)t4);
    t15 = (t14 == (unsigned char)2);
    t2 = t15;
    goto LAB13;

LAB14:    xsi_set_current_line(44, ng0);
    t1 = (t0 + 1992U);
    t7 = *((char **)t1);
    t8 = *((int *)t7);
    t16 = (t8 == 3);
    if (t16 != 0)
        goto LAB20;

LAB22:
LAB21:    goto LAB15;

LAB17:    t1 = (t0 + 1832U);
    t4 = *((char **)t1);
    t14 = *((unsigned char *)t4);
    t15 = (t14 == (unsigned char)2);
    t2 = t15;
    goto LAB19;

LAB20:    xsi_set_current_line(45, ng0);
    t1 = (t0 + 4096);
    t10 = (t1 + 56U);
    t11 = *((char **)t10);
    t12 = (t11 + 56U);
    t13 = *((char **)t12);
    *((unsigned char *)t13) = (unsigned char)3;
    xsi_driver_first_trans_fast(t1);
    goto LAB21;

LAB23:    xsi_set_current_line(51, ng0);
    t1 = (t0 + 2152U);
    t4 = *((char **)t1);
    t8 = *((int *)t4);
    t9 = (t8 + 1);
    t1 = (t0 + 4160);
    t7 = (t1 + 56U);
    t10 = *((char **)t7);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((int *)t12) = t9;
    xsi_driver_first_trans_fast(t1);
    xsi_set_current_line(53, ng0);
    t1 = (t0 + 1672U);
    t3 = *((char **)t1);
    t5 = *((unsigned char *)t3);
    t6 = (t5 == (unsigned char)3);
    if (t6 == 1)
        goto LAB29;

LAB30:    t2 = (unsigned char)0;

LAB31:    if (t2 != 0)
        goto LAB26;

LAB28:
LAB27:    xsi_set_current_line(59, ng0);
    t1 = (t0 + 1672U);
    t3 = *((char **)t1);
    t5 = *((unsigned char *)t3);
    t6 = (t5 == (unsigned char)2);
    if (t6 == 1)
        goto LAB38;

LAB39:    t2 = (unsigned char)0;

LAB40:    if (t2 != 0)
        goto LAB35;

LAB37:
LAB36:    xsi_set_current_line(65, ng0);
    t1 = (t0 + 1672U);
    t3 = *((char **)t1);
    t5 = *((unsigned char *)t3);
    t6 = (t5 == (unsigned char)2);
    if (t6 == 1)
        goto LAB47;

LAB48:    t2 = (unsigned char)0;

LAB49:    if (t2 != 0)
        goto LAB44;

LAB46:
LAB45:    goto LAB24;

LAB26:    xsi_set_current_line(54, ng0);
    t1 = (t0 + 2152U);
    t7 = *((char **)t1);
    t8 = *((int *)t7);
    t16 = (t8 == 10);
    if (t16 != 0)
        goto LAB32;

LAB34:
LAB33:    goto LAB27;

LAB29:    t1 = (t0 + 1832U);
    t4 = *((char **)t1);
    t14 = *((unsigned char *)t4);
    t15 = (t14 == (unsigned char)3);
    t2 = t15;
    goto LAB31;

LAB32:    xsi_set_current_line(55, ng0);
    t1 = (t0 + 4032);
    t10 = (t1 + 56U);
    t11 = *((char **)t10);
    t12 = (t11 + 56U);
    t13 = *((char **)t12);
    *((unsigned char *)t13) = (unsigned char)2;
    xsi_driver_first_trans_fast(t1);
    goto LAB33;

LAB35:    xsi_set_current_line(60, ng0);
    t1 = (t0 + 2152U);
    t7 = *((char **)t1);
    t8 = *((int *)t7);
    t16 = (t8 == 30);
    if (t16 != 0)
        goto LAB41;

LAB43:
LAB42:    goto LAB36;

LAB38:    t1 = (t0 + 1832U);
    t4 = *((char **)t1);
    t14 = *((unsigned char *)t4);
    t15 = (t14 == (unsigned char)3);
    t2 = t15;
    goto LAB40;

LAB41:    xsi_set_current_line(61, ng0);
    t1 = (t0 + 4096);
    t10 = (t1 + 56U);
    t11 = *((char **)t10);
    t12 = (t11 + 56U);
    t13 = *((char **)t12);
    *((unsigned char *)t13) = (unsigned char)2;
    xsi_driver_first_trans_fast(t1);
    goto LAB42;

LAB44:    xsi_set_current_line(66, ng0);
    t1 = (t0 + 4160);
    t7 = (t1 + 56U);
    t10 = *((char **)t7);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((int *)t12) = 0;
    xsi_driver_first_trans_fast(t1);
    goto LAB45;

LAB47:    t1 = (t0 + 1832U);
    t4 = *((char **)t1);
    t14 = *((unsigned char *)t4);
    t15 = (t14 == (unsigned char)2);
    t2 = t15;
    goto LAB49;

}

static void work_a_2614843129_1342404463_p_1(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;

LAB0:    xsi_set_current_line(75, ng0);
    t1 = (t0 + 1672U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 4224);
    t4 = (t1 + 56U);
    t5 = *((char **)t4);
    t6 = (t5 + 56U);
    t7 = *((char **)t6);
    *((unsigned char *)t7) = t3;
    xsi_driver_first_trans_fast_port(t1);
    xsi_set_current_line(76, ng0);
    t1 = (t0 + 1832U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 4288);
    t4 = (t1 + 56U);
    t5 = *((char **)t4);
    t6 = (t5 + 56U);
    t7 = *((char **)t6);
    *((unsigned char *)t7) = t3;
    xsi_driver_first_trans_fast_port(t1);
    t1 = (t0 + 3888);
    *((int *)t1) = 1;

LAB1:    return;
}


extern void work_a_2614843129_1342404463_init()
{
	static char *pe[] = {(void *)work_a_2614843129_1342404463_p_0,(void *)work_a_2614843129_1342404463_p_1};
	xsi_register_didat("work_a_2614843129_1342404463", "isim/ghost_tb_isim_beh.exe.sim/work/a_2614843129_1342404463.didat");
	xsi_register_executes(pe);
}
