//******************************************************************************************************* 
//                                        UNIVERSIDADE FEDERAL DE RORAIMA
//                                      INTRODUÇÃO AOS SISTEMAS EMBARCADOS
//                                          PROF: HERBERT OLIVEIRA ROCHA 
//                    ALUNOS:  EWELLY FABIANE CUNHA DE SOUSA & IBUKUN CHIFE DIDIER ADJITCHE
//Programa : RFID-SERVO-ULTRASONICO-PIR- Controle de acesso e de gerenciamento de vaga no estacionamento
// ******************************************************************************************************

#include <Ultrasonic.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h> 
Servo microservo9g;
//Define os pinos para o trigger e echo DO ULTRASONICO
#define echoPin 1 // Recebe o pulso do echo
#define trigPin 2 // Envia o pulso para gerar echo
//ACIONANDO OS LEDS jUNTO AO ULTRASONICO
int ledVerde = 7;
int ledVermelho = 8;
// ACIONANDO O PERIFERICO RFID
#define SAD_PIN 10
#define RST_PIN 9
// Leds indicadores acesso liberado ou negado DO SERVO JUNTO AO RFIG
int led_liberado = 5;
int led_negado = 6;
char st[20];
//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(1,2);
// Definicoes pino modulo RC522
MFRC522 mfrc522(SAD_PIN, RST_PIN); 
//PIR SENSOR
int PinoPir = 4;  //Pino ligado ao sensor PIR
int acionamento;  //Variavel para guardar valor do sensor

void setup() { 
  // Inicia a serial
  Serial.begin(9600);
  //ULTRASONICO
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  //LED DE OCUPAÇÃO JUNTO AO ULTRASONICO
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  //PIR SENSOR
  pinMode(PinoPir, INPUT);   //Define pino sensor como entrada
  //LED DE ACESSO JUNTO AO SERVO
  pinMode(led_liberado, OUTPUT);
  pinMode(led_negado, OUTPUT);
  // Define que o servo esta ligado a porta digital 3
  microservo9g.attach(3);
  // Move o servo para a posicao inicial (cancela fechada)
  microservo9g.write(90);
  // Inicia  SPI bus
  SPI.begin();
  // Inicia MFRC522
  mfrc522.PCD_Init(); 
  // Mensagens iniciais no serial monitor
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
}

void loop() {

 acionamento = digitalRead(PinoPir); //Le o valor do sensor PIR
 if (acionamento == LOW)  //Sem movimento, 
 {
;
 }
 else  //Caso seja detectado um movimento,
 {
        //FUNCIONAMENTO DDO RFIG
        // Aguarda a aproximacao do cartao
        if ( ! mfrc522.PICC_IsNewCardPresent()) 
        {
          return;
        }
        // Seleciona um dos cartoes
        if ( ! mfrc522.PICC_ReadCardSerial()) 
        {
          return;
        }
        // Mostra UID na serial
        Serial.print("UID da tag :");
        String conteudo= "";
        byte letra;
        for (byte i = 0; i < mfrc522.uid.size; i++) 
        {
           Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
           Serial.print(mfrc522.uid.uidByte[i], HEX);
           conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
           conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
        }
        Serial.println();
        Serial.print("Mensagem : ");
        conteudo.toUpperCase();
        
        // Testa se o cartao1 foi lido
        if (conteudo.substring(1) == "D8 4B 12 22")
        {
          // Levanta a cancela e acende o led verde
          microservo9g.write(-90);
          digitalWrite(led_liberado, HIGH);
          Serial.println("Cartao1 - Acesso liberado !");
          Serial.println();
          delay(3000);
          microservo9g.write(90);
          digitalWrite(led_liberado, LOW);
          }
          
        // Testa se o cartao2 foi lido
        if (conteudo.substring(1) == "87 4B DC 8A")
        {
          Serial.println("Cartao2 - Acesso negado !!");
          Serial.println();
          // Pisca o led vermelho
          for (int i= 1; i<5 ; i++)
          {
            digitalWrite(led_negado, HIGH);
            delay(200);
            digitalWrite(led_negado, LOW);
            delay(200);
          }
        }
        delay(1000);
// FUNCIONAMENTO DO ULTRASONICO
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
          int distanciaCentimetros = (ultrasonic.distanceRead()); //(ultrasonic.Ranging(CM));
          Serial.print("Distância em centimetros = ");
          Serial.print(distanciaCentimetros);
          
          if(distanciaCentimetros < 5){
            digitalWrite(ledVerde, HIGH);
            delayMicroseconds(10);
            digitalWrite(ledVerde, LOW);
          } else {
            digitalWrite(ledVermelho, HIGH);
            delayMicroseconds(10);
            digitalWrite(ledVermelho, LOW);
          }
          
          delay(1000);  

 }

}
