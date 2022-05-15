#include "ram.h"
#include "cache.h"

cacheset::cacheset()
{
    data = 0;
    tag = 0;
    dirty = 1;
}

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

void cacheset::set_data(uint d) 
{
    data = d;
} 

uint cacheset::get_data() 
{
    return data;
}

cacheblock::cacheblock()
{
    index = 0;
    lru = 0;
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

void cacheblock::print_cacheblock()
{
    cout << "index: " << index << " lru: " << lru << " ";
    cout << " c0: " ;
    cache_set[0].print_cacheset();
    cout << " c1: " ;
    cache_set[1].print_cacheset();  
    cout << endl;
}

uint cache::read_mem(uint raddr) 
{
    //return B.read_ram(raddr);
}

void cache::writethrough(uint waddr, uint wdata)
{
    //B.write_ram(waddr, wdata);
    tlm::tlm_generic_payload *trans = new tlm::tlm_generic_payload;
    sc_time delay = sc_time(10, SC_NS);
    trans->set_command(tlm::TLM_WRITE_COMMAND);
    trans->set_address(waddr);
    trans->set_data_ptr(reinterpret_cast<unsigned char *>(&wdata));
    trans->set_data_length(4);
    trans->set_streaming_width(4);
    trans->set_dmi_allowed(false);                            // Mandatory initial value
    trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value

    socket->b_transport(*trans, delay); // Blocking transport call
    if (trans->is_response_error())
        SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
    cout << "data: " << wdata << " written at: " << waddr << " at " << sc_time_stamp() << endl;
    wait(delay);
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
            mem[addr_index].cache_set[0].set_tag(addr_tag);
            mem[addr_index].cache_set[0].set_data(rdata);
            mem[addr_index].set_lru(1);
        }
        else 
        {
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
        writethrough(addr_tag, wdata);
    }
    else if (addr_tag == mem[addr_index].cache_set[1].get_tag())
    {
        mem[addr_index].set_lru(0);
        mem[addr_index].cache_set[1].set_data(wdata);
        writethrough(addr_tag, wdata);
    }
    else
    {
        cout << "cache write miss" << endl;
        uint garbage = read_mem(addr_tag);
        if (mem[addr_index].get_lru() == 0)
        {
            mem[addr_index].cache_set[0].set_tag(addr_tag);
            mem[addr_index].cache_set[0].set_data(wdata);
            mem[addr_index].set_lru(1);
            writethrough(addr_tag, wdata);
        }
        else
        {
            cout << "write miss, writing to ram" << endl;
            mem[addr_index].cache_set[1].set_tag(addr_tag);
            mem[addr_index].cache_set[1].set_data(wdata);
            mem[addr_index].set_lru(0);
            writethrough(addr_tag, wdata);
        }
    }
}

void cache::thread_process()
{
 
    cache_write(0b1010100000010, 999);
    print_cache();
    cache_write(0b1110100000010, 555);
    print_cache();
    cout << "\n \n";
}