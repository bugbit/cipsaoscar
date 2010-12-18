#pragma once

//--Forward Declaration--
class CPlayer;

class CPlayerInput
{
public:
	inline CPlayerInput(void):m_pPlayer(NULL)	{}
	inline ~CPlayerInput(void)	{	}
	inline void											SetPlayer						(CPlayer * player) {m_pPlayer = player;}
	virtual void										UpdateInputAction		(float elapsedTime)=0;	
protected:
	CPlayer*												m_pPlayer;	
};
