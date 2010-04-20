#include "Board.h"
#include "InputManager.h"


CBoard::CBoard()
{
	m_Count = 0;
	m_isFinished = false;
	int cipsa_source = 0;
}


CBoard::~CBoard()
{	

}

void CBoard::Render	(CDebugPrintText2D& printText2d)
{

}

void CBoard::Update	(float dt)
{

}

void	CBoard::Show	(CDebugPrintText2D& printText2d)
{
	int dy = 0;
	for(int cont = 0; cont < m_Symbols.size(); cont++)
	{
		switch(m_Symbols[cont])
		{
		case SYMBOL_A:
			dy += printText2d.PrintText(400,dy,0xffffffff,"A");
			break;
		case SYMBOL_B:
			dy += printText2d.PrintText(400,dy,0xffffffff,"B");
			break;
		case SYMBOL_C:
			dy += printText2d.PrintText(400,dy,0xffffffff,"C");
			break;
		case SYMBOL_D:
			dy += printText2d.PrintText(400,dy,0xffffffff,"D");
			break;
		default:
			dy += printText2d.PrintText(400,dy,0xffffffff,"ERROR!!");
			break;
		}
	}
}

void CBoard::Generate ()
{
	ESymbols symbol;
	symbol = (ESymbols)(rand()%SYMBOL_LAST);
	m_Symbols.push_back(symbol);
}


bool CBoard::Check			(ESymbols symbol)
{

	bool isOk = false;
	if (m_Symbols[m_Count] == symbol)
	{
		isOk = true;
	}
	else
	{
		isOk = false;
	}

	m_Count++;
	if (m_Count == m_Symbols.size())
	{
		m_Count = 0;
		m_isFinished = true;
	}

	return isOk;
}

bool CBoard::IsFinished	()
{
	/*if (m_isFinished == true)
	{
		m_isFinished = false;
		return true;
	}
	else
	{
		return false;
	}*/
	return m_isFinished;
	
}