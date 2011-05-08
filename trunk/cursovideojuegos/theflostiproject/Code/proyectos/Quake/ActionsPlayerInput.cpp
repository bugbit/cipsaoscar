#include "__PCH_Quake.h"

#include "ActionsPlayerInput.h"
#include "Player.h"
#include "GameLogic.h"

#include "assert.h"

//---Engine Includes----
#include "Core/Core.h"
#include "Input/ActionToInput.h"

CActionsPlayerInput::CActionsPlayerInput(void)
{
}

CActionsPlayerInput::~CActionsPlayerInput(void)
{
}

void CActionsPlayerInput::UpdateInputAction	(float elapsedTime)
{
	CActionToInput* input2Action = CORE->GetActionToInput();
	float delta;
	
	assert(m_pPlayer!=NULL);
	assert(m_pGameLogic!=NULL);

	if (input2Action->DoAction("YawPlayer", delta))
	{
			delta = delta * 0.01f;
			float yaw = m_pPlayer->GetYaw();
			m_pPlayer->SetYaw(yaw+delta);
	}
	if (input2Action->DoAction("PitchPlayer", delta))
	{
			float pitch = m_pPlayer->GetPitch();
			if (pitch<-ePIf/4.f)
				m_pPlayer->SetPitch(-ePIf/4.f);
			else if(pitch>ePIf/4.f)
				m_pPlayer->SetPitch(ePIf/4.f);
			else
			{
				delta = delta * 0.01f;
				m_pPlayer->SetPitch(pitch+delta);
			}
	}
	bool speed=false;
	m_pPlayer->SetCleanMove();
	if (input2Action->DoAction("MoveUpPlayer"))
	{
		m_pPlayer->SetMoveUp(speed,elapsedTime);
	}
	if (input2Action->DoAction("MoveDownPlayer"))
	{
			m_pPlayer->SetMoveDown(speed,elapsedTime);
	}
	if (input2Action->DoAction("MoveLeftPlayer"))
	{
			m_pPlayer->SetMoveLeft(speed,elapsedTime);
	}
	if (input2Action->DoAction("MoveRightPlayer"))
	{
			m_pPlayer->SetMoveRight(speed,elapsedTime);
	}
	m_pPlayer->Move(elapsedTime);
	if (input2Action->DoAction("changeGun"))
	{
			m_pGameLogic->ChangeSelectedGun(*m_pPlayer);
	}
	if (input2Action->DoAction("shoot"))
	{
			m_pGameLogic->Shot(*m_pPlayer);
	}
}
