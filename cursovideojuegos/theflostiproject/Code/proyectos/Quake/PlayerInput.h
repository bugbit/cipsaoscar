#pragma once

#include "Player.h"

//--Forward Declaration--

class CPlayerInput
{
public:
	inline CPlayerInput(void):m_pPlayer(NULL)	{}
	inline ~CPlayerInput(void)	{	}
	inline void											SetPlayer						(CPlayer * player) {m_pPlayer = player;}
	inline bool											IsOk								() const { return !m_pPlayer==NULL || m_pPlayer->IsOk(); }
	virtual void										UpdateInputAction		(float elapsedTime)=0;	
protected:
	CPlayer*												m_pPlayer;	
};
