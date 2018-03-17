#include "Arduino.h"
#include "EEPROM.h"

class MemoryController
{
 public:
  uint8_t readByte (int addr)
   {
     byte b;
     EEPROM.get(addr, b);
     return b;
   };
  uint16_t readWord (int addr)
   {
    uint16_t w;
    EEPROM.get (addr, w);    
    return w;
   }
  void writeByte (int addr, uint8_t value)
   {     
     EEPROM.put (addr, value);
   };
  void writeWord (int addr, uint16_t value)
   {
     EEPROM.put (addr, value);
   };
};
