LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
ENTITY eight_bit_adder_tb IS
END eight_bit_adder_tb;
 
ARCHITECTURE behavior OF eight_bit_adder_tb IS 
 
  -- Component Declaration for the Unit Under Test (UUT)
  COMPONENT eight_bit_adder
    Port( 
      a0 : in  STD_LOGIC;
      a1 : in  STD_LOGIC;
      a2 : in  STD_LOGIC;
      a3 : in  STD_LOGIC;
      a4 : in  STD_LOGIC;
      a5 : in  STD_LOGIC;
      a6 : in  STD_LOGIC;
      a7 : in  STD_LOGIC;
      b0 : in  STD_LOGIC;
      b1 : in  STD_LOGIC;
      b2 : in  STD_LOGIC;
      b3 : in  STD_LOGIC;
      b4 : in  STD_LOGIC;
      b5 : in  STD_LOGIC;
      b6 : in  STD_LOGIC;
      b7 : in  STD_LOGIC;
      s0 : out  STD_LOGIC;
      s1 : out  STD_LOGIC;
      s2 : out  STD_LOGIC;
      s3 : out  STD_LOGIC;
      s4 : out  STD_LOGIC;
      s5 : out  STD_LOGIC;
      s6 : out  STD_LOGIC;
      s7 : out  STD_LOGIC;
      c_out : out  STD_LOGIC
    );
  END COMPONENT;
    
   --Inputs
  signal a0 : std_logic := '0';
  signal a1 : std_logic := '0';
  signal a2 : std_logic := '0';
  signal a3 : std_logic := '0';
  signal a4 : std_logic := '0';
  signal a5 : std_logic := '0';
  signal a6 : std_logic := '0';
  signal a7 : std_logic := '0';
  signal b0 : std_logic := '0';
  signal b1 : std_logic := '0';
  signal b2 : std_logic := '0';
  signal b3 : std_logic := '0';
  signal b4 : std_logic := '0';
  signal b5 : std_logic := '0';
  signal b6 : std_logic := '0';
  signal b7 : std_logic := '0';

   --Outputs
  signal s0 : std_logic;
  signal s1 : std_logic;
  signal s2 : std_logic;
  signal s3 : std_logic;
  signal s4 : std_logic;
  signal s5 : std_logic;
  signal s6 : std_logic;
  signal s7 : std_logic;
  signal c_out : std_logic;

BEGIN
 
  -- Instantiate the Unit Under Test (UUT)
  uut: eight_bit_adder PORT MAP (
    a0 => a0,
    a1 => a1,
    a2 => a2,
    a3 => a3,
    a4 => a4,
    a5 => a5,
    a6 => a6,
    a7 => a7,
    b0 => b0,
    b1 => b1,
    b2 => b2,
    b3 => b3,
    b4 => b4,
    b5 => b5,
    b6 => b6,
    b7 => b7,
    s0 => s0,
    s1 => s1,
    s2 => s2,
    s3 => s3,
    s4 => s4,
    s5 => s5,
    s6 => s6,
    s7 => s7,
    c_out => c_out
  );

  -- Stimulus process
  stim_proc: process
  
  type pattern_type is record
    --inputs
    a0,a1,a2,a3,a4,a5,a6,a7,b0,b1,b2,b3,b4,b5,b6,b7: std_logic;
    -- and expected outputs
    s0,s1,s2,s3,s4,s5,s6,s7, c_out : std_logic;
  end record;
      
  -- Erstellen der Wahrheitstabelle als Array
  -- Die gewaehlte Wahrheitstabelle deckt Grenzwerte ab.
  -- Ein ausfuehrlicherer Test wurde nicht gewaehlt, da
  -- dieses Modul nur im Zusammenhang mit dem four_b_multi
  -- verwendet wird und dieser durch eine eigene TB getestet
  -- wird. Die notwendige funktion dieses Moduls wird somit durch den
  -- Test des uebergeordneten Moduls gewaehrleistet.
  type pattern_array is array (natural range <>) of pattern_type;
  constant patterns : pattern_array := (
      ( '0','0','0','0','0','0','0','0',
        '0','0','0','0','0','0','0','0',
        '0','0','0','0','0','0','0','0',
        '0'),
      ( '1','1','1','1','1','1','1','1',
        '0','0','0','0','0','0','0','0',
        '1','1','1','1','1','1','1','1',
        '0'),
      ( '0','0','0','0','0','0','0','0',
        '1','1','1','1','1','1','1','1',
        '1','1','1','1','1','1','1','1',
        '0'),
      ( '0','1','0','1','0','1','0','1',
        '1','0','1','0','1','0','1','0',
        '1','1','1','1','1','1','1','1',
        '0'),
      ( '1','0','0','0','0','0','0','0',
        '1','1','1','1','1','1','1','1',
        '0','0','0','0','0','0','0','0',
        '1') );

  begin
    -- hold reset state for 100 ns (settle system).
    wait for 100 ns;

    report "Beginn der Testbench";
    
    -- Scheife zum durchgehen der Wahrheitstabelle 
    for i in patterns'range loop
      report "Testschritt: " & integer'image(i);

      -- Eingangswerte zuweisen
      a0 <= patterns(i).a0;
      a1 <= patterns(i).a1;
      a2 <= patterns(i).a2;
      a3 <= patterns(i).a3;
      a4 <= patterns(i).a4;
      a5 <= patterns(i).a5;
      a6 <= patterns(i).a6;
      a7 <= patterns(i).a7;
      b0 <= patterns(i).b0;
      b1 <= patterns(i).b1;
      b2 <= patterns(i).b2;
      b3 <= patterns(i).b3;
      b4 <= patterns(i).b4;
      b5 <= patterns(i).b5;
      b6 <= patterns(i).b6;
      b7 <= patterns(i).b7;
      
      -- sicherstellen das quantum abgelaufen ist und
      -- der output den neuen Wert angenommen hat
      wait for 2 ns;
      
      -- Fehlermeldung, wenn eine erwartete Ausgabe nicht der realen entspricht!
      -- Abbruch wenn ein Fehler auftritt, wodurch ein solcher leichter entdeckt werden kann
      assert (s0 = patterns(i).s0) report "Wrong sum value[s0]" severity error;
      assert (s1 = patterns(i).s1) report "Wrong sum value[s1]" severity error;
      assert (s2 = patterns(i).s2) report "Wrong sum value[s2]" severity error;
      assert (s3 = patterns(i).s3) report "Wrong sum value[s3]" severity error;
      assert (s4 = patterns(i).s4) report "Wrong sum value[s4]" severity error;
      assert (s5 = patterns(i).s5) report "Wrong sum value[s5]" severity error;
      assert (s6 = patterns(i).s6) report "Wrong sum value[s6]" severity error;
      assert (s7 = patterns(i).s7) report "Wrong sum value[s7]" severity error;
      assert (c_out = patterns(i).c_out) report "Wrong carry value"
        severity error;
      
    end loop;
    
    -- Report to show that the testbench ended successful
    assert false report "Ende der Testbench!" severity failure;
    wait;
  end process;

END;