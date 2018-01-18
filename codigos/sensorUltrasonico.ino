#include <Ultrasonic.h>

//Define os pinos para o trigger e echo
#define echoPin 13 // Recebe o pulso do echo
#define trigPin 6 // Envia o pulso para gerar echo

int ledVerde = 8;
int ledVermelho = 9;

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(6,13);

void setup() {
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
}

void loop() {
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
