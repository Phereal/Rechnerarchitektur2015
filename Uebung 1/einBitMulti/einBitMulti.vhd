library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity multi is
  port (
    in0 : in STD_LOGIC;
    in1 : in STD_LOGIC; 
    res : out STD_LOGIC);
end multi;

architecture multi_impl of multi is
  -- Signal temp1 dient dazu, dass res von Beginn an definiert ist!
  signal temp1 : STD_LOGIC := '0';

begin

  -- Alle Eingänge werden in der Sensibilitaetsliste mit aufgenommen,
  -- damit der Multiplexer sofort auf jede Eingangsaenderung reagiert.
  multiplier: process(in0, in1)
  begin
    -- Ein ein Bit Multiplizierer kann mit einem AND-Gatter der 
    -- beiden Eingänge abgebildet werden, da kein Carry erwartet wird!
    temp1 <= in0 AND in1;
  end process;
  
  -- Ergebnis des Prozesses sofort an die Ausgabe geben
  res <= temp1;

end multi_impl;


