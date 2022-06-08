#ifndef __SC__
#define __SC__

#include <systemc.h>
#include <iostream>
using namespace std;

#include "ram.h"
#include "cache.h"
#include "snoopyBus.h"
#include "initiator.h"

#define NUM_CORES 2

SC_MODULE(Top)
{
    Cache *cache[NUM_CORES];
    Memory *memory;
    SnoopyBus<NUM_CORES> *snoopyBus;
    

    SC_CTOR(Top)
    {
        // Instantiate components
        memory = new Memory("memory");

        snoopyBus = new SnoopyBus<NUM_CORES>("snoopybus");
        for( int i = 0; i < NUM_CORES; i++) 
        {
            char txt[20];
            sprintf(txt, "socket_%d", i);
            cache[i] = new Cache("txt");
        }

        // One initiator is bound directly to one target with no intervening bus

        // Bind initiator socket to target socket
        //cache->initiator_socket.bind(memory->socket);
        for (int i = 0; i < NUM_CORES; i++)
        {
            cache[i]->initiator_socket.bind(*(snoopyBus->target_socket[i]));
        }
        snoopyBus->initiator_socket.bind(memory->socket);
    }
};

#endif