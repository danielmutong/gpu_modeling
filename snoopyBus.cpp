#include "snoopyBus.h"
enum s {M, S, I};
template <unsigned int NUM_CACHES>

void SnoopyBus<NUM_CACHES>::sm(string op)
{
    switch (state) 
    {
        case M:
            if (op == "PrRd" || op == "PrWr") 
            {
                state = M;
            }
            else if (op == "BusRd")
            {
                state = S;
            }
            else if (op == "BusRdX")
            {
                state = I;
            }
            break;
        case S:
            if (op == "PrRd")
            {
                state = S;
            }
            else if (op == "PrWr")
            {
                state = M;
            }
            else if (op == "BusRdX")
            {
                state = I;
            }
            break;
        case I:
            if (op == "PrRd" || op == "BusRd")
            {
                state = S;
            }
            else if (op == "PrWr" || op == "BusRdX")
            {
                state = M;
            }
            else if (op == "BusRdX" || op == "BusRsX")
            {
                state = I;
            }
            break;
    }
}