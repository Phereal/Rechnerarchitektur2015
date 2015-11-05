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
static const char *ng0 = "/home/stefan/Rechnerarchitektur2015/vierBitMulti/four_b_multi_tb.vhd";
extern char *STD_STANDARD;
extern char *IEEE_P_1242562249;
extern char *IEEE_P_2592010699;

char *ieee_p_1242562249_sub_10420449594411817395_1035706684(char *, char *, int , int );
int ieee_p_1242562249_sub_17802405650254020620_1035706684(char *, char *, char *);


static void work_a_1575707885_2372691052_p_0(char *t0)
{
    char t13[16];
    char t19[16];
    char t21[16];
    char t28[16];
    char t30[16];
    char t34[16];
    char t39[16];
    char t44[16];
    char t46[16];
    char t50[16];
    char t59[16];
    char *t1;
    char *t2;
    char *t3;
    int64 t4;
    int t5;
    int t6;
    char *t7;
    char *t8;
    int t9;
    int t10;
    char *t11;
    char *t12;
    char *t14;
    char *t15;
    char *t16;
    char *t17;
    char *t18;
    char *t20;
    char *t22;
    char *t23;
    int t24;
    unsigned int t25;
    char *t26;
    char *t27;
    char *t29;
    char *t31;
    char *t32;
    int t33;
    char *t35;
    char *t36;
    char *t37;
    char *t38;
    char *t40;
    char *t41;
    char *t43;
    char *t45;
    char *t47;
    char *t48;
    int t49;
    char *t51;
    char *t52;
    int t53;
    int t54;
    int t55;
    char *t56;
    char *t57;
    char *t58;
    char *t60;
    char *t61;
    unsigned int t62;
    unsigned int t63;
    char *t64;
    unsigned int t65;
    unsigned int t66;
    unsigned int t67;
    char *t68;
    unsigned int t69;
    unsigned int t70;
    unsigned char t71;
    unsigned char t72;
    unsigned char t73;
    unsigned char t74;
    unsigned char t75;
    unsigned char t76;
    unsigned char t77;
    unsigned char t78;

LAB0:    t1 = (t0 + 5304U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(101, ng0);
    t2 = (t0 + 9568);
    xsi_report(t2, 16U, 0);
    xsi_set_current_line(104, ng0);
    t4 = (100 * 1000LL);
    t2 = (t0 + 5112);
    xsi_process_wait(t2, t4);

LAB6:    *((char **)t1) = &&LAB7;

LAB1:    return;
LAB4:    xsi_set_current_line(108, ng0);
    t2 = (t0 + 9584);
    *((int *)t2) = 0;
    t3 = (t0 + 9588);
    *((int *)t3) = 15;
    t5 = 0;
    t6 = 15;

LAB8:    if (t5 <= t6)
        goto LAB9;

LAB11:    xsi_set_current_line(136, ng0);
    t2 = (t0 + 9636);
    xsi_report(t2, 16U, (unsigned char)3);
    xsi_set_current_line(138, ng0);

LAB26:    *((char **)t1) = &&LAB27;
    goto LAB1;

LAB5:    goto LAB4;

LAB7:    goto LAB5;

LAB9:    xsi_set_current_line(109, ng0);
    t7 = (t0 + 9592);
    *((int *)t7) = 0;
    t8 = (t0 + 9596);
    *((int *)t8) = 15;
    t9 = 0;
    t10 = 15;

LAB12:    if (t9 <= t10)
        goto LAB13;

LAB15:
LAB10:    t2 = (t0 + 9584);
    t5 = *((int *)t2);
    t3 = (t0 + 9588);
    t6 = *((int *)t3);
    if (t5 == t6)
        goto LAB11;

LAB23:    t9 = (t5 + 1);
    t5 = t9;
    t7 = (t0 + 9584);
    *((int *)t7) = t5;
    goto LAB8;

LAB13:    xsi_set_current_line(114, ng0);
    t11 = (t0 + 9600);
    t14 = ((STD_STANDARD) + 384);
    t15 = (t0 + 9584);
    t16 = xsi_int_to_mem(*((int *)t15));
    t17 = xsi_string_variable_get_image(t13, t14, t16);
    t20 = ((STD_STANDARD) + 984);
    t22 = (t21 + 0U);
    t23 = (t22 + 0U);
    *((int *)t23) = 1;
    t23 = (t22 + 4U);
    *((int *)t23) = 9;
    t23 = (t22 + 8U);
    *((int *)t23) = 1;
    t24 = (9 - 1);
    t25 = (t24 * 1);
    t25 = (t25 + 1);
    t23 = (t22 + 12U);
    *((unsigned int *)t23) = t25;
    t18 = xsi_base_array_concat(t18, t19, t20, (char)97, t11, t21, (char)97, t17, t13, (char)101);
    t23 = (t0 + 9609);
    t29 = ((STD_STANDARD) + 984);
    t31 = (t30 + 0U);
    t32 = (t31 + 0U);
    *((int *)t32) = 1;
    t32 = (t31 + 4U);
    *((int *)t32) = 3;
    t32 = (t31 + 8U);
    *((int *)t32) = 1;
    t33 = (3 - 1);
    t25 = (t33 * 1);
    t25 = (t25 + 1);
    t32 = (t31 + 12U);
    *((unsigned int *)t32) = t25;
    t27 = xsi_base_array_concat(t27, t28, t29, (char)97, t18, t19, (char)97, t23, t30, (char)101);
    t32 = ((STD_STANDARD) + 384);
    t35 = (t0 + 9592);
    t36 = xsi_int_to_mem(*((int *)t35));
    t37 = xsi_string_variable_get_image(t34, t32, t36);
    t40 = ((STD_STANDARD) + 984);
    t38 = xsi_base_array_concat(t38, t39, t40, (char)97, t27, t28, (char)97, t37, t34, (char)101);
    t41 = (t0 + 9612);
    t45 = ((STD_STANDARD) + 984);
    t47 = (t46 + 0U);
    t48 = (t47 + 0U);
    *((int *)t48) = 1;
    t48 = (t47 + 4U);
    *((int *)t48) = 3;
    t48 = (t47 + 8U);
    *((int *)t48) = 1;
    t49 = (3 - 1);
    t25 = (t49 * 1);
    t25 = (t25 + 1);
    t48 = (t47 + 12U);
    *((unsigned int *)t48) = t25;
    t43 = xsi_base_array_concat(t43, t44, t45, (char)97, t38, t39, (char)97, t41, t46, (char)101);
    t48 = ((STD_STANDARD) + 384);
    t51 = (t0 + 9584);
    t52 = (t0 + 9592);
    t53 = *((int *)t51);
    t54 = *((int *)t52);
    t55 = (t53 * t54);
    t56 = xsi_int_to_mem(t55);
    t57 = xsi_string_variable_get_image(t50, t48, t56);
    t60 = ((STD_STANDARD) + 984);
    t58 = xsi_base_array_concat(t58, t59, t60, (char)97, t43, t44, (char)97, t57, t50, (char)101);
    t61 = (t13 + 12U);
    t25 = *((unsigned int *)t61);
    t62 = (9U + t25);
    t63 = (t62 + 3U);
    t64 = (t34 + 12U);
    t65 = *((unsigned int *)t64);
    t66 = (t63 + t65);
    t67 = (t66 + 3U);
    t68 = (t50 + 12U);
    t69 = *((unsigned int *)t68);
    t70 = (t67 + t69);
    xsi_report(t58, t70, 0);
    xsi_set_current_line(115, ng0);
    t2 = (t0 + 9584);
    t3 = ieee_p_1242562249_sub_10420449594411817395_1035706684(IEEE_P_1242562249, t13, *((int *)t2), 4);
    t7 = (t0 + 4088U);
    t8 = *((char **)t7);
    t7 = (t8 + 0);
    t11 = (t13 + 12U);
    t25 = *((unsigned int *)t11);
    t25 = (t25 * 1U);
    memcpy(t7, t3, t25);
    xsi_set_current_line(116, ng0);
    t2 = (t0 + 9592);
    t3 = ieee_p_1242562249_sub_10420449594411817395_1035706684(IEEE_P_1242562249, t13, *((int *)t2), 4);
    t7 = (t0 + 4208U);
    t8 = *((char **)t7);
    t7 = (t8 + 0);
    t11 = (t13 + 12U);
    t25 = *((unsigned int *)t11);
    t25 = (t25 * 1U);
    memcpy(t7, t3, t25);
    xsi_set_current_line(117, ng0);
    t2 = (t0 + 4088U);
    t3 = *((char **)t2);
    t24 = (0 - 3);
    t25 = (t24 * -1);
    t62 = (1U * t25);
    t63 = (0 + t62);
    t2 = (t3 + t63);
    t71 = *((unsigned char *)t2);
    t7 = (t0 + 5688);
    t8 = (t7 + 56U);
    t11 = *((char **)t8);
    t12 = (t11 + 56U);
    t14 = *((char **)t12);
    *((unsigned char *)t14) = t71;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(117, ng0);
    t2 = (t0 + 4088U);
    t3 = *((char **)t2);
    t24 = (1 - 3);
    t25 = (t24 * -1);
    t62 = (1U * t25);
    t63 = (0 + t62);
    t2 = (t3 + t63);
    t71 = *((unsigned char *)t2);
    t7 = (t0 + 5752);
    t8 = (t7 + 56U);
    t11 = *((char **)t8);
    t12 = (t11 + 56U);
    t14 = *((char **)t12);
    *((unsigned char *)t14) = t71;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(117, ng0);
    t2 = (t0 + 4088U);
    t3 = *((char **)t2);
    t24 = (2 - 3);
    t25 = (t24 * -1);
    t62 = (1U * t25);
    t63 = (0 + t62);
    t2 = (t3 + t63);
    t71 = *((unsigned char *)t2);
    t7 = (t0 + 5816);
    t8 = (t7 + 56U);
    t11 = *((char **)t8);
    t12 = (t11 + 56U);
    t14 = *((char **)t12);
    *((unsigned char *)t14) = t71;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(117, ng0);
    t2 = (t0 + 4088U);
    t3 = *((char **)t2);
    t24 = (3 - 3);
    t25 = (t24 * -1);
    t62 = (1U * t25);
    t63 = (0 + t62);
    t2 = (t3 + t63);
    t71 = *((unsigned char *)t2);
    t7 = (t0 + 5880);
    t8 = (t7 + 56U);
    t11 = *((char **)t8);
    t12 = (t11 + 56U);
    t14 = *((char **)t12);
    *((unsigned char *)t14) = t71;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(118, ng0);
    t2 = (t0 + 4208U);
    t3 = *((char **)t2);
    t24 = (0 - 3);
    t25 = (t24 * -1);
    t62 = (1U * t25);
    t63 = (0 + t62);
    t2 = (t3 + t63);
    t71 = *((unsigned char *)t2);
    t7 = (t0 + 5944);
    t8 = (t7 + 56U);
    t11 = *((char **)t8);
    t12 = (t11 + 56U);
    t14 = *((char **)t12);
    *((unsigned char *)t14) = t71;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(118, ng0);
    t2 = (t0 + 4208U);
    t3 = *((char **)t2);
    t24 = (1 - 3);
    t25 = (t24 * -1);
    t62 = (1U * t25);
    t63 = (0 + t62);
    t2 = (t3 + t63);
    t71 = *((unsigned char *)t2);
    t7 = (t0 + 6008);
    t8 = (t7 + 56U);
    t11 = *((char **)t8);
    t12 = (t11 + 56U);
    t14 = *((char **)t12);
    *((unsigned char *)t14) = t71;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(118, ng0);
    t2 = (t0 + 4208U);
    t3 = *((char **)t2);
    t24 = (2 - 3);
    t25 = (t24 * -1);
    t62 = (1U * t25);
    t63 = (0 + t62);
    t2 = (t3 + t63);
    t71 = *((unsigned char *)t2);
    t7 = (t0 + 6072);
    t8 = (t7 + 56U);
    t11 = *((char **)t8);
    t12 = (t11 + 56U);
    t14 = *((char **)t12);
    *((unsigned char *)t14) = t71;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(118, ng0);
    t2 = (t0 + 4208U);
    t3 = *((char **)t2);
    t24 = (3 - 3);
    t25 = (t24 * -1);
    t62 = (1U * t25);
    t63 = (0 + t62);
    t2 = (t3 + t63);
    t71 = *((unsigned char *)t2);
    t7 = (t0 + 6136);
    t8 = (t7 + 56U);
    t11 = *((char **)t8);
    t12 = (t11 + 56U);
    t14 = *((char **)t12);
    *((unsigned char *)t14) = t71;
    xsi_driver_first_trans_fast(t7);
    xsi_set_current_line(121, ng0);
    t2 = (t0 + 3728U);
    t3 = *((char **)t2);
    t4 = *((int64 *)t3);
    t2 = (t0 + 5112);
    xsi_process_wait(t2, t4);

LAB18:    *((char **)t1) = &&LAB19;
    goto LAB1;

LAB14:    t2 = (t0 + 9592);
    t9 = *((int *)t2);
    t3 = (t0 + 9596);
    t10 = *((int *)t3);
    if (t9 == t10)
        goto LAB15;

LAB22:    t24 = (t9 + 1);
    t9 = t24;
    t7 = (t0 + 9592);
    *((int *)t7) = t9;
    goto LAB12;

LAB16:    xsi_set_current_line(127, ng0);
    t2 = (t0 + 3432U);
    t3 = *((char **)t2);
    t71 = *((unsigned char *)t3);
    t2 = (t0 + 3272U);
    t7 = *((char **)t2);
    t72 = *((unsigned char *)t7);
    t8 = ((IEEE_P_2592010699) + 4000);
    t2 = xsi_base_array_concat(t2, t13, t8, (char)99, t71, (char)99, t72, (char)101);
    t11 = (t0 + 3112U);
    t12 = *((char **)t11);
    t73 = *((unsigned char *)t12);
    t14 = ((IEEE_P_2592010699) + 4000);
    t11 = xsi_base_array_concat(t11, t19, t14, (char)97, t2, t13, (char)99, t73, (char)101);
    t15 = (t0 + 2952U);
    t16 = *((char **)t15);
    t74 = *((unsigned char *)t16);
    t17 = ((IEEE_P_2592010699) + 4000);
    t15 = xsi_base_array_concat(t15, t21, t17, (char)97, t11, t19, (char)99, t74, (char)101);
    t18 = (t0 + 2792U);
    t20 = *((char **)t18);
    t75 = *((unsigned char *)t20);
    t22 = ((IEEE_P_2592010699) + 4000);
    t18 = xsi_base_array_concat(t18, t28, t22, (char)97, t15, t21, (char)99, t75, (char)101);
    t23 = (t0 + 2632U);
    t26 = *((char **)t23);
    t76 = *((unsigned char *)t26);
    t27 = ((IEEE_P_2592010699) + 4000);
    t23 = xsi_base_array_concat(t23, t30, t27, (char)97, t18, t28, (char)99, t76, (char)101);
    t29 = (t0 + 2472U);
    t31 = *((char **)t29);
    t77 = *((unsigned char *)t31);
    t32 = ((IEEE_P_2592010699) + 4000);
    t29 = xsi_base_array_concat(t29, t34, t32, (char)97, t23, t30, (char)99, t77, (char)101);
    t35 = (t0 + 2312U);
    t36 = *((char **)t35);
    t78 = *((unsigned char *)t36);
    t37 = ((IEEE_P_2592010699) + 4000);
    t35 = xsi_base_array_concat(t35, t39, t37, (char)97, t29, t34, (char)99, t78, (char)101);
    t38 = (t0 + 4328U);
    t40 = *((char **)t38);
    t38 = (t40 + 0);
    t25 = (1U + 1U);
    t62 = (t25 + 1U);
    t63 = (t62 + 1U);
    t65 = (t63 + 1U);
    t66 = (t65 + 1U);
    t67 = (t66 + 1U);
    t69 = (t67 + 1U);
    memcpy(t38, t35, t69);
    xsi_set_current_line(128, ng0);
    t2 = (t0 + 4328U);
    t3 = *((char **)t2);
    t2 = (t0 + 9496U);
    t24 = ieee_p_1242562249_sub_17802405650254020620_1035706684(IEEE_P_1242562249, t3, t2);
    t7 = (t0 + 9584);
    t8 = (t0 + 9592);
    t33 = *((int *)t7);
    t49 = *((int *)t8);
    t53 = (t33 * t49);
    t71 = (t24 == t53);
    if (t71 == 0)
        goto LAB20;

LAB21:    goto LAB14;

LAB17:    goto LAB16;

LAB19:    goto LAB17;

LAB20:    t11 = (t0 + 9615);
    t14 = ((STD_STANDARD) + 384);
    t15 = (t0 + 4328U);
    t16 = *((char **)t15);
    t15 = (t0 + 9496U);
    t54 = ieee_p_1242562249_sub_17802405650254020620_1035706684(IEEE_P_1242562249, t16, t15);
    t17 = xsi_int_to_mem(t54);
    t18 = xsi_string_variable_get_image(t13, t14, t17);
    t22 = ((STD_STANDARD) + 984);
    t23 = (t21 + 0U);
    t26 = (t23 + 0U);
    *((int *)t26) = 1;
    t26 = (t23 + 4U);
    *((int *)t26) = 21;
    t26 = (t23 + 8U);
    *((int *)t26) = 1;
    t55 = (21 - 1);
    t25 = (t55 * 1);
    t25 = (t25 + 1);
    t26 = (t23 + 12U);
    *((unsigned int *)t26) = t25;
    t20 = xsi_base_array_concat(t20, t19, t22, (char)97, t11, t21, (char)97, t18, t13, (char)101);
    t26 = (t13 + 12U);
    t25 = *((unsigned int *)t26);
    t62 = (21U + t25);
    xsi_report(t20, t62, (unsigned char)3);
    goto LAB21;

LAB24:    goto LAB2;

LAB25:    goto LAB24;

LAB27:    goto LAB25;

}


extern void work_a_1575707885_2372691052_init()
{
	static char *pe[] = {(void *)work_a_1575707885_2372691052_p_0};
	xsi_register_didat("work_a_1575707885_2372691052", "isim/four_b_multi_tb_isim_beh.exe.sim/work/a_1575707885_2372691052.didat");
	xsi_register_executes(pe);
}
