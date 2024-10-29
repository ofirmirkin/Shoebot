#ifndef BUZZER_H
#define BUZZER_H

#include <pitches.h>

class Buzzer {
  const int pin;

public:
  Buzzer(int buzzerPin)
    : pin(buzzerPin) {}

  void init() {
    pinMode(pin, OUTPUT);
    Serial.println("pin : " + pin);
    stop();
  }

  void setTone(int note) {
    tone(pin, note);
  }

  void buzz(int note = NOTE_B0, int duration = 1000) {
    tone(pin, note);  // Set the voltage to high and makes a noise
    delay(duration);  // Waits for 1000 milliseconds
    noTone(pin);      // Sets the voltage to low and makes no noise
  }

  void stop() {
    noTone(pin);
  }

  // Adapted from https://docs.arduino.cc/built-in-examples/digital/toneMelody/
  void playMelody() {
    // notes in the melody:
    int melody[] = {
      NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
    };
    // note durations: 4 = quarter note, 8 = eighth note, etc.:
    int noteDurations[] = {
      4, 8, 8, 4, 4, 4, 4, 4
    };
    for (int thisNote = 0; thisNote < 8; thisNote++) {
      // to calculate the note duration, take one second divided by the note type.
      // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(pin, melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      noTone(pin);
    }
  }

  void siren() {
    // Frequencies
    const int lowFreq = NOTE_A4;   // Low frequency (A4)
    const int highFreq = NOTE_A5;  // High frequency (A5)

    // Timing
    const int delayTime = 10;    // Time between each frequency step (ms)
    const int sweepTime = 1000;  // Time for one sweep cycle (ms)

    // Sweep from low to high frequency
    for (int freq = lowFreq; freq <= highFreq; freq += 10) {
      tone(pin, freq);
      delay(delayTime);
    }

    // Sweep from high to low frequency
    for (int freq = highFreq; freq >= lowFreq; freq -= 10) {
      tone(pin, freq);
      delay(delayTime);
    }

    delay(sweepTime);  // Add some delay between cycles
  }
};

#endif