#include "MDD10A.h"

MDD10A::MDD10A() {

}

void MDD10A::init() {
	pinMode(_mc0_dir0, OUTPUT);
	//pinMode(_mc0_dir1, OUTPUT);
}

void MDD10A::setMotorSpeed(int controller, int speed) {
	if (speed < 0) {
		dir = 1;
		speed *= -1;
	}
	else {
		dir = 0;
	}

	switch (controller) {
	case 0:
		digitalWrite(_mc0_dir0, dir);
		analogWrite(_mc0_pwm0, speed);
		break;

	case 1:
		digitalWrite(_mc0_dir1, dir);
		analogWrite(_mc0_pwm1, speed);
		break;

	case 2:
		digitalWrite(_mc1_dir0, dir);
		analogWrite(_mc1_pwm0, speed);
		break;

	case 3:
		digitalWrite(_mc1_dir1, dir);
		analogWrite(_mc1_pwm1, speed);
		break;

	}
}


