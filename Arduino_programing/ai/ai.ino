#include <AccelStepper.h>

#define MOTOR_STEPS 200  // Adjust this according to your motor's specification
#define DIR_PIN 5
#define STEP_PIN 2

AccelStepper stepper = AccelStepper(1, STEP_PIN, DIR_PIN);

unsigned long previousMillis = 0;
const long interval = 4000;  // 1 second
int checking = 0;

void setup() {
  Serial.begin(9600);
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(200);
}

void loop() {
  String command = Serial.readString();
  command.trim();  // Remove any leading/trailing whitespace
  if (command == "ON") {
    checking++;
   // Serial.println(checking);
    if (checking % 2 == 1) {
      Serial.println("Go");
    stepper.setSpeed(1000);
    stepper.moveTo(80);  // Move 90 degrees
    while (stepper.distanceToGo() != 0) {
      stepper.run();
      }
       previousMillis = millis();
    while (millis() - previousMillis < interval) {
      // Wait for 6 second using millis()
     // Serial.println("on");
    }
    }
    else if (checking % 2 == 0) {
      Serial.println("Back");
      stepper.moveTo(0);
      while (stepper.distanceToGo() != 0) {
      stepper.run();
      }
    }
   
  }
  stepper.run();
}