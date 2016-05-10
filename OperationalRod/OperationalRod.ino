// **************************************************************************************
// **																				   **
// **		(C) FOOSBOT - Final Software Engineering Project, 2015 - 2016			   **
// **		(C) Authors: M.Toubian, M.Shimon, E.Kleinman, O.Sasson, J.Gleyzer          **
// **			Advisors: Mr.Resh Amit & Dr.Hoffner Yigal							   **
// **		The information and source code here belongs to Foosbot project			   **
// **		and may not be reproduced or used without authors explicit permission.	   **
// **																				   **
// **************************************************************************************

#include <Servo.h>
#include <PID_v1.h>
#include "ServoWrapper.h"
#include "InputHandler.h"
#include "DcMotor.h";

#define BAUD_RATE 115200

volatile int _dcPosition;

//Pointer to DC Motor object instance
DcMotor _dcMotor(&_dcPosition);

//Pointer to Servo object instance
ServoWrapper _servoWrapper;

//Pointer to input handler instance
InputHandler _inputHandler(&_dcMotor, &_servoWrapper);

void initI();
bool _isInitialized;

//Encoder A interrupt handler function
void handleEncoderA(){

	//look for a low-to-high on channel A
	if (digitalRead(_dcMotor.ENCODER_A) == HIGH)
	{
		// check channel B to see which way encoder is turning
		if (digitalRead(_dcMotor.ENCODER_B) == LOW)
		{
			//CW
			_dcPosition--;
		}
		else {
			// CCW
			_dcPosition++;

		}
	}

	else   // must be a high-to-low edge on channel A                                       
	{
		// check channel B to see which way encoder is turning  
		if (digitalRead(_dcMotor.ENCODER_B) == HIGH) {
			// CW
			_dcPosition--;
		}
		else {
			// CCW
			_dcPosition++;
		}
	}
}

//Encoder B interrupt handler function
void handleEncoderB()
{
	//look for a low-to-high on channel B
	if (digitalRead(_dcMotor.ENCODER_B) == HIGH) {

		// check channel A to see which way encoder is turning
		if (digitalRead(_dcMotor.ENCODER_A) == HIGH) {
			//moving clock wise
			_dcPosition--;
		}
		else {
			//moving contra clock wise
			_dcPosition++;
		}
	}
	//look for a high-to-low on channel B
	else
	{
		// check channel B to see which way encoder is turning  
		if (digitalRead(_dcMotor.ENCODER_A) == LOW) {
			//moving clock wise
			_dcPosition--;
		}
		else {
			//moving contra clock wise
			_dcPosition++;
		}
	}
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