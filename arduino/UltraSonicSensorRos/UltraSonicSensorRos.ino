/*
  Ultrasonic Sensor HC-SR04 ROS Publisher

*/


#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>


ros::NodeHandle  nh;

sensor_msgs::Range range_msg;
ros::Publisher pub_range( "range_raw", &range_msg);

char frameid[] = "/ultrasound";

// defines pins numbers
const int trigPin = 9;
const int echoPin = 10;
// defines variables
long duration;
double distanceCm, distanceInch;

#define USE_ROS

void setup() {

#ifdef USE_ROS

  nh.initNode();
  nh.advertise(pub_range);

#else
  Serial.begin(115200); // Starts the serial communication
#endif
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

unsigned long publisher_timer;

void loop() {

  if ( millis() > publisher_timer ){

    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculating the distance
    //distanceCm = random(10, 200); //duration * 0.034 / 2;
    distanceCm = duration * 0.034 / 2;
    //distanceInch = duration * 0.0133 / 2;

#ifdef USE_ROS

      
    range_msg.range = distanceCm;
    range_msg.header.stamp = nh.now();
    pub_range.publish(&range_msg);
#else
    // Prints the distance on the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(distanceCm);

    //Serial.print("Distance (inch): ");
    //Serial.println(distanceInch);
#endif
    publisher_timer =  millis() + 1000;

  }
  nh.spinOnce();
}
