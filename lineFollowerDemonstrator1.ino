#include <Pixy2.h>
#include <math.h>

Pixy2 pixy;

Servo leftMotor;
Servo rightMotor;

int16_t frameWidth;
int16_t frameHeight;

enum RightLeft {right, left};

struct AngleAndSide {
  float angle;
  RightLeft side;
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Starting...\n");

  // we need to initialize the pixy object
  pixy.init();
  // Change to line tracking program
  pixy.changeProg("line");
  pixy.setLamp(0xFF, 0xFF);

  pixy.getResolution();
  frameWidth  = pixy.frameWidth;
  frameHeight = pixy.frameHeight;

  Serial.print("Frame Width: ");
  Serial.println(frameWidth);
  Serial.print("Frame Height: ");
  Serial.println(frameHeight);

  leftMotor.attach( 9, MIN_SPEED, MAX_SPEED);
  rightMotor.attach(10, MIN_SPEED, MAX_SPEED);

  leftMotor.writeMicroseconds(1500);
  rightMotor.writeMicroseconds(1500);
}

void loop() {
  // put your main code here, to run repeatedly:

  char buf[128];

  pixy.line.getMainFeatures();
  
  // print all vectors
  if (pixy.line.numVectors > 0) {
    AngleAndSide angleAndSide = getAngleFromPixy(0);
    
    Serial.print("Angle: ");
    Serial.println(angleAndSide.angle);

    Serial.print("Side: ");
    if (angleAndSide.side == left) {
      Serial.println("left");
      leftMotor.writeMicroseconds(MAX_SPEED);
      righttMotor.writeMicroseconds(MIN_SPEED);
    }
    if (angleAndSide.side == right) {
      Serial.println("right");
      leftMotor.writeMicroseconds(MIN_SPEED);
      righttMotor.writeMicroseconds(MAX_SPEED);
    }

    Serial.print("X1: ");
    Serial.print(pixy.line.vectors[0].m_x1);
    Serial.print(" X0: ");
    Serial.println(pixy.line.vectors[0].m_x0);
    
  } else {
    Serial.println("no line seen");
    // Rotate left until you see a line
    leftMotor.writeMicroseconds(MAX_SPEED);
    righttMotor.writeMicroseconds(MIN_SPEED);
  }
}

AngleAndSide getAngleFromPixy(uint8_t vectorNum) {
  AngleAndSide toReturn;
  
  int16_t vectorWidth  = pixy.line.vectors[vectorNum].m_x1 -pixy.line.vectors[vectorNum].m_x0;
  int16_t vectorHeight = pixy.line.vectors[vectorNum].m_y1 -pixy.line.vectors[vectorNum].m_y0;

  toReturn.angle = 180*atan(vectorHeight/vectorWidth)/PI;
  if (pixy.line.vectors[vectorNum].m_x1 > pixy.line.vectors[vectorNum].m_x0) {
    toReturn.side  = right;
  } else if (pixy.line.vectors[vectorNum].m_x1 < pixy.line.vectors[vectorNum].m_x0) {
    toReturn.side  = left;
  }

  return toReturn;
}
