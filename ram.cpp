#include "ram.h"

ram::ram()
{
    for (int i = 0; i < SIZE_OF_RAM; i++) 
    {
        memory[i] = 123;
    }
}

void ram::print_ram() 
{
    for (int i = 0; i < SIZE_OF_RAM; i++)
    {
        cout << i << " " << memory[i] << endl;
    }
}

void ram::write_ram(uint addr, uint wdata)
{
    memory[addr] = wdata;
}

uint ram::read_ram(uint raddr)
{   
    return memory[raddr];
}
