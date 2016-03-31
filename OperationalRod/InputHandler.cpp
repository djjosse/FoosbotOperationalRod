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
* Input Handler class
* See "InputHandler.h" for function comments
*/

#include "InputHandler.h"
#define INPUT_SIZE 10

InputHandler::InputHandler(DcMotor * dcMotor, ServoWrapper * servoWrapper)
{
	_dcMotor = dcMotor;
	_servoWrapper = servoWrapper;
}

void InputHandler::handleInput()
{
	if (Serial.available() > 0)
	{
		int dc = -1;
		int servo = 0;
		// Get next command from Serial (add 1 for final 0)
		char input[INPUT_SIZE + 1];
		byte size = Serial.readBytes(input, INPUT_SIZE);
		input[size] = 0;
		int count = 0;
		char* command = strtok(input, "&");
		while (command != 0)
		{
			if (count == 0)
			{
				dc = atoi(command);	
			}
			if (count == 1)
			{			
				servo = atoi(command);
			}
			command = strtok(0, "&");
			count++;
		}
		if (dc != -1)
		{
			Serial.println(dc);
			_dcMotor->setPosition(dc);
		}
		if (servo!=0)
		{
			Serial.println(servo);
			_servoWrapper->setState(servo);
		}
	}
}