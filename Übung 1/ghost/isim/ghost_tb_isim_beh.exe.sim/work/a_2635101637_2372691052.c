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
static const char *ng0 = "/home/stefan/Rechnerarchitektur2015/ghost/ghost_tb.vhd";



static void work_a_2635101637_2372691052_p_0(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    int64 t7;
    int64 t8;

LAB0:    t1 = (t0 + 2784U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(48, ng0);
    t2 = (t0 + 3416);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t4 + 56U);
    t6 = *((char **)t5);
    *((unsigned char *)t6) = (unsigned char)3;
    xsi_driver_first_trans_fast(t2);
    xsi_set_current_line(49, ng0);
    t2 = (t0 + 1808U);
    t3 = *((char **)t2);
    t7 = *((int64 *)t3);
    t8 = (t7 / 2);
    t2 = (t0 + 2592);
    xsi_process_wait(t2, t8);

LAB6:    *((char **)t1) = &&LAB7;

LAB1:    return;
LAB4:    xsi_set_current_line(50, ng0);
    t2 = (t0 + 3416);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t4 + 56U);
    t6 = *((char **)t5);
    *((unsigned char *)t6) = (unsigned char)2;
    xsi_driver_first_trans_fast(t2);
    xsi_set_current_line(51, ng0);
    t2 = (t0 + 1808U);
    t3 = *((char **)t2);
    t7 = *((int64 *)t3);
    t8 = (t7 / 2);
    t2 = (t0 + 2592);
    xsi_process_wait(t2, t8);

LAB10:    *((char **)t1) = &&LAB11;
    goto LAB1;

LAB5:    goto LAB4;

LAB7:    goto LAB5;

LAB8:    goto LAB2;

LAB9:    goto LAB8;

LAB11:    goto LAB9;

}

static void work_a_2635101637_2372691052_p_1(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    int64 t4;
    int64 t5;
    unsigned char t6;
    unsigned char t7;
    char *t8;
    char *t9;
    char *t10;

LAB0:    t1 = (t0 + 3032U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(58, ng0);
    t2 = (t0 + 5552);
    xsi_report(t2, 21U, 0);
    xsi_set_current_line(60, ng0);
    t2 = (t0 + 5573);
    xsi_report(t2, 23U, 0);
    xsi_set_current_line(61, ng0);
    t2 = (t0 + 1808U);
    t3 = *((char **)t2);
    t4 = *((int64 *)t3);
    t5 = (t4 * 1);
    t2 = (t0 + 2840);
    xsi_process_wait(t2, t5);

LAB6:    *((char **)t1) = &&LAB7;

LAB1:    return;
LAB4:    xsi_set_current_line(62, ng0);
    t2 = (t0 + 1512U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)2);
    if (t7 == 0)
        goto LAB8;

LAB9:    xsi_set_current_line(63, ng0);
    t2 = (t0 + 1352U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)2);
    if (t7 == 0)
        goto LAB10;

LAB11:    xsi_set_current_line(66, ng0);
    t2 = (t0 + 5661);
    xsi_report(t2, 45U, 0);
    xsi_set_current_line(67, ng0);
    t2 = (t0 + 3480);
    t3 = (t2 + 56U);
    t8 = *((char **)t3);
    t9 = (t8 + 56U);
    t10 = *((char **)t9);
    *((unsigned char *)t10) = (unsigned char)3;
    xsi_driver_first_trans_fast(t2);
    xsi_set_current_line(68, ng0);
    t2 = (t0 + 1808U);
    t3 = *((char **)t2);
    t4 = *((int64 *)t3);
    t5 = (t4 * 1);
    t2 = (t0 + 2840);
    xsi_process_wait(t2, t5);

LAB14:    *((char **)t1) = &&LAB15;
    goto LAB1;

LAB5:    goto LAB4;

LAB7:    goto LAB5;

LAB8:    t2 = (t0 + 5596);
    xsi_report(t2, 32U, (unsigned char)1);
    goto LAB9;

LAB10:    t2 = (t0 + 5628);
    xsi_report(t2, 33U, (unsigned char)1);
    goto LAB11;

LAB12:    xsi_set_current_line(69, ng0);
    t2 = (t0 + 1512U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)3);
    if (t7 == 0)
        goto LAB16;

