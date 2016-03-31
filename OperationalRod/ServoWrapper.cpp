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

ServoWrapper::ServoWrapper()
{
	_servo.attach(SERVO_PIN);
}

void ServoWrapper::setState(int state)
{
	if (_servoState != state)
	{
		if (state == 1)
		{
			_servo.write(30);
			_servoState = state;
		}
		else if (state == 2)
		{
			_servo.write(90);
			_servoState = state;
		}
		else if (state == 3)
		{
			_servo.write(150);
			_servoState = state;
		}
	}
}

void ServoWrapper::callibrate()
{
	if (!_isCallibrated)
	{
		setState(2);
		_isCallibrated = true;
	}
}