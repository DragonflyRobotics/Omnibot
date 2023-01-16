#include "ppm.h"
#include "radio.h"
#include "printRadio.h"
#include <ServoTimer2.h>
#include <Wire.h>

ServoTimer2 PWM2;
ServoTimer2 PWM3;
ServoTimer2 PWM4;
ServoTimer2 PWM5;
ServoTimer2 PWM6;
ServoTimer2 PWM7;
ServoTimer2 PWM8;


void setup() {
  Serial.begin(115200);
  Wire.begin();
  PWM2.attach(3);
  PWM3.attach(4);
  PWM4.attach(5);
  PWM5.attach(6);
  PWM6.attach(7);
  PWM7.attach(8);
  PWM8.attach(A0);

  //printf_begin();
  //_radio.printDetails();
  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
  {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }

  //initiallize default ppm values
  for (int i = 0; i < CHANNEL_NUMBER; i++) {
    ppm[i] = CHANNEL_DEFAULT_VALUE;
  }

  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)

  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;

  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();
}

void loop() {
  Serial.println("sent");
  while (_radio.hasData())
  {
    Serial.println("in loop");
    _radio.readData(&data);
    Wire.beginTransmission(4); 
    String dataM = String(data.car_fb)+","+String(data.car_lr)+","+String(data.gim_ud)+","+String(data.gim_lr)+","+String(data.car_speed)+","+String(data.dance)+","+String(data.blade);
    Wire.write(dataM.c_str());      
    Wire.endTransmission(); 
 
    Serial.println("hihihihihi");
    /////////IF PWM ENABLED///////////////////////////////////////////////
    if (data.ppmMode == 0) {
      PWM2.write(data.car_fb);
      PWM3.write(data.car_lr);
      PWM4.write(data.gim_ud);
      PWM5.write(data.dance);
      PWM6.write(data.car_speed);
      PWM7.write(data.gim_lr);//repeat
      PWM8.write(data.blade);
      printRadio();
      Serial.println("PWM");
    }

    else if (data.ppmMode == 1) {
      ppm[0] = data.car_fb;
      ppm[1] = data.car_lr;
      ppm[2] = data.gim_ud;
      ppm[3] = data.gim_lr;
      ppm[4] = data.car_speed;
      ppm[5] = data.blade;
      ppm[6] = data.dance;
      printRadio();
      Serial.println("PPM");
    }

  }
}

ISR(TIMER1_COMPA_vect) { //leave this alone
  static boolean state = true;

  TCNT1 = 0;

  if (state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PULSE_LENGTH * 2;
    state = false;
  } else { //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;

    digitalWrite(sigPin, !onState);
    state = true;

    if (cur_chan_numb >= CHANNEL_NUMBER) {
      cur_chan_numb = 0;
      calc_rest = calc_rest + PULSE_LENGTH;//
      OCR1A = (FRAME_LENGTH - calc_rest) * 2;
      calc_rest = 0;
    }
    else {
      OCR1A = (ppm[cur_chan_numb] - PULSE_LENGTH) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }
  }
}