LAB17:    xsi_set_current_line(70, ng0);
    t2 = (t0 + 1352U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)2);
    if (t7 == 0)
        goto LAB18;

LAB19:    xsi_set_current_line(73, ng0);
    t2 = (t0 + 5770);
    xsi_report(t2, 46U, 0);
    xsi_set_current_line(74, ng0);
    t2 = (t0 + 1808U);
    t3 = *((char **)t2);
    t4 = *((int64 *)t3);
    t5 = (t4 * 3);
    t2 = (t0 + 2840);
    xsi_process_wait(t2, t5);

LAB22:    *((char **)t1) = &&LAB23;
    goto LAB1;

LAB13:    goto LAB12;

LAB15:    goto LAB13;

LAB16:    t2 = (t0 + 5706);
    xsi_report(t2, 31U, (unsigned char)1);
    goto LAB17;

LAB18:    t2 = (t0 + 5737);
    xsi_report(t2, 33U, (unsigned char)1);
    goto LAB19;

LAB20:    xsi_set_current_line(75, ng0);
    t2 = (t0 + 1512U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)3);
    if (t7 == 0)
        goto LAB24;

LAB25:    xsi_set_current_line(76, ng0);
    t2 = (t0 + 1352U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)3);
    if (t7 == 0)
        goto LAB26;

LAB27:    xsi_set_current_line(79, ng0);
    t2 = (t0 + 5880);
    xsi_report(t2, 23U, 0);
    xsi_set_current_line(80, ng0);
    t2 = (t0 + 1808U);
    t3 = *((char **)t2);
    t4 = *((int64 *)t3);
    t5 = (t4 * 10);
    t2 = (t0 + 2840);
    xsi_process_wait(t2, t5);

LAB30:    *((char **)t1) = &&LAB31;
    goto LAB1;

LAB21:    goto LAB20;

LAB23:    goto LAB21;

LAB24:    t2 = (t0 + 5816);
    xsi_report(t2, 31U, (unsigned char)1);
    goto LAB25;

LAB26:    t2 = (t0 + 5847);
    xsi_report(t2, 33U, (unsigned char)1);
    goto LAB27;

LAB28:    xsi_set_current_line(81, ng0);
    t2 = (t0 + 1512U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)3);
    if (t7 == 0)
        goto LAB32;

LAB33:    xsi_set_current_line(82, ng0);
    t2 = (t0 + 1352U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)3);
    if (t7 == 0)
        goto LAB34;

LAB35:    xsi_set_current_line(85, ng0);
    t2 = (t0 + 5967);
    xsi_report(t2, 46U, 0);
    xsi_set_current_line(86, ng0);
    t2 = (t0 + 3480);
    t3 = (t2 + 56U);
    t8 = *((char **)t3);
    t9 = (t8 + 56U);
    t10 = *((char **)t9);
    *((unsigned char *)t10) = (unsigned char)2;
    xsi_driver_first_trans_fast(t2);
    xsi_set_current_line(87, ng0);
    t2 = (t0 + 1808U);
    t3 = *((char **)t2);
    t4 = *((int64 *)t3);
    t5 = (t4 * 1);
    t2 = (t0 + 2840);
    xsi_process_wait(t2, t5);

LAB38:    *((char **)t1) = &&LAB39;
    goto LAB1;

LAB29:    goto LAB28;

LAB31:    goto LAB29;

LAB32:    t2 = (t0 + 5903);
    xsi_report(t2, 31U, (unsigned char)1);
    goto LAB33;

LAB34:    t2 = (t0 + 5934);
    xsi_report(t2, 33U, (unsigned char)1);
    goto LAB35;

LAB36:    xsi_set_current_line(88, ng0);
    t2 = (t0 + 1512U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)3);
    if (t7 == 0)
        goto LAB40;

LAB41:    xsi_set_current_line(89, ng0);
    t2 = (t0 + 1352U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)3);
    if (t7 == 0)
        goto LAB42;

LAB43:    xsi_set_current_line(92, ng0);
    t2 = (t0 + 6077);
    xsi_report(t2, 50U, 0);
    xsi_set_current_line(93, ng0);
    t2 = (t0 + 1808U);
    t3 = *((char **)t2);
    t4 = *((int64 *)t3);
    t5 = (t4 * 10);
    t2 = (t0 + 2840);
    xsi_process_wait(t2, t5);

