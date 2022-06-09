#ifndef INITIATOR_H
#define INITIATOR_H

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"

#include "ram.h"
#include "cache.h"
#include "sc.h"

#include <assert.h>
extern struct Top top;

#define NUM_CORES 2

// Initiator module generating generic payload transactions

struct Initiator: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_initiator_socket<Initiator> socket;
  int i;
  SC_HAS_PROCESS(Initiator);

  Initiator(sc_module_name nm, int init) : socket(nm) // Construct and name socket
  {
    i = init;
    SC_THREAD(thread_process);
  }

  void thread_process()
  {
    if (i == 0) {
      unit_test0();
    }
    else{
      unit_test1();
    }
  }

  void unit_test0();
  void unit_test1();
    

  // Internal data buffer used by initiator with generic payload
  int data;
};

#endif
