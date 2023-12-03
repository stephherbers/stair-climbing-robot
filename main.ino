//author: Steph Herbers

#include "Adafruit_VL53L0X.h"
#include <AsyncTCP.h>
#include <AsyncWebServer.h>
#include <ESP32Servo.h>
#include "WiFi.h"
#include <Wire.h>

// Define motor control pins
const int frontPWM = 3;
const int frontRightDirection = 4;
const int frontLeftDirection = 5;

const int middlePWM = 3;
const int middleRightDirection = 4;
const int middleLeftDirection = 5;

const int backPWM = 3;
const int backRightDirection = 4;
const int backLeftDirection = 5;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int sda_pin=21, scl_pin=22; //LiDAR pins

const char* ssid = "Tufts_Robot";
const char* password = "";
const char* param_servo = "servo";
const char* param_target = "target";
AsyncWebServer server(80);

void setup() {
  Serial.begin(9600);

  // Set the motor control pins as outputs
  pinMode(frontPWM, OUTPUT);
  pinMode(frontRightDirection, OUTPUT);
  pinMode(frontLeftDirection, OUTPUT);

  pinMode(middlePWM, OUTPUT);
  pinMode(middleRightDirection, OUTPUT);
  pinMode(middleLeftDirection, OUTPUT);

  pinMode(backPWM, OUTPUT);
  pinMode(backRightDirection, OUTPUT);
  pinMode(backLeftDirection, OUTPUT);

  //LiDAR Setup
  Wire.begin(sda_pin, scl_pin);
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
  while(1);
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Route for root / webpage
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", generateRemoteControlPage());
  });

  // Route for handling actions from buttons
  server.on("/control", HTTP_POST, [](AsyncWebServerRequest *request){
    String command;
    if (request->hasParam("command", true)) {
      command = request->getParam("command", true)->value();
      
      // Perform action based on the received command
      if (command == "forward") {
        // TODO: Code to move forward
        Serial.println("Moving forward");
      } else if (command == "backward") {
        // TODO: Code to move backward
        Serial.println("Moving backward");
      } else if (command == "stop") {
        // TODO: Code to stop movement
        Serial.println("Stop");
      }
    }
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

String generateRemoteControlPage() {
  String remotePage = "<html><body>";
  remotePage += "<h1>ESP32 Remote Control</h1>";
  remotePage += "<form action='/control' method='post'>";
  remotePage += "<button type='submit' name='command' value='forward'>Forward</button>";
  remotePage += "<button type='submit' name='command' value='backward'>Backward</button>";
  remotePage += "<button type='submit' name='command' value='stop'>Stop</button>";
  remotePage += "</form></body></html>";
  return remotePage;
}

void controlFrontMotorsSpeed(int speed) {
    digitalWrite(frontRightDirection, HIGH); // Set direction to forward
    digitalWrite(frontLeftDirection, HIGH);
    analogWrite(frontPWM, speed); // Set speed (0-255)
}

void controlMiddleMotorsSpeed(int speed) {
    digitalWrite(middleRightDirection, HIGH);
    digitalWrite(middleLeftDirection, HIGH);
    analogWrite(middlePWM, speed);
}

void controlBackMotorsSpeed(int speed) {
    digitalWrite(backRightDirection, HIGH);
    digitalWrite(backLeftDirection, HIGH);
    analogWrite(backPWM, speed);
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  float distanceToStair = measure.RangeMilliMeter; // lidar output in mm

  controlFrontMotorsSpeed(150);
  controlMiddleMotorsSpeed(150);
  controlBackMotorsSpeed(150);
}
