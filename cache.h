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

struct Initiator : sc_module
{
    cacheblock mem[CACHE_LINES];

public:
    tlm_utils::simple_initiator_socket<Initiator> socket;

    SC_CTOR(Initiator)
        : socket("socket") // Construct and name socket
    {
        for (int i = 0; i < CACHE_LINES; i++)
        {
            mem[i].set_index(i);
            mem[i].cache_set[0].set_tag(i);
            mem[i].cache_set[0].set_data(i * 10);
            mem[i].cache_set[1].set_tag(2 * i);
            mem[i].cache_set[1].set_data(2 * i * 10);
        }
        SC_THREAD(thread_process);
    }
    void thread_process();
    void print_cache();
    uint cache_read(uint);
    void cache_write(uint, uint);
    void writethrough(uint, uint);
    uint read_mem(uint);
};

#endif