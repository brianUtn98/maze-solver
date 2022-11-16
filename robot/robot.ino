#include <Arduino.h>

// 0 - 13 Pines digitales

int motorForwardLeft = 9;
int motorBackwardLeft = 10;
int motorEnableForward = 11;

int motorForwardRight = 3;
int motorBackwardRight = 4;
int motorEnableBackward = 5;

// A0 - A5 Pines analogicos
int frontTrigger = A0;
int frontEcho = A1;

int leftTrigger = A2;
int leftEcho = A3;

int rightTrigger = A4;
int rightEcho = A5;

void setup(){
  setupForwardMotor();

  setupBackwardMotor();

  setupTriggers();
}

void loop(){
  Serial.begin(9600);
  long frontDistance, leftDistance, rightDistance;

  // Calcular distanciad

  // Distancia frontal - Conversión según sensor
  frontDistance = getDistance(frontTrigger, frontEcho);

  // Distancia izquierda - Conversión según sensor
  leftDistance = getDistance(leftTrigger, leftEcho);

  // Distancia derecha - Conversión según sensor
  rightDistance = getDistance(rightTrigger, rightEcho);

  // DEBUG
  Serial.print("front = ");
  Serial.println(frontDistance);
  Serial.print("Left = ");
  Serial.println(leftDistance);
  Serial.print("Right = ");
  Serial.println(rightDistance);
  delay(50);

  if (leftDistance > 20){
    Stop();
    delay(1000);
    left();
    delay(400);
    forward();
  } else if (frontDistance > 20){
    if (isBetween(leftDistance, 0, 10)){
      right();
      delay(30);
      forward();
    } else if (isBetween(rightDistance, 0, 10)){
      left();
      delay(30);
      forward();
    } else {
      forward();
    }
  } else if (rightDistance > 20) {
    Stop();
    delay(1000);
    right();
    delay(400);
    forward();
  } else {
    Stop();
    delay(1000);
    right();
    delay(800);
    forward();
  }
}

/*
------------------------------------------------ Setup ------------------------------------------------
*/

void setupForwardMotor(){
  pinMode(motorForwardLeft, OUTPUT);
  pinMode(motorForwardRight, OUTPUT);
  pinMode(motorEnableForward, OUTPUT);
  analogWrite(motorEnableForward, 80);
}

void setupBackwardMotor(){
  pinMode(motorBackwardLeft, OUTPUT);
  pinMode(motorBackwardRight, OUTPUT);
  pinMode(motorEnableBackward, OUTPUT);
  analogWrite(motorEnableBackward, 88);
}

void setupTriggers(){
  pinMode(frontTrigger, OUTPUT);
  pinMode(frontEcho, INPUT);

  pinMode(leftTrigger, OUTPUT);
  pinMode(leftEcho, INPUT);

  pinMode(rightTrigger, OUTPUT);
  pinMode(rightEcho, INPUT);
}

/*
------------------------------------------------ Movimiento ------------------------------------------------
*/

void forward(){
  digitalWrite(motorForwardLeft, 1);
  digitalWrite(motorBackwardLeft, 0);
  digitalWrite(motorForwardRight, 1);
  digitalWrite(motorBackwardRight, 0);
  delay(1000);
}

void right(){
  digitalWrite(motorForwardLeft, 1);
  digitalWrite(motorBackwardLeft, 0);
  digitalWrite(motorForwardRight, 0);
  digitalWrite(motorBackwardRight, 1);
  delay(10);
}

void left(){
  digitalWrite(motorForwardLeft, 0);
  digitalWrite(motorBackwardLeft, 1);
  digitalWrite(motorForwardRight, 1);
  digitalWrite(motorBackwardRight, 0);
  delay(10);
}

void Stop(){
  digitalWrite(motorForwardLeft, 0);
  digitalWrite(motorBackwardLeft, 0);
  digitalWrite(motorForwardRight, 0);
  digitalWrite(motorBackwardRight, 0);
  delay(300);
}

/*
------------------------------------------------ Distancia ------------------------------------------------
*/

long getDistance(int trigger, int echo){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  long duration = pulseIn(echo, HIGH);

  return duration * 0.034 / 2;
}

/*
--------------------------------------------- Funciones útiles ---------------------------------------------
*/

int isBetween(long value, long min, long max){
  return value >= min && value <= max;
}