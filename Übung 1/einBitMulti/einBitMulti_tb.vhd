LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
ENTITY multi_tb IS
END multi_tb;
 
ARCHITECTURE behavior OF multi_tb IS 
 
  -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT multi
    PORT(
         in0 : IN  std_logic;
         in1 : IN  std_logic;
         res : OUT  std_logic
        );
    END COMPONENT;

   --Inputs
   signal in0 : std_logic := '0';
   signal in1 : std_logic := '0';

   --Outputs
   signal res : std_logic;
 
BEGIN
 
  -- Instantiate the Unit Under Test (UUT)
   uut: multi PORT MAP (
          in0 => in0,
          in1 => in1,
          res => res
        );

   -- Stimulus process
   stim_proc: process
  
  -- Es wird eine Wahrheitstabelle fuer die einzelnen Testfaelle verwendet,
  -- um den eigentlichen Testcode simpel zu halten (eine Schleife).
  
  -- Erstellen des Datentypen 'Wahrheitstabelle'
  type wahrheitstabelle is record
  -- inputs 
  in0, in1 : std_logic;
  -- and exprected outputs
  res : std_logic;
  end record;
  
  -- Erstellen der Wahrheitstabelle als Array
  type pattern_array is array (natural range <>) of wahrheitstabelle;
  constant patterns : pattern_array :=
  (('0','0','0'),
  ('0','1','0'),
  ('1','0','0'),
  ('1','1','1'));
  
  begin
    -- init prozesse abwarten
    wait for 100 ns; 
    report "Beginn der Testbench";

    -- Scheife zum durchgehen der Wahrheitstabelle 
    for i in patterns'range loop
      report "Testschritt: " & integer'image(i);
      
      in0 <= patterns(i).in0;
      in1 <= patterns(i).in1;
      
      wait for 2 ns;
      
      -- Errormeldung, wenn eine erwartete Ausgabe nicht der realen entspricht!
      -- Abbruch wenn ein Fehler auftritt, wodurch ein solcher leichter entdeckt werden kann
      assert (res = patterns(i).res) report "Falscher Wert am Ausgang!" severity error;
      
    end loop;

    -- Ausgabe damit erkennbar wird, dass der Test erfolgreich durchlief
    report "Ende der Testbench" severity failure;
    wait;
  end process;

END;
