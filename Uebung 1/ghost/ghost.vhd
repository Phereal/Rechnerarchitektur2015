library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity ghost is
  port (
  -- Die Uhr, jede Sekunde gibt es eine steigende Flanke
  clk : in STD_LOGIC;
  -- Der Bewegunsmelder
  move : in STD_LOGIC; 
  -- Die Lampen fuer die Augen
  eyes : out STD_LOGIC; 
  -- Der Lautsprecher fuer das Gerassel der Ketten
  chains : out STD_LOGIC);
end ghost;

architecture ghost_impl of ghost is

-- Lokale Variablen, die abgefragt werden koennen. Die geschalteten Ausgaenge ergeben zusammen einen Zustand,
-- dieser Zustand kann anhand der folgenden Variablen abgefragt werden!
signal speaker : std_logic := '0';
signal bulb : std_logic := '0';
-- Die Counter, die nach aktivieren von Move und deaktivieren von Move anfangen zu zaehlen.
signal counterON : Integer := 0;
signal counterOFF : Integer := 0;

begin

Zustandsuebergangsschaltnetz: process(clk)
begin
-- Wir reagieren nur auf die steigende Taktflanke
if rising_edge(clk) then
	
	if(move = '1') then -- Der Bewegungsmelder detektiert eine Bewegung!
		counterON <= counterON + 1;
		-- Wenn der Bewegungsmelder aktiviert ist und sowohl speaker als auch bulb deaktiviert sind
		-- muss der Counter auf 1 gesetzt werden und der speaker soll aktiviert werden.
		if(speaker = '0' AND bulb = '0') then 
			counterON <= 1;
			speaker <= '1';
		end if;
		
		-- Sobald der speaker aktiviert ist soll es laut aufgabenstellung 3 sekunden dauern, bis die bulb aktiviert wird.
		if(speaker = '1' AND bulb = '0') then
			if(counterON = 3) then
				bulb <= '1';
			end if;
		end if;
	end if;
	
	if(move = '0') then-- Es kann keine Bewegung mehr festgestellt werden!
		counterOFF <= counterOFF + 1;
		-- Wenn der speaker und die bulb noch angeschaltet sind, soll nach 10 Sekunden der speaker augeschaltet werden.
		if(speaker = '1' AND bulb = '1') then
			if (counterOFF = 10) then
				speaker <= '0';
			end if;
		end if;
		-- Wenn der Speaker bereits ausgeschaltet ist, dauert es weitere 20 Sekunden, bis die bulb deaktiviert wird!
		if(speaker = '0' AND bulb = '1') then
			if(counterOFF = 30) then
				bulb <= '0';
			end if;
		end if;
		-- Wenn keine Bewegung detektiert wird und beide Ausgaenge deaktiviert sind, wird der counter dauerhaft resetet!
		if(speaker = '0' AND bulb = '0') then
			counterOFF <= 0;
		end if;
	end if;
end if;

end process;

Ausgabeschaltnetz: process(bulb, speaker)
begin
	chains <= speaker;
	eyes <= bulb;
end process;

end ghost_impl;