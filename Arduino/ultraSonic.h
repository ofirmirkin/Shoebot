#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include <Servo.h>
#include "move.h"

const int initialAngle = 90;
const int servoSpeed = 10;     // Defines the delay in ms between each movement of the servo, i.e. larger = slower
const int speedOfSound = 343;  // m/s
const int microsecondsPerCm = 29;

// Depends on the specific project
const int servoPin = 9;
const int sonicPingPin = 12;
const int sonicRecivePin = 13;

const int minDistance = 40;  // in cm

class Eyes {
private:
  Servo servo;
public:

  Eyes() {}

  void initServo(int angle = initialAngle) {
    servo.attach(servoPin);
    look(angle);
  }

  void initSonic() {
    pinMode(sonicPingPin, OUTPUT);
    pinMode(sonicRecivePin, INPUT);
  }

  void init(int angle = initialAngle) {
    initServo(angle);
    initSonic();
  }

  void look(int endAngle) {
    int currentAngle = servo.read();
    int step = (currentAngle < endAngle) ? 1 : -1;

    // To avoid sudden turns, move the servo with steps of 1 degree
    for (int pos = currentAngle; pos != endAngle; pos += step) {
      servo.write(pos);
      delay(servoSpeed);
    }
  }

  void lookForward() {
    look(initialAngle);
  }

  void lookLeft() {
    look(135);
  }

  void lookRight() {
    look(45);
  }

  void sendPing() {
    // The PING is triggered by a HIGH pulse of 2 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    digitalWrite(sonicPingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(sonicPingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(sonicPingPin, LOW);
  }

  float recivePing() {
    float duration = pulseIn(sonicRecivePin, HIGH);  // In microseconds
    delay(10);
    return duration / microsecondsPerCm / 2;
  }

  float getDistance() {
    sendPing();
    return recivePing();
  }

  /* Returns Forward is no obstacle is detected */
  int shouldMoveForward();

  /* Choses if the robot should turn right or left */
  int chooseSide();
};

#endif