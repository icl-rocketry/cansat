#ifndef VIBMOTOR_H
#define VIBMOTOR_H

class Vib{
    int vibpin;
public:
    Vib(int vibpin);
    void vibstart();
    void vibstop();
};

Vib::Vib(int vibpin) {
    Vib::vibpin = vibpin;
}

// Code to start vibration motor
void Vib::vibstart() {
  digitalWrite(vibpin,HIGH);
}

/* Code to end vibration motor
void Vib::vibstop() {
  digitalWrite(vibpin,LOW);
}*/

#endif
