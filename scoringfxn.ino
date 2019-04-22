# define GUN 0
# define SHIELD 1
# define RELOAD 3

int store1 = 0;
int store2 = 0;
int WIN1 = 0;
int WIN2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  scoring(3,1);
  Serial.print("Stores");
  Serial.print(store1);
  Serial.println(store2);
  
  delay(1000);

  scoring(3,3);
  Serial.print("Stores");
  Serial.print(store1);
  Serial.println(store2);

  
  delay(1000);

  scoring(3,0);
  Serial.print("Stores");
  Serial.print(store1);
  Serial.println(store2);

  
  delay(1000);
}

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


// fxn at the end of each round -- updates the LCD, clears variables, creates a delay for players to regroup, and gives it the beat to restart
void endround() {
  // updates the LCD
  
  
  // clears reload variables for next round
  store1 = 0;
  store2 = 0;
  
  // delay for regroup (via sound)
  // playcomplete()
  
  
}
