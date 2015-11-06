--Den Volladdierer und die Testbench des Volladdierers haben wir aus den auf StudIP hochgeladenen Dateien entnommen.
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity full_adder is
    Port ( a : in  STD_LOGIC;
           b : in  STD_LOGIC;
           c_in : in  STD_LOGIC;
           s : out  STD_LOGIC;
           c_out : out  STD_LOGIC);
end full_adder;

architecture Behavioral of full_adder is

begin
	s <= a xor b xor c_in;
	c_out <= (a and b) or (a and c_in) or (b and c_in);
end Behavioral;
