#ifndef __SnoopyBus__
#define __SnoopyBus__

#include <iostream>
using namespace std;
//#define SIZE_OF_CACHE 2048
#define SIZE_OF_CACHE 32
#define CACHE_LINES (SIZE_OF_CACHE / 4 / 2)

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "extension.h"

template <unsigned int NUM_CACHES>
struct SnoopyBus : sc_module
{
public:
    tlm_utils::simple_initiator_socket<SnoopyBus> initiator_socket;
    tlm_utils::simple_target_socket<SnoopyBus>* target_socket[NUM_CACHES];

    SC_CTOR(SnoopyBus)
    {
        for (unsigned int i = 0; i < NUM_CACHES; i++)
        {
            char txt[20];
            sprintf(txt, "socket_%d", i);
            target_socket[i] = new tlm_utils::simple_target_socket<SnoopyBus>(txt);
        }

        for (int i = 0; i < NUM_CACHES; i++) 
        {
            target_socket[i]->register_b_transport(this, &SnoopyBus::b_transport);
        }
    }

    virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay)
    {
        cout << "blocking transport SnoopyBus" << endl;
        unsigned char *ptr = trans.get_data_ptr();
        tlm::tlm_command cmd = trans.get_command();
        uint addr = trans.get_address();
        cout << "new transaction received at snoopybus " << cmd << " addr: " << addr << endl;
        
        //get extension
        ID_extension *id_extension;
        trans.get_extension(id_extension);
        int x = id_extension->transaction_id;
        cout << "id extension " << id_extension->transaction_id << endl;
        initiator_socket->b_transport(trans, delay);
    }
};

#endif
