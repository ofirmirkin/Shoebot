#include "move.h"

void motorInit() {
  pinMode(SHCP_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(PWM1_PIN, OUTPUT);
  pinMode(PWM2_PIN, OUTPUT);
}

void move(int direction, int Speed) {
  digitalWrite(EN_PIN, LOW);
  analogWrite(PWM1_PIN, Speed);
  analogWrite(PWM2_PIN, Speed);

  digitalWrite(STCP_PIN, LOW);
  shiftOut(DATA_PIN, SHCP_PIN, MSBFIRST, direction);
  digitalWrite(STCP_PIN, HIGH);

  if (direction == Stop)
    delay(10);
}

// General move function to handle different movement types
void moveForTime(int timeMs, int direction) {
  move(direction);
  delay(timeMs);
  move(Stop);
}

// Function to turn the robot by a specific angle
void turnAngle(int angle, bool clockwise) {
  static const float angularVelocity = 360.0f / timeFullRotation; // degrees per second
  float timeRequired = (angle / angularVelocity) * 1000;          // Time in milliseconds
  moveForTime((int)timeRequired, clockwise ? Clockwise : Contrarotate);
}

// Function to move forward or backward a specific distance
void moveDistance(int distance, bool forward) {
  static const float linearVelocity = 100.0f / time1meter;        // cm per second
  float timeRequired = (distance / linearVelocity) * 1000;        // Time in milliseconds
  moveForTime((int)timeRequired, forward ? Forward : Backward);
}

// Function to move sideways (right or left) a specific distance
void sideMoveDistance(int distance, bool right) {
  static const float sideVelocity = 100.0f / timeSideway1meter;   // cm per second
  float timeRequired = (distance / sideVelocity) * 1000;          // Time in milliseconds
  moveForTime((int)timeRequired, right ? Right : Left);
}