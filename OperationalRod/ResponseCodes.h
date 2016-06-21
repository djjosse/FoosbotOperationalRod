// **************************************************************************************
// **																				   **
// **		(C) FOOSBOT - Final Software Engineering Project, 2015 - 2016			   **
// **		(C) Authors: M.Toubian, M.Shimon, E.Kleinman, O.Sasson, J.Gleyzer          **
// **			Advisors: Mr.Resh Amit & Dr.Hoffner Yigal							   **
// **		The information and source code here belongs to Foosbot project			   **
// **		and may not be reproduced or used without authors explicit permission.	   **
// **																				   **
// **************************************************************************************

#ifndef _RESPONSE_CODES_H
#define _RESPONSE_CODES_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

enum ArduinoCodes
{
	INIT_REQUERED = 100,
	INIT_REQUESTED = 101,
	INIT_STARTED = 102,
	INIT_FINISHED = 103,

	NEW_SERVO_STATE_KICK = 110,
	NEW_SERVO_STATE_DEFENCE = 111,
	NEW_SERVO_STATE_RISE = 112,

	DC_RANGE_INVALID = 120,
	DC_RECEIVED_OK = 121,
	DC_CALIBRATED = 122,

	INPUT_INIT_REQUEST = 255
};



#endif