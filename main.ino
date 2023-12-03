//author: Steph Herbers

#include "Adafruit_VL53L0X.h"
#include <ESP32Servo.h>
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
