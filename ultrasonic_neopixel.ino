# Ultrasonic distance measurement virtualized with neopixels

#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUMPIXELS 60
long duration, dis;

// Running average variables
const int numReadings = 20;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int newValue = 0;
int lastpixel = 0;

int trigPin = 8;    //Trig - green Jumper
int echoPin = 7;    //Echo - yellow Jumper
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
 Serial.begin(115200);
 pixels.begin(); // This initializes the NeoPixel library.
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 // Reset running average array
 for (int thisReading = 0; thisReading < numReadings; thisReading++) {
 readings[thisReading] = 0;
 }
}
 
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // convert the time into a distance
  dis = (duration/2) / 29.1;
 
 // map distance to LED strip
 // map 20-110cm distance to LEDs #0 to #59
 // play with this value to match your LED strip
 int dispix = map(dis,18,95,0,60);
 
 // Ignore results out of bound, send -10 to average array instead
 // This leads to LED marker gliding to end of strip when no object is within distance
 if ((dispix >= 0) && (dispix < 60)) {
 newValue = dispix;
 } else {
 newValue = -10;
 }
 
 // Running average code from
 // https://www.arduino.cc/en/Tutorial/Smoothing
 // subtract the last reading:
 total = total - readings[readIndex];
 // read from the sensor:
 readings[readIndex] = newValue;
 // add the reading to the total:
 total = total + readings[readIndex];
 // advance to the next position in the array:
 readIndex = readIndex + 1;
 // if we're at the end of the array...
 if (readIndex >= numReadings) {
 // ...wrap around to the beginning:
 readIndex = 0;
 }
 // calculate the average:
 int avePos = total / numReadings;
 
 // set LEDs with following function and show them
 draw(avePos);
 pixels.show();
 
}
 
// Simple function resetting all pixels and lighten some around desired distance
void draw(int pix) {
 
 for (int i=0; i<60; i++) {
 pixels.setPixelColor(i, pixels.Color(2,0,0)); 
 } 
 pixels.setPixelColor(pix-2, pixels.Color(0,0,30)); 
 pixels.setPixelColor(pix-1, pixels.Color(0,0,100)); 
 pixels.setPixelColor(pix, pixels.Color(0,0,250)); 
 pixels.setPixelColor(pix+1, pixels.Color(50,0,50)); 
 pixels.setPixelColor(pix+2, pixels.Color(20,0,20)); 
}
