--Das Testziel dieser Testbench ist ein Ablauf wie in der Aufgabenstellung beschrieben.
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
ENTITY ghost_tb IS
END ghost_tb;
 
ARCHITECTURE behavior OF ghost_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT ghost
    PORT(
         clk : IN  std_logic;
         move : IN  std_logic;
         eyes : OUT  std_logic;
         chains : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal clk : std_logic := '0';
   signal move : std_logic := '0';

 	--Outputs
   signal eyes : std_logic;
   signal chains : std_logic;

   -- Clock period definitions
   constant clk_period : time := 1000 ms;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: ghost PORT MAP (
          clk => clk,
          move => move,
          eyes => eyes,
          chains => chains
        );

   -- Clock process definitions
   clk_process :process
   begin
	-- Die Taktflanke wird immer zu Beginn des Taktzyklus gegeben und nicht erst zur häflte des Taktzyklusses.
	-- Damit lassen sich beim testen die Zeiten besser ablesen!
		clk <= '1';
		wait for clk_period/2;
		clk <= '0';
		wait for clk_period/2;

   end process;

   -- Stimulus process
   stim_proc: process
   begin
		report "Beginn der Testbench!";
		-- Testen wie sich die Schaltung nach dem einschalten erhält!
		report "Testschritt 1 nichtstun";
      wait for clk_period*1;
		assert chains = '0' report "Kettengeraschel, soll aber nicht" severity warning;
		assert eyes = '0' report "Augen Leuchten, sollen aber nicht" severity warning;
		
		-- Testen, was passiert wenn eine Bewegung erkannt wird! Die Ketten sollen aktiviert werden!
		report "Testschritt 2 aktivieren des Bewegungsmelders";
		move <= '1';
		wait for clk_period*1; -- Zeit, bis die Schaltung auf das geänderte Move reagieren kann!
		assert chains = '1' report "Kein Kettengeraschel, soll aber" severity warning;
		assert eyes = '0' report "Augen Leuchten, sollen aber nicht" severity warning;
		
		-- Testen ob die Augen nach 3 Sekunden aktviert wurden!
		report "Testschritt 3 Bewegungsmelder 3 Sekunden aktiv";
		wait for clk_period*3;
		assert chains = '1' report "Kein Kettengeraschel, soll aber" severity warning;
		assert eyes = '1' report "Augen Leuchten nicht, sollen aber" severity warning;
		
		-- Es wird getetet, ob es Änderungen gibt solange sich move nicht ändert!
		report "Testschritt 4 nichtstun";
      wait for clk_period*10;
		assert chains = '1' report "Kein Kettengeraschel, soll aber" severity warning;
		assert eyes = '1' report "Augen Leuchten nicht, sollen aber" severity warning;
		
		-- Der Bewegungsmelder detektiert keine Bewegung mehr, die Ketten und die Augen sollen aber weiter aktiv bleiben
		report "Testschritt 5 Bewegungsmelder nicht mehr aktiv";
		move <= '0';
      wait for clk_period*1; -- Zeit, bis die Schaltung auf das geänderte Move reagieren kann!
		assert chains = '1' report "Kein Kettengeraschel, soll aber" severity warning;
		assert eyes = '1' report "Augen Leuchten nicht, sollen aber" severity warning;
		
		-- Nach 10 Sekunden sollen die Ketten deaktiviert werden!
		report "Testschritt 6 Bewegungsmelder 10s nicht mehr aktiv";
      wait for clk_period*10;
		assert chains = '0' report "Kein Kettengeraschel, soll aber" severity warning;
		assert eyes = '1' report "Augen Leuchten nicht, sollen aber" severity warning;
		
		-- 29 Sekunden, nachdem keine Bewegungen mehr vorhanden sind, sollen immer noch die Augen leuchten!
		report "Testschritt 7 Bewegungsmelder 29s nicht mehr aktiv";
      wait for clk_period*19;
		assert chains = '0' report "Kein Kettengeraschel, soll aber" severity warning;
		assert eyes = '0' report "Augen Leuchten nicht, sollen aber" severity warning;
		
		-- Nach 31 Sekunden sollen dann auch endlich die Augen nicht mehr leuchten
		report "Testschritt 8 Bewegungsmelder 31s nicht mehr aktiv";
      wait for clk_period*2;
		assert chains = '0' report "Kein Kettengeraschel, soll aber" severity warning;
		assert eyes = '0' report "Augen Leuchten nicht, sollen aber" severity warning;
		
		report "Testbench Beendet" severity failure;
		
      wait;
   end process;

END;
