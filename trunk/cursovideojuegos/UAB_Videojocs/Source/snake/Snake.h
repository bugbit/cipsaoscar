#ifndef _SNAKE_H
#define _SNAKE_H

#include "../BaseGame.h"

#include <Vector>

#include "BodySnake.h"

class CSnake: public CBaseGame
{
public:

	enum EDIRECTIONS
	{
			NONE,UP,DOWN,LEFT,RIGHT
	};

	CSnake();
	~CSnake();

	void		Render		(CDebugPrintText2D& printText2d);
	void		Update		(float dt);

	void		Init		();
	void		DeInit		();

private:

	float m_PosX,m_PosY,m_MaxX,m_MaxY,m_Speed,m_Time,m_PosXGrown,m_PosYGrown;
	static int PIXELBODY;
	EDIRECTIONS m_Direction;
	bool m_IsEnd,m_IsGrown;
	std::vector <CBodySnake *> m_Bodies;
	void UpdateSnake(float dt);
	void GrownSnake();
};

#endif //_SIMON_H