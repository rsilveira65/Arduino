
#include <SoftwareSerial.h>
SoftwareSerial bt(5, 6); // RX  TX
int led = 9;
int led2 = 7;
int buzzer=2;
char c;


void setup() {                
  Serial.begin(19200);
  bt.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT); 
  pinMode(buzzer, OUTPUT); 
  digitalWrite(led2, HIGH);  
}


void loop() {
  if (bt.available()){                       
              c = bt.read();
              Serial.println(c);
              if (c=='l')  {digitalWrite(led, HIGH); digitalWrite(buzzer, HIGH);delay(1000);digitalWrite(buzzer, LOW); }
              if (c=='d') {digitalWrite(led, LOW); digitalWrite(buzzer, HIGH);delay(1000);digitalWrite(buzzer, LOW); }
              if (c=='b') {digitalWrite(buzzer, HIGH);delay(1000);digitalWrite(buzzer, LOW); }
     }
  if (Serial.available()>0){
     
      c = Serial.read();
      Serial.println(c);
      if (c=='l')  {digitalWrite(led, HIGH); digitalWrite(buzzer, HIGH);delay(1000);digitalWrite(buzzer, LOW); }
      if (c=='d') {digitalWrite(led, LOW); digitalWrite(buzzer, HIGH);delay(1000);digitalWrite(buzzer, LOW); }
      if (c=='b') {digitalWrite(buzzer, HIGH);delay(1000);digitalWrite(buzzer, LOW); }
   }
  

}
