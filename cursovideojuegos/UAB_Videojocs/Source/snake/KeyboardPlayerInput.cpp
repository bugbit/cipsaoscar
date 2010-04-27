#include "KeyboardPlayerInput.h"
#include "../InputManager.h"

CKeyboardPlayerInput::CKeyboardPlayerInput(void)
{
	m_sMoveUp = "snake_up";
	m_sMoveDown= "snake_down";
	m_sMoveRight= "snake_right";
	m_sMoveLeft= "snake_left";
}

CKeyboardPlayerInput::~CKeyboardPlayerInput(void)
{
}

void CKeyboardPlayerInput::UpdateInputAction(float dt)
{
	CInputManager * input = CInputManager::GetInstance();

	//Update Snake 1:
	if( input->DoAction(m_sMoveDown) )
	{
		m_Snake->SetDirection( DIR_DOWN );
	}
	else if( input->DoAction(m_sMoveUp) )
	{
		m_Snake->SetDirection( DIR_UP );
	}
	else if( input->DoAction(m_sMoveRight) )
	{
		m_Snake->SetDirection( DIR_RIGHT );
	}
	else if( input->DoAction(m_sMoveLeft) )
	{
		m_Snake->SetDirection( DIR_LEFT );
	}
}