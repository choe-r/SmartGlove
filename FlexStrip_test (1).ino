#define OPEN 1
#define CLOSED 0


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  int A_FS_mid_avg = 0;
//  int B_FS_ind_avg = 0;

 for (int i=1; i <=100; i++){
   int FS_PLYR1_mid_AR = analogRead(0);
   int FS_PLYR1_ind_AR = analogRead(1);
  float A_FS_mid_avg = (A_FS_mid_avg * ((float)i-1) + (float)FS_PLYR1_mid_AR)/(float)i;
//  float B_FS_ind_avg = (B_FS_ind_avg * ((float)i-1) + (float)FS_PLYR1_mid_AR)/(float)i;
  delay(5);

  bool FS_P1_mid = flexing(A_FS_mid_avg);
//  bool FS_P1_ind = flexing(A_FS_ind_avg);

  Serial.println(FS_P1_mid);
 
}
}

bool flexing(float analoginFS){
  if (analoginFS >= 400){
    return OPEN;
  }
  else {
    return CLOSED;
  }
}

