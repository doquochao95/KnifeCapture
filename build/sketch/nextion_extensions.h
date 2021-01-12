#ifndef nextion_extension_h
#define nextion_extension_h
#include <HardwareSerial.h>
#include "NextionVariableString.h"
#include "NextionVariableNumeric.h"
#include "NextionText.h"
#include "NextionPicture.h"
#include "NextionPage.h"
#include "NextionNumber.h"
#include "NextionButton.h"
#include "Nextion.h"
#include "INextionTouchable.h"
#include "motor_serial0.h"

#define NextionSerial Serial2
extern const char *NexPropertyType[5];
extern Nextion nex;
extern INextionTouchable PAGE_LOADING_EVENT;

void nex_listening();
void sendCommand(char *commandStr);
void nex_backto_screen1();
void nex_goto_page(char *page_name);
void getPropertyName(const char *inputName, char *outputName, uint8_t type);
void setNumberProperty(const char *pageName, char *propertyName, uint32_t value);
uint32_t getNumberProperty(const char *pageName, char *propertyName);
void setStringProperty(const char *pageName, char *propertyName, char *value);
size_t getStringProperty(const char *pageName, char *propertyName, char *value, size_t len);
void setNumberProperty(const char *propertyName, uint32_t value);
uint32_t getNumberProperty(const char *propertyName);
void setStringProperty(const char *propertyName, char *value);
size_t getStringProperty(const char *propertyName, char *value, size_t len);
void nex_send_message(char *message);
void nex_init();
void nex_set_page_number(int num);
#endif
