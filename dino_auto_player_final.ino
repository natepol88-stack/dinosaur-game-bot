#include <Servo.h>

Servo dinoJump;

const int sensorPin = A0;      
const int servoPin = 9;        

int sensorValue = 0;
float sensorVoltage = 0.0;
float thresholdVoltage = 3.5;  

const int restPosition = 90;    
const int pressPosition = 60;  


const float START_GAME_SPEED = 360.0;  
const float MAX_GAME_SPEED = 780.0;     
const float SPEED_ACCEL_PER_SEC = 3.6;  

unsigned long gameStartTime = 0;       

int extraDelayCoefficientMs = 150;      

float currentGameSpeed() {
  float elapsedSec = (millis() - gameStartTime) / 1000.0;
  float speed = START_GAME_SPEED + SPEED_ACCEL_PER_SEC * elapsedSec;
  if (speed > MAX_GAME_SPEED) speed = MAX_GAME_SPEED;
  return speed;
}

void setup() {
  Serial.begin(9600);
  dinoJump.attach(servoPin);
  dinoJump.write(restPosition);
  gameStartTime = millis();
}

void loop() {
  sensorValue = analogRead(sensorPin);
  sensorVoltage = sensorValue * (5.0 / 1023.0);


  if (sensorVoltage <= thresholdVoltage) {
    float speed = currentGameSpeed();
    int extraDelay = (int)(extraDelayCoefficientMs * (MAX_GAME_SPEED / speed - 1.0));
    if (extraDelay > 0) delay(extraDelay);

    dinoJump.write(pressPosition);
    delay(120);
    dinoJump.write(restPosition);
  }

  delay(5);
}
