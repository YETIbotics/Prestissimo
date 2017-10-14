
#include "XBOXRECV.h"
#include "usbhub.h"
#include "MDD10A.h"

USB Usb;
USBHub  Hub1(&Usb); // The controller has a built in hub, so this instance is needed
XBOXRECV Xbox(&Usb);
MDD10A mc;



void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // halt
  }
  Serial.print(F("\r\nXBOX Library Started"));
  
}

int loopcnt = 0;
float speedDiv = 1;
float maxDifference = 40;

void loop() {
  // put your main code here, to run repeatedly:
Usb.Task();

  float DriveLeftSpeed = 0;
  float DriveRightSpeed = 0;
  float RollerSpeed = 0;
{


    //127 is middle
    if (Xbox.getAnalogHat(LeftHatY) > 7500) {
      //Going backwards
      DriveLeftSpeed = map(Xbox.getAnalogHat(LeftHatY), 7500, 32767, 0, 255);
    }
    else if (Xbox.getAnalogHat(LeftHatY) < -7500)
    {
      //going forwards
      DriveLeftSpeed = map(Xbox.getAnalogHat(LeftHatY), -32767, -7500, -255, 0);
    }


if (Xbox.getAnalogHat(RightHatY) > 7500) {
      //Going backwards
      DriveRightSpeed = map(Xbox.getAnalogHat(RightHatY), 7500, 32767, 0, 255);
    }
    else if (Xbox.getAnalogHat(RightHatY) < -7500)
    {
      //going forwards
      DriveRightSpeed = map(Xbox.getAnalogHat(RightHatY), -32767, -7500, -255, 0);
    }

  if(Xbox.getButtonPress(R2) > 10){
    RollerSpeed = Xbox.getButtonPress(R2);
  }else{
    RollerSpeed = -1 * Xbox.getButtonPress(L2);
  }

  if (Xbox.getButtonClick(UP))
  {
    if(speedDiv < 1)
      speedDiv = speedDiv + .1;
  }

    if (Xbox.getButtonClick(DOWN))
  {
    if(speedDiv > 0)
      speedDiv = speedDiv - .1;
  }


  if (Xbox.getButtonClick(RIGHT))
  {
    if(maxDifference < 1)
      maxDifference = maxDifference + 5;
  }

    if (Xbox.getButtonClick(LEFT))
  {
    if(maxDifference > 0)
      maxDifference = maxDifference - 5;
  }
       
    
        loopcnt++;
  if (loopcnt >= 20)
  {
    loopcnt = 0;

    //DriveLeftSpeed = DriveLeftSpeed * -1;

    //DriveLeftSpeed = DriveLeftSpeed * abs(DriveLeftSpeed) / 255;
    //DriveRightSpeed = DriveRightSpeed * abs(DriveRightSpeed) / 255;

    DriveLeftSpeed = DriveLeftSpeed * speedDiv;
    DriveRightSpeed = DriveRightSpeed * speedDiv;

/*
    if(DriveRightSpeed >= 0 && DriveLeftSpeed >= 0)
    {
      if(abs(DriveRightSpeed - DriveLeftSpeed) > maxDifference)  
      {
          if(DriveRightSpeed > DriveLeftSpeed)
              DriveLeftSpeed = DriveRightSpeed - maxDifference;
          else
              DriveRightSpeed = DriveLeftSpeed - maxDifference;
      }
    }
    else if(DriveRightSpeed <= 0 && DriveLeftSpeed <= 0)
    {
       if(abs(DriveRightSpeed - DriveLeftSpeed) > maxDifference)
       {
          if(DriveRightSpeed < DriveLeftSpeed)
              DriveLeftSpeed = DriveRightSpeed + maxDifference;
          else
              DriveRightSpeed = DriveLeftSpeed + maxDifference;
       }
    }
*/
    
    //write to motor controllers
    mc.setMotorSpeed(2, DriveLeftSpeed * -1);
    mc.setMotorSpeed(3, DriveRightSpeed);
    mc.setMotorSpeed(0, RollerSpeed);
    mc.setMotorSpeed(1, RollerSpeed);
  }

  delay(1);
}
}