LAB46:    *((char **)t1) = &&LAB47;
    goto LAB1;

LAB37:    goto LAB36;

LAB39:    goto LAB37;

LAB40:    t2 = (t0 + 6013);
    xsi_report(t2, 31U, (unsigned char)1);
    goto LAB41;

LAB42:    t2 = (t0 + 6044);
    xsi_report(t2, 33U, (unsigned char)1);
    goto LAB43;

LAB44:    xsi_set_current_line(94, ng0);
    t2 = (t0 + 1512U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)2);
    if (t7 == 0)
        goto LAB48;

LAB49:    xsi_set_current_line(95, ng0);
    t2 = (t0 + 1352U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)3);
    if (t7 == 0)
        goto LAB50;

LAB51:    xsi_set_current_line(98, ng0);
    t2 = (t0 + 6191);
    xsi_report(t2, 50U, 0);
    xsi_set_current_line(99, ng0);
    t2 = (t0 + 1808U);
    t3 = *((char **)t2);
    t4 = *((int64 *)t3);
    t5 = (t4 * 19);
    t2 = (t0 + 2840);
    xsi_process_wait(t2, t5);

LAB54:    *((char **)t1) = &&LAB55;
    goto LAB1;

LAB45:    goto LAB44;

LAB47:    goto LAB45;

LAB48:    t2 = (t0 + 6127);
    xsi_report(t2, 31U, (unsigned char)1);
    goto LAB49;

LAB50:    t2 = (t0 + 6158);
    xsi_report(t2, 33U, (unsigned char)1);
    goto LAB51;

LAB52:    xsi_set_current_line(100, ng0);
    t2 = (t0 + 1512U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)2);
    if (t7 == 0)
        goto LAB56;

LAB57:    xsi_set_current_line(101, ng0);
    t2 = (t0 + 1352U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)2);
    if (t7 == 0)
        goto LAB58;

LAB59:    xsi_set_current_line(104, ng0);
    t2 = (t0 + 6305);
    xsi_report(t2, 50U, 0);
    xsi_set_current_line(105, ng0);
    t2 = (t0 + 1808U);
    t3 = *((char **)t2);
    t4 = *((int64 *)t3);
    t5 = (t4 * 2);
    t2 = (t0 + 2840);
    xsi_process_wait(t2, t5);

LAB62:    *((char **)t1) = &&LAB63;
    goto LAB1;

LAB53:    goto LAB52;

LAB55:    goto LAB53;

LAB56:    t2 = (t0 + 6241);
    xsi_report(t2, 31U, (unsigned char)1);
    goto LAB57;

LAB58:    t2 = (t0 + 6272);
    xsi_report(t2, 33U, (unsigned char)1);
    goto LAB59;

LAB60:    xsi_set_current_line(106, ng0);
    t2 = (t0 + 1512U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)2);
    if (t7 == 0)
        goto LAB64;

LAB65:    xsi_set_current_line(107, ng0);
    t2 = (t0 + 1352U);
    t3 = *((char **)t2);
    t6 = *((unsigned char *)t3);
    t7 = (t6 == (unsigned char)2);
    if (t7 == 0)
        goto LAB66;

LAB67:    xsi_set_current_line(109, ng0);
    t2 = (t0 + 6419);
    xsi_report(t2, 17U, (unsigned char)3);
    xsi_set_current_line(111, ng0);

LAB70:    *((char **)t1) = &&LAB71;
    goto LAB1;

LAB61:    goto LAB60;

LAB63:    goto LAB61;

LAB64:    t2 = (t0 + 6355);
    xsi_report(t2, 31U, (unsigned char)1);
    goto LAB65;

LAB66:    t2 = (t0 + 6386);
    xsi_report(t2, 33U, (unsigned char)1);
    goto LAB67;

LAB68:    goto LAB2;

LAB69:    goto LAB68;

LAB71:    goto LAB69;

}


extern void work_a_2635101637_2372691052_init()
{
	static char *pe[] = {(void *)work_a_2635101637_2372691052_p_0,(void *)work_a_2635101637_2372691052_p_1};
	xsi_register_didat("work_a_2635101637_2372691052", "isim/ghost_tb_isim_beh.exe.sim/work/a_2635101637_2372691052.didat");
	xsi_register_executes(pe);
}
