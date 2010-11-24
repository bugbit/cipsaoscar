#pragma once

//--Forward Declaration--
class CQuakePlayer;

class CQuakePlayerInput
{
public:
	CQuakePlayerInput(void);
	~CQuakePlayerInput(void);

	inline void	SetPlayer					(CQuakePlayer * player) {m_Player = player;}
	virtual void UpdateInputAction	(float elapsedTime)=0;	

protected:

	CQuakePlayer*	m_Player;	
};
