---------------------------------------------------------------------------------------------------
-- Rechnerarchitektur und Eingebettete Systeme
-- Uebungszettel 2 - Aufgabe 2: Direct Mapped Cache
--
-- Der Grossteil der Quelltextkommentierung wurde hier durchgefuehrt, um die Uebersicht im Code zu
-- erhalten. Kommentare in den Codeabschnitten dienen groesstenteils zur einfacheren Verfolgung
-- des Programmablaufs.
--
--
-- Der Cache hat, entsprechend der Aufgabenstellung, eine Nettogroesse von 16 Byte in 16
-- Cachelines.
--
-- Als Schreibstrategien wurde "Write Back" (WB) verwendet um Schreibzugriffe auf den unter-
-- liegenden Speicher zu verringern. Konkret heisst das, dass beim schreibenden Zugriff auf den
-- Speicher das Datum nur in den Cache geschrieben wird und die Cacheline als "dirty" (d) markiert
-- wird. Der eigentliche Schreibzugriff auf den Speicher wird erst ausgeloest, wenn die Cacheline 
-- durch einen anderen Wert ersetzt werden soll (Lesen oder Schreiben einer anderen Adresse mit 
-- gleichem "Tag").
-- 
-- Um 16 Byte im Cache Speichern zu koennen werden 16 Cachelines angelegt, die ueber die 4 LSB der
-- Speicheradresse (2^4 = 16) indexiert werden. Daraus folgt das die restlichen 4 Bit (MSB: 7 - 4)
-- als "Tag" in der Cacheline gespeichert werden muessen. Fuer die Daten werden 8 Bit in der Cache-
-- line benoetigt. Zur Verwaltung der Cachelines werden noch ein Valid-Bit und ein Dirty-Bit je
-- Cacheline gespeichert. Der eigentliche Cache hat somit eine Brutogroesse von 16* 14Bit = 28Byte.
--
-- Das Dirty-Bit bestimmt ob die Cacheline ggf. bei Aenderung in den Speicher zurueckgeschrieben
-- werden muss. Das Valid-Bit bestimmt ob die Cacheline Daten aus dem Speicher enthaelt.
--
-- Die Realisierung des Caches wurde ueber vier Prozesse und ein Ausgabenetz ausserhalb der
-- umgesetzt.
-- Der Prozess "mem_clk_process" generiert den Clock fuer den Speicher, welche mit 1/8 des Cache-
-- clocks laeuft (Aufgabenstellung).
-- Der Prozess "execute" verarbeitet die Eingaenge des Caches und wird im weiteren Verlauf auch als
-- "main" bezeichnet. Er prueft welche Eingaenge gesetzt sind und entscheidet dann je nach Zustand
-- des Caches/ der Cacheline und der Eingaben was getan werden soll. Zugriffe auf den langsamen
-- Speicher werden an den Prozess "memDriver" weitergeleitet, welcher dann entsprechend die Daten
-- in den Speicher schreibt, oder aus dem Speicher liest und an den Prozess "cacheDriver" weiter-
-- gibt. Der Prozess "cacheDriver" empfaengt Daten aus "main" und aus "memDriver" und schreibt die
-- Daten entsprechend in den Cache.
--
-- Die Architektur mit drei Prozessen wurde so gewaehlt um vor allem den Zugriff auf den unter-
-- liegenden Speicher zu schuetzen, da dieser um einiges laenger benoetigt bis er eine Anfrage ver-
-- arbeitet hat. Durch eine Selbstsperre ("memState") und Synchronisationssignale ("memRequest", 
-- "memContentState") wird sichergestellt, dass der Speicher genug Zeit hat um die geforderte
-- Aktion auszufuehren.
--
-- Um den Cache konsistent zu halten und sowohl Cache-Aenderung aus "main" als auch aus "memDriver"
-- verarbeiten zu koennen - Ein schreibender Zugriff nur aus "main" wuerde nach Speicherzugriffen 
-- einen zusaetzlichen Takt benoetigen und wuerde hoeheren Aufwand bei der Synchronisation der
-- beiden Prozesse erfordern. - werden alle schreibenden Cachezugriffe durch "cacheDriver"
-- verwaltet. Zur Synchronisation dienen die Signale "cacheRequestFromMem" und 
-- "cacheRequestFromMain", ein Lock wird nicht benoetigt, da der Prozess auf Aenderungen dieser
-- Signale reagiert und beim internen Zugriff auf den Cache keine Verzoegerungen beruecksichtigt 
-- werden muessen. Die zu schreibenden Daten und Adressen werden in Buffern zwischengespeichert
-- ("main" und "memDriver" haben jeweils ihr eigenen Set von Buffern zum Zugriff auf "cacheDriver")
---------------------------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.all;
USE IEEE.NUMERIC_STD.ALL;

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
  -- memState is used to lock the main process and to ensure in memDriver that access to mem is
  -- valid (e.g. one mem_clk has finished). It is set via memDriver.
  type memStates is (memIdle, memBusy);
  signal memState : memStates := memIdle;
  
  -- memRequest is used by main to tell memDriver what it has to do. It is set by main.
  type memRequests is ( noMemRequest, memRequestInit, memRequestReset, memRequestRead, 
                        memRequestWB, memRequestWBForDump, memRequestDump);
  signal memRequest : memRequests := noMemRequest;
  
  -- memContenState is used by memDriver to tell main whether it has finished a certain request or
  -- not. It is set by memDriver.
  type memContentStates is (memContentUndefined, memContentInit, memContentReset,
                            memContentRead, memContentWB, memContentDumped);
  signal memContentState : memContentStates := memContentUndefined;
  
  -- the buffer_mem_clk is the clock generated by mem_clk_process for mem. It is not directly
  -- connected to mem, to enable memDriver to set mem's inputs first.
  signal buffer_mem_clk   : std_logic := '0';
  -- buffer_mem_addr is used by main to buffer the line that has to be written back during dump.
  signal buffer_mem_addr  : std_logic_vector(7 downto 0) := (others => '0');
  
  
  -- ==========
  -- Cache states and buffer
  -- ==========
  -- cacheRequestFromMem is used by memDriver to tell cacheDriver what it has to do. It is set by
  -- memDriver.
  -- cacheRequestFromMain is used by main to tell cacheDriver what it has to do. It is set by main.
  -- Two signals are needed, since only one process can write to one single signal
  type cacheRequests is ( noCacheRequest, cacheRequestInit, cacheRequestReset,
                          cacheRequestWrite, cacheRequestCleanLine);
  signal cacheRequestFromMem  : cacheRequests := noCacheRequest;
  signal cacheRequestFromMain : cacheRequests := noCacheRequest;
  
  -- this set of buffers used by memDriver to tell cacheDriver what and where he has to update the
  -- cache.
  signal buffer_cacheFromMem_addr : std_logic_vector(7 downto 0) := (others => '0');
  signal buffer_cacheFromMem_data : std_logic_vector(7 downto 0) := (others => '0');
  signal buffer_cacheFromMem_d    : std_logic := '0';
  signal buffer_cacheFromMem_v    : std_logic := '0';
  
  -- this set of buffers used by main to tell cacheDriver what and where he has to update the
  -- cache.
  signal buffer_cacheFromMain_addr : std_logic_vector(7 downto 0) := (others => '0');
  signal buffer_cacheFromMain_data : std_logic_vector(7 downto 0) := (others => '0');
  signal buffer_cacheFromMain_d    : std_logic := '0';
  signal buffer_cacheFromMain_v    : std_logic := '0';
  
  -- these signals are directly connected to the cache's outputs ack and output. They are set in
  -- main.
  signal bufferAck : STD_LOGIC := '0';
  signal bufferOut : STD_LOGIC_VECTOR (7 downto 0) := (others => '0');
  
  
