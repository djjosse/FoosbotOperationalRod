﻿// **************************************************************************************
// **																				   **
// **		(C) FOOSBOT - Final Software Engineering Project, 2015 - 2016			   **
// **		(C) Authors: M.Toubian, M.Shimon, E.Kleinman, O.Sasson, J.Gleyzer          **
// **			Advisors: Mr.Resh Amit & Dr.Hoffner Yigal							   **
// **		The information and source code here belongs to Foosbot project			   **
// **		and may not be reproduced or used without authors explicit permission.	   **
// **																				   **
// **************************************************************************************

#include <TimerOne.h>
#include <Servo.h>
#include <PID_v1.h>
#include "ServoWrapper.h"
#include "InputHandler.h"
#include "DcMotor.h";

//Serial connection baud-rate
#define BAUD_RATE 115200

//Rod Types
#define GOAL_KEEPER_ROD 1
#define DEFENCE_ROD 2

//Rod Possible Directions
#define ROD_DIRECTION_DIRECT 0
#define ROD_DIRECTION_REVERSED 1

/*
*
*********** MAKE SURE TO CHOOSE THE RIGHT ROD!*****************
*
*/
#define CURRENT_ROD DEFENCE_ROD

#if CURRENT_ROD == GOAL_KEEPER_ROD
const int CURRENT_ROD_LENGTH = 620;
const int ROD_DIRECTION = ROD_DIRECTION_DIRECT;
const int CALIBRATION_SPEED = 125;
const double KP = 2.23;
const double KI = 2.45;
const double KD = 0.68;
const int BUFFER = 20;
const int KICK_D = 30;
const int DEFENCE_D = 110;
const int RISE_D = 140;
const int _encoderState[4][4] = {
	{ 0, 1, -1, 0 },
	{ -1, 0, 0, 1 },
	{ 1, 0, 0, -1 },
	{ 0, -1, 1, 0 }
};
#elif CURRENT_ROD == DEFENCE_ROD
const int CURRENT_ROD_LENGTH = 6056;
const int ROD_DIRECTION = ROD_DIRECTION_REVERSED;
const int CALIBRATION_SPEED = 100;
const double KP = 2.23;
const double KI = 2.45;
const double KD = 0.68;
const int BUFFER = 40;
const int KICK_D = 20;
const int DEFENCE_D = 110;
const int RISE_D = 160;
const int _encoderState[4][4] = {
	{ 0, -1, 1, 0 },
	{ 1, 0, 0, -1 },
	{ -1, 0, 0, 1 },
	{ 0, 1, -1, 0 }
};
#endif

//Current dc position of encoder
volatile int _dcPosition;

//Pointer to DC Motor object instance
DcMotor _dcMotor(&_dcPosition, CURRENT_ROD_LENGTH, ROD_DIRECTION, CALIBRATION_SPEED, KP, KI, KD, BUFFER);

//Pointer to Servo object instance
ServoWrapper _servoWrapper(KICK_D, DEFENCE_D, RISE_D);

//Pointer to input handler instance
InputHandler _inputHandler(&_dcMotor, &_servoWrapper);

//Is rod initialized
bool _isInitialized;

//Encoder A interrupt handler function
void handleEncoderA()
{
	int currentA = digitalRead(_dcMotor.ENCODER_A);
	int currentB = digitalRead(_dcMotor.ENCODER_B);
	int a = ((1 - currentA) << 1) + currentA;
	int b = (currentB << 1) + currentB;
	_dcPosition += _encoderState[a][b];
}

//Encoder B interrupt handler function
void handleEncoderB()
{
	int currentA = digitalRead(_dcMotor.ENCODER_A);
	int currentB = digitalRead(_dcMotor.ENCODER_B);
	int a = (currentA << 1) + currentA;
	int b = ((1 - currentB) << 1) + currentB;
	_dcPosition += _encoderState[a][b];
}

//Arduino Setup Function
void setup() {
	//start serial communication
	Serial.begin(BAUD_RATE);
	_isInitialized = false;
}

void initialize()
{
	_servoWrapper.setCalibrated(false);
	_servoWrapper.calibrate();

	//attached stopper buttons for rod
	pinMode(_dcMotor.START_BUTTON, INPUT);
	pinMode(_dcMotor.END_BUTTON, INPUT);

	//attach interrupts for encoders (0 - pin 2, 1 - pin 3)
	pinMode(_dcMotor.ENCODER_A, INPUT);
	pinMode(_dcMotor.ENCODER_B, INPUT);

	attachInterrupt(0, handleEncoderA, CHANGE);
	attachInterrupt(1, handleEncoderB, CHANGE);

	_dcMotor.setCalibrated(false);
	_isInitialized = true;
}

int counter= 0;
//Arduino loop function
void loop() {
	if (!_isInitialized)
	{
		Serial.println("Need initialization!");
		if (_inputHandler.handleInput())
		{
			initialize();
		}
	}
	else
	{
		//calibrate rod - only if not calibrated yet
		_dcMotor.calibrate();

		//handle input from serial port
		if (_inputHandler.handleInput())
		{
			initialize();
		}
		else
		{
			if (_isInitialized)
			{
				//verify DC motor location and fix
				_dcMotor.verifyPosition();
			}
		}
	}
}