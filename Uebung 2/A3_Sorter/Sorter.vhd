library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.ALL;
use ieee.std_logic_unsigned.ALL;

entity Sorter is
    Port ( clk : in  STD_LOGIC;
           addr_start : in  STD_LOGIC_VECTOR (7 downto 0);
           addr_end : in  STD_LOGIC_VECTOR (7 downto 0);
           start : in  STD_LOGIC;
           done : out  STD_LOGIC);
end Sorter;

architecture Behavioral of Sorter is

component CachedMemory
   Port (
    clk     : in  STD_LOGIC;
    init    : in  STD_LOGIC;
    dump    : in  STD_LOGIC;
    reset   : in  STD_LOGIC;
    re      : in  STD_LOGIC;
    we      : in  STD_LOGIC;
    addr    : in  STD_LOGIC_VECTOR (7 downto 0);
    data_in : in  STD_LOGIC_VECTOR (7 downto 0);
    output  : out  STD_LOGIC_VECTOR (7 downto 0);
    ack     : out  STD_LOGIC
   );
end component;
-- Component Signals
signal mem_init     : std_logic := '0';
signal mem_dump     : std_logic := '0';
signal mem_reset    : std_logic := '0';
signal mem_re       : std_logic := '0';
signal mem_we       : std_logic := '0';
signal mem_addr     : std_logic_vector(7 downto 0) := (others => '0');
signal mem_data_in  : std_logic_vector(7 downto 0) := (others => '0');
signal mem_output   : std_logic_vector(7 downto 0);
signal mem_ack      : std_logic := '0';

-- State definition
type sortState is (notStarted, 
							isStarted, 
							readLeft, 
							counter0,
							leftReaden, 
							readRight,
							counter1,
							rightReaden,
							areReaden, 
							mustBeChanged, 
							changeLeft, 
							counter2,
							leftChanged,
							changeRight, 
							counter3,
							rightChanged,
							wereChanged, 
							isFinished
							);
signal state : sortState:= notStarted;

--Intern Signals
signal lvalue : std_logic_vector(7 downto 0) := (others => '0');
signal rvalue : std_logic_vector(7 downto 0) := (others => '0');
signal ladd: integer := 0;
signal tmpvalue: std_logic_vector(7 downto 0) := (others => '0');
signal ischanged: std_logic := '0';
signal counter: integer := 0;
signal resetdone: std_logic := '0';

begin
mem: CachedMemory PORT MAP (
    clk     => clk,
    init    => mem_init,
    dump    => mem_dump,
    reset   => mem_reset,
    re      => mem_re,
    we      => mem_we,
    addr    => mem_addr,
    data_in => mem_data_in,
    output  => mem_output,
    ack     => mem_ack
);

execute: process (clk)   

variable startedCount : integer := 0;

