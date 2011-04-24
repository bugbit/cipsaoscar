//----------------------------------------------------------------------------------
// CGameLogicLaberynth class
// Author: Enric Vergara
//
// Description:
// Clase encargada de gestionar la lógica del juego Laberynth. El juego se basa en el 
// típico ...
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_BOARD_GAME_LABERYNTH_H_
#define INC_BOARD_GAME_LABERYNTH_H_

#define TIME_TO_SHOW_END 10000

//---Game Includes---
#include "EndGameCondition.h"
//-------------------

//---Engine Includes---
#include "Graphics/ASEObject/ASEObject.h"
//---------------------

//---Forward Declaration---
//Engine:
class CRenderManager;
class CFontManager;
//Game:
class CLaberynthBoardInput;
class CLaberynthBoard;
class CEndGameCondition;
//-------------------------


class CGameLogicLaberynth
{

public:
	CGameLogicLaberynth(	CLaberynthBoard *board, std::vector<CLaberynthBoardInput*>& vecPlayerInput, 
												std::vector<CEndGameCondition*>& vecEndGameCondition);
	virtual ~CGameLogicLaberynth();

	void	Update			(float elapsedTime);
	void	RenderScene	(CRenderManager* renderManager, CFontManager* fontManager, bool inSettings);
	bool	Exit				() const;
	void	SetForceRJ1	(float forceRJ1);
	void	SetForceRJ2	(float forceRJ2);

private:

	CLaberynthBoard*										m_pBoard;
	std::vector<CLaberynthBoardInput*>	m_VecPlayerInput;
	std::vector<CEndGameCondition*>			m_VecEndGameCondition;
	bool																m_bFinished;
	bool																m_bExit;
	float																m_fTimeToShowEndGame;
	float																m_fCountTime;
	ETypeEndGameCondition								m_eTypeOfEndGameCondition;
};

#endif //INC_BOARD_GAME_LABERYNTH_H_