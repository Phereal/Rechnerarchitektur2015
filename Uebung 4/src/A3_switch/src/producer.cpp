#include "packet.h"
#include <systemc.h>

SC_MODULE (producer) {
    sc_in_clk     clock ;      // Clock input of the design
    sc_out<packet> out;		//gesendete pakete

    //------------Local Variables Here---------------------
    sc_uint<4>	count;
  } // End of function incr_count
