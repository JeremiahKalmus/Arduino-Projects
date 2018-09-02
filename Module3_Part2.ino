#include "Adafruit_HT1632.h"
#define HT_DATA 2
#define HT_WR   3
#define HT_CS   4
#define HT_CS2  5

Adafruit_HT1632LEDMatrix matrix =
  Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS);

int leftBut = 9, score = 1;
int pixlPositionX, pixlPositionY;
int diagX, diagY, lowX, lowY, leftX, leftY;
int irandom = random(0, 13), jrandom = random(4, 17);
unsigned long timer;
unsigned long penalty;


void setup() {
  Serial.begin(9600);
  matrix.begin(ADA_HT1632_COMMON_16NMOS);
  pinMode(leftBut, INPUT);
  randomSeed(analogRead(0));
}

void loop() {

  int angY = analogRead(A0);
  int angX = analogRead(A1);
  //Serial.print(angX);
  //Serial.print(' ');
  //Serial.println(angY);
  bool valL = digitalRead(leftBut); 
  //matrix.setPixel(jrandom, irandom);
 
  Serial.println(valL);
  startTime(timer);

  endTime(timer, valL, penalty);
  
  matrix.drawRect(jrandom, irandom, 4, 4, 1);

  //Prints the boarder LED's
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 24; j++) {
      if (j < 4 || j > 19) {
        matrix.setPixel(j, i);
      }
    }
  }

  //Clears the old pixels
  matrix.clrPixel(pixlPositionX, pixlPositionY);
  matrix.clrPixel(diagX, diagY);
  matrix.clrPixel(lowX, lowY);
  matrix.clrPixel(leftX, leftY);

  //making the lead pixel
  pixlPositionX = ((angX / 48) + 1);
  pixlPositionY = ((angY / 48) - 2);

  //borders
  if (pixlPositionX > 18) pixlPositionX = 18;
  if (pixlPositionX < 4) pixlPositionX = 4;
  if (pixlPositionY < 1) pixlPositionY = 1;
  if (pixlPositionY > 15) pixlPositionY = 15;

  //making the other 3 pixels
  diagY = pixlPositionY - 1;
  diagX = pixlPositionX + 1;
  lowY = pixlPositionY - 1;
  lowX = pixlPositionX;
  leftY = pixlPositionY;
  leftX = pixlPositionX + 1;

  //setting the 2x2
  matrix.setPixel(pixlPositionX, pixlPositionY);
  matrix.setPixel(diagX, diagY);
  matrix.setPixel(lowX, lowY);
  matrix.setPixel(leftX, leftY);


  if ((valL == 1) && (pixlPositionX == jrandom + 1) && (pixlPositionY == irandom + 2)) score = 1;
  else score = 0;
  if (score == 1) {
 //   matrix.clrPixel(jrandom, irandom);
  matrix.drawRect(jrandom, irandom, 4, 4, 0);
    irandom = random(0, 13);
    jrandom = random(4, 17);
     matrix.drawRect(jrandom, irandom, 4, 4, 1);
    score = 0;
  }

  //draw
  matrix.writeScreen();
}

void startTime(unsigned long timer){
 Serial.print("Start Time: ");
 timer = millis();
 Serial.print(' ');
 Serial.print(timer);
 Serial.print(' ');
 delay(1000);
}
void endTime(unsigned long timer, int valL, unsigned long penalty){

  if(valL == 1){
    delay(100);
    penalty = penalty + 5000;
  }
  Serial.print("End Time: ");
  timer = millis();
  timer = timer + penalty;
  Serial.println(timer);
  Serial.print(' ');
  Serial.print("Penalty Time: ");
  Serial.print(' ');
  Serial.println(penalty);
  delay(1000);
  
}



