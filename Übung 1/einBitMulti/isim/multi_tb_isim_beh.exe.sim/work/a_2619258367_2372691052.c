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
static const char *ng0 = "/home/stefan/Rechnerarchitektur2015/einBitMulti/einBitMulti_tb.vhd";
extern char *STD_STANDARD;



static void work_a_2619258367_2372691052_p_0(char *t0)
{
    char t9[16];
    char t15[16];
    char t17[16];
    char *t1;
    char *t2;
    int64 t3;
    char *t4;
    int t5;
    int t6;
    char *t7;
    char *t8;
    char *t10;
    char *t11;
    char *t12;
    char *t13;
    char *t14;
    char *t16;
    char *t18;
    char *t19;
    int t20;
    unsigned int t21;
    unsigned int t22;
    int t23;
    unsigned int t24;
    unsigned int t25;
    unsigned char t26;
    unsigned char t27;
    unsigned char t28;

LAB0:    t1 = (t0 + 2624U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(59, ng0);
    t3 = (100 * 1000LL);
    t2 = (t0 + 2432);
    xsi_process_wait(t2, t3);

LAB6:    *((char **)t1) = &&LAB7;

LAB1:    return;
LAB4:    xsi_set_current_line(60, ng0);
    t2 = (t0 + 5280);
    xsi_report(t2, 20U, 0);
    xsi_set_current_line(63, ng0);
    t2 = (t0 + 5300);
    *((int *)t2) = 0;
    t4 = (t0 + 5304);
    *((int *)t4) = 3;
    t5 = 0;
    t6 = 3;

LAB8:    if (t5 <= t6)
        goto LAB9;

LAB11:    xsi_set_current_line(78, ng0);
    t2 = (t0 + 5346);
    xsi_report(t2, 18U, (unsigned char)3);
    xsi_set_current_line(79, ng0);

LAB21:    *((char **)t1) = &&LAB22;
    goto LAB1;

LAB5:    goto LAB4;

LAB7:    goto LAB5;

LAB9:    xsi_set_current_line(64, ng0);
    t7 = (t0 + 5308);
    t10 = ((STD_STANDARD) + 384);
    t11 = (t0 + 5300);
    t12 = xsi_int_to_mem(*((int *)t11));
    t13 = xsi_string_variable_get_image(t9, t10, t12);
    t16 = ((STD_STANDARD) + 984);
    t18 = (t17 + 0U);
    t19 = (t18 + 0U);
    *((int *)t19) = 1;
    t19 = (t18 + 4U);
    *((int *)t19) = 13;
    t19 = (t18 + 8U);
    *((int *)t19) = 1;
    t20 = (13 - 1);
    t21 = (t20 * 1);
    t21 = (t21 + 1);
    t19 = (t18 + 12U);
    *((unsigned int *)t19) = t21;
    t14 = xsi_base_array_concat(t14, t15, t16, (char)97, t7, t17, (char)97, t13, t9, (char)101);
    t19 = (t9 + 12U);
    t21 = *((unsigned int *)t19);
    t22 = (13U + t21);
    xsi_report(t14, t22, 0);
    xsi_set_current_line(66, ng0);
    t2 = (t0 + 1648U);
    t4 = *((char **)t2);
    t2 = (t0 + 5300);
    t20 = *((int *)t2);
    t23 = (t20 - 0);
    t21 = (t23 * 1);
    t22 = (8U * t21);
    t24 = (0 + t22);
    t25 = (t24 + 0U);
    t7 = (t4 + t25);
    t26 = *((unsigned char *)t7);
    t8 = (t0 + 3008);
    t10 = (t8 + 56U);
    t11 = *((char **)t10);
    t12 = (t11 + 56U);
    t13 = *((char **)t12);
    *((unsigned char *)t13) = t26;
    xsi_driver_first_trans_fast(t8);
    xsi_set_current_line(67, ng0);
    t2 = (t0 + 1648U);
    t4 = *((char **)t2);
    t2 = (t0 + 5300);
    t20 = *((int *)t2);
    t23 = (t20 - 0);
    t21 = (t23 * 1);
    t22 = (8U * t21);
    t24 = (0 + t22);
    t25 = (t24 + 1U);
    t7 = (t4 + t25);
    t26 = *((unsigned char *)t7);
    t8 = (t0 + 3072);
    t10 = (t8 + 56U);
    t11 = *((char **)t10);
    t12 = (t11 + 56U);
    t13 = *((char **)t12);
    *((unsigned char *)t13) = t26;
    xsi_driver_first_trans_fast(t8);
    xsi_set_current_line(69, ng0);
    t3 = (2 * 1000LL);
    t2 = (t0 + 2432);
    xsi_process_wait(t2, t3);

LAB14:    *((char **)t1) = &&LAB15;
    goto LAB1;

LAB10:    t2 = (t0 + 5300);
    t5 = *((int *)t2);
    t4 = (t0 + 5304);
    t6 = *((int *)t4);
    if (t5 == t6)
        goto LAB11;

LAB18:    t20 = (t5 + 1);
    t5 = t20;
    t7 = (t0 + 5300);
    *((int *)t7) = t5;
    goto LAB8;

LAB12:    xsi_set_current_line(73, ng0);
    t2 = (t0 + 1352U);
    t4 = *((char **)t2);
    t26 = *((unsigned char *)t4);
    t2 = (t0 + 1648U);
    t7 = *((char **)t2);
    t2 = (t0 + 5300);
    t20 = *((int *)t2);
    t23 = (t20 - 0);
    t21 = (t23 * 1);
    t22 = (8U * t21);
    t24 = (0 + t22);
    t25 = (t24 + 2U);
    t8 = (t7 + t25);
    t27 = *((unsigned char *)t8);
    t28 = (t26 == t27);
    if (t28 == 0)
        goto LAB16;

LAB17:    goto LAB10;

LAB13:    goto LAB12;

LAB15:    goto LAB13;

LAB16:    t10 = (t0 + 5321);
    xsi_report(t10, 25U, (unsigned char)2);
    goto LAB17;

LAB19:    goto LAB2;

LAB20:    goto LAB19;

LAB22:    goto LAB20;

}


extern void work_a_2619258367_2372691052_init()
{
	static char *pe[] = {(void *)work_a_2619258367_2372691052_p_0};
	xsi_register_didat("work_a_2619258367_2372691052", "isim/multi_tb_isim_beh.exe.sim/work/a_2619258367_2372691052.didat");
	xsi_register_executes(pe);
}
