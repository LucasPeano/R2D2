  //LCD// 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x26,20,4);

      //TEMPERATURE//
#include <DHT.h>
#define DHTPIN 3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float hum;
float temp;

       //BLUETOOTH//
#include<SoftwareSerial.h>
#define RX 11
#define TX 12
SoftwareSerial BlueT(RX,TX);
char Data; 

//-- MOTEUR A --//
int ENA=9; //Connecté à Arduino pin 9(sortie PWM)
int IN1=4; //Connecté à Arduino pin 4
int IN2=5; //Connecté à Arduino pin 5

//-- MOTEUR B --//
int ENB=10; //Connecté à Arduino pin 10(Sortie PWM)
int IN3=6; //Connecté à Arduino pin 6
int IN4=7; //Connecté à Arduino pin 7
int i=0;

//LED//
#include <Adafruit_NeoPixel.h>
#define PIXEL_PIN 2 // On définit le pin où est connecté la patte DATA du bandeau
#define PIXEL_COUNT 33 // On définit le nombre de LED//

// Paramètre l'objet strip qui correspond à toute les LED du ruban//
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800); 

void setup() {
  
  Serial.begin(9600);
  BlueT.begin(9600);
  dht.begin();
  
  //Init the screen and print the first text//
  lcd.init();
  lcd.backlight();
  
  // Envoi du message//
  SMS_DEB();
  delay(2500);
  lcd.clear();
  SMS_DEB1 ();
  delay(2500);

  pinMode(ENA,OUTPUT); // Configurer 
  pinMode(ENB,OUTPUT); // les broches
  pinMode(IN1,OUTPUT); // comme sortie
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  // Moteur A - Ne pas tourner //
  digitalWrite(ENA,LOW);

  // Moteur B - Ne pas tourner //
  digitalWrite(ENB,LOW);
  
  // Direction du Moteur A //
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  
  // Direction du Moteur B //
  // NB: en sens inverse du moteur A //
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);

  // LED //
  // Lance la connection //
  strip.begin();
  // Initialise toute les led à 'off' //
  strip.show(); 
  
}

void loop() {

  hum = dht.readHumidity();
  temp = dht.readTemperature();
  strip.setBrightness(50);

  if (BlueT.available()) {
    Data=BlueT.read();
    Serial.println(Data);

    //TEMPERATURE//
    if (Data == 'T') {
      SMS_TEMP();
    }
    //HUMIDITÉ//
    if (Data == 'H') {
      SMS_HUM();
    }

  //Moteur A - Plein régime//
    if (Data == 'A') {
      //LES DEUX MOTEURS EN MARCHE AVANT//
      digitalWrite(IN1,LOW);
      digitalWrite(IN2,HIGH);
      digitalWrite(IN3,HIGH);
      digitalWrite(IN4,LOW);
      //AVANT();
      MOTEUR();
      MOTEUR(); 
    }
    
    if (Data=='Z') {
      analogWrite(ENA,0);
      analogWrite(ENB,0);
    }
    
    if (Data=='B') {
      //LES DEUX MOTEURS EN MARCHE ARRIÈRE//
      digitalWrite(IN1,HIGH);
      digitalWrite(IN2,LOW);
      digitalWrite(IN3,LOW);
      digitalWrite(IN4,HIGH);
      //ARRIÈRE();
      MOTEUR();
      MOTEUR();
    }
    
    if (Data=='C') {
      analogWrite(ENA,0);
      analogWrite(ENB,0);
    }
    
    //LED//
    if (Data == 'D') {
      all(50, 0, 0, 255);
      }
      
    if (Data == 'E') {
      all(0, 0, 0, 0); 
      } 
      
    if (Data == 'F') {
      all(50, 255, 255, 255);
      }
      
    if (Data == 'G') {
      all(0, 0, 0, 0);
      }
      
    if (Data == 'I') {
      all(50, 255, 0, 0);
      }
      
    if (Data == 'J') {
      all(0, 0, 0, 0);
      }
    }
  }
  //METTRE UN SWITCH / CASE A,J.. AU LIEU DE TOUS LES IF 
  //POUR ALLÉGER LE PROGRAMME ET LE RENDRE PLUS RAPIDE 


/////FONCTIONS/////

  ///MOTEURS ACTIVÉS PAR PETIT CRAN///
void MOTEUR() {
      analogWrite(ENA,255);
      analogWrite(ENB,255);
      delay(100);
      analogWrite(ENA,0);
      analogWrite(ENB,0);
      delay(100);
}

  ///MARCHE AVANT///
void AVANT() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

  ///MARCHE ARRIÈRE///
void ARRIERE() {
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

///MESSAGES SUR LCD POUR DETECTEUR DE TEMPERATURE///

  //MESSAGE DEBUT 1ÈRE PARTIE///
void SMS_DEB (){
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("BONJOUR");
  //le curseur se positionne à la 1ère colonne, 2ième ligne//
  lcd.setCursor(3, 1); 
  lcd.print("BIENVENUE");
}

  ///MESSAGE DEBUT 2ÈME PARTIE///
void SMS_DEB1 (){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("QUELLES DONNÉES");
  //le curseur se positionne à la 1ère colonne, 2ième ligne//
  lcd.setCursor(1, 1); 
  lcd.print("VOULEZ-VOUS ?");
}
  ///MESSAGE ANNONÇANT TEMPÉRATURE ACTUELLE///
void SMS_TEMP (){
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("TEMPERATURE:");
  lcd.setCursor(2, 1);
  lcd.print(temp);
  lcd.setCursor(8, 1);
  lcd.print("DEGRES");
  delay(1000);
}

  ///MESSAGE ANNONÇANT HUMIDITÉ ACTUELLE///
void SMS_HUM (){
  lcd.clear();
  lcd.setCursor(3, 0); 
  lcd.print("HUMIDITE :");
  lcd.setCursor(5, 1);
  lcd.print(hum);
  lcd.setCursor(10, 1);
  lcd.print("%");
  delay(1000);
}

///FONCTIONS RUBAN LED///

  //FONCTION CHENILLARD//
void chenillardUp(int temps, int r, int v, int b) {

  for(int i = 0 ; i < 33 ; i++) {
     strip.setPixelColor(i, r, v, b);
     strip.show();
     strip.setPixelColor(i, 0, 0, 0);
     strip.show();
     delay(temps);
  }
}

  //FONCTION ALL//
void all(int temps, int r, int v, int b) {
   for(int i = 0 ; i < 33 ; i++) {
     strip.setPixelColor(i, r, v, b);
     strip.show();
     delay(temps);
   } 
}

  //FONCTION PROGRESSIVE//
void progressiveUp(int temps, int r, int v, int b) {
  for(int lumi = 0 ; lumi < 10 ; lumi++) {
     for(int i = 0 ; i < 33 ; i++) {
       strip.setPixelColor(i, r, v, b);
       strip.setBrightness(lumi); 
       strip.show();
       delay(temps);
     }
  }
}

void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { 
      
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); 
    delay(wait);  
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<30; a++) {  
    for(int b=0; b<3; b++) { 
      strip.clear();         
      
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); 
      }
      strip.show(); 
      delay(wait);  
    }
  }
}
