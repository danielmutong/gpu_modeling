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

struct SnoopyBus : sc_module
{
public:
    tlm_utils::simple_initiator_socket<SnoopyBus> initiator_socket;
    tlm_utils::simple_target_socket<SnoopyBus> target_socket;

    SC_CTOR(SnoopyBus)
        : target_socket("socket") // Construct and name socket
    {
        target_socket.register_b_transport(this, &SnoopyBus::b_transport);
        cout << "snoopybus ctor called" << endl;
    }

    virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay)
    {
        cout << "blocking transport SnoopyBus" << endl;
        unsigned char *ptr = trans.get_data_ptr();
        tlm::tlm_command cmd = trans.get_command();
        uint addr = trans.get_address();
        cout << "new transaction received at snoopybus " << cmd << " addr: " << addr << endl;
        initiator_socket->b_transport(trans, delay);
    }
};

#endif
