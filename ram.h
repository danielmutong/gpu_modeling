#ifndef __RAM__
#define __RAM__

#include <iostream>
#define SIZE_OF_RAM 32
using namespace std;

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

struct Memory : sc_module
{
    tlm_utils::simple_target_socket<Memory> socket;
    uint memory[SIZE_OF_RAM];
public:
    SC_CTOR(Memory)
        : socket("socket")
    {
        // Register callback for incoming b_transport interface method call
        socket.register_b_transport(this, &Memory::b_transport);

        // Initialize memory with random data
        for (int i = 0; i < SIZE_OF_RAM; i++)
        {
            memory[i] = 123;
        }
    }
    virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay) 
    {
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64 adr = trans.get_address();
        unsigned char *ptr = trans.get_data_ptr();
        unsigned int len = trans.get_data_length();
        unsigned char *byt = trans.get_byte_enable_ptr();
        unsigned int wid = trans.get_streaming_width();

        // Obliged to implement read and write commands
        if (cmd == tlm::TLM_READ_COMMAND)
        {
            *(uint *)ptr = read_ram(adr);
        }
        if (cmd == tlm::TLM_WRITE_COMMAND)
        {
            cout << "ram addr: " << adr << " data: " << *(uint *)ptr << endl;
            write_ram(adr, *(uint*)ptr);
        }

        // Obliged to set response status to indicate successful completion
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }
    void print_ram();
    void write_ram(uint, uint);
    uint read_ram(uint);

};

#endif