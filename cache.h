#ifndef __CACHE__
#define __CACHE__

#include <iostream>
#include <string>
using namespace std;
//#define SIZE_OF_CACHE 2048
#define SIZE_OF_CACHE 32
#define CACHE_LINES (SIZE_OF_CACHE / 4 / 2)

class cacheset
{
    uint data;
    uint tag;
    uint dirty;
    string status;

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

class cache
{
    cacheblock mem[CACHE_LINES];

public:
    cache();
    void print_cache();
    uint cache_read(uint);
    void cache_write(uint, uint);
    void writethrough(uint, uint);
    uint read_mem(uint);
};

#endif