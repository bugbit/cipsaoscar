#ifndef _SNAKE_H
#define _SNAKE_H

#include "BaseGame.h"

class CSnake: public CBaseGame
{
public:
	CSnake();
	~CSnake();

	void		Render		(CDebugPrintText2D& printText2d);
	void		Update		(float dt);

	void		Init			(){};
	void		DeInit		(){};

private:


};

#endif //_SIMON_H