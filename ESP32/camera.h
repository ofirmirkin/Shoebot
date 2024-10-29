#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include "esp_camera.h"

#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

class ESPcam {
  camera_config_t config;
public:
  ESPcam() {}
  bool init(); // True if init successfull
  camera_fb_t * takePicture();
  void returnFrameBuffer(camera_fb_t *&fb);
};

#endif