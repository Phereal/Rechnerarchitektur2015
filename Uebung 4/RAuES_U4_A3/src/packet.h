#include <iostream>
#include <systemc.h>
#include <utility>

using namespace std;


//Tutoriums-Datei dist.ccp entnommen
struct packet
    {
    int addr;
    int payload;
    bool flag;

    // must implement '=' '==' and '<<' (nice would be 'sc_trace')

    packet(int a, int p, bool f) :
	addr(a), payload(p), flag(f)
	{
	}

    packet() :
	addr(0), payload(0), flag(false)
	{
	}

    inline bool operator==(const packet &rhs) const
	{
	return (rhs.addr == addr && rhs.payload == payload && rhs.flag == flag);
	}

    inline packet &operator=(const packet &rhs)
	{
	addr = rhs.addr;
	payload = rhs.payload;
	flag = rhs.flag;
	return *this;
	}

    inline friend void sc_trace(sc_trace_file *tf, const packet &v,
	    const std::string &NAME)
	{
	sc_trace(tf, v.addr, NAME + ".addr");
	sc_trace(tf, v.payload, NAME + ".payload");
	sc_trace(tf, v.flag, NAME + ".flag");
	}

    inline friend ostream &operator<<(ostream &os, packet const &v)
	{
	os << "(" << v.addr << "," << v.payload << "," << v.flag << ")";
	return os;
	}
    };
