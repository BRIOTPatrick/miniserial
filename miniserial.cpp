#include "miniserial.h"

#ifdef SERIALACTIVE
volatile FIFO FifoRx;

//------------------------------------------------------------------------------
ISR(USART_RX_vect) {//96
  if((FifoRx.tail + 1) % FIFO_SIZE != FifoRx.head)
    FifoRx.buff[FifoRx.tail = ++FifoRx.tail % FIFO_SIZE] = UDR0;
}
//------------------------------------------------------------------------------
uint32_t pow_bwl(uint32_t base, uint8_t expo) {
  return (expo?base*pow_bwl(base,expo-1):1);
}
//------------------------------------------------------------------------------
void miniserial::data_(uint32_t val, uint8_t bwl, uint8_t base=DEC, uint8_t zero=false) {//108
  uint8_t len;
  switch (base) {
    case BIN : len=bwl?(--bwl?32:16):8; break;
    case OCT : len=bwl?(--bwl?11: 6):3; break;
    case DEC : len=bwl?(--bwl?10: 5):3; break;
    case HEX : len=bwl?(--bwl? 8: 4):2; break;
    default  : return;
  }
  while (len--) {
    uint32_t tmp2 = pow_bwl(base, len);
    uint8_t tmp = (val/tmp2);
    val -= tmp*tmp2;

    if(zero=(zero) | (tmp) | !(len)) {
      MiniSerial_write(tmp+(tmp<10?'0':'A'-10));
    }
  }
}
//------------------------------------------------------------------------------
void miniserial::print_(const __FlashStringHelper *ifsh) {//12
  const char * __attribute__((progmem)) p = (const char * ) ifsh;
  while (unsigned char c = pgm_read_byte(p++)) {
    MiniSerial_write(c);
  }
}
//------------------------------------------------------------------------------
void miniserial::write_(uint8_t b) {//130
  while (((1 << UDRIE0) & UCSR0B) || !(UCSR0A & (1 << UDRE0))) {}
  UDR0 = b;
}
//------------------------------------------------------------------------------
uint8_t miniserial::read_() {//150
  if(FifoRx.tail == FifoRx.head) return 0;
  uint8_t data = FifoRx.buff[FifoRx.head];
  FifoRx.head = ++FifoRx.head % FIFO_SIZE;
  return data;
}
//------------------------------------------------------------------------------
uint8_t miniserial::available_() {//116
  return ((FifoRx.tail - FifoRx.head + FIFO_SIZE) % FIFO_SIZE);
}
//------------------------------------------------------------------------------
void miniserial::begin_(uint32_t baud) {//34
  FifoRx.head = FifoRx.tail = 0;

  uint16_t baud_setting;
  // don't worry, the compiler will squeeze out F_CPU != 16000000UL
  if ((F_CPU != 16000000UL || baud != 57600) && baud > (F_CPU/(4*(2*0XFFF + 1)) + 1)) {
    // Double the USART Transmission Speed
    UCSR0A = 1 << U2X0;
    baud_setting = (F_CPU / 4 / baud - 1) / 2;
  } else {
    // hardcoded exception for compatibility with the bootloader shipped
    // with the Duemilanove and previous boards and the firmware on the 8U2
    // on the Uno and Mega 2560.
    UCSR0A = 0;
    baud_setting = (F_CPU / 8 / baud - 1) / 2;
  }
  // assign the baud_setting
  UBRR0H = baud_setting >> 8;
  UBRR0L = baud_setting;

  // Enable Transmitter | Receiver | Rx Complete Interrupt
  UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
}
//------------------------------------------------------------------------------
#endif SERIALACTIVE
