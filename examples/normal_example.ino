#include "miniserial.h"

int setup () {
  MiniSerial_begin(9600);
  MiniSerial_print(F("ARDUINO MINISERIAL - "__DATE__" "__TIME__"\n"));
}

int loop() {
  if (MiniSerial_available()) {
    uint8_t curkey = MiniSerial_read() & 0x7F;
    MiniSerial_print(F("Key pressed was:0x"));
    MiniSerial_byte(curkey,HEX,true);
    MiniSerial_write('\n');

    static uint32_t memoMillis;
    memoMillis = millis();
    MiniSerial_print(F("Millis is: "));
    MiniSerial_long(memoMillis);
    MiniSerial_print(F("ms.\n"));
  }
}
