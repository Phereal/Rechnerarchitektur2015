---------------------------------------------------------------------------------------------------
-- Rechnerarchitektur und Eingebettete Systeme
-- Uebungszettel 2 - Aufgabe 2: Direct Mapped Cache Testbench
--
-- Die Testfaelle wurden so gewahlt, dass zum Einen die Anforderungen aus der Aufgabestellung ge-
-- prueft werden und zum Anderen wurden weitere Testsfaelle gewaehlt, die die verschiedenen
-- Zustaende des Caches pruefen (Init, Reset, Dump, Read, Write, KeineEingabe).
-- Weiterhin wurde die Funktionalitaet WriteBack, ausgeloest sowohl durch read als auch durch write
-- getestet und das Verhalten der Beschreibung am Beginn der CachedMemory.vhd entspricht.
-- Es wurde geprueft ob alle 80 Byte des Speichers addressierbar sind.
--
-- Zuordnung von Testfall zu Aufgabenstellung:
-- 1. "Der obige Speicher soll um einen direct mapped Cache erweitert werden, der 16 Adressen 
--    vorhalten kann."
--    Wird durch Testfall 2 und 3 getestet indem zwei Sets von 16 Adressen hintereinander gelesen
--    werden. Das Ueberpruefen von ack laesst darauf schliessen ob der 2. set aus dem Speicher
--    geladen wurde (zeit unterschied bis ack). Weiterhin muessen die gelesenen Cachelines mit
--    den erwarteten Werten im Speicher ueber einstimmen.
--
-- 2. "Der Ausgang ack signalisiert hierbei, dass die gewünschten Daten am Ausgang anliegen. Der
--    Wert wird auf 0 gesetzt, sobald eine Leseanfrage eingeht."
--    Wird durch Testfall 8 geprueft.
--
-- 3. "Beachte hierbei, dass es wichtig ist, dass ack nicht auf 1 steht, bevor die Daten wirklich
--    vorliegen."
--    Wird durch fast alle Tests geprueft (z.B. Testfall 1 und 2) da direkt nach rising_edge(ack)
--    der output geprueft wird.
--
-- 4. "Es gilt zu beachten, dass der Cache über eine achtmal schnellere clock betrieben wird, als
--    der eigentliche Speicher (da sonst ein Cache auch ziemlich sinnlos wäre). Es wird also eine
--    zweite clock domain benötigt. Hierzu muss ein entsprechender clock divider implementiert
--    werden."
--    Wird durch fast alle Tests geprueft (z.B. Testfall 1) ueber ack = '0' bei Speicher
--    zugriffen.
--
-- 5. "Beachte, dass beim dumpen des Caches natürlich die Einträge im Cache selbst berücksichtigt
--    werden müssen."
--    Wird durch Testfall 9 und 10 getestet indem zuerst der Speicher mit Werten gefuellt wird,
--    wovon einige dirty sind und der Speicher danach gedumpt wird. Das dumpfile dump.dat wurde
--    danach manuell ueberpruft, es entsprach den erwarteten Werten.
--
-- Weitere Tests:
-- 6.   Init wird vor Testfall 1 ausgefuehrt und dann durch Testfall 1 geprueft. Weiterhin wurde in
--      Isim der interne Speicher von mem manuel ueberprueft (Entsprach den Vorstellungen)
-- 7.   Reset wird vor Testfall 1 ausgefuehrt und dann manuell in Isim ueberprueft ob alle Cachelines
--      invalid sind.
-- 8.   Read mit CacheMiss wird in Testfall 1 geprueft.
-- 9.   Read mit CacheHit wird in Testfall 2 geprueft.
-- 10.  Read mit WriteBack wird in Testfall 3 geprueft.
-- 11.  Mehrfacher Read nach WriteBack wird in Testfall 4 geprueft.
-- 12.  Schreiben ohne WriteBack wird in Testfall 5 geprueft.
-- 13.  Ob zuvor geschriebene Werte durch Schreiben anderer Daten zurueckgeschrieben 
--      (writeback) werden, wird in Testfall 6 geprueft.
-- 14.  Ob zuvor geschriebene Werte durch Lesen anderer Daten zurueckgeschrieben 
--      (writeback) werden, wird in Testfall 7 geprueft.
-- 15.  Addressierbarkeit aller 80 Speicheradressen, wird in Testfall 9 geprueft.
---------------------------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;
 
