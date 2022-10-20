#if defined(ARDUINO_ARCH_SAMD)
#ifndef IOT_EEPROM_h
#define IOT_EEPROM_h

#include "IoTFlashStorage.h"

#ifndef EEPROM_EMULATION_SIZE
#define EEPROM_EMULATION_SIZE 1024
#endif

typedef struct {
  byte data[EEPROM_EMULATION_SIZE];
  boolean valid;
} EEPROM_EMULATION;


class IoTEEPROMClass 
{

  public:
    IoTEEPROMClass(void);

    uint8_t read(int);

    void write(int, uint8_t);

    void update(int, uint8_t);

    bool isValid();

    void commit();

    uint16_t length() { return EEPROM_EMULATION_SIZE; }

  private:
    void init();

    bool initialized;
    EEPROM_EMULATION eeprom;
    bool dirty;
};

extern class IoTEEPROMClass EEPROM;

#endif
#endif