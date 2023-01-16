// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include "buzzer.h"
#include "ultrasonic.h"
#include <NewPing.h>
#include <Servo.h>

#define TRIGGER_PIN  52  // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN     53  // Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Servo ultrasonic;
Servo buzzer;

String RX;
const String readString = "1,2,3,4,5,6,7";

const int time = 0;
int pos = 0; 
String C_FB; //data String
String C_LR;
String G_UD;
String G_LR;
String C_SPD;
String Dance;
String Blade;

int ind1; // , locations
int ind2;
int ind3;
int ind4;
int ind5;
int ind6;
int ind7;

const int IN1 = 24;
const int IN2 = 25;
const int IN3 = 26;
const int IN4 = 27;

const int IN5 = A6;
const int IN6 = A7;
const int IN7 = A8;
const int IN8 = A9;

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  //Serial.println(RX);
  ultrasonic.attach(8);
  buzzer.attach(11);

}

void loop()
{
  splice();
  while (!C_FB.toInt()) {
    stop();
    Serial.println("wait");
    splice();
  }

  range();
  buzzer.detach();
  if (C_FB.toInt() > 1700) {
    Serial.println("FORWARD");
    fd();
  }
  else if (C_FB.toInt() < 1300) {
    Serial.println("BACKWARD");
    bd();
  }
  if (C_LR.toInt() > 1700) {
    Serial.println("LEFT");
    left();
  }
  else if (C_LR.toInt() < 1300) {
    Serial.println("RIGHT");
    right();
  }
  if (C_LR.toInt() > 1700 && Blade.toInt() == 20) {
    Serial.println("OMNI-LEFT");
    omni_left();
  }
  else if (C_LR.toInt() < 1300 && Blade.toInt() == 20) {
    Serial.println("OMNI-RIGHT");
    omni_right();
  }
  Serial.println("stop");
  if (C_LR.toInt() > 1300 && C_LR.toInt() < 1700 && C_FB.toInt() > 1300 && C_FB.toInt() < 1700) {
    stop();
  }
}



void receiveEvent(int howMany)
{
  RX = "";
  while (1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    //Serial.print(c);         // print the character
    RX.concat(c);
  }
  int x = Wire.read();    // receive byte as an integer
  //Serial.println(x + "hihih");         // print the integer
  RX.concat(char(x));

}

void splice() {
  ind1 = RX.indexOf(',');  //finds location of first ,
  C_FB = RX.substring(0, ind1);   //captures first data String

  ind2 = RX.indexOf(',', ind1 + 1 ); //finds location of second ,
  C_LR = RX.substring(ind1 + 1, ind2 + 1); //captures second data String

  ind3 = RX.indexOf(',', ind2 + 1 );
  G_UD = RX.substring(ind2 + 1, ind3 + 1);

  ind4 = RX.indexOf(',', ind3 + 1 );
  G_LR = RX.substring(ind3 + 1, ind4 + 1);

  ind5 = RX.indexOf(',', ind4 + 1 );
  C_SPD = RX.substring(ind4 + 1, ind5 + 1);

  ind6 = RX.indexOf(',', ind5 + 1 );
  Dance = RX.substring(ind5 + 1, ind6 + 1);

  ind7 = RX.indexOf(',', ind6 + 1 );
  Blade = RX.substring(ind6 + 1, ind7 + 1);
  Blade = RX.substring(RX.length() - 4);
  C_LR.replace(",", "");
  G_UD.replace(",", "");
  G_LR.replace(",", "");
  C_SPD.replace(",", "");
  Dance.replace(",", "");
  Blade.replace("0,", "");

  Serial.print("car_fb: ");
  Serial.print(C_FB);//gim ud
  Serial.print("       ");
  Serial.print("car_lr: ");
  Serial.print(C_LR);//gim lr
  Serial.print("       ");
  Serial.print("gim_ud: ");
  Serial.print(G_UD);//car lr
  Serial.print("       ");
  Serial.print("gim_lr: ");
  Serial.print(G_LR);//car ud
  Serial.print("       ");
  Serial.print("speed: ");
  Serial.print(C_SPD);
  Serial.print("       ");
  Serial.print("dance: ");
  Serial.print(Dance);
  Serial.print("       ");
  Serial.print("blade: ");
  Serial.print(Blade);
  Serial.println("       ");
}

void fd() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, LOW);

  delay(time);
}

void bd() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, LOW);

  delay(time);
}

void left() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);

  delay(time);
}

void right() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);

  delay(time);
}

void omni_right() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);

  delay(time);
}

void omni_left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);

  delay(time);
}

void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, LOW);

  delay(time);
}

void range() {
  buzzer.attach(11);
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");

  if (uS / US_ROUNDTRIP_CM <= 15 && uS / US_ROUNDTRIP_CM != 0) {
    Serial.println("DIE========================================================================================================");
    ultrasonic.write(70);
    delay(500);
    ultrasonic.write(110);
    delay(500);
    ultrasonic.write(90);

    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      buzzer.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      buzzer.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    
  }
}
