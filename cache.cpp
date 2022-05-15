#include "ram.h"
#include "cache.h"
extern ram B;
void cacheset::print_cacheset()
{
    cout << " tag: " << tag << " data: " << data << " dirty " << dirty << " " ;
}
uint cacheset::get_tag()
{
    return tag;
}

void cacheset::set_tag(uint t)
{
    tag = t;
}

void cacheset::set_dirty(uint d)
{
    dirty = d;
}

uint cacheset::get_dirty()
{
    return dirty;
}
void cacheset::set_data(uint d) 
{
    data = d;
} 
uint cacheset::get_data() 
{
    return data;
}
cacheset::cacheset()
{
    data = 0;
    tag = 0;
    dirty = 0;
}

void cacheblock::set_index(int i)
{
    index = i;
}

void cacheblock::set_lru(uint l)
{
    lru = l;
}

uint cacheblock::get_lru()
{
    return lru;
}
cacheblock::cacheblock()
{
    index = 0;
    lru = 0;
}

void cacheblock::print_cacheblock()
{
    cout << "index: " << index << " lru: " << lru << " ";
    cout << " c0: " ;
    cache_set[0].print_cacheset();
    cout << " c1: " ;
    cache_set[1].print_cacheset();  
    cout << endl;
}



cache::cache()
{
    for (int i = 0; i < CACHE_LINES; i++) 
    {
        mem[i].set_index(i);
        mem[i].cache_set[0].set_tag(i);
        mem[i].cache_set[0].set_data(i * 10);
        mem[i].cache_set[1].set_tag(2*i);
        mem[i].cache_set[1].set_data(2 * i * 10);
    }
}
uint cache::read_mem(uint raddr) 
{
    return B.read_ram(raddr);
}

void cache::writethrough(uint waddr, uint wdata) 
{
    B.write_ram(waddr, wdata);
}
void cache::print_cache()
{
    for (int i = 0; i < CACHE_LINES; i++)
    {
        mem[i].print_cacheblock();
    }
}

uint cache::cache_read(uint addr)
{
    uint addr_index = addr & 0xFF;
    uint addr_tag = (addr >> 8);

    cout << "index " << addr_index << " addr_tag: " << addr_tag << endl;

    if (addr_tag == mem[addr_index].cache_set[0].get_tag()) 
    {
        mem[addr_index].set_lru(1);
        return mem[addr_index].cache_set[0].get_data();
    }
    else if (addr_tag == mem[addr_index].cache_set[1].get_tag()) 
    {
        mem[addr_index].set_lru(0);
        return mem[addr_index].cache_set[1].get_data();
    }
    else
    {
        uint rdata = read_mem(addr_tag);
        if (mem[addr_index].get_lru() == 0) 
        {
            if (mem[addr_index].cache_set[0].get_dirty() == 1)
            {
                cout << "replacing data dirty, write back" << endl;
                writethrough(mem[addr_index].cache_set[0].get_tag(), mem[addr_index].cache_set[0].get_data());
                mem[addr_index].cache_set[0].set_dirty(0);
            }
            mem[addr_index].cache_set[0].set_tag(addr_tag);
            mem[addr_index].cache_set[0].set_data(rdata);
            mem[addr_index].set_lru(1);
        }
        else 
        {
            if (mem[addr_index].cache_set[1].get_dirty() == 1)
            {
                cout << "replacing data dirty, write back" << endl;
                writethrough(mem[addr_index].cache_set[1].get_tag(), mem[addr_index].cache_set[1].get_data());
                mem[addr_index].cache_set[1].set_dirty(0);
            }
            mem[addr_index].cache_set[1].set_tag(addr_tag);
            mem[addr_index].cache_set[1].set_data(rdata);
            mem[addr_index].set_lru(0);
        }
        return rdata;
    }
}

void cache::cache_write(uint addr, uint wdata) 
{
    uint addr_index = addr & 0xFF;
    uint addr_tag = (addr >> 8);
    cout << "index " << addr_index << " addr_tag: " << addr_tag << endl;

    if (addr_tag == mem[addr_index].cache_set[0].get_tag())
    {
        mem[addr_index].set_lru(1);
        mem[addr_index].cache_set[0].set_data(wdata);
        mem[addr_index].cache_set[0].set_dirty(1);
        //writethrough(addr_tag, wdata);
    }
    else if (addr_tag == mem[addr_index].cache_set[1].get_tag())
    {
        mem[addr_index].set_lru(0);
        mem[addr_index].cache_set[1].set_data(wdata);
        mem[addr_index].cache_set[1].set_dirty(1);
        //writethrough(addr_tag, wdata);
    }
    else
    {
        cout << "cache write miss" << endl;
        uint garbage = read_mem(addr_tag);
        if (mem[addr_index].get_lru() == 0)
        {
            if (mem[addr_index].cache_set[0].get_dirty() == 1) 
            {
                cout << "replacing data dirty, write back" << endl;
                writethrough(mem[addr_index].cache_set[0].get_tag(), mem[addr_index].cache_set[0].get_data());
            }
            mem[addr_index].cache_set[0].set_tag(addr_tag);
            mem[addr_index].cache_set[0].set_data(wdata);
            mem[addr_index].cache_set[0].set_dirty(1);
            mem[addr_index].set_lru(1);
            
        }
        else
        {
            if (mem[addr_index].cache_set[1].get_dirty() == 1)
            {
                cout << "replacing data dirty, write back" << endl;
                writethrough(mem[addr_index].cache_set[1].get_tag(), mem[addr_index].cache_set[1].get_data());
            }
            mem[addr_index].cache_set[1].set_tag(addr_tag);
            mem[addr_index].cache_set[1].set_data(wdata);
            mem[addr_index].cache_set[1].set_dirty(1);
            mem[addr_index].set_lru(0);
        }
    }
}

