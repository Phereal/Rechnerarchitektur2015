--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   17:32:27 11/15/2015
-- Design Name:   
-- Module Name:   /home/steffen/Dokumente/Git/GitHub/Rechnerarchitektur2015/Uebung 2/A2_CachedMemory/CachedMemoryTb.vhd
-- Project Name:  A2_CachedMemory
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: CachedMemory
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
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY CachedMemoryTb IS
END CachedMemoryTb;
 
ARCHITECTURE behavior OF CachedMemoryTb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT CachedMemory
    PORT(
         clk : IN  std_logic;
         init : IN  std_logic;
         dump : IN  std_logic;
         reset : IN  std_logic;
         re : IN  std_logic;
         we : IN  std_logic;
         addr : IN  std_logic_vector(7 downto 0);
         data_in : IN  std_logic_vector(7 downto 0);
         output : OUT  std_logic_vector(7 downto 0);
         ack : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal clk : std_logic := '0';
   signal init : std_logic := '0';
   signal dump : std_logic := '0';
   signal reset : std_logic := '0';
   signal re : std_logic := '0';
   signal we : std_logic := '0';
   signal addr : std_logic_vector(7 downto 0) := (others => '0');
   signal data_in : std_logic_vector(7 downto 0) := (others => '0');

 	--Outputs
   signal output : std_logic_vector(7 downto 0);
   signal ack : std_logic;

   -- Clock period definitions
   constant clk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: CachedMemory PORT MAP (
          clk => clk,
          init => init,
          dump => dump,
          reset => reset,
          re => re,
          we => we,
          addr => addr,
          data_in => data_in,
          output => output,
          ack => ack
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
      -- hold reset state for 100 ns.
      wait for 100 ns;	

      wait for clk_period*10;

      -- insert stimulus here 

      wait;
   end process;

END;
