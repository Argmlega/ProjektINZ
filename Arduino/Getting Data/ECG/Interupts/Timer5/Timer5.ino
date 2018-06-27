String url;
int a = 0;
void setup() {
  //Timer interrupt f at 200Hz setup
  cli(); //Stop interrupts
  TCCR5A = 0;
  TCCR5B = 0;
  TCNT5 = 0;
  //Compare match register value
  OCR5A = 1249;       //Value can be greater than 255 because Timer1 is a 16 bit timer,
  //Enable CTC mode   //this means that it can store a maximum counter value of 65535.
  TCCR5B |= (1 << WGM52);
  //Prescalar setup for 64
  TCCR5B |= (1 << CS50) | (1 << CS51);
  //Enable timer compare interrupt
  TIMSK5 |= (1 << OCIE5A);
  Serial.begin(115200);
  delay(500);
  sei(); //Allow interrupts
}

void loop() {
//  if (a==199){
//    a=0;
//    Serial.println(url);
//    url = "";
//  }
}

ISR(TIMER5_COMPA_vect) {
//  url += analogRead(A0);
//  if(a<198){
//    url += ",";
//  }
//  ++a;
  Serial.println(analogRead(A0));
}
