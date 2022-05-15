#include "ram.h"

void Memory::print_ram() 
{
    for (int i = 0; i < SIZE_OF_RAM; i++)
    {
        cout << i << " " << memory[i] << endl;
    }
}

void Memory::write_ram(uint addr, uint wdata)
{
    cout << "writing ram " << endl;
    memory[addr] = wdata;
    print_ram();
}

uint Memory::read_ram(uint raddr)
{   
    return memory[raddr];
}

