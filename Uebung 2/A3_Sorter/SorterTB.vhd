--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   14:54:35 11/18/2015
-- Design Name:   
-- Module Name:   /home/daalta/Rechnerarchitektur2015/Uebung 2/A3_Sorter/SorterTB.vhd
-- Project Name:  A3_Sorter
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: Sorter
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY SorterTB IS
END SorterTB;
 
ARCHITECTURE behavior OF SorterTB IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT Sorter
    PORT(
         clk : IN  std_logic;
         addr_start : IN  std_logic_vector(7 downto 0);
         addr_end : IN  std_logic_vector(7 downto 0);
         start : IN  std_logic;
         done : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal clk : std_logic := '0';
   signal addr_start : std_logic_vector(7 downto 0) := (others => '0');
   signal addr_end : std_logic_vector(7 downto 0) := (others => '0');
   signal start : std_logic := '0';

 	--Outputs
   signal done : std_logic;

   -- Clock period definitions
   constant clk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: Sorter PORT MAP (
          clk => clk,
          addr_start => addr_start,
          addr_end => addr_end,
          start => start,
          done => done
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      
      report "Testbench beginnt.";
      report "Inputs auf 0 setzen...";
      
      addr_start  <= "00000000";
      addr_end    <= "00000000";
      start       <= '0';
      
      report "Inputs auf 0 gesetzt.";
      
      -- hold reset state for 100 ns.

      wait for clk_period*10;

      -- insert stimulus here 
      
      report "Input auf gesamten Speicherbereich setzen und Sortierung starten...";
      
      addr_start  <= "00000000";
      addr_end    <= "01001111";
      start       <= '1';
      
      report "Sortierung gestartet. Warte auf done-Leitung..";

      wait until done = '1';
      
      wait for clk_period * 30;
      
      report "Hier würde der dump überprüft werden.";
      
      start <= '1';
      
      wait for clk_period * 9999;
      
      wait;
      
   end process;

END;
