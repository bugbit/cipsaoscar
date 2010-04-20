#ifndef _3_EN_RAYA_H
#define _3_EN_RAYA_H

#include "DebugPrintText2D.h"
#include "Board.h"
#include "BaseGame.h"

class CTresEnRaya: public CBaseGame
{
public:
	CTresEnRaya();
	~CTresEnRaya();

	void		Render		(CDebugPrintText2D& printText2d);
	void		Update		(float dt);

	void		Init			(){};
	void		DeInit		(){};

private:

	CBoard	m_board;
	float		m_Time;
	bool		m_CheckOk;

};

#endif //_3_EN_RAYA_H