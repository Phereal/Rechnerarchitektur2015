
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity memory is
port(
	clk: in std_logic;
	init: in std_logic;
	reset: in std_logic;
	re: in std_logic;
	we: in std_logic;
	addr: in std_logic_vector(7 downto 0);
	data_in: in std_logic_vector(7 downto 0);
	output: out std_logic_vector(7 downto 0)
);
end memory;

architecture memory_impl of memory is

-- Komponenten

-- Typendefinitonen
type cell is record
	address:	std_logic_vector(7 downto 0);
	data:		std_logic_vector(7 downto 0);	 
end record;

type storage is array (o to 79) of cell;

-- Signale
signal reg : storage;
signal i : integer := 0;

begin

initialize: process(init)
begin
	ram(i).address <= "00000000";
	for i in 1 to reg'range loop
		ram(i).address <= reg(i-1).address + "1";
	end loop;
end process;

load: process(re)
begin
	if rising_edge(clk) then
		while reg(i).address /= addr loop
			i <= i + 1;		
		end loop;
		output <= reg(i).data;
	end if;
i <= 0;
end process;

store: process(we)
begin
	if rising_edge(clk) then
		while reg(i).address /= addr loop
			i <= i + 1;		
		end loop;
		reg(i).data <= data_in;
	end if;
i <= 0;
end process;

flush: process(reset)
begin
	for i in 0 to reg'range loop
		ram(i).data <= "00000000";
	end loop;
end process;

end memory_impl;