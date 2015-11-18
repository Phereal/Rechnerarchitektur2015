
-- Okay in dieser Aufgabe ist es Ziel, ein Register [oder Arbeitsspeicher] in VHDL zu schreiben.
-- Das Register wird intern mit einem Array aus 80 x 8 Bit Vektoren dargestellt. Die
-- Adressierung wird über die Position des Arrayeleemntes vorgenommen. Da Arrayelemente allerdings
-- mit einem Integer addressiert werden und wir hier einen 'addr' Eingang haben der aus einem 8 Bit
-- Vektor besteht, müssen wir diesen Vektor in einen Integer konvertieren. Später müssen wir die
-- Arraypositon in eine Datei schreiben,hier müssen wir den Integer wieder zurück in einen Bitvektor
-- konvertieren.

-- Für diese Typkonvertierungen laden wir die Bibliotheken IEEE.numeric_std.ALL und IEEE.STD_LOGIC_UNSIGNED.ALL,
-- da diese über entsprechende Methoden verfügen um diese Konvertierungen durchzuführen.

-- Die Libraries use IEEE.STD_LOGIC_TEXTIO.ALL; und use STD.TEXTIO.ALL benötigen wir um aus Dateien zu lesen
-- und auch um in Dateien zu schreiben. sie bieten uns also Methoden um Dateioperationen auszuführen.
-- Diese benötigen wir, weil wir aus der Datei 'memory.dat' in das Register schreiben wollen und 
-- aus dem Register in die Datei dum.dat

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.STD_LOGIC_TEXTIO.ALL;

library STD;
use STD.TEXTIO.ALL;

-- Die schnittstellen waren vorgegeben und müssen daher hier nicht explizit kommentiert werden!

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
-- Zusätzlich haben wir ein Arrayelement, Element 80, dass für falsche Eingabe dient!
type storage is array (0 to 80) of std_logic_vector(7 downto 0);

-- Signale
-- Wir erstellen unser 'Register' vom Type storage. Initial werden alle Registerelemente mit einem
-- leeren Bitvektor gefüllt. Nur Element 80 im Array wird vordefiniert für falsche Adresswerte!
signal reg : storage := (80 => "XXXXXXXX", others => "00000000");


begin
-- Der Prozess execute reagiert auf die steigende Taktflanke (clk)
-- und schreibt die internen Signale entsprechend der Eingänge
-- re und we in das Register oder legt den Wert am Ausgang an.
-- Zusätzich reagiert er auf die Eingänge init und dump und liest entweder
-- aus der Datei memory.dat in den Array oder gibt den Inhalt des
-- Arrays in der Datei dump.dat aus
-- Ein synchroner Reset, der auf den Eingang 'reset' reagiert ist ebenfalls in diesem
-- Prozess umgesetzt.
-- Wir haben anhand der IF-Klauseln eine Fallunterscheidung eingeführt, sodass nur gültige
-- Eingabgen an den Eingängen auch zu einem Ergebnis am Ausgang führen können,
-- in allen Fällen die wir nicht anhand der fallunterscheidung betrachten geben wir
-- am Ausgang 'output' einen Bitvektor aus, der mit 'X' gefüllt ist, um einen
-- Fehler zu signalisieren
-- Hinweis: Auch in den Lese- und Schreibphasen in und aus memory.dat und dump.dat
-- liegt, wie in der Aufgabenstellung gefordert ein Bitvektor aus 'X' an
-- output an.
execute: process (clk)
-- Deklaration der Hilfsvariablen zum Einlesen und Schreiben der Dateien
-- memory.dat und dump.dat
file iofile : text;
variable ioline : line;
variable ioaddr : integer;
variable iodata : std_logic_vector(7 downto 0);
variable iotmp : std_logic_vector(7 downto 0);

-- Wir erstellen die lokale Variable 'iaddr', in den wir den 8 Bit Eingangsvektor addr als Integer
-- abspeichern wollen.
variable iaddr : integer range 0 to 255 := 0;

