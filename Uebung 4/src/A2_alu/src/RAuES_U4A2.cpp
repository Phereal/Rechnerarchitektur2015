#include <iostream>
#include <systemc.h>

struct fib_config {
    int n, f1, f2;

    fib_config(int n, int f1, int f2) : n(n), f1(f1), f2(f2) { }

    fib_config() : n(0), f1(0), f2(0) { }

    inline bool operator==(const fib_config &rhs) const {
        return (rhs.n == n && rhs.f1 == f2 && rhs.f2 == f2);
    }

    inline fib_config &operator=(const fib_config &rhs) {
        n = rhs.n;
        f1 = rhs.f1;
        f2 = rhs.f2;
        return *this;
    }

    inline friend void sc_trace(sc_trace_file *tf, const fib_config &v,
                                const std::string &NAME) {
        sc_trace(tf, v.n, NAME + ".n");
        sc_trace(tf, v.f1, NAME + ".f1");
        sc_trace(tf, v.f2, NAME + ".f2");
    }

    inline friend ostream &operator<<(ostream &os, fib_config const &v) {
        os << "(" << v.n << "," << v.f1 << "," << v.f2 << ")";
        return os;
    }
};


SC_MODULE(fibonacci) {

    sc_in<fib_config> in;
    sc_out<int> res;


    void do_it() {
        int n = in.read().n;
        int f1 = in.read().f1;
        int f2 = in.read().f2;
        int tmp = 0;

        // wir gehen davon aus, dass n >= 1 gilt, sonst wird einfach f2 ausgegeben
        switch (n) {
            case 1:
                res = f1;
                break;
            case 2:
                res = f2;
                break;
            default:
                for (int i = 0; i < n - 2; ++i) {
                    tmp = f2;
                    f2 = f1 + f2;
                    f1 = tmp;
                }
                res = f2;
        }


    }

    SC_CTOR(fibonacci) {
        SC_METHOD(do_it);
        sensitive << in;

    }
};

SC_MODULE(stimulator) {

    sc_out<fib_config> config;

    void send_value() {


        // werte für die klassische Fibonacci-Folge
        // https://de.wikipedia.org/wiki/Fibonacci-Folge#Definition_der_Fibonacci-Folge
        config = fib_config(4, 1, 1);
        wait(5, SC_NS);

        config = fib_config(24, 1, 1);
        wait(5, SC_NS);

        // verallgemeinerte Folge
        // https://de.wikipedia.org/wiki/Fibonacci-Folge#Verallgemeinerungen
        config = fib_config(1, 2, 3);
        wait(5, SC_NS);


        config = fib_config(7, 7, 3);
        wait(5, SC_NS);
        config = fib_config(8, 7, 3);
        wait(5, SC_NS);




    }

    SC_CTOR(stimulator) {
        SC_THREAD(send_value);
        // keine sensitivity, da es einfach nur einmal beim Start des
        // Simulations-Kernels loslegen soll
    }
};

SC_MODULE(monitor) {

    sc_in<int> n;

    void receive() {
        cout << "received: " << n << "\n";
    }

    SC_CTOR(monitor) {
        SC_METHOD(receive);
        sensitive << n;
    }
};


int sc_main(int, char *[]) {

    monitor mon("Monitor");
    stimulator stim("Stimulator");
    fibonacci fib("Fibonacci");

    sc_signal<fib_config> fc;
    sc_signal<int> number;

    stim.config(fc);
    fib.in(fc);

    fib.res(number);
    mon.n(number);

    sc_start();


}
