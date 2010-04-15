#ifndef _3_EN_RAYA_H
#define _3_EN_RAYA_H

#include "DebugPrintText2D.h"
#include "Board.h"
#include "Boardrender.h" // class's header file
#include "Player.h"

class CTresEnRaya
{
public:
	CTresEnRaya();
	~CTresEnRaya();

	void		Render		(CDebugPrintText2D& printText2d);
	void		Update		(float dt);

private:

	CBoard m_Board;
	CBoardRender *m_BoardRender;
	CPlayer *m_P[2];
	int m_TurnoPlayer;
	float m_Time;
};

#endif //_3_EN_RAYA_H