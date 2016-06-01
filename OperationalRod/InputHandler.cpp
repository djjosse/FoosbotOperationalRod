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

//constructor
InputHandler::InputHandler(DcMotor * dcMotor, ServoWrapper * servoWrapper)
{
	_dcMotor = dcMotor;
	_servoWrapper = servoWrapper;
}

//handles input form serial port
//must be called each arduino loop iteration
//returns flag if reset requested - true
bool InputHandler::handleInput()
{
	if (Serial.available() > 0)
	{
		int servoB = 0;
		int dcB = 0;
		int input = Serial.read();
		switch (input)
		{
			case 255:
				Serial.print(F("Initializing..."));
				return true;
			default:
				//get command from serial as 8 bits (assuming a 8 bit int)
				int bits[8];
				for (int i = 0; i < 8; ++i) 
				{  
					bits[i] = input & (1 << i) ? 1 : 0;
				}
				//get servo command bits 0 and 1
				servoB = bits[0] + bits[1] * 2;
				//get DC position between MIN_CODED_DC_POSITION (1) and MAX_CODED_DC_POSITION (62)
				for (int i = 7; i >= 2; i--)
				{
					dcB <<= 1;
					dcB += bits[i];
				}

				//calculate requested position in ticks
				float m = _dcMotor->getDecodeFactor();
				int dc = m*(float)(dcB);
				
				//set requested position
				Serial.println(dc);
				_dcMotor->setPosition(dc);
				_servoWrapper->setState(servoB);

				break;
		}
		return false;
	}
}