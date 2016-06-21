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
#include "ResponseCodes.h";
#define INPUT_BUFFER_LENGTH 1
#define REFRESH 1000

//Input Handler class
class InputHandler
{
	private:
		//Pointer to dc motor object
		DcMotor * _dcMotor;

		//Pointer to servo wrapper instance
		ServoWrapper * _servoWrapper;

		//input buffer to read from serial port
		uint8_t inputBuffer[INPUT_BUFFER_LENGTH];

		//Initialization was requested flag
		bool _initRequested;

		//Last initialization request time-stamp
		unsigned long _lastRefreshTime = 0;

		//decode provided input and set states of DC and Servo
		void decodeInputAndSetStates(int input);

		//check if provided input is initialization request
		//respond init requested if is initialization request
		//returns true if requested, false otherwise
		bool initializationRequested(uint8_t input);
	public:
		//constructor
		InputHandler(DcMotor * dcMotor, ServoWrapper * servoWrapper);
		
		//handles input form serial port
		//must be called each arduino loop iteration
		void handle();

		//check if initialization requested directly reading from serial port
		//returns true if requested, false otherwise
		bool initializationRequested();

		//send initialization request if REFRESH time passed since last request 
		void sendInitializationRequest();

		//check if initialization request received by flag and reset the flag
		//returns true if requested, false otherwise
		bool initializationRequestReceived();
};


#endif

