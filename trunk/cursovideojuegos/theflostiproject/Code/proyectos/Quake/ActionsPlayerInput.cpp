#include "__PCH_Quake.h"

#include "ActionsPlayerInput.h"
#include "Player.h"

CActionsPlayerInput::CActionsPlayerInput(void)
{
}

CActionsPlayerInput::~CActionsPlayerInput(void)
{
}

void CActionsPlayerInput::UpdateInputAction	(float elapsedTime)
{
	Vect3f directionXZ(0.f,0.f,0.f);

	m_Player->Move(.05f*directionXZ,elapsedTime);
}
