#ifndef REMOTE_H
#define REMOTE_H

#include <IRremote.hpp>

#define IR_RECEIVE_PIN 3

class Remote {
  bool run = false;

public:
  void init() {
    IrReceiver.begin(IR_RECEIVE_PIN);
  }

  void listen() {
    if (IrReceiver.decode()) {
      IrReceiver.resume();  // Early enable receiving of the next IR frame
      if (IrReceiver.decodedIRData.command == 0x45) { // CH- button
        run = true;
      } else if (IrReceiver.decodedIRData.command == 0x46) { // CH+ button
        run = false;
      }
    }
  }

  bool getState() {
    return run;
  }
};

#endif