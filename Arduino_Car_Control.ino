/*Arduino Car Controller
 *
 * The following program will allow the user to run a car equipped with an arduino, motor shield, and onesheeld to
  drive the car using their android smartphone. 
  It makes use of the Adafruit Motor Shield Library. 
  The motor.run() command sets the motor to move either FORWARD,BACKWARD, or RELEASE.
  The motor.setSpeed(int) command sets the speed of the motor, and the int is a value between 0 and 255

  The OneSheeld app is used to control the car. Scan and select your OneSheeld to begin.
  
  The accelerometer x value is used to decide how fast the car moves forward or backwards, while the y value decides the turning.
*/
#define CUSTOM_SETTINGS
#define INCLUDE_ACCELEROMETER_SENSOR_SHIELD

#include <OneSheeld.h>
#include <AFMotor.h>

//initialize the motors
AF_DCMotor motor4(4);
AF_DCMotor motor3(3);

const int FWD_THRESHOLD = 5; //Threshold for max speed forward. If X is more than this, then it should be full speed forward.
const int TURN_THRESHOLD = 7; //Threshold for full turn. If Y is more than this, then it should be a full, sharp, turn.
const int MAX = 255; //Max Speed

int mSpeed = MAX; //mSpeed is motor speed, and it is a number that will always be between 0 - 255, indicating the speed of the motor.
int slow = 0;  //The slow variable serves as an offset of the mSpeed variable. A larger slow variable means the speed of the motor will be slower.

void setup() {
  OneSheeld.begin();
  motor3.run(RELEASE);
  motor4.run(RELEASE);

}

void loop() {

  float x = AccelerometerSensor.getX();
  float y = AccelerometerSensor.getY();

  if (x >= 1) //If x >= 1 that means that the car should move FORWARD
  {
    motor3.run(FORWARD);
    motor4.run(FORWARD); //Set both motors to move foward

    if ( y > -1 && y < 1) //No turn. If y is in the middle, that means that the phone is not tilted enough to make a turn.
      forward(x);

    if (y >= 1) //right turn. Phone is tilted right.
      turnRight(y);

    if (y <= -1) //left turn. Phone is tilted left.
      turnLeft(y);

  }
  else if (x <= -1) //BACKWARD
  {

    motor3.run(BACKWARD);
    motor4.run(BACKWARD); //Set both motors to move backward

    if (y > -1 && y < 1) //no turn
      backward(x);

    if (y >= 1) //right turn
      turnRight(y);

    if (y <= -1) //left turn
      turnLeft(y);
  }

  else //HALT
  {
    motor3.run(RELEASE);  //Stops both motors
    motor4.run(RELEASE);
  }

}


void turnLeft(int yVal) {

  if (yVal <= -TURN_THRESHOLD)   //if y <= -7, make the car turn sharply left
    slow = MAX;
  else                          //else, turn it based on how far the phone is tilted left. Allows gentle turns.
    slow = map(-yVal, 1, TURN_THRESHOLD, 0, MAX);  

  motor3.setSpeed(mSpeed);
  motor4.setSpeed(mSpeed - slow);

}

void turnRight(int yVal) {

  if (yVal >= TURN_THRESHOLD)   //if y >= 7, make the car turn sharply right
    slow = MAX;
  else                          //else, turn it based on how far the phone is tilted right.
    slow = map(yVal, 1, TURN_THRESHOLD, 0, MAX);

  motor3.setSpeed(mSpeed - slow);
  motor4.setSpeed(mSpeed);

}

void forward(int xVal) {

  if (xVal >= FWD_THRESHOLD)   //if x >= 5, full speed
    mSpeed = MAX;
  else                         //else, change the speed depending on how far the phone is tilted forward
    mSpeed = map(xVal, 1, FWD_THRESHOLD, 0, MAX);

  motor3.setSpeed(mSpeed);
  motor4.setSpeed(mSpeed);

}

void backward(int xVal) {
  if (xVal <= -FWD_THRESHOLD) //if x <= -5, full speed backward
    mSpeed = MAX;
  else                        //else, change the speed depending on how far the phone is tilted backward
    mSpeed = map(-xVal, 1, FWD_THRESHOLD, 0, MAX);

  motor3.setSpeed(mSpeed);
  motor4.setSpeed(mSpeed);
}


