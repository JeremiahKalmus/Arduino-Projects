//Dominic Burgi
#include <QueueArray.h>
#include "Adafruit_HT1632.h"
#define HT_DATA 2
#define HT_WR   3
#define HT_CS   4
#define HT_CS2  5

Adafruit_HT1632LEDMatrix matrix =
Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS);

int rightBut = 8, leftBut = 9;
int oldX, oldY, foodX, foodY, checkX, checkY, t, curDir, len, headX, headY;
bool lose = 1;
QueueArray<int>horiz;
QueueArray<int>vert;

//letter Arrays
int greenX[] = {23, 23, 23, 23, 23, 22, 22, 21, 21, 21, 20, 20, 20, 20, //G
                18, 18, 18, 18, 18, 17, 17, 16, 16, 16, 15, 15, 15, 15, //R
                13, 13, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 10, //E
                8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5,               //E
                3, 3, 3, 3, 3, 2, 1, 0, 0, 0, 0, 0};                    //N

int greenY[] = {12, 11, 10, 9, 8, 12, 8, 12, 10, 8, 12, 10, 9, 8,       //G
                12, 11, 10, 9, 8, 12, 10, 12, 10, 9, 12, 11, 10, 8,     //R
                12, 11, 10, 9, 8, 12, 10, 8, 12, 10, 8, 12, 10, 8,      //E
                12, 11, 10, 9, 8, 12, 10, 8, 12, 10, 8, 12, 10, 8,      //E
                12, 11, 10, 9, 8, 11, 10, 12, 11, 10, 9, 8};            //N

int playX[] = {23, 23, 23, 23, 23, 22, 22, 21, 21, 20, 20, 20,          //P
               18, 18, 18, 18, 18, 17, 16, 15,                          //L
               13, 13, 13, 13, 13, 12, 12, 11, 11, 10, 10, 10, 10, 10,  //A
               8, 7, 6, 6, 6, 5, 4,                                     //Y
               2, 1, 1, 1, 0, 0, 0};                                    //?

int playY[] = {12, 11, 10, 9, 8, 12, 10, 12, 10, 12, 11, 10,            //P
               12, 11, 10, 9, 8, 8, 8, 8,                               //L
               12, 11, 10, 9, 8, 12, 10, 12, 10, 12, 11, 10, 9, 8,      //A
               12, 11, 10, 9, 8, 11, 12,                                //Y
               12, 12, 10, 8, 12, 11, 10};                              //?

void setup() {
  Serial.begin(9600);
  matrix.begin(ADA_HT1632_COMMON_16NMOS);
  pinMode(rightBut, INPUT);
  pinMode(leftBut, INPUT);
  randomSeed(analogRead(A2));
}

void loop() {
  matrix.clearScreen();
  while (lose == 1) {
    for (int i = 0; i < 68; i++){
      matrix.setPixel(greenX[i],greenY[i]-6);
    }
    for (int i = 0; i < 53; i++){
      matrix.setPixel(playX[i],playY[i]+1);
    }
    matrix.writeScreen();
    bool valL = digitalRead(leftBut);
    if (valL == 1){
      lose = 0;
      matrix.clearScreen();
    }
  }
  t = 0, curDir = 1, len = 4, headX = 0, headY = 0;
  for (int i = 0; i < len; i++) {
    horiz.enqueue(headX);
    vert.enqueue(headY);
  }
  foodX = random(24);
  foodY = random(16);
  matrix.setPixel(foodX, foodY);

  while (lose == 0) {
    int angX = analogRead(A1);
    int angY = analogRead(A0);

    //move pixel
    if (t % 15 == 0) {
      if ((angX < 300) && (curDir != 2)) curDir = 1;
      else if ((angX > 700) && (curDir != 1)) curDir = 2;
      else if ((angY < 300) && (curDir != 4)) curDir = 3;
      else if ((angY > 700) && (curDir != 3)) curDir = 4;

      if (curDir == 1) headX++;
      else if (curDir == 2) headX--;
      else if (curDir == 3) headY--;
      else if (curDir == 4) headY++;

      matrix.setPixel(foodX, foodY);
      matrix.setPixel(headX, headY);
      for (int i = 0; i < len; i++) {
        horiz.enqueue(headX);
        vert.enqueue(headY);
        headX = horiz.dequeue();
        headY = vert.dequeue();
      }

      oldX = headX;
      oldY = headY;
      headX = horiz.front();
      headY = vert.front();
      if ((oldX == foodX) && (oldY == foodY)) {
        len++;
        foodX = random(24);
        foodY = random(16);
        horiz.enqueue(oldX);
        vert.enqueue(oldY);
      }
      else matrix.clrPixel(oldX, oldY);
    }

    //check borders
    if (headX > 23 || headY > 15 || headX < 0 || headY < 0) lose = 1;

    //check body collision
    checkX = horiz.dequeue();
    checkY = vert.dequeue();
    horiz.enqueue(checkX);
    vert.enqueue(checkY);
    for (int i = 0; i < len - 1; i++) {
      checkX = horiz.dequeue();
      checkY = vert.dequeue();
      if ((checkX == headX) && (checkY == headY))
        lose = 1;
      horiz.enqueue(checkX);
      vert.enqueue(checkY);
    }

    //draw and incriment time
    matrix.writeScreen();
    if (t == 60) t = 0;
    t++;
  }
  
  //clean up
  while (!horiz.isEmpty ()) {
    horiz.dequeue();
    vert.dequeue();
  }
  delay(2000);
}
