#include <systemc.h>
#include <iostream>
using namespace std;

#include "ram.h"
#include "cache.h"

SC_MODULE(Top)
{
    cache *initiator;
    Memory *memory;

    SC_CTOR(Top)
    {
        // Instantiate components
        initiator = new cache("initiator");
        memory = new Memory("memory");

        // One initiator is bound directly to one target with no intervening bus

        // Bind initiator socket to target socket
        initiator->socket.bind(memory->socket);
    }
};

int sc_main(int argc, char *argv[])
{
    Top top("top");
    sc_start();
    return 0;
}
