#ifndef LED_H
#define LED_H

struct Color {
  int red, green, blue;
};

constexpr Color GREEN{ 0, 255, 0 };
constexpr Color RED{ 255, 0, 0 };
constexpr Color BLUE{ 0, 0, 255 };
constexpr Color PURPLE{ 170, 0, 255 };
constexpr Color WHITE{ 255, 255, 255 };
constexpr Color OFF{ 0, 0, 0 };

class Led {
  int redPin, greenPin, bluePin;
public:
  Led(int r, int g, int b)
    : redPin(r), greenPin(g), bluePin(b) {}

  void init() {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    off();
  }

  void off() {
    set(OFF);
  }

  void set(const Color &c) const {
    analogWrite(redPin, c.red);
    analogWrite(greenPin, c.green);
    analogWrite(bluePin, c.blue);
  }

  /* Duration of led in ms */
  void blink(const Color &c, int duration) const {
    set(c);
    delay(duration);
    set(OFF);
  }

  void policeCar() {
    // Timing
    const int flashDuration = 200;  // Flash duration in milliseconds
    const int pauseDuration = 100;  // Pause between flashes

    // Turn on Red LED and turn off Blue LED
    set(RED);
    delay(flashDuration);  // Wait for the flash duration

    // Pause (both LEDs off)
    set(OFF);
    delay(pauseDuration);

    // Turn on Blue LED and turn off Red LED
    set(BLUE);
    delay(flashDuration);  // Wait for the flash duration

    // Pause (both LEDs off)
    analogWrite(bluePin, 0);  // Blue off
    delay(pauseDuration);
  }
};

#endif