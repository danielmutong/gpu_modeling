#include "ram.h"

void Memory::print_ram() 
{
    cout << "printing ram" << endl;
    for (int i = 0; i < SIZE_OF_RAM; i++)
    {
        cout << i << ": " << memory[i] << endl;
    }
}

void Memory::write_ram(uint addr, uint wdata)
{
    cout << "writing ram " << endl;
    memory[addr] = wdata;
}

uint Memory::read_ram(uint raddr)
{   
    cout << "reading ram" << endl;
    return memory[raddr];
}

uint Memory::get_value(uint i)
{
    return memory[i];
}
