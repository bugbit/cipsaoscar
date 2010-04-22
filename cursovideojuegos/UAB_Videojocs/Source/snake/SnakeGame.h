#ifndef _SNAKEGAME_H
#define _SNAKEGAME_H

#include "../BaseGame.h"

#include "Snake.h"

class CSnakeGame: public CBaseGame
{
public:
	CSnakeGame();
	~CSnakeGame();

	void		Render							(CDebugPrintText2D& printText2d);
	void		Update							(float dt);

	void		Init							(){};
	void		DeInit							(){};

private:
	bool								m_bIsEnd;
	
	float								m_fSpeed;
	float								m_fMoveTime;
	

	float								m_fGrowTime;
	

	CSnake								*m_Snake;

	void		UpdateInputActions	(float dt);
private:

	
};

#endif //_SNAKE_H