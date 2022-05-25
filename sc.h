#ifndef __SC__
#define __SC__

#include <systemc.h>
#include <iostream>
using namespace std;

#include "ram.h"
#include "cache.h"
#include "snoopyBus.h"

SC_MODULE(Top)
{
    Cache *cache;
    Memory *memory;
    SnoopyBus *snoopyBus;

    SC_CTOR(Top)
    {
        // Instantiate components
        cache = new Cache("cache");
        memory = new Memory("memory");
        snoopyBus = new SnoopyBus("snoopyBus");
        
        

        // One initiator is bound directly to one target with no intervening bus

        // Bind initiator socket to target socket
        //cache->initiator_socket.bind(memory->socket);
        cache->initiator_socket.bind(snoopyBus->target_socket);
        snoopyBus->initiator_socket.bind(memory->socket);
    }
};

#endif