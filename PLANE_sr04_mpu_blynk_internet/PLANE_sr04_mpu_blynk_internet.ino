
// Fill-in information from your Blynk Template here
//#define BLYNK_TEMPLATE_ID           "TMPLxxxxxx"
//#define BLYNK_DEVICE_NAME           "Device"
#define BLYNK_TEMPLATE_ID "TMPL0vhaO0H1"
#define BLYNK_DEVICE_NAME "Plane"

#define BLYNK_FIRMWARE_VERSION  "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG
int X,Y,T,RS,PS,XA,YA;
#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#include <ESP8266WiFi.h>
#include<Wire.h>
#include <Servo.h>

 Servo pi;
 Servo ya;
 Servo l;
 Servo r;
 Servo m1;
 Servo m2;

const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 
int minVal=265;
int maxVal=402;
 
double x;
double y;
double z;

const int trigPin = 12; 
const int echoPin = 16; 

long duration;
int distance;
 
BLYNK_WRITE(V0)
{X=param.asInt();}
BLYNK_WRITE(V1)
{Y=param.asInt();}
BLYNK_WRITE(V2)
{T=param.asInt();}
BLYNK_WRITE(V3)
{XA=param.asInt();}
BLYNK_WRITE(V4)
{YA=param.asInt();}
BLYNK_WRITE(V5)
{X=param.asInt();}
BLYNK_WRITE(V6)
{RS=param.asInt();}
BLYNK_WRITE(V7)
{PS=param.asInt();}

void setup()
{
  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();
  pi.attach(0);
  l.attach(14);
  r.attach(2);

  m1.attach(13,1000,2000);
  m2.attach(15,1000,2000);
  
  m1.write(1000);
  m2.write(1000);
delay(7000);
Wire.begin();
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);

pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT);
  // TODO you setup code
}

void loop() {
  BlynkEdgent.run();

  m1.writeMicroseconds(map(T,0,100,1000,1680));
  m2.writeMicroseconds(map(T,0,100,1000,1680));
  pi.write(map(Y,-100,100,0,180));
  l.write(map(X,-100,100,0,180));
  r.write(180-map(X,-100,100,0,180));

  Blynk.virtualWrite(V4, y);
  Blynk.virtualWrite(V3, distance);
  
Wire.beginTransmission(MPU_addr);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true);
AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read();
int xAng = map(AcX,minVal,maxVal,-90,90);
int yAng = map(AcY,minVal,maxVal,-90,90);
int zAng = map(AcZ,minVal,maxVal,-90,90);
 
x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
 
Serial.print("AngleX= ");
Serial.println(x);
 
Serial.print("AngleY= ");
Serial.println(y);
 
Serial.print("AngleZ= ");
Serial.println(z);
Serial.println("-----------------------------------------");


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
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);


  Serial.print("\tX:\t");
  Serial.print(X);
  Serial.print("\t");

  Serial.print("Y:\t");
  Serial.print(Y);
  Serial.print("\t");

  Serial.print("T:\t");
  Serial.print(T);
  Serial.print("\t");

  Serial.print("XA:\t");
  Serial.print(XA);
  Serial.print("\t");

  Serial.print("YA:\t");
  Serial.print(YA);
  Serial.print("\t");

  Serial.print("X:\t");
  Serial.print(RS);
  Serial.print("\t");

  Serial.print("X:\t");
  Serial.print(PS);
  Serial.print("\t\n");
}
