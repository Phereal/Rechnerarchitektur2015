-- Der Sorter wurde als Zustandsautomat geschrieben, er basiert auf einem Bubblesort Algortihmus und entscheid anahand von IF-Klauseln,
-- in welchen Zustand er wechselt. Im folgenden werden an den expliziten stellen, die Entscheidungen beschrieben, warum sich
-- für dieses Modell entschienden wurde!

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.ALL;
use ieee.std_logic_unsigned.ALL;

-- Die Schnittstellendefinition!
entity Sorter is
    Port ( clk : in  STD_LOGIC;
           addr_start : in  STD_LOGIC_VECTOR (7 downto 0);
           addr_end : in  STD_LOGIC_VECTOR (7 downto 0);
           start : in  STD_LOGIC;
           done : out  STD_LOGIC);
end Sorter;

architecture Behavioral of Sorter is

-- Die Einbindung von Cached Memory
component CachedMemory
   Port (
    clk     : in  STD_LOGIC;
    init    : in  STD_LOGIC;
    dump    : in  STD_LOGIC;
    reset   : in  STD_LOGIC;
    re      : in  STD_LOGIC;
    we      : in  STD_LOGIC;
    addr    : in  STD_LOGIC_VECTOR (7 downto 0);
    data_in : in  STD_LOGIC_VECTOR (7 downto 0);
    output  : out  STD_LOGIC_VECTOR (7 downto 0);
    ack     : out  STD_LOGIC
   );
end component;


-- Die Signale, die für die Anbindung des CachedMemory an die Svhaltung benötzigt werden.
-- Component Signals
signal mem_init     : std_logic := '0';
signal mem_dump     : std_logic := '0';
signal mem_reset    : std_logic := '0';
signal mem_re       : std_logic := '0';
signal mem_we       : std_logic := '0';
signal mem_addr     : std_logic_vector(7 downto 0) := (others => '0');
signal mem_data_in  : std_logic_vector(7 downto 0) := (others => '0');
signal mem_output   : std_logic_vector(7 downto 0);
signal mem_ack      : std_logic := '0';

-- Der Zustandsatuomat mit sienen einzelnen Zuständen, als Typ definiert! Die einzelnen Zustände sind selbsterklären!
-- State definition
type sortState is (notStarted, -- noch nicht gestartet, start also noch nicht 1
							isStarted, -- gestartet, somit ist satrt 1
							readLeft,  -- das erste element wird in den linken bubble geladen
							counter0, -- eine Zeitverzögerung für das abwarten, bis die Leseausgabe von memory bereit ist
							leftReaden, -- das element wurde einglesen und in den lokalen signalen gespeichert!
							readRight, -- das zweite element wird in den rechten buuble geladen
							counter1, -- eine Zeitverzögerung für das abwarten, bis die Leseausgabe von memory bereit ist
							rightReaden, -- das element wurde einglesen und in den lokalen signalen gespeichert!
							areReaden, -- alle elemente wurden eingelesen
							mustBeChanged, -- ein vergleich der elemente hat ergeben, dass sie getauscht werden müssen
							changeLeft, -- der linke bubble wird in den rechten geschrieben
							counter2, -- eine Zeitverzögerung für das abwarten, bis die Schreiboperation beendet ist
							leftChanged, -- der linke bubble wurde in den rechrten geschrieben
							changeRight, -- der rechte bubble wird anhand einer temporören variable geschrieben
							counter3, -- eine Zeitverzögerung für das abwarten, bis die Schreiboperation beendet ist
							rightChanged, -- der rechte bubble wurde geschrieben
							wereChanged, -- die elemente wurde getauscht
							isFinished -- zunmindest ein vergleich der elemente wurde durchgeführt, eine überprüfung auf weitere findet statt
							);
							
-- Der Startzustand des Zustandsautomaten
signal state : sortState:= notStarted;

--Intern Signals
signal lvalue : std_logic_vector(7 downto 0) := (others => '0'); -- Inhalt der linken Speicherzelle
signal rvalue : std_logic_vector(7 downto 0) := (others => '0'); -- Inhalt der rechten Speicherzelle
signal ladd: integer := 0; -- Adresse des linke Elementes, auf eine Addressierung des rechten elementes wurde verzichtet,
-- da es immer nur die linke adresse um einen Wer inkrementiert ist und diues daher genutzt wird
signal tmpvalue: std_logic_vector(7 downto 0) := (others => '0'); -- der lokale wert um den urpsrungswert des rechten elementes zwischenzuspeichern
signal ischanged: std_logic := '0'; -- es hat ein wechsel während des durchlaufes des ersten bis zum letzten element gegeben.
signal counter: integer := 0; -- eine countervariable die für das warten auf die lese und schreiboperationen benötigt wird
signal resetdone: std_logic := '0'; -- ein flag, das sppeichert, ob ein reset durchgeführt wurde.

