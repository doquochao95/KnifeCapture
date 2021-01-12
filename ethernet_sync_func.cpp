#include "ethernet_sync_func.h"

http_header_t header;
bool request_successed = false;
bool rfid_pass = false;
bool retry_request = false;
int retry_counter = 0;
void sync_send(char *request_url, char *data, uint8_t method, int timeout, const char *receive_key_word, receive_cb_func cb_func)
{
    printf("sync_send\n");
    request_successed = false;
    retry_request = false;
    retry_counter = 0;
    char temp_data[512] = {0};
    memccpy(temp_data, data, 0, sizeof(temp_data));

    do
    {
        request_successed = false;
        if (knife_capture.ethernet_handle.start_connect_to_server(3000))
        {
            knife_capture.ethernet_handle.ethernet_pr.ethernet_send_request(knife_capture.ethernet_handle.client,
                                                                            request_url, temp_data, method);
            sync_received(timeout, receive_key_word, cb_func);
        }
        retry_counter++;
    } while (!request_successed && (retry_counter < 5));
}

//
void sync_received(int timeout, const char *key_word, receive_cb_func cb_func)
{
    printf("sync_received\n");
    int time = timeout;
    while (time-- > 0 || knife_capture.ethernet_handle.client.available() > 0)
    {
        int receivedBytes = knife_capture.ethernet_handle.client.available();

        delay(1);
        if (receivedBytes == 0)
            continue;

        memset(header.buf, 0, sizeof(header.buf));
        knife_capture.ethernet_handle.client.readBytesUntil('\r\n', header.buf, receivedBytes);
        printf("New data received: %s\r\n", header.buf);

        if (strstr(header.buf, "200 OK: ok") != nullptr)
        {
            printf("response: Success\n\n");
            request_successed = true;
        }
        if (strstr(header.buf, "Access denied") != nullptr)
        {
            printf("response: Access denied\n\n");

            knife_capture.sys_requesting = false;

            if (knife_capture.knife_capture_submit)
                knife_capture.knife_capture_submit = false;

            request_successed = false;
            retry_request = true;
            break;
        }
        if (strstr(header.buf, "404 Not Found") != nullptr)
        {
            printf("response: Not Found\n\n");
            request_successed = false;
            knife_capture.sys_requesting = false;

            if (knife_capture.knife_capture_submit)
                knife_capture.knife_capture_submit = false;

            break;
        }
        if (strstr(header.buf, "400 Bad Request") != nullptr)
        {
            printf("response: Bad Request\n\n");
            request_successed = false;
            knife_capture.sys_requesting = false;

            if (knife_capture.knife_capture_submit)
                knife_capture.knife_capture_submit = false;
            break;
        }

        if (strstr(header.buf, "{") != nullptr)
        {
            printf("new data is comming\r\n");
            DynamicJsonDocument json_doc = DynamicJsonDocument(1024 * 2);
            DeserializationError JsonErr = deserializeJson(json_doc, header.buf);
            printf("parse json object code: %s\r\n", JsonErr.c_str());

            if (!JsonErr)
            {
                printf("Serialize object");
                String eop = json_doc.getMember("EOP").as<const char *>();
                char mesg[100]{0};
                String CurrentTime = "";

                // checking rfid function
                if (eop == key_word)
                {
                    if (cb_func != NULL)
                        cb_func(json_doc);

                    request_successed = true;
                }
            }
        }
    }
}
