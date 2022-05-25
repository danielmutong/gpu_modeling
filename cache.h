#ifndef __CACHE__
#define __CACHE__

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

class cacheset
{
    uint data;
    uint tag;
    uint dirty;

public:
    cacheset();
    void print_cacheset();
    uint get_tag();
    void set_tag(uint);
    uint get_data();
    void set_data(uint);
    void set_dirty(uint);
    uint get_dirty();
};
class cacheblock
{
    uint index;
    uint lru;

public:
    cacheset cache_set[2];
    cacheblock();
    void set_index(int);
    void print_cacheblock();
    void set_lru(uint);
    uint get_lru();
};

struct Cache : sc_module
{
    cacheblock mem[CACHE_LINES];

public:
    tlm_utils::simple_initiator_socket<Cache> initiator_socket;
    tlm_utils::simple_target_socket<Cache> target_socket;

    SC_CTOR(Cache)
        : target_socket("socket") // Construct and name socket
    {
        target_socket.register_b_transport(this, &Cache::b_transport);
        for (int i = 0; i < CACHE_LINES; i++){
            mem[i].set_index(i);
        }
        for (int i = 0; i < CACHE_LINES; i++)
        {
            mem[i].cache_set[0].set_tag(i);
            mem[i].cache_set[0].set_data(i);
            mem[i].cache_set[0].set_dirty(1);
            mem[i].cache_set[1].set_tag(i * 10);
            mem[i].cache_set[1].set_data(i * 10);
            mem[i].cache_set[1].set_dirty(1);
        }
        print_cache();
    }
    virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay)
    {
        cout << "new transaction received at cache" << endl;
        unsigned char *ptr = trans.get_data_ptr();
        tlm::tlm_command cmd = trans.get_command();
        if (cmd == tlm::TLM_READ_COMMAND) {
            *(uint *)ptr = cache_read(trans.get_address(), trans);
        }
        else {
            cache_write(trans.get_address(), *(uint *)ptr, trans);
        }
        cout << "time cache process done: " << sc_time_stamp() << endl;
    }
    void print_cache();
    uint cache_read(uint, tlm::tlm_generic_payload &trans);
    void cache_write(uint, uint, tlm::tlm_generic_payload &trans);
    void writethrough(uint, uint, tlm::tlm_generic_payload &trans);
    uint read_mem(uint, tlm::tlm_generic_payload &trans);
    uint get_value(uint index0, uint index1);
};

#endif