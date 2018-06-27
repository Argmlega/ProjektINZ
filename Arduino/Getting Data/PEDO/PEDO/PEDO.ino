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
double X,x,Y,y,Z,z,xyz;
double thre = 2;
int stepcounter = 0;
int a = 0;

void setup(){
  Wire.begin();
  Serial.begin(115200);
  delay(200);
  
  Wire.beginTransmission(SensorAddress);
  Wire.write(PR);
  Wire.write(8);
  Wire.endTransmission();
}

void loop(){
  calc_Steps();
  if(a<stepcounter){
    a = stepcounter;
    Serial.println(a);
  }
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
