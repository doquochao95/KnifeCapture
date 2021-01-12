#ifndef ethernet_sync_func_h
#define ethernet_sync_func_h
#include "sys_config.h"

extern http_header_t header;
extern bool request_successed;
extern bool rfid_pass;
typedef void (*receive_cb_func)(JsonDocument &json_doc);

void sync_send(char *request_url, char *data, uint8_t method, int timeout,
               const char *receive_key_word, receive_cb_func cb_func);
void sync_received(int timeout, const char *receive_key_word, receive_cb_func cb_func);
#endif
