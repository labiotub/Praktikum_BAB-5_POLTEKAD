#include <Servo.h>
#include <Stepper.h>

// Initialize Servo motor
Servo servoMotor;

// Initialize Stepper motor (28BYJ-48)
const int stepsPerRevolution = 2048;  // Number of steps for a full revolution
Stepper myStepper(stepsPerRevolution, 11, 10, 9, 8);  // Pins for stepper motor

// Variables to store mode and angle
int mode = 0;  // Default mode: 0 for Servo, 1 for Stepper
int lastStepperPosition = 0;  // To track the current position of the stepper motor (in steps)

void setup() {
  // Start serial communication with Raspberry Pi using Serial0
  Serial.begin(9600);
  
  // Attach the servo motor to pin 7
  servoMotor.attach(7);
  
  // Set the speed of the stepper motor
  myStepper.setSpeed(5);  // Speed in RPM

  Serial.println("Arduino Uno is ready. Waiting for commands...");
}

void loop() {
  // Check if there is serial data available from Raspberry Pi
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');  // Read the incoming command
    command.trim();  // Remove any whitespace/newlines

    // Display the received command in the Serial Monitor
    Serial.print("Command received: ");
    Serial.println(command);  // Print the command for debugging

    // Check if the command is to change mode
    if (command.startsWith("mode:")) {
      mode = command.substring(5).toInt();  // Extract mode value (0 for Servo, 1 for Stepper)
      Serial.print("Mode changed to: ");
      Serial.println(mode == 0 ? "Servo" : "Stepper");
    }

    // Check if the command is to change the angle
    if (command.startsWith("angle:")) {
      int angle = command.substring(6).toInt();  // Extract angle value from the command
      Serial.print("Angle received: ");
      Serial.println(angle);

      if (angle >= 0 && angle <= 100) {
        if (mode == 0) {  // Servo mode
          int servoAngle = map(angle, 0, 100, 0, 180);  // Map the input angle to the servo's range (0-180 degrees)
          servoMotor.write(servoAngle);  // Move the servo to the desired angle
          Serial.print("Servo moved to angle: ");
          Serial.println(servoAngle);
        } else if (mode == 1) {  // Stepper mode
          int steps = map(angle, 0, 100, 0, stepsPerRevolution);  // Map the input angle to 0-2048 steps (full rotation)
          int stepsToMove = steps - lastStepperPosition;  // Calculate the relative number of steps to move

          // Move the stepper by the calculated number of steps
          myStepper.step(stepsToMove);

          // Store the new position as the current position of the stepper
          lastStepperPosition = steps;

          Serial.print("Stepper moved to angle: ");
          Serial.println(angle);
        }
      } else {
        Serial.println("Invalid angle. Please provide a value between 0 and 100.");
      }
    }
  }
}