begin
if rising_edge(clk) then
	if(resetdone = '0') then
		mem_reset <= '1';
		resetdone <= '1';
	end if;
	-- notStarted
	if(state = notStarted AND resetdone = '1') then
	mem_reset <= '0';
		if(start = '1') then
			startedCount := 0;
			done <= '0';
			ladd <= to_integer(unsigned(addr_start));
			mem_init <= '1';
			state <= isStarted;
			end if;
	end if;
	-- ##################
	-- isStarted
	if(state = isStarted) then
		startedCount := startedCount + 1;
		if(startedCount >= 20) then
			report "isStarted";
			mem_init <= '0';
			state <= readLeft;  
			end if;
		end if;
	-- ##################
	-- readLeft
	if(state = readLeft) then
		report "readLeft";
		mem_re <= '1';
		mem_addr <= std_logic_vector(to_unsigned(ladd, mem_addr'length));
		state <= counter0;
	end if;
	-- ##################
	-- counter0
	if(state = counter0) then
		if(counter < 34) then
			counter <= counter + 1;
		elsif(counter = 34) then
			report "counter0";
			counter <= 0;
			state <= leftReaden;
		end if;
	end if;
	-- ##################
	-- leftReaden
	if(state = leftReaden AND mem_ack = '1') then
		report "leftReaden";
		mem_re <= '0';
		lvalue <= mem_output;
		state <= readRight;
	end if;
	-- ##################
	-- readRight
	if(state = readRight) then
		report "readRight";
		mem_re <= '1';
		mem_addr <= std_logic_vector(to_unsigned((ladd + 1), mem_addr'length));
		state <= counter1;
	end if;
	-- ##################
	-- counter1
	if(state = counter1) then
		if(counter < 34) then
			counter <= counter + 1;
		elsif(counter = 34) then
			report "counter1";
			counter <= 0;
			state <= rightReaden;
		end if;
	end if;
	-- ##################
	-- rightReaden
	if(state = rightReaden AND mem_ack = '1') then
		report "rightReaden";
		mem_re <= '0';
		rvalue <= mem_output;
		state <= areReaden;
	end if;
	-- ##################
	-- areReaden
	if(state = areReaden) then
		report "areReaden";
		if(to_integer(unsigned(lvalue)) > to_integer(unsigned(rvalue))) then
			state <= mustBeChanged;
		else
			state <= isFinished;
		end if;
	end if;
	-- ##################
	--  mustBeChanged
	if(state = mustBeChanged) then
		report "mustBeChanged";
		ischanged <= '1';
		state <= changeLeft;
	end if;
	-- ##################
	-- changeLeft
	if(state = changeLeft) then
		report "changeLeft";
		mem_we <= '1';
		tmpvalue <= rvalue;
		mem_data_in <= lvalue;
		mem_addr <= std_logic_vector(to_unsigned((ladd + 1), mem_addr'length));
		state <= counter2;
	end if;
	-- ##################
	-- counter2
	if(state = counter2) then
		if(counter < 34) then
			counter <= counter + 1;
		elsif(counter = 34) then
			report "counter2";
			counter <= 0;
			state <= leftChanged;
		end if;
	end if;
	-- ##################
	-- leftChanged
	if(state = leftChanged) then
		report "leftChanged";
		mem_we <= '0';
		state <= changeRight;
	end if;
	-- ##################
	-- changeRight
	if(state = changeRight) then
		report "changeRight";
		mem_we <= '1';
		mem_data_in <= tmpvalue;
		mem_addr <= std_logic_vector(to_unsigned(ladd, mem_addr'length));
		state <= counter3;
	end if;
	-- ##################
	-- counter2
	if(state = counter3) then
		if(counter < 34) then
			counter <= counter + 1;
		elsif(counter = 34) then
			report "counter3";
			counter <= 0;
			state <= rightChanged;
		end if;
	end if;
	-- ##################
	-- rightChanged
	if(state = rightChanged) then
		report "rightChanged";
		mem_we <= '0';
		state <= wereChanged;	
	end if;
	-- ##################
	-- wereChanged
	if(state = wereChanged) then
		report "wereChanged";
		if((ladd + 1) < to_integer(unsigned(addr_end))) then
			report "wereChanged 0";
			ladd <= ladd + 1;
			state <= isStarted;
		elsif((ladd + 1) = to_integer(unsigned(addr_end))) then
			report "wereChanged 1";
			ladd <= to_integer(unsigned(addr_start));
			ischanged <= '0';
			state <= isStarted;
		end if;
	end if;
	-- ##################
	-- isFinished
	if(state = isFinished) then
		report "isFinished";
		if((ladd + 1) < to_integer(unsigned(addr_end))) then
			report "isFinished 0";
			ladd <= ladd + 1;
			state <= isStarted;
		elsif((ladd + 1) = to_integer(unsigned(addr_end)) AND ischanged = '0') then
			report "isFinished 1";
			mem_re <= '0';
			mem_we <= '0';
			mem_init <= '0';
			mem_reset <= '0';
			mem_dump <= '1';
			done <= '1';
		end if;
	end if;
	-- ##################
end if;
end process;
end Behavioral;