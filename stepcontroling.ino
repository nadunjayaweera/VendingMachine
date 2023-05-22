String command;

#include <Servo.h>
Servo servo;   // DS04-NFC motors
Servo clutchservo;

#include <SoftwareSerial.h>
SoftwareSerial mySerial(12,13); //RX, TX D0->13 D1->12
int movepoint;
int res =5;
int IRSensor = 10;
// Stepper motors pins
#define dirPinVertical 2
#define stepPinVertical 3
#define dirPinHorizontal 4
#define stepPinHorizontal 5

#define microSwitchV 6
#define microSwitchH 7

int pos =0;
int sensorStatus;
void setup() {
  servo.attach(8);
  clutchservo.attach(9);

  Serial.begin(115200);
  mySerial.begin(9600);
  Serial.print("Welcome...");
  pinMode(dirPinVertical, OUTPUT);
  pinMode(stepPinVertical, OUTPUT);
  pinMode(dirPinHorizontal, OUTPUT);
  pinMode(stepPinHorizontal, OUTPUT);


  pinMode(microSwitchV, INPUT_PULLUP);
  pinMode(microSwitchH, INPUT_PULLUP);

  pinMode(IRSensor, INPUT);
  // Horizontal starting position
  digitalWrite(dirPinHorizontal, HIGH);
  while (true) {
    if (digitalRead(microSwitchH) == LOW) {
//      moveRight(70);
      break;
    }
    digitalWrite(stepPinHorizontal, LOW);
    delayMicroseconds(1500);
    digitalWrite(stepPinHorizontal, HIGH);
    delayMicroseconds(1500);
  }

   // Vertical starting position
  digitalWrite(dirPinVertical, HIGH); // Set the stepper to move in a particular direction
  while (true) {
    if (digitalRead(microSwitchV) == LOW) { // If the micro switch is pressed, move the platfor a little bit up and exit the while loop
      moveUp(70);
      moveRight(70);
      break;
    }
    // Move the carrier up until the micro switch is pressed
    digitalWrite(stepPinVertical, HIGH);
    delayMicroseconds(1500);
    digitalWrite(stepPinVertical, LOW);
    delayMicroseconds(1500);
  }
}

///////////////////////////////////////////////////////////////////////////////////
void moveUp (int steps) {
  digitalWrite(dirPinVertical, LOW);
  for (int x = 0; x < steps; x++) {
    digitalWrite(stepPinVertical, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPinVertical, LOW);
    delayMicroseconds(1000);
  }
}
void moveDown (int steps) {
  digitalWrite(dirPinVertical, HIGH);
  for (int x = 0; x < steps; x++) {
    digitalWrite(stepPinVertical, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPinVertical, LOW);
    delayMicroseconds(1000);
  }
}
void moveLeft (int steps) {
  digitalWrite(dirPinHorizontal, HIGH);
  for (int x = 0; x < steps; x++) {
    digitalWrite(stepPinHorizontal, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPinHorizontal, LOW);
    delayMicroseconds(1000);
  }
}
void moveRight (int steps) {
  digitalWrite(dirPinHorizontal, LOW);
  for (int x = 0; x < steps; x++) {
    digitalWrite(stepPinHorizontal, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPinHorizontal, LOW);
    delayMicroseconds(1000);
  }
}
/////////////////////////////////////////////////////////////////////////////////////
void loop() {
      if(mySerial.available()){
        movepoint = mySerial.read();
        Serial.print("The location is ");
        Serial.println(movepoint);
        if(movepoint==6){
          mySerial.write(res);
        }
      }
      switch(movepoint){
      // Move the container to location 1
      case 1:
      moveUp(3830); // Move up 4900 steps (Note: the stepper motor is set in Quarter set resolution)
      delay(200);
      moveRight(820); // Move left 1700 steps
      delay(300);
      // Rotate the helical coil, discharge the selected item
      for (pos =95; pos <=155; pos +=1){
      clutchservo.write(pos);
      delay(15);
      }
      sensorStatus = digitalRead(IRSensor);
      while(sensorStatus ==0){
      sensorStatus = digitalRead(IRSensor);
      servo.writeMicroseconds(135); // rotate
      }
      servo.writeMicroseconds(1500);  // stop
      delay(500);
      for (pos =155; pos >=95; pos -=1){
      clutchservo.write(pos);
      delay(15);
      }
      
      // Move the container back to starting position
      moveLeft(820);
      delay(200);
      moveDown(3830);
      mySerial.write(res);
      movepoint =0;
      break;
      
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
      // Move the container to location 2
      case 2:
      moveUp(40); // Move up 4900 steps (Note: the stepper motor is set in Quarter set resolution)
      delay(200);
      moveRight(820); // Move left 1700 steps
      delay(300);
      // Rotate the helical coil, discharge the selected item
      for (pos =95; pos <=155; pos +=1){
      clutchservo.write(pos);
      delay(15);
      }
      sensorStatus = digitalRead(IRSensor);
      while(sensorStatus ==0){
      sensorStatus = digitalRead(IRSensor);
      servo.writeMicroseconds(135); // rotate
      }
      servo.writeMicroseconds(1500);  // stop
      delay(500);
      for (pos =155; pos >=95; pos -=1){
      clutchservo.write(pos);
      delay(15);
      }
      // Move the container back to starting position
      moveLeft(820);
      delay(200);
      moveDown(40);
      mySerial.write(res);
      movepoint =0;
      break;

      // Move the container to location 3
      case 3:
      moveUp(3730); // Move up 4900 steps (Note: the stepper motor is set in Quarter set resolution)
      delay(200);
      moveRight(4070); // Move left 1700 steps
      delay(300);
      // Rotate the helical coil, discharge the selected item
      for (pos =95; pos <=155; pos +=1){
      clutchservo.write(pos);
      delay(15);
      }
      sensorStatus = digitalRead(IRSensor);
      while(sensorStatus ==0){
      sensorStatus = digitalRead(IRSensor);
      servo.writeMicroseconds(135); // rotate
      }
      servo.writeMicroseconds(1500);  // stop
      delay(500);
      for (pos =155; pos >=95; pos -=1){
      clutchservo.write(pos);
      delay(15);
      }
      
      // Move the container back to starting position
      moveLeft(4070);
      delay(200);
      moveDown(3730);
      mySerial.write(res);
      movepoint =0;
      break;
      

      // Move the container to location 4
      case 4:
      moveUp(40); // Move up 4900 steps (Note: the stepper motor is set in Quarter set resolution)
      delay(200);
      moveRight(4450); // Move left 1700 steps
      delay(300);
      // Rotate the helical coil, discharge the selected item
      for (pos =95; pos <=155; pos +=1){
      clutchservo.write(pos);
      delay(15);
      }
      sensorStatus = digitalRead(IRSensor);
      while(sensorStatus ==0){
      sensorStatus = digitalRead(IRSensor);
      servo.writeMicroseconds(135); // rotate
      }
      servo.writeMicroseconds(1500);  // stop
      delay(500);
      for (pos =155; pos >=95; pos -=1){
      clutchservo.write(pos);
      delay(15);
      }
      
      // Move the container back to starting position
      moveLeft(4450);
      delay(200);
      moveDown(40);
      mySerial.write(res);
      movepoint =0;
      break;
      

      }
  }
  
