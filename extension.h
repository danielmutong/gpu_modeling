#ifndef __EXTENSION__
#define __EXTENSION__

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

struct ID_extension: tlm::tlm_extension<ID_extension>
{
    ID_extension():transaction_id(0){}

    virtual tlm_extension_base* clone() const {
        ID_extension * t = new ID_extension;
        t->transaction_id = this->transaction_id;
        return t;
    }
    virtual void copy_from(tlm_extension_base const & ext) {
        transaction_id = static_cast<ID_extension const &>(ext).transaction_id;
    }
    unsigned int transaction_id;
};

#endif