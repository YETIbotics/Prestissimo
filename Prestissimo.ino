
#include "XBOXRECV.h"
#include "usbhub.h"
#include "MDD10A.h"
#include "PololuMaestro.h"

#define maestroSerial SERIAL_PORT_HARDWARE_OPEN

USB Usb;
USBHub  Hub1(&Usb); // The controller has a built in hub, so this instance is needed
XBOXRECV Xbox(&Usb);
MDD10A mc;
MiniMaestro maestro(maestroSerial);


void setup() {
	// put your setup code here, to run once:
	maestroSerial.begin(9600);
	Serial.begin(115200);
#if !defined(__MIPSEL__)
	while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
	if (Usb.Init() == -1) {
		Serial.print(F("\r\nOSC did not start"));
		while (1); // halt
	}
	Serial.print(F("\r\nXBOX Library Started"));

	maestro.setAcceleration(0, 10);
	maestro.setSpeed(0, 0);
	maestro.setTarget(0, 1500 * 4);

	maestro.setAcceleration(1, 10);
	maestro.setSpeed(1, 0);
	maestro.setTarget(1, 1500 * 4);

}

int loopcnt = 0;
float speedDiv = 1;
float maxDifference = 40;

void loop()
{
	// put your main code here, to run repeatedly:
	Usb.Task();

	float DriveLeftSpeed = 0;
	float DriveRightSpeed = 0;
	float RollerSpeed = 0;
	float ChainbarX = 0;
	float ChainbarY = 0;


	int curController = 0;
	if (Xbox.getAnalogHat(LeftHatY, curController) > 7500) {
		//Going backwards
		DriveLeftSpeed = map(Xbox.getAnalogHat(LeftHatY, curController), 7500, 32767, 0, 255);
	}
	else if (Xbox.getAnalogHat(LeftHatY, curController) < -7500)
	{
		//going forwards
		DriveLeftSpeed = map(Xbox.getAnalogHat(LeftHatY, curController), -32767, -7500, -255, 0);
	}


	if (Xbox.getAnalogHat(RightHatY, curController) > 7500) {
		//Going backwards
		DriveRightSpeed = map(Xbox.getAnalogHat(RightHatY, curController), 7500, 32767, 0, 255);
	}
	else if (Xbox.getAnalogHat(RightHatY, curController) < -7500)
	{
		//going forwards
		DriveRightSpeed = map(Xbox.getAnalogHat(RightHatY, curController), -32767, -7500, -255, 0);
	}

	if (Xbox.getButtonClick(UP, curController))
	{
		if (speedDiv < 1)
			speedDiv = speedDiv + .1;
	}

	if (Xbox.getButtonClick(DOWN, curController))
	{
		if (speedDiv > 0)
			speedDiv = speedDiv - .1;
	}


	if (Xbox.getButtonClick(RIGHT, curController))
	{
		if (maxDifference < 1)
			maxDifference = maxDifference + 5;
	}

	if (Xbox.getButtonClick(LEFT, curController))
	{
		if (maxDifference > 0)
			maxDifference = maxDifference - 5;
	}

	if (Xbox.getButtonClick(UP, curController))
	{
		if (speedDiv < 1)
			speedDiv = speedDiv + .1;
	}

	//CONTROLLER 2
	curController = 1;

	int servoMin = 500 * 4;
	int servoNeut = 1500 * 4;
	int servoMax = 2500 * 4;

	if (Xbox.getButtonPress(R2, curController) > 10) {
		RollerSpeed = Xbox.getButtonPress(R2, curController);
	}
	else {
		RollerSpeed = -1 * Xbox.getButtonPress(L2, curController);
	}

	if (Xbox.getAnalogHat(LeftHatY, curController) > 7500) {
		ChainbarY = map(Xbox.getAnalogHat(LeftHatY, curController), 7500, 32767, servoNeut, servoMax);
	}
	else if (Xbox.getAnalogHat(LeftHatY, curController) < -7500)
	{
		ChainbarY = map(Xbox.getAnalogHat(LeftHatY, curController), -32767, -7500, servoMin, servoNeut);
	}

	if (Xbox.getAnalogHat(RightHatY, curController) > 7500) {
		ChainbarX = map(Xbox.getAnalogHat(RightHatY, curController), 7500, 32767, servoNeut, servoMax);
	}
	else if (Xbox.getAnalogHat(RightHatY, curController) < -7500)
	{
		ChainbarX = map(Xbox.getAnalogHat(RightHatY, curController), -32767, -7500, servoMin, servoNeut);
	}



	loopcnt++;
	if (loopcnt >= 20)
	{
		loopcnt = 0;

		DriveLeftSpeed = DriveLeftSpeed * speedDiv;
		DriveRightSpeed = DriveRightSpeed * speedDiv;

		//write to motor controllers
		mc.setMotorSpeed(2, DriveLeftSpeed * -1);
		mc.setMotorSpeed(3, DriveRightSpeed);
		mc.setMotorSpeed(0, RollerSpeed);
		mc.setMotorSpeed(1, RollerSpeed);

		// 500 - 2500
		maestro.setTarget(1, ChainbarY);
		maestro.setTarget(0, ChainbarX);
	}

	delay(1);
}


