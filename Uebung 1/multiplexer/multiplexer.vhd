library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
 
entity multiplexer is
  Port (  in0 : in    STD_LOGIC;
          in1 : in    STD_LOGIC;
          sel : in    STD_LOGIC;
          res : out   STD_LOGIC);
end multiplexer;

architecture multiplexer_impl of multiplexer is
  -- Signal temp1 dient dazu, dass res von Beginn an definiert ist!
  signal temp1 : STD_LOGIC := '0';

  begin
    
    -- Alle Eingänge werden in der Sensibilitaetsliste mit aufgenommen,
    -- damit der Multiplexer sofort auf jede Eingangsaenderung reagiert.
    multiplexer: process(sel, in0, in1)
    begin
		
    -- Wenn sel = 0 dann leite in0 auf temp1 um.
    if (sel = '0') then 
      temp1 <= in0;
    -- Sonst leite in1 auf temp1 um.
    else
      temp1 <= in1;
    end if;
    
	end process;
  
  -- Ergebnis des Prozesses an die Ausgabe geben
	res <= temp1;

end multiplexer_impl;

