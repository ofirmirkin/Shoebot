#ifndef COMMUNICATION_H
#define COMMUNICATION_H

class Comm {
    long baudRate = 9600;
    String receivedMessage = "";

public:
    Comm(long baudRate = 9600) {
        this->baudRate = baudRate;
    }

    void init() {
        Serial.begin(baudRate);
    }

    void sendCmd(String command) {
        Serial.print(command);
    }

    String receiveCmd() {
        receivedMessage = "";
        while (Serial.available() > 0) {
            char receivedChar = Serial.read();
            if (receivedChar == '\n') {
                Serial.println(receivedMessage); // Print the received message in the Serial monitor
                return receivedMessage;
            } else {
                receivedMessage += receivedChar; // Append characters to the received message
            }
        }
        return "";
    }
};

#endif