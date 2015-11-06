
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
-- Der 8-Bit Addierer wurde von uns bereits im ersten Übungsblatt umgesetzt und wurde hier nur in das 
-- Projekt eingebunden.
component eight_bit_adder
    Port ( 
      a0 : in  STD_LOGIC;
      a1 : in  STD_LOGIC;
      a2 : in  STD_LOGIC;
      a3 : in  STD_LOGIC;
      a4 : in  STD_LOGIC;
      a5 : in  STD_LOGIC;
      a6 : in  STD_LOGIC;
      a7 : in  STD_LOGIC;
      b0 : in  STD_LOGIC;
      b1 : in  STD_LOGIC;
      b2 : in  STD_LOGIC;
      b3 : in  STD_LOGIC;
      b4 : in  STD_LOGIC;
      b5 : in  STD_LOGIC;
      b6 : in  STD_LOGIC;
      b7 : in  STD_LOGIC;
      s0 : out  STD_LOGIC;
      s1 : out  STD_LOGIC;
      s2 : out  STD_LOGIC;
      s3 : out  STD_LOGIC;
      s4 : out  STD_LOGIC;
      s5 : out  STD_LOGIC;
      s6 : out  STD_LOGIC;
      s7 : out  STD_LOGIC;
      c_out : out  STD_LOGIC);
end component;

-- Typendefinitonen
-- Wir erstellen uns einen neuen Datentypen, der je ein 8-Bit Wort speichern kann,
-- dieses Wort wird einer 8-Bit Adresse zuegordnet, die später initial geschrieben wird.
type cell is record
	address:	std_logic_vector(7 downto 0);
	data:		std_logic_vector(7 downto 0);	 
end record;
-- Bei 80Byte Nutzdaten und 8-Bit Worten [1Byte = 8 Bit] benötigen wir
-- einen 80x1Byte großen Array.
type storage is array (0 to 79) of cell;

-- Signale
-- Wir erstellen unser 'Register' vom Type storage.
signal reg : storage;
-- Diese Zählervariable benötigen wir um für lese- und schreibprozesse mit einer
-- gegebenen Adresse die zugehörige Speicherzelle herauszufinden. Dieser Suchvorgang
-- wird mit einer while-Schleife durchgeührt und die zählervariable wird solange
-- erhöht, bis die entsprechende Adresse gefunden ist!
signal i : integer := 0;
-- In der Variable [oder in dem Signal] storeident wird die Arrayposition
-- von i zwischengespeichert in dem die übereinstimmende Adresse gefunden wurde.
-- Später wird auf eine steigende Taktflanke der anliegende Wert dann in das
-- zugehörige Datenfeld geschrieben.
signal storeident : integer := 0;
-- dataout dient dazu, die Daten, die in der betreffenden Adresse gefunden wurden
-- zwischenzuspeichern, damit sie erst auf die nächste steigende Taktflanke
-- am Ausgang angelegt werden können.
signal dataout: std_logic_vector(7 downto 0) := "00000000";
-- storeEx soll signalisieren, dass der asynchrone Teil der Schaltung eine
-- store-Operation druchgeführt hat und der Wert jetzt mit der nächsten
-- steigenden Taktflanke in das Register geschrieben werden kann!
signal storeEx: std_logic := '0';
-- storeEx soll signalisieren, dass der asynchrone Teil der Schaltung eine
-- load-Operation druchgeführt hat und der Wert jetzt mit der nächsten
-- steigenden Taktflanke an den Ausgang angelegt werden kann!
signal loadEx: std_logic := '0';
-- Die beiden temporären Bitvektoren tmp0 und tmp1 dienen dazu, den 8-Bit Addierer
-- aus dem initalize-Prozess ansteuern zu können.
signal tmp0: std_logic_vector(7 downto 0) := "00000000";
signal tmp1: std_logic_vector(7 downto 0) := "00000000";
-- Das Signal zero ist mit '0' festverdrahtet und soll beim 8-Bit Addierer
-- fest auf das carry-out gelegt werden, da dieses hier nicht genutzt wird.
signal zero : std_logic := '0';

begin
-- Der 8-Bit Addierer soll nur einmal alle Adresse in das Register
-- schreiben, wenn init = '1' ist und ansonste nichts tun.
eba0: eight_bit_adder PORT MAP(
      tmp0(0),tmp0(1),tmp0(2),tmp0(3),tmp0(4),tmp0(5),tmp0(6),tmp0(7),
		'0','0','0','0','0','0','0','1',
      tmp1(0),tmp1(1),tmp1(2),tmp1(3),tmp1(4),tmp1(5),tmp1(6),tmp1(7),
      zero);

-- Die Adressen müssen in die Adressfelder des Arrays geschrieben werden.
-- Mit 2⁸ Bit Adresse können maximal 256 Speicherzellen adressiert werden,
-- wir benötigen allerdings nur eine Adressierung für 80 Speicherzellen.
initialize: process(init)
begin
	if(init = '1') then
		reg(i).address <= "00000000";
		for i in 1 to reg'length-1 loop
			tmp0 <= reg(i-1).address;
			reg(i).address <= tmp1;
		end loop;
	end if;
end process;

-- Wenn am Eingang re eine '1' anliegt, sollen die Daten, die in der Speicherzelle
-- "addr" am Ausgang "output" angelegt werden! Diese werden durch die load Operation
-- zuerst in das lokale Signal 'dataout' geschrieben und mit dem Prozess changing_states
-- auf die nächste steigende Taktflanke dann am Ausgang angelegt! Wenn wieder eine '0' anliegt,
-- kann die Zählervariable auf den Wert 0 zurückgestezt werden um für den nächsten load-Vorgang
-- initialisiert zu sein.
load: process(re)
begin
	if(re = '1') then
		while reg(i).address /= addr loop
			i <= i + 1;		
		end loop;
		dataout <= reg(i).data;
		loadEx <= '1';
	end if;
	if(re = '0') then
		i <= 0;
	end if;
end process;

-- Wenn am Eingang "we" eine '1' anliegt, sollen die Daten, die am Eingang "data_in"
-- anliegen in die Speicherzelle geschrieben werden, die durch "addr" definiert ist.
-- Dazu wird im folgenden Prozess die betreffende Position im Array anhand des Einganges
-- "addr" per sequentieller suche definiert und diese Position in der Variable storeident 
-- zwischengespeichert! Zusätzlich wird storeEx auf '1' gesetzt, damit auf die nächste steigende
-- Taktflanke der Wert in das betreffende Register per Prozess changing_states geschrieben
-- werden kann.
store: process(we)
begin
	if(we = '1') then
		while reg(i).address /= addr loop
			i <= i + 1;		
		end loop;
		storeident <= i;
		storeEx <= '1';
	end if;
	if(we = '0') then
		i <= 0;
	end if;
end process;

-- Der Prozess flush lösche Alle Einträge des Registers und setzt sie initial auf 0 zurück.
-- Dabei wird das Array sequentiell durchlaufen.
flush: process(reset)
begin
	for i in 0 to reg'length loop
		reg(i).data <= "00000000";
	end loop;
end process;

-- Der Prozess changing_states reagiert auf die steigende Taktflanke (clk)
-- und schreibt die internen Signale entsprechend den Zuständen der Variablen
-- storeEx und loadEx in das Register oder legt den Wert am Ausgang an.
changing_states: process(clk)
begin
	if(rising_edge(clk)) then
		if(storeEx = '1') then
			reg(storeident).data <= data_in;
			storeEx <= '0';
		end if;
		if(loadEx = '1') then
			output <= dataout;
			loadEx <= '0';
		end if;
	end if;
end process;

end memory_impl;