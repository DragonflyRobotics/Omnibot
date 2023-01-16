//////////////////////CONFIGURATION///////////////////////////////
#define CHANNEL_NUMBER 7  //set the number of chanels
#define CHANNEL_DEFAULT_VALUE 1500  //set the default servo value
#define FRAME_LENGTH 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PULSE_LENGTH 300  //set the pulse length
#define onState 1  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 2  //set PPM signal output pin on the arduino

/*this array holds the servo values for the ppm signal
  change theese values in your code (usually servo values move between 1000 and 2000)*/
int ppm[CHANNEL_NUMBER];
