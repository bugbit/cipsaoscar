#ifndef _BASE_GAME_H
#define _BASE_GAME_H

#include "DebugPrintText2D.h"


class CBaseGame
{
public:
	CBaseGame();
	~CBaseGame();

	virtual void		Render		(CDebugPrintText2D& printText2d) = 0;
	virtual void		Update		(float dt) = 0;

	virtual void		Init			() = 0;
	virtual void		DeInit		() = 0;



private:

};

#endif //_BASE_GAME_H