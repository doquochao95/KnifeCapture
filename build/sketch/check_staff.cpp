#include "check_staff.h"

void rfid_checking()
{
    // checking rfid
    if (mfrc522_read_new_tag())
    {
        printf("Tag number: %s\r\n", (char*)TagNumber);
        //int LocalMachinesSize = machine_handle.machines.size();
        DynamicJsonDocument json_doc = DynamicJsonDocument(1024 * 1);
        memset(header.buf, 0, sizeof(header.buf));
        // request package
        //{PostId, KC_DeviceId, EOP, TagNumber}
        json_doc["PostId"] = millis();
        json_doc["KC_DeviceId"] = knife_capture.local_device_id;
        json_doc["EOP"] = "checking_staff";
        json_doc["TagNumber"] = (const char *)TagNumber;
        serializeJson(json_doc, header.buf);
        printf("Serialize local device data: success\r\n");
        sync_send("kc_api-staff", (char*)header.buf, HTTP_POST, 5000, "checking_staff", checking_staff_callback);
    }
}

//response type
// { request_id, message, pass, username, eop}
// checking staff callback function
void checking_staff_callback(JsonDocument &json_doc)
{
    printf ("checking_staff_callback\n");
    bool rfid_pass = json_doc.getMember("Pass").as<bool>();
    JsonParse_Element(json_doc, "Pass", rfid_pass);
    int request_id = 0;
    JsonParse_Element(json_doc, "Request_Id", request_id);
    String message = json_doc.getMember("Message").as<const char *>();
    String username = json_doc.getMember("UserName").as<const char *>();

    // true == pass
    if (rfid_pass)
    {
        knife_capture.checking_staff_flag = false;
        knife_capture.submit_recheck_flag = true;
        printf("checking tag return pass\r\n");
        // check the confirmaion
        if (ConfirmTimeOut_TimerHandle == NULL)
        {
            if (!sys_create_confirm_timeout_timer())
            {
                setStringProperty("SCREEN1", "t6.txt", "Error: Please press button again. Thank you");
            }
        }
        else
        {
            xTimerReset(ConfirmTimeOut_TimerHandle, (TickType_t)0);
        }

        nex_goto_page("KNIFE_PICKER");
        Output_Alarm();
        nex_send_message((char *)username.c_str()); // pick from server useless
        //
    }
    else
    {
        nex_goto_page("SCREEN1");
        Output_Alarm(2);
        nex_send_message((char *)message.c_str());
    }
}
