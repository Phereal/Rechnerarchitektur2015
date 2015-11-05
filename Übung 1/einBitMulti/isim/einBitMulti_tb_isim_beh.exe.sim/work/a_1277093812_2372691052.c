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
static const char *ng0 = "/home/stefan/Rechnerarchitektur/einBitMulti/einBitMulti_tb.vhd";
extern char *STD_STANDARD;



static void work_a_1277093812_2372691052_p_0(char *t0)
{
    char t8[16];
    char t14[16];
    char t16[16];
    char *t1;
    char *t2;
    char *t3;
    int t4;
    int t5;
    char *t6;
    char *t7;
    char *t9;
    char *t10;
    char *t11;
    char *t12;
    char *t13;
    char *t15;
    char *t17;
    char *t18;
    int t19;
    unsigned int t20;
    unsigned int t21;
    int t22;
    unsigned int t23;
    unsigned int t24;
    unsigned char t25;
    int64 t26;
    unsigned char t27;
    unsigned char t28;

LAB0:    t1 = (t0 + 2624U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(56, ng0);
    t2 = (t0 + 5240);
    xsi_report(t2, 20U, 0);
    xsi_set_current_line(59, ng0);
    t2 = (t0 + 5260);
    *((int *)t2) = 0;
    t3 = (t0 + 5264);
    *((int *)t3) = 3;
    t4 = 0;
    t5 = 3;

LAB4:    if (t4 <= t5)
        goto LAB5;

LAB7:    xsi_set_current_line(72, ng0);
    t2 = (t0 + 5306);
    xsi_report(t2, 18U, (unsigned char)3);
    xsi_set_current_line(73, ng0);

LAB17:    *((char **)t1) = &&LAB18;

LAB1:    return;
LAB5:    xsi_set_current_line(60, ng0);
    t6 = (t0 + 5268);
    t9 = ((STD_STANDARD) + 384);
    t10 = (t0 + 5260);
    t11 = xsi_int_to_mem(*((int *)t10));
    t12 = xsi_string_variable_get_image(t8, t9, t11);
    t15 = ((STD_STANDARD) + 984);
    t17 = (t16 + 0U);
    t18 = (t17 + 0U);
    *((int *)t18) = 1;
    t18 = (t17 + 4U);
    *((int *)t18) = 13;
    t18 = (t17 + 8U);
    *((int *)t18) = 1;
    t19 = (13 - 1);
    t20 = (t19 * 1);
    t20 = (t20 + 1);
    t18 = (t17 + 12U);
    *((unsigned int *)t18) = t20;
    t13 = xsi_base_array_concat(t13, t14, t15, (char)97, t6, t16, (char)97, t12, t8, (char)101);
    t18 = (t8 + 12U);
    t20 = *((unsigned int *)t18);
    t21 = (13U + t20);
    xsi_report(t13, t21, 0);
    xsi_set_current_line(62, ng0);
    t2 = (t0 + 1648U);
    t3 = *((char **)t2);
    t2 = (t0 + 5260);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (8U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 0U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 3008);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(63, ng0);
    t2 = (t0 + 1648U);
    t3 = *((char **)t2);
    t2 = (t0 + 5260);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (8U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 1U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 3072);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(65, ng0);
    t26 = (2 * 1000LL);
    t2 = (t0 + 2432);
    xsi_process_wait(t2, t26);

LAB10:    *((char **)t1) = &&LAB11;
    goto LAB1;

LAB6:    t2 = (t0 + 5260);
    t4 = *((int *)t2);
    t3 = (t0 + 5264);
    t5 = *((int *)t3);
    if (t4 == t5)
        goto LAB7;

LAB14:    t19 = (t4 + 1);
    t4 = t19;
    t6 = (t0 + 5260);
    *((int *)t6) = t4;
    goto LAB4;

LAB8:    xsi_set_current_line(68, ng0);
    t2 = (t0 + 1352U);
    t3 = *((char **)t2);
    t25 = *((unsigned char *)t3);
    t2 = (t0 + 1648U);
    t6 = *((char **)t2);
    t2 = (t0 + 5260);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (8U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 2U);
    t7 = (t6 + t24);
    t27 = *((unsigned char *)t7);
    t28 = (t25 == t27);
    if (t28 == 0)
        goto LAB12;

LAB13:    goto LAB6;

LAB9:    goto LAB8;

LAB11:    goto LAB9;

LAB12:    t9 = (t0 + 5281);
    xsi_report(t9, 25U, (unsigned char)2);
    goto LAB13;

LAB15:    goto LAB2;

LAB16:    goto LAB15;

LAB18:    goto LAB16;

}


extern void work_a_1277093812_2372691052_init()
{
	static char *pe[] = {(void *)work_a_1277093812_2372691052_p_0};
	xsi_register_didat("work_a_1277093812_2372691052", "isim/einBitMulti_tb_isim_beh.exe.sim/work/a_1277093812_2372691052.didat");
	xsi_register_executes(pe);
}
