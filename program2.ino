#include <Servo.h>

// Define servo objects
Servo servo1;  // Aileron+Elevator 1
Servo servo2;  // Aileron+Elevator 2
Servo servo3;  // Rudder+Elevator 1
Servo servo4;  // Rudder+Elevator 2

// Pin assignments
const int servo1Pin = 9;
const int servo2Pin = 10;
const int servo3Pin = 11;
const int servo4Pin = 12;

// Receiver input pins
const int elevatorPin = 2;  // Channel 1 (PWM input)
const int aileronPin = 3;   // Channel 2 (PWM input)
const int rudderPin = 4;    // Channel 3 (PWM input)

// Variables for PWM reading
volatile unsigned long elevatorStart, aileronStart, rudderStart;
volatile int elevatorPWM = 1500, aileronPWM = 1500, rudderPWM = 1500;

// Mixing parameters (adjust as needed)
const float MIX_FACTOR = 0.5; // How much elevator mixes with aileron/rudder (0.0 to 1.0)

void setup() {
  // Attach servos to pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);
  
  // Set up receiver input pins
  pinMode(elevatorPin, INPUT);
  pinMode(aileronPin, INPUT);
  pinMode(rudderPin, INPUT);
  
  // Attach interrupts for PWM reading
  attachInterrupt(digitalPinToInterrupt(elevatorPin), calcElevator, CHANGE);
  attachInterrupt(digitalPinToInterrupt(aileronPin), calcAileron, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rudderPin), calcRudder, CHANGE);
  
  // Center all servos initially
  servo1.writeMicroseconds(1500);
  servo2.writeMicroseconds(1500);
  servo3.writeMicroseconds(1500);
  servo4.writeMicroseconds(1500);
  
  delay(2000); // Wait for servos to initialize
}

void loop() {
  // Calculate servo positions with mixing
  
  // Servos 1 & 2: Aileron + Elevator mix
  int servo1Pos = aileronPWM + ((elevatorPWM - 1500) * MIX_FACTOR);
  int servo2Pos = (3000 - aileronPWM) + ((elevatorPWM - 1500) * MIX_FACTOR); // Opposite direction for second aileron
  
  // Servos 3 & 4: Rudder + Elevator mix
  int servo3Pos = rudderPWM + ((elevatorPWM - 1500) * MIX_FACTOR);
  int servo4Pos = (3000 - rudderPWM) + ((elevatorPWM - 1500) * MIX_FACTOR); // Opposite direction for second rudder
  
  // Constrain values to safe range (1000-2000 μs)
  servo1Pos = constrain(servo1Pos, 1000, 2000);
  servo2Pos = constrain(servo2Pos, 1000, 2000);
  servo3Pos = constrain(servo3Pos, 1000, 2000);
  servo4Pos = constrain(servo4Pos, 1000, 2000);
  
  // Write to servos
  servo1.writeMicroseconds(servo1Pos);
  servo2.writeMicroseconds(servo2Pos);
  servo3.writeMicroseconds(servo3Pos);
  servo4.writeMicroseconds(servo4Pos);
  
  // Small delay to prevent servo jitter
  delay(20);
}

// Interrupt service routines for PWM reading
void calcElevator() {
  if (digitalRead(elevatorPin) == HIGH) {
    elevatorStart = micros();
  } else {
    if (elevatorStart != 0) {
      elevatorPWM = micros() - elevatorStart;
      elevatorStart = 0;
      // Constrain to valid range
      elevatorPWM = constrain(elevatorPWM, 1000, 2000);
    }
  }
}

void calcAileron() {
  if (digitalRead(aileronPin) == HIGH) {
    aileronStart = micros();
  } else {
    if (aileronStart != 0) {
      aileronPWM = micros() - aileronStart;
      aileronStart = 0;
      // Constrain to valid range
      aileronPWM = constrain(aileronPWM, 1000, 2000);
    }
  }
}

void calcRudder() {
  if (digitalRead(rudderPin) == HIGH) {
    rudderStart = micros();
  } else {
    if (rudderStart != 0) {
      rudderPWM = micros() - rudderStart;
      rudderStart = 0;
      // Constrain to valid range
      rudderPWM = constrain(rudderPWM, 1000, 2000);
    }
  }
}