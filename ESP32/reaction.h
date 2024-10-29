#ifndef REACTION_H
#define REACTION_H

#include "buzzer.h"
#include "led.h"

class Reactor {
  Buzzer buzzer;
  Led led;

public:
  Reactor(const Led &l, const Buzzer &b)
    : led(l), buzzer(b) {}

  void init() {
    led.init();
    buzzer.init();
  }

  void madReaction(int n) {
    for (int i = 0; i < n; i++) {
      policeCar();
    }
    buzzer.stop();
    led.off();
  }

  void policeCar() {
    // Frequencies
    const int lowFreq = NOTE_A4;   // Low frequency (A4)
    const int highFreq = NOTE_A5;  // High frequency (A5)

    // Timing
    const int delayTime = 10;    // Time between each frequency step (ms)
    const int sweepTime = 1000;  // Time for one sweep cycle (ms)

    // Sweep from low to high frequency
    led.set(RED);
    for (int freq = lowFreq; freq <= highFreq; freq += 10) {
      buzzer.setTone(freq);
      delay(delayTime);
    }

    // Sweep from high to low frequency
    led.set(BLUE);
    for (int freq = highFreq; freq >= lowFreq; freq -= 10) {
      buzzer.setTone(freq);
      delay(delayTime);
    }
  }

  void react(int prediction) {
    switch (prediction) {
      case 0:
        Serial.println("Predicted : Shoes");
        madReaction(5);
        break;
      case 1:
        Serial.println("Predicted : Flip-flops");
        led.set(GREEN);
        buzzer.playMelody();
        led.set(OFF);
        break;
      case 2:
        Serial.println("Predicted : None");
        led.blink(WHITE, 3000);
        led.set(OFF);
        break;
      default:
        Serial.println("Predicted : Error");
        led.set(OFF);
    }
  }
};

#endif