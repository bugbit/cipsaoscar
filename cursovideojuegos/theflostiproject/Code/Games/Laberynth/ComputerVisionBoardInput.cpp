#include "__PCH_Laberynth.h"

//---Engines Includes----
#include "Input/InputManager.h"
#include "Core/Core.h"
#include "PhysX/PhysicRevoluteJoint.h"
//-----------------------

//---Game Includes----
#include "ComputerVisionBoardInput.h"
#include "LaberynthBoardInput.h"
#include "LaberynthBoard.h"
//-----------------------


CComputerVisionBoardInput::CComputerVisionBoardInput(CLaberynthBoard* board, float forceRJ1, float forceRJ2)
: CLaberynthBoardInput(board, forceRJ1, forceRJ2) 
{
	m_pBoard->GetRevoluteJoint2()->ActiveMotor(0.f);
	m_pBoard->GetRevoluteJoint1()->ActiveMotor(0.f);
}

void CComputerVisionBoardInput::UpdateInputs()
{
	//TODO...
	int jorls = 0;
}