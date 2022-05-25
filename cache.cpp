#define DEBUG 1

#include "ram.h"
#include "cache.h"
sc_time delay = sc_time(10, SC_NS);
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
void cacheset::set_dirty(uint d)
{
    dirty = d;
}

uint cacheset::get_dirty()
{
    return dirty;
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
    lru = 1;
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

uint Cache::read_mem(uint raddr, tlm::tlm_generic_payload &trans)
{
    if (DEBUG) cout << "cache::readmem begin" << endl;

    initiator_socket->b_transport(trans, delay); // Blocking transport call
    wait(delay);
    unsigned char *ptr = trans.get_data_ptr();
    uint rdata = *(uint *)ptr;
    cout << "data: " << rdata << " read at: " << raddr << " at " << sc_time_stamp() << endl;

    if (DEBUG) cout << "cache::readmem end" << endl;
    return rdata;
}

void Cache::writethrough(uint waddr, uint wdata, tlm::tlm_generic_payload &trans2)
{

    tlm::tlm_generic_payload * trans = new tlm::tlm_generic_payload;
    // Generate a random sequence of reads and writes

    tlm::tlm_command cmd = tlm::TLM_WRITE_COMMAND;
    trans->set_command(cmd);
    trans->set_address(waddr);
    trans->set_data_ptr(reinterpret_cast<unsigned char *>(&wdata));
    trans->set_data_length(4);
    trans->set_streaming_width(4);                            // = data_length to indicate no streaming
    trans->set_byte_enable_ptr(0);                            // 0 indicates unused
    trans->set_dmi_allowed(false);                            // Mandatory initial value
    trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value
    initiator_socket->b_transport(*trans, delay);
    cout << "data: " << wdata << " written at: " << waddr << " at " << sc_time_stamp() << endl;
    wait(delay);
}

void Cache::print_cache()
{
    for (int i = 0; i < CACHE_LINES; i++)
    {
        mem[i].print_cacheblock();
    }
}

uint Cache::cache_read(uint addr, tlm::tlm_generic_payload &trans)
{
    if (DEBUG)
        cout << "cache::cache_read begin" << endl;

    uint addr_index = addr & 0xFF;
    uint addr_tag = (addr >> 8);

    cout << "index " << addr_index << " addr_tag: " << addr_tag << endl;

    if (addr_tag == mem[addr_index].cache_set[0].get_tag()) 
    {
        mem[addr_index].set_lru(1);

        if (DEBUG)
            cout << "cache::cache_read end" << endl;
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        print_cache();
        return mem[addr_index].cache_set[0].get_data();
    }
    else if (addr_tag == mem[addr_index].cache_set[1].get_tag()) 
    {
        mem[addr_index].set_lru(0);

        if (DEBUG)
            cout << "cache::cache_read end" << endl;
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        print_cache();
        return mem[addr_index].cache_set[1].get_data();
    }
    else
    {
        uint rdata = read_mem(addr_tag, trans);
        if (mem[addr_index].get_lru() == 0)
        {
            if (mem[addr_index].cache_set[0].get_dirty() == 1)
            {
                cout << "replacing data dirty, write back" << endl;
                uint addr_tag = mem[addr_index].cache_set[0].get_tag();
                uint addr = addr_index + (addr_tag << 8);
                writethrough(addr, mem[addr_index].cache_set[0].get_data(), trans);
                mem[addr_index].cache_set[0].set_dirty(0);
            }
            mem[addr_index].cache_set[0].set_tag(addr_tag);
            mem[addr_index].cache_set[0].set_data(rdata);
            mem[addr_index].set_lru(1);
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
        }
        else
        {
            if (mem[addr_index].cache_set[1].get_dirty() == 1)
            {
                cout << "replacing data dirty, write back" << endl;
                uint addr_tag = mem[addr_index].cache_set[1].get_tag();
                uint addr = addr_index + (addr_tag << 8);
                writethrough(addr, mem[addr_index].cache_set[1].get_data(), trans);
                mem[addr_index].cache_set[1].set_dirty(0);
            }
            mem[addr_index].cache_set[1].set_tag(addr_tag);
            mem[addr_index].cache_set[1].set_data(rdata);
            mem[addr_index].set_lru(0);
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
        }

        if (DEBUG)
            cout << "cache::cache_read begin" << endl;
        print_cache();
        return rdata;
    }
}

void Cache::cache_write(uint addr, uint wdata, tlm::tlm_generic_payload &trans)
{
    uint addr_index = addr & 0xFF;
    uint addr_tag = (addr >> 8);
    cout << "index " << addr_index << " addr_tag: " << addr_tag << endl;

    if (addr_tag == mem[addr_index].cache_set[0].get_tag())
    {
        mem[addr_index].set_lru(1);
        mem[addr_index].cache_set[0].set_data(wdata);
        mem[addr_index].cache_set[0].set_dirty(1);
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        print_cache();
    }
    else if (addr_tag == mem[addr_index].cache_set[1].get_tag())
    {
        mem[addr_index].set_lru(0);
        mem[addr_index].cache_set[1].set_data(wdata);
        mem[addr_index].cache_set[1].set_dirty(1);
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        print_cache();
    }
    else
    {
        cout << "cache write miss" << endl;
        if (mem[addr_index].get_lru() == 0)
        {
            if (mem[addr_index].cache_set[0].get_dirty() == 1)
            {
                cout << "replacing data dirty, write back" << endl;
                uint addr_tag = mem[addr_index].cache_set[0].get_tag();
                uint addr = addr_index + (addr_tag << 8);
                writethrough(addr, mem[addr_index].cache_set[0].get_data(), trans);
            }
            mem[addr_index].cache_set[0].set_tag(addr_tag);
            mem[addr_index].cache_set[0].set_data(wdata);
            mem[addr_index].cache_set[0].set_dirty(1);
            mem[addr_index].set_lru(1);
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
        }
        else
        {
            if (mem[addr_index].cache_set[1].get_dirty() == 1)
            {
                cout << "replacing data dirty, write back" << endl;
                uint addr_tag = mem[addr_index].cache_set[1].get_tag();
                uint addr = addr_index + (addr_tag << 8);
                writethrough(addr, mem[addr_index].cache_set[1].get_data(), trans);
            }
            mem[addr_index].cache_set[1].set_tag(addr_tag);
            mem[addr_index].cache_set[1].set_data(wdata);
            mem[addr_index].cache_set[1].set_dirty(1);
            mem[addr_index].set_lru(0);
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
        }
        print_cache();
    }
}

uint Cache::get_value(uint i0, uint i1) 
{
    return mem[i0].cache_set[i1].get_data();
}

