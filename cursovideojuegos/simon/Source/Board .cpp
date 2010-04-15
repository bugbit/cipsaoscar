#include "Board.h"

CBoard::CBoard()
{
	Init();
}

CBoard::~CBoard()
{

}

void CBoard::Init()
{
	ResetCount();
	m_isFinished=false;
}

void CBoard::ResetSymbols()
{
	m_Symbols.clear();
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

	symbol=(ESymbols) (rand()%SYMBOL_LAST);

	m_Symbols.push_back(symbol);
}

ESymbols CBoard::Show()
{
	return m_Symbols[m_Count  % m_Symbols.size() ];		
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