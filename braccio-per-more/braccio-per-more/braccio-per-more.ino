/* Sweep
  by BARRAGAN <http://barraganstudio.com>
  This example code is in the public domain.

  modified 28 May 2015
  by Michael C. Miller
  modified 8 Nov 2013
  by Scott Fitzgerald

  http://arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo[6];  // create servo object to control a servo
// twelve servo objects can be created on most boards

const int m_speed = 5;


static void test_pwm(int joint, int min_angle, int max_angle)
{
  int pos;

  for (pos=min_angle; pos<=max_angle; ++pos) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo[joint].write(pos);              // tell servo to go to position in variable 'pos'
    delay(m_speed);                       // waits 15ms for the servo to reach the position
  }
  for (pos=max_angle; pos>=min_angle; --pos) { // goes from 180 degrees to 0 degrees
    myservo[joint].write(pos);              // tell servo to go to position in variable 'pos'
    delay(m_speed);                       // waits 15ms for the servo to reach the position
  }
}




void setup() {
  for (int i=0; i<6; ++i) {
    myservo[i].attach(2+i);
    delay(500);
  }
}


void loop()
{
    for (int i=0; i<6; ++i) {
      myservo[i].write(60);              // tell servo to go to position in variable 'pos'
      delay(250);
    }

    delay(3000);

    for (int i=0; i<6; ++i) {
      myservo[i].write(120);              // tell servo to go to position in variable 'pos'
      delay(250);
    }

    delay(3000);

    for (int i=0; i<6; ++i) {
      myservo[i].write(90);              // tell servo to go to position in variable 'pos'
      delay(250);
    }

    delay(3000);

    return;
    
    for (int i=0; i<5; ++i) {
      myservo[i].write(180);              // tell servo to go to position in variable 'pos'
      delay(3000);
    }

    myservo[5].write(70);              // tell servo to go to position in variable 'pos'
    delay(3000);

    for (int i=0; i<6; ++i) {
      myservo[i].write(0);              // tell servo to go to position in variable 'pos'
      delay(3000);
    }

    return;
  
  test_pwm(0, 0, 180);
  test_pwm(1, 0, 180);
  test_pwm(2, 0, 180);
  test_pwm(3, 0, 180);
  test_pwm(4, 0, 180);
  test_pwm(5, 0, 70);

  for (int i=0; i<6; ++i) {
    myservo[i].write(0);              // tell servo to go to position in variable 'pos'
  }

  delay(5000);

}
