----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    18:11:20 11/13/2015 
-- Design Name: 
-- Module Name:    CachedMemory - Behavioral 
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
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity CachedMemory is
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
    ack     : out  STD_LOGIC);
end CachedMemory;


-- Direct Mapped Cache
-- Nettogroesse:            
--  16 Byte
-- Schreibstrategien:
--  - Write Back
--  - Fetch on Write

architecture Behavioral of CachedMemory is
  component Memory
    Port (
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
  end component;
  
  signal mem_clk : std_logic := '0';
  



  type cacheLine is record
    v : STD_LOGIC;
    d : STD_LOGIC;
    tag : STD_LOGIC_VECTOR (3 downto 0);
    date : STD_LOGIC_VECTOR (7 downto 0);
  end record;
  type cacheStruct is array (0 to 15) of cacheLine;
  
  signal cache : cacheStruct;
  




begin

  -- Clock process definitions
  mem_clk_process :process (clk)
    variable clkCnt : integer := 0;
    constant clkDiv : integer := 8; -- muss groeßer 1 und durch 2 teilbar sein.
  begin
  
  if rising_edge(clk) then
    if clkCnt = (clkDiv / 2 - 1) then
      clkCnt := 0;
      mem_clk <= not(mem_clk);
    else
      clkCnt := clkCnt + 1;
    end if;
    end if;
  end process;

  mem: Memory PORT MAP (
    clk     => mem_clk,
    init    => init,
    dump    => dump,
    reset   => reset, 
    re      => re,
    we      => we,
    addr    => addr,
    data_in => data_in, 
    output  => output);

  execute: process (clk)
    variable tmpTag : STD_LOGIC_VECTOR (3 downto 0);
    variable tmpIndex : integer;
  begin
    if rising_edge(clk) then
      -- *** Init ***
      if(init = '1' AND dump = '0' AND reset = '0' AND re = '0' AND we = '0') then
        -- TODO:
        
      -- *** Dump ***
      elsif(init = '0' AND dump = '1' AND reset = '0' AND re = '0' AND we = '0') then
        -- TODO:
        
      -- *** Reset ***
      elsif(init = '0' AND dump = '0' AND reset = '1' AND re = '0' AND we = '0') then
        for i in 0 to cache'length-1 loop
          cache(i).v <= '0';
        end loop;
        -- TODO: Reset memory
        
      -- *** Read ***
      elsif(init = '0' AND dump = '0' AND reset = '0' AND re = '1' AND we = '0') then
        tmpTag := addr(7 downto 4);
        tmpIndex := to_integer(unsigned(addr(3 downto 0)));

        -- TODO: noch keine Absicherung falls lesen aus Speicher fehlschlaegt
        if(cache(tmpIndex).v = '1') then
          if(tmpTag /= cache(tmpIndex).tag) then
            if(cache(tmpIndex).d = '1') then
              -- Miss: Andere Daten im Cache und dirty.
              --  Schreibe zurueck in Speicher. Setze d = 0
              --  Lesen aus Speicher wird spaeter durchgefuehrt
              -- TODO: Write Back
            end if;
            -- Miss: Andere Daten im Cache und sauber.
            --  Hole Daten aus dem Speicher. Setze data = read
            -- TODO: Read
          end if;
          -- Hit: Daten im Cache. Dirty egal da write back Strategie.
          --  Lesen aus Speicher wird nicht benoetigt.
        else
          -- Miss: Daten nicht im Cache. Dirty und Tag egal da invalid.
          --  Hole Daten aus dem Speicher. Setze v = 1, d = 0, data = read
        end if;
        
        -- Daten sind jetzt im Cache und koennen zurueck gegeben werden
        -- TODO
        
        
        
        
      -- *** Write ***
      elsif(init = '0' AND dump = '0' AND reset = '0' AND re = '0' AND we = '1') then
        -- TODO
      
      -- *** Fehlerhafte Eingabe ***
      else
        -- TODO
      end if;
    end if;
  end process;

end Behavioral;

