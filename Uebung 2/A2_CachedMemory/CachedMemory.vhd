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
  
  -- ==========
  -- Memory states and buffer
  -- ==========
  type memStates is (memIdle, memBusy);
  signal memState : memStates := memIdle;
  
  type memRequests is (noMemRequest, memRequestInit, memRequestReset, memRequestRead, memRequestWB);
  signal memRequest : memRequests := noMemRequest;
  
  type memContentStates is (memContentUndefined, memContentInit, memContentReset, memContentRead, memContentWB);
  signal memContentState : memContentStates := memContentUndefined;
  
  signal buffer_mem_clk      : std_logic := '0';
  signal buffer_mem_addr     : std_logic_vector(7 downto 0) := (others => '0');
  
  
  -- ==========
  -- Cache states and buffer
  -- ==========
  type cacheRequests is (noCacheRequest, cacheRequestInit, cacheRequestReset, cacheRequestWrite, cacheRequestCleanLine);
  signal cacheRequestFromMem  : cacheRequests := noCacheRequest;
  signal cacheRequestFromMain : cacheRequests := noCacheRequest;
  
  signal buffer_cacheFromMem_addr : std_logic_vector(7 downto 0) := (others => '0');
  signal buffer_cacheFromMem_data : std_logic_vector(7 downto 0) := (others => '0');
  signal buffer_cacheFromMem_d    : std_logic := '0';
  signal buffer_cacheFromMem_v    : std_logic := '0';
  
  signal buffer_cacheFromMain_addr : std_logic_vector(7 downto 0) := (others => '0');
  signal buffer_cacheFromMain_data : std_logic_vector(7 downto 0) := (others => '0');
  signal buffer_cacheFromMain_d    : std_logic := '0';
  signal buffer_cacheFromMain_v    : std_logic := '0';
  
  signal bufferAck : STD_LOGIC := '0';
  signal bufferOut : STD_LOGIC_VECTOR (7 downto 0) := (others => '0');
  
  
