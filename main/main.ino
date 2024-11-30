#include <Arduino.h>
#include <Stepper.h>

#define MOTOR_STEPS_PER_REV 200
#define MICROSTEP_RESOLUTION 4

Stepper stepper(MOTOR_STEPS_PER_REV, 6, 7);

void setup() {
   // Used to enable microstepping pin
   pinMode(2, OUTPUT);
   digitalWrite(2, HIGH);

   stepper.setSpeed(300);
}

void loop() {
   stepper.step(400 * MICROSTEP_RESOLUTION);
   delay(3 * 1000);
}
