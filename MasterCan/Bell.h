#ifndef BELL_H
#define BELL_H

class Bell {

  int buzzerpin;
  int greenLEDPin;
  int redLEDPin;

public:

  Bell(int buzzerpin, int greenLEDPin, int redLEDPin);
  void start();
  void buzzerStart();
  void buzzerStop();
  void error();
  void fatalError();

};

// Set variables
Bell::Bell(int buzzerpin, int greenLEDPin, int redLEDPin) {
  Bell::buzzerpin = buzzerpin;
  Bell::greenLEDPin = greenLEDPin;
  Bell::redLEDPin = redLEDPin;
  
}

// Initialise LEDs as outputs and start green LED
void Bell::start() {
    pinMode(greenLEDPin, OUTPUT);
    pinMode(redLEDPin, OUTPUT);
    digitalWrite(greenLEDPin,HIGH);
}

//Code to Start buzzer
void Bell::buzzerStart() {
  tone(buzzerpin, 10000);
}

// Code to end buzzer
void Bell::buzzerStop() {
  noTone(buzzerpin);
}

// Function to turn on the red LED in case of an error
void Bell::error() {
    digitalWrite(redLEDPin,HIGH);
}

// Function to stop all code, beep the buzzer and the LEDs
void Bell::fatalError() {
  while (true)
  {
    buzzerStop();
    digitalWrite(redLEDPin,HIGH);
    delay(500);
    buzzerStart();
    digitalWrite(redLEDPin,LOW);
    delay(500);
  }
}

#endif
