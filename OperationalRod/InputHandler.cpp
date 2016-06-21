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
*/

#include "InputHandler.h"
#define INPUT_SIZE 10

//constructor
InputHandler::InputHandler(DcMotor * dcMotor, ServoWrapper * servoWrapper)
{
	_dcMotor = dcMotor;
	_servoWrapper = servoWrapper;
	_initRequested = false;
}

//handles input form serial port
//must be called each arduino loop iteration
void InputHandler::handle()
{
	if (Serial.available() > 0)
	{
		Serial.readBytes(inputBuffer, INPUT_BUFFER_LENGTH);
		int input = inputBuffer[0];
		if (!initializationRequested(input))
		{
			decodeInputAndSetStates(input);
		}
	}
}

//decode provided input and set states of DC and Servo
void InputHandler::decodeInputAndSetStates(int input)
{
	int dcB = 0;

	//get command from serial as 8 bits (assuming a 8 bit int)
	int bits[8];
	for (int i = 0; i < 8; ++i)
	{
		bits[i] = input & (1 << i) ? 1 : 0;
	}
	//get servo command bits 0 and 1
	int servoB = bits[0] + bits[1] * 2;
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
	_dcMotor->setPosition(dc, true);
	_servoWrapper->setState(servoB);
}

//send initialization request if REFRESH time passed since last request 
void InputHandler::sendInitializationRequest()
{
	if (millis() - _lastRefreshTime >= REFRESH)
	{
		_lastRefreshTime += REFRESH;
		Serial.write((char)ArduinoCodes::INIT_REQUERED);
	}
}

//check if initialization requested directly reading from serial port
//returns true if requested, false otherwise
bool InputHandler::initializationRequested()
{
	if (Serial.available() > 0)
	{
		Serial.readBytes(inputBuffer, INPUT_BUFFER_LENGTH);
		return initializationRequested(inputBuffer[0]);
	}
	return false;
}

//check if provided input is initialization request
//respond init requested if is initialization request
//returns true if requested, false otherwise
bool InputHandler::initializationRequested(uint8_t input)
{
	switch (input)
	{
		case ArduinoCodes::INPUT_INIT_REQUEST:
			_initRequested = true;
			Serial.write((char)ArduinoCodes::INIT_REQUESTED);
			return true;
		default:
			return false;
	}
}

//check if initialization request received by flag and reset the flag
//returns true if requested, false otherwise
bool InputHandler::initializationRequestReceived()
{
	if (_initRequested)
	{
		_initRequested = false;
		return true;
	}
	return false;
}