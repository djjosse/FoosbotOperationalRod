// **************************************************************************************
// **																				   **
// **		(C) FOOSBOT - Final Software Engineering Project, 2015 - 2016			   **
// **		(C) Authors: M.Toubian, M.Shimon, E.Kleinman, O.Sasson, J.Gleyzer          **
// **			Advisors: Mr.Resh Amit & Dr.Hoffner Yigal							   **
// **		The information and source code here belongs to Foosbot project			   **
// **		and may not be reproduced or used without authors explicit permission.	   **
// **																				   **
// **************************************************************************************

/*
 * Class represents instance of DCMotor with encoder and stopper buttons 
 * See comments for functions in "DcMotor.h"
 */

#include "DcMotor.h"
#include "Arduino.h"

DcMotor::DcMotor()
{
	_currentPosition = 0;
	_setpoint = 0;
	_pid = new PID(&_input, &_output, &_setpoint, KP, KI, KD, DIRECT);
	_pid->SetMode(AUTOMATIC);
	_pid->SetSampleTime(1);
	_isCallibrated = false;
}

void DcMotor::encoderA(){

	// look for a low-to-high on channel A
	if (digitalRead(ENCODER_A) == HIGH) {

		// check channel B to see which way encoder is turning
		if (digitalRead(ENCODER_B) == LOW) {
			//CW
			_currentPosition--;
		}
		else {
			// CCW
			_currentPosition++;
			
		}
	}

	else   // must be a high-to-low edge on channel A                                       
	{
		// check channel B to see which way encoder is turning  
		if (digitalRead(ENCODER_B) == HIGH) {
			// CW
			_currentPosition--;
		}
		else {
			// CCW
			_currentPosition++;
		}
	}
}

void DcMotor::encoderB(){

	// look for a low-to-high on channel B
	if (digitalRead(ENCODER_B) == HIGH) {

		// check channel A to see which way encoder is turning
		if (digitalRead(ENCODER_A) == HIGH) {
			// CW
			_currentPosition--;
		}
		else {
			// CCW
			_currentPosition++;
		}
	}
	// Look for a high-to-low on channel B
	else {
		// check channel B to see which way encoder is turning  
		if (digitalRead(ENCODER_A) == LOW) {
			// CW
			_currentPosition--;
		}
		else {
			// CCW
			_currentPosition++;
		}
	}
}

void DcMotor::setForward()
{
	digitalWrite(DC_DIRECTION, HIGH);
}

void DcMotor::setBackward()
{
	digitalWrite(DC_DIRECTION, LOW);
}

void DcMotor::setSpeed(double speed)
{
	analogWrite(DC_POWER, speed);
}

void DcMotor::setPosition(int newPosition)
{
	_lastReceivedPosition = newPosition;
	_setpoint = newPosition + _startPosition;
	_input = _currentPosition;

	_pid->Compute();

	if (_setpoint > _currentPosition + PID_ERROR)
	{
		if (digitalRead(END_BUTTON) != LOW)
		{
			setForward();
			setSpeed(_output);
		}
	}
	else if (_setpoint  < _currentPosition - PID_ERROR)
	{
		if (digitalRead(START_BUTTON) != LOW)
		{
			setBackward();
			setSpeed(255);
		}
	}
	//Position reached
	else
	{
		setSpeed(0);
	}
	Serial.println(getCurrentDcPosition());
}

void DcMotor::verifyPosition()
{
	setPosition(_lastReceivedPosition);
}

void DcMotor::callibrate()
{
	if (!_isCallibrated)
	{
		//move to start pin (0)
		while (digitalRead(START_BUTTON) != LOW)
		{
			setBackward();
			setSpeed(75);
			Serial.println(getCurrentDcPosition());
		}
		setSpeed(0);
		_startPosition = _currentPosition;
		_setpoint = _startPosition;

		Serial.print(getCurrentDcPosition());
		Serial.print(" ");
		Serial.print(_startPosition);
		Serial.print(" ");
		Serial.println(_setpoint);

		
		_isCallibrated = true;
	}
}