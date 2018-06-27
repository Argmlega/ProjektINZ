#include <Wire.h>

#define DX0 0x32
#define DX1 0x33
#define DY0 0x34
#define DY1 0x35
#define DZ0 0x36
#define DZ1 0x37
#define PR 0x2D
int SensorAddress = 0x53;
int X0,X1,XT,Y0,Y1,YT,Z0,Z1,ZT;
int a = 0;
int b = 0;
double X,x,Y,y,Z,z,xyz;
double thre = 2;
int stepcounter = 0;
String url;

void setup(){
  //Timer interrupt f at 200Hz setup
  cli(); //Stop interrupts
  TCCR5A = 0;
  TCCR5B = 0;
  TCNT5 = 0;
  //Compare match register value
  OCR5A = 1249;       //Value can be greater than 255 because Timer3 is a 16 bit timer,
  //Enable CTC mode   //this means that it can store a maximum counter value of 65535.
  TCCR5B |= (1 << WGM52);
  //Prescalar setup for 64
  TCCR5B |= (1 << CS50) | (1 << CS51);
  sei(); //Allow interrupts
  Serial.begin(115200);
  delay(500);
  //Initiating wire library.
  Wire.begin();
  //Sending the adress off the "slave" device data is being sent. 
  Wire.beginTransmission(SensorAddress);
  //Sending the internal register adress of the slave device.
  Wire.write(PR);
  //Sending the data to be written to the internal register.
  Wire.write(8);
  //Stoping the transmission.
  Wire.endTransmission();
  cli(); //Stop interrupts
  //Enable timer compare interrupt
  TIMSK5 |= (1 << OCIE5A);
  sei(); //Allow interrupts
}

void loop(){
  calc_Steps();
  if(a<stepcounter){
    a = stepcounter;
    Serial.println(a);
  }
  if(b==199){
    Serial.println(url);
    url = "";
    b = 0;
  }
}

ISR(TIMER5_COMPA_vect) {
  url += analogRead(A0);
  if(b<198){
    url += ","; 
  }
  ++b;
}

void calc_Steps(){
  x = calc_X();
  y = calc_Y();
  z = calc_Z();
  x = sq(x);
  y = sq(y);
  z = sq(z);
  xyz = x + y + z;
  if(xyz>=thre){
    ++stepcounter;
    delay(300);
  }
}
double calc_X(){
  Wire.beginTransmission(SensorAddress);
  Wire.write(DX0);
  Wire.write(DX1);
  Wire.endTransmission();
  
  Wire.requestFrom(SensorAddress,2);
  if(Wire.available()<=2){
    X0 = Wire.read();
    X1 = Wire.read();
    X1 = X1 << 8;
    XT = X0 + X1;
    X = XT/256.0;
    return X;
  }
}

double calc_Y(){
  Wire.beginTransmission(SensorAddress);
  Wire.write(DY0);
  Wire.write(DY1);
  Wire.endTransmission();
  
  Wire.requestFrom(SensorAddress,2);
  if(Wire.available()<=2){
    Y0 = Wire.read();
    Y1 = Wire.read();
    Y1 = Y1 << 8;
    YT = Y0 + Y1;
    Y = YT/256.0;
    return Y;
  }
}

double calc_Z(){
  Wire.beginTransmission(SensorAddress);
  Wire.write(DZ0);
  Wire.write(DZ1);
  Wire.endTransmission();
  
  Wire.requestFrom(SensorAddress,2);
  if(Wire.available()<=2){
    Z0 = Wire.read();
    Z1 = Wire.read();
    Z1 = Z1 << 8;
    ZT = Z0 + X1;
    Z = ZT/256.0;
    return Z;
  }
}
