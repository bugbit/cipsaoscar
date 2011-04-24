#include "__PCH_Laberynth.h"

//---Engines Includes----
#include "Input/InputManager.h"
#include "Core/Core.h"
#include "PhysX/PhysicRevoluteJoint.h"
//-----------------------

//---Game Includes----
#include "KeyboardBoardInput.h"
#include "LaberynthBoardInput.h"
#include "LaberynthBoard.h"
//-----------------------

CKeyboardBoardInput::CKeyboardBoardInput(CLaberynthBoard* board, float forceRJ1, float forceRJ2)
: CLaberynthBoardInput(board, forceRJ1, forceRJ2) 
{
	m_pBoard->GetRevoluteJoint2()->ActiveMotor(0.f);
	m_pBoard->GetRevoluteJoint1()->ActiveMotor(0.f);
}

void CKeyboardBoardInput::UpdateInputs()
{
	CInputManager* input = CORE->GetInputManager();

	if (input->IsDown(IDV_KEYBOARD,KEY_RIGHT) )
	{
		m_pBoard->GetRevoluteJoint2()->ActiveMotor(-CLaberynthBoardInput::m_fForceRJ1);
	}
	else if (input->IsDown(IDV_KEYBOARD,KEY_LEFT) )
	{
		m_pBoard->GetRevoluteJoint2()->ActiveMotor(CLaberynthBoardInput::m_fForceRJ1);
	}
	else
	{
		m_pBoard->GetRevoluteJoint2()->ActiveMotor(0.f);
	}

	if (input->IsDown(IDV_KEYBOARD,KEY_UP) )
	{
		m_pBoard->GetRevoluteJoint1()->ActiveMotor(CLaberynthBoardInput::m_fForceRJ2);
	}
	else if (input->IsDown(IDV_KEYBOARD,KEY_DOWN) )
	{
		m_pBoard->GetRevoluteJoint1()->ActiveMotor(-CLaberynthBoardInput::m_fForceRJ2);
	}
	else
	{
		m_pBoard->GetRevoluteJoint1()->ActiveMotor(0.f);
	}



	

}