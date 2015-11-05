library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity four_b_multi is
  port (
    a0 : in STD_LOGIC;
    a1 : in STD_LOGIC; 
    a2 : in STD_LOGIC; 
    a3 : in STD_LOGIC; 
    b0 : in STD_LOGIC;
    b1 : in STD_LOGIC; 
    b2 : in STD_LOGIC; 
    b3 : in STD_LOGIC; 
    out0 : out STD_LOGIC;
    out1 : out STD_LOGIC;
    out2 : out STD_LOGIC;
    out3 : out STD_LOGIC;
    out4 : out STD_LOGIC;
    out5 : out STD_LOGIC;
    out6 : out STD_LOGIC;
    out7 : out STD_LOGIC
    );
end four_b_multi;

architecture four_b_multi_impl of four_b_multi is
  component eight_bit_adder
    Port ( 
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
      c_out : out  STD_LOGIC);
  end component;


  -- Zusammenfassen der einzelnen Eingangsleitungen für b(0-3) zu einem Bitvektor
  -- Vector wurde verwendet um das Programm uebersichtlicher zu halten und den 
  -- Zugriff auf b0 - b3 zu vereinfachen
  signal b: STD_LOGIC_VECTOR(3 downto 0);
  -- Zur Zwischenspeicherung der carry bits
  signal zero: STD_LOGIC_VECTOR(2 downto 0);

  -- Hilfssignale um das die Ergebnisse der 1 bit multi. und des Shiften zu sichern
  -- Es wurden acht bit verwendet um das Programm uebersichtlicher zu halten.
  signal tmp0 : STD_LOGIC_VECTOR(7 downto 0) := "00000000";
  signal tmp1 : STD_LOGIC_VECTOR(7 downto 0) := "00000000";
  signal tmp2 : STD_LOGIC_VECTOR(7 downto 0) := "00000000";
  signal tmp3 : STD_LOGIC_VECTOR(7 downto 0) := "00000000";

  -- Temporaere Variablen für das aufsummieren der einzelnen Multiplikationen
  -- Es wurden acht bit verwendet um das Programm uebersichtlicher zu halten.
  signal tmp4 : STD_LOGIC_VECTOR(7 downto 0) := "00000000";
  signal tmp5 : STD_LOGIC_VECTOR(7 downto 0) := "00000000";
  signal tmp6 : STD_LOGIC_VECTOR(7 downto 0) := "00000000";

begin
  -- Eingangsleitungen b0,b1,b2,b3 in den Eingangsbitvektor b schreiben.
  b(0) <= b0;
  b(1) <= b1;
  b(2) <= b2;
  b(3) <= b3;
  --Zuweisungen für zerodefinitionen!
  zero(0) <= '0';
  zero(1) <= '0';
  zero(2) <= '0';

  multiply : process (b, a0, a1, a2, a3)
  begin
    if(a0 = '1') then
      tmp0 <= '0' & '0' & '0' & '0' & b;
    else
      tmp0 <= "00000000";
    end if;

    if(a1 = '1') then
      tmp1 <= '0' & '0' & '0' & b & '0';
    else
      tmp1 <= "00000000";
    end if;

    if(a2 = '1') then
      tmp2 <= '0' & '0' & b & '0' & '0';
    else
      tmp2 <= "00000000";
    end if;

    if(a3 = '1') then
      tmp3 <= '0' & b & '0' & '0' & '0';
    else
      tmp3 <= "00000000";
    end if;
  end process;

  -- Addition der zuvor bestimmten Zwischenergebnisse (Ergebnis in tmp6)
  -- Verwendung von 8-bit Addierern um das Shiften einfacher zu handhaben
  eba0: eight_bit_adder PORT MAP(
      tmp0(0),tmp0(1),tmp0(2),tmp0(3),tmp0(4),tmp0(5),tmp0(6),tmp0(7),
      tmp1(0),tmp1(1),tmp1(2),tmp1(3),tmp1(4),tmp1(5),tmp1(6),tmp1(7),
      tmp4(0),tmp4(1),tmp4(2),tmp4(3),tmp4(4),tmp4(5),tmp4(6),tmp4(7),
      zero(0));
  eba1: eight_bit_adder PORT MAP(
      tmp2(0),tmp2(1),tmp2(2),tmp2(3),tmp2(4),tmp2(5),tmp2(6),tmp2(7),
      tmp3(0),tmp3(1),tmp3(2),tmp3(3),tmp3(4),tmp3(5),tmp3(6),tmp3(7),
      tmp5(0),tmp5(1),tmp5(2),tmp5(3),tmp5(4),tmp5(5),tmp5(6),tmp5(7),
      zero(1));
  eba2: eight_bit_adder PORT MAP(
      tmp4(0),tmp4(1),tmp4(2),tmp4(3),tmp4(4),tmp4(5),tmp4(6),tmp4(7),
      tmp5(0),tmp5(1),tmp5(2),tmp5(3),tmp5(4),tmp5(5),tmp5(6),tmp5(7),
      tmp6(0),tmp6(1),tmp6(2),tmp6(3),tmp6(4),tmp6(5),tmp6(6),tmp6(7),
      zero(2));
      
  -- Das Ergebnis tmp6 wird an den Ausgang out(0-7) gelegt!
  out0 <= tmp6(0);
  out1 <= tmp6(1);
  out2 <= tmp6(2);
  out3 <= tmp6(3);
  out4 <= tmp6(4);
  out5 <= tmp6(5);
  out6 <= tmp6(6);
  out7 <= tmp6(7);


end four_b_multi_impl;
