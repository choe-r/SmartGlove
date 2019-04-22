# define UP 0
# define DOWN 1

float A_TS_avg;
float B_TS_avg; 
int A;
int B;

int Accel_Position(float analogin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println("Boom");
delay(1000);
Serial.println("Boom");
delay(1000);

A_TS_avg = 0;
B_TS_avg = 0;

// average value over a sampling period
for (int i=1; i <=100; i++){
  A = analogRead(0);
  B = analogRead(1);
  A_TS_avg = (A_TS_avg * ((float)i-1) + (float)A)/(float)i;
  B_TS_avg = (B_TS_avg * ((float)i-1) + (float)B)/(float)i;
  delay(5);
}

// analog in value to a posistion
int A_TS = Accel_Position(A_TS_avg);
int B_TS = Accel_Position(B_TS_avg);

Serial.print(A_TS_avg);
Serial.print(" x ");
Serial.println(B_TS_avg);

//// temp fxn to output based on positions
//if (A_TS == 3 || B_TS == 3){
//  Serial.println("Error");
//}
//else if(A_TS == UP){
//  if (B_TS == UP){
//    Serial.println("up");
//  }
//  else {
//    Serial.println("down");
//  }
//}
//else {
//  if (B_TS == UP){
//    
//    Serial.println("Up");
//  }
//  else {
//    Serial.println("down");
//  }
//} 
//
//
//delay(500);

}


// fxn to determine what position the TS is in
int Accel_Position(float analogin){
  if(analogin >=150 && analogin <= 280){
    Serial.println("UP");
    return UP;
  }
  else if (analogin >= 300 && analogin<=400){
    Serial.println("DOWN");
    return DOWN;
  }
  else {
    return 3;
  }
}

