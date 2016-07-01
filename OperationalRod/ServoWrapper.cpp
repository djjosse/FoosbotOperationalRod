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
#include "ResponseCodes.h"

#define WAIT_SERVO 150

//set servo to desired state
//0 - NA, 1 - Kick, 2 - Defence, 3 - Rise
void ServoWrapper::setState(int state)
{
	if (!_servo.attached()) _servo.attach(SERVO_PIN);
	if (_servoState != state)
	{
		//Wait after last received command and ignore new commands
		//this is made in order to stop burining servos
		if (millis() - _lastActionTime >= WAIT_SERVO)
		{
			_lastActionTime = millis();
			switch (state)
			{
			case KICK:
				Serial.write((char)ArduinoCodes::NEW_SERVO_STATE_KICK);
				_servo.write(KICK_DEGREES);
				_servoState = state;
				break;
			case DEFENCE:
				Serial.write((char)ArduinoCodes::NEW_SERVO_STATE_DEFENCE);
				_servo.write(DEFENCE_DEGREES);
				_servoState = state;
				break;
			case RISE:
				Serial.write((char)ArduinoCodes::NEW_SERVO_STATE_RISE);
				_servo.write(RISE_DEGREES);
				_servoState = state;
				break;
			default:
				break;
			}
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