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

//Pointer to DC Motor object instance
DcMotor * _dcMotor;

//Pointer to Servo object instance
ServoWrapper * _servoWrapper;

//Pointer to input handler instance
InputHandler * _inputHandler;

//Encoder A interrupt handler function
void handleEncoderA()
{
	_dcMotor->encoderA();
}

//Encoder B interrupt handler function
void handleEncoderB()
{
	_dcMotor->encoderB();
}

//Arduino Setup Function
void setup() {
	//start serial communication
	Serial.begin(9600);

	//create used objects
	_dcMotor = new DcMotor();
	_servoWrapper = new ServoWrapper();
	_inputHandler = new InputHandler(_dcMotor, _servoWrapper);

	//attached stopper buttons for rod
	pinMode(_dcMotor->START_BUTTON, INPUT);
	pinMode(_dcMotor->END_BUTTON, INPUT);

	//attach interrupts for encoders (0 - pin 2, 1 - pin 3)
	pinMode(_dcMotor->ENCODER_A, INPUT);
	pinMode(_dcMotor->ENCODER_B, INPUT);
	attachInterrupt(0, handleEncoderA, CHANGE);
	attachInterrupt(1, handleEncoderB, CHANGE);
}

//Arduino loop function
void loop() {
	//callibrate rod - only if not callibrated yet
	_dcMotor->callibrate();
	
	//handle input from serial port
	_inputHandler->handleInput();

	//verify DC motor location and fix
	_dcMotor->verifyPosition();
}