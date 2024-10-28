#ifndef MOVE_H
#define MOVE_H

#include <Arduino.h>

// PWM control pin
#define PWM1_PIN 5
#define PWM2_PIN 6

// 74HCT595N Chip pins
#define SHCP_PIN 2  // The displacement of the clock
#define EN_PIN 7    // Can make control
#define DATA_PIN 8  // Serial data
#define STCP_PIN 4  // Memory register clock

const int Forward = 92;        // forward
const int Backward = 163;      // back
const int Left = 149;          // left translation
const int Right = 106;         // Right translation
const int Top_Left = 20;       // Upper left mobile
const int Bottom_Left = 129;   // Lower left mobile
const int Top_Right = 72;      // Upper right mobile
const int Bottom_Right = 34;   // The lower right move
const int Stop = 0;            // stop
const int Contrarotate = 172;  // Counterclockwise rotation
const int Clockwise = 83;      // Rotate clockwise

const int defaultSpeed = 250;
const float timeFullRotation = 2.5;  // Time in seconds to complete a 360 rotation using the default speed
const float time1meter = 3.5;   // Time in seconds to travel a 1m
const float timeSideway1meter = 4;   // Time in seconds to travel a 1m sideways

void motorInit();

/* Signal the motors. A delay() is needed for the robot to move */
void move(int direction, int Speed = defaultSpeed);

/* Move in the given direction for the given time */
void moveForTime(int timeMs, int direction);

/* Turn by a given angle */
void turnAngle(int angle, bool clockwise);

/* Move a given distance */
void moveDistance(int distance, bool forward);

/* Move to the side by a given distance */
void sideMoveDistance(int distance, bool right);

#endif