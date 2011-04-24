//----------------------------------------------------------------------------------
// CKeyboardBoardInput class
// Author: Enric Vergara
//
// Description:
// ...
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_KEYBOARD_BOARD_INPUT_H_
#define INC_KEYBOARD_BOARD_INPUT_H_

//---Game Includes---
#include "LaberynthBoardInput.h"
//-------------------

class CKeyboardBoardInput: public CLaberynthBoardInput
{
public:
	CKeyboardBoardInput(CLaberynthBoard* board, float forceRJ1, float forceRJ2);
	virtual ~CKeyboardBoardInput() {/*Nothing*/;}

	virtual void	UpdateInputs	();
};

#endif //INC_KEYBOARD_BOARD_INPUT_H_