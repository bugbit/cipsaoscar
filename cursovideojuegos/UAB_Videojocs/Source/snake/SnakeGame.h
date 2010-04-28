#ifndef _SNAKEGAME_H
#define _SNAKEGAME_H

#include "../BaseGame.h"
#include "Snake.h"
#include "PlayerInput.h"
#include "Items.h"

#include <vector>

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
	

	float								m_fGrowTime;
	
	std::vector <CSnake *>				m_Snakes;
	std::vector <CPlayerInput *>		m_PlayerInputs;
	std::vector	<CItems *>				m_Items;
	int									m_Nivel;
	int									m_CoutBonus;

	void		UpdateInputActions	(float dt);
	void		Collision(float posx,float posy);
	void		Collision(CSnake &snake,float posx,float posy);
	void		BuildScreenNivel();
private:

	
};

#endif //_SNAKE_H