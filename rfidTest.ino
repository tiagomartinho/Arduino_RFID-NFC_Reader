#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

// Use Arduino default SPI connection
// SCK = 13, MOSI = 11, MISO = 12, SS = 10
#define SS_PIN   (10)
Adafruit_PN532 nfc(SS_PIN);

void setup(void) {
  Serial.begin(115200);
  setupReader();
}

void setupReader(void)
{
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find RFID reader");
    while (1); // halt
  }
  Serial.println("Found RFID reader");
  nfc.SAMConfig();
  Serial.println("Waiting for card");
}

uint8_t success;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID//
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

void loop(void) {

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    printUID();
    delay(1000);
  }
}

void printUID(void) {
  Serial.print("\n{\n \"UID\" : \"");
  nfc.PrintHex(uid, uidLength);
  Serial.print("\"}\n");
}