begin
-- Die potmap mit verlinkung auf den aktuellen clk und die erstellen temporären variablen für die Zuweisung und abfrage der Komponentne
mem: CachedMemory PORT MAP (
    clk     => clk,
    init    => mem_init,
    dump    => mem_dump,
    reset   => mem_reset,
    re      => mem_re,
    we      => mem_we,
    addr    => mem_addr,
    data_in => mem_data_in,
    output  => mem_output,
    ack     => mem_ack
);

execute: process (clk)   
-- besondere variable zur startverzögerung des Zustandsautomaten, bis die initialisierung abgeschlossen ist
variable startedCount : integer := 0;

begin
if rising_edge(clk) then -- wir reagieren nur auf die steigende taktflanke
	if(resetdone = '0') then -- überpürfung auf reset und ggf. durchführung
		mem_reset <= '1';
		resetdone <= '1';
	end if;
	-- notStarted
	-- der zustand nicht gestartet, done wird initialisiert und der addresspointer wird der startadresse zugeweisen.
	-- die initialisierung mit memory.dat wird durchgeführt! Und es findet ein wechsel in den Zustand isstarted statt.
	if(state = notStarted AND resetdone = '1') then
	mem_reset <= '0';
		if(start = '1') then
			startedCount := 0;
			done <= '0';
			ladd <= to_integer(unsigned(addr_start));
			mem_init <= '1';
			state <= isStarted;
			end if;
	end if;
	-- ##################
	-- isStarted
	-- mem_init wird zurückgesetzt und in den nächsten Zustand gewechselt.
	if(state = isStarted) then
		startedCount := startedCount + 1;
		if(startedCount >= 20) then
			report "isStarted";
			mem_init <= '0';
			state <= readLeft;  
			end if;
		end if;
	-- ##################
	-- readLeft
	-- read enable wird auf 1 gesetzt und es soll in das linke element gelesen werden
	if(state = readLeft) then
		report "readLeft";
		mem_re <= '1';
		mem_addr <= std_logic_vector(to_unsigned(ladd, mem_addr'length));
		state <= counter0;
	end if;
	-- ##################
	-- counter0
	-- eine einfache Zeitverögerung! 34 errechnete Zyklen, um sicherzugehen, dass auch der Wert durch cache und memeory bereit gestellt werden kann
	if(state = counter0) then
		if(counter < 34) then
			counter <= counter + 1;
		elsif(counter = 34) then
			report "counter0";
			counter <= 0;
			state <= leftReaden;
		end if;
	end if;
	-- ##################
	-- leftReaden
	-- Abspeicherung des linken wertes anhand der ausgabe von memory in die lokale variable.
	if(state = leftReaden AND mem_ack = '1') then
		report "leftReaden";
		mem_re <= '0';
		lvalue <= mem_output;
		state <= readRight;
	end if;
	-- ##################
	-- readRight
	-- das rechte elemente soll eingelesen werden, dazu wird wieder mem_re aktiviert.
	if(state = readRight) then
		report "readRight";
		mem_re <= '1';
		mem_addr <= std_logic_vector(to_unsigned((ladd + 1), mem_addr'length));
		state <= counter1;
	end if;
	-- ##################
	-- counter1
	-- eine einfache Zeitverögerung! 34 errechnete Zyklen, um sicherzugehen, dass auch der Wert durch cache und memeory bereit gestellt werden kann
	if(state = counter1) then
		if(counter < 34) then
			counter <= counter + 1;
		elsif(counter = 34) then
			report "counter1";
			counter <= 0;
			state <= rightReaden;
		end if;
	end if;
	-- ##################
	-- rightReaden
	-- das rechte elemente wird gespeichert und in den nächste zustand gewechselt!
	if(state = rightReaden AND mem_ack = '1') then
		report "rightReaden";
		mem_re <= '0';
		rvalue <= mem_output;
		state <= areReaden;
	end if;
	-- ##################
	-- areReaden
	-- alle elemente sind eingelesen, es wird verglichen, welches der elemente größer ist, falls das linke größer ist als das rechte wird in den zustand mustBeChanged verwiesen
	-- andernfalls direkt in isFinisched.
	if(state = areReaden) then
		report "areReaden";
		if(to_integer(unsigned(lvalue)) > to_integer(unsigned(rvalue))) then
			state <= mustBeChanged;
		else
			state <= isFinished;
		end if;
	end if;
	-- ##################
	--  mustBeChanged
	-- die variable wird gesetzt, dass ein elementwechsel stattgefunden hat.
	if(state = mustBeChanged) then
		report "mustBeChanged";
		ischanged <= '1';
		state <= changeLeft;
	end if;
	-- ##################
	-- changeLeft
	-- es wird eine anfrage gestellt, den wert des linken elementes in das rechte feld zu speichern. Der Wert des rechtne wird vorher in tmp gesichert.
	if(state = changeLeft) then
		report "changeLeft";
		mem_we <= '1';
		tmpvalue <= rvalue;
		mem_data_in <= lvalue;
		mem_addr <= std_logic_vector(to_unsigned((ladd + 1), mem_addr'length));
		state <= counter2;
	end if;
	-- ##################
	-- counter2
	-- eine einfache Zeitverögerung! 34 errechnete Zyklen, um sicherzugehen, dass auch der Wert durch cache und memeory gespeichert werden kann
	if(state = counter2) then
		if(counter < 34) then
			counter <= counter + 1;
		elsif(counter = 34) then
			report "counter2";
			counter <= 0;
			state <= leftChanged;
		end if;
	end if;
	-- ##################
	-- leftChanged
	-- das linke element wurde nun im rechten gespeichert.
	if(state = leftChanged) then
		report "leftChanged";
		mem_we <= '0';
		state <= changeRight;
	end if;
	-- ##################
	-- changeRight
	-- das rechte element wird mit dem temporäre element gespeichert, nach abschluss sind die elemtne im register vertauscht.
	if(state = changeRight) then
		report "changeRight";
		mem_we <= '1';
		mem_data_in <= tmpvalue;
		mem_addr <= std_logic_vector(to_unsigned(ladd, mem_addr'length));
		state <= counter3;
	end if;
	-- ##################
	-- counter2
	-- eine einfache Zeitverögerung! 34 errechnete Zyklen, um sicherzugehen, dass auch der Wert durch cache und memeory gespeichert werden kann
	if(state = counter3) then
		if(counter < 34) then
			counter <= counter + 1;
		elsif(counter = 34) then
			report "counter3";
			counter <= 0;
			state <= rightChanged;
		end if;
	end if;
	-- ##################
	-- rightChanged
	-- die rechte element wurd egeschrieben.
	if(state = rightChanged) then
		report "rightChanged";
		mem_we <= '0';
		state <= wereChanged;	
	end if;
	-- ##################
	-- wereChanged
	-- die elemente wurden getauscht, es kann überprüft werden, ob wir am ende des gewünschten sortierbereiches angelangt sind, wenn nicht
	-- erhöhen wir die adresse um 1 und fangen beim Zustand isStarted erneut an. Wenn wir am ende angelangt sind setzen wir das Flag isChanged
	-- auf 0 zurück und fangen wieder mit dem ersten element einen neuen sortiervorgang an.
	if(state = wereChanged) then
		report "wereChanged";
		if((ladd + 1) < to_integer(unsigned(addr_end))) then
			report "wereChanged 0";
			ladd <= ladd + 1;
			state <= isStarted;
		elsif((ladd + 1) = to_integer(unsigned(addr_end))) then
			report "wereChanged 1";
			ladd <= to_integer(unsigned(addr_start));
			ischanged <= '0';
			state <= isStarted;
		end if;
	end if;
	-- ##################
	-- isFinished
	-- es wurde kein tausch der elemnte vorgenommen [der letzen beiden elemente], wenn man nicht bei addr_end angekommen ist,
	-- wir die adresse inkrementiert und wir fangen wieder beim startzustand an.
	-- wenn wir bei addr_end angelangt sind und es hat kein wechsel im gesamten sortiervorang stattgefunden, ist die Sortierung abgeschlossen!
	if(state = isFinished) then
		report "isFinished";
		if((ladd + 1) < to_integer(unsigned(addr_end))) then
			report "isFinished 0";
			ladd <= ladd + 1;
			state <= isStarted;
		elsif((ladd + 1) = to_integer(unsigned(addr_end)) AND ischanged = '0') then
			report "isFinished 1";
			mem_re <= '0';
			mem_we <= '0';
			mem_init <= '0';
			mem_reset <= '0';
			mem_dump <= '1'; -- Alle Signale werden auf 0 gesetzt und dumo auf 1, damit der dump erstellt wird.
			done <= '1'; -- am ausgang des sortiertes wird eine 1 ausgegeben, um anzuzeigen, dass der sortiervorgang abgeschlossen ist.
		end if;
	end if;
	-- ##################
end if;
end process;
end Behavioral;