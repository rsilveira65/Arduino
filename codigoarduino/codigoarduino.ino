#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_PCD8544.h>

SoftwareSerial mySerial(2, 3); // RX, TX
SoftwareSerial mySerial2(4, 5);

int ledCheck = 13;
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);

void info() {
  digitalWrite(ledCheck, HIGH);
  delay(30);
  digitalWrite(ledCheck, LOW);
  delay(30);
}

void setup() {
  mySerial.begin(9600);
  mySerial2.begin(19200);
  Serial.begin(9600);
  pinMode(ledCheck, OUTPUT);
   Serial.begin(9600);
  display.begin();
  display.setContrast(50); //Ajusta o contraste do display
  display.clearDisplay();   //Apaga o buffer e o display
  display.setTextSize(1);  //Seta o tamanho do texto
  display.drawRect(0,0, 84,48, BLACK); //Desenha o retangulo da borda
  display.drawRect(1,1, 82,46, BLACK); //Desenha o retangulo da borda
   display.setCursor(0,20);  //Seta a posição do cursor
  display.setTextColor(WHITE, BLACK); 
  display.setTextSize(1); 
  display.println("APROXIME O CARTAO!!");

  display.display();
  delay(2000);
  
}

void loop () {
  byte i = 0;
  byte val = 0;
  byte code[6];
  byte checksum = 0;
  byte bytesread = 0;
  byte tempbyte = 0;

  if(mySerial.available() > 0) {
    info();
    if((val = mySerial.read()) == 2) {                  // check for header 
      info();
      bytesread = 0; 
      while (bytesread < 12) {                        // read 10 digit code + 2 digit checksum
        if( mySerial.available() > 0) { 
          val = mySerial.read();
          info();
          if((val == 0x0D)||(val == 0x0A)||(val == 0x03)||(val == 0x02)) { // if header or stop bytes before the 10 digit reading 
            break;                                    // stop reading
          }

          // Do Ascii/Hex conversion:
          if ((val >= '0') && (val <= '9')) {
            val = val - '0';
          } 
          else if ((val >= 'A') && (val <= 'F')) {
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
          } 
          else {
            tempbyte = val;                           // Store the first hex digit first...
          };

          bytesread++;                                // ready to read next digit
        } 
      } 

      // Output to Serial:

      if (bytesread == 12) {                          // if 12 digit read is complete
        Serial.print("ID:");
         display.setTextColor(WHITE, BLACK); 
        display.setTextSize(2);
        display.setTextColor(BLACK);
        display.clearDisplay();  
        display.println("ID:");
     
        display.display();
        for (i=0; i<5; i++) {
          if (code[i] < 16) Serial.print("0");
          Serial.print(code[i], HEX);
          Serial.print(" ");
               
        display.setTextSize(2); 
        display.setTextColor(BLACK);
        display.print(code[i], HEX); 
        display.display();
        
        }
        Serial.println();
        mySerial2.println("teste");
        delay(6000);
        display.clearDisplay();  
        display.setTextColor(WHITE, BLACK); 
        display.setTextSize(1);
        display.println("APROXIME O CARTAO!");
        display.display();

        //Serial.print("Checksum: ");
        //Serial.print(code[5], HEX);
        //Serial.println(code[5] == checksum ? " -- passed." : " -- error.");
        //Serial.println();
      }

      bytesread = 0;

    }

  }
  
}

