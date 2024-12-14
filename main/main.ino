#include <AccelStepper.h>
#include <Arduino.h>

#define MOTOR_STEPS_PER_REV 200
#define MICROSTEP_RESOLUTION 4

AccelStepper base_stepper(AccelStepper::DRIVER, 7, 6);
AccelStepper end_stepper(AccelStepper::DRIVER, 9, 8);
int direction = 1;

void setup() {
   // Used to enable microstepping pin
   pinMode(2, OUTPUT);
   digitalWrite(2, HIGH);
   pinMode(3, OUTPUT);
   digitalWrite(3, HIGH);

   base_stepper.setMaxSpeed(100 * MICROSTEP_RESOLUTION);
   base_stepper.setSpeed(100 * MICROSTEP_RESOLUTION);
   base_stepper.setAcceleration(400);
   base_stepper.moveTo(0);

   end_stepper.setMaxSpeed(200 * MICROSTEP_RESOLUTION);
   end_stepper.setSpeed(200 * MICROSTEP_RESOLUTION);
}

void loop() {
   if (base_stepper.distanceToGo() == 0) {
      int diff_to_next_pos = MOTOR_STEPS_PER_REV / 4 * MICROSTEP_RESOLUTION;
      base_stepper.moveTo(base_stepper.currentPosition() + diff_to_next_pos * direction);
      direction = -direction;
   }

   base_stepper.run();
   end_stepper.runSpeed();
}
