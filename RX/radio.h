#include <SPI.h>
#include <NRFLite.h>
//#include "printf.h"

const static uint8_t RADIO_ID = 0;       // Our radio's id.  The transmitter will send to this id.
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;

struct RadioPacket {
  int car_fb; //We define each byte of data input, in this case just 6 channels
  int car_lr;
  int gim_ud;
  int gim_lr;
  int car_speed;
  int dance;
  int blade;
  int ppmMode;
};

NRFLite _radio;
RadioPacket data;
