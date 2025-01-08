#include <NewPing.h>
#include <Servo.h>
#include <AFMotor.h>

#define TRIGGER_PIN A2
#define ECHO_PIN A3
#define MAX_DISTANCE 50

// IR sensor pins
#define IR_LEFT A0
#define IR_RIGHT A1

// Motor speed
#define DEFAULT_SPEED 180

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;

int leftDistance = 0;
int rightDistance = 0;

void setup() {
  Serial.begin(9600);
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
  myservo.attach(10);
  myservo.write(90);

  motor1.setSpeed(DEFAULT_SPEED);
  motor2.setSpeed(DEFAULT_SPEED);
  motor3.setSpeed(DEFAULT_SPEED);
  motor4.setSpeed(DEFAULT_SPEED);
}

void loop() {
  bool irLeft = digitalRead(IR_LEFT);
  bool irRight = digitalRead(IR_RIGHT);

  if (!irLeft && !irRight) {
    objectAvoid();
  } else if (!irLeft && irRight) {
    Serial.println("Turning Left");
    moveLeft();
  } else if (irLeft && !irRight) {
    Serial.println("Turning Right");
    moveRight();
  } else {
    Serial.println("Stopping");
    Stop();
  }
}

void objectAvoid() {
  int distance = getDistance();
  if (distance <= 15) {
    Stop();
    Serial.println("Object Detected! Deciding Turn Direction...");

    leftDistance = lookLeft();
    rightDistance = lookRight();

    if (rightDistance > leftDistance) {
      Serial.println("Turning Right");
      moveRight();
    } else {
      Serial.println("Turning Left");
      moveLeft();
    }
  } else {
    Serial.println("Moving Forward");
    moveForward();
  }
}

int getDistance() {
  delay(50);
  int cm = sonar.ping_cm();
  return cm == 0 ? 100 : cm;
}

int lookLeft() {
  myservo.write(150);
  delay(500);
  int distance = getDistance();
  myservo.write(90);
  Serial.print("Left Distance: ");
  Serial.println(distance);
  return distance;
}

int lookRight() {
  myservo.write(30);
  delay(500);
  int distance = getDistance();
  myservo.write(90);
  Serial.print("Right Distance: ");
  Serial.println(distance);
  return distance;
}

void Stop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void moveForward() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void moveLeft() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void moveRight() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}
