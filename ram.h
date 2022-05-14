#ifndef __RAM__
#define __RAM__

#include <iostream>
#define SIZE_OF_RAM 32
using namespace std;
class ram
{
    uint memory[SIZE_OF_RAM];
public:
    ram();
    void print_ram();
    void write_ram(uint, uint);
    uint read_ram(uint);

};

#endif