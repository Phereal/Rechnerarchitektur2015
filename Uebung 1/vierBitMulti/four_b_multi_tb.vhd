LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;
 
ENTITY four_b_multi_tb IS
END four_b_multi_tb;
 
ARCHITECTURE behavior OF four_b_multi_tb IS 
 
  -- Component Declaration for the Unit Under Test (UUT)
 
  COMPONENT four_b_multi
  PORT(
    a0 : IN  std_logic;
    a1 : IN  std_logic;
    a2 : IN  std_logic;
    a3 : IN  std_logic;
    b0 : IN  std_logic;
    b1 : IN  std_logic;
    b2 : IN  std_logic;
    b3 : IN  std_logic;
    out0 : OUT  std_logic;
    out1 : OUT  std_logic;
    out2 : OUT  std_logic;
    out3 : OUT  std_logic;
    out4 : OUT  std_logic;
    out5 : OUT  std_logic;
    out6 : OUT  std_logic;
    out7 : OUT  std_logic
    );
  END COMPONENT;

  --Inputs
  signal a0 : std_logic := '0';
  signal a1 : std_logic := '0';
  signal a2 : std_logic := '0';
  signal a3 : std_logic := '0';
  signal b0 : std_logic := '0';
  signal b1 : std_logic := '0';
  signal b2 : std_logic := '0';
  signal b3 : std_logic := '0';

  --Outputs
  signal out0 : std_logic;
  signal out1 : std_logic;
  signal out2 : std_logic;
  signal out3 : std_logic;
  signal out4 : std_logic;
  signal out5 : std_logic;
  signal out6 : std_logic;
  signal out7 : std_logic;
  
  -- Zeitkonstante damit immer eine feste Zeit zwischen
  -- den Testschritten gewartet wird
  constant clk_i_period : time := 10 ns;
 
BEGIN
 
  -- Instantiate the Unit Under Test (UUT)
  uut: four_b_multi PORT MAP (
    a0 => a0,
    a1 => a1,
    a2 => a2,
    a3 => a3,
    b0 => b0,
    b1 => b1,
    b2 => b2,
    b3 => b3,
    out0 => out0,
    out1 => out1,
    out2 => out2,
    out3 => out3,
    out4 => out4,
    out5 => out5,
    out6 => out6,
    out7 => out7
  ); 

  -- Stimulus process
  stim_proc: process
  
  -- All input combination of a0-a3 and b0-b3 are tested.
  -- To reduce the size of this testbench the inputs will
  -- be generated via two nested for loops.
  -- Since the manual creation of a truth table with 2^8 
  -- entries is prone to errors, the expected result will
  -- be calculated via the ieee library, which functions
  -- have high credit.
  
  -- schleifen zaehler
  variable i : integer := 0;
  variable j : integer := 0;
  
  -- Vectors are used instead of single bits to clear up the code
  variable a : std_logic_vector(3 downto 0) := "0000";
  variable b : std_logic_vector(3 downto 0) := "0000";
  variable o : std_logic_vector(7 downto 0) := "00000000";
  
  begin
    report "Start testbench!";
  
      -- hold reset state for 100 ns (settle system).
      wait for 100 ns;

    -- check for all multiplication options
    -- 0 to 15 for 4-bit inputs
    for i in 0 to 15 loop
      for j in 0 to 15 loop
        -- assign inputs
        -- Inputs and expected result are reported to give the tester a chance
        -- to see where the test may have failed and that the testbench really
        -- has done something.
        report "Testing: " & integer'image(i) & " * " & integer'image(j) & " = " & integer'image(i * j);
        a := std_logic_vector(to_unsigned(i, a'length));
        b := std_logic_vector(to_unsigned(j, b'length));
        a0 <= a(0);  a1 <= a(1);  a2 <= a(2);  a3 <= a(3);
        b0 <= b(0);  b1 <= b(1);  b2 <= b(2);  b3 <= b(3);
        
        -- wait long enough for out0 to out7 to be processed
        wait for clk_i_period;
        
        -- Check the result
        -- Assign single bits to the vector for easier checking via ieee convert functions
        -- Output of the real result if an error occured to be able to compare it with the
        -- expected one
        o := (out7 & out6 & out5 & out4 & out3 & out2 & out1 & out0);
        assert(to_integer(unsigned(o)) = (i * j))
          report "Failure with result: " & integer'image(to_integer(unsigned(o)))
          severity failure;
          
      end loop;
    end loop;
    
    -- Report to show that the testbench ended successful
    report "End of testbench" severity failure;

    wait;
  end process;

END;
