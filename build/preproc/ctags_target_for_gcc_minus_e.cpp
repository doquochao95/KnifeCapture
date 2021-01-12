# 1 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
// Creator: khale
// Date:    16/2/2020 , 19:37:20
// Author: Techteam
# 5 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 2
# 6 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 2
# 7 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 2

inline void user_handler_task_callback_func();
inline void ethernet_handler_task_callback_fnc();
inline void main_task_callback_func();

void main_task(void *pr);
void ethernet_handler_task(void *pr);
void user_handler_task(void *pr);

int CirclePostTime_Ticks = 0;
int EthernetMaintain_Ticks = 0;
bool EthernetMaintainNow = false;
bool DevicePostNow = false;
/******************************************************************************************/
void runtime_timer(TimerHandle_t pxTimer)
{
    pinMode(LED_BUILTIN /* backward compatibility*/, !digitalRead(LED_BUILTIN /* backward compatibility*/));
    CirclePostTime_Ticks++;
    EthernetMaintain_Ticks++;
    if (EthernetMaintain_Ticks >= 60 * 30)
    {
        EthernetMaintainNow = true;
        EthernetMaintain_Ticks = 0;
    }
    if (CirclePostTime_Ticks >= 60 * 5)
    {
        DevicePostNow = true;
        CirclePostTime_Ticks = 0;
    }
}
/******************************************************************************************/

void setup()
{
    // getting things ready first
    start_up();

    TimerHandle_t Runtime_timer = xTimerCreate("runtime_timer", (1000), ( ( BaseType_t ) 1 ), 
# 44 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                                                                               __null
# 44 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                                                                                   , runtime_timer);

    if (Runtime_timer == 
# 46 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                        __null
# 46 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                            )
    {
    }
    else
    {
        if (xTimerGenericCommand( ( Runtime_timer ), ( ( BaseType_t ) 1 ), ( xTaskGetTickCount() ), 
# 51 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
           __null
# 51 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
           , ( 0 ) ) != ( ( ( BaseType_t ) 1 ) ))
        {
            printf("Can not start request timer\r\n");
        }
        else
            printf("start request timer successfully\r\n");
    }

    sys_create_request_timeout_timer();
    sys_create_confirm_timeout_timer();

    xSemaphore = xQueueCreateCountingSemaphore( ( 3 ), ( 3 ) );

    if (xSemaphore != 
# 64 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                     __null
# 64 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                         )
    {
        printf("The semaphore was created successfully\r\n"); // \r: về đầu hàng, \n: xuống dòng
        if (xQueueGenericSend( ( QueueHandle_t ) ( xSemaphore ), 
# 67 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
           __null
# 67 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
           , ( ( TickType_t ) 0U ), ( ( BaseType_t ) 0 ) ) != ( ( BaseType_t ) 1 ))
        {
            // We would expect this call to fail because we cannot give
            // a semaphore without first "taking" it!
            printf("Semaphore is free now\r\n");
        }
    }

    // create all the task
    BaseType_t error = xTaskCreatePinnedToCore(main_task, "main_task", 1024 * 100, 
# 76 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                                                                                  __null
# 76 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                                                                                      , 1, 
# 76 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                                                                                           __null
# 76 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                                                                                               , 1);
    printf("Create main_task code: %d\r\n", error);
    // Pin "main_task" into core 1
    error = xTaskCreatePinnedToCore(user_handler_task, "user_handler_task", 1024 * 20, 
# 79 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                                                                                      __null
# 79 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                                                                                          , 1, 
# 79 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                                                                                               __null
# 79 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                                                                                                   , 0);
    printf("Create user_handler_task code: %d\r\n", error);
    // Pin "user_handler_task" into core 2
}

/******************************************************************************************/
void loop()
{
    // all the things are done in tasks
    vTaskSuspend(
# 88 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                __null
# 88 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                    ); // stop this task
}

/******************************************************************************************/
void ethernet_handler_task(void *pr)
{
    if (xQueueGenericReceive( ( QueueHandle_t ) ( xSemaphore ), 
# 94 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
       __null
# 94 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
       , ( (TickType_t)0 ), ( ( BaseType_t ) 0 ) ))
    {
        printf("ethernet_handler_task has taken the semaphored, semaphore value: %d\r\n", uxQueueMessagesWaiting( ( QueueHandle_t ) ( xSemaphore ) ));

        ethernet_handler_task_callback_fnc();

        vTaskDelete(RequestTaskHandle);
        if (xQueueGenericSend( ( QueueHandle_t ) ( xSemaphore ), 
# 101 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
           __null
# 101 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
           , ( ( TickType_t ) 0U ), ( ( BaseType_t ) 0 ) ) == ( ( BaseType_t ) 1 ))
        {
            printf("ethernet_handler_task has given the semaphored\r\n");
            printf("Task end\r\n");
        }
    }
}

/******************************************************************************************/
void user_handler_task(void *pr)
{
    int ticks = 0;
    for (;;)
    {
        if (xQueueGenericReceive( ( QueueHandle_t ) ( xSemaphore ), 
# 115 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
           __null
# 115 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
           , ( (TickType_t)0 ), ( ( BaseType_t ) 0 ) ))
        {
            //printf("user_handler_task has taken the semaphored, semaphore value: %d\r\n",
            //       uxSemaphoreGetCount(xSemaphore));

            user_handler_task_callback_func();
            // check ethernet cable

            if (millis() - ticks > 500)
            {
                ticks = millis();
                knife_capture.ethernet_handle.checking_ethernet_module();
            }

            xQueueGenericSend( ( QueueHandle_t ) ( xSemaphore ), 
# 129 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
           __null
# 129 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
           , ( ( TickType_t ) 0U ), ( ( BaseType_t ) 0 ) );
        }

        vTaskDelay(2);
    }
}

/******************************************************************************************/
void main_task(void *pr)
{
    for (;;)
    {
        if (!InitializeFinish && !knife_capture.sys_requesting)
        {
            InitialTimes--;
            printf("Retry to initializes the device\r\n");

            if (RequestTimeOut_TimerHandle == 
# 146 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                                             __null
# 146 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                                                 )
            {
                if (!sys_create_request_timeout_timer())
                {
                    knife_capture.checking_reset_counter_request();
                }
            }
            else
            {
                xTimerGenericCommand( ( RequestTimeOut_TimerHandle ), ( ( BaseType_t ) 2 ), ( xTaskGetTickCount() ), 
# 155 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
               __null
# 155 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
               , ( (TickType_t)0 ) );
                knife_capture.checking_reset_counter_request();
            }
        }

        if (xQueueGenericReceive( ( QueueHandle_t ) ( xSemaphore ), 
# 160 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
           __null
# 160 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
           , ( (TickType_t)0 ), ( ( BaseType_t ) 0 ) ))
        {
            //printf("main_task has taken the semaphored, semaphore value: %d\r\n",
            //       uxSemaphoreGetCount(xSemaphore));
            // listen to nextion screen
            //nex_listening();

            // ethernet_waiting for data coming in
            //knife_capture.ethernet_handle.running();
            main_task_callback_func();

            xQueueGenericSend( ( QueueHandle_t ) ( xSemaphore ), 
# 171 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
           __null
# 171 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
           , ( ( TickType_t ) 0U ), ( ( BaseType_t ) 0 ) );
        }

        vTaskDelay(2);
    }
}

/******************************************************************************************/
inline void user_handler_task_callback_func()
{
    nex_listening();

    // ethernet_waiting for data coming in
    knife_capture.ethernet_handle.running();

    if (DevicePostNow)
    {
        printf("Post local data now\r\n");
        DevicePostNow = false;
        knife_capture.local_device_post_data();
    }

    if (EthernetMaintainNow)
    {
        EthernetMaintainNow = false;
        knife_capture.ethernet_handle.ethernet_maintain();
    }
}

inline void ethernet_handler_task_callback_fnc()
{
    printf("\r\n[USER_DEBUG] ----> Func: %s at line: %d\r\n", __func__, 202);
    // if create timer success, then make new request

    if (RequestTimeOut_TimerHandle == 
# 205 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                                     __null
# 205 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                                         )
    {
        if (!sys_create_request_timeout_timer())
        {
            if (!knife_capture.ethernet_request_next())
            {
                printf("Connect to server failed\r\n");

                //BaseType_t excp = xTimerStop(RequestTimeOut_TimerHandle, (TickType_t)0);
                //printf("Stop request waiting timer, Excp code: %d\r\n", excp);
                knife_capture.sys_requesting = false;
            }
        }
    }

    if (RequestTimeOut_TimerHandle != 
# 220 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                                     __null
# 220 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                                         )
    {
        BaseType_t excp = xTimerGenericCommand( ( RequestTimeOut_TimerHandle ), ( ( BaseType_t ) 2 ), ( xTaskGetTickCount() ), 
# 222 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                         __null
# 222 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                         , ( (TickType_t)0 ) );
        printf("Reset request waiting timer, Excp code: %d\r\n", excp);
        if (!knife_capture.ethernet_request_next())
        {
            printf("Connect to server failed\r\n");

            //BaseType_t excp = xTimerStop(RequestTimeOut_TimerHandle, (TickType_t)0);
            knife_capture.sys_requesting = false;
            //printf("Stop request waiting timer, Excp code: %d\r\n", excp);
        }
    }
}

/******************************************************************************************/
inline void main_task_callback_func()
{
    // if ethernet queue exsit, create new request task
    if (knife_capture.ethernet_queue_available())
    {
        if (RequestTimeOut_TimerHandle == 
# 241 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                                         __null
# 241 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                                             )
        {
            if (!sys_create_request_timeout_timer())
            {
                if (!knife_capture.ethernet_post_capture())
                {
                    printf("Connect to server failed\r\n");

                    //BaseType_t excp = xTimerStop(RequestTimeOut_TimerHandle, (TickType_t)0);
                    // printf("Stop request waiting timer, Excp code: %d\r\n", excp);
                    knife_capture.sys_requesting = false;
                }
            }
        }

        if (RequestTimeOut_TimerHandle != 
# 256 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                                         __null
# 256 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                                             )
        {
            BaseType_t excp = xTimerGenericCommand( ( RequestTimeOut_TimerHandle ), ( ( BaseType_t ) 2 ), ( xTaskGetTickCount() ), 
# 258 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino" 3 4
                             __null
# 258 "c:\\Users\\Administrator\\Documents\\GitHub\\KnifeCapture\\KnifeCapture\\KnifeCapture.ino"
                             , ( (TickType_t)0 ) );
            printf("Reset request waiting timer, Excp code: %d\r\n", excp);
            if (!knife_capture.ethernet_post_capture())
            {
                printf("Connect to server failed\r\n");

                //BaseType_t excp = xTimerStop(RequestTimeOut_TimerHandle, (TickType_t)0);
                //printf("Stop request waiting timer, Excp code: %d\r\n", excp);
                knife_capture.sys_requesting = false;
            }
        }
    }
}

void ethernet_data_received_callback(EthernetClient &stream)
{
    printf("\r\n[USER_DEBUG] ----> Func: %s at line: %d\r\n", __func__, 274);
    int receivedBytes = stream.available();
    memset(knife_capture.http_header.buf, 0, sizeof(knife_capture.http_header.buf));
    stream.readBytesUntil('\r\n', knife_capture.http_header.buf, receivedBytes);

    printf("New data received: %s\r\n", knife_capture.http_header.buf);

    if (strstr(knife_capture.http_header.buf, "200 OK") != nullptr)
    {
        printf("response: Success\n\n");
    }

    if (strstr(knife_capture.http_header.buf, "Access denied") != nullptr)
    {
        printf("response: Access denied\n\n");

        knife_capture.sys_requesting = false;

        if (knife_capture.knife_capture_submit)
            knife_capture.knife_capture_submit = false;
    }

    if (strstr(knife_capture.http_header.buf, "404 Not Found") != nullptr)
    {
        printf("response: Not Found\n\n");

        knife_capture.sys_requesting = false;

        if (knife_capture.knife_capture_submit)
            knife_capture.knife_capture_submit = false;
    }

    if (strstr(knife_capture.http_header.buf, "400 Bad Request") != nullptr)
    {
        printf("response: Bad Request\n\n");

        knife_capture.sys_requesting = false;

        if (knife_capture.knife_capture_submit)
            knife_capture.knife_capture_submit = false;
    }

    if (strstr(knife_capture.http_header.buf, "{") != nullptr)
    {
        printf("new data is comming\r\n");
        DynamicJsonDocument json_doc = DynamicJsonDocument(1024 * 2);
        DeserializationError JsonErr = deserializeJson(json_doc, knife_capture.http_header.buf);
        printf("parse json object code: %s\r\n", JsonErr.c_str());

        if (!JsonErr)
        {
            String eop = json_doc.getMember("EOP").as<const char *>();
            char mesg[100]{0};
            String CurrentTime = "";

            if (eop == "knife_capture")
            {
                printf("Reviced new Json data\r\n");
                CurrentTime = json_doc.getMember("CurrentTime").as<const char *>();
                snprintf(mesg, sizeof(mesg), "Last request time: %s", CurrentTime.c_str());
                nex_send_message(mesg);

                knife_capture.ethernet_parse_all_received_data(json_doc, true);

                printf("Remove request from queue\r\n");
                if (knife_capture.request_list.size() > 0)
                    knife_capture.request_list.pop();

                printf("Post local data\r\n");
                knife_capture.local_device_post_data();

                if (knife_capture.knife_capture_submit)
                  knife_capture.knife_capture_submit = false;

                knife_capture.sys_requesting = false;
            }

            if (eop == "kc_initial") // initial resp
            {
                InitializeFinish = true;
                printf("Reviced new Json data\r\n");
                CurrentTime = json_doc.getMember("CurrentTime").as<const char *>();
                snprintf(mesg, sizeof(mesg), "Last initializes time: %s", CurrentTime.c_str());
                nex_send_message(mesg);

                knife_capture.ethernet_parse_all_received_data(json_doc, false);

                printf("Post local data now\r\n");
                knife_capture.local_device_post_data();

                if (knife_capture.knife_capture_submit)
                  knife_capture.knife_capture_submit = false;

                knife_capture.sys_requesting = false;
            }

            if (eop == "post_resp") // post
            {
                printf("Reviced new Json data\r\n");

                CurrentTime = json_doc.getMember("CurrentTime").as<const char *>();
                snprintf(mesg, sizeof(mesg), "Last post time: %s", CurrentTime.c_str());
                nex_send_message(mesg);

                printf("Remove request from queue\r\n");
                if (knife_capture.request_list.size() > 0)
                    knife_capture.request_list.pop();

                if (knife_capture.knife_capture_submit)
                  knife_capture.knife_capture_submit = false;

                knife_capture.sys_requesting = false;
            }
        }
    }
    printf("End ethernet_data_received_callback\r\n");
}