begin

  -- Clock process definitions
  mem_clk_process :process (clk)
    variable clkCnt : integer := 0;
    constant clkDiv : integer := 8; -- muss groeßer 1 und durch 2 teilbar sein.
  begin
    if rising_edge(clk) then
      if clkCnt = (clkDiv / 2 - 1) then
        clkCnt := 0;
        buffer_mem_clk <= not(buffer_mem_clk);
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
    output  => mem_output
  );
  
  
  
  memDriver: process (buffer_mem_clk)
    variable lastMemRequest : memRequests := noMemRequest;
    variable lastMemAddr : std_logic_vector(7 downto 0) := (others => '0');
  begin
    -- preset cacheDriver request. Located here to make sure the signal is reset between mem_clks,
    -- since the cacheDriver only reacts on changing of the signal and other states than noCacheRequest.
    cacheRequestFromMem <= noCacheRequest;
      
    if rising_edge(buffer_mem_clk) then
      if(memState = memIdle) then
        -- preset mem inputs. if an request is pending, it will set its needed inputs
        mem_init    <= '0';
        mem_dump    <= '0';
        mem_reset   <= '0';
        mem_re      <= '0';
        mem_we      <= '0';
        mem_addr    <= (others => '0');
        mem_data_in <= (others => '0');
        
        -- save last mem request for later processing in busy
        lastMemRequest  := memRequest;
        lastMemAddr     := addr;
        
        -- *** Init request ***
        if(memRequest = memRequestInit) then
          mem_init <= '1';
          -- tell this process and the main process that mem is busy and its contents are undefined.
          memState <= memBusy;
          memContentState <= memContentUndefined;
        
        -- *** Reset request ***
        elsif(memRequest = memRequestReset) then
          mem_reset <= '1';
          -- tell this process and the main process that mem is busy and its contents are undefined.
          memState <= memBusy;
          memContentState <= memContentUndefined;
        
        -- *** Read request ***
        elsif(memRequest = memRequestRead) then
          mem_re    <= '1';
          mem_addr  <= lastMemAddr;
          -- tell this process and the main process that mem is busy and its contents are undefined.
          memState <= memBusy;
          memContentState <= memContentUndefined;
        
        -- *** WB request ***
        elsif(memRequest = memRequestWB) then
          mem_we    <= '1';
          mem_addr(3 downto 0) <= lastMemAddr(3 downto 0);
          mem_addr(7 downto 4) <= cache(to_integer(unsigned(lastMemAddr(3 downto 0)))).tag;
          mem_data_in <= cache(to_integer(unsigned(lastMemAddr(3 downto 0)))).data;
          
          -- tell this process and the main process that mem is busy and its contents are undefined.
          memState <= memBusy;
          memContentState <= memContentUndefined;
        end if;
      else
        -- mem was busy for one mem_clk and should have processed its in-/outputs.
        -- stop mem from doing unwanted things.
        mem_init    <= '0';
        mem_dump    <= '0';
        mem_reset   <= '0';
        mem_re      <= '0';
        mem_we      <= '0';
        mem_addr    <= (others => '0');
        mem_data_in <= (others => '0');
        -- tell main process that mem is idle again.
        memState <= memIdle;
        
        -- check last request (maybe as variable?)
        -- *** Last request: Init ***
        if(lastMemRequest = memRequestInit) then
          -- tell the main process that the content of mem is now initialized.
          memContentState <= memContentInit;
          -- no need to set cacheRequestFromMem here, since cache init is controlled by main
        
        -- *** Last request: Reset ***
        elsif(lastMemRequest = memRequestReset) then
          -- tell the main process that the content of mem is now reset.
          memContentState <= memContentReset;
          -- no need to set cacheRequestFromMem here, since cache init is controlled by main
        
        -- *** Last request: Read ***
        elsif(lastMemRequest = memRequestRead) then
          -- check if mem_output is valid
          if( (mem_output /= "XXXXXXXX") AND (mem_output /= "UUUUUUUU") ) then
            -- data is valid => write it in the cache via CacheDriver
            -- buffer cache inputs for cacheDriver
            buffer_cacheFromMem_addr  <= lastMemAddr;
            buffer_cacheFromMem_data  <= mem_output;
            buffer_cacheFromMem_d     <= '0';
            buffer_cacheFromMem_v     <= '1';
            -- tell cacheDriver to write data to cache
            cacheRequestFromMem <= cacheRequestWrite;
          end if;
          -- tell the main process that the content of mem is now read (not yet used, just here for consistency).
          memContentState <= memContentRead;
        
        -- *** Last request: WB ***
        elsif(lastMemRequest = memRequestWB) then
          -- update bits in cache line via CacheDriver
          -- buffer cache inputs for cacheDriver
          buffer_cacheFromMem_addr  <= lastMemAddr;
          -- tell cacheDriver to update data in cache
          cacheRequestFromMem <= cacheRequestCleanLine;
          -- tell the main process that the content of mem is now wb (not yet used, just here for consistency).
          memContentState <= memContentWB;
        end if;
      end if;
    end if;
    
    -- forward the clock signal to mem after its inputs are set.
    mem_clk <= buffer_mem_clk;
  end process;
  
  
  
  cacheDriver: process(cacheRequestFromMem, cacheRequestFromMain)
    variable tmpCacheIndex : integer := 0;
    variable prevRequestFromMem  : cacheRequests := noCacheRequest;
    variable prevRequestFromMain : cacheRequests := noCacheRequest;
  begin
    if( cacheRequestFromMem /= prevRequestFromMem) then
      -- save this request as previous
      prevRequestFromMem := cacheRequestFromMem;
      -- buffer the cache index, since the conversion is so long
      tmpCacheIndex := to_integer(unsigned(buffer_cacheFromMem_addr(3 downto 0)));
      
      -- *** Request from mem: Write ***
      if( cacheRequestFromMem = cacheRequestWrite ) then
        -- write buffered values from mem to cache
        cache(tmpCacheIndex).tag  <= buffer_cacheFromMem_addr(7 downto 4);
        cache(tmpCacheIndex).data <= buffer_cacheFromMem_data ;
        cache(tmpCacheIndex).d    <= buffer_cacheFromMem_d;
        cache(tmpCacheIndex).v    <= buffer_cacheFromMem_v;
        
      -- *** Request from mem: CleanLine ***
      elsif( cacheRequestFromMem = cacheRequestCleanLine ) then
        -- update dirty bit in cache at buffered index from mem
        cache(tmpCacheIndex).d <= '0';
      end if;
    end if;
    
    
    if( cacheRequestFromMain /= prevRequestFromMain) then
      -- save this request as previous
      prevRequestFromMain := cacheRequestFromMain;
      -- buffer the cache index, since the conversion is so long
      tmpCacheIndex := to_integer(unsigned(buffer_cacheFromMain_addr(3 downto 0)));
      
      -- *** Request from main: Init ***
      if( cacheRequestFromMain = cacheRequestInit ) then
        -- invalidate cache for init
        for i in 0 to cache'length-1 loop
          cache(i).v <= '0';
        end loop;
        
      -- *** Request from main: Reset ***
      elsif( cacheRequestFromMain = cacheRequestReset ) then
        -- invalidate cache for reset
        for i in 0 to cache'length-1 loop
          cache(i).v <= '0';
        end loop;
        
      -- *** Request from main: Write ***
      elsif( cacheRequestFromMain = cacheRequestWrite ) then
        -- write buffered values from main to cache
        cache(tmpCacheIndex).tag  <= buffer_cacheFromMain_addr(7 downto 4);
        cache(tmpCacheIndex).data <= buffer_cacheFromMain_data ;
        cache(tmpCacheIndex).d    <= buffer_cacheFromMain_d;
        cache(tmpCacheIndex).v    <= buffer_cacheFromMain_v;
      end if;
    end if;
  end process;
  
  
  
  
  
  execute: process (clk)
    variable tmpTag   : STD_LOGIC_VECTOR (3 downto 0) := (others => '0');
    variable tmpIndex : STD_LOGIC_VECTOR (3 downto 0) := (others => '0');
    variable tmpIntTag   : integer := 0;
    variable tmpIntIndex : integer := 0;
    
  begin
    -- preset cacheDriver request. Located here to make sure the signal is reset between clks,
    -- since the cacheDriver only reacts on changing of the signal and other states than noCacheRequest.
    cacheRequestFromMain <= noCacheRequest;
  
    if rising_edge(clk) then
      -- aufteilen und zwischenspeichern der adresse zur leicheren handhabung
      tmpTag    := addr(7 downto 4);
      tmpIndex  := addr(3 downto 0);
      tmpIntTag   := to_integer(unsigned(tmpTag));
      tmpIntIndex := to_integer(unsigned(tmpIndex));
      
      -- buffer vorbelegen, so nicht jeder if-fall auf das zuruecksetzen aufpassen muss
      bufferAck <= '0';
      bufferOut <= "XXXXXXXX";
      
      memRequest <= noMemRequest;
    
      -- *** Init ***
      -- may take up to 3 mem_clks if there are pending mem operations
      if(init = '1' AND dump = '0' AND reset = '0' AND re = '0' AND we = '0') then
        -- invalidate output no matter whether mem is busy or not, since the cache will be
        -- invalidated immediately.
        bufferAck <= '0';
        bufferOut <= "XXXXXXXX";
        -- cache is immediately invalidated, since the content of mem will be overwritten.
        -- the user has to make sure the init signal is set long enough.
        cacheRequestFromMain <= cacheRequestInit;
        
        -- wait until mem has finished its things
        if((memState = memIdle) AND (memContentState /= memContentInit) ) then
          -- send request to memDriver to init mem
          memRequest <= memRequestInit;
        end if;
      -- *** Init End ***
      
      
      
      -- *** Dump ***
      elsif(init = '0' AND dump = '1' AND reset = '0' AND re = '0' AND we = '0') then
      -- *** Dump End ***
      
      
      
      -- *** Reset ***
      elsif(init = '0' AND dump = '0' AND reset = '1' AND re = '0' AND we = '0') then
        -- invalidate output no matter whether mem is busy or not, since the cache will be
        -- invalidated immediately.
        bufferAck <= '0';
        bufferOut <= "XXXXXXXX";
        -- cache is immediately invalidated, since the content of mem will be overwritten.
        -- the user has to make sure the reset signal is set long enough.
        cacheRequestFromMain <= cacheRequestReset;
        
        -- wait until mem has finished its things
        if((memState = memIdle) AND (memContentState /= memContentReset) ) then
          -- send request to memDriver to reset mem
          memRequest <= memRequestReset;
        end if;
      -- *** Reset End ***
        
      
      
      -- *** Read ***
      elsif(init = '0' AND dump = '0' AND reset = '0' AND re = '1' AND we = '0') then
        -- invalidate output no matter whether data is in cache or not. Will be set
        -- if data is in cache.
        bufferAck <= '0';
        bufferOut <= "XXXXXXXX";
        
        -- check whether data is in cache
        if(cache(tmpIntIndex).v = '1') then
          if(tmpTag /= cache(tmpIntIndex).tag) then
            if(cache(tmpIntIndex).d = '1') then
              -- valid, other data, dirty => wb the dirty cacheline
              -- wait until mem has finished its things
              if(memState = memIdle) then
                -- no need to save the addr to ensure that only the value at rising clk is
                -- used, since the mem_clk has its rising edge at the same time (disregarding gate latency).
                -- send request to memDriver to wb mem
                memRequest <= memRequestWB;
              end if;
            else
              -- valid, other data, clean => read data from cache
              -- wait until mem has finished its things
              if(memState = memIdle) then
                -- no need to save the addr to ensure that only the value at rising clk is
                -- used, since the mem_clk has its rising edge at the same time (disregarding gate latency).
                -- send request to memDriver to read mem
                memRequest <= memRequestRead;
              end if;
            end if;
          else
            -- valid, same data => output data in cache. no need to write back yet if dirty
            bufferAck <= '1';
            bufferOut <= cache(tmpIntIndex).data;
          end if;
        else
          -- invalid => read data from cache (if invalid, then no need to check the other values)
          -- wait until mem has finished its things
          if(memState = memIdle) then
            -- no need to save the addr to ensure that only the value at rising clk is
            -- used, since the mem_clk has its rising edge at the same time (disregarding gate latency).
            -- send request to memDriver to read mem
            memRequest <= memRequestRead;
          end if;
        end if;
      
      
      
      -- *** Write ***
      elsif(init = '0' AND dump = '0' AND reset = '0' AND re = '0' AND we = '1') then
        -- check addressed cacheline
        if(cache(tmpIntIndex).v = '1') then
          if(tmpTag /= cache(tmpIntIndex).tag) then
            if(cache(tmpIntIndex).d = '1') then
              -- valid, other data, dirty => wb the dirty cacheline
              -- wait until mem has finished its things
              if(memState = memIdle) then
                -- no need to save the addr to ensure that only the value at rising clk is
                -- used, since the mem_clk has its rising edge at the same time (disregarding gate latency).
                -- send request to memDriver to wb mem
                memRequest <= memRequestWB;
              end if;
            else
              -- valid, other data, clean => write data to cache
              -- check if input is valid
              if( (data_in /= "XXXXXXXX") AND (data_in /= "UUUUUUUU") ) then
                -- data is valid => write it in the cache via CacheDriver
                -- buffer cache inputs for cacheDriver
                buffer_cacheFromMain_addr  <= addr;
                buffer_cacheFromMain_data  <= data_in;
                buffer_cacheFromMain_d     <= '1';
                buffer_cacheFromMain_v     <= '1';
                -- tell cacheDriver to write data to cache
                cacheRequestFromMain <= cacheRequestWrite;
                -- if needed, an write-ack can be send here
                -- NOTE OPTIONAL ACK (email)
              end if;
            end if;
          else
            -- valid, same data => overwrite write data in cache (no need to check dirty, 
            -- since wb only needed when other data)
            -- check if input is valid
            if( (data_in /= "XXXXXXXX") AND (data_in /= "UUUUUUUU") ) then
              -- data is valid => write it in the cache via CacheDriver
              -- buffer cache inputs for cacheDriver
              buffer_cacheFromMain_addr  <= addr;
              buffer_cacheFromMain_data  <= data_in;
              buffer_cacheFromMain_d     <= '1';
              buffer_cacheFromMain_v     <= '1';
              -- tell cacheDriver to write data to cache
              cacheRequestFromMain <= cacheRequestWrite;
              -- if needed, an write-ack can be send here
              -- NOTE OPTIONAL ACK (email)
            end if;
          end if;
        else
          -- invalid, same data => overwrite write data in cache (if invalid, 
          -- then no need to check the other values)
          -- check if input is valid
          if( (data_in /= "XXXXXXXX") AND (data_in /= "UUUUUUUU") ) then
            -- data is valid => write it in the cache via CacheDriver
            -- buffer cache inputs for cacheDriver
            buffer_cacheFromMain_addr  <= addr;
            buffer_cacheFromMain_data  <= data_in;
            buffer_cacheFromMain_d     <= '1';
            buffer_cacheFromMain_v     <= '1';
            -- tell cacheDriver to write data to cache
            cacheRequestFromMain <= cacheRequestWrite;
            -- if needed, an write-ack can be send here
            -- NOTE OPTIONAL ACK (email)
          end if;
        end if;
      
      
      
      -- *** Nothing ***
      elsif(init = '0' AND dump = '0' AND reset = '0' AND re = '0' AND we = '0') then
      
      
      
      -- *** Fehlerhafte Eingabe ***
      else
      
      
      
      end if;
    end if;
  end process;
  
  -- Ausgabenetz
  ack     <= bufferAck;
  output  <= bufferOut;

end Behavioral;

