--------------------------------------------------------------------------------
-- Den Volladdierer und die Testbench des Volladdierers haben wir aus den auf StudIP hochgeladenen Dateien entnommen.
-- Engineer:
--
-- Create Date:   08:56:25 10/22/2015
-- Design Name:   
-- Module Name:   /home/keszocze/uni/lehre/2015/WS/RAuES/repo/wise1516/exercises/vhdl/foo_adder/full_adder_tb.vhd
-- Project Name:  foo_adder
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: full_adder
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
 
ENTITY full_adder_tb IS
END full_adder_tb;
 
ARCHITECTURE behavior OF full_adder_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT full_adder
    PORT(
         a : IN  std_logic;
         b : IN  std_logic;
         c_in : IN  std_logic;
         s : OUT  std_logic;
         c_out : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal a : std_logic := '0';
   signal b : std_logic := '0';
   signal c_in : std_logic := '0';

 	--Outputs
   signal s : std_logic;
   signal c_out : std_logic;

 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: full_adder PORT MAP (
          a => a,
          b => b,
          c_in => c_in,
          s => s,
          c_out => c_out
        );

 

   -- Stimulus process
   stim_proc: process
	
	type pattern_type is record
      --inputs
      a, b, c_in : std_logic;
      -- and expected outputs
      s, c_out : std_logic;
    end record;
	 
	 
	     -- An array to put test patterns into
    type pattern_array is array (natural range <>) of pattern_type;
    -- Initialize that array
    constant patterns : pattern_array :=
      (('0','0','0','0','0'),
       ('0','0','1','1','0'),
       ('0','1','0','1','0'),
       ('0','1','1','0','1'),
       ('1','0','0','1','0'),
       ('1','0','1','0','1'),
       ('1','1','0','0','1'),
       ('1','1','1','1','1'));

   begin		

		for i in patterns'range loop
			a <= patterns(i).a;
			b <= patterns(i).b;
			c_in <= patterns(i).c_in;
			
			wait for 2 ns;
			
			assert (s = patterns(i).s) report "Wrong sum value" severity error;
			assert (c_out = patterns(i).c_out) report "Wrong carry value"
				severity error;
			
		end loop;
		
		assert false report "Bin mit dem Test durch Yea!" severity note;
      wait;
   end process;

END;
