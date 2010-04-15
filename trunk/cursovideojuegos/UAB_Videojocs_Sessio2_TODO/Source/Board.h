#ifndef _BOARD_H
#define _BOARD_H

#include "DebugPrintText2D.h"

#include <vector>

typedef enum ESymbols { SYSMBOL_A, SYSMBOL_B, SYSMBOL_C, SYSMBOL_D, SYSMBOL_LAST};

class CBoard
{
public:
	CBoard();
	~CBoard();
	
	void		Generate();
	void		Show(CDebugPrintText2D& printText2d);
	bool        Check(ESymbols symbol);
	bool		IsFinished();

	void		Render		(CDebugPrintText2D& printText2d);
	

private:

	std::vector <ESymbols>	m_Symbols;
	int						m_Count;
	bool					m_isFinished;

};

#endif //_BOARD_H