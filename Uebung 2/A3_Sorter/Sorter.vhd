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

begin
  mem: CachedMemory PORT MAP (
    clk     => mem_clk,
    init    => mem_init,
    dump    => mem_dump,
    reset   => mem_reset,
    re      => mem_re,
    we      => mem_we,
    addr    => mem_addr,
    data_in => mem_data_in,
    output  => mem_output);

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
 
 begin
  if rising_edge(clk)
  then
  
   if(start = '1' AND isRunning = '0')
   then
      isRunning := '1';
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
   end if;
   
   --Sortierung!
   if(waitForOutput = '1')
   then
      --1 cycle Verzögerung
      waitForOutput := '0';
      
   else 
      if(isRunning ='1' AND initDelay = 0)
      then
         if(firstValueValid = '0')
         then 
            if (getOutput = '0')
            then
               mem_addr <= addr_start;
               mem_re   <= '1';
               waitForOutput := '1';
               getOutput := '1';
            else
               firstValue := mem_output;
               firstValueValid := '1';
            end if;
         end if;
         
         --Dies ist eine if-Abfrage statt eines else, da die
         --Anweisung in demselben Schritt erfolgen darf.
         if(firstValueValid = '1' AND currentValueValid = '0')
         then
            currentValue := firstValue;
            currentValueValid := 1;
         end if;

         if(currentValueValid = '1' AND nextValueValid = '0')
         then 
            if (getOutput = '0')
            then
               mem_addr <= std_logic_vector(unsigned(pointer)+1); --noch night ganz richtig
               mem_re   <= '1';
               waitForOutput := '1';
               getOutput := '1';
            else
               nextValue := mem_output;
               nextValueValid := '1';
            end if;
         end if;
            
         
      end if;
         
   end if;
 end if;
 end process;
end Behavioral;

