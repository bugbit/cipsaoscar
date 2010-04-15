#ifndef _3_SIMON_H
#define _3_SIMON_H

#include "DebugPrintText2D.h"
#include "Board.h"

class CSimon
{
public:
	CSimon();
	~CSimon();

	void		Render		(CDebugPrintText2D& printText2d);
	void		Update		(float dt);

private:

	enum ESTATE
	{
		STATE_START,STATE_SHOW,STATE_CHECK,STATE_ENDGAME
	};

	ESTATE		m_State;
	CBoard		m_board;
	float		m_Time;
	bool		m_CheckOk;

	void RenderStateStart(CDebugPrintText2D& printText2d);
	void UpdateStateStart(float dt);
	void RenderStateShow(CDebugPrintText2D& printText2d);
	void UpdateStateShow(float dt);

	void Print(CDebugPrintText2D& pt2d, ESymbols e);
};

#endif //_3_SIMON_H