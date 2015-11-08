-- Testziele:
-- + Init
--    - ???????????????
-- + Reset
--  - Reset erfolgt
--  - Asynchron
--    - Sofort übernommen
--    - Keine Aenderung auch nach einem Takt und anderen Signalen


LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;

 
ENTITY memoryTB IS
END memoryTB;
 
ARCHITECTURE behavior OF memoryTB IS 
 
  -- Component Declaration for the Unit Under Test (UUT)
  COMPONENT memory
  PORT(
       clk : IN  std_logic;
       init : IN  std_logic;
       reset : IN  std_logic;
       re : IN  std_logic;
       we : IN  std_logic;
       addr : IN  std_logic_vector(7 downto 0);
       data_in : IN  std_logic_vector(7 downto 0);
       output : OUT  std_logic_vector(7 downto 0)
      );
  END COMPONENT;
    

  --Inputs
  signal clk : std_logic := '0';
  signal init : std_logic := '1';
  signal reset : std_logic := '0';
  signal re : std_logic := '0';
  signal we : std_logic := '0';
  signal addr : std_logic_vector(7 downto 0) := (others => '0');
  signal data_in : std_logic_vector(7 downto 0) := (others => '0');
  
  signal pruefzeitpunkt : std_logic := '1';

  --Outputs
  signal output : std_logic_vector(7 downto 0);

  -- Clock period definitions
  constant clk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
  uut: memory PORT MAP (
        clk => clk,
        init => init,
        reset => reset,
        re => re,
        we => we,
        addr => addr,
        data_in => data_in,
        output => output
      );

  -- Clock process definitions
  clk_process :process
  begin
    clk <= '0';
	  wait for clk_period/2;
    clk <= '1';
    wait for clk_period/2;
   end process;
   
  -- Signal zum Steuern der Testbench: Wann soll geprueft wird,
  -- und wann die Eingaeng gesetzt werden.
  pruefzeitpunkt_process: process
  begin
    pruefzeitpunkt <= '0';
    wait for 0.6 * clk_period;
    pruefzeitpunkt <= '1';
    wait for 0.2 * clk_period;
    pruefzeitpunkt <= '0';
    wait for 0.2 * clk_period;
  end process;


  -- Stimulus process
  stim_proc: process
    
    -- schleifen zaehler
    variable i : integer := 0;
    variable j : integer := 0;
    
    -- Speichergroeße
    variable n : integer := 80;
    
    -- Test Ein- und Ausgaben
    variable testMode : std_logic := '0';  -- 0:(re=1,we=0); 1:(re=0,we=1);
    variable testAddr : std_logic_vector(7 downto 0) := (others => '0');
    variable testData_in : std_logic_vector(7 downto 0) := (others => '0');
    variable testOutput : std_logic_vector(7 downto 0) := (others => '0');

    
  
  begin		
    -- hold reset state for 100 ns.
    wait for 100 ns;
    
    report "Beginn der Testbench!";
    
    
    
    -- Sichergehen das memory initialisiert ist.
    addr <= "00000000";
    data_in <= "00000000";
    re <= '0';
    we <= '0';
    init <= '1';
    reset <= '1';
    wait for clk_period*10;
    init <= '0';
    reset <= '0';
    wait for clk_period*10;
    
    -- ==========
    -- Testfall 1
    -- Pueft den Zustand nach Init und Reset (Output jeder Speicherzelle "00000000").
    -- Prueft das Lesen ("re") des Wertes "00000000".
    -- ==========
    for i in 0 to (n-1) loop
      -- Setze Testwerte
      testMode := '0';
      testAddr := std_logic_vector(to_unsigned(i, testAddr'length));
      testData_in := (others => '0');
      testOutput := (others => '0');
      -- Setze Eingaenge
      wait until falling_edge(pruefzeitpunkt);
      if testMode = '0' then
        re <= '1';
        we <= '0';
      else
        re <= '0';
        we <= '1';
      end if;
      addr <= testAddr;
      data_in <= testData_in;
      -- Pruefe Ausgaenge
      wait until rising_edge(pruefzeitpunkt);
      assert( output = testOutput )
        report "Testfall 1.1:" & CR &
          "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
          "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
          "Output   = " & integer'image(to_integer(unsigned(output)))
        severity failure;
    end loop;
    
    
    
    
    
    -- ==========
    -- Testfall 2
    -- Prueft das Schreiben ("we") in jede Speicherzelle.
    -- Prueft das "output" seinen Wert beim Schreiben nicht aendert.
    -- Prueft das Lesen ("re") jeder Speicherzelle.
    -- Prueft das "output" beim Lesen den gelesenen Wert nicht beeinflusst.
    -- Prueft das jeder geschriebene Wert der richtigen Speicherzelle zugeordnet wird.
    -- Prueft das jeder Schreib- oder Lesevorgang nach einem Takt abgeschlossen ist.
    -- Prueft das Wirken der Signale "addr", "data_in", "we", "re" und "output"
    --   (z.B. "addr" bestimmt die zu schreibende/lesende Adresse").
    -- Prueft das mindestens 80 Byte gespeichert werden koennen (Adresse "00000000" - "01001111").
    -- ==========
    for i in 0 to (n-1) loop
      -- Setze Testwerte
      testMode := '1';
      testAddr := std_logic_vector(to_unsigned(i, testAddr'length));
      testData_in := std_logic_vector(to_unsigned(n-i, testData_in'length));
      testOutput := (others => '0'); -- es soll immer noch 00000000 anliegen da write
      -- Setze Eingaenge
      wait until falling_edge(pruefzeitpunkt);
      if testMode = '0' then
        re <= '1';
        we <= '0';
      else
        re <= '0';
        we <= '1';
      end if;
      addr <= testAddr;
      data_in <= testData_in;
      -- Pruefe Ausgaenge
      wait until rising_edge(pruefzeitpunkt);
      assert( output = testOutput )
        report "Testfall 2.1:" & CR &
          "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
          "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
          "Output   = " & integer'image(to_integer(unsigned(output)))
        severity failure;
    end loop;
    
    for i in 0 to (n-1) loop
      -- Setze Testwerte
      testMode := '0';
      testAddr := std_logic_vector(to_unsigned(i, testAddr'length));
      testData_in := (others => '0'); -- sollte keine Auswirkung auf den Ausgang haben
      testOutput := std_logic_vector(to_unsigned(n-i, testAddr'length));
      -- Setze Eingaenge
      wait until falling_edge(pruefzeitpunkt);
      if testMode = '0' then
        re <= '1';
        we <= '0';
      else
        re <= '0';
        we <= '1';
      end if;
      addr <= testAddr;
      data_in <= testData_in;
      -- Pruefe Ausgaenge
      wait until rising_edge(pruefzeitpunkt);
      assert( output = testOutput )
        report "Testfall 2.2:" & CR &
          "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
          "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
          "Output   = " & integer'image(to_integer(unsigned(output)))
        severity failure;
    end loop;
    
    
    
    
    
    -- ==========
    -- Testfall 3
    -- Prueft das nach einem Reset der gesante Speicher geloescht ist.
    -- Prueft das jedes Bit jeder Speicherzelle mit 0 und 1 beschrieben werden kann.
    -- Prueft das nebeneinanderer liegende Bit einer Speicherzelle nicht ueberschieben werden.
    -- ==========
    -- *** Testinit ***
    -- Memory zuruecksetzen um definierten Teststart zu haben.
    addr <= "00000000";
    data_in <= "00000000";
    re <= '0';
    we <= '0';
    reset <= '1';
    wait for 1ns;
    reset <= '0';
    wait for 1ns;
    
    -- *** Testschritt 1 ***
    for i in 0 to (n-1) loop
      -- Setze Testwerte
      testMode := '1';
      testAddr := std_logic_vector(to_unsigned(i, testAddr'length));
      testData_in := "01010101";
      testOutput := (others => '0'); -- es soll immer noch 00000000 anliegen da write
      -- Setze Eingaenge
      wait until falling_edge(pruefzeitpunkt);
      if testMode = '0' then
        re <= '1';
        we <= '0';
      else
        re <= '0';
        we <= '1';
      end if;
      addr <= testAddr;
      data_in <= testData_in;
      -- Pruefe Ausgaenge
      wait until rising_edge(pruefzeitpunkt);
      assert( output = testOutput )
        report "Testfall 3.1:" & CR &
          "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
          "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
          "Output   = " & integer'image(to_integer(unsigned(output)))
        severity failure;
    end loop;
    
    -- *** Testschritt 2 ***
    for i in 0 to (n-1) loop
      -- Setze Testwerte
      testMode := '0';
      testAddr := std_logic_vector(to_unsigned(i, testAddr'length));
      testData_in := (others => '0'); -- sollte keine Auswirkung auf den Ausgang haben
      testOutput := "01010101";
      -- Setze Eingaenge
      wait until falling_edge(pruefzeitpunkt);
      if testMode = '0' then
        re <= '1';
        we <= '0';
      else
        re <= '0';
        we <= '1';
      end if;
      addr <= testAddr;
      data_in <= testData_in;
      -- Pruefe Ausgaenge
      wait until rising_edge(pruefzeitpunkt);
      assert( output = testOutput )
        report "Testfall 3.2:" & CR &
          "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
          "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
          "Output   = " & integer'image(to_integer(unsigned(output)))
        severity failure;
    end loop;
    
    -- *** Testschritt 3 ***
    for i in 0 to (n-1) loop
      -- Setze Testwerte
      testMode := '1';
      testAddr := std_logic_vector(to_unsigned(i, testAddr'length));
      testData_in := "10101010";
      testOutput := "01010101"; -- es soll immer noch "01010101" anliegen da write
      -- Setze Eingaenge
      wait until falling_edge(pruefzeitpunkt);
      if testMode = '0' then
        re <= '1';
        we <= '0';
      else
        re <= '0';
        we <= '1';
      end if;
      addr <= testAddr;
      data_in <= testData_in;
      -- Pruefe Ausgaenge
      wait until rising_edge(pruefzeitpunkt);
      assert( output = testOutput )
        report "Testfall 3.3:" & CR &
          "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
          "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
          "Output   = " & integer'image(to_integer(unsigned(output)))
        severity failure;
    end loop;
    
    -- *** Testschritt 4 ***
    for i in 0 to (n-1) loop
      -- Setze Testwerte
      testMode := '0';
      testAddr := std_logic_vector(to_unsigned(i, testAddr'length));
      testData_in := (others => '0'); -- sollte keine Auswirkung auf den Ausgang haben
      testOutput := "10101010";
      -- Setze Eingaenge
      wait until falling_edge(pruefzeitpunkt);
      if testMode = '0' then
        re <= '1';
        we <= '0';
      else
        re <= '0';
        we <= '1';
      end if;
      addr <= testAddr;
      data_in <= testData_in;
      -- Pruefe Ausgaenge
      wait until rising_edge(pruefzeitpunkt);
      assert( output = testOutput )
        report "Testfall 3.4:" & CR &
          "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
          "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
          "Output   = " & integer'image(to_integer(unsigned(output)))
        severity failure;
    end loop;
    
    
    
    
    
    -- ==========
    -- Testfall 4
    -- Prueft das eine Speicherzelle jeden moeglichen 8-Bit-Wert annehmen kann.
    -- Es wird nur drei Speicherzellen geprüft: 1. Speicherzelle, n. Speicherzelle und eine beliebige (2.) dazwichen.
    -- ==========
    -- *** Testinit ***
    -- Memory zuruecksetzen um definierten Teststart zu haben.
    addr <= "00000000";
    data_in <= "00000000";
    re <= '0';
    we <= '0';
    reset <= '1';
    wait for 1ns;
    reset <= '0';
    wait for 1ns;
    
    -- *** Testschritt 1 ***
    -- 1. Speicherzelle
    for i in 0 to (testData_in'length-1) loop
      -- Setze Testwerte
      testMode := '1';
      testAddr := "00000000";
      testData_in := std_logic_vector(to_unsigned(i, testData_in'length));
      testOutput := std_logic_vector(to_unsigned(i, testOutput'length)); -- es soll immer noch 00000000 anliegen da write
      -- Setze Eingaenge
      wait until falling_edge(pruefzeitpunkt);
      if testMode = '0' then
        re <= '1';
        we <= '0';
      else
        re <= '0';
        we <= '1';
      end if;
      addr <= testAddr;
      data_in <= testData_in;
      wait until rising_edge(pruefzeitpunkt);
      testMode := '0';
      wait until falling_edge(pruefzeitpunkt);
      if testMode = '0' then
        re <= '1';
        we <= '0';
      else
        re <= '0';
        we <= '1';
      end if;
      addr <= testAddr;
      data_in <= testData_in;
      -- Pruefe Ausgaenge
      wait until rising_edge(pruefzeitpunkt);
      assert( output = testOutput )
        report "Testfall 4.1:" & CR &
          "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
          "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
          "Output   = " & integer'image(to_integer(unsigned(output)))
        severity failure;
    end loop;
    
    -- *** Testschritt 2 ***
    -- Letzte Speicherzelle
    for i in 0 to (testData_in'length-1) loop
      -- Setze Testwerte
      testMode := '1';
      testAddr := std_logic_vector(to_unsigned(n-1, testAddr'length));
      testData_in := std_logic_vector(to_unsigned(i, testData_in'length));
      testOutput := std_logic_vector(to_unsigned(i, testOutput'length));
      -- Setze Eingaenge
      wait until falling_edge(pruefzeitpunkt);
      if testMode = '0' then
        re <= '1';
        we <= '0';
      else
        re <= '0';
        we <= '1';
      end if;
      addr <= testAddr;
      data_in <= testData_in;
      wait until rising_edge(pruefzeitpunkt);
      testMode := '0';
      wait until falling_edge(pruefzeitpunkt);
      if testMode = '0' then
        re <= '1';
        we <= '0';
      else
        re <= '0';
        we <= '1';
      end if;
      addr <= testAddr;
      data_in <= testData_in;
      -- Pruefe Ausgaenge
      wait until rising_edge(pruefzeitpunkt);
      assert( output = testOutput )
        report "Testfall 4.2:" & CR &
          "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
          "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
          "Output   = " & integer'image(to_integer(unsigned(output)))
        severity failure;
    end loop;
    
    -- *** Testschritt 3 ***
    -- 2. Speicherzelle
    for i in 0 to (testData_in'length-1) loop
      -- Setze Testwerte
      testMode := '1';
      testAddr := "00000001";
      testData_in := std_logic_vector(to_unsigned(i, testData_in'length));
      testOutput := std_logic_vector(to_unsigned(i, testOutput'length));
      -- Setze Eingaenge
      wait until falling_edge(pruefzeitpunkt);
      if testMode = '0' then
        re <= '1';
        we <= '0';
      else
        re <= '0';
        we <= '1';
      end if;
      addr <= testAddr;
      data_in <= testData_in;
      wait until rising_edge(pruefzeitpunkt);
      testMode := '0';
      wait until falling_edge(pruefzeitpunkt);
      if testMode = '0' then
        re <= '1';
        we <= '0';
      else
        re <= '0';
        we <= '1';
      end if;
      addr <= testAddr;
      data_in <= testData_in;
      -- Pruefe Ausgaenge
      wait until rising_edge(pruefzeitpunkt);
      assert( output = testOutput )
        report "Testfall 4.3:" & CR &
          "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
          "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
          "Output   = " & integer'image(to_integer(unsigned(output)))
        severity failure;
    end loop;
    
    
    
    
    
    -- ==========
    -- Testfall 5
    -- Prueft das maximal an Adresse n-1 -also 80 Speicherzellen- geschrieben
    --   oder gelesen werden kann. Ansonsten soll "XXXXXXXX" an "output" anliegen.
    -- Prueft das danach wieder geschrieben und gelesen werden kann
    -- ==========
    -- *** Testinit ***
    -- Memory zuruecksetzen um definierten Teststart zu haben.
    addr <= "00000000";
    data_in <= "00000000";
    re <= '0';
    we <= '0';
    reset <= '1';
    wait for 1ns;
    reset <= '0';
    wait for 1ns;
    
    -- *** Testschritt 1 ***
    -- Lesender Zugriff
    -- Setze Testwerte
    testMode := '0';
    testAddr := std_logic_vector(to_unsigned(n+1, testAddr'length));
    testData_in := (others => '0'); -- beliebiege Eingangsdaten
    testOutput := "XXXXXXXX";
    -- Setze Eingaenge
    wait until falling_edge(pruefzeitpunkt);
    if testMode = '0' then
      re <= '1';
      we <= '0';
    else
      re <= '0';
      we <= '1';
    end if;
    addr <= testAddr;
    data_in <= testData_in;
    -- Pruefe Ausgaenge
    wait until rising_edge(pruefzeitpunkt);
    -- Ausgabe mit std_logic'image da mit der anderen Funktion nur '0' und '1' konvertiert werden
    assert( output = testOutput )
      report "Testfall 5.1:" & CR &
        "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
        "Erwartet = " &
          std_logic'image(testOutput(7)) & std_logic'image(testOutput(6)) & std_logic'image(testOutput(5)) & std_logic'image(testOutput(4)) &
          std_logic'image(testOutput(3)) & std_logic'image(testOutput(2)) & std_logic'image(testOutput(1)) & std_logic'image(testOutput(0)) &
        "Output   = " &
          std_logic'image(output(7)) & std_logic'image(output(6)) & std_logic'image(output(5)) & std_logic'image(output(4)) &
          std_logic'image(output(3)) & std_logic'image(output(2)) & std_logic'image(output(1)) & std_logic'image(output(0))
      severity failure;
      
    -- *** Testschritt 2 ***
    -- Schreibe und lese anderen Wert
    -- Setze Testwerte
    testMode := '1';
    testAddr := "00000000"; -- beliebiege Adresse
    testData_in := (others => '0'); -- beliebiege Eingangsdaten
    testOutput := (others => '0');
    -- Setze Eingaenge
    wait until falling_edge(pruefzeitpunkt);
    if testMode = '0' then
      re <= '1';
      we <= '0';
    else
      re <= '0';
      we <= '1';
    end if;
    addr <= testAddr;
    data_in <= testData_in;
    wait until rising_edge(pruefzeitpunkt);
    testMode := '0';
    wait until falling_edge(pruefzeitpunkt);
    if testMode = '0' then
      re <= '1';
      we <= '0';
    else
      re <= '0';
      we <= '1';
    end if;
    addr <= testAddr;
    data_in <= testData_in;
    -- Pruefe Ausgaenge
    wait until rising_edge(pruefzeitpunkt);
    -- Ausgabe mit std_logic'image da mit der anderen Funktion nur '0' und '1' konvertiert werden
    assert( output = testOutput )
      report "Testfall 5.2:" & CR &
        "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
        "Erwartet = " &
          std_logic'image(testOutput(7)) & std_logic'image(testOutput(6)) & std_logic'image(testOutput(5)) & std_logic'image(testOutput(4)) &
          std_logic'image(testOutput(3)) & std_logic'image(testOutput(2)) & std_logic'image(testOutput(1)) & std_logic'image(testOutput(0)) &
        "Output   = " &
          std_logic'image(output(7)) & std_logic'image(output(6)) & std_logic'image(output(5)) & std_logic'image(output(4)) &
          std_logic'image(output(3)) & std_logic'image(output(2)) & std_logic'image(output(1)) & std_logic'image(output(0))
      severity failure;
      
    -- *** Testschritt 3 ***
    -- Schreibender Zugriff
    -- Setze Testwerte
    testMode := '1';
    testAddr := std_logic_vector(to_unsigned(n+1, testAddr'length));
    testData_in := (others => '0'); -- beliebiege Eingangsdaten
    testOutput := "XXXXXXXX";
    -- Setze Eingaenge
    wait until falling_edge(pruefzeitpunkt);
    if testMode = '0' then
      re <= '1';
      we <= '0';
    else
      re <= '0';
      we <= '1';
    end if;
    addr <= testAddr;
    data_in <= testData_in;
    -- Pruefe Ausgaenge
    wait until rising_edge(pruefzeitpunkt);
    -- Ausgabe mit std_logic'image da mit der anderen Funktion nur '0' und '1' konvertiert werden
    assert( output = testOutput )
      report "Testfall 5.3:" & CR &
        "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
        "Erwartet = " &
          std_logic'image(testOutput(7)) & std_logic'image(testOutput(6)) & std_logic'image(testOutput(5)) & std_logic'image(testOutput(4)) &
          std_logic'image(testOutput(3)) & std_logic'image(testOutput(2)) & std_logic'image(testOutput(1)) & std_logic'image(testOutput(0)) &
        "Output   = " &
          std_logic'image(output(7)) & std_logic'image(output(6)) & std_logic'image(output(5)) & std_logic'image(output(4)) &
          std_logic'image(output(3)) & std_logic'image(output(2)) & std_logic'image(output(1)) & std_logic'image(output(0))
      severity failure;
    
    
    
    
    
    -- ==========
    -- Testfall 6
    -- Prueft das Lese- und Schreiboperationen nur bei steigender Flanke uebernommen wird.
    -- Prueft das ein Abbrechen des Lesevorgangs "output" nicht ueberschreibt und "output"
    --  bis zum naechsten Lesebefehl gehalten wird (ein Takt ohne Lesebefehl)
    -- ==========
    -- *** Testinit ***    
    -- Init:
    --  0: "00000000"
    --  1: "01010101"
    -- Memory zuruecksetzen um definierten Teststart zu haben.
    addr <= "00000000";
    data_in <= "00000000";
    re <= '0';
    we <= '0';
    reset <= '1';
    wait for 1ns;
    reset <= '0';
    wait for 1ns;
    -- Belege Speicher vor
    addr <= "00000001";
    data_in <= "01010101";
    we <= '1';
    wait until rising_edge(clk);
    we <= '0';

    -- *** Testschritt 1 ***
    -- Vorzeitige Aenderung "we"
    -- Ablauf:
    --  Warte bis steigende Flanke
    --  we <= '1'
    --  addr <= "00000000"
    --  data_in <= "10101010"
    --  Warte bis fallende Flanke
    --  we <= '0'
    --  Warte bis steigende Flanke
    --  Lese und Pruefe Addr. 0
    -- Setze Testwerte
    testAddr := "00000000";
    testData_in := "10101010"; -- beliebiege Eingangsdaten
    testOutput := "00000000"; -- keine Aenderung zum Reset
    -- Warte bis steigende Taktflanke vorbei
    wait until rising_edge(clk);
    wait for (clk_period/10);
    -- Setze Eingaenge
    re <= '0';
    we <= '1';
    addr <= testAddr;
    data_in <= testData_in;
    -- Warte auf fallende Flanke vorbei
    wait until falling_edge(clk);
    wait for (clk_period/4);
    -- Breche "we" ab
    we <= '0';
    -- Warte bis steigende Taktflanke vorbei
    wait until rising_edge(clk);
    wait for (clk_period/10);
    -- Pruefe Vorgang
    -- Setze Eingaenge zum Lesen
    re <= '1';
    we <= '0';
    addr <= testAddr;
    data_in <= testData_in;
    -- Pruefe ob alter Wert noch vorhanden
    wait until rising_edge(pruefzeitpunkt);
    assert( output = testOutput )
      report "Testfall 6.1:" & CR &
        "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
        "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
        "Output   = " & integer'image(to_integer(unsigned(output)))
      severity failure;

    -- *** Testschritt 2 ***
    -- Vorzeitige Aenderung "data_in"
    -- Ablauf:
    --  Warte bis steigende Flanke
    --  we <= '1'
    --  addr <= "00000000"
    --  data_in <= "10101010"
    --  Warte bis fallende Flanke
    --  data_in <= "01010101"
    --  Warte bis steigende Flanke
    --  Lese und Pruefe Addr. 00
    -- Setze Testwerte
    testAddr := "00000000";
    testData_in := "10101010"; -- beliebiege Eingangsdaten
    testOutput := "00000000"; -- keine Aenderung zum Reset
    -- Warte bis steigende Taktflanke vorbei
    wait until rising_edge(clk);
    wait for (clk_period/10);
    -- Setze Eingaenge
    re <= '0';
    we <= '1';
    addr <= testAddr;
    data_in <= testData_in;
    -- Warte auf fallende Flanke vorbei
    wait until falling_edge(clk);
    wait for (clk_period/4);
    -- Aendere "data_in" ab
    data_in <= "01010101";
    -- Warte bis steigende Taktflanke vorbei
    wait until rising_edge(clk);
    wait for (clk_period/10);
    -- Pruefe Vorgang
    -- Setze Eingaenge zum Lesen
    re <= '1';
    we <= '0';
    addr <= testAddr;
    data_in <= testData_in;
    -- Pruefe ob alter Wert noch vorhanden
    wait until rising_edge(pruefzeitpunkt);
    assert( output = testOutput )
      report "Testfall 6.2:" & CR &
        "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
        "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
        "Output   = " & integer'image(to_integer(unsigned(output)))
      severity failure;

    -- *** Testschritt 3 ***
    -- Vorzeitige Aenderung "addr"
    -- Ablauf:
    --  Warte bis steigende Flanke
    --  we <= '1'
    --  addr <= "00000000"
    --  data_in <= "10101010"
    --  Warte bis fallende Flanke
    --  addr <= "00000001"
    --  Warte bis steigende Flanke
    --  Lese und Pruefe Addr. 01
    -- Setze Testwerte
    testAddr := "00000000";
    testData_in := "10101010"; -- beliebiege Eingangsdaten
    testOutput := "00000000"; -- keine Aenderung zum Reset Addr 01
    -- Warte bis steigende Taktflanke vorbei
    wait until rising_edge(clk);
    wait for (clk_period/10);
    -- Setze Eingaenge
    re <= '0';
    we <= '1';
    addr <= testAddr;
    data_in <= testData_in;
    -- Warte auf fallende Flanke vorbei
    wait until falling_edge(clk);
    wait for (clk_period/4);
    -- Aendere "data_in" ab
    addr <= "00000001";
    -- Warte bis steigende Taktflanke vorbei
    wait until rising_edge(clk);
    wait for (clk_period/10);
    -- Pruefe Vorgang Addr 0
    -- Setze Eingaenge zum Lesen
    re <= '1';
    we <= '0';
    addr <= testAddr;
    data_in <= testData_in;
    -- Pruefe ob alter Wert noch vorhanden
    wait until rising_edge(pruefzeitpunkt);
    assert( output = testOutput )
      report "Testfall 6.3.1:" & CR &
        "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
        "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
        "Output   = " & integer'image(to_integer(unsigned(output)))
      severity failure;
    testAddr := "00000001";
    testOutput := "01010101";
    -- Pruefe Vorgang Addr 1
    -- Setze Eingaenge zum Lesen
    re <= '1';
    we <= '0';
    addr <= testAddr;
    data_in <= testData_in;
    -- Pruefe ob alter Wert noch vorhanden
    wait until rising_edge(pruefzeitpunkt);
    assert( output = testOutput )
      report "Testfall 6.3.2:" & CR &
        "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
        "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
        "Output   = " & integer'image(to_integer(unsigned(output)))
      severity failure;

    -- *** Testschritt 4 ***
    -- Vorzeitige Aenderung "data_in"
    -- Ablauf:
    --  Lese Adresse 0 und setzte "re" danach auf '0'
    --  Warte bis steigende Flanke
    --  re <= '1'
    --  addr <= "00000001"
    --  testOutput <= "00000000" (Addr 0)
    --  Warte bis fallende Flanke
    --  re <= '0'
    --  Warte bis steigende Flanke
    --  Lese und Pruefe Addr. 0
    -- Setze Eingaenge (Lese Addr 0)
    re <= '1';
    we <= '0';
    addr <= "00000000";
    data_in <= "00000000";
    -- Setze Testwerte
    testAddr := "00000001";
    testData_in := "00000000"; -- beliebiege Eingangsdaten
    testOutput := "00000000"; -- keine Aenderung zum Reset
    -- Lese
    wait until rising_edge(clk);
    wait for (clk_period/10);
    re <= '0';
    -- Warte bis steigende Taktflanke vorbei
    wait until rising_edge(clk);
    wait for (clk_period/10);
    -- Setze Eingaenge (Test Addr 1)
    re <= '1';
    we <= '0';
    addr <= testAddr;
    data_in <= testData_in;
    -- Warte auf fallende Flanke vorbei
    wait until falling_edge(clk);
    wait for (clk_period/4);
    -- Breche "re" ab
    re <= '0';
    -- Warte bis steigende Taktflanke vorbei
    wait until rising_edge(clk);
    wait for (clk_period/10);
    assert( output = testOutput )
      report "Testfall 6.4:" & CR &
        "Adresse  = " & integer'image(to_integer(unsigned(testAddr))) & CR &
        "Erwartet = " & integer'image(to_integer(unsigned(testOutput))) & CR &
        "Output   = " & integer'image(to_integer(unsigned(output)))
      severity failure;
    
    
    
    
    report "Ende der Testbench!" severity failure;
    wait;
  end process;

END;
