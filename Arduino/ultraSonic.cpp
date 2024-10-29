#include "ultraSonic.h"

int Eyes::shouldMoveForward() {
  return (getDistance() >= minDistance) ? Forward : Stop;
}

int Eyes::chooseSide() {
  float leftDistance, rightDistance;
  while (true) {
    lookLeft();
    leftDistance = getDistance();
    lookRight();
    rightDistance = getDistance();
    lookForward();

    if (leftDistance < minDistance && rightDistance < minDistance) {
      move(Backward);
      delay(500);
      move(Stop);
    } else {
      return (leftDistance > rightDistance) ? Contrarotate : Clockwise;
    }
  }
}