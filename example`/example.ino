#include "miniserial.h"

//===============================================================================================================
// ARDUINO
//===============================================================================================================
#include <wiring.c>
//---------------------------------------------------------------------------------------------------------------
void initTimer() {
  sei();
#if defined(TCCR0A) && defined(WGM01)
  sbi(TCCR0A, WGM01);
  sbi(TCCR0A, WGM00);
#endif

  // set timer 0 prescale factor to 64
#if defined(TCCR0B) && defined(CS01) && defined(CS00)
  // this combination is for the standard 168/328/1280/2560
  sbi(TCCR0B, CS01);
  sbi(TCCR0B, CS00);
#else
  #error Timer 0 prescale factor 64 not set correctly
#endif

  // enable timer 0 overflow interrupt
#if defined(TIMSK) && defined(TOIE0)
  sbi(TIMSK, TOIE0);
#elif defined(TIMSK0) && defined(TOIE0)
  sbi(TIMSK0, TOIE0);
#else
  #error  Timer 0 overflow interrupt not set correctly
#endif
}
//---------------------------------------------------------------------------------------------------------------
int main () {
  // Setup ***************************************************************
  initTimer();
  MiniSerial_begin(9600);
  MiniSerial_print(F("ARDUINO MINISERIAL - "__DATE__" "__TIME__"\n"));

  // Loop  ***************************************************************
  for (;;) {
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
}
//---------------------------------------------------------------------------------------------------
