#ifndef _SIMON_H
#define _SIMON_H

#include "DebugPrintText2D.h"
#include "Board.h"

class CSimon {
	public:
		CSimon();
		~CSimon();

		void Render(CDebugPrintText2D&);
		void Update(float);

	private:
		void Print(CDebugPrintText2D&, ESymbols);

		CBoard m_board;
		float m_fTime;
		bool m_hasBeep;
		ESymbols m_tecla;
};

#endif //_SIMON_H