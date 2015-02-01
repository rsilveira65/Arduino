	
//Programa : Teste display Nokia 5110 usando biblioteca Adafruit
//Autor : Arduino e Cia

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SPI.h>

int positivo =8;

// pin 8 - Serial clock out (SCLK)
// pin 9 - Serial data out (DIN)
// pin 10 - Data/Command select (D/C)
// pin 11 - LCD chip select (CS/CE)
// pin 12 - LCD reset (RST)

//Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);
//Adafruit_PCD8544 display = Adafruit_PCD8544(0, 1, 5, 6, 7);
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, A0);

void setup()   
{
  Serial.begin(9600);
  pinMode(positivo,OUTPUT);
  digitalWrite(positivo,1);
  display.begin();
  display.setContrast(60); //Ajusta o contraste do display
  display.clearDisplay();   //Apaga o buffer e o display
  display.setTextSize(1);  //Seta o tamanho do texto
  display.setTextColor(BLACK); //Seta a cor do texto
  display.setCursor(0,0);  //Seta a posição do cursor
  display.println("Micro 2014/2");  

  //Texto invertido - Branco com fundo preto
  display.setTextColor(WHITE, BLACK); 

  display.println("Participante:");

  display.setTextSize(2); 
  display.setTextColor(BLACK);
  display.print("Rafael Silveira");
  display.display();
  delay(2000);
}

void loop()
{
  // Codigo adicional
}
