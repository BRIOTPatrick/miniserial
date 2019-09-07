#ifndef MINISERIAL_h
#define MINISERIAL_h

// Version 1.00 31/08/2019 Final stable development -- footprint Code:328 RAM:18

#include <avr/interrupt.h>              // mainly for ISR
#include <Wstring.h>                    // mainly for __FlashStringHelper and data type (uint8_t,...)
#include <Print.h>                      // mainly for HEX DEC OCT BIN

#define SERIALACTIVE                    // when not defined, functions below inactivated.

#ifdef SERIALACTIVE
#define MiniSerial_byte(val,...)        MiniSerial_data_((uint8_t)val, 0,##__VA_ARGS__)
#define MiniSerial_word(val,...)        MiniSerial_data_((uint16_t)val,1,##__VA_ARGS__)
#define MiniSerial_long(val,...)        MiniSerial_data_((uint32_t)val,2,##__VA_ARGS__)
#define MiniSerial_print(ifsh)          MiniSerial_print_(ifsh)
#define MiniSerial_write(b)             MiniSerial_write_(b)
#define MiniSerial_read()               MiniSerial_read_()
#define MiniSerial_available()          MiniSerial_available_()
#define MiniSerial_begin(baud)          MiniSerial_begin_(baud)
#else
#define MiniSerial_byte(val,...)
#define MiniSerial_word(val,...)
#define MiniSerial_long(val,...)
#define MiniSerial_print(ifsh)
#define MiniSerial_write(b)
#define MiniSerial_read()               (0)
#define MiniSerial_available()          (0)
#define MiniSerial_begin(baud)
#endif SERIALACTIVE

#define FIFO_SIZE 16

typedef struct {
  uint8_t head;
  uint8_t tail;
  uint8_t buff[FIFO_SIZE];
} FIFO;

// swap
//uint8_t a = 10; uint8_t b = 20;
//asm ( ""  : "=r" (a),"=r" (b)  : "0" (b),"1" (a));

#ifdef __cplusplus
extern "C" {
#endif __cplusplus

void    MiniSerial_data_(uint32_t val, uint8_t bwl, uint8_t base=DEC, uint8_t zero=false);
void    MiniSerial_print_(const __FlashStringHelper *ifsh);
void    MiniSerial_write_(uint8_t b);
uint8_t MiniSerial_read_();
uint8_t MiniSerial_available_();
void    MiniSerial_begin_(uint32_t baud);

#ifdef __cplusplus
}
#endif __cplusplus

#endif  MINISERIAL_h
