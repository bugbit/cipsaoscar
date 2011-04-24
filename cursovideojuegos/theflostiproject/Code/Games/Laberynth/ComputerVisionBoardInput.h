//----------------------------------------------------------------------------------
// CComputerVisionBoardInput class
// Author: Enric Vergara
//
// Description:
// ...
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_COMPUTER_VISION_BOARD_INPUT_H_
#define INC_COMPUTER_VISION_BOARD_INPUT_H_

#include "LaberynthBoardInput.h"

//---Forward Declarations--
class CLaberynthBoard;
//-------------------------

class CComputerVisionBoardInput: public CLaberynthBoardInput
{
public:
	CComputerVisionBoardInput(CLaberynthBoard* board, float forceRJ1, float forceRJ2);
	virtual ~CComputerVisionBoardInput() {/*Nothing*/;}

	virtual void	UpdateInputs	();
};

#endif //INC_COMPUTER_VISION_BOARD_INPUT_H_