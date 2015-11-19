----------------------------------------------------------------------------------
--Aufgabe 3 konnten wir nicht rechtzeitig in eine funktionierende Form bringen.
--Unsere Testbench demonstriert in der Konsole recht gut die Idee unseres Lösungsansatzes,
--zeigt allerdings die zahlreichen Fehler, die beim Schreiben in den Speicher auftreten.
-- Company: 
-- Engineer: 
-- 
-- Create Date:    20:24:49 11/14/2015 
-- Design Name: 
-- Module Name:    Sorter - Behavioral 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.ALL;
use ieee.std_logic_unsigned.ALL;


entity Sorter is
    Port ( clk : in  STD_LOGIC;
           addr_start : in  STD_LOGIC_VECTOR (7 downto 0);
           addr_end : in  STD_LOGIC_VECTOR (7 downto 0);
           start : in  STD_LOGIC;
           done : out  STD_LOGIC);
end Sorter;

architecture Behavioral of Sorter is


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

  signal mem_clk      : std_logic := '0';
  signal mem_init     : std_logic := '0';
  signal mem_dump     : std_logic := '0';
  signal mem_reset    : std_logic := '0';
  signal mem_re       : std_logic := '0';
  signal mem_we       : std_logic := '0';
  signal mem_addr     : std_logic_vector(7 downto 0) := (others => '0');
  signal mem_data_in  : std_logic_vector(7 downto 0) := (others => '0');
  signal mem_output   : std_logic_vector(7 downto 0);
  signal mem_ack      : std_logic := '0';

begin
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
    ack     => mem_ack);

 execute: process (clk)
   --Verzögert Sortierung, damit init den Speicher füllen kann.
   --Erinnerung: Muss bei dump wieder auch Ursprungswert gestellt werden!
   variable initDelay : Integer := 500;
   
   variable isRunning   : STD_LOGIC                   := '0';--Gibt an, ob Sortierungg läuft.
   variable pointer     : std_logic_vector(7 downto 0):= addr_start;--Speicheradresse für Vergleiche im Speicher.
   
   variable currentValue: std_logic_vector(7 downto 0);--Aktueller zwischengespeicherter Vergleichswert
   variable nextValue   : std_logic_vector(7 downto 0);--Nächster zwischengespeicherter Vergleichswert (zwischengespeichert)
   variable currentValueValid: STD_LOGIC              := '0';--Gibt an, ob currentValue aktualisiert werden muss.
   variable nextValueValid: STD_LOGIC                 := '0';--Gibt an, ob nextValue aktualisiert werden muss.
   
   variable getOutput: STD_LOGIC                      := '0';--Gibt an, ob der Output des Speichers bereit zum auslesen ist.
   variable waitForOutput: STD_LOGIC                  := '0';--Gibt an, ob auf den Output des Speichers überhaupt zu warten ist.
   variable amountCorrectLastDigits: Integer          := 0; --Anzahl korrekter Adressen am Ende des Adressraums. Erhöht sich nach und nach.
   variable sortAgain: STD_LOGIC                      := '0';--Gibt an, ob im aktuellen Sortierdurchlauf zwei Werte getauscht wurden.
   variable replaceCurrentValue: STD_LOGIC            := '0';--Gibt an, ob currentValue einen Wert enthält, der noch in den Speicher geschrieben werden muss.
   
   variable saveValue    : std_logic_vector(7 downto 0);--Gibt den zu rettenden Wert (ehemals in currentValue) an.
   variable saveAddress   : std_logic_vector(7 downto 0);--Gibt die Adresse des zu rettenden Wertes an.
   variable saveLastValuePreviousSort : std_logic     := '0';--Gibt an, ob currentValue am Ende des letztes Sortierdurchlaufes
                                                             --*nicht* in den Speicher geschrieben konnte und daher anschließend
                                                             --noch gespeichert werden muss.
   
   variable internalDone : std_logic                  := '0'; --zur Verzögerung der Umstellung von done
 
