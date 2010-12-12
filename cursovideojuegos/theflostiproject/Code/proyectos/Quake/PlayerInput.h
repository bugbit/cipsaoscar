#pragma once

//--Forward Declaration--
class CPlayer;

class CPlayerInput
{
public:
	inline CPlayerInput(void):m_Player(NULL)	{}
	inline ~CPlayerInput(void)	{	}
	inline void	SetPlayer						(CPlayer * player) {m_Player = player;}
	virtual void UpdateInputAction	(float elapsedTime)=0;	
protected:
	CPlayer*												m_Player;	
};
