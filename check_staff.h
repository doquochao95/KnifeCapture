#ifndef check_staff_h
#define check_staff_h
#include "ethernet_sync_func.h"
#include "rfid_mfrc522.h"
#include "sys_config.h"
void rfid_checking();
//response type
// { request_id, message, pass, eop}
// checking staff callback function
void checking_staff_callback(JsonDocument &json_doc);
void rfid_checking();
#endif