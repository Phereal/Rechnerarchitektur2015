----------------------------------------------------------------------------------
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
   variable initDelay : Integer := 200;
   --Gibt an, ob Sortierungg läuft.
   variable isRunning   : STD_LOGIC                   := '0';
   variable pointer     : std_logic_vector(7 downto 0):= addr_start;
   variable currentValue: std_logic_vector(7 downto 0);
   variable nextValue   : std_logic_vector(7 downto 0);
   variable firstValue  : std_logic_vector(7 downto 0);
   variable currentValueValid: STD_LOGIC              := '0';
   variable nextValueValid: STD_LOGIC                 := '0';
   variable firstValueValid: STD_LOGIC                := '0';
   variable getOutput: STD_LOGIC                      := '0';
   variable waitForOutput: STD_LOGIC                  := '0';   
   variable amountCorrectLastDigits: Integer          := 0; --Anzahl korrekte Endziffern
   variable sortAgain: STD_LOGIC                      := '0'; --Durchlauf ohne Sortierung?
   variable replaceCurrentValue: STD_LOGIC            := '0';
   
   
 
 begin
  if rising_edge(clk)
  then
  
   if(start = '1' AND isRunning = '0')
   then
      isRunning := '1';
      initDelay := 200;
      done <= '0';
   end if;
  
   if(isRunning = '1')
   then
      --Load init values.
      if (initDelay = 200)
      then
         mem_init <= '1';
      end if;
      
      --Count down the delay.
      if (initDelay > 0)
      then
         initDelay := initDelay-1;
      end if;      
   
      if(isRunning ='1' AND initDelay = 0)
      then
         if(firstValueValid = '0')
         then 
            if (getOutput = '0')
            then
               mem_addr <= addr_start;
               mem_we   <= '0';
               mem_re   <= '1';
               getOutput := '1';
            else if (mem_ack='1')
            then
               firstValue := mem_output;
               firstValueValid := '1';
               getOutput := '0';
            end if;
         end if;
         
         --Dies ist eine if-Abfrage statt eines else, da die
         --Anweisung in demselben Schritt erfolgen darf.
         if(firstValueValid = '1' AND currentValueValid = '0')
         then
            currentValue := firstValue;
            currentValueValid := '1';
         end if;

         if(currentValueValid = '1' AND nextValueValid = '0')
         then 
            if (getOutput = '0')
            then
               mem_addr <= std_logic_vector(to_unsigned(to_integer(unsigned(pointer)) + 1, 8)); --so richtig?
               mem_we <= '0';
               mem_re   <= '1';
               getOutput := '1';
            
            else if (getOutput = '1' AND mem_ack = '1')
            then
               nextValue := mem_output;
               nextValueValid := '1';
               getOutput := '0';
               end if;
            end if;
         end if;
         
         --Im folgenden Codeblock wird festgestellt, was sortiert wird.
         --Allerdings wird bei jedem Durchlauf nur 1 Schreibvorgang durchgeführt!
         --Dies spart Zeit, da der Speicher 1 Clock cycle zum speichern benötigt.
         --
         --1. Fall
         --Wenn der aktuelle Wert größer ist als der nächste Wert, wird der aktuelle Wert
         --durch den nächsten ersetzt. 
         --Der zweite Wert wird allerdings nocht nicht verändert und liegt daher doppelt vor.
         --Er wird zwischengespeichert und im nächsten Schritt als aktueller Wert behandelt
         --
         --2. Fall
         --Wenn die beiden Werte in der korrekten Reihenfolge sind, wird der aktuelle Wert
         --mit dem zwischengespeicherten aktuellen Wert überschrieben,
         --
         --Wenn der erste Fall bei der Sortierung bisher nicht auftrat, geschieht hier nichts.
         if(firstValueValid = '1' AND currentValueValid = '1' AND nextValueValid = '1')
         then
            mem_re <= '0'; --Auf keinen Fall lesen!
            nextValueValid := '0';
            
            if (currentValue>nextValue)
            then
            --Werte tauschen
               mem_addr <= pointer;
               mem_data_in <= nextValue;
               mem_we <= '1';
               --currentValue bleibt gleich, da es nach vorne verschoben wurde!
               replaceCurrentValue := '1';
               sortAgain := '1';
            else
            --Werte bereits in richtiger Reihefolge
               if (replaceCurrentValue = '1')
               then
                  replaceCurrentValue := '0';
                  mem_addr <= pointer;
                  mem_data_in <= currentValue;
                  mem_we <= '1';
                  mem_re <= '0';
               end if;
            
            currentValue := nextValue;
            end if;
            
            pointer := std_logic_vector(to_unsigned(to_integer(unsigned(pointer)) + 1, 8));
            
            --Suchdurchlauf beenden?
            if (to_integer(unsigned(pointer)) > to_integer(unsigned(addr_end)) - amountCorrectLastDigits)
            then
               --Wir sparen uns einen Vergleich pro Suchdurchlauf.
               amountCorrectLastDigits := amountCorrectLastDigits +1;
               
               --Für erneute Sortierung vorbereiten:
               if (sortAgain = '1')
               then
                  firstValueValid:= '0';
                  currentValueValid:= '0';
                  nextValueValid := '0';
                  --Zwischengespeicherten Wert schreiben
                  if (replaceCurrentValue = '1')
                  then
                     replaceCurrentValue := '0';
                     mem_addr <= std_logic_vector(to_unsigned(to_integer(unsigned(pointer)) - 1, 8));
                     mem_data_in <= currentValue;
                     mem_we <= '1';
                     mem_re <= '0';
                  end if;
                  
                  pointer := addr_start; --Wichtig!
               
               --FERTIG SORTIERT!
               else
                  if (replaceCurrentValue = '1')
                  then
                     replaceCurrentValue := '0';
                     mem_addr <= std_logic_vector(to_unsigned(to_integer(unsigned(pointer)) - 1, 8));
                     mem_data_in <= currentValue;
                     mem_we <= '1';
                     mem_re <= '0';
                  end if;
                  mem_dump <= '1';
                  done <= '1';
               end if;
                  
               
            end if;
            end if;
         end if;
      end if;
   end if;
 end if;
 end process;
end Behavioral;

