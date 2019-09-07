#ifndef h//15436-1796
#define h

// Version 1.00 31/08/2019 Final stable development -- footprint Code:328 RAM:18

#include <avr/interrupt.h>              // mainly for ISR
#include <Wstring.h>                    // mainly for __FlashStringHelper and data type (uint8_t,...)
#include <Print.h>                      // mainly for HEX DEC OCT BIN

#define SERIALACTIVE                    // when not defined, functions below inactivated.

class miniserial {
public:
  #ifdef SERIALACTIVE
    #define byte(val,...)        data_((uint8_t)val, 0,##__VA_ARGS__)
    #define word(val,...)        data_((uint16_t)val,1,##__VA_ARGS__)
    #define long(val,...)        data_((uint32_t)val,2,##__VA_ARGS__)
    #define print(ifsh)          print_(ifsh)
    #define write(b)             write_(b)
    #define read()               read_()
    #define available()          available_()
    #define begin(baud)          begin_(baud)
  #else
    #define byte(val,...)
    #define word(val,...)
    #define long(val,...)
    #define print(ifsh)
    #define write(b)
    #define read()               (0)
    #define available()          (0)
    #define begin(baud)
  #endif SERIALACTIVE

  #define FIFO_SIZE 16

private:
  void    data_(uint32_t val, uint8_t bwl, uint8_t base=DEC, uint8_t zero=false);
  void    print_(const __FlashStringHelper *ifsh);
  void    write_(uint8_t b);
  uint8_t read_();
  uint8_t available_();
  void    begin_(uint32_t baud);

  typedef struct {
    uint8_t head;
    uint8_t tail;
    uint8_t buff[FIFO_SIZE];
  } FIFO;
};

#endif  h
