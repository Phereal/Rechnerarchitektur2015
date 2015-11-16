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
  
  signal mem_clk      : std_logic := '0';
  signal mem_init     : std_logic := '0';
  signal mem_dump     : std_logic := '0';
  signal mem_reset    : std_logic := '0';
  signal mem_re       : std_logic := '0';
  signal mem_we       : std_logic := '0';
  signal mem_addr     : std_logic_vector(7 downto 0) := (others => '0');
  signal mem_data_in  : std_logic_vector(7 downto 0) := (others => '0');
  signal mem_output   : std_logic_vector(7 downto 0);
  

  type cacheLine is record
    v : STD_LOGIC;
    d : STD_LOGIC;
    tag : STD_LOGIC_VECTOR (3 downto 0);
    data : STD_LOGIC_VECTOR (7 downto 0);
  end record;
  type cacheStruct is array (0 to 15) of cacheLine;
  signal cache : cacheStruct;
  
  type stateType is (none, wb, wb_ready, reading, re_ready);
  signal state : stateType := none;
  
  signal dataReady : STD_LOGIC := '0';
  
  
  




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
    init    => mem_init,
    dump    => mem_dump,
    reset   => mem_reset,
    re      => mem_re,
    we      => mem_we,
    addr    => mem_addr,
    data_in => mem_data_in,
    output  => mem_output);

  execute: process (clk, mem_clk)
    variable tmpTag : STD_LOGIC_VECTOR (3 downto 0);
    variable tmpIndex : integer;
    variable waitForMem : std_logic := '0';
    variable waitedOneMemClk : std_logic := '0';
    
  begin
    if rising_edge(mem_clk) then
      if(state = wb) then
        if(waitedOneMemClk = '1') then
          waitedOneMemClk := '0';
          state <= wb_ready;
        else
          waitedOneMemClk := '1';
        end if;
      elsif(state = reading) then
        if(waitedOneMemClk = '1') then
          waitedOneMemClk := '0';
          state <= re_ready;
        else
          waitedOneMemClk := '1';
        end if;
      else
        -- ignore other states
      end if;
    end if;
  
    if rising_edge(clk) then
      waitForMem := '0';
    
      -- *** Init ***
      if(init = '1' AND dump = '0' AND reset = '0' AND re = '0' AND we = '0') then
        dataReady <= '0';
        -- TODO:
        
      -- *** Dump ***
      elsif(init = '0' AND dump = '1' AND reset = '0' AND re = '0' AND we = '0') then
        dataReady <= '0';
        -- TODO:
        
      -- *** Reset ***
      elsif(init = '0' AND dump = '0' AND reset = '1' AND re = '0' AND we = '0') then
        dataReady <= '0';
        for i in 0 to cache'length-1 loop
          cache(i).v <= '0';
        end loop;
        -- TODO: Reset memory
        
      -- *** Read ***
      elsif(init = '0' AND dump = '0' AND reset = '0' AND re = '1' AND we = '0') then
        dataReady <= '0';
        if(state = none or state = wb_ready or state = re_ready) then
          tmpTag := addr(7 downto 4);
          tmpIndex := to_integer(unsigned(addr(3 downto 0)));

          -- TODO: noch keine Absicherung falls lesen aus Speicher fehlschlaegt, pruefung nach unten in valid = 1?
          if(cache(tmpIndex).v = '1') then
            if(tmpTag /= cache(tmpIndex).tag) then
              if(cache(tmpIndex).d = '1') then
                -- Miss: Andere Daten im Cache und dirty.
                --  Schreibe zurueck in Speicher. Setze d = 0
                --  Lesen aus Speicher wird spaeter durchgefuehrt
                if(state = none) then
                  -- TODO: Write Back
                  mem_init    <= '0';
                  mem_dump    <= '0';
                  mem_reset   <= '0';
                  mem_re      <= '0';
                  mem_we      <= '1';
                  mem_addr    <= addr;
                  mem_data_in <= cache(tmpIndex).data;
                  
                  cache(tmpIndex).d <= '0';
                  
                  state <= wb;
                  waitForMem := '1'; -- wird zusaetzlich zu state gesetzt, da sich state erst nach dem prozess aendert
                end if;
              end if;
              -- Miss: Andere Daten im Cache und sauber.
              --  Hole Daten aus dem Speicher. Setze data = read
              if((state = none or state = wb_ready) and (waitForMem = '0')) then
                -- TODO: Read
                mem_init    <= '0';
                mem_dump    <= '0';
                mem_reset   <= '0';
                mem_re      <= '1';
                mem_we      <= '0';
                mem_addr    <= addr;
                mem_data_in <= (others => '0');
                
                cache(tmpIndex).data <= mem_output;
                cache(tmpIndex).tag <= tmpTag;
                cache(tmpIndex).v <= '0'; -- maybe set in rifing_edge(mem_clk)
                
                state <= reading;
                waitForMem := '1'; -- wird zusaetzlich zu state gesetzt, da sich state erst nach dem prozess aendert
              end if;
            end if;
            -- Hit: Daten im Cache. Dirty egal da write back Strategie.
            --  Lesen aus Speicher wird nicht benoetigt.
          else
            -- Miss: Daten nicht im Cache. Dirty und Tag egal da invalid.
            --  Hole Daten aus dem Speicher. Setze v = 1, d = 0, data = read
            if(state = none) then
              -- TODO: Read
              mem_init    <= '0';
              mem_dump    <= '0';
              mem_reset   <= '0';
              mem_re      <= '1';
              mem_we      <= '0';
              mem_addr    <= addr;
              mem_data_in <= (others => '0');
                
              cache(tmpIndex).data <= mem_output;
              cache(tmpIndex).tag <= tmpTag;
              cache(tmpIndex).v <= '0'; -- maybe set in rifing_edge(mem_clk)
              
              state <= reading;
              waitForMem := '1'; -- wird zusaetzlich zu state gesetzt, da sich state erst nach dem prozess aendert
            end if;
          end if;
        
          -- Daten sind jetzt im Cache und koennen zurueck gegeben werden
          if((state = none or state = re_ready) and (waitForMem = '0')) then
            mem_init    <= '0';
            mem_dump    <= '0';
            mem_reset   <= '0';
            mem_re      <= '0';
            mem_we      <= '0';
            mem_addr    <= (others => '0');
            mem_data_in <= (others => '0');
            --mem_output <= (others => '0');
              
            cache(tmpIndex).v <= '1';
            cache(tmpIndex).d <= '0';
            
            dataReady <= '1';
          end if;
        end if;
        
        
        
      -- *** Write ***
      elsif(init = '0' AND dump = '0' AND reset = '0' AND re = '0' AND we = '1') then
        dataReady <= '0';
      -- TODO
      
      -- *** Nothing ***
      elsif(init = '0' AND dump = '0' AND reset = '0' AND re = '0' AND we = '0') then
      
      -- *** Fehlerhafte Eingabe ***
      else
        dataReady <= '0';
        -- TODO
      end if;
    end if;
  end process;
  
  -- Ausgabe
  ack <= dataReady;
  output <= cache(to_integer(unsigned(addr(3 downto 0)))).data;

end Behavioral;

