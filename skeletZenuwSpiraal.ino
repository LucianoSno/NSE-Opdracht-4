/*
* skelet voor opdracht 4 Zenuwspiraal (Embedded Systems 1718)
* Auteur: Wouter Pijnacker Hordijk, HBO-ICT, Haagse Hogeschool
* versie 1.0
* 
* condities zijn nog niet ingevuld
* elke conditie ontwikkel je in drie stappen:
* stap 1: bepaal de ingangssignalen (2 of 3 variabelen/functies van type boolean), 
*         noem die A, B (en C) en stel de volledige waarheidstabel op
* stap 2: bepaal met behulp van de methode Sum of Products de formule en schrijf die uit
* stap 3: schrijf de formule om naar de betreffende C-code (AND: &&, OR: ||, NOT: !)
* 
* versie 0.1: vul condities op regels 192 en 200 aan zodat het spel goed op de knoppen reageert
* versie 0.2: vul condities op regels 208 en 212 aan zodat de teller correct start en stopt
* versie 0.3: vul conditie op regel 216 aan zodat de zoemer alleen aan is als je 'af' bent en de draad raakt
* versie 0.4: vul condities op regels 184, 193 en 201 aan zodat je het spel met de zenuwspiraal kunt bedienen
* versie 0.5: verwijder het commentaar op regel 188  en vul de conditie op regel 132 aan voor het aftellen
* 
* Zet regelnummers in de editor aan met Bestand/Voorkeuren/Regelnummers weergeven
* LET OP: regelnummers veranderen als je regels toevoegd of verwijdert!!
*/

#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

const int D1PIN = 13;
const int D2PIN = 12;
const int D3PIN = 11;
const int D4PIN = 10;

const int ZOEMERPIN = 3;

const int HANDVATPIN = A5;
const int STARTPIN = 5;
const int DRAADPIN = 6;
const int EINDPIN = 9;

const int KNOP1PIN = A1;
const int KNOP2PIN = A2;
const int KNOP3PIN = A3;

unsigned long huidigeTijdMillis;
unsigned long laatsteTijdMillis;

float speeltijd;

int toestand;
const int SPEL_INGESCHAKELD  = 0;
const int STARTPUNT_GERAAKT  = 1;
const int DRAAD_GERAAKT      = 2;
const int EINDPUNT_BEREIKT   = 3;

void LED1Aan(){
  digitalWrite(D1PIN, LOW);
}

void LED2Aan(){
  digitalWrite(D2PIN, LOW);
}

void LED3Aan(){
  digitalWrite(D3PIN, LOW);
}

void LED4Aan(){
  digitalWrite(D4PIN, LOW);
}

void LED1Uit(){
  digitalWrite(D1PIN, HIGH);
}

void LED2Uit(){
  digitalWrite(D2PIN, HIGH);
}

void LED3Uit(){
  digitalWrite(D3PIN, HIGH);
}

void LED4Uit(){
  digitalWrite(D4PIN, HIGH);
}

void zoemerAan(){
  digitalWrite(ZOEMERPIN, LOW);
}

void zoemerUit(){
  digitalWrite(ZOEMERPIN, HIGH);
}

void piep(){
  zoemerAan();
  delay(50);
  zoemerUit();
}

boolean startpunt(){
  return !digitalRead(STARTPIN);
}

boolean draad(){
  return !digitalRead(DRAADPIN);
}

boolean eindpunt(){
  return !digitalRead(EINDPIN);
}

boolean knop1Gedrukt(){
  return !digitalRead(KNOP1PIN);
}

boolean knop2Gedrukt(){
  return !digitalRead(KNOP2PIN);
}

boolean knop3Gedrukt(){
  return !digitalRead(KNOP3PIN);
}

void aftellen(){
  int teller = 3;
  boolean afgeteld = false;
  huidigeTijdMillis= millis();
  laatsteTijdMillis = huidigeTijdMillis;
  LED4Aan();
  MFS.write (teller);
  
  while (!afgeteld /* plus extra condities */) {
    huidigeTijdMillis= millis();
    if (huidigeTijdMillis > laatsteTijdMillis + 1000){ // dus als er één seconde voorbij is
      laatsteTijdMillis = huidigeTijdMillis;
      teller--;
      MFS.write (teller);
      piep();
      afgeteld = (teller == 0);
    }
    delay(50);
  }
  LED4Uit();
}

void resetTimer(){
  speeltijd = 0;
  MFS.write (speeltijd, 2);
}

void updateTimer() {
  huidigeTijdMillis = millis();
  if (huidigeTijdMillis > laatsteTijdMillis + 10){
    laatsteTijdMillis = huidigeTijdMillis;
    speeltijd += 0.01;
    MFS.write (speeltijd, 2);
  }
}

void setup()
{
  pinMode(D1PIN, OUTPUT);
  pinMode(D2PIN, OUTPUT);
  pinMode(D3PIN, OUTPUT);
  pinMode(D4PIN, OUTPUT);
  pinMode(ZOEMERPIN, OUTPUT);
  pinMode(HANDVATPIN, OUTPUT);
  pinMode(STARTPIN, INPUT_PULLUP);
  pinMode(DRAADPIN, INPUT_PULLUP);
  pinMode(EINDPIN, INPUT_PULLUP);

  Timer1.initialize();
  MFS.initialize(&Timer1);

  toestand = SPEL_INGESCHAKELD;
  LED1Uit();
  LED2Uit();
  LED3Uit();
  LED4Uit();
}

void loop()
{
  if (knop1Gedrukt() /* plus code voor reageren op startpunt */ ) {
    LED1Aan();
    LED2Uit();
    LED3Uit();
    // aftellen(); 
    toestand = STARTPUNT_GERAAKT;
  }

  if (knop2Gedrukt() /* plus andere condities uit opdracht 3b */
                     /* plus code voor reageren op draad      */ ) {
    LED1Uit();
    LED2Aan();
    LED3Uit();
    toestand = DRAAD_GERAAKT;
  }

  if (knop3Gedrukt() /* plus andere condities uit opdracht 3b */
                     /* plus code voor reageren op eindpunt   */ ) {
    LED1Uit();
    LED2Uit();
    LED3Aan();
    toestand = EINDPUNT_BEREIKT;
  }

  if (false /* vervangen door andere conditie! */) {
    updateTimer();
  }

  if (false /* vervangen door andere conditie! */) {
    resetTimer();
  }

  if (false /* vervangen door andere conditie! */) {
    zoemerAan();
  } else {
    zoemerUit();
  }
}

