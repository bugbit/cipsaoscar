#ifndef _PLAYER_INPUT_H
#define _PLAYER_INPUT_H

#include <vector>

#include "../DebugPrintText2D.h"
#include "Snake.h"

class CPlayerInput
{
public:
	CPlayerInput();
	~CPlayerInput();

	inline void	SetSnake					(CSnake* snake) {m_Snake = snake;}
	virtual void UpdateInputAction	(float dt)=0;	

protected:

	CSnake*	m_Snake;	
	
};

#endif //_PLAYER_INPUT_H
