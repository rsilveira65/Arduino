//Teste de exibição e formatação de texto no Módulo Display Nokia 5110
//www.portaldoarduino.com.br
#include <Adafruit_GFX.h>            //incluindo a biblioteca gráfica
#include <Adafruit_PCD8544.h>        //incluindo a biblioteca principal

// pin 8 - Serial clock out (SCLK)
// pin 9 - Serial data out (DIN)
// pin 10 - Data/Command select (D/C)
// pin 11 - LCD chip select (CS/CE)
// pin 12 - LCD reset (RST)
int positivo =3;
//definindo os pinos
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);

void setup()   {  
  Serial.begin(9600); // iniciando a porta seria com uma taxa de 9600 baude rate

  display.begin();
  pinMode(positivo,OUTPUT);
  digitalWrite(positivo,1);
  display.setContrast(15); //Ajusta o contraste do display , este valor pode não ser o ideal para seu display, faça vários teste até encontrar a melhor configuração
  display.clearDisplay();   //Apaga o buffer e o display
  
  display.setTextSize(1);  //Seta o tamanho do texto
  display.println("portal"); // exibe a palavra 'portal' no display  
  
  display.setTextSize(3);  //Seta o tamanho do texto
  display.setCursor(25, 5);  //Seta a posição do cursor, o primeiro número é em relação a posição na horizontal (eixo X) e o segunto a vertical (eixo y)
  display.println("do");  
  
  display.setTextSize(1);  //Seta o tamanho do texto
  display.setTextColor(WHITE, BLACK); //Inverte as cores do texto, o funco fica preto, e as letras ficam brancas
  display.setCursor( 42, 28);  //Seta a posição do cursor
  display.println("arduino");  
  
  display.setTextSize(2);  //Seta o tamanho do texto
  display.setTextColor(BLACK, WHITE); //Voltando com o texto para o padrão (fundo branco, texto preto), sem essa linha o texto abaixo também seria exibido invertido
  display.setCursor( 0, 35);  //Seta a posição do cursor
  display.println(".com.br");
  display.display();  
}
void loop()
{
 }
