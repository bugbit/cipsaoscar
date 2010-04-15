#include "Board.h"

CBoard::CBoard()
{
	ResetCount();
	m_isFinished=false;
}

CBoard::~CBoard()
{

}

void CBoard::ResetCount()
{
	m_Count=0;
}

bool CBoard::nextSymbol()
{
	if (m_Count>=m_Symbols.size())
		return false;
	m_Count++;

	return true;
}

void CBoard::Generate()
{
	ESymbols symbol;

	symbol=(ESymbols) (rand()%SYSMBOL_LAST);

	m_Symbols.push_back(symbol);
}

ESYMBOLS CBoard::Show()
{
	return m_Symbols[m_Count];		
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