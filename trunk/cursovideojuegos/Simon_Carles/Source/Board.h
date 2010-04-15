#ifndef _BOARD_H
#define _BOARD_H

#include "DebugPrintText2D.h"
#include <vector>

typedef enum ESymbols { SYMBOL_A, SYMBOL_B, SYMBOL_C, SYMBOL_D, SYMBOL_LAST };
typedef enum ELoopState { LOOPSTATE_CHECK, LOOPSTATE_SHOW, LOOPSTATE_WAIT, LOOPSTATE_LAST };

class CBoard {
	public:
		CBoard();
		~CBoard();

		void Reiniciar();
		void Generate();
		ESymbols Show();
		bool Check(ESymbols symbol);
		bool Increase();
		ELoopState GetEstado();
		void SetEstado(ELoopState);

	private:
		std::vector<ESymbols> m_Symbols;
		unsigned int m_Count;
		ELoopState m_Estado;
};

#endif //_BOARD_H