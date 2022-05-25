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
// Initiator module generating generic payload transactions
struct Initiator: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_initiator_socket<Initiator> socket;

  SC_CTOR(Initiator)
  : socket("socket")  // Construct and name socket
  {
    SC_THREAD(thread_process);
  }

  void thread_process()
  {
    unit_test();
  }

  void unit_test();
    

  // Internal data buffer used by initiator with generic payload
  int data;
};

#endif
