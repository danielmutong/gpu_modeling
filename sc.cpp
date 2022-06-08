#include "sc.h"
Top top("top");

int sc_main(int argc, char *argv[])
{
    Initiator *initiator[4];
    for (int i = 0; i < NUM_CORES; i++)
    {
        char txt[20];
        sprintf(txt, "socket_%d", i);
        initiator[i] = new Initiator("txt");
        initiator[i]->socket.bind(top.cache[i]->target_socket);
    }    
    

    sc_start();
    return 0;
}
