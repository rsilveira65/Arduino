/***************************************************\
|            TRABALHO DE PC-II /  MICRO             |
| Componentes:                                      |
| Rafael Silveira, Eduardo, Lúcio, Naigon e Júlia   |                                                
|                                                   |
|                                                   |
\***************************************************/

/*
  
 * Ethernet shield pinos 10, 11, 12, 13
 * LCD pinos(7, 6, 5, 4, 3, 2)
 * BUZZER A2
 * SENSOR CHUVA 1, A0
 * SENSOR TEMP/UMIDADE A1
 * BT  9,8


 ___
|( )|  PIN 1 = PWM PIN 9
|___|  PIN 2 = GND
| | |  PIN 3 = 3V3
| | |
1 2 3

*/

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <IRremote.h>
#include <dht11.h>


#define dht_dpin A1 //Pino 2 Data do sensor vai ligado a porta A1 do Arduino

int SensorNivel = A3;
dht11 DHT; //Inicialização do sensor
 
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); //com ethernet
//------------------------------------------------------------
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "www.silveira.96.lt";    
IPAddress ip(192,168,0,177);
EthernetClient client;
char c;
char val='j';
int nivel;
long randNumber;
SoftwareSerial bt(8, 9); // RX  TX
int intensidadeChuva=0;
//------------------------------------------------------------
byte termometro[8] = {B00100, B01010, B01010, B01110, B01110, B11111, B11111, B01110};
byte bolinha[8] = {B01100, B10010, B10010, B01100, B00000, B00000, B00000, B00000};
int buzzer = A2;
int pinoSensor = A2;
//int azul =A3;
int vermelho = A4;
int verde = A5;
int valorLido = 0;
long tempo = 60000; // 1min default
long tempo2=5000;   // 5seg default
long previousMillis = 0;
long previousMillis2 = 0;
int valor = 0;
int lado=1;
float temperatura = 0;
int sleep=0;
int n=1;
int mute =1;
//------------------------------------------------------------
int RECV_PIN = 9;  
float armazenavalor;  
IRrecv irrecv(RECV_PIN);  
decode_results results; 
//------------------------------------------------------------
int pino_d = 1; //Pino ligado ao D0 do sensor chuva
int pino_a = A0; //Pino ligado ao A0 do sensor chuva fio verde
int val_d = 0; //Armazena o valor lido do pino digital
int val_a = 0; //Armazena o valor lido do pino analogico

void setup() {
       Serial.begin(19200);
       bt.begin(9600);
       randomSeed(analogRead(0));
       Serial.println("_______________________________");
       Serial.println("  CONFIGURACOES:  ");
       Serial.println("Enviar temperatura - t");
       Serial.println("Tempo++ : +");
       Serial.println("Tempo-- : -");
       Serial.println("Menu  :  <  e  >");
       Serial.println("_______________________________");
       
       lcd.createChar(2,termometro);
       pinMode(pino_d, INPUT);
       pinMode(pino_a, INPUT);
       lcd.createChar(1,bolinha);
       lcd.begin(16, 2);
       irrecv.enableIRIn(); // Inicializa o receptor IR
       pinMode(buzzer, OUTPUT);
      // pinMode(azul, OUTPUT);
       pinMode(verde, OUTPUT);
       pinMode(vermelho, OUTPUT);
       //digitalWrite(azul, HIGH);
       digitalWrite(buzzer, HIGH);
       delay(10);
       digitalWrite(buzzer, LOW);
       lcd.clear();
       lcd.print("ESTACAO METEOROLOGICA ONLINE");
      
       for (int i = 0; i < 26; i++) {
          lcd.scrollDisplayLeft();
          delay(100);    
      }
      lcd.clear();
      lcd.print("----|E M O|----");
     
}

void loop(){       
  unsigned long currentMillis = millis();   // a cada iteração conta milisegundos
  unsigned long currentMillis2 = millis();
  
   /*if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }*/
  /*if (Serial.available()>0){
      c = Serial.read();
      Menu();     
   }*/
  if (bt.available()){                        //le BT armazena na val, e chama menu
              val = bt.read();
              Menu();
  }
              
              
 /* if (irrecv.decode(&results)){ 
    
       // Serial.print("Valor lido : ");  
        Serial.println(results.value, HEX);  
        armazenavalor = (results.value);  
        Menu();
        irrecv.resume(); //Le o próximo valor
  }*/
  
   if(currentMillis2 - previousMillis2 > tempo2){  //tempo para trocar menu, 5 seg default
                   if (n ==5) { 
                           n=1;
                    }
                   menuDisplay(n);  
                   n++;
                   previousMillis2 = currentMillis2;  
   }
  
  if(currentMillis - previousMillis > tempo){    //tempo para ler e enviar dados 1 min default
                   previousMillis = currentMillis;
                   //lerTemperatura();
                   lerUmidade();
                   delay(1000);
                   lerNivel();
                   //enviaDados(0);
                   delay(1000);
                   //enviaDados(1);
                   lerSensorChuva();
                   //enviaDados(2); 
                   
                   if(sleep==0){
                               lcd.clear();
                               lcd.print("----|E M O|----");
                               lcd.setCursor(0,1);
                               lcd.print("DADOS ENVIADOS!");
                   }
                  
   }

}

void lerTemperatura(){

                            temperatura = (5.0 * analogRead(pinoSensor) * 100.0) / 1024.0;
                            apito();
                            Serial.print("Temperatura lida: ");
                            lcd.clear();
                            lcd.print("----|E M O|----");
                            lcd.setCursor(0,1);
                            lcd.write(2);
                            lcd.print("LENDO SENSORES");
                            lcd.print(temperatura); 
                            //lcd.write(1);
                            //Serial.println(temperatura);
}
void lerNivel(){
                            nivel = analogRead(SensorNivel);
                            apito();
                            Serial.print("Sensor Nivel: ");
                            Serial.println(nivel);
                            lcd.clear();
                            lcd.print("----|E M O|----");
                            lcd.setCursor(0,1);
                            lcd.write(2);
                            lcd.print("LENDO SENSORES");
                            //lcd.print(temperatura); 
                            //lcd.write(1);
                            //Serial.println(temperatura);
}
void printNivel(){
                            nivel = analogRead(SensorNivel);
                            //apito();
                            Serial.print("Sensor Nivel: ");
                            Serial.println(nivel);

                            //lcd.print(temperatura); 
                            //lcd.write(1);
                            //Serial.println(temperatura);
}

void enviaDados(int dado){
  
   
  if (client.connect(server, 80)) {
              if (dado==0){
                                 // Ethernet.begin(mac);
                                 
                                  Serial.println("conectando...");
                                  Serial.println("connectado");
                                  client.print("PUT /entrada.php?temp=");
                                  Serial.print("PUT /entrada.php?temp=");
                                  //client.print(temperatura);
                                  client.print(DHT.temperature);
                                  Serial.print(DHT.temperature);
                                  client.println(" HTTP/1.1");
                                  Serial.println(" HTTP/1.1");
                                  client.println("Host: www.silveira.96.lt");
                                  client.println("Connection: Keep-Alive");
                                  //client.println("Connection: close");
                                  client.println();
                                  client.stop();
                                  Serial.println("Dados enviados com sucesso!");
              }
              
            
              if (dado==1){
                                 // Ethernet.begin(mac);
                                  Serial.println("conectando...");
                                  //client.connect(server, 80);
                                  Serial.println("connectado");
                                  client.print("PUT /entradaumidade.php?umidade=");
                                  Serial.print("PUT /entradaumidade.php?umidade=");
                                  client.print(DHT.humidity);
                                  Serial.print(DHT.humidity);
                                  client.println(" HTTP/1.1");
                                  Serial.println(" HTTP/1.1");
                                  client.println("Host: www.silveira.96.lt");
                                  client.println("Connection: Keep-Alive");
                                 // client.println("Connection: close");
                                  client.println();
                                  client.stop();
                                  Serial.println("Dados enviados com sucesso!");
              }
              if (dado==2){
                                  //Ethernet.begin(mac);
                                  Serial.println("conectando...");
                                  //client.connect(server, 80);
                                  Serial.println("connectado");
                                  client.print("PUT /entradachuva.php?chuva=");
                                  Serial.print("PUT /entradachuva.php?chuva=");
                                  if (intensidadeChuva==0){
                                        client.print("1"); 
                                  }
                                  if (intensidadeChuva==1){
                                        client.print("2");
                                  }
                                  if (intensidadeChuva==2){
                                        client.print("3"); 
                                  }
                          
                                  //Serial.print(temperatura);
                                  client.println(" HTTP/1.1");
                                  Serial.println(" HTTP/1.1");
                                  client.println("Host: www.silveira.96.lt");
                                  client.println("Connection: Keep-Alive");
                                 // client.println("Connection: close");
                                  client.println();
                                  client.stop();
                                  Serial.println("Dados enviados com sucesso!");
              }
  
  } 
  else {
                Serial.println("Deu pau!");
  }
  
}

void apito(){
  if (mute==1){
                      digitalWrite(buzzer, HIGH);
                      delay(10);
                      digitalWrite(buzzer, LOW);
  }
}

int menuDisplay(int entrada){
  if(sleep==0){
                if (entrada==1){
                                    //apito();
                                    //digitalWrite(azul, HIGH);
                                    digitalWrite(vermelho, LOW);
                                    digitalWrite(verde, HIGH);
                                    lcd.clear();
                                    lcd.print("----|E M O|----");
                                    lcd.setCursor(0,1);
                                    lcd.write(2);
                                    lcd.print("T: ");
                                    //lcd.print(temperatura); 
                                    lcd.print(DHT.temperature);
                                    lcd.write(1);
                                    printNivel();
                }
                if (entrada==2){
                                   //digitalWrite(azul, LOW);
                                    digitalWrite(vermelho, HIGH);
                                    digitalWrite(verde, LOW);
                                    lcd.clear();
                                    lcd.print("----|E M O|----");
                                    lcd.setCursor(0,1);
                                    lcd.print("Chuva: ");
                                    printNivel();
                                    if (intensidadeChuva==0){
                                          lcd.print("sem chuva"); 
                                    }
                                    if (intensidadeChuva==1){
                                          lcd.print("moderada"); 
                                    }
                                    if (intensidadeChuva==2){
                                          lcd.print("tensa"); 
                                    }
                }
                if (entrada==3){
                                   // digitalWrite(azul, LOW);
                                    digitalWrite(vermelho, LOW);
                                    digitalWrite(verde, HIGH);
                                    lcd.clear();
                                    lcd.print("----|E M O|----");
                                    lcd.setCursor(0,1);
                                    lcd.print("Nivel: ");
                                    lcd.print(nivel);
                       
                                    printNivel();
                                   
     
                                    
                }
                if (entrada==4){
                                    //apito();
                                    //digitalWrite(azul, HIGH);
                                    digitalWrite(vermelho, HIGH);
                                    digitalWrite(verde, LOW);
                                    lcd.clear();
                                    lcd.print("----|E M O|----");
                                    lcd.setCursor(0,1);
                                    lcd.print("Umidade: ");
                                    lcd.print(DHT.humidity);
                                    lcd.print("%");
                                   printNivel(); 
                }
                if (entrada==5){
                                    //apito();
                                    //digitalWrite(azul, LOW);
                                    digitalWrite(vermelho, LOW);
                                    digitalWrite(verde, HIGH);
                                    lcd.clear();
                                    lcd.clear();
                                    lcd.print("----|E M O|----");
                                    lcd.setCursor(0,1);
                                    lcd.print("Tempo: ");
                                    lcd.print(tempo/60000);  
                                    lcd.print(" min");
                }
  }
          
}

void Menu(){
                        if ((armazenavalor == 0xFFA857) || (armazenavalor == 0x4658) || (c == '+') || (val == 'u')){   //+  0xCC0031CE
                              apito();
                              tempo=tempo + 60000;
                              menuDisplay(5);
                             // Serial.println("Tempo: +");
                              c='m';
                              val='j';
                          }  
                        
                        if ((armazenavalor == 0xFFE01F) || (armazenavalor == 0x4659) || (c == '-')|| (val == 'd')) {  //- 0xCC00B14E
                              apito();
                              tempo=tempo-60000;
                              if(tempo <60000){
                                                  tempo = 60000;
                              }
                              menuDisplay(5);
                              //Serial.println("Tempo: -");
                              c='m';
                              val='j';
                        }  
                        if ((armazenavalor ==  0xFF02FD) || (armazenavalor ==  0x465B) || (c == '>')|| (val == 'r')){  //>> 0xCC00B847 
                              apito();
                              if(sleep==0){
                                              lcd.clear();
                                              lcd.print("----|E M O|----");
                              }
                        
                             switch(lado) {
                                                case 1:{
                                                      menuDisplay(1);
                                                      lado=2; 
                                                      break;
                                                };
                                                
                                                case 2:{
                                                      menuDisplay(2); 
                                                      lado=3; 
                                                      break;
                                                };
                                                
                                                case 3:{
                                                      menuDisplay(3);
                                                      lado=4; 
                                                      break;
                                                };
                                                
                                                case 4:{
                                                      menuDisplay(4); 
                                                      lado=1; 
                                                      break;
                                                };
                        };
                        c='m';
                        val='j';
                        }  
                         if ((armazenavalor == 0xFF22DD) || (armazenavalor == 0x465A) || (c == '<') || (val == 'e')){  //<< 0xCC007887 || 
                              apito();
                              if(sleep==0){
                                            lcd.clear();
                                            lcd.print("----|E M O|----");
                              }
                              c='m';
                              val='j';
                              switch(lado){ 
                              
                                            case 1:{
                                                  menuDisplay(1);
                                                  lado=4; 
                                                  break;
                                            };
                                            
                                            case 2:{
                                                  menuDisplay(2);
                                                  lado=1; 
                                                  break;
                                            };
                                            
                                            case 3:{
                                                  menuDisplay(3);
                                                  lado=2; 
                                                  break;
                                            };
                                            
                                            case 4:{
                                                  menuDisplay(4); 
                                                  lado=3; 
                                                  break;
                                            };
                              
                        }
                     }
                        if ((armazenavalor ==  0xFFC23D)||(armazenavalor ==  0x465C) || (c == 't')|| (val == 'l')){  //botao que força leitura de temp 0xCC00E817
                              apito();
                              if(sleep==0){
                                                  lcd.clear();
                                                  lcd.print("----|E M O|----");
                                                  lcd.setCursor(0,1);
                                                  lcd.print("LENDO SENSORES");
                              }
                       //lcd.clear();
                       lerUmidade();
                       delay(1000);
                       //enviaDados(0);
                       delay(1000);
                       //enviaDados(1);
                       lerSensorChuva();
                       //enviaDados(2); 
                       if(sleep==0){
                                       lcd.clear();
                                       lcd.print("----|E M O|----");
                                       lcd.setCursor(0,1);
                                       lcd.print("DADOS ENVIADOS!");
                       }
                       c='m';
                       val='j';
                        
                        }  
                         if ((armazenavalor == 0xFFE21D) ||(armazenavalor == 0x46C7) || (c == 's')|| (val == 's')){  //botao sleep0xCC0000FF || 
                             c='m';
                             val='j';
                             apito();
                             
                              if (sleep==0) {
                                    
                                                 sleep=1;
                                                 lcd.clear();
                                                 lcd.print("MODO: POWER LOW :D ");
                                      
                                                 for (int i = 0; i < 20; i++) {
                                                          lcd.scrollDisplayLeft();
                                                          delay(500);    
                                                 }
                                                 lcd.clear();
                               
                              } 
                              if (sleep==1) {
                                    
                                                  lcd.clear(); 
                                                  menuDisplay(1);
                                                  sleep=0;
                                                  apito();
                               }     
                       
                       }  
                       if ((armazenavalor == 0xFF906F)|| (val == 'm')){  //botao sleep mode  0xCC0000FF || 
                             c='m';
                             val='j';
                             apito();
                             delay(100);
                             apito();
                             switch(mute){ 
                              
                                            case 1:{
                                                  mute=0;
                                                  if(sleep==0){
                                                      lcd.clear();
                                                      lcd.print("----|E M O|----");
                                                      lcd.setCursor(0,1);
                                                      lcd.print("MUTE ON");
                                                  }
                                                  break;
                                                };
                                             case 0:{
                                               if(sleep==0){
                                                  mute=1;
                                                  lcd.clear();
                                                  lcd.print("----|E M O|----");
                                                  lcd.setCursor(0,1);
                                                  lcd.print("MUTE OFF");
                                               }
                                                  break;
                                                };
                             }
                         
                 
                }
}  
  
void lerSensorChuva(){
  
                                val_d = digitalRead(pino_d);  //Le e armazena o valor do pino digital
                                
                                val_a = analogRead(pino_a);  //Le e armazena o valor do pino analogico
                                
                                Serial.println("______________________________________");  //Envia as informacoes para o serial monitor
                                Serial.println(" SENSOR CHUVA ");
                                Serial.print(" Valor analogico : ");
                                Serial.println(val_a);
                               
                                if (val_a >900 && val_a <1024){
                                           intensidadeChuva=0;
                                           Serial.println(" Sem Chuva! ");
                                           Serial.println("______________________________________");
                                }
                                if (val_a >400 && val_a <900){
                                          intensidadeChuva=1;
                                          Serial.println(" - Chuva Moderada ");
                                          Serial.println("______________________________________");
                                 
                                }
                                if (val_a > 0 && val_a <400){
                                           intensidadeChuva=2;
                                           Serial.println(" Chuvendo pra CARALHO ");
                                           Serial.println("______________________________________");
                                 
                                }
                                delay(1000);
}

void lerUmidade(){          //le sensor umidade e Temperatura

                            DHT.read(dht_dpin); //Faz leitura das informações do sensor
                            Serial.println("______________________________________");
                            Serial.println(" SENSOR TEMPERATURA/UMIDADE RELATIVA ");
                            Serial.print("Umidade: ");
                            Serial.print(DHT.humidity);
                            Serial.println(" % ");
                            Serial.println("Temperatura: ");
                            Serial.print(DHT.temperature); 
                            Serial.println(" Celsius ");
                            Serial.println("______________________________________");
                            delay(2000);  //Faz a leitura a cada 2 segundos, aconselho a não diminuir muito o valor
}


