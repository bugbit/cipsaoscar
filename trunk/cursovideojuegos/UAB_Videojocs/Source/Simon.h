#ifndef _SIMON_H
#define _SIMON_H

#include "BaseGame.h"

class CSimon: public CBaseGame
{
public:
	CSimon();
	~CSimon();

	void		Render		(CDebugPrintText2D& printText2d);
	void		Update		(float dt);

	void		Init			(){};
	void		DeInit		(){};

private:


};

#endif //_SIMON_H