ENTITY CachedMemoryTb IS
END CachedMemoryTb;
 
ARCHITECTURE behavior OF CachedMemoryTb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT CachedMemory
    PORT(
         clk : IN  std_logic;
         init : IN  std_logic;
         dump : IN  std_logic;
         reset : IN  std_logic;
         re : IN  std_logic;
         we : IN  std_logic;
         addr : IN  std_logic_vector(7 downto 0);
         data_in : IN  std_logic_vector(7 downto 0);
         output : OUT  std_logic_vector(7 downto 0);
         ack : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal clk : std_logic := '0';
   signal init : std_logic := '0';
   signal dump : std_logic := '0';
   signal reset : std_logic := '0';
   signal re : std_logic := '0';
   signal we : std_logic := '0';
   signal addr : std_logic_vector(7 downto 0) := (others => '0');
   signal data_in : std_logic_vector(7 downto 0) := (others => '0');

 	--Outputs
   signal output : std_logic_vector(7 downto 0);
   signal ack : std_logic;

   -- Clock period definitions
   constant clk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: CachedMemory PORT MAP (
          clk => clk,
          init => init,
          dump => dump,
          reset => reset,
          re => re,
          we => we,
          addr => addr,
          data_in => data_in,
          output => output,
          ack => ack
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
 

  -- Stimulus process
  stim_proc: process
  begin
    init     <= '0';
    dump     <= '0';
    reset    <= '0';
    re       <= '0';
    we       <= '0';
    addr     <= (others => '0');
    data_in  <= (others => '0');
  
    wait for 100 ns;
    
    
    
    -- hold reset state for 100 ns.
    
    report "Beginn der Testbench!";
    reset    <= '1';
    wait until rising_edge(clk);
    wait for (clk_period*16);
    reset    <= '0';
    wait until rising_edge(clk);
    
    init    <= '1';
    wait until rising_edge(clk);
    wait for (clk_period*16);
    init    <= '0';
    wait until rising_edge(clk);
    
    -- Testfall 1: Pruefe das auslesen jeder cache line (hole data aus mem)
    for i in 0 to (15) loop
      init     <= '0';
      dump     <= '0';
      reset    <= '0';
      re       <= '1';
      we       <= '0';
      addr     <= std_logic_vector(to_unsigned(i, addr'length));
      data_in  <= (others => '0');
      
      wait until rising_edge(clk);
      wait for (clk_period/2);
      
      assert( ack = '0' )
        report "Testfall 1." & integer'image(i) & ": Ack != 0"
        severity failure;
        
      wait until rising_edge(ack);
      assert( output = std_logic_vector(to_unsigned(i, addr'length)) )
        report "Testfall 1." & integer'image(i) & ": out != " & integer'image(i)
        severity failure;
        
    end loop;
    
    -- Testfall 2: Pruefe das auslesen jeder cache line (hole data aus cache)
    for i in 0 to (15) loop
      init     <= '0';
      dump     <= '0';
      reset    <= '0';
      re       <= '1';
      we       <= '0';
      addr     <= std_logic_vector(to_unsigned(i, addr'length));
      data_in  <= (others => '0');
      
      wait until rising_edge(clk);
      wait for (clk_period/2);
      
      assert( ack = '1' )
        report "Testfall 2." & integer'image(i) & ": Ack != 1"
        severity failure;
      assert( output = std_logic_vector(to_unsigned(i, addr'length)) )
        report "Testfall 2." & integer'image(i) & ": out != " & integer'image(i)
        severity failure;
    end loop;
    
    -- Testfall 3: Pruefe das uebr-lesen jeder cache line (anderer tag, hole data aus mem)
    for i in 16 to (31) loop
      init     <= '0';
      dump     <= '0';
      reset    <= '0';
      re       <= '1';
      we       <= '0';
      addr     <= std_logic_vector(to_unsigned(i, addr'length));
      data_in  <= (others => '0');
      
      wait until rising_edge(clk);
      wait for (clk_period/2);
      
      assert( ack = '0' )
        report "Testfall 3." & integer'image(i) & ": Ack != 0"
        severity failure;
        
      wait until rising_edge(ack);
      assert( output = std_logic_vector(to_unsigned(i, addr'length)) )
        report "Testfall 3." & integer'image(i) & ": out != " & integer'image(i)
        severity failure;
        
    end loop;
    
    -- Testfall 4: Pruefe das auslesen jeder zuvor ueber-lesenden cache line (hole data aus cache)
    for i in 16 to (31) loop
      init     <= '0';
      dump     <= '0';
      reset    <= '0';
      re       <= '1';
      we       <= '0';
      addr     <= std_logic_vector(to_unsigned(i, addr'length));
      data_in  <= (others => '0');
      
      wait until rising_edge(clk);
      wait for (clk_period/2);
      
      assert( ack = '1' )
        report "Testfall 4." & integer'image(i) & ": Ack != 1"
        severity failure;
      assert( output = std_logic_vector(to_unsigned(i, addr'length)) )
        report "Testfall 4." & integer'image(i) & ": out != " & integer'image(i)
        severity failure;
    end loop;
    
    -- Testfall 5: Pruefe das Schreiben in den cache (kein mem Zugriff, da wb nur wenn valid, dirty und andere daten)
    for i in 0 to (15) loop
      init     <= '0';
      dump     <= '0';
      reset    <= '0';
      re       <= '0';
      we       <= '1';
      addr     <= std_logic_vector(to_unsigned(i, addr'length));
      data_in  <= std_logic_vector(to_unsigned(i+3, addr'length));
      
      wait until rising_edge(clk);
      wait for (clk_period/2);
      
      assert( ack = '0' )
        report "Testfall 5.1." & integer'image(i) & ": Ack != 0"
        severity failure;
    end loop;
    
    for i in 0 to (15) loop
      init     <= '0';
      dump     <= '0';
      reset    <= '0';
      re       <= '1';
      we       <= '0';
      addr     <= std_logic_vector(to_unsigned(i, addr'length));
      data_in  <= (others => '0');
      
      wait until rising_edge(clk);
      wait for (clk_period/2);
      
      assert( ack = '1' )
        report "Testfall 5.2." & integer'image(i) & ": Ack != 1"
        severity failure;
      assert( output = std_logic_vector(to_unsigned(i+3, addr'length)) )
        report "Testfall 5.2." & integer'image(i) & ": out != " & integer'image(i+3)
        severity failure;
    end loop;
    
    -- Testfall 6: Pruefe wb durch ueberschreiben (wb, da valid, dirty und andere daten durch Testfall 5)
    for i in 16 to (31) loop
      init     <= '0';
      dump     <= '0';
      reset    <= '0';
      re       <= '0';
      we       <= '1';
      addr     <= std_logic_vector(to_unsigned(i, addr'length));
      data_in  <= std_logic_vector(to_unsigned(i+7, addr'length));
      
      wait until rising_edge(clk);
      wait for (clk_period*16); -- warte lange genug fuer wb
      
      assert( ack = '0' )
        report "Testfall 6.1." & integer'image(i) & ": Ack != 0"
        severity failure;
    end loop;
    
    for i in 16 to (31) loop
      init     <= '0';
      dump     <= '0';
      reset    <= '0';
      re       <= '1';
      we       <= '0';
      addr     <= std_logic_vector(to_unsigned(i, addr'length));
      data_in  <= (others => '0');
      
      wait until rising_edge(clk);
      wait for (clk_period/2); -- kurz warten da 16-31 in cache
      
      assert( ack = '1' )
        report "Testfall 6.2." & integer'image(i) & ": Ack != 1"
        severity failure;
      assert( output = std_logic_vector(to_unsigned(i+7, addr'length)) )
        report "Testfall 6.2." & integer'image(i) & ": out != " & integer'image(i+7)
        severity failure;
    end loop;
    
    -- Testfall 7: Pruefe wb durch ueberlesen (wb, da valid, dirty und andere daten durch Testfall 5)
    for i in 0 to (15) loop
      init     <= '0';
      dump     <= '0';
      reset    <= '0';
      re       <= '1';
      we       <= '0';
      addr     <= std_logic_vector(to_unsigned(i, addr'length));
      data_in  <= (others => '0');
      
      wait until rising_edge(clk);
      wait for (clk_period/2); -- kurz warten um ack zu pruefen
      
      assert( ack = '0' )
        report "Testfall 7.1." & integer'image(i) & ": Ack != 0"
        severity failure;
      
      wait until rising_edge(ack);
      
      -- vergleich mit werten aus Testfall 5
      assert( output = std_logic_vector(to_unsigned(i+3, addr'length)) )
        report "Testfall 7.1." & integer'image(i) & ": out != " & integer'image(i+3)
        severity failure;
    end loop;
    
    for i in 16 to (31) loop
      init     <= '0';
      dump     <= '0';
      reset    <= '0';
      re       <= '1';
      we       <= '0';
      addr     <= std_logic_vector(to_unsigned(i, addr'length));
      data_in  <= (others => '0');
      
      wait until rising_edge(clk);
      wait for (clk_period/2); -- kurz warten um ack zu pruefen
      
      assert( ack = '0' )
        report "Testfall 7.2." & integer'image(i) & ": Ack != 0"
        severity failure;
      
      wait until rising_edge(ack);
      
      -- vergleich mit werten aus Testfall 6
      assert( output = std_logic_vector(to_unsigned(i+7, addr'length)) )
        report "Testfall 7.2." & integer'image(i) & ": out != " & integer'image(i+7)
        severity failure;
    end loop;
    
    
    -- Testfall 8: Pruefe das ack bis zum naechsten lesen 1 ist
    -- bereite dirty cache vor
    for i in 32 to (47) loop
      init     <= '0';
      dump     <= '0';
      reset    <= '0';
      re       <= '0';
      we       <= '1';
      addr     <= std_logic_vector(to_unsigned(i, addr'length));
      data_in  <= (others => '1');
      
      wait until rising_edge(clk);
      wait for (clk_period/2); -- kurz warten um ack zu pruefen
      
      assert( ack = '0' )
        report "Testfall 8.1." & integer'image(i) & ": Ack != 0"
        severity failure;
    end loop;
    
    -- lese wert aus cache
    init     <= '0';
    dump     <= '0';
    reset    <= '0';
    re       <= '1';
    we       <= '0';
    addr     <= std_logic_vector(to_unsigned(32, addr'length));
    data_in  <= (others => '0');
      
    wait until rising_edge(clk);
    wait for (clk_period/2); -- kurz warten um ack zu pruefen
      
    assert( ack = '1' )
      report "Testfall 8.2: Ack != 1"
      severity failure;
    assert( output = "11111111" )
      report "Testfall 8.2: out != 11111111"
      severity failure;
    
    -- disable re
    init     <= '0';
    dump     <= '0';
    reset    <= '0';
    re       <= '0';
    we       <= '0';
    addr     <= (others => '0');
    data_in  <= (others => '0');
    
    for i in 0 to (100) loop -- warte 100 takte
      wait until rising_edge(clk);
      wait for (clk_period/2); -- kurz warten um ack zu pruefen
      
      assert( ack = '1' )
        report "Testfall 8.3." & integer'image(i) & ": Ack != 1"
        severity failure;
      assert( output = "11111111" )
        report "Testfall 8.3: out != 11111111"
        severity failure;
    end loop;
    
    -- lese wert aus mem um ack auf 0 zu setzen
    init     <= '0';
    dump     <= '0';
    reset    <= '0';
    re       <= '1';
    we       <= '0';
    addr     <= (others => '0');
    data_in  <= (others => '0');
      
    wait until rising_edge(clk);
    wait for (clk_period/2); -- kurz warten um ack zu pruefen
      
    assert( ack = '0' )
      report "Testfall 8.4: Ack != 0"
      severity failure;
      
    wait until rising_edge(ack);
    
    assert( output = std_logic_vector(to_unsigned(0+3, addr'length)) ) -- wert aus Testfall 5
      report "Testfall 8.4: out != " & integer'image(0+3)
      severity failure;
    
    -- Testfall 9: Pruefe das der gesamte Speicher adressierbar ist 0-79 (schreiben und lesen)
    -- bereite dirty cache vor
    for i in 0 to (79) loop
      init     <= '0';
      dump     <= '0';
      reset    <= '0';
      re       <= '0';
      we       <= '1';
      addr     <= std_logic_vector(to_unsigned(i, addr'length));
      data_in  <= std_logic_vector(to_unsigned(i*2, addr'length));
      
      wait until rising_edge(clk);
      wait for (clk_period*16); -- kurz warten um ack zu pruefen
      
      assert( ack = '0' )
        report "Testfall 9.1." & integer'image(i) & ": Ack != 0"
        severity failure;
    end loop;
    
    for i in 0 to (79) loop
      init     <= '0';
      dump     <= '0';
      reset    <= '0';
      re       <= '1';
      we       <= '0';
      addr     <= std_logic_vector(to_unsigned(i, addr'length));
      data_in  <= (others => '0');
      
      wait until rising_edge(clk);
      wait for (clk_period/2); -- kurz warten um ack zu pruefen
      if(ack = '0') then
        wait until rising_edge(ack);
      end if;
    
    assert( output = std_logic_vector(to_unsigned(i*2, output'length)) )
      report "Testfall 9.2." & integer'image(i) & ": out != " & integer'image(i*2)
      severity failure;
    end loop;
    
    
    
    
    
    -- Tetsfall 10:  Gebe den Inhalt des Caches + memory in dump.dat aus.
    -- Schreibe den Inhalt des Caches in Memory und wenn alle Schreiboperationen abgeschlossen sind,
    -- setze das mem_dump = '1'!
    init     <= '0';
    dump     <= '1';
    reset    <= '0';
    re       <= '0';
    we       <= '0';
    addr     <= (others => '0');
    data_in  <= (others => '0');
  
    wait until rising_edge(clk); -- wir warten auf die stabile Taktflanke, damit die Signale gesetzt werden können
    wait for (clk_period*34*8); -- wir müssen 16 Memory [daher die *8] Zyklen warten, damit der aktuelle Inhalt des caches
    -- in den Memory geschrieben werden kann. 16 weitere Zyklen nutzen wir um auf das Aknowledge des Memory-Prozesses zu warten!
    -- Anschließend warten wir noch 2 Memory Zyklen, damit Memory in dump.dat schreiben.
    -- kann und dies auf jeden Fall abgeschlossen hat. Somit kommen wir auch insgesamt 34*8 Zyklen!
    report "dump.dat geschrieben!";
  
    init     <= '0';
    dump     <= '0';
    reset    <= '0';
    re       <= '0';
    we       <= '0';
    addr     <= (others => '0');
    data_in  <= (others => '0');
  
    wait until rising_edge(clk); 
    wait for (clk_period*2*8);
    
    

    report "Ende der Testbench!" severity failure;
    wait;
  end process;

END;