begin
 if rising_edge(clk)
 then
  
   if(start = '1' AND isRunning = '0')
   then
      --Initialisiere zahlreiche Werte.
      mem_dump <= '0';
      pointer := addr_start;
      isRunning := '1';
      initDelay := 500;
      done <= '0';
      saveLastValuePreviousSort := '0';
      internalDone := '0';
   end if;
  
   if(isRunning = '1')
   then
      --initDelay wird genutzt, um dem Speicher genug Zeit zu geben, init auszuführen.
      if (initDelay = 500)
      then
         report "Habe mem_init auf 1 gesetzt und warte jetzt einen Takt.";
         mem_init <= '1';
      end if;
      
      if (initDelay = 499)
      then
         report "Habe mem_init wieder auf 0 gesetzt.";
         report "Warte noch ein paar hundert Zyklen, um init Zeit zu lassen.";
         mem_init <= '0';
      end if;
      
      --delay runerzählen.
      if (initDelay > 0)
      then
         initDelay := initDelay-1;
      end if;      
   
      --Nach Verzögerung:
      if(isRunning ='1' AND initDelay = 0)
      then

         --Wir laden, wenn nötig, den aktuellen Wert aus dem Speicher.
         --currentValue wird nur zu Beginn eines Suchdurchlaufes abgerufen,
         --da nextValue genügt, um currentValue für den jeweils nächsten Schritt
         --festzulegen.
         if(currentValueValid = '0')
         then 
            if (getOutput = '0')
            then
               mem_addr <= pointer;
               mem_we <= '0';
               mem_re   <= '1';
               getOutput := '1';
            
            else 
               if (getOutput = '1' AND mem_ack = '1')
               then
                  --Speicher hat ack auf 1 gesetzt, daher ist der Wert nun abrufbar.
                  report "currentValue: " &integer'image(to_integer(unsigned(mem_output)));
                  currentValue := mem_output;
                  currentValueValid := '1';
                  getOutput := '0';
               end if;
            end if;
         end if;

         --Ähnlich wie für currentValue wird der benötigte Wert aus dem Speicher geladen.
         if(currentValueValid = '1' AND nextValueValid = '0')
         then 
            if (getOutput = '0')
            then
               mem_addr <= std_logic_vector(unsigned(pointer) + 1);
               mem_we <= '0';
               mem_re   <= '1';
               getOutput := '1';
            
            else 
               if (getOutput = '1' AND mem_ack = '1')
               then
                  report "nextValue: " &integer'image(to_integer(unsigned(mem_output)));
                  nextValue := mem_output;
                  nextValueValid := '1';
                  getOutput := '0';
               end if;
            end if;
         end if;
         
         --Wenn im vorherigen Durchlauf als letzter Schritt eine Sortierung statt fand,
         --wird der 'gerettete' letzte Wert nun noch kurz gespeichert.
         --Dies ist vonnöten, da in einem solchen Fall 2 writes im gleichen Clock cycle
         --ausgeführt werden würden. Das wird so vermieden.
         if(saveLastValuePreviousSort='1' AND currentValueValid = '1' AND nextValueValid = '1')
         then
            report "Saving last value.";
            report "#Write " &integer'image(to_integer(unsigned(saveValue))) & " to address" &integer'image(to_integer(unsigned(saveAddress)));
            mem_addr <= saveAddress;
            mem_data_in<= saveValue;
            mem_re <= '0';
            mem_we <= '1';
            saveLastValuePreviousSort:='0';
            initDelay := 100;
         end if;
         
         --Wenn wir 'intern* fertig sind und kein Wert mehr zu retten ist, können wir den Wert dumpen.
         if (internalDone = '1' AND saveLastValuePreviousSort='0')
         then
            report "Set done to true.";
            initDelay := 5;
            mem_re <= '0';
            mem_we <= '0';
            mem_reset <= '0';
            mem_init <= '0';
            mem_dump <= '1';
            done <= '1';
            isRunning := '0';
         end if;
            
         
         --Im folgenden Codeblock wird festgestellt, was sortiert wird.
         --Allerdings wird bei jedem Durchlauf nur 1 Schreibvorgang durchgeführt!
         --Dies spart Zeit, da der Speicher 1 Clock cycle zum speichern benötigt.
         --
         --1. Fall
         --Wenn der aktuelle Wert größer ist als der nächste Wert, wird der aktuelle Wert
         --durch den nächsten ersetzt. 
         --currentValue wird nicht verändert. Somit hat der Speicher zu diesem Zeitpunkt
         --einen doppelten Eintrag! Der 2. Fall oder das Erreichen des Endes des Adressraumes
         --behebt diese Inkonsistenz.
         --
         --2. Fall
         --Wenn die beiden Werte in der korrekten Reihenfolge sind, wird die aktuelle Adresse
         --mit dem zwischengespeicherten aktuellen Wert überschrieben, wenn der 1. Fall unmittel-
         --bar zuvor auftrat.
         if(currentValueValid = '1' AND nextValueValid = '1' AND saveLastValuePreviousSort = '0' AND internalDone = '0')
         then
            --In jedem Fall muss der nächste Wert gleich neu geladen werden.
            nextValueValid := '0';
            report "pointer: " & integer'image(to_integer(unsigned(pointer)));
            report "currentValue: " &integer'image(to_integer(unsigned(currentValue)))& ", nextValue: " &integer'image(to_integer(unsigned(nextValue)));
            mem_re <= '0'; --Auf keinen Fall etwas lesen!
            
            if (currentValue>nextValue)
            then  --1. Fall
            --Werte tauschen
               report "#Tausche currentValue und nextValue, weil nextValue kleiner war.";
               report "#Write " &integer'image(to_integer(unsigned(nextValue)))& " (next) to adress" &integer'image(to_integer(unsigned(pointer)));
               
               initDelay := 100;
               mem_addr <= pointer;
               mem_data_in <= nextValue;
               mem_we <= '1';
               sortAgain := '1';
               replaceCurrentValue := '1';
               --currentValue bleibt gleich, da es nach vorne verschoben wurde!
               --Daher wird currentValue kein neuer Wert zugewiesen.
               
            else --2. Fall
               --Werte bereits in richtiger Reihefolge
               report "#Die Werte waren bereits in korrekter Reihenfolge.";
               report "#Write " &integer'image(to_integer(unsigned(currentValue)))
                      & " (curr) to adress " &integer'image(to_integer(unsigned(pointer)))
                      & " (just in case.)";
               initDelay := 100;
               replaceCurrentValue := '0';
               mem_addr <= pointer;
               mem_data_in <= currentValue;
               mem_we <= '1';
               mem_re <= '0';
               
               currentValue := nextValue;
               
            end if;
            
            --Pointer für nächsten Schritt vorbereiten.
            pointer := std_logic_vector(unsigned(pointer)+1 );
            
            --Wir überprüfen, ob der Pointer den Adressraum verlassen würde.
            --Wir berücksichtigen, dass mit jedem Durchlauf des Bubble Sort der Suchlauf
            --eine Adresse früher beendet werden kann, da pro Durchlauf ein hoher Wert
            --das Ende des Adressraums füllt-
            if (to_integer(unsigned(pointer) +1 ) > to_integer(unsigned(addr_end))  - amountCorrectLastDigits)
            then
               report "Ende des Sortierbereiches erreicht.";
               --Wir sparen uns einen Vergleich pro Suchdurchlauf.
               amountCorrectLastDigits := amountCorrectLastDigits +1;
               
               --Wenn der Sortiervorgang fertig ist, ist currentValue für die nächste
               --Adresse noch nicht gespeichert worden, wenn replaceCurrentValue wahr ist.
               --Das muss allerdings geschehen, bevor weitersortiert werden kann, #
               --da der Speicher sonst falsch ist.
               if (replaceCurrentValue = '1')
               then
                  report "Scheduled to save last value edit.";
                  replaceCurrentValue := '0';
                  saveLastValuePreviousSort := '1';
                  saveAddress := std_logic_vector(unsigned(pointer));
                  saveValue := std_logic_vector(unsigned(currentValue));
               end if;
               
               --Für erneute Sortierung vorbereiten:
               if (sortAgain = '1')
               then
                  report "Starte Sortiervorgang neu.";
                  sortAgain := '0';
                  currentValueValid:= '0';
                  nextValueValid := '0';
                  
                  report "--------------";
                  pointer := addr_start; --Wichtig!
               
               --FERTIG SORTIERT!
               else
                  report "Heureka! Sortierung ist fertig.";
                  nextValueValid := '1';
                  currentValueValid := '1';
                  
                  internalDone := '1';
               end if;
            end if;
         end if;
      end if;
   end if;
 end if;
end process;
end Behavioral;

