#include "__PCH_Laberynth.h"

//---Game Includes---
#include "TimeOutEndGameCondition.h"
//-------------------


bool CTimeOutEndGameCondition::Check (CLaberynthBoard *board, float elapsedTime) 
{
	m_fCountTime += elapsedTime;
	if( m_fCountTime  > m_fTimeOut )
	{
		return true;
	}
	return false;
}

