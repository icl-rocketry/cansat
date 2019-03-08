#ifndef BUZZER_H
#define BUZZER_H

class Buzzer{
  int buzzerpin;
public:
  Buzzer(int buzzerpin);
  void buzzerStart();
  void buzzerStop();
  void errorBeep();
};

Buzzer::Buzzer(int buzzerpin) {
  Buzzer::buzzerpin = buzzerpin;
}

//Code to Start buzzer
void Buzzer::buzzerStart() {
  tone(buzzerpin, 10000);
}

// Code to end buzzer
void Buzzer::buzzerStop() {
  noTone(buzzerpin);
}

void Buzzer::errorBeep() {
  while (true)
  {
    buzzerStop();
    delay(500);
    buzzerStart();
    delay(500);
  }
}

#endif