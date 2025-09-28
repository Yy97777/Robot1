//#include <analogWrite.h>

#include <Ps3Controller.h>
#include <ESP32Servo.h>

//....................Motor pin number setting
#define LFN 13
#define LFP 12
#define LBN 14
#define LBP 27
#define RBP 4
#define RBN 16
#define RFP 17
#define RFN 18

//....................Servo pin number setting
#define mid 19
#define Left  21
#define Right 22
#define air 23
#define valve 5


int Left_stick_x = 0, Left_stick_y = 0, Right_stick_x = 0, Right_stick_y = 0;

int Motor_Speed = 150;//The value range is 0~250, the larger the value, the faster the motor rotates.


Servo msM, msL, msR, msA, msV; //宣告5個伺服馬達的物件名稱，ms=my servo,後面字母分別區分為左,中,右伺服馬達，抽氣馬達與電子氣閥。

int mL = 90, mM = 95, mR = 90;

//初始位置 mL = 90, mM = 95, mR = 90;
//平台吸方塊 mL = 70, mM = 95, mR = 120;
//地板吸方塊 mL = 60, mM = 95, mR = 165;
//收 mL = 125, mM = 95, mR = 115;S
void setup()
{
  //..............................Motor pin setting
  pinMode(LFN, OUTPUT);
  pinMode(LFP, OUTPUT);
  pinMode(LBN, OUTPUT);
  pinMode(LFP, OUTPUT);
  pinMode(RBP, OUTPUT);
  pinMode(RBN, OUTPUT);
  pinMode(RFP, OUTPUT);
  pinMode(RFN, OUTPUT);

  //...........................設定伺服馬達的腳位與脈衝(脈衝可設定也可不設定)，可允許的脈衝範圍是500~2400µs
  msM.attach(mid, 500, 2400); // 修正脈衝寬度範圍500~2500
  msL.attach(Left, 500, 2400);
  msR.attach(Right, 500, 2400);
  msA.attach(air, 500, 2400);
  msV.attach(5, 500, 2400);

  //...........................設定伺服馬達的初始位置，機械手臂的預設值角度90為手臂在正中間的位置。
  msM.write(mM);
  msL.write(mL);
  msR.write(mR);
  delay(1000);

  //.....................................Stop the operation of the motor
  analogWrite(LFN, 0); //front left
  analogWrite(LFP, 0);
  analogWrite(LBN, 0); //left rear
  analogWrite(LBP, 0);
  analogWrite(RBP, 0); //front right
  analogWrite(RBN, 0);
  analogWrite(RFP, 0); //right rear
  analogWrite(RFN, 0);
///Ps3.begin("20:22:09:29:19:03");

  Serial.begin(115200);
  Ps3.begin("20:22:09:16:16:07");
  Serial.println("Ready.");

}

void loop() { /**/

  if (Ps3.isConnected()) {//..............當手把連線時
    Left_stick_y = Ps3.data.analog.stick.ly;
    Left_stick_x = Ps3.data.analog.stick.lx;
    Right_stick_y = Ps3.data.analog.stick.ry;
    Right_stick_x = Ps3.data.analog.stick.rx;

    // Serial.print("LX: ");
    // Serial.print(Left_stick_x);
    // Serial.print("   ");
    // Serial.print("LY: ");
    // Serial.print(Left_stick_y);
    // Serial.print("   ");
    // Serial.print("RX: ");
    // Serial.print(Right_stick_x);
    // Serial.print("   ");
    // Serial.print("RY: ");
    // Serial.println(Right_stick_y);

    //........................................left motor
    if (Left_stick_y < -100)       Left_forward();
    else if (Left_stick_y > 100)   Left_backward();
    else if (Left_stick_x > 100)   Left_outward();
    else if (Left_stick_x < -100)  Left_inward();
    else                           Left_stop();
    //........................................Right motor
    if (Right_stick_y < -100)      Right_forward();
    else if (Right_stick_y > 100)  Right_backward();
    else if (Right_stick_x > 100)  Right_inward();
    else if (Right_stick_x < -100) Right_outward();
    else                           Right_stop();
    delay(1);

    if ( Ps3.data.button.cross ) {//......當X被按到，手臂向前
      if (mL < 175) {
        mL = mL + 2;
        msL.write(mL);
        delay(10);
      }
    }
    else if ( Ps3.data.button.triangle ) {//...當△被按到，手臂向後
      if (mL > 5) {
        mL = mL - 2;
        msL.write(mL);
        delay(10);
      }
    }
//    else if ( Ps3.data.button.square ) {//.....當□被按到，手臂向左
//      if (mM < 175) {
//        mM = mM + 2;
//        msM.write(mM);
//        delay(10);
//      }
//    }
//    else if ( Ps3.data.button.circle ) {//.....當○被按到，手臂向右
//      if (mM > 5) {
//        mM = mM - 2;
//        msM.write(mM);
//        delay(10);
//      }
//    }
    else if ( Ps3.data.button.square ) {//.....當□被按到，手臂向左
      mL = 125, mM = 95, mR = 115;
      msL.write(mL);
      delay(500);
      msR.write(mR);
    }
    else if ( Ps3.data.button.circle ) {//.....當○被按到，手臂向右
      mL = 70, mM = 95, mR = 110;
      msR.write(mR);
      delay(500);
      msL.write(mL);
    }
    else if ( Ps3.data.button.right ) {//.........當右被按到，手臂向上
      mL = 60, mM = 95, mR = 165;
      msL.write(mL);
      delay(500);
      msR.write(mR);
      
    }
    else if ( Ps3.data.button.down ) {//.........當↑被按到，手臂向上
      if (mR < 170) {
        mR = mR + 2;
        msR.write(mR);
        delay(50);
      }
    }
    else if ( Ps3.data.button.up ) {//.......當↓被按到，手臂向下
      if (mR > 20) {
        mR = mR - 2;
        msR.write(mR);
        delay(50);
      }
    }
   
    //     (11的)  delay(1000);
//      msA.write(0);//停止吸氣
//      msV.write(0);
//      delay(100);

//      msA.write(170);
//      msV.write(0);
//      delay(100);
//      msA.write(0);
//      msV.write(0);
//      delay(100);

    else if ( Ps3.data.button.l1 ) {//......當L1被按到，吸氣
      Serial.println("吸氣");
      msA.write(170);//吸氣
      msV.write(0);//關氣閥
    }
    else if ( Ps3.data.button.r1 ) {//...當R1被按到，洩氣
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
  analogWrite(LFN, 0); //front left
  analogWrite(LFP, 0);
  analogWrite(LBN, 0); //left rear
  analogWrite(LBP, 0);
}
void Left_backward()
{
  analogWrite(LFN, 0); //front left，forward
  analogWrite(LFP, Motor_Speed);
  analogWrite(LBN, 0); //left rear，forward
  analogWrite(LBP, Motor_Speed);
}
void Left_forward()
{
  analogWrite(LFN, Motor_Speed); //front left，backward
  analogWrite(LFP, 0);
  analogWrite(LBN, Motor_Speed); //left rear，backward
  analogWrite(LBP, 0);
}
void Left_outward()
{
  analogWrite(LFN, Motor_Speed); //front left，inward
  analogWrite(LFP, 0);
  analogWrite(LBN, 0); //left rear，inward
  analogWrite(LBP, Motor_Speed);
}
void Left_inward()
{
  analogWrite(LFN, 0); //front left，outward
  analogWrite(LFP, Motor_Speed);
  analogWrite(LBN, Motor_Speed); //left rear，outward
  analogWrite(LBP, 0);
}
//..........................................right

void Right_stop()
{
  analogWrite(RBP, 0); //front right
  analogWrite(RBN, 0);
  analogWrite(RFP, 0); //right rear
  analogWrite(RFN, 0);
}
void Right_forward()
{
  analogWrite(RBP, Motor_Speed); //front right，forward
  analogWrite(RBN, 0);
  analogWrite(RFP, Motor_Speed); //right rear，forward
  analogWrite(RFN, 0);
}
void Right_backward()
{
  analogWrite(RBP, 0); //front right，backward
  analogWrite(RBN, Motor_Speed);
  analogWrite(RFP, 0); //right rear，backward
  analogWrite(RFN, Motor_Speed);
}
void Right_inward()
{
  analogWrite(RBP, 0); //front right，inward
  analogWrite(RBN, Motor_Speed);
  analogWrite(RFP, Motor_Speed); //right rear，inward
  analogWrite(RFN, 0);
}
void Right_outward()
{
  analogWrite(RBP, Motor_Speed); //front right，outward
  analogWrite(RBN, 0);
  analogWrite(RFP, 0); //right rear，outward
  analogWrite(RFN, Motor_Speed);
}
