#ifndef _3_EN_RAYA_H
#define _3_EN_RAYA_H

#include "DebugPrintText2D.h"
#include "Board.h"

class CTresEnRaya
{
public:
	CTresEnRaya();
	~CTresEnRaya();

	void		Render		(CDebugPrintText2D& printText2d);
	void		Update		(float dt);

private:

	CBoard m_board;
	float m_Time;

};

#endif //_3_EN_RAYA_H