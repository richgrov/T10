#include <AccelStepper.h>
#include <Arduino.h>

#define MOTOR_STEPS_PER_REV 200
#define MICROSTEP_RESOLUTION 4

AccelStepper stepper(AccelStepper::DRIVER, 7, 6);

void setup() {
   // Used to enable microstepping pin
   pinMode(2, OUTPUT);
   digitalWrite(2, HIGH);

   stepper.setMaxSpeed(200 * MICROSTEP_RESOLUTION);
   stepper.setSpeed(200 * MICROSTEP_RESOLUTION);
   stepper.setAcceleration(9999);
   stepper.moveTo(0);
}

void loop() {
   if (stepper.distanceToGo() == 0) {
      stepper.moveTo(stepper.currentPosition() + MOTOR_STEPS_PER_REV / 4 * MICROSTEP_RESOLUTION);
      delay(3 * 1000);
   }

   stepper.run();
}
