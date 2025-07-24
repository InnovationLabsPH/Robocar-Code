// Define constants
const int BLACK = 1;  // Black line returns HIGH (1)
const int WHITE = 0;  // White surface returns LOW (0)

// Motor control pins
const int LEFT_MOTOR_DIR = 2;
const int LEFT_MOTOR_PWM = 5;
const int RIGHT_MOTOR_DIR = 4;
const int RIGHT_MOTOR_PWM = 6;

// Sensor pins
const int LEFT_SENSOR = 7;
const int RIGHT_SENSOR = 9;

// Motor speeds - SLOWER VALUES
const int BASE_SPEED = 45;        // Normal forward speed (reduced from 80)
const int TURN_SPEED = 55;        // Speed during turns (reduced from 90)
const int SLOW_SPEED = 25;        // Speed when both sensors detect black
const int CORRECTION_SPEED = 35;  // Speed for minor corrections

// Timing
const unsigned long SAMPLE_DELAY = 25;  // Slightly longer delay
const unsigned long TURN_CONTINUE_TIME = 200; // Longer turn continuation

// State variables
bool lastTurnWasLeft = false;
unsigned long lastTurnTime = 0;
int straightCounter = 0;
bool wasOnLine = false;

void setup() {
  Serial.begin(9600);
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
  pinMode(LEFT_MOTOR_DIR, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIR, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  
  stopMotors();
  delay(1000);
  Serial.println("Starting line follower...");
}

void loop() {
  lineFollower();
  delay(SAMPLE_DELAY);
}

void lineFollower() {
  int leftVal = digitalRead(LEFT_SENSOR);
  int rightVal = digitalRead(RIGHT_SENSOR);
  
  Serial.print("Sensors: L=");
  Serial.print(leftVal);
  Serial.print(" R=");
  Serial.println(rightVal);

  // Current line detection state
  bool onLine = (leftVal == BLACK || rightVal == BLACK);
  
  // Case 1: Both sensors on white
  if (leftVal == WHITE && rightVal == WHITE) {
    if (wasOnLine && millis() - lastTurnTime < TURN_CONTINUE_TIME) {
      // Continue last turn direction
      if (lastTurnWasLeft) {
        smoothLeft();
        Serial.println("Continuing left turn");
      } else {
        smoothRight();
        Serial.println("Continuing right turn");
      }
    } 
    else {
      // Gentle correction based on last turn direction
      if (lastTurnWasLeft) {
        slightRight();
        Serial.println("Correcting right");
      } else {
        slightLeft();
        Serial.println("Correcting left");
      }
    }
    wasOnLine = false;
    straightCounter = 0;
  }
  // Case 2: Left sensor on black (turn LEFT)
  else if (leftVal == BLACK && rightVal == WHITE) {
    turnLeft();
    lastTurnWasLeft = true;
    lastTurnTime = millis();
    straightCounter = 0;
    wasOnLine = true;
    Serial.println("Turning LEFT");
  }
  // Case 3: Right sensor on black (turn RIGHT)
  else if (leftVal == WHITE && rightVal == BLACK) {
    turnRight();
    lastTurnWasLeft = false;
    lastTurnTime = millis();
    straightCounter = 0;
    wasOnLine = true;
    Serial.println("Turning RIGHT");
  }
  // Case 4: Both sensors on black
  else if (leftVal == BLACK && rightVal == BLACK) {
    if (straightCounter > 2) { // Confident straight line
      moveForward();
      Serial.println("Forward straight");
    } else {
      moveForwardSlow();
      Serial.println("Forward cautious");
    }
    straightCounter++;
    wasOnLine = true;
  }
}

void moveForward() {
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
}

void moveForwardSlow() {
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTOR_PWM, SLOW_SPEED);
  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(RIGHT_MOTOR_PWM, SLOW_SPEED);
}

void turnLeft() {
  // Left motor backward, right motor forward
  digitalWrite(LEFT_MOTOR_DIR, LOW);
  analogWrite(LEFT_MOTOR_PWM, TURN_SPEED);
  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(RIGHT_MOTOR_PWM, TURN_SPEED);
}

void turnRight() {
  // Left motor forward, right motor backward
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTOR_PWM, TURN_SPEED);
  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  analogWrite(RIGHT_MOTOR_PWM, TURN_SPEED);
}

void smoothLeft() {
  // Left motor slow forward, right motor faster forward
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTOR_PWM, CORRECTION_SPEED);
  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(RIGHT_MOTOR_PWM, TURN_SPEED);
}

void smoothRight() {
  // Left motor faster forward, right motor slow forward
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTOR_PWM, TURN_SPEED);
  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(RIGHT_MOTOR_PWM, CORRECTION_SPEED);
}

void slightLeft() {
  // Very slight left adjustment
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED - 15);
  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED + 15);
}

void slightRight() {
  // Very slight right adjustment
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED + 15);
  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED - 15);
}

void stopMotors() {
  digitalWrite(LEFT_MOTOR_DIR, LOW);
  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(LEFT_MOTOR_PWM, 0);
  analogWrite(RIGHT_MOTOR_PWM, 0);
}