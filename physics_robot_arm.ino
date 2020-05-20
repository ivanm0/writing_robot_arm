#include <Servo.h>

Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;

double armLength = 12.0; //cm
double shortestLength = 0.05;
double minDegreesPerLoop = 0.01;

double vertices[40][2];// = {{-13, 18}, {-13, 12}, {-13, 15}, {-11, 15}, {-11, 18}, {-11, 12}};// = { {-4, 22}, {-4, 10}, {-4, 16}, {-1, 16}, {-1, 10}, {-1, 22}, {0, 0}, {1, 22}, {4, 22}, {2.5, 22}, {2.5, 10}, {1, 10}, {4, 10} };
String toWrite = "MATH";
int alphabet[][8] = {{7, 1, 3, 9, 6, 4, 0, 0},
                    {7, 1, 3, 6, 4, 6, 9, 7},
                    {3, 1, 7, 9, 0, 0, 0, 0},
                    {7, 1, 6, 7, 0, 0, 0, 0},
                    {3, 1, 4, 5, 4, 7, 9, 0},
                    {3, 1, 4, 5, 4, 7, 0, 0},
                    {3, 1, 7, 9, 6, 5, 0, 0},
                    {1, 7, 4, 6, 3, 9, 0, 0},
                    {1, 3, 2, 8, 7, 9, 0, 0},
                    {1, 3, 2, 8, 7, 4, 0, 0},
                    {1, 7, 4, 3, 4, 9, 0, 0},
                    {1, 7, 9, 0, 0, 0, 0, 0},
                    {7, 1, 5, 3, 9, 0, 0, 0},
                    {7, 1, 9, 3, 0, 0, 0, 0},
                    {1, 3, 9, 7, 1, 0, 0, 0},
                    {4, 6, 3, 1, 7, 0, 0, 0},
                    {9, 7, 1, 3, 9, 5, 0, 0},
                    {7, 1, 3, 6, 4, 9, 0, 0},
                    {3, 1, 4, 6, 9, 7, 0, 0},
                    {1, 3, 2, 8, 0, 0, 0, 0},
                    {1, 7, 9, 3, 0, 0, 0, 0},
                    {1, 8, 3, 0, 0, 0, 0, 0},
                    {1, 7, 5, 9, 3, 0, 0, 0},
                    {1, 9, 5, 3, 7, 0, 0, 0},
                    {1, 5, 3, 7, 0, 0, 0, 0},
                    {1, 3, 7, 9, 0, 0, 0, 0}
                    };
                    
double lastPos[4];
int numVertices;
boolean firstLoop;
boolean drawn;

void setup() {
  servo0.attach(8);
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);
  Serial.begin(9600);
  
  char letters[toWrite.length()];
  toWrite.toCharArray(letters, toWrite.length()+1);
  Serial.println((int)letters[5]);
  int index = 0;
  for(int i = 0; i < sizeof(letters); i++) {
    for(int j = 0; j < 8; j++) {
      if (letters[i] != 32) {
        switch (alphabet[letters[i]-65][j]) {
          case 0:
            break;
          case 1:
          case 4:
          case 7:
            vertices[index][0] = -12+i*4;
            break;
          case 2:
          case 5:
          case 8:
            vertices[index][0] = -10.5+i*4;
            break;
          case 3:
          case 6:
          case 9:
            vertices[index][0] = -9+i*4;
            break;
        }
        switch (alphabet[letters[i]-65][j]) {
          case 0:
            break;
          case 1:
          case 2:
          case 3:
            vertices[index][1] = 18;
            index++;
            break;
          case 4:
          case 5:
          case 6:
            vertices[index][1] = 15;
            index++;
            break;
          case 7:
          case 8:
          case 9:
            vertices[index][1] = 12;
            index++;
            break;
        }
        if (j == 7) {
          vertices[index][0] = 0;
          vertices[index][1] = 0;
          index++;
        }
      }
    }
  }

  numVertices = sizeof(vertices)/sizeof(vertices[0]);
  for(int m = 0; m < numVertices; m++) {
    Serial.println((String)vertices[m][0] + ", " + (String)vertices[m][1]);
  }

  lastPos[0] = atan2(vertices[0][1], vertices[0][0]) * 180 / PI;
  lastPos[1] = 79 + acos(sqrt(vertices[0][0] * vertices[0][0] + vertices[0][1] * vertices[0][1]) / 2 / armLength) * 180 / PI;
  lastPos[2] = 2.7 + 2 * acos(sqrt(vertices[0][0] * vertices[0][0] + vertices[0][1] * vertices[0][1]) / 2 / armLength) * 180 / PI;
  lastPos[3] = 82 + acos(sqrt(vertices[0][0] * vertices[0][0] + vertices[0][1] * vertices[0][1]) / 2 / armLength) * 180 / PI;

  servo0.write(lastPos[0]);
  servo1.write(lastPos[1]);
  servo2.write(lastPos[2]);
  servo3.write(lastPos[3]);
  Serial.println((String)lastPos[0] + ", " + (String)lastPos[1] + ", " + (String)lastPos[2] + ", " + (String)lastPos[3]);
  delay(5000);
  
  firstLoop = true;
  drawn = false;
}

void loop() {
  if (!drawn) {
    for (int i = 0; i < numVertices-1; i++) {
      if (vertices[i+1][0] == 0 && vertices[i+1][1] == 0) {
        if (i < numVertices-2 && vertices[i+2][0] != 0 && vertices[i+2][1] != 0) {
          double totLength = sqrt((vertices[i][0] - vertices[i+2][0]) * (vertices[i][0] - vertices[i+2][0]) + (vertices[i][1] - vertices[i+2][1]) * (vertices[i][1] - vertices[i+2][1]));
          int numSegs = ceil(totLength / shortestLength);
          double segX = (vertices[i+2][0] - vertices[i][0]) / numSegs;
          double segY = (vertices[i+2][1] - vertices[i][1]) / numSegs;
          double currentPos[4];
          currentPos[1] = lastPos[1] + 20;
          servo3.write(currentPos[3]);
          delay(500);
          for (int j = 1; j <= numSegs; j++) {
            currentPos[0] = atan2(vertices[i][1] + segY * j, vertices[i][0] + segX * j) * 180 / PI;
            currentPos[1] = 79 + acos(sqrt((vertices[i][0]+segX*j) * (vertices[i][0]+segX*j) + (vertices[i][1]+segY*j) * (vertices[i][1]+segY*j)) / 2 / armLength) * 180 / PI;
            currentPos[2] = 2.7 + 2*acos(sqrt((vertices[i][0]+segX*j) * (vertices[i][0]+segX*j) + (vertices[i][1]+segY*j) * (vertices[i][1]+segY*j)) / 2 / armLength) * 180 / PI;
            currentPos[3] = 82 + acos(sqrt((vertices[i][0]+segX*j) * (vertices[i][0]+segX*j) + (vertices[i][1]+segY*j) * (vertices[i][1]+segY*j)) / 2 / armLength) * 180 / PI;
            if (j != numSegs) {
              currentPos[1] += 20;
            }
            
            servo0.write(currentPos[0]);
            servo1.write(currentPos[1]);
            servo2.write(currentPos[2]);
            servo3.write(currentPos[3]);
    
            int index = 0;
            for (int k = 1; k < 4; k++) {
              if (abs(currentPos[k]-lastPos[k]) > abs(currentPos[index]-lastPos[index])) {
                index = k;
              }
            }
            
            delay(abs(currentPos[index]-lastPos[index]) * 100 / 6);
            for (int l = 0; l < 4; l++) {
              lastPos[l] = currentPos[l];
            }
          }
        }
      } else if (vertices[i][0] == 0 && vertices[i][1] == 0) {
        
      } else {
        double totLength = sqrt((vertices[i][0] - vertices[i+1][0]) * (vertices[i][0] - vertices[i+1][0]) + (vertices[i][1] - vertices[i+1][1]) * (vertices[i][1] - vertices[i+1][1]));
        int numSegs = ceil(totLength / shortestLength);
        double segX = (vertices[i+1][0] - vertices[i][0]) / numSegs;
        double segY = (vertices[i+1][1] - vertices[i][1]) / numSegs;
        for (int j = 1; j <= numSegs; j++) {
          double currentPos[4];
          currentPos[0] = atan2(vertices[i][1] + segY * j, vertices[i][0] + segX * j) * 180 / PI;
          currentPos[1] = 79 + acos(sqrt((vertices[i][0]+segX*j) * (vertices[i][0]+segX*j) + (vertices[i][1]+segY*j) * (vertices[i][1]+segY*j)) / 2 / armLength) * 180 / PI;
          currentPos[2] = 2.7 + 2*acos(sqrt((vertices[i][0]+segX*j) * (vertices[i][0]+segX*j) + (vertices[i][1]+segY*j) * (vertices[i][1]+segY*j)) / 2 / armLength) * 180 / PI;
          currentPos[3] = 82 + acos(sqrt((vertices[i][0]+segX*j) * (vertices[i][0]+segX*j) + (vertices[i][1]+segY*j) * (vertices[i][1]+segY*j)) / 2 / armLength) * 180 / PI;
          
          servo0.write(currentPos[0]);
          servo1.write(currentPos[1]);
          servo2.write(currentPos[2]);
          servo3.write(currentPos[3]);
  
          int index = 0;
          for (int k = 1; k < 4; k++) {
            if (abs(currentPos[k]-lastPos[k]) > abs(currentPos[index]-lastPos[index])) {
              index = k;
            }
          }
          
          delay(abs(currentPos[index]-lastPos[index]) * 150 / 6);
          for (int l = 0; l < 4; l++) {
            lastPos[l] = currentPos[l];
          }
        }
      }
    }
    drawn = true;
    servo1.write(160);
    servo2.write(20);
    servo3.write(95);
  }

//  servo0.write(90);
//  servo1.write(170);
//  servo2.write(95);
//  servo3.write(82);
//  delay(6000);
//  servo0.write(180);
//  servo1.write(180);
//  servo2.write(180);
//  servo3.write(180);
//  delay(3000);  
}
