//BUZZER// && //LED//
int buzzer = 11 ; 
int smoke = A5 ;
int sensor = 700 ;
int led_rouge = 13 ; 
int led_verte = 12 ;

//LCD config//
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,20,4);

//MOTEUR SERVO//
#include <Servo.h>

//PAD//
#include <Keypad.h>

//Variables//
int mot_min = 140;  //min servo angle  (set yours)
int mot_max = 0; //Max servo angle   (set yours)
int character = 0;
int activated =0;
char Str[16] = {' ', ' ', ' ', ' ', ' ', ' ', '-', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' '};  

//Pins//
Servo myservo;
//int external = 12; //pin to inside open
//int internal = 13; //pin to inside close

//Keypad configuration//
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad//
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){

  Serial.begin(9600);
  myservo.attach(10); //attach the servo to pin D10
  pinMode(buzzer,OUTPUT); 
  //Init the screen and print the first text//
  lcd.init();
  lcd.backlight();
  // Envoi du message//
  lcd.setCursor(2, 0);
  lcd.print("VOICI NOTRE");
  lcd.setCursor(3,1);
  lcd.print("SMART HOME");

  //put the servo in the close position first
  myservo.write(mot_min); 

  //LED && BUZZER//
  pinMode(led_rouge,OUTPUT);
  pinMode(led_verte,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(smoke,INPUT);
  
}
  
void loop(){

  //KEYPAD OPEN/CLOSE//  
  char customKey = customKeypad.getKey(); //this function reads the presed key
  
  if (customKey){
    if (character == 0) {  
      Serial.println(customKey);
      Str[6]= customKey; 
      //STANDARD();//  
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("    PASSWORD");    
      lcd.setCursor(0,1);
      lcd.print(Str);
    }

    if (character == 1) {  
      Serial.println(customKey);
      Str[7]= customKey;  
      //STANDARD();// 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("    PASSWORD");    
      lcd.setCursor(0,1);
      lcd.print(Str);   
    }

    if (character == 2) {  
      Serial.println(customKey);
      Str[8]= customKey;  
      //STANDARD();// 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("    PASSWORD");    
      lcd.setCursor(0,1);
      lcd.print(Str);
    }

    if (character == 3) {  
      Serial.println(customKey);
      Str[9]= customKey;
      //STANDARD();//   
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("    PASSWORD");    
      lcd.setCursor(0,1);
      lcd.print(Str);  
    }

    if (character == 4) {  
      Serial.println(customKey);
      Str[10]= customKey;
      activated=1;
    }
    character=character+1;
  }

  if (activated == 1)
    {
/*Change your password below!!! 
Change each of Str[6], Str[7], Str[8], Str[9]*/

    if(Str[10]='A' && character==5 && Str[6]=='0' && Str[7]=='8' && Str[8]=='2' && Str[9]=='4' )
    {
      //OUVERTURE();//
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("PORTE");
      lcd.setCursor(4,1);
      lcd.print("OUVERTE");
      myservo.write(mot_max);
      activated = 2;
      analogWrite(buzzer,240);
      delay(250);
      analogWrite(buzzer,200);
      delay(250);
      analogWrite(buzzer,180);
      delay(250);
      analogWrite(buzzer,250);
      delay(250);
      analogWrite(buzzer,LOW);
      delay(1000);
      
      lcd.clear();    
      lcd.setCursor(0,0);
      lcd.print("JE VOUS EN PRIE");
      delay(500);
      lcd.setCursor(4,1);
      lcd.print("ENTREZ");
      delay(1000);

      lcd.clear();    
      lcd.setCursor(1,0);
      lcd.print("COMMENT CA VA");
      lcd.setCursor(2,1);
      lcd.print("AUJOURD'HUI ?");
      digitalWrite(led_rouge, LOW);
      digitalWrite(led_verte, LOW);
      
    }
    else
    {
      //ERREUR();//
      digitalWrite(led_rouge, HIGH);
      digitalWrite(led_verte, LOW);
      lcd.clear();    
      lcd.setCursor(1,0);
      lcd.print("PASSWORD ERROR");
      lcd.setCursor(3,1);
      lcd.print("REESSAYEZ");
      analogWrite(buzzer,150);
      delay(3000);
      analogWrite(buzzer,LOW);
      character=0;
      Str[6]= '-';
      Str[7]= '*'; 
      Str[8]= '*'; 
      Str[9]= '*';
      Str[10]= ' ';
      activated = 0;
      lcd.clear();    
      lcd.setCursor(4,0);
      lcd.print("PASSWORD");
      lcd.setCursor(0,1);
      lcd.print(Str); 
      digitalWrite(led_rouge, LOW);
      digitalWrite(led_verte, LOW);  
    }
  }
  if (activated == 2)
    {
    if(customKey == 'B' )
    {
      //FERMETURE();//
      myservo.write(mot_min);
      activated = 0;
      character=0;
      Str[6]= '-';
      Str[7]= '*'; 
      Str[8]= '*'; 
      Str[9]= '*';
      Str[10]= ' ';   
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("    PASSWORD");    
      lcd.setCursor(0,1);
      lcd.print(Str);
     
    }
  }

  //BUZZER//
  int analogSensor= analogRead(smoke);
  Serial.println(analogSensor);

  if (analogSensor > sensor){
    //BUZZ_ON();//
    digitalWrite(led_rouge, HIGH);
    digitalWrite(led_verte, LOW);
    tone(buzzer, 1000, 200);
  }

  else {
    //BUZZ_OFF();//
    digitalWrite(led_rouge, LOW);
    digitalWrite(led_verte, HIGH);
    noTone(buzzer);
  }

  delay(100);
}


/////FONCTIONS/////

  ///PORTE///
void FERMETURE () {
  myservo.write(mot_min);
  activated = 0;
  character=0;
  Str[6]= '-';
  Str[7]= '*'; 
  Str[8]= '*'; 
  Str[9]= '*';
  Str[10]= ' ';   
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("    PASSWORD");    
  lcd.setCursor(0,1);
  lcd.print(Str);
  }

void OUVERTURE () {
  myservo.write(mot_max);
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("PORTE");
  lcd.setCursor(4,1);
  lcd.print("OUVERTE");
  activated = 2;
  analogWrite(buzzer,240);
  delay(250);
  analogWrite(buzzer,200);
  delay(250);
  analogWrite(buzzer,180);
  delay(250);
  analogWrite(buzzer,250);
  delay(250);
  analogWrite(buzzer,LOW);
  delay(1000);
      
  lcd.clear();    
  lcd.setCursor(0,0);
  lcd.print("JE VOUS EN PRIE");
  delay(500);
  lcd.setCursor(4,1);
  lcd.print("ENTREZ");
  delay(1000);

  lcd.clear();    
  lcd.setCursor(1,0);
  lcd.print("COMMENT CA VA");
  lcd.setCursor(2,1);
  lcd.print("AUJOURD'HUI");
}
 void ERREUR () {
      digitalWrite(led_rouge, HIGH);
      digitalWrite(led_verte, LOW);
      lcd.clear();    
      lcd.setCursor(1,0);
      lcd.print("PASSWORD ERROR");
      lcd.setCursor(3,1);
      lcd.print("RÉESSAYEZ");
      analogWrite(buzzer,150);
      delay(3000);
      analogWrite(buzzer,LOW);
      character=0;
      Str[6]= '-';
      Str[7]= '*'; 
      Str[8]= '*'; 
      Str[9]= '*';
      Str[10]= ' ';
      activated = 0;
      lcd.clear();    
      lcd.setCursor(4,0);
      lcd.print("PASSWORD");
      lcd.setCursor(0,1);
      lcd.print(Str); 
 }

  ///BUZZER///
void BUZZ_ON () {
  digitalWrite(led_rouge, HIGH);
  digitalWrite(led_verte, LOW);
  tone(buzzer, 1000, 200);
}

void BUZZ_OFF () {
  digitalWrite(led_rouge, LOW);
  digitalWrite(led_verte, HIGH);
  noTone(buzzer);
}

  ///MESSAGE QUAND ON TAPPE SUR LE PAD///
void STANDARD () {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("    PASSWORD");    
  lcd.setCursor(0,1);
  lcd.print(Str);
}
