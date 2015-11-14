----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    20:24:49 11/14/2015 
-- Design Name: 
-- Module Name:    Sorter - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.ALL;
use ieee.std_logic_unsigned.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Sorter is
    Port ( clk : in  STD_LOGIC;
           addr_start : in  STD_LOGIC_VECTOR (7 downto 0);
           addr_end : in  STD_LOGIC (7 downto 0);
           start : in  STD_LOGIC;
           done : out  STD_LOGIC);
end Sorter;

architecture Behavioral of Sorter is
   --Pointer, auf das aktuelel Vergleichselement zeigt.
   variable pointerCompare   : STD_LOGIC_VECTOR (7 downto 0); 
   
   --Pointer, der auf das einzusortierende Element zeigt.
   variable pointerSort      : STD_LOGIC_VECTOR (7 downto 0); 

   --Speichert Wert an pointerSort, um Speicherzugriffe zu verringern.
   variable valueToInsert   : STD_LOGIC_VECTOR (7 downto 0); 

begin
 execute: process (clk)
 begin
  if rising_edge(clk)
  then
   --Wenn Sortierung fertig ist und start immer noch '1' ist,
   --muss done auf '0' gesetzt werden und die Variablen mit
   --Startwerten gefüllt werden.
   if (start = '1' AND done ='1')
   then
      done <= '0';
      pointerCompare <= addr_start;
      pointerSort <= std_logic_vector(unsigned(addr_start)+1);
      
   end if;     
  end if;
 end process;
end Behavioral;

