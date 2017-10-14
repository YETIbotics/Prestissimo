#ifndef MDD10A_h
#define MDD10A_h

#include <Arduino.h>

class MDD10A{
	
public:

	MDD10A();

	void setMotorSpeed(int controller, int speed);
	void init();

	int dir = 0;


	//PinOuts

	const int _mc0_dir0 = 22;
	const int _mc0_pwm0 = 2;

  const int _mc0_dir1 = 24;
  const int _mc0_pwm1 = 3;

  
  const int _mc1_dir0 = 40;
  const int _mc1_pwm0 = 11;
  
  const int _mc1_dir1 = 38;
  const int _mc1_pwm1 = 10;




};

#endif


