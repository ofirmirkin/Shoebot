// #include "ESPHttpClient.h"
#include "camera.h"
#include "esp_camera.h"
#include "reaction.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <comm.h>

const String ssid = "YOUR_WIFI";
const String password = "YOUR_PASSWORD";
const String serverIP = "http://YOUR_IP:8000/";

WiFiClient client;
ESPcam camera;
Comm comm;
Led led(12, 13, 15);
Buzzer buzzer(14);
Reactor reactor(led, buzzer);

void setup() {
    reactor.init();

    Serial.begin(9600);
    Serial.setDebugOutput(true);
    Serial.println();

    /* WiFi initialization */
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");

    /* Camera initialization */
    while (!camera.init()) {
        Serial.println("camera.init failed");
        delay(5000);
    }

    /* Take 2 pictures because the first 2 are often corrupted */
    for (int i = 0; i < 2; i++) {
        camera_fb_t *fb = camera.takePicture();
        if (fb) {
            Serial.println("Pic succes");
            esp_camera_fb_return(fb);
        }
    }

    /* Server health check */
    if (performHealthCheck()) {
        Serial.println("Server is healthy, proceeding.");
    } else {
        Serial.println("Server health check failed");
    }

    /* Comm initialization */
    comm.init();

    Serial.println("Starting"); // Debugging
}

String cmd = "";
void loop() {
    cmd = comm.receiveCmd();
    if (cmd == "p") {
        Serial.println("Got post cmd"); // Debugging
        String line = sendPicture();

        if (!line.isEmpty()) {
            // Parse the JSON data
            DynamicJsonDocument doc(200);
            DeserializationError error = deserializeJson(doc, line);
            if (error) {
                Serial.print(F("deserializeJson() failed: ")); // Debugging
                Serial.println(error.f_str());
                return;
            }

            // Extract the prediction value
            int prediction = doc["prediction"].as<int>();
            Serial.println("Prediction: " + String(prediction)); // Debugging

            if (prediction == 0)
                comm.sendCmd("s\n"); // "Shoe detected" - The robot will stop for longer
            else
                comm.sendCmd("-\n"); // "No shoe detected"

            reactor.react(prediction); // React to the prediction

        } else {
            Serial.println("Line didn't start with {"); // Debugging
        }
        cmd = "";
    } else {
        // Serial.println("Got cmd : " + cmd); // Debugging
    }
}

String sendPicture() {
    const String BOUNDARY = "----MK----";
    const String fileName = "image";
    const int TIMEOUT = 10000;

    // Capture an image
    camera_fb_t *fb = camera.takePicture();
    if (!fb) {
        Serial.println("Camera capture failed");
        return "";
    }

    // Init the HTTP client
    HTTPClient http;
    http.begin(serverIP + "upload"); // Specify server Url
    http.addHeader("Content-Type", "multipart/form-data; boundary=" + BOUNDARY);

    String head = "--" + BOUNDARY + "\r\nContent-Disposition: form-data; name=\"image\"; filename=\"" + fileName + ".jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--" + BOUNDARY + "--\r\n";

    int totalLen = head.length() + fb->len + tail.length();
    http.addHeader("Content-Length", String(totalLen));

    // Create a buffer to hold the entire request body
    uint8_t *body = (uint8_t *)malloc(totalLen);
    if (!body) {
        Serial.println("Memory allocation failed");
        esp_camera_fb_return(fb);
        return "";
    }

    // Copy head, image data, and tail into the buffer
    memcpy(body, head.c_str(), head.length());
    memcpy(body + head.length(), fb->buf, fb->len);
    memcpy(body + head.length() + fb->len, tail.c_str(), tail.length());

    // Send the POST request
    int httpResponseCode = http.POST(body, totalLen);
    String response = http.getString();

    if (httpResponseCode > 0) {
        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
        Serial.println(response);
    } else {
        Serial.printf("Error code: %d\n", httpResponseCode);
    }

    // Clean up
    free(body);
    http.end();
    esp_camera_fb_return(fb);

    return response;
}

bool performHealthCheck() {
    HTTPClient http;
    http.begin(serverIP + "health");
    int httpResponseCode = http.GET(); // Send the GET request

    if (httpResponseCode > 0) { // Check if request was successful
        String response = http.getString();
        Serial.print("Health check response: ");
        Serial.println(response);

        if (httpResponseCode == 200) {
            http.end(); // Close connection
            return true;
        }
    } else {
        Serial.print("Error on HTTP request: ");
        Serial.println(httpResponseCode);
    }

    http.end(); // Close connection
    return false;
}
