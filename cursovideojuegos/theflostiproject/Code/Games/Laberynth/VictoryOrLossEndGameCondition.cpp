#include "__PCH_Laberynth.h"

//----Engine Includes-------------
#include "Core/Core.h"
#include "PhysX/PhysicsManager.h"
#include "Sound/SoundManager.h"
#include "GUI/GuiManager.h"
//--------------------------------

//----Game Includes---------------
#include "VictoryOrLossEndGameCondition.h"
#include "LaberynthBoard.h"
//--------------------------------


CVictoryOrLossEndGameCondition::CVictoryOrLossEndGameCondition ()
: CEndGameCondition(TC_VICTORY)
, m_bBallEnterInTrigger(false) 
{
	CPhysicsManager* physicManager = CORE->GetPhysicManager();
	assert(physicManager);
	physicManager->SetTriggerReport(this);
	CORE->GetGUIManager()->SetVisibleGuiElement("game_stars",false);

	CORE->GetGUIManager()->SetVisibleGuiElement("game_stars2",false);
	
}

bool CVictoryOrLossEndGameCondition::Check (CLaberynthBoard *board, float elapsedTime) 
{
	return m_bBallEnterInTrigger;
}


void CVictoryOrLossEndGameCondition::OnEnter (CPhysicUserData* trigger, CPhysicUserData* other_shape)
{
	CGameEntity * entity = (CGameEntity*) trigger;
	assert(entity);
	std::string m_sNameTrigger = entity->GetName();

	entity = (CGameEntity*) other_shape;
	assert(entity);
	std::string m_sNameOtherShape = entity->GetName();

	if ( (m_sNameTrigger.compare("trigger_Goal") == 0) && (m_sNameOtherShape.compare("Ball") == 0) )
	{
		m_bBallEnterInTrigger = true;
		CORE->GetSoundManager()->PlayAction2D("win");
		CEndGameCondition::m_eTypeEndGameCondition = TC_VICTORY;
		
		if (CORE->IsEndTimer("game_star",10.f))
		{
			CORE->GetGUIManager()->SetVisibleGuiElement("game_stars",true);
			CORE->GetGUIManager()->PlayImage("game_stars",0.1f,true);
		}
		if (CORE->IsEndTimer("game_star2",10.f))
		{
			CORE->GetGUIManager()->SetVisibleGuiElement("game_stars2",true);
			CORE->GetGUIManager()->PlayImage("game_stars2",0.1f,true);
		}		
		
	}
	else if ( (m_sNameTrigger.compare("trigger_Flat") == 0) && (m_sNameOtherShape.compare("Ball") == 0) )
	{
		m_bBallEnterInTrigger = true;
		CORE->GetSoundManager()->PlayAction2D("gameOver");
		CEndGameCondition::m_eTypeEndGameCondition = TC_LOSS;
	}
}

void CVictoryOrLossEndGameCondition::OnLeave (CPhysicUserData* trigger1, CPhysicUserData* other_shape)
{
	//Nothing...
}