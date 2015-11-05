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
static const char *ng0 = "/home/stefan/Rechnerarchitektur2015/vierBitMulti/eight_bit_adder_tb.vhd";
extern char *STD_STANDARD;



static void work_a_4239154850_2372691052_p_0(char *t0)
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

LAB0:    t1 = (t0 + 6144U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(136, ng0);
    t2 = (t0 + 11752);
    xsi_report(t2, 20U, 0);
    xsi_set_current_line(137, ng0);
    t2 = (t0 + 11772);
    *((int *)t2) = 0;
    t3 = (t0 + 11776);
    *((int *)t3) = 4;
    t4 = 0;
    t5 = 4;

LAB4:    if (t4 <= t5)
        goto LAB5;

LAB7:    xsi_set_current_line(173, ng0);
    if ((unsigned char)0 == 0)
        goto LAB31;

LAB32:    xsi_set_current_line(174, ng0);

LAB35:    *((char **)t1) = &&LAB36;

LAB1:    return;
LAB5:    xsi_set_current_line(139, ng0);
    t6 = (t0 + 11780);
    t9 = ((STD_STANDARD) + 384);
    t10 = (t0 + 11772);
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
    xsi_set_current_line(141, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 0U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 6528);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(142, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 1U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 6592);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(143, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 2U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 6656);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(144, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 3U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 6720);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(145, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 4U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 6784);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(146, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 5U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 6848);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(147, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 6U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 6912);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(148, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 7U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 6976);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(149, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 8U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 7040);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(150, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 9U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 7104);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(151, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 10U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 7168);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(152, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 11U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 7232);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(153, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 12U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 7296);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(154, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 13U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 7360);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(155, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 14U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 7424);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(156, ng0);
    t2 = (t0 + 5168U);
    t3 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 15U);
    t6 = (t3 + t24);
    t25 = *((unsigned char *)t6);
    t7 = (t0 + 7488);
    t9 = (t7 + 56U);
    t10 = *((char **)t9);
    t11 = (t10 + 56U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = t25;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(158, ng0);
    t26 = (2 * 1000LL);
    t2 = (t0 + 5952);
    xsi_process_wait(t2, t26);

LAB10:    *((char **)t1) = &&LAB11;
    goto LAB1;

LAB6:    t2 = (t0 + 11772);
    t4 = *((int *)t2);
    t3 = (t0 + 11776);
    t5 = *((int *)t3);
    if (t4 == t5)
        goto LAB7;

LAB30:    t19 = (t4 + 1);
    t4 = t19;
    t6 = (t0 + 11772);
    *((int *)t6) = t4;
    goto LAB4;

LAB8:    xsi_set_current_line(160, ng0);
    t2 = (t0 + 3592U);
    t3 = *((char **)t2);
    t25 = *((unsigned char *)t3);
    t2 = (t0 + 5168U);
    t6 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 16U);
    t7 = (t6 + t24);
    t27 = *((unsigned char *)t7);
    t28 = (t25 == t27);
    if (t28 == 0)
        goto LAB12;

LAB13:    xsi_set_current_line(161, ng0);
    t2 = (t0 + 3752U);
    t3 = *((char **)t2);
    t25 = *((unsigned char *)t3);
    t2 = (t0 + 5168U);
    t6 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 17U);
    t7 = (t6 + t24);
    t27 = *((unsigned char *)t7);
    t28 = (t25 == t27);
    if (t28 == 0)
        goto LAB14;

LAB15:    xsi_set_current_line(162, ng0);
    t2 = (t0 + 3912U);
    t3 = *((char **)t2);
    t25 = *((unsigned char *)t3);
    t2 = (t0 + 5168U);
    t6 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 18U);
    t7 = (t6 + t24);
    t27 = *((unsigned char *)t7);
    t28 = (t25 == t27);
    if (t28 == 0)
        goto LAB16;

LAB17:    xsi_set_current_line(163, ng0);
    t2 = (t0 + 4072U);
    t3 = *((char **)t2);
    t25 = *((unsigned char *)t3);
    t2 = (t0 + 5168U);
    t6 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 19U);
    t7 = (t6 + t24);
    t27 = *((unsigned char *)t7);
    t28 = (t25 == t27);
    if (t28 == 0)
        goto LAB18;

LAB19:    xsi_set_current_line(164, ng0);
    t2 = (t0 + 4232U);
    t3 = *((char **)t2);
    t25 = *((unsigned char *)t3);
    t2 = (t0 + 5168U);
    t6 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 20U);
    t7 = (t6 + t24);
    t27 = *((unsigned char *)t7);
    t28 = (t25 == t27);
    if (t28 == 0)
        goto LAB20;

LAB21:    xsi_set_current_line(165, ng0);
    t2 = (t0 + 4392U);
    t3 = *((char **)t2);
    t25 = *((unsigned char *)t3);
    t2 = (t0 + 5168U);
    t6 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 21U);
    t7 = (t6 + t24);
    t27 = *((unsigned char *)t7);
    t28 = (t25 == t27);
    if (t28 == 0)
        goto LAB22;

LAB23:    xsi_set_current_line(166, ng0);
    t2 = (t0 + 4552U);
    t3 = *((char **)t2);
    t25 = *((unsigned char *)t3);
    t2 = (t0 + 5168U);
    t6 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 22U);
    t7 = (t6 + t24);
    t27 = *((unsigned char *)t7);
    t28 = (t25 == t27);
    if (t28 == 0)
        goto LAB24;

LAB25:    xsi_set_current_line(167, ng0);
    t2 = (t0 + 4712U);
    t3 = *((char **)t2);
    t25 = *((unsigned char *)t3);
    t2 = (t0 + 5168U);
    t6 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 23U);
    t7 = (t6 + t24);
    t27 = *((unsigned char *)t7);
    t28 = (t25 == t27);
    if (t28 == 0)
        goto LAB26;

LAB27:    xsi_set_current_line(168, ng0);
    t2 = (t0 + 4872U);
    t3 = *((char **)t2);
    t25 = *((unsigned char *)t3);
    t2 = (t0 + 5168U);
    t6 = *((char **)t2);
    t2 = (t0 + 11772);
    t19 = *((int *)t2);
    t22 = (t19 - 0);
    t20 = (t22 * 1);
    t21 = (32U * t20);
    t23 = (0 + t21);
    t24 = (t23 + 24U);
    t7 = (t6 + t24);
    t27 = *((unsigned char *)t7);
    t28 = (t25 == t27);
    if (t28 == 0)
        goto LAB28;

LAB29:    goto LAB6;

LAB9:    goto LAB8;

LAB11:    goto LAB9;

LAB12:    t9 = (t0 + 11793);
    xsi_report(t9, 19U, (unsigned char)2);
    goto LAB13;

LAB14:    t9 = (t0 + 11812);
    xsi_report(t9, 19U, (unsigned char)2);
    goto LAB15;

LAB16:    t9 = (t0 + 11831);
    xsi_report(t9, 19U, (unsigned char)2);
    goto LAB17;

LAB18:    t9 = (t0 + 11850);
    xsi_report(t9, 19U, (unsigned char)2);
    goto LAB19;

LAB20:    t9 = (t0 + 11869);
    xsi_report(t9, 19U, (unsigned char)2);
    goto LAB21;

LAB22:    t9 = (t0 + 11888);
    xsi_report(t9, 19U, (unsigned char)2);
    goto LAB23;

LAB24:    t9 = (t0 + 11907);
    xsi_report(t9, 19U, (unsigned char)2);
    goto LAB25;

LAB26:    t9 = (t0 + 11926);
    xsi_report(t9, 19U, (unsigned char)2);
    goto LAB27;

LAB28:    t9 = (t0 + 11945);
    xsi_report(t9, 17U, (unsigned char)2);
    goto LAB29;

LAB31:    t2 = (t0 + 11962);
    xsi_report(t2, 27U, (unsigned char)3);
    goto LAB32;

LAB33:    goto LAB2;

LAB34:    goto LAB33;

LAB36:    goto LAB34;

}


extern void work_a_4239154850_2372691052_init()
{
	static char *pe[] = {(void *)work_a_4239154850_2372691052_p_0};
	xsi_register_didat("work_a_4239154850_2372691052", "isim/eight_bit_adder_tb_isim_beh.exe.sim/work/a_4239154850_2372691052.didat");
	xsi_register_executes(pe);
}
