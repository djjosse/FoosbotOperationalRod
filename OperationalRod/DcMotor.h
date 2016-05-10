// **************************************************************************************
// **																				   **
// **		(C) FOOSBOT - Final Software Engineering Project, 2015 - 2016			   **
// **		(C) Authors: M.Toubian, M.Shimon, E.Kleinman, O.Sasson, J.Gleyzer          **
// **			Advisors: Mr.Resh Amit & Dr.Hoffner Yigal							   **
// **		The information and source code here belongs to Foosbot project			   **
// **		and may not be reproduced or used without authors explicit permission.	   **
// **																				   **
// **************************************************************************************

#ifndef _DCMOTOR_h
#define _DCMOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <PID_v1.h>

#define ROD_LENGTH 2600
#define MAX_CODED_DC_POSITION 62
#define MIN_CODED_DC_POSITION 1

//Class represents instance of DCMotor with encoder and stopper buttons
class DcMotor
{
	private:
		//current dc position on axe
		//volatile int _currentPosition;
		volatile int * _curPosPtr;
		//pointer to PID instance
		PID * _pid;
		//flag for rod calibration
		bool _isCalibrated;
		//axe start stopper
		//int _startPosition = 0;
		//last received position input stored
		int _lastReceivedPosition = 0;
		//pid Input parameter - current position
		double _input;
		//pid Output parameter - speed 
		double _output;
		//pid Setpoint parameter - desired position
		double _setpoint;

	public:
		//encoder A arduino pin
		const int ENCODER_A = 2;
		//encoder B arduino pin
		const int ENCODER_B = 4;
		//pwm control for motor outputs 1 and 2 is on digital pin 3
		const int DC_POWER = 3; 
		//direction control for motor outputs 1 and 2 is on digital pin 12
		const int DC_DIRECTION = 12;  
		//start stopper button pin
		const int START_BUTTON = 5;
		//end stopper button pin
		const int END_BUTTON = 7;

		//Buffer for move in ticks
		const int BUFFER = 100;

		//pid kP parameter
		const double KP = 1;
		//pid kI parameter
		const double KI = 0.05;
		//pid kD parameter
		const double KD = 0.25;
		//allowed position error in ticks
		const int PID_ERROR = 10;

		//decoding factor to convert bits to ticks
		float _decodingFactor;

		//constructor
		DcMotor(volatile int * currentPosition);

		//set dc direction to forward
		void setForward();

		//set dc direction to 
		void setBackward();

		//set dc speed (255 max, 0 stop)
		void setSpeed(double speed);

		//set dc position - moves back or forward according to PID and current position,
		//must be called on new input
		void setPosition(int newPosition);

		//verifies current position is as desired - uses setPosition() to fix,
		//must be called every loop
		void verifyPosition();

		//calibrattion detects 0 position of a rod, must be called once at the begging of a programm
		//sets calibration flag as true, second call will be ignored
		void calibrate();

		//set current calibration status
		void setCalibrated(bool isCalibrated) { _isCalibrated = isCalibrated; }

		//get decoding factor used to convert decoded dc value to ticks
		float getDecodeFactor() { return _decodingFactor; }
};

#endif