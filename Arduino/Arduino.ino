#include "ultraSonic.h"
#include "move.h"
#include "remote.h"
#include <comm.h>

const int STOP_DELAY = 250;
const int LONG_DELAY = 5000;
const int SHORT_DELAY = 1000;
const int MSG_CHECK_INTERVAL = 100;
const int MAX_MSG_CHECKS = 30;
const int TURN_ANGLE = 45;
const int MOVE_DELAY = 500;

Eyes eyes;       // Controls the servo and ultrasonic sensor
Remote remote;   // Controls the IR remote
Comm esp32Comm;  // Communications with ESP32

void setup() {
  Serial.begin(9600);

  eyes.init(90);
  motorInit();
  remote.init();

  esp32Comm.init();

  // Wait for start signal from the remote
  while (remote.getState() == false) {
    remote.listen();
    delay(200);
  }
}


/* The robot will move forward until an obstacle is detexted */
int dir = Forward;
void loop() {
  if (dir == Stop) {
    move(Stop);                // Stop the movement
    delay(STOP_DELAY);         // Short delay after stopping to allow camera to stabilize
    esp32Comm.sendCmd("p\n");  // Send a command to ESP32 to take a picture

    // Try to receive a message for a fixed number of attempts
    for (int i = 0; i < MAX_MSG_CHECKS; i++) {
      String msg = esp32Comm.receiveCmd();

      if (msg == "s") {
        delay(LONG_DELAY);  // Stop for a longer time on receiving "s"
        break;
      } else if (msg == "-") {
        delay(SHORT_DELAY);  // Shorter delay for "-"
        break;
      }
      delay(MSG_CHECK_INTERVAL);  // Small delay between checks
    }

    // After stoping, choose a new direction
    int side = eyes.chooseSide();
    turnAngle(TURN_ANGLE, side == Clockwise);  // Turn based on chosen side
    dir = Forward;
  }

  // Move the robot forward
  move(dir);
  delay(MOVE_DELAY);  // Delay to allow movement to complete

  // Select the next direction
  dir = eyes.shouldMoveForward();
}

/* 360° scan of an object */
void scan(int nPics) {
  eyes.look(180);  // Look as much to the left as possible

  float distanceToObject = eyes.getDistance();
  float perimeter = circlePerimeter(distanceToObject);
  float edgeLen = perimeter / nPics;
  int angle = 360 / nPics;

  for (int i = 0; i < nPics; i++) {
    esp32Comm.sendCmd("Take picture\n");  // Take a picture
    delay(2000);                          // Ensure enough time for the picture to be taken

    turnAngle(angle / 2, false);  // Turn left by half the angle
    moveDistance(edgeLen, true);  // Move by edgeLen to the right
    turnAngle(angle / 2, false);  // Turn left by the other half of the angle
    move(Stop);                   // Stop after each step
    delay(10);
  }
}

float circlePerimeter(float radius) {
  return 2 * PI * radius;
}
