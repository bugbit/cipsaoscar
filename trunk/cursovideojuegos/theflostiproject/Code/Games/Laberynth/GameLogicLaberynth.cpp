#include "__PCH_Laberynth.h"

//---Engine Includes----
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Logger/Logger.h"
#include "Graphics/ASEObject/ASETextureManager.h"
#include "Core/Core.h"
//----------------------

//---Game Includes----
#include "GameLogicLaberynth.h"
#include "LaberynthBoardInput.h"
#include "LaberynthBoard.h"
//---------------------

CGameLogicLaberynth::CGameLogicLaberynth(	CLaberynthBoard *board, std::vector<CLaberynthBoardInput*>& vecPlayerInputt,
																					std::vector<CEndGameCondition*>& vecEndGameCondition)
: m_pBoard(board)
, m_VecPlayerInput(vecPlayerInputt)
, m_bFinished(false)
, m_VecEndGameCondition(vecEndGameCondition)
, m_eTypeOfEndGameCondition(TC_NONE)
{
	assert( m_pBoard );
}

CGameLogicLaberynth::~CGameLogicLaberynth()
{
	CHECKED_DELETE(m_pBoard);

	std::vector<CLaberynthBoardInput*>::iterator it = m_VecPlayerInput.begin();
	std::vector<CLaberynthBoardInput*>::iterator itEnd = m_VecPlayerInput.end();
	for (; it != itEnd; it++)
	{
		CLaberynthBoardInput* boardInput = *it;
		CHECKED_DELETE(boardInput);
	}

	std::vector<CEndGameCondition*>::iterator it_EGC = m_VecEndGameCondition.begin();
	std::vector<CEndGameCondition*>::iterator itEnd_EGC = m_VecEndGameCondition.end();
	for (; it_EGC != itEnd_EGC; it_EGC++)
	{
		CEndGameCondition* condition = *it_EGC;
		CHECKED_DELETE(condition);
	}	
}

void CGameLogicLaberynth::Update (float elapsedTime)
{
	if( !m_bFinished ) 
	{
		//-----------Update Inputs-----------------------------------
		std::vector<CLaberynthBoardInput*>::iterator it = m_VecPlayerInput.begin();
		std::vector<CLaberynthBoardInput*>::iterator itEnd = m_VecPlayerInput.end();
		for (; it != itEnd; it++)
		{
			CLaberynthBoardInput* boardInput = *it;
			boardInput->UpdateInputs();
		}

		//-----------Update End Game Conditions------------------------
		std::vector<CEndGameCondition*>::iterator it_EGC = m_VecEndGameCondition.begin();
		std::vector<CEndGameCondition*>::iterator itEnd_EGC = m_VecEndGameCondition.end();
		for (; it_EGC != itEnd_EGC; it_EGC++)
		{
			CEndGameCondition* condition = *it_EGC;
			if (condition->Check(m_pBoard,elapsedTime) )
			{
				m_bFinished = true;
				m_eTypeOfEndGameCondition = condition->GetTypeOfEndGameCondition();
			}
		}

		//------------Update Board--------------------------------------
		m_pBoard->UpdateScene(elapsedTime);
	}
	else
	{
		m_fCountTime += elapsedTime;
		if (m_fCountTime > m_fTimeToShowEndGame)
			m_bExit = true;
	}
}

void CGameLogicLaberynth::SetForceRJ1 (float forceRJ1)
{
	std::vector<CLaberynthBoardInput*>::iterator it = m_VecPlayerInput.begin();
	std::vector<CLaberynthBoardInput*>::iterator itEnd = m_VecPlayerInput.end();
	for (; it != itEnd; it++)
	{
		CLaberynthBoardInput* boardInput = *it;
		boardInput->SetForceRJ1(forceRJ1);
	}
}

void CGameLogicLaberynth::SetForceRJ2 (float forceRJ2)
{
	std::vector<CLaberynthBoardInput*>::iterator it = m_VecPlayerInput.begin();
	std::vector<CLaberynthBoardInput*>::iterator itEnd = m_VecPlayerInput.end();
	for (; it != itEnd; it++)
	{
		CLaberynthBoardInput* boardInput = *it;
		boardInput->SetForceRJ2(forceRJ2);
	}
}

void CGameLogicLaberynth::RenderScene (CRenderManager* renderManager, CFontManager* fm, bool inSettings)
{
	m_pBoard->RenderScene(renderManager, fm);
	
	if (m_bFinished && !inSettings)
	{
		uint32 posX = 500;
		uint32 posY = 500;

		switch(m_eTypeOfEndGameCondition)
		{
			case TC_TIME_OUT:
				{
					fm->DrawDefaultText(posX,posY,colWHITE,"CGameLogicLaberynth::RenderScene -> Ha PERDIDO la partida por sobrepasar el tiempo limite!");
				}
				break;
		
			case TC_VICTORY:
				{
					fm->DrawDefaultText(posX,posY,colWHITE,"CGameLogicLaberynth::RenderScene -> Ha GANADO la partida!");
				}
				break;

			case TC_LOSS:
				{
					fm->DrawDefaultText(posX,posY,colWHITE,"CGameLogicLaberynth::RenderScene -> Ha PERDIDO la partida!");
				}
				break;

			case TC_NONE:
				{
					LOGGER->AddNewLog(ELL_ERROR, "CGameLogicLaberynth::RenderScene -> un end game condition de tipo TC_NONE ha finalizado la partida!");
				}
				break;
			default:
				{
					LOGGER->AddNewLog(ELL_ERROR, "CGameLogicLaberynth::RenderScene -> un end game condition de tipo tipo desconocido ha finalizado la partida!");
				}
		}//END switch(m_eTypeOfEndGameCondition)

	}//END if (m_bFinished)
}

bool CGameLogicLaberynth::Exit () const 
{
	return m_bExit;
}