#ifndef sys_config_h
#define sys_config_h
#include <EEPROM.h>
#include "global_config.h"
#include "global_scope.h"
#include "knife_capture_handler.h"

extern SemaphoreHandle_t xSemaphore;
extern TaskHandle_t RequestTaskHandle;

extern TimerHandle_t RequestTimeOut_TimerHandle;
extern TimerHandle_t ConfirmTimeOut_TimerHandle;

extern volatile bool InitializeFinish;
extern volatile bool InitialRequestNow;
extern int InitialTimes;
extern int led;
extern int zero_button;
extern bool motor;

extern knife_capture_class knife_capture;

void confirm_timeout_timer(TimerHandle_t pxTimer);
void request_timeout_timer(TimerHandle_t pxTimer);

bool sys_create_confirm_timeout_timer();
bool sys_create_request_timeout_timer();

void ethernet_data_received_callback(EthernetClient &stream);

// nextion callback fucntions
void nx_knife_capture_submit(uint8_t but_id);
void PAGE_LOADING_EVENT_CALLBACK(uint8_t pageId, uint8_t componentId, uint8_t eventType);

void start_up();
void io_init();
void ethernet_init();
void local_data_init();

#endif
