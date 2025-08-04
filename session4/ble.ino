#include <WString.h>
#include <HardwareSerial.h>

String G_Bluetooth_value;
volatile int BLE_Change_SPEED;
int maxSpeed = 60;
int turnSpeed = 50;
int speedStep = 10; 

String lastMovementCommand = "S"; 

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void executeMovement(String command, float value) {
  if (command == "F") {
    digitalWrite(2, HIGH);
    analogWrite(5, (BLE_Change_SPEED / 10) * 22.5);
    digitalWrite(4, LOW);
    analogWrite(6, (BLE_Change_SPEED / 10) * 22.5);
  } else if (command == "B") {
    digitalWrite(2, LOW);
    analogWrite(5, (BLE_Change_SPEED / 10) * 22.5);
    digitalWrite(4, HIGH);
    analogWrite(6, (BLE_Change_SPEED / 10) * 22.5);
  } else if (command == "L") {
    digitalWrite(2, LOW);
    analogWrite(5, (turnSpeed / 10) * 11.25);
    digitalWrite(4, LOW);
    analogWrite(6, (turnSpeed / 10) * 11.25);
  } else if (command == "R") {
    digitalWrite(2, HIGH);
    analogWrite(5, (turnSpeed / 10) * 11.25);
    digitalWrite(4, HIGH);
    analogWrite(6, (turnSpeed / 10) * 11.25);
  } else if (command == "FL") {
    digitalWrite(2, HIGH);
    analogWrite(5, (BLE_Change_SPEED / 10) * 11.25); 
    digitalWrite(4, LOW);
    analogWrite(6, (BLE_Change_SPEED / 10) * 22.5); 
  } else if (command == "FR") {
    digitalWrite(2, HIGH);
    analogWrite(5, (BLE_Change_SPEED / 10) * 22.5); 
    digitalWrite(4, LOW);
    analogWrite(6, (BLE_Change_SPEED / 10) * 11.25); 
  } else if (command == "BL") {
    digitalWrite(2, LOW);
    analogWrite(5, (BLE_Change_SPEED / 10) * 11.25); 
    digitalWrite(4, HIGH);
    analogWrite(6, (BLE_Change_SPEED / 10) * 22.5); 
  } else if (command == "BR") {
    digitalWrite(2, LOW);
    analogWrite(5, (BLE_Change_SPEED / 10) * 22.5); 
    digitalWrite(4, HIGH);
    analogWrite(6, (BLE_Change_SPEED / 10) * 11.25); 
  } else if (command == "S") {
    digitalWrite(2, LOW);
    analogWrite(5, 0);
    digitalWrite(4, LOW);
    analogWrite(6, 0);
  } 
}

void setup(){
  Serial.begin(9600);
  G_Bluetooth_value = "";
  BLE_Change_SPEED = 0;
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop(){
  while (Serial.available() > 0) {
    G_Bluetooth_value = G_Bluetooth_value + ((char)(Serial.read()));
    delay(2);
  }
  if (G_Bluetooth_value.length() > 0) {
    String cmd = G_Bluetooth_value;
    float value = (BLE_Change_SPEED / 10) * 15;

    // Check for BLE settings commands
    if (cmd.startsWith("MAX:")) {
      int val = cmd.substring(4).toInt();
      if (val >= 0 && val <= 255) maxSpeed = val;
      G_Bluetooth_value = "";
      return;
    } else if (cmd.startsWith("TURN:")) {
      int val = cmd.substring(5).toInt();
      if (val >= 0 && val <= 100) turnSpeed = val;
      G_Bluetooth_value = "";
      return;
    } else if (cmd.startsWith("STEP:")) {
      int val = cmd.substring(5).toInt();
      if (val > 0 && val <= 100) speedStep = val;
      G_Bluetooth_value = "";
      return;
    }

    // Check for 2-char commands (diagonals)
    String movementCmd = "";
    if (cmd.startsWith("FL") || cmd.startsWith("FR") || cmd.startsWith("BL") || cmd.startsWith("BR")) {
      movementCmd = cmd.substring(0, 2);
    } else if (cmd.startsWith("F") || cmd.startsWith("B") || cmd.startsWith("L") || cmd.startsWith("R") || cmd.startsWith("S")) {
      movementCmd = cmd.substring(0, 1);
    }

    if (movementCmd.length() > 0) {
      lastMovementCommand = movementCmd;
      executeMovement(movementCmd, value);
    } else {
      char command = cmd.charAt(0);
      switch (command) {
        case '+': // Increase speed
          BLE_Change_SPEED += speedStep;
          if (BLE_Change_SPEED > maxSpeed) BLE_Change_SPEED = maxSpeed;
          Serial.println(BLE_Change_SPEED);
          executeMovement(lastMovementCommand, (BLE_Change_SPEED / 10) * 15);
          break;
        case '-': // Decrease speed
          BLE_Change_SPEED -= speedStep;
          if (BLE_Change_SPEED < 0) BLE_Change_SPEED = 0;
          Serial.println(BLE_Change_SPEED);
          executeMovement(lastMovementCommand, (BLE_Change_SPEED / 10) * 15);
          break;
        case '/': 
          BLE_Change_SPEED = 0;
          Serial.println(BLE_Change_SPEED);
          executeMovement(lastMovementCommand, (BLE_Change_SPEED / 10) * 15);
          break;
        default:
          Serial.println("Invalid Command");
          break;
      }
    }
    G_Bluetooth_value = "";
  }
}