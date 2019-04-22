//Rachel & Rashida
//Fall 2018
//This program will determine what move is made:
//Gun, Shield, or Reload based on
//analog input of flex strip and accelerometers

#define GUN 1
#define RELOAD 2
#define SHIELD 3 

//Player 1 Pins
const int plyr1TSPin= 0;
//copy over to rock paper scissors #define plyr1IndPin  1
const int plyr1RingPin=2;
//Player 2 pins
const int plyr2TSPin=3;
//copy over to rock paper scissors #define plyr2IndPin  4
const int plyr2RingPin=5;

//ACCELEROMETER
# define UP 0
# define DOWN 1
float TS_avg;
int AR_TS;
int Accel_Position(float analogin);

//FLEX STRIP
#define OPEN 1
#define CLOSED 0


//SOUND
// From Github for The Play6_HC Example

#include <FatReader.h>
#include <SdReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"
SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
FatReader f;      // This holds the information for the file we're play
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

void playcomplete(char *name);


// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void)
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 
 
void sdErrorCheck(void)
{
  if (!card.errorCode()) return;
  putstring("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  putstring(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
} 

//SCORING VARS
int store1 = 0;
int store2 = 0;
int WIN1 = 0;
int WIN2 = 0;

//LCD
#include <LiquidCrystal.h> 
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs=0, en= 1, d4=6, d5=7, d6=8, d7=9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

 



/////////////////////////////////////////////////
//////////SETUP/////////////////////////////////
//////////////////////////////////////////////////
void setup() {
 Serial.begin(9600);
 ///////////////set mode for ACCELEROMETER pins
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);

  ////////////////setup for SOUND
   putstring_nl("WaveHC with 6 buttons");
  
   putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(freeRam());      // if this is under 150 bytes it may spell trouble!
  
  // Set the output pins for the DAC control. This pins are defined in the library
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
 
 
  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    putstring_nl("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }
  
  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);
 
// Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    putstring_nl("No valid FAT partition!");
    sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }
  
  // Whew! We got past the tough parts.
  putstring_nl("Ready!");

///////////////////Setup for LCD
//setup lcd cols and rows 
lcd.begin(16, 2);
//print out player names, play1 on row 0, play2 on row 1
lcd.print("Player1: ");
lcd.setCursor(0,1);
lcd.print("Player2: ");   
}

/////////////////////////////////////////////////
////////LOOP/////////////////////////////////
//////////////////////////////////////////////////
void loop() {
    //player 1's move
    int play1= detMove(plyr1TSPin,plyr1RingPin);
    //player 2's move
    int play2= detMove(plyr2TSPin,plyr2RingPin);
    //playWhichSound(play1, play2);
Serial.println("PLAY1: ");
Serial.println(play1);
delay(1000);
}


//////////////
///HELPERS///
/////////////

//////////////ACCELEROMETERS///////////////////////////
//Function Accel_Position will determine what position
//the accelerometer(tilt sensor) is in
//Argument analogin is analog read from accelerometer
//returns int 0 for up, 1 for down
int TS_Position(float analogin){
  if(analogin >= 150 && analogin <= 280){
    return UP;
  }
  else if (analogin >= 300 && analogin <=400){
    return DOWN;
  }
  else {
    return 3;
  }
}

//Function avgTSInput will give averaged analog input
//from the accelerometer
//Argument plyrTSPin is the player's tilt sensor
//pin number 
int avgTSInput(int plyrTSPin){
    TS_avg = 0;
        // average value over a sampling period
        for (int i=1; i <=100; i++){
            AR_TS = analogRead(plyrTSPin);
            TS_avg = (TS_avg * ((float)i-1) + (float)AR_TS)/(float)i;
        }
    return TS_avg;
}

//////////////FLEX STRIP///////////////////////////
//Function flexing returns boolean
//if finger is bent or not
//Argument analoginFS is analog read from flex strip
bool flexing(float analoginFS){
  if (analoginFS >= 400){
    return OPEN;
  }
  else {
    return CLOSED;
  }
}

//Function avgFSInput will return average analog input 
//of flex strip
float avgFSInput(int plyrFSPin){
      // need 4 pairs of middle and index finger avg variables
      //initialize averaging vars for middle and index fingers
      float avgFSIn = 0;
      //take in analog input of middle and index finger for both players
      for (int i=1; i <=100; i++){
         int FS_PLYR_AR = analogRead(plyrFSPin);
         //average the analog values for each finger
         avgFSIn = (avgFSIn * ((float)i-1) + (float)FS_PLYR_AR)/(float)i;
      }
return avgFSIn;
}

//Function detMove() will return an int
//which corresponds to gun, reload, shield move
//Args: int plyrTSPin corresponds to the player's
//tilt sensor pin on arduino, int plyrFSPin is the player's 
//flex strip pin on arduino
int detMove(int plyrTSPin, int plyrFSPin){
  //take the info from that players pin
  //if analog for down and bent, it is a gun
  if( TS_Position(avgTSInput(plyrTSPin))==DOWN && flexing(avgFSInput(plyrFSPin))== CLOSED ){
  return GUN;
  }
  //if we have analog values for lifted up and bent, it is a reload
  else if( TS_Position(avgTSInput(plyrTSPin))==UP && flexing(avgFSInput(plyrFSPin))== CLOSED ){
  return RELOAD;
  }
  //if analog for unbent and down, it is shield
  //NEED TO SPECIFY THIS IF BECAUSE IF PLAYER MADE WEIRD MOVEMENT THEN IT COULD OUTPUT SHIELD WHEN WE DON'T WANT IT TO
  else if(flexing(avgFSInput(plyrFSPin))== OPEN ){
    return SHIELD; 
  }
}

//////////////SOUND///////////////////////////
//Function playWhichSound() will play a sound 
//on the speaker corresponding to which move was made
//Args: integers play1, play2 for the move player 1 & 2 made
void playWhichSound (int play1, int play2) { 
        //CASE 1: if one hand is a gun
        if (play1==GUN || play2==GUN){
          playcomplete("GUN.WAV");
        }
             //CASE 2: if no guns, and one hand is reload
             else if(play1==RELOAD|| play2==RELOAD){
                 playcomplete("RELOAD.WAV");
             }
                //CASE 3: if two shields 
                else {
                    playcomplete("SHIELD.WAV");
                }
}

// Plays a full file from beginning to end with no pause.
void playcomplete(char *name) {
  // call our helper to find and play this name
  playfile(name);
  while (wave.isplaying) {
  // do nothing while its playing
  }
  // now its done playing
}
 
void playfile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  // look in the root directory and open the file
  if (!f.open(root, name)) {
    putstring("Couldn't open file "); Serial.print(name); return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); return;
  }
  
  // ok time to play! start playback
  wave.play();
}

//////////////////////////LCD//////////////////////////

//Function scoring will update and keep track
//of score of each player with vars: WIN1, WIN2
//based on move each player made 
//Args: player1, player 2 are the moves player1 and 2 made
void scoring(int player1, int player2){
  // GUN, GUN case
  Serial.print(player1);
  Serial.println(player2);

  
  if (player1 + player2 == GUN + GUN) {
    // determine if either has store == 5 --> end game
    if (store1 >= 5 || store2 >= 5){
      if (store1 >= 5 && store2 < 5) {
        // Player 1 wins
        WIN1 += 1;      // increase WIN by 1
        endround();     // write code for what happens at the end of each round
      }
      else if (store1 < 5 && store2 >= 5) {
        // Player 2 wins
        WIN2 += 1;      // increase WIN by 1
        endround();
      }
      else {
        // Both blew each other up
        endround();
      }
    }
    // clear their storage
    store1 = 0;
    store2 = 0;
    // continue game
  }
  
  // GUN, RELOAD case
  else if (player1 + player2 == GUN + RELOAD){
    // player with gun wins
    if (player1 == GUN) {
      // Player 1 wins
      WIN1 += 1;
      endround();
    }
    else {
      // Player 2 wins
      WIN2 += 1;
      endround();
    }
  }
  
  // GUN, SHIELD case
  else if (player1 + player2 == GUN + SHIELD){
    // determine which one had a gun
    if (player1 == GUN){
      if (store1 >= 5) {
        // Player 1 wins
        WIN1 += 1;
        endround();
      }
      else {
        // game continues
        store1 = 0;
      }
    }
    else {
      if (store2 >= 5) {
        // Player 2 wins
        WIN2 += 1;
        endround();
      }
      else {
        // game continues
        store2 = 0;
      }
    }
  }
  
  // RELOAD, RELOAD or RELOAD, SHIELD case
  else if (player1 + player2 == RELOAD + SHIELD || player1 + player2 == RELOAD + RELOAD) {
    // if RELOAD, add to that players store
    if (player1 == RELOAD) {
      store1 += 1;
    }
    if (player2 == RELOAD) {
      store2 += 1;
    }
  }
  
  // SHIELD, SHIELD case
  else {
    // this is the shield all case so nothing...
  }
}


//Function displayScore will print score
//of each player onto LCD
void displayScore(){
  lcd.setCursor(10,0);
  lcd.print(WIN1);
  lcd.setCursor(10, 1)
  lcd.print(WIN2);
}
