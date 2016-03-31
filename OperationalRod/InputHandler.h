// **************************************************************************************
// **																				   **
// **		(C) FOOSBOT - Final Software Engineering Project, 2015 - 2016			   **
// **		(C) Authors: M.Toubian, M.Shimon, E.Kleinman, O.Sasson, J.Gleyzer          **
// **			Advisors: Mr.Resh Amit & Dr.Hoffner Yigal							   **
// **		The information and source code here belongs to Foosbot project			   **
// **		and may not be reproduced or used without authors explicit permission.	   **
// **																				   **
// **************************************************************************************

#ifndef _INPUTHANDLER_h
#define _INPUTHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "DcMotor.h"
#include "ServoWrapper.h";

//Input Handler class
class InputHandler
{
	private:
		//Pointer to dc motor object
		DcMotor * _dcMotor;
		//Pointer to servo wrapper instance
		ServoWrapper * _servoWrapper;
	public:
		//constructor
		InputHandler(DcMotor * dcMotor, ServoWrapper * servoWrapper);
		
		//handles input form serial port
		//must be called each arduino loop iteration
		void handleInput();
};


#endif

