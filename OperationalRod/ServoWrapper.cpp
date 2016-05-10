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
* Wrapper for servo in foosbot context
* See "ServoWrapper.h" for function explanations
*/

#include "ServoWrapper.h"

//set servo to desired state
//0 - NA, 1 - Kick, 2 - Defence, 3 - Rise
void ServoWrapper::setState(int state)
{
	if (!_servo.attached()) _servo.attach(SERVO_PIN);
	if (_servoState != state)
	{
		Serial.print(F("Setting servo state: "));
		switch (state)
		{
		case KICK:
			_servo.write(KICK_DEGREES);
			_servoState = state;
			break;
		case DEFENCE:
			_servo.write(DEFENCE_DEGREES);
			_servoState = state;
			break;
		case RISE:
			_servo.write(RISE_DEGREES);
			_servoState = state;
			break;
		default:
			break;
		}
	}
}

//calibration method sets servo to defence mode
void ServoWrapper::calibrate()
{
	if (!_isCalibrated)
	{
		setState(DEFENCE);
		_isCalibrated = true;
	}
}