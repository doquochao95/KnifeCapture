#include "rfid_mfrc522.h"
MFRC522 mfrc(MFRC522_SS_PIN, MFRC522_RST_PIN);
uint8_t TagNumber[16];

void mfrc522_init()
{
    printf("Initialize MFRC522\r\n");
    SPI.begin();
    mfrc.PCD_Init();
    delay(10);
    mfrc.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
    printf("Scan PICC to see UID, SAK, type, and data blocks...\r\n");
}

String read_tagNumber()
{
    String uid = "";

    for (int8_t i = 0; i < 4; i++)
    {
        (mfrc.uid.uidByte[3 - i] < 0x10) ? uid += '0' : "";

        uid += String(mfrc.uid.uidByte[3 - i], HEX);
    }
    uid.toUpperCase();
    return uid;
}

bool mfrc522_read_new_tag()
{
    if (mfrc.PICC_IsNewCardPresent())
    {
        if (mfrc.PICC_ReadCardSerial())
        {
            mfrc.PICC_HaltA();
            mfrc.PCD_StopCrypto1();

            String tag = read_tagNumber();
            memccpy(TagNumber, tag.c_str(), 0, sizeof(TagNumber));
            return true;
        }
    }
    return false;
}

void Output_Alarm(uint8_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        digitalWrite(ALARM_PIN, LOW);
        delay(50);
        digitalWrite(ALARM_PIN, HIGH);
        delay(50);
    }
}