begin
	-- Wie in der Aufgabenstellung gefordert reagieren wir nur auf die Steigende Taktflanke von clk
	-- wir bauen also eine synchrone Schaltung!
	if rising_edge(clk) then
		-- Routine, die den Bitvektor addr in einen Integer wandelt,
		-- damit er für die Adressierung anhand der Array Position genutzt werden kann
		-- Dabei werden nur Interger Werte angenommen, die kleiner als 80 sind
		-- Somit werden OutofBounds Exception verhindert!
		iaddr := to_integer(unsigned(addr));
		if(iaddr >= 80) then
			iaddr := 80;
			report "Array Index out of Bounds: " & integer'image(iaddr);
		end if;
		-- Wenn am Eingang 'init' eine '1' anliegt, soll der Inhalt der Datei memory.dat in das register
		-- geladen werden. Die Fallunterscheidung verbietet eine '1' an einem anderen Eingang.
		if (init = '1' AND dump = '0' AND reset = '0' AND re = '0' AND we = '0') then
			-- Wir öffnen die Datei 'memory.dat' im reademode und lesen Sie Zeilenweise ein,
			-- die ersten 8 Bits jeder Zeile konvertieren wir on the fly in einen Integerwert,
			-- der und die Speicherposition im Array angibt und die zweiten 8 Bits
			-- sind dann die Daten, die wir in die betreffende Speicherzelle schreiben.
			-- Diesen Vorgang wiederholen wir so lange, bis wir das Ende der Datei erreicht haben.
			file_open(iofile,"memory.dat",read_mode);
				while not endfile(iofile) loop
					readline(iofile,ioline);
					read(ioline,iotmp);
					ioaddr := to_integer(unsigned(iotmp));
					read(ioline,iotmp);
					iodata := iotmp;
					reg(ioaddr) <= iodata;
				end loop;
			-- Wenn wir alles eingelesen haben müssen wir die Datei wieder schließen.
			file_close(iofile);
			-- Nach Abschluss des Einlesevorgangsgeben legen wir einen Bitvektor auf den Ausgang. Dies müssen wir tun,
			-- da wir in Xilinx normalerweise keine Dateieingaben und Ausgaben schreiben können, da wir ja Hardware bauen.
			-- Da die Dateieingabe und Ausgabe also nicht als Signale gelten würden wir die Eingänge init und dump nicht
			-- benutzen und sie würden bei der Synthese wegoptimiert. Daher ein 8 Bit Vektor bestehend aus 'X' am Ausgang.
			output <= "XXXXXXXX";
		-- Wenn am Eingang 'dump' eine '1' anliegt, soll der Inhalt des Registers in die Datei dump.dat geschrieben
		-- werden. Die Fallunterscheidung verbietet eine '1' an einem anderen Eingang.
		elsif(dump = '1' AND init = '0' AND reset = '0' AND re = '0' AND we = '0') then
			-- Wir öffnen die Datei 'dump.dat' im writemode und schreiben den Inhalt unseres Registers
			-- Zeilenweise in die Datei. Dazu verwenden wir eine for-Schleife die bei null beginnt zu
			-- zählen und verwenden diesen counter als Adresse, da wir das Array sequentiell durchlaufen.
			-- Dabei geben wir durch die Typkonvertierung immer einen vollen 8 Bit Vektor als Adresse
			-- in die datei ein.
			-- Den Inhalt der betreffenden Speicherzelle an der Counter-Position des Arrays
			-- schreiben wir dann in die selbe Zeile. Wir schreiben immer den gesamten
			-- Inhalt des Arrays in die Datei.
			-- Diesen Vorgang wiederholen wir so lange, bis wir das Ende des Arrays erreicht haben.
			file_open(iofile,"dump.dat",write_mode);
				for i in 0 to reg'length-1 loop
					iotmp := std_logic_vector(to_unsigned(i, iotmp'length));
					write(ioline,iotmp);
					iotmp := reg(i);
					write(ioline,iotmp);
					writeline(iofile,ioline);
				end loop;
			-- Wenn wir alles geschrieben haben müssen wir die Datei wieder schließen.
			file_close(iofile);
			-- Nach Abschluss des Schreibevorgangs legen wir einen Bitvektor auf den Ausgang. Dies müssen wir tun,
			-- da wir in Xilinx normalerweise keine Dateieingaben und Ausgaben schreiben können, da wir ja Hardware bauen.
			-- Da die Dateieingabe und Ausgabe also nicht als Signale gelten würden wir die Eingänge init und dump nicht
			-- benutzen und sie würden bei der Synthese wegoptimiert. Daher ein 8 Bit Vektor bestehend aus 'X' am Ausgang.
			output <= "XXXXXXXX";
		-- Wenn am Eingang re eine '1' anliegt, sollen die Daten, die in der Speicherzelle
		-- "addr" am Ausgang "output" angelegt werden! 
		-- Die Fallunterscheidung verbietet eine '1' an einem anderen Eingang.
		elsif(re = '1' AND dump = '0' AND init = '0' AND reset = '0' AND we = '0') then
			output <= reg(iaddr);
		-- Wenn am Eingang "we" eine '1' anliegt, sollen die Daten, die am Eingang "data_in"
		-- anliegen in die Speicherzelle geschrieben werden, die durch "addr" definiert ist.
		-- Die Fallunterscheidung verbietet eine '1' an einem anderen Eingang.
		elsif(we = '1' AND re = '0' AND dump = '0' AND init = '0' AND reset = '0') then
			reg(iaddr) <= data_in;
		-- Wenn der Reseteingang aktiviert ist, werden alle Einträge des Registers auf 0 zurückgesetzt.
		-- Dabei wird das Array sequentiell durchlaufen.
		-- Die Fallunterscheidung verbietet eine '1' an einem anderen Eingang.
		elsif(reset = '1' AND we = '0' AND re = '0' AND dump = '0' AND init = '0') then
			reg <= (80 => "XXXXXXXX", others => "00000000");
		-- Wenn keiner der oben genannten Fälle abgefangen wurde, muss es sich um eine fehlerhafte Belegung der
		-- Eingänge handeln. In diesem Fall verändern wir den Inhalt des Registers nicht und geben nur einen
		--	Bitvektor gefüllt mit 'X' an den Ausgang.
		else
			output <= "XXXXXXXX";
		end if;
	end if;
end process;

end memory_impl;