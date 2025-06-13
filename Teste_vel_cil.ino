//#include <Ultrasonic.h>
#include <Wire.h>
//#include "MedianFilterLib.h"
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
//ENTRADAS
#define in1 D3
#define in2 D4
#define in3 D5
#define in4 D6
//SAÍDAS
#define out0 D0
#define out1 D7
#define out2 D8
LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned long delaybtw;
unsigned long delay_ava;
unsigned long delay_rec;
bool acnA_ = 0;
bool acnR_ = 0;
int count = 0;
int atraso = 0;

int libe[] = {0,0};
void ICACHE_RAM_ATTR le_ava() {
  acnA_ = 1;
  acnR_ = 0;
  // É a primeira vez que o sensor leu?
 if(count == 0) {
    libe[0] = 1;
    delay_ava = millis();
    count = 1;
  }
  delay(atraso);
}
void ICACHE_RAM_ATTR le_rec(){
  // É a primeira vez que o sensor leu?
  acnR_ = 1;
  acnA_ = 0;
  if(count == 0) {
    libe[0] = 1;
    delay_rec = millis();
    count = 1;
  }
  delay(atraso);

}

void setup() {
Serial.begin(9600);
//Interrupções
lcd.init();
lcd.backlight();
lcd.clear();
delay(10);
attachInterrupt(digitalPinToInterrupt(in1), le_ava, FALLING); // Configura interrupção no pino 2 para quando houver uma borda de descida
attachInterrupt(digitalPinToInterrupt(in2), le_rec, FALLING); // Configura interrupção no pino 2 para quando houver uma borda de descida
pinMode(in1, INPUT_PULLUP);
pinMode(in2, INPUT_PULLUP);
pinMode(in3, INPUT_PULLUP);
pinMode(in4, INPUT_PULLUP);
pinMode(out1,OUTPUT);
pinMode(out2,OUTPUT);
delaybtw = 0;
acnA_ = 0;
acnR_ = 0;
count = 0;
delay_rec = 0;
delay_ava = 0;
atraso = 1000;

}


void loop() {
//Inicia LCD
lcd.setCursor(0,0);
lcd.print("Teste de vel.:");
//Leitura das entradas

//Verifica avanço
if(acnA_ && libe[1] == 1) {
  delay_ava = millis() - (delay_ava - atraso);
  delay_rec = millis();//Pega o instante em que o cilindro está recuado
  char buffer_ava[4];
  sprintf(buffer_ava, "%-5d", delay_ava); // Formata o número à esquerda ocupando 5 bits
  lcd.setCursor(9,1); 
  lcd.print("Av");
  lcd.setCursor(11,1);
  lcd.print(buffer_ava);
  Serial.print("Buffer Ava");
  Serial.println(buffer_ava);
  Serial.println(acnA_);
  digitalWrite(out1, LOW);
  libe[0] = 1;
  libe[1] = 0;
  acnA_ = 0;
  Serial.print("ACNA");
  Serial.println(acnA_);
}
// POR QUE AS VEZES NÃO LE O RECUO
if(acnR_ && libe[0] == 1) {
  //Pega o tempo
  delay_ava = millis(); //Pega o instante em que o cilindro está avançado
  delay_rec = millis() - (delay_rec - atraso);
  digitalWrite(out1, HIGH); 
  char buffer_rec[4];
  sprintf(buffer_rec, "%-5d", delay_rec); // Formata o número à esquerda ocupando 5 bits
  lcd.setCursor(0,1); 
  lcd.print("Re");
  lcd.setCursor(3,1);
  lcd.print(buffer_rec);
  Serial.print("Buffer Rec");
  Serial.println(buffer_rec);
  //Finaliza o ciclo
  libe[0] = 0;
  libe[1] = 1;
  acnR_ = 0;
  Serial.print("ACNR");
  Serial.println(acnR_);
}
delay(10);

}
