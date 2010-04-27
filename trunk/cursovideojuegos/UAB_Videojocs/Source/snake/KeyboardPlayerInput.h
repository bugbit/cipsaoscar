#ifndef _KEYBOARD_PLAYER_INPUT_H
#define _KEYBOARD_PLAYER_INPUT_H

#include "PlayerInput.h"

class CKeyboardPlayerInput :
	public CPlayerInput
{
public:
	CKeyboardPlayerInput(void);
	~CKeyboardPlayerInput(void);
	virtual void UpdateInputAction	(float dt);
	inline void	SetMoveUp					(std::string MoveUp)		{m_sMoveUp=MoveUp;}
	inline void	SetMoveDown				(std::string MoveDown)	{m_sMoveDown=MoveDown;}
	inline void	SetMoveRight			(std::string MoveRight) {m_sMoveRight=MoveRight;}
	inline void	SetMoveLeft				(std::string MoveLeft)	{m_sMoveLeft=MoveLeft;}
private:
	std::string m_sMoveUp;
	std::string m_sMoveDown;
	std::string m_sMoveRight;
	std::string m_sMoveLeft;
};

#endif	// _KEYBOARD_PLAYER_INPUT_H