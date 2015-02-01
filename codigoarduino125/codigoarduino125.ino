#include <LiquidCrystal.h> 
#include <SoftwareSerial.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int ledverde=A0;
int ledvermelho=A1;
char val2;
SoftwareSerial mySerial(6, 7);
char inData[20]; // Allocate some space for the string
char inChar; // Where to store the character read
byte index = 0; // Index into array; where to store the character
void setup() {
  mySerial.begin(9600);
  Serial.begin(19200);   
  pinMode(ledverde, OUTPUT);   
  pinMode(ledvermelho, OUTPUT);  
  digitalWrite(ledverde,1);
  lcd.begin(16, 2); 
  lcd.setCursor(0, 0); 
        lcd.print("SISGE|BEM VINDO!"); 
        lcd.setCursor(0, 1);
        lcd.print("rsilveira.info");
}

void loop () {
  byte i = 0;
  byte val = 0;
  byte code[6];
  byte checksum = 0;
  byte bytesread = 0;
  byte tempbyte = 0;
  while(Serial.available()> 0){    
    
      if(index < 19) // One less than the size of the array
      {
          inChar = Serial.read(); // Read a character
          inData[index] = inChar; // Store it
          index++; // Increment where to write next
          inData[index] = '\0'; // Null terminate the string
      }
        lcd.clear();
        lcd.setCursor(0, 0); 
        lcd.print("ID CARTAO:"); 
        lcd.setCursor(0, 1);
        lcd.print(inData);
  }

 
        
  
  if(mySerial.available() > 0) {
    if((val = mySerial.read()) == 2) {                  // check for header 
      bytesread = 0; 
      while (bytesread < 12) {                        // read 10 digit code + 2 digit checksum
        if( mySerial.available() > 0) { 
          val = mySerial.read();
          if((val == 0x0D)||(val == 0x0A)||(val == 0x03)||(val == 0x02)) { // if header or stop bytes before the 10 digit reading 
            break;                                    // stop reading
          }

          // Do Ascii/Hex conversion:
          if ((val >= '0') && (val <= '9')) {
            val = val - '0';
          } else if ((val >= 'A') && (val <= 'F')) {
            val = 10 + val - 'A';
          }

          // Every two hex-digits, add byte to code:
          if (bytesread & 1 == 1) {
            // make some space for this hex-digit by
            // shifting the previous hex-digit with 4 bits to the left:
            code[bytesread >> 1] = (val | (tempbyte << 4));

            if (bytesread >> 1 != 5) {                // If we're at the checksum byte,
              checksum ^= code[bytesread >> 1];       // Calculate the checksum... (XOR)
            };
          } else {
            tempbyte = val;                           // Store the first hex digit first...
          };

          bytesread++;                                // ready to read next digit
        } 
      } 

      // Output to Serial:
       String conteudo= "";
      if (bytesread == 12) {                          // if 12 digit read is complete
        //Serial.print("5-byte code: ");
        for (i=0; i<5; i++) {
          if (code[i] < 16) conteudo.concat(String("0"));
         
         // Serial.print(code[i], HEX);
          conteudo.concat(String(code[i], HEX));
          //Serial.print(" ");
        }
        Serial.println();
        conteudo.toUpperCase();
        lcd.clear();
        lcd.setCursor(0, 0); 
        lcd.print("ID CARTAO:"); 
        lcd.setCursor(0, 1);
        Serial.println(conteudo);
        lcd.print(conteudo);
        digitalWrite(ledverde,0);
        digitalWrite(ledvermelho,1);
        delay(5000);
         digitalWrite(ledverde,1);
        digitalWrite(ledvermelho,0);
        lcd.clear();
        lcd.setCursor(0, 0); 
        lcd.print("SISGE|BEM VINDO!"); 
        lcd.setCursor(0, 1);
        lcd.print("rsilveira.info");
        

        //Serial.print("Checksum: ");
        //Serial.print(code[5], HEX);
        //conteudo.concat(String(code[5], HEX));
        //Serial.println(code[5] == checksum ? " -- passed." : " -- error.");
        //Serial.println();
      }

      bytesread = 0;
    }
  }
}
