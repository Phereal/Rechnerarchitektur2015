--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: P.20131013
--  \   \         Application: netgen
--  /   /         Filename: Sorter_synthesis.vhd
-- /___/   /\     Timestamp: Tue Nov 17 22:29:45 2015
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -ar Structure -tm Sorter -w -dir netgen/synthesis -ofmt vhdl -sim Sorter.ngc Sorter_synthesis.vhd 
-- Device	: xc7a100t-3-csg324
-- Input file	: Sorter.ngc
-- Output file	: /home/daalta/Rechnerarchitektur2015/Uebung 2/A3_Sorter/netgen/synthesis/Sorter_synthesis.vhd
-- # of Entities	: 1
-- Design Name	: Sorter
-- Xilinx	: /opt/Xilinx/14.7/ISE_DS/ISE/
--             
-- Purpose:    
--     This VHDL netlist is a verification model and uses simulation 
--     primitives which may not represent the true implementation of the 
--     device, however the netlist is functionally correct and should not 
--     be modified. This file cannot be synthesized and should only be used 
--     with supported simulation tools.
--             
-- Reference:  
--     Command Line Tools User Guide, Chapter 23
--     Synthesis and Simulation Design Guide, Chapter 6
--             
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
use UNISIM.VPKG.ALL;

entity Sorter is
  port (
    clk : in STD_LOGIC := 'X'; 
    start : in STD_LOGIC := 'X'; 
    done : out STD_LOGIC; 
    addr_start : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
    addr_end : in STD_LOGIC_VECTOR ( 7 downto 0 ) 
  );
end Sorter;

architecture Structure of Sorter is
  component Memory
    port (
      clk : in STD_LOGIC := 'X'; 
      init : in STD_LOGIC := 'X'; 
      dump : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      re : in STD_LOGIC := 'X'; 
      we : in STD_LOGIC := 'X'; 
      addr : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
      data_in : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
      output : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
    );
  end component;
  signal N1 : STD_LOGIC; 
  signal NLW_mem_mem_output_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_mem_output_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_mem_output_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_mem_output_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_mem_output_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_mem_output_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_mem_output_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_mem_output_0_UNCONNECTED : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => N1
    );
  mem_mem : Memory
    port map (
      clk => N1,
      init => N1,
      dump => N1,
      reset => N1,
      re => N1,
      we => N1,
      addr(7) => N1,
      addr(6) => N1,
      addr(5) => N1,
      addr(4) => N1,
      addr(3) => N1,
      addr(2) => N1,
      addr(1) => N1,
      addr(0) => N1,
      data_in(7) => N1,
      data_in(6) => N1,
      data_in(5) => N1,
      data_in(4) => N1,
      data_in(3) => N1,
      data_in(2) => N1,
      data_in(1) => N1,
      data_in(0) => N1,
      output(7) => NLW_mem_mem_output_7_UNCONNECTED,
      output(6) => NLW_mem_mem_output_6_UNCONNECTED,
      output(5) => NLW_mem_mem_output_5_UNCONNECTED,
      output(4) => NLW_mem_mem_output_4_UNCONNECTED,
      output(3) => NLW_mem_mem_output_3_UNCONNECTED,
      output(2) => NLW_mem_mem_output_2_UNCONNECTED,
      output(1) => NLW_mem_mem_output_1_UNCONNECTED,
      output(0) => NLW_mem_mem_output_0_UNCONNECTED
    );

end Structure;

