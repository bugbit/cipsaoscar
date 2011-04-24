//----------------------------------------------------------------------------------
// CTimeOutEndGameCondition class
// Author: Enric Vergara
//
// Description:
// Clase encargada de comprobar la finalizacion de la partida en el caso de que haya transcurrido un tiempo establecido.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_TIME_OUT_END_GAME_CONDITION_H_
#define INC_TIME_OUT_END_GAME_CONDITION_H_

//---Game Includes---
#include "EndGameCondition.h"
//-------------------

//---Forward Declarations---
class CLaberynthBoard;
//--------------------------

class CTimeOutEndGameCondition : public CEndGameCondition 
{
public:
	CTimeOutEndGameCondition (float timeOut) :	CEndGameCondition(TC_TIME_OUT), m_fCountTime(0.f), 
																							m_fTimeOut(timeOut) {assert( m_fTimeOut > 0.f );}
	virtual ~CTimeOutEndGameCondition () {/*Nothing*/;}

	//-----CEndGameCondition Interface--------
	bool	Check	(CLaberynthBoard *board, float elapsedTime);
	//-----------------------------------------

private:
	float	m_fTimeOut;
	float	m_fCountTime;
};

#endif //INC_TIME_OUT_END_GAME_CONDITION_H_