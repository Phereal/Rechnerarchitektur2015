library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity eight_bit_adder is
  Port ( a0 : in  STD_LOGIC;
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
    c_out : out  STD_LOGIC);
end eight_bit_adder;

-- Der 8-Bit Adder verwendet den im studip bereitgestellten full\_adder

architecture Behavioral of eight_bit_adder is
  component full_adder
      Port ( a : in  STD_LOGIC;
           b : in  STD_LOGIC;
           c_in : in  STD_LOGIC;
           s : out  STD_LOGIC;
           c_out : out  STD_LOGIC);
  end component;
  
  signal zero : STD_LOGIC := '0';
  signal tmp0,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6 : STD_LOGIC;
begin

  va0: full_adder PORT MAP(a => a0, b => b0, c_in => zero,s => s0,
        c_out => tmp0);
  va1: full_adder PORT MAP(a1,b1,tmp0,s1,tmp1);
  va2: full_adder PORT MAP(a2,b2,tmp1,s2,tmp2);
  va3: full_adder PORT MAP(a3,b3,tmp2,s3,tmp3);
  va4: full_adder PORT MAP(a4,b4,tmp3,s4,tmp4);
  va5: full_adder PORT MAP(a5,b5,tmp4,s5,tmp5);
  va6: full_adder PORT MAP(a6,b6,tmp5,s6,tmp6);
  va7: full_adder PORT MAP(a7,b7,tmp6,s7,c_out);

end Behavioral;

