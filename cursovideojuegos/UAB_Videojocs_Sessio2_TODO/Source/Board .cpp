#include "Board.h"

CBoard::CBoard()
{
	m_Count=0;
	m_isFinished=false;
}

CBoard::~CBoard()
{

}

void CBoard::Render(CDebugPrintText2D& printText2d)
{
}

void CBoard::Generate()
{
	ESymbols symbol;

	symbol=(ESymbols) (rand()%SYSMBOL_LAST);

	m_Symbols.push_back(symbol);
}

void CBoard::Show(CDebugPrintText2D& printText2d)
{
	int dy=0;
	for (int cont=0;cont<m_Symbols.size();cont++)
	{
		switch(m_Symbols[cont])
		{
			case ESymbols::SYSMBOL_A:
				dy += printText2d.PrintText(400,dy,0xffffffff,"A");
				break;	
			case ESymbols::SYSMBOL_B:
				dy += printText2d.PrintText(400,dy,0xffffffff,"B");
				break;	
			case ESymbols::SYSMBOL_C:
				dy += printText2d.PrintText(400,dy,0xffffffff,"C");
				break;	
			case ESymbols::SYSMBOL_D:
				dy += printText2d.PrintText(400,dy,0xffffffff,"D");
				break;	
			default:
				break;
		}
	}
		
}

bool CBoard::Check(ESymbols symbol)
{
	bool isOk=false;

	if (m_Symbols[m_Count]==symbol)
		isOk=true;
	m_Count++;
	if (m_Count ==m_Symbols.size())
	{
		m_Count=0;
		m_isFinished=true;
	}
	return isOk;
}

bool CBoard::IsFinished()
{
	/*if (m_isFinished)
	{
		m_isFinished=true;
		return true;
	}
	else
	{
		return false;
	}*/

	return m_isFinished;
}