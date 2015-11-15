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
           addr_end : in  STD_LOGIC (7 downto 0);
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


   --Pointer, der auf das einzusortierende Element zeigt.
   variable pointerSort : STD_LOGIC_VECTOR (7 downto 0); 

   --Ist '1', wenn der Speicher im aktuellen Durchlauf sortert werden musste.
   variable sortAgain   : STD_LOGIC_VECTOR;

   --Indiziert, ob auf die Antwort des Caches gewartet werden soll.
   variable waitOneCycle   : STD_LOGIC_VECTOR (7 downto 0); 
   
      
   --Zwischenspeicher, da der Cache-Output nicht immer kosistent bleibten kann:
   
   --Daten, auf die der Pointer zeigt.
   variable pointerSortData : STD_LOGIC_VECTOR (7 downto 0); 
   
   --Daten, die den vom Pointer gezeigten folgen.
   variable pointerSortDataNext : STD_LOGIC_VECTOR (7 downto 0); 

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
      done <= '0';               --Sortierung läuft.
      pointerSort <= addr_start; --Fange an Anfangsadresse an.
      sortAgain <= 0;            --Noch kein Anlass für 2. Durchlauf.
      waitOneCycle <= 1;         --Read zu Beginn der Suche, daher warten.
       --+1 ist das hier: std_logic_vector(unsigned(addr_start)+1);
   end if;
   
   --Wenn done '0' ist, muss die Sortierung laufen.
   if (done = '0')
   then
      --Wir stellen sicher, dass wir genau 1 Clock Cycle auf den Output
      --des Cache-Speichers warten.
      if (waitOneCycle = '1')
      then
         waitOneCycle = '0';
      --Muss aktueller Wert mit nächstem getauscht werden?
      else if (unsigned(pointerSort) > unsigned(pointerSort)+1)
      then
         sortAgain <= '1';
         
      --Ende des Sortierbereiches erreicht?
      else if(unsigned(pointerSort)+1 < unsigned(addr_end))
      then
         
         
      
  end if;
 end process;
end Behavioral;

