
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.STD_LOGIC_TEXTIO.ALL;

library STD;
use STD.TEXTIO.ALL;

entity Memory is
port(
	clk: in std_logic;
	init: in std_logic;
	dump: in std_logic;
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
-- Wir erstellen uns einen neuen Datentypen, der aus einem Array aus 8-Bit Wort besteht.
-- Bei 80Byte Nutzdaten und 8-Bit Worten [1Byte = 8 Bit] benötigen wir
-- einen 80x1Byte großen Array. Die Addresszuordnung wird über die Arrayposition umgesetzt.
type storage is array (0 to 79) of std_logic_vector(7 downto 0);

-- Signale
-- Wir erstellen unser 'Register' vom Type storage.
signal reg : storage := (others => "00000000");
-- Wir erstellen das lokale Signal 'iaddr', in den wir den 8 Bit Eingangsvektor addr als Integer abspeichern.
signal iaddr : integer := 0;

begin

castBVtoInt: process(addr)
begin
	-- Routine, die den Bitvektor addr in einen Integer wandelt,
	-- damit er für die Adressierung anhand der Array Position genutzt werden kann
	iaddr <= to_integer(unsigned(addr));
end process;

-- Der Prozess execute reagiert auf die steigende Taktflanke (clk)
-- und schreibt die internen Signale entsprechend der Eingänge
-- re und we in das Register oder legt den Wert am Ausgang an.
-- Zusätzich reagiert er auf die Eingänge init und dump und liest entweder
-- aus der Datei memory.dat in den Array oder gibt den Inhalt des
-- Arrays in der Datei dump.dat aus
-- Ein synchroner rest, der auf den Eingang 'reset' reagiert ist ebenfalls in diesem
-- Prozess umgesetzt.
execute: process (clk)
-- Deklaration der Hilfsvariablen zum einlesen und schreiben der Dateien
-- memory.dat und dump.dat
file iofile : text;
variable ioline : line;
variable ioaddr : integer;
variable iodata : std_logic_vector(7 downto 0);
variable iotmp : std_logic_vector(7 downto 0);

begin
	if rising_edge(clk) then
		-- Wenn am Eingang 'init' eine '1' anliegt, soll der Inhalt der Datei memory.dat in das register
		-- geladen werden.
		if (init = '1') then
			file_open(iofile,"memory.dat",read_mode);
				while not endfile(iofile) loop
					readline(iofile,ioline);
					read(ioline,iotmp);
					ioaddr := to_integer(unsigned(iotmp));
					read(ioline,iotmp);
					iodata := iotmp;
					reg(ioaddr) <= iodata;
				end loop;
			file_close(iofile);
			output <= "UUUUUUUU";
		end if;
		
		-- Wenn am Eingang 'dump' eine '1' anliegt, soll der Inhalt des Registers in die Datei dump.dat geschrieben
		-- werden.
		if(dump = '1') then
			file_open(iofile,"dump.dat",write_mode);
				for i in 0 to reg'length-1 loop
					iotmp := std_logic_vector(to_unsigned(i, iotmp'length));
					write(ioline,iotmp);
					iotmp := reg(i);
					write(ioline,iotmp);
					writeline(iofile,ioline);
				end loop;
			file_close(iofile);
			output <= "UUUUUUUU";
		end if;
		
		-- Wenn am Eingang re eine '1' anliegt, sollen die Daten, die in der Speicherzelle
		-- "addr" am Ausgang "output" angelegt werden! 
		if(re = '1') then
			output <= reg(iaddr);
		end if;
		-- Wenn am Eingang "we" eine '1' anliegt, sollen die Daten, die am Eingang "data_in"
		-- anliegen in die Speicherzelle geschrieben werden, die durch "addr" definiert ist.
		if(we = '1') then
			reg(iaddr) <= data_in;
		end if;
		
		if (reset = '1') then
		-- Wenn der Reseteingang aktiviert ist, werden alle Einträge des Registers auf 0 zurückgesetzt.
		-- Dabei wird das Array sequentiell durchlaufen.
			reg <= (others => "00000000");
		end if;
	end if;
end process;

end memory_impl;