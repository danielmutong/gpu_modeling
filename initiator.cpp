#include "initiator.h"
/*
void Initiator::unit_test0()
{
     
     // TLM-2 generic payload transaction, reused across calls to b_transport
     tlm::tlm_generic_payload *trans = new tlm::tlm_generic_payload;
     sc_time delay = sc_time(10, SC_NS);

     // Generate a random sequence of reads and writes
     //---------------------------------------------------
     //cache miss

     tlm::tlm_command cmd = tlm::TLM_READ_COMMAND;

     uint address = 0b1010100000010;
     trans->set_command(cmd);
     trans->set_address(address);
     trans->set_data_ptr(reinterpret_cast<unsigned char *>(&data));
     trans->set_data_length(4);
     trans->set_streaming_width(4);                            // = data_length to indicate no streaming
     trans->set_byte_enable_ptr(0);                            // 0 indicates unused
     trans->set_dmi_allowed(false);                            // Mandatory initial value
     trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value
     socket->b_transport(*trans, delay);                       // Blocking transport call
                                                                 // Initiator obliged to check response status and delay
     if (trans->is_response_error())
          SC_REPORT_ERROR("TLM-2", "Response error from b_transport");

     cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << address
          << " } , data = " << data << " at time " << sc_time_stamp()
          << " delay = " << delay << endl;

     wait(delay);
     //assert(data == 123);
     //assert(top.memory->get_value(20) == 20);
     //assert(top.cache->get_value(2, 1) == 123);
     
     //-------------------------------------------------------------
     //cache hit
     address = 0b1000000010;
     trans->set_command(cmd);
     trans->set_address(address);
     trans->set_data_ptr(reinterpret_cast<unsigned char *>(&data));
     trans->set_data_length(4);
     trans->set_streaming_width(4);                            // = data_length to indicate no streaming
     trans->set_byte_enable_ptr(0);                            // 0 indicates unused
     trans->set_dmi_allowed(false);                            // Mandatory initial value
     trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value
     socket->b_transport(*trans, delay);                       // Blocking transport call
                                                                 // Initiator obliged to check response status and delay
     if (trans->is_response_error())
          SC_REPORT_ERROR("TLM-2", "Response error from b_transport");

     cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << address
          << " } , data = " << data << " at time " << sc_time_stamp()
          << " delay = " << delay << endl;

     // Realize the delay annotated onto the transport call

     wait(delay);
     //assert(data == 2);
     //assert(top.memory->get_value(20) == 20);
     //assert(top.cache->get_value(2, 1) == 123);

     
     //-------------------------------------------------------------------
     //cache hit
     cmd = tlm::TLM_WRITE_COMMAND;
     data = 999;
     address = 0b1100000011;
     trans->set_command(cmd);
     trans->set_address(address);
     trans->set_data_ptr(reinterpret_cast<unsigned char *>(&data));
     trans->set_data_length(4);
     trans->set_streaming_width(4);                            // = data_length to indicate no streaming
     trans->set_byte_enable_ptr(0);                            // 0 indicates unused
     trans->set_dmi_allowed(false);                            // Mandatory initial value
     trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value
     socket->b_transport(*trans, delay);                       // Blocking transport call
                                                                 // Initiator obliged to check response status and delay
     if (trans->is_response_error())
          SC_REPORT_ERROR("TLM-2", "Response error from b_transport");

     cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << address
          << " } , data = " << data << " at time " << sc_time_stamp()
          << " delay = " << delay << endl;
     wait(delay);
     assert(top.cache->get_value(3, 0) == 999);
     
     //-------------------------------------------------------------------
     //cache miss
     cmd = tlm::TLM_WRITE_COMMAND;
     data = 999;
     address = 0b11100000011;
     trans->set_command(cmd);
     trans->set_address(address);
     trans->set_data_ptr(reinterpret_cast<unsigned char *>(&data));
     trans->set_data_length(4);
     trans->set_streaming_width(4);                            // = data_length to indicate no streaming
     trans->set_byte_enable_ptr(0);                            // 0 indicates unused
     trans->set_dmi_allowed(false);                            // Mandatory initial value
     trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value
     socket->b_transport(*trans, delay);                       // Blocking transport call
                                                                 // Initiator obliged to check response status and delay
     if (trans->is_response_error())
          SC_REPORT_ERROR("TLM-2", "Response error from b_transport");

     cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << address
          << " } , data = " << data << " at time " << sc_time_stamp()
          << " delay = " << delay << endl;
     wait(delay);
     assert(top.cache->get_value(3, 1) == 999);
     assert(top.memory->get_value(30) == 30);
     
}
*/
void Initiator::unit_test0()
{
     
     // TLM-2 generic payload transaction, reused across calls to b_transport
     tlm::tlm_generic_payload *trans = new tlm::tlm_generic_payload;
     sc_time delay = sc_time(10, SC_NS);

     // Generate a random sequence of reads and writes
     //---------------------------------------------------
     //cache miss

     tlm::tlm_command cmd = tlm::TLM_READ_COMMAND;

     uint address = 0b1000000010;
     trans->set_command(cmd);
     trans->set_address(address);
     trans->set_data_ptr(reinterpret_cast<unsigned char *>(&data));
     trans->set_data_length(4);
     trans->set_streaming_width(4);                            // = data_length to indicate no streaming
     trans->set_byte_enable_ptr(0);                            // 0 indicates unused
     trans->set_dmi_allowed(false);                            // Mandatory initial value
     trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value
     socket->b_transport(*trans, delay);                       // Blocking transport call
                                                                 // Initiator obliged to check response status and delay
     if (trans->is_response_error())
          SC_REPORT_ERROR("TLM-2", "Response error from b_transport");

     cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << address
          << " } , data = " << data << " at time " << sc_time_stamp()
          << " delay = " << delay << endl;

     wait(delay);
}

void Initiator::unit_test1()
{
     wait(100,SC_NS);
     // TLM-2 generic payload transaction, reused across calls to b_transport
     tlm::tlm_generic_payload *trans = new tlm::tlm_generic_payload;
     sc_time delay = sc_time(10, SC_NS);

     // Generate a random sequence of reads and writes
     //---------------------------------------------------
     // cache miss

     tlm::tlm_command cmd = tlm::TLM_READ_COMMAND;

     uint address = 0b1010100000011;
     trans->set_command(cmd);
     trans->set_address(address);
     trans->set_data_ptr(reinterpret_cast<unsigned char *>(&data));
     trans->set_data_length(4);
     trans->set_streaming_width(4);                            // = data_length to indicate no streaming
     trans->set_byte_enable_ptr(0);                            // 0 indicates unused
     trans->set_dmi_allowed(false);                            // Mandatory initial value
     trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value
     socket->b_transport(*trans, delay);                       // Blocking transport call
                                                               // Initiator obliged to check response status and delay
     if (trans->is_response_error())
          SC_REPORT_ERROR("TLM-2", "Response error from b_transport");

     cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << address
          << " } , data = " << data << " at time " << sc_time_stamp()
          << " delay = " << delay << endl;

     wait(delay);
}