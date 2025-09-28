//#include <analogWrite.h>

#include <Ps3Controller.h>
#include <ESP32Servo.h>

#define LFN 13
#define LFP 12
#define LBN 14
#define LBP 27
#define RBP 4
#define RBN 16
#define RFP 17
#define RFN 18


#define mid 19
#define Left  21
#define Right 22
#define air 23
#define valve 5


int Left_stick_x = 0, Left_stick_y = 0, Right_stick_x = 0, Right_stick_y = 0;

int Motor_Speed = 150;


Servo msM, msL, msR, msA, msV; 

int mL = 90, mM = 95, mR = 90;

void setup()
{

  pinMode(LFN, OUTPUT);
  pinMode(LFP, OUTPUT);
  pinMode(LBN, OUTPUT);
  pinMode(LFP, OUTPUT);
  pinMode(RBP, OUTPUT);
  pinMode(RBN, OUTPUT);
  pinMode(RFP, OUTPUT);
  pinMode(RFN, OUTPUT);

  msM.write(mM);
  msL.write(mL);
  msR.write(mR);
  delay(1000);


  analogWrite(LFN, 0); //front left
  analogWrite(LFP, 0);
  analogWrite(LBN, 0); 
  analogWrite(LBP, 0);
  analogWrite(RBP, 0); //front right
  analogWrite(RBN, 0);
  analogWrite(RFP, 0); 
  analogWrite(RFN, 0);
///Ps3.begin("20:22:09:29:19:03");

  Serial.begin(115200);
  Ps3.begin("20:22:09:16:16:07");
  Serial.println("Ready.");

}

void loop() {

  if (Ps3.isConnected()) {
    Left_stick_y = Ps3.data.analog.stick.ly;
    Left_stick_x = Ps3.data.analog.stick.lx;
    Right_stick_y = Ps3.data.analog.stick.ry;
    Right_stick_x = Ps3.data.analog.stick.rx;

    if (Left_stick_y < -100)       Left_forward();
    else if (Left_stick_y > 100)   Left_backward();
    else if (Left_stick_x > 100)   Left_outward();
    else if (Left_stick_x < -100)  Left_inward();
    else                           Left_stop();

    if (Right_stick_y < -100)      Right_forward();
    else if (Right_stick_y > 100)  Right_backward();
    else if (Right_stick_x > 100)  Right_inward();
    else if (Right_stick_x < -100) Right_outward();
    else                           Right_stop();
    delay(1);

    if ( Ps3.data.button.cross ) {
      if (mL < 175) {
        mL = mL + 2;
        msL.write(mL);
        delay(10);
      }
    }
    else if ( Ps3.data.button.triangle ) {
      if (mL > 5) {
        mL = mL - 2;
        msL.write(mL);
        delay(10);
      }
    }
//    else if ( Ps3.data.button.square ) {
//      if (mM < 175) {
//        mM = mM + 2;
//        msM.write(mM);
//        delay(10);
//      }
//    }
//    else if ( Ps3.data.button.circle ) {
//      if (mM > 5) {
//        mM = mM - 2;
//        msM.write(mM);
//        delay(10);
//      }
//    }
    else if ( Ps3.data.button.square ) {
      mL = 125, mM = 95, mR = 115;
      msL.write(mL);
      delay(500);
      msR.write(mR);
    }
    else if ( Ps3.data.button.circle ) {
      mL = 70, mM = 95, mR = 110;
      msR.write(mR);
      delay(500);
      msL.write(mL);
    }
    else if ( Ps3.data.button.right ) {
      mL = 60, mM = 95, mR = 165;
      msL.write(mL);
      delay(500);
      msR.write(mR);
      
    }
    else if ( Ps3.data.button.down ) {
      if (mR < 170) {
        mR = mR + 2;
        msR.write(mR);
        delay(50);
      }
    }
    else if ( Ps3.data.button.up ) {
      if (mR > 20) {
        mR = mR - 2;
        msR.write(mR);
        delay(50);
      }
    }


    else if ( Ps3.data.button.l1 ) {
      Serial.println("吸氣");
      msA.write(170);//吸氣
      msV.write(0);//關氣閥
    }
    else if ( Ps3.data.button.r1 ) {
      Serial.println("洩氣");
      msA.write(0);
      msV.write(170);
      delay(300);
    }
    // else if ( Ps3.data.button.r2 ) {
    //   mM = 85;
    //   mL = 90;
    //   mR = 90;
    //   msM.write(mM);
    //   msL.write(mL);
    //   msR.write(mR);
    //   delay(600);
    // }
    // else if ( Ps3.data.button.l2 ) {
    //   mM = 85;
    //   mL = 8;
    //   mR = 164;
    //   msM.write(mM);
    //   msL.write(mL);
    //   msR.write(mR);
    //   delay(600);
    // }
    else {    }
        Serial.print("Up_Down: ");
        Serial.print(mL);
        Serial.print("   ");
        Serial.print("Left_Right: ");
        Serial.print(mM);
        Serial.print("   ");
        Serial.print("Forwad_Back: ");
        Serial.println(mR);
        delay(10);
  }
  else {
    Left_backward();
    Right_backward();
    delay(5);
    Left_stop();
    Right_stop();
    while (!Ps3.isConnected()) {}
  }
}
void Left_stop()
{
  analogWrite(LFN, 0); 
  analogWrite(LFP, 0);
  analogWrite(LBN, 0); 
  analogWrite(LBP, 0);
}
void Left_backward()
{
  analogWrite(LFN, 0); 
  analogWrite(LFP, Motor_Speed);
  analogWrite(LBN, 0); 
  analogWrite(LBP, Motor_Speed);
}
void Left_forward()
{
  analogWrite(LFN, Motor_Speed); 
  analogWrite(LFP, 0);
  analogWrite(LBN, Motor_Speed); 
  analogWrite(LBP, 0);
}
void Left_outward()
{
  analogWrite(LFN, Motor_Speed); 
  analogWrite(LFP, 0);
  analogWrite(LBN, 0); 
  analogWrite(LBP, Motor_Speed);
}
void Left_inward()
{
  analogWrite(LFN, 0); 
  analogWrite(LFP, Motor_Speed);
  analogWrite(LBN, Motor_Speed); 
  analogWrite(LBP, 0);
}


void Right_stop()
{
  analogWrite(RBP, 0); 
  analogWrite(RBN, 0);
  analogWrite(RFP, 0); 
  analogWrite(RFN, 0);
}
void Right_forward()
{
  analogWrite(RBP, Motor_Speed); 
  analogWrite(RBN, 0);
  analogWrite(RFP, Motor_Speed); 
  analogWrite(RFN, 0);
}
void Right_backward()
{
  analogWrite(RBP, 0); 
  analogWrite(RBN, Motor_Speed);
  analogWrite(RFP, 0); 
  analogWrite(RFN, Motor_Speed);
}
void Right_inward()
{
  analogWrite(RBP, 0); 
  analogWrite(RBN, Motor_Speed);
  analogWrite(RFP, Motor_Speed); 
  analogWrite(RFN, 0);
}
void Right_outward()
{
  analogWrite(RBP, Motor_Speed); 
  analogWrite(RBN, 0);
  analogWrite(RFP, 0); 
  analogWrite(RFN, Motor_Speed);
}

