#include "sc.h"
#include "initiator.h"
Top top("top");
int sc_main(int argc, char *argv[])
{
    Initiator * initiator = new Initiator("initator");
    
    initiator->socket.bind(top.cache->target_socket);

    sc_start();
    return 0;
}