begin

  -- Clock process definitions
  -- generates a clock signal in buffer_mem_clk with 1/8 the frequency of clk
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
  
  
  -- ====================
  -- Controlls access to the memory mem.
  -- ====================
  memDriver: process (buffer_mem_clk)
    -- holds the last value of memRequest and is set in idle-mode. It is used in busy-mode to
    -- determine what was done.
    variable lastMemRequest : memRequests := noMemRequest;
    -- holds the last value of the cache address. It is also set in idle-mode and used in busy-mode
    -- to tell the cacheDrive which cacheline he has to update.
    variable lastMemAddr : std_logic_vector(7 downto 0) := (others => '0');
  begin
    -- preset cacheDriver request. Located here to make sure the signal is reset between mem_clks,
    -- since the cacheDriver only reacts on changing of the signal and other states than
    -- noCacheRequest.
    cacheRequestFromMem <= noCacheRequest;
    
    -- reacts to the slower clock to ensure that the slow mem has enough time to process
    if rising_edge(buffer_mem_clk) then
      -- if the memDriver is not busy it will check whether an request from main is pending.
      -- the inputs of mem are then set to process the request.
      if(memState = memIdle) then
        -- preset mem inputs. if an request is pending, it will set its needed inputs
        -- initialized here to ensure that if no request is pending nothing is done in mem.
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
        
        -- *** WB for dump request ***
        elsif(memRequest = memRequestWBForDump) then
          -- save the cache line index for later use in busy
          lastMemAddr := buffer_mem_addr;
          mem_we    <= '1';
          mem_addr(3 downto 0) <= lastMemAddr(3 downto 0);
          mem_addr(7 downto 4) <= cache(to_integer(unsigned(lastMemAddr(3 downto 0)))).tag;
          mem_data_in <= cache(to_integer(unsigned(lastMemAddr(3 downto 0)))).data;
          
          -- tell this process and the main process that mem is busy and its contents are undefined.
          memState <= memBusy;
          memContentState <= memContentUndefined;
        
        -- *** Dump request ***
        elsif(memRequest = memRequestDump) then
          mem_dump <= '1';
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
        
        -- check last request to call the cacheDriver to update the cache if necessary and to tell
        -- main that it has finished the request
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
          -- tell the main process that the content of mem is now read (not yet used, just here for
          -- consistency).
          memContentState <= memContentRead;
        
        -- *** Last request: WB ***
        elsif(lastMemRequest = memRequestWB) then
          -- update bits in cache line via CacheDriver
          -- buffer cache inputs for cacheDriver
          buffer_cacheFromMem_addr  <= lastMemAddr;
          -- tell cacheDriver to update data in cache
          cacheRequestFromMem <= cacheRequestCleanLine;
          -- tell the main process that the content of mem is now wb (not yet used, just here for
          -- consistency).
          memContentState <= memContentWB;
        
        -- *** Last request: WB ***
        elsif(lastMemRequest = memRequestWBForDump) then
          -- update bits in cache line via CacheDriver
          -- buffer cache inputs for cacheDriver
          buffer_cacheFromMem_addr  <= lastMemAddr;
          -- tell cacheDriver to update data in cache
          cacheRequestFromMem <= cacheRequestCleanLine;
          -- tell the main process that the content of mem is now wb (not yet used, just here for
          -- consistency).
          memContentState <= memContentWB;
        
        -- *** Last request: Dump ***
        elsif(lastMemRequest = memRequestDump) then
          -- tell the main process that the content of mem is now dumped.
          memContentState <= memContentDumped;
          -- no need to set cacheRequestFromMem here, since cache dump is controlled by main
        end if;
      end if;
    end if;
    
    -- forward the clock signal to mem after its inputs are set.
    mem_clk <= buffer_mem_clk;
  end process;
  
  
  -- ====================
  -- Controlls access to the cache.
  -- ====================
  cacheDriver: process(cacheRequestFromMem, cacheRequestFromMain)
    -- variable to temporarily save the index of the addressed cache line to reduce the conversion-
    -- calls from std_logic_vector to integer.
    variable tmpCacheIndex : integer := 0;
    -- holds the last value of cacheRequestFromMem and cacheRequestFromMain and is set if a
    -- different request comes in. They are used to determine whether memDriver or main has
    -- triggered the process. If both processes have pending requests, the request from main will
    -- be prioritized.
    variable prevRequestFromMem  : cacheRequests := noCacheRequest;
    variable prevRequestFromMain : cacheRequests := noCacheRequest;
  begin
    -- make sure it was cacheRequestFromMem that triggered the process
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
    
    
    -- make sure it was cacheRequestFromMain that triggered the process
    if( cacheRequestFromMain /= prevRequestFromMain) then
      -- save this request as previous
      prevRequestFromMain := cacheRequestFromMain;
      -- buffer the cache index, since the conversion is so long
      tmpCacheIndex := to_integer(unsigned(buffer_cacheFromMain_addr(3 downto 0)));
      
      -- *** Request from main: Init ***
      if( cacheRequestFromMain = cacheRequestInit ) then
        -- no need to write back cache, since all values in memory may change.
        -- no need to set all cacheline values (e.g. tag, d), valid ensures that cache access will
        -- query to mem
        -- invalidate cache for init
        for i in 0 to cache'length-1 loop
          cache(i).v <= '0';
        end loop;
        
      -- *** Request from main: Reset ***
      elsif( cacheRequestFromMain = cacheRequestReset ) then
        -- no need to write back cache, since all values in memory change
        -- no need to set all cacheline values (e.g. tag, d), valid ensures that cache access will
        -- query to mem
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
  
  
  
  -- ====================
  -- Controlls access to the cache.
  -- ====================
  execute: process (clk)
    -- variable to temporarily save the index and tag of the addressed cache line to reduce the
    -- conversion calls from std_logic_vector to integer and to make code lines shorter.
    variable tmpTag   : STD_LOGIC_VECTOR (3 downto 0) := (others => '0');
    variable tmpIndex : STD_LOGIC_VECTOR (3 downto 0) := (others => '0');
    variable tmpIntTag   : integer := 0;
    variable tmpIntIndex : integer := 0;
    
    -- help variable used to dump mem and cache. It is set to '0' if not all cache lines are
    -- written back to ensure that mem is only dumped with clean cache
    variable readyForDump : STD_LOGIC := '1';
    
  begin
    -- preset cacheDriver request. Located here to make sure the signal is reset between clks,
    -- since the cacheDriver only reacts on changing of the signal and other states than
    -- noCacheRequest.
    cacheRequestFromMain <= noCacheRequest;
  
    if rising_edge(clk) then
      -- aufteilen und zwischenspeichern der adresse zur leicheren handhabung
      tmpTag    := addr(7 downto 4);
      tmpIndex  := addr(3 downto 0);
      tmpIntTag   := to_integer(unsigned(tmpTag));
      tmpIntIndex := to_integer(unsigned(tmpIndex));
      
      -- buffer vorbelegen
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
        readyForDump := '1';
      
        -- write back cache for mem dump (last cache line first)
        for i in 0 to cache'length-1 loop
          if( (cache(i).v = '1') AND (cache(i).d = '1') ) then
            readyForDump := '0';
            -- wait until mem has finished its things
            if(memState = memIdle) then
              -- save the addr to provide memDriver with the right cache line
              buffer_mem_addr(3 downto 0) <= std_logic_vector(to_unsigned(i, 4));
              buffer_mem_addr(7 downto 4) <= (others => '0');
              -- send request to memDriver to wb mem
              memRequest <= memRequestWBForDump;
            end if;
          end if;
        end loop;
        
        if( readyForDump = '1' ) then
          -- wait until mem has finished its things
          if((memState = memIdle) AND (memContentState /= memContentDumped) ) then
            -- send request to memDriver to reset mem
            memRequest <= memRequestDump;
          end if;
        end if;
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
                -- used, since the mem_clk has its rising edge at the same time (disregarding gate
                -- latency).
                -- send request to memDriver to wb mem
                memRequest <= memRequestWB;
              end if;
            else
              -- valid, other data, clean => read data from cache
              -- wait until mem has finished its things
              if(memState = memIdle) then
                -- no need to save the addr to ensure that only the value at rising clk is
                -- used, since the mem_clk has its rising edge at the same time (disregarding gate
                -- latency).
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
            -- used, since the mem_clk has its rising edge at the same time (disregarding gate
            -- latency).
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
                -- used, since the mem_clk has its rising edge at the same time (disregarding gate
                -- latency).
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
        -- same outputs if no request to cache is forwarded to hold the current state
        bufferAck <= bufferAck;
        bufferOut <= bufferOut;
      
      
      -- *** Fehlerhafte Eingabe ***
      else
        -- disable outputs if erroneous inputs
        bufferAck <= '0';
        bufferOut <= "XXXXXXXX";
      
      end if;
    end if;
  end process;
  
  -- Ausgabenetz
  ack     <= bufferAck;
  output  <= bufferOut;

end Behavioral;