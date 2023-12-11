//author: Steph Herbers

#include "Adafruit_VL53L0X.h"
#include <AsyncTCP.h>
#include <AsyncWebServer.h>
#include <ESP32Servo.h>
#include "WiFi.h"
#include <Wire.h>

// Define motor control pins
const int frontPWM = 13;
const int front1Direction = 5;
const int front2Direction = 4;

const int middlePWM = 12;
const int middle1Direction = 16;
const int middle2Direction = 17;

const int backPWM = 14;
const int back1Direction = 18;
const int back2Direction = 19;

const int frequency = 500;
const int pwm_channel = 0;
const int resolution = 8;

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
  pinMode(front1Direction, OUTPUT);
  pinMode(front2Direction, OUTPUT);

  pinMode(middlePWM, OUTPUT);
  pinMode(middle1Direction, OUTPUT);
  pinMode(middle2Direction, OUTPUT);

  pinMode(backPWM, OUTPUT);
  pinMode(back1Direction, OUTPUT);
  pinMode(back2tDirection, OUTPUT);

  edcSetup(pwm_channel, frequency, resolution);
  ledcAttachPin(frontPWM, pwm_channel);
  ledcAttachPin(middlePWM, pwm_channel);
  ledcAttachPin(backPWM, pwm_channel);

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
        controlFrontMotorsSpeed(255);
        controlMiddleMotorsSpeed(255);
        controlBackMotorsSpeed(255);
        // TODO: Code to move forward
        Serial.println("Moving forward");
      } else if (command == "backward") {
        // TODO: Code to move backward
        Serial.println("Moving backward");
      } else if (command == "stop") {
        controlFrontMotorsSpeed(0);
        controlMiddleMotorsSpeed(0);
        controlBackMotorsSpeed(0);
        // TODO: Code to stop movement
        Serial.println("Stop");
      }
    }
    request->send(200, "text/html", generateRemoteControlPage());
    // request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

String generateRemoteControlPage() {
  String remote_page = "<html><head><style>";
  remote_page += "body { text-align: center; }";
  remote_page += "button { font-size: 24px; padding: 15px 20px; margin: 10px; }";
  remote_page += "</style></head><body>";
  remote_page += "<h1 style='text-align: center'>DocOckBot Remote Control</h1>";
  remote_page += "<div class='container' style='border: 20px solid black; text-align: center;'>";
  remote_page += "<form action='/control' method='post'>";
  remote_page += "<button type='submit' name='command' value='forward' style='font-size: 100px; width: 50%; border: 4px solid #04AA6D; margin:10px;'>&#8679;</button><br>";
  remote_page += "<button type='submit' name='command' value='stop' style='font-size:100px; width: 50%; border: 4px solid red; margin:10px;'>&#10005;</button><br>";
  remote_page += "<button type='submit' name='command' value='backward' style='font-size: 100px; width: 50%; border: 4px solid #04AA6D; margin:10px;'>&#8681;</button><br>";
  remote_page += "</div></form></body></html>";
  return remote_page;
}

void controlFrontMotorsSpeed(int speed) {
    digitalWrite(front1Direction, HIGH); // Set direction to forward
    digitalWrite(front2Direction, LOW);
    ledcWrite(pwm_channel, speed);
}

void controlMiddleMotorsSpeed(int speed) {
    digitalWrite(middle1Direction, HIGH);
    digitalWrite(middle2Direction, LOW);
    ledcWrite(pwm_channel, speed);
}

void controlBackMotorsSpeed(int speed) {
    digitalWrite(back1Direction, HIGH);
    digitalWrite(back2Direction, LOW);
    ledcWrite(pwm_channel, speed);
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  float distanceToStair = measure.RangeMilliMeter; // lidar output in mm

  controlFrontMotorsSpeed(150);
  controlMiddleMotorsSpeed(150);
  controlBackMotorsSpeed(150);
}
