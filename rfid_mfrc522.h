#ifndef rfid_mfrc522_h
#define rfid_mfrc522_h

#include "global_config.h"
#include "MFRC522.h"
#include "SPI.h"

extern MFRC522 mfrc;
extern uint8_t TagNumber[16];

void mfrc522_init();
String read_tagNumber();
bool mfrc522_read_new_tag();
void Output_Alarm(uint8_t count = 1);
#endif
