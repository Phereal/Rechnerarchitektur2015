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
static const char *ng0 = "/home/stefan/Rechnerarchitektur2015/vierBitMulti/full_adder_tb.vhd";



static void work_a_2249932033_2372691052_p_0(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    int t4;
    int t5;
    char *t6;
    char *t7;
    int t8;
    int t9;
    unsigned int t10;
    unsigned int t11;
    unsigned int t12;
    unsigned int t13;
    char *t14;
    unsigned char t15;
    char *t16;
    char *t17;
    char *t18;
    char *t19;
    char *t20;
    int64 t21;
    unsigned char t22;
    unsigned char t23;

LAB0:    t1 = (t0 + 2944U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(102, ng0);
    t2 = (t0 + 5832);
    *((int *)t2) = 0;
    t3 = (t0 + 5836);
    *((int *)t3) = 7;
    t4 = 0;
    t5 = 7;

LAB4:    if (t4 <= t5)
        goto LAB5;

LAB7:    xsi_set_current_line(115, ng0);
    if ((unsigned char)0 == 0)
        goto LAB17;

LAB18:    xsi_set_current_line(116, ng0);

LAB21:    *((char **)t1) = &&LAB22;

LAB1:    return;
LAB5:    xsi_set_current_line(103, ng0);
    t6 = (t0 + 1968U);
    t7 = *((char **)t6);
    t6 = (t0 + 5832);
    t8 = *((int *)t6);
    t9 = (t8 - 0);
    t10 = (t9 * 1);
    t11 = (8U * t10);
    t12 = (0 + t11);
    t13 = (t12 + 0U);
    t14 = (t7 + t13);
    t15 = *((unsigned char *)t14);
    t16 = (t0 + 3328);
    t17 = (t16 + 56U);
    t18 = *((char **)t17);
    t19 = (t18 + 56U);
    t20 = *((char **)t19);
    *((unsigned char *)t20) = t15;
    xsi_driver_first_trans_fast(t16);
    xsi_set_current_line(104, ng0);
    t2 = (t0 + 1968U);
    t3 = *((char **)t2);
    t2 = (t0 + 5832);
    t8 = *((int *)t2);
    t9 = (t8 - 0);
    t10 = (t9 * 1);
    t11 = (8U * t10);
    t12 = (0 + t11);
    t13 = (t12 + 1U);
    t6 = (t3 + t13);
    t15 = *((unsigned char *)t6);
    t7 = (t0 + 3392);
    t14 = (t7 + 56U);
    t16 = *((char **)t14);
    t17 = (t16 + 56U);
    t18 = *((char **)t17);
    *((unsigned char *)t18) = t15;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(105, ng0);
    t2 = (t0 + 1968U);
    t3 = *((char **)t2);
    t2 = (t0 + 5832);
    t8 = *((int *)t2);
    t9 = (t8 - 0);
    t10 = (t9 * 1);
    t11 = (8U * t10);
    t12 = (0 + t11);
    t13 = (t12 + 2U);
    t6 = (t3 + t13);
    t15 = *((unsigned char *)t6);
    t7 = (t0 + 3456);
    t14 = (t7 + 56U);
    t16 = *((char **)t14);
    t17 = (t16 + 56U);
    t18 = *((char **)t17);
    *((unsigned char *)t18) = t15;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(107, ng0);
    t21 = (2 * 1000LL);
    t2 = (t0 + 2752);
    xsi_process_wait(t2, t21);

LAB10:    *((char **)t1) = &&LAB11;
    goto LAB1;

LAB6:    t2 = (t0 + 5832);
    t4 = *((int *)t2);
    t3 = (t0 + 5836);
    t5 = *((int *)t3);
    if (t4 == t5)
        goto LAB7;

LAB16:    t8 = (t4 + 1);
    t4 = t8;
    t6 = (t0 + 5832);
    *((int *)t6) = t4;
    goto LAB4;

LAB8:    xsi_set_current_line(109, ng0);
    t2 = (t0 + 1512U);
    t3 = *((char **)t2);
    t15 = *((unsigned char *)t3);
    t2 = (t0 + 1968U);
    t6 = *((char **)t2);
    t2 = (t0 + 5832);
    t8 = *((int *)t2);
    t9 = (t8 - 0);
    t10 = (t9 * 1);
    t11 = (8U * t10);
    t12 = (0 + t11);
    t13 = (t12 + 3U);
    t7 = (t6 + t13);
    t22 = *((unsigned char *)t7);
    t23 = (t15 == t22);
    if (t23 == 0)
        goto LAB12;

LAB13:    xsi_set_current_line(110, ng0);
    t2 = (t0 + 1672U);
    t3 = *((char **)t2);
    t15 = *((unsigned char *)t3);
    t2 = (t0 + 1968U);
    t6 = *((char **)t2);
    t2 = (t0 + 5832);
    t8 = *((int *)t2);
    t9 = (t8 - 0);
    t10 = (t9 * 1);
    t11 = (8U * t10);
    t12 = (0 + t11);
    t13 = (t12 + 4U);
    t7 = (t6 + t13);
    t22 = *((unsigned char *)t7);
    t23 = (t15 == t22);
    if (t23 == 0)
        goto LAB14;

LAB15:    goto LAB6;

LAB9:    goto LAB8;

LAB11:    goto LAB9;

LAB12:    t14 = (t0 + 5840);
    xsi_report(t14, 15U, (unsigned char)2);
    goto LAB13;

LAB14:    t14 = (t0 + 5855);
    xsi_report(t14, 17U, (unsigned char)2);
    goto LAB15;

LAB17:    t2 = (t0 + 5872);
    xsi_report(t2, 27U, (unsigned char)0);
    goto LAB18;

LAB19:    goto LAB2;

LAB20:    goto LAB19;

LAB22:    goto LAB20;

}


extern void work_a_2249932033_2372691052_init()
{
	static char *pe[] = {(void *)work_a_2249932033_2372691052_p_0};
	xsi_register_didat("work_a_2249932033_2372691052", "isim/full_adder_tb_isim_beh.exe.sim/work/a_2249932033_2372691052.didat");
	xsi_register_executes(pe);
}
