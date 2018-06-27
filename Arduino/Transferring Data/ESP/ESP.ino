#include <ESP8266WiFi.h>
 
const char* ssid     = "UPC198FB2A";
const char* password = "1W2P3B4i5a6j";
const int httpPort = 80;
const char* host = "192.168.0.38";
String url = "/collectdata.php";
String data = "ecg=";
//String readString;


char receivedChar;
boolean newData = false;

void setup(){
  Serial.begin(115200);
  delay(10);
  
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");  
}

void loop(){
  /*if (Serial.available()){
    char c = Serial.read();  //gets one byte from serial buffer
    if (c == ','){
      //do stuff
      Serial.println(readString);
      sendRequest(readString);
      readString="";    
     }  
    else {     
      readString += c; //makes the string readString
    }
  }*/
  recvOneChar();
  showNewData();
}

void sendRequest(char dat){
  WiFiClient client;
  if (!client.connect(host, httpPort)){
    Serial.println("Connection to server failed.");
    return;
  }
  Serial.println("Connected to server.");
  data += dat;
  Serial.println();
  Serial.println(data);
  
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
  "Host: " + host + "\r\n" +
  "Accept: *" + "/" +"*\r\n" +
  "Content-Length: " + data.length() + "\r\n" +
  "Content-Type: application/x-www-form-urlencoded\r\n" +
  "\r\n" + data);
}

void recvOneChar(){
  if (Serial.available() > 0) {
    receivedChar = Serial.read();
    newData = true;
  }
}

void showNewData(){
  if (newData == true) {
    Serial.print("This just in ... ");
    Serial.println(receivedChar);
    sendRequest(receivedChar);
  newData = false;
  }
}
