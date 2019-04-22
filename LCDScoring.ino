//LCD
#include <LiquidCrystal.h> 
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs=0, en= 1, d4=6, d5=7, d6=8, d7=9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
// LCD setup
//setup serial, output, input pins
     Serial.begin(9600);
     
     //setup lcd cols and rows 
     lcd.begin(16, 2);
     //print out player names, play1 on row 0, play2 on row 1
     lcd.print("Player1: ");
     lcd.setCursor(0,1);
     lcd.print("Player2: ");
     
}

void loop() {
  // put your main code here, to run repeatedly:

}

//Function displayScore will print score
//of each player onto LCD
void displayScore(){
  lcd.setCursor(10,0);
  lcd.print(WIN1);
  lcd.setCursor(10, 1)
  lcd.print(WIN2);
}

