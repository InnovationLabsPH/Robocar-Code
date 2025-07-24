// Motor A pins
const int in1 = 2;
const int enA = 5;

// Motor B pins (B1)
const int in2 = 4;
const int enB = 6;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
}

void loop() {
  int speed = 100;
  int turnTime = 700;
  int forwardTime = 1000;
  int spinTime = 1200;
  int pause = 500;

  // 1. Move forward
  digitalWrite(in1, HIGH);
  analogWrite(enA, speed);
  digitalWrite(in2, LOW);
  analogWrite(enB, speed);
  delay(forwardTime);

  // Stop
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  delay(pause);

  // Move backward to original position
  digitalWrite(in1, LOW);
  analogWrite(enA, speed);
  digitalWrite(in2, HIGH);
  analogWrite(enB, speed);
  delay(forwardTime);

  // Stop
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  delay(pause);

  // 2. Turn right a bit
  digitalWrite(in1, HIGH);
  analogWrite(enA, speed);
  digitalWrite(in2, HIGH);
  analogWrite(enB, speed);
  delay(turnTime);

  // Stop
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  delay(pause);

  // Move forward
  digitalWrite(in1, HIGH);
  analogWrite(enA, speed);
  digitalWrite(in2, LOW);
  analogWrite(enB, speed);
  delay(forwardTime);

  // Stop
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  delay(pause);

  // Move backward to original position
  digitalWrite(in1, LOW);
  analogWrite(enA, speed);
  digitalWrite(in2, HIGH);
  analogWrite(enB, speed);
  delay(forwardTime);

  // Stop
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  delay(pause);

  // Turn left to original heading
  digitalWrite(in1, LOW);
  analogWrite(enA, speed);
  digitalWrite(in2, LOW);
  analogWrite(enB, speed);
  delay(turnTime);

  // Stop
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  delay(pause);

  // 3. Turn left a bit
  digitalWrite(in1, LOW);
  analogWrite(enA, speed);
  digitalWrite(in2, LOW);
  analogWrite(enB, speed);
  delay(turnTime);

  // Stop
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  delay(pause);

  // Move forward
  digitalWrite(in1, HIGH);
  analogWrite(enA, speed);
  digitalWrite(in2, LOW);
  analogWrite(enB, speed);
  delay(forwardTime);

  // Stop
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  delay(pause);

  // Move backward to original position
  digitalWrite(in1, LOW);
  analogWrite(enA, speed);
  digitalWrite(in2, HIGH);
  analogWrite(enB, speed);
  delay(forwardTime);

  // Stop
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  delay(pause);

  // Turn right to original heading
  digitalWrite(in1, HIGH);
  analogWrite(enA, speed);
  digitalWrite(in2, HIGH);
  analogWrite(enB, speed);
  delay(turnTime);

  // Stop
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  delay(pause);

  // 4. Spin in place
  digitalWrite(in1, HIGH);
  analogWrite(enA, speed);
  digitalWrite(in2, HIGH);
  analogWrite(enB, speed);
  delay(spinTime);

  // Stop
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  delay(1000); // Wait before repeating
}
