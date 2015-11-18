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
    init     <= '0';
    dump     <= '0';
    reset    <= '0';
    re       <= '0';
    we       <= '0';
    addr     <= (others => '0');
    data_in  <= (others => '0');
  
    wait for 100 ns;
    
    
    
    -- hold reset state for 100 ns.
    
    report "Beginn der Testbench!";
    report "Reset UUT!";
    reset    <= '1';
    wait until rising_edge(clk);
    wait for (clk_period*16);
    reset    <= '0';
    wait until rising_edge(clk);
    
    --report "Init UUT!";
    --init    <= '1';
    --wait until rising_edge(clk);
    --wait for (clk_period*1000);
    --init    <= '0';
    --wait for (clk_period*100);
    --wait until rising_edge(clk);
    
    report "Write value to UUT!";
    we       <= '1';
    addr     <= "00000000";
    data_in  <= "01010101";
    wait until rising_edge(clk);
    wait for (clk_period/2);
    we       <= '0';
    data_in  <= "00000000";
    
    report "Write another value to UUT!";
    we       <= '1';
    addr     <= "00000001";
    data_in  <= "11001100";
    wait until rising_edge(clk);
    wait for (clk_period/2);
    we       <= '0';
    data_in  <= "00000000";
    
    
    report "Reread value from UUT!";
    re       <= '1';
    addr     <= "00000000";
    wait until rising_edge(clk);
    wait for (clk_period/2);
    re       <= '0';
    addr     <= "00000000";
    data_in  <= "00000000";
    assert( ack = '1' ) report "Reread 1: ack != 1" severity failure;
    assert( output = "01010101" ) report "Reread 1: out != 01010101" severity failure;
    
    
    report "Reread another value from UUT!";
    re       <= '1';
    addr     <= "00000001";
    wait until rising_edge(clk);
    wait for (clk_period/2);
    re       <= '0';
    addr     <= "00000000";
    data_in  <= "00000000";
    assert( ack = '1' ) report "Reread 2: ack != 1" severity failure;
    assert( output = "11001100" ) report "Reread 2: out != 11001100" severity failure;
    
    
    
    
    report "Overwrite value to UUT!";
    we       <= '1';
    addr     <= "00010000";
    data_in  <= "11100011";
    wait until rising_edge(clk);
    wait for (clk_period*20);
    we       <= '0';
    data_in  <= "00000000";
    
    report "Reread value from UUT!";
    re       <= '1';
    wait until rising_edge(clk);
    wait for (clk_period/2);
    re       <= '0';
    addr     <= "00000000";
    data_in  <= "00000000";
    assert( ack = '1' ) report "Reread 3: ack != 1" severity failure;
    assert( output = "11100011" ) report "Reread 3: out != 11100011" severity failure;
    
    report "Reread value from UUT!";
    re       <= '1';
    addr     <= "00000001";
    wait until rising_edge(clk);
    wait for (clk_period/2);
    re       <= '0';
    addr     <= "00000000";
    data_in  <= "00000000";
    assert( ack = '1' ) report "Reread 4: ack != 1" severity failure;
    assert( output = "11001100" ) report "Reread 4: out != 11001100" severity failure;
    
    
    report "Reread value from UUT!";
    re       <= '1';
    addr     <= "00000000";
    wait until rising_edge(clk);
    wait for (clk_period*20);
    re       <= '0';
    addr     <= "00000000";
    data_in  <= "00000000";
    assert( ack = '1' ) report "Overrread 1: ack != 1" severity failure;
    assert( output = "01010101" ) report "Overrread 1: out != 01010101" severity failure;
    
    
    
    
    
    
    
    
    
    --report "Overread value from UUT!";
    --re       <= '1';
    --addr     <= "00010000";
    --wait until rising_edge(clk);
    --wait for (clk_period/2);
    --assert( ack = '0' ) report "Overread 1: ack != 0" severity failure;
    --wait for (clk_period*100);
    --assert( ack = '1' ) report "Overread 2: ack != 1" severity failure;
    --assert( output = "00000000" ) report "Overread 2: out != 00000000" severity failure;
    --re       <= '0';
    --addr     <= "00000000";
    --data_in  <= "00000000";
    
    
    
    
    
    
    wait for (clk_period*16);
    
    
    

    
    
    
    
    
    
    
    
    
      
      
      

    -- insert stimulus here 

    report "Ende der Testbench!" severity failure;
    wait;
  end process;

END;
