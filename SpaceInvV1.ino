//Jeremiah Frank Kalmus
#include "Adafruit_HT1632.h"
#define HT_DATA 2
#define HT_WR   3
#define HT_CS   4
#define HT_CS2  5

Adafruit_HT1632LEDMatrix matrix = 
Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS);

int rightBut = 9, leftBut = 8;
int t = 0, curDir = 1, pixlX = 23, pixlY = 0;
int UFOx = 0, UFOy = 15; 
void setup() {
  Serial.begin(9600);
  matrix.begin(ADA_HT1632_COMMON_16NMOS);
//  pinMode(rightBut, INPUT);
  pinMode(leftBut, INPUT);
}

void loop() {
  int anlgX = analogRead(A1);
  int anlgY = analogRead(A0);
  bool valL = digitalRead(leftBut);
  // int missilePlayerX = -1;
  // int missilePlayerY = -1;

  //move pixel

  if (t % 10 == 0) {
    clrPlayerShip();
    clrUFO();
    (pixlX, pixlY) = movePlayerShip(pixlX, pixlY);
    (UFOx, UFOy) = moveUFO(UFOx, UFOy);
    setPlayerShip();
    setUFO();
    
    if(valL == 1){
      int missilePlayerX = pixlX - 2;
      int missilePlayerY = pixlY;
      matrix.setPixel(missilePlayerX, missilePlayerY);
      while (missilePlayerX >= 0){
          matrix.clrPixel(missilePlayerX, missilePlayerY);
          missilePlayerX = missilePlayerX - 1;
          matrix.setPixel(missilePlayerX, missilePlayerY);
          matrix.writeScreen();
        }
    //matrix.setPixel(missilePlayerX, missilePlayerY);
    }
   // matrix.setPixel(missilePlayerX, missilePlayerY);
  }

  //UFOs
  

  //draw and incriment time
  matrix.writeScreen();
  if (t == 60) t = 0;
  t++;
}

int movePlayerShip(int pixlX, int pixlY) {
  int curDir = 1;
  int anlgX = analogRead(A1);
  int anlgY = analogRead(A0);
  
  if (t % 10 == 0) {
    if (anlgX < 400)  curDir = 1;
    else if (anlgX > 600) curDir = 2;
    else curDir = 0;
  }
  //matrix.clrPixel(pixlX, pixlY);

  if(curDir == 1) pixlY++;
  else if(curDir == 2) pixlY--;

  if (pixlY > 15) pixlY = 15;
  if (pixlY < 0) pixlY = 0;

  return pixlX, pixlY;
}

void setPlayerShip(){
  int pixlLeftY = pixlY - 1, pixlLeftX = pixlX,
  pixlRightY = pixlY + 1, pixlRightX = pixlX,
  pixlTopY = pixlY, pixlTopX = pixlX - 1;
  matrix.setPixel(pixlX, pixlY), matrix.setPixel(pixlLeftX, pixlLeftY), matrix.setPixel(pixlRightX, pixlRightY), matrix.setPixel(pixlTopX, pixlTopY);
}

void clrPlayerShip(){
  int pixlLeftY = pixlY - 1, pixlLeftX = pixlX,
  pixlRightY = pixlY + 1, pixlRightX = pixlX,
  pixlTopY = pixlY, pixlTopX = pixlX - 1;
  matrix.clrPixel(pixlX, pixlY), matrix.clrPixel(pixlLeftX, pixlLeftY), matrix.clrPixel(pixlRightX, pixlRightY), matrix.clrPixel(pixlTopX, pixlTopY);
}

int moveUFO(int UFOx, int UFOy){
 while(UFOx < 21){
   if(UFOx % 2 == 0){
     while(UFOy > 0){
     UFOy--;
    }
    UFOx++;
    }
   else if(UFOx % 2 == 1){
   while(UFOy < 15){
     UFOy++;
   }
   UFOx++;
  }
 }
 return UFOx,UFOy;
}

void setUFO(){
  int UFOLeftY = UFOy - 1, UFOLeftX = UFOx,
  UFORightY = UFOy + 1, UFORightX = UFOx;
  matrix.setPixel(UFOx, UFOy), matrix.setPixel(UFOLeftX, UFOLeftY), matrix.setPixel(UFORightX, UFORightY);
}
void clrUFO(int UFOx, int UFOy){
  int UFOLeftY = UFOy - 1, UFOLeftX = UFOx,
  UFORightY = UFOy + 1, UFORightX = UFOx;
  matrix.clrPixel(UFOx, UFOy), matrix.clrPixel(UFOLeftX, UFOLeftY), matrix.clrPixel(UFORightX, UFORightY);
}

