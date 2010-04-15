#include "TresEnRaya.h"

#include "InputManager.h"

CTresEnRaya::CTresEnRaya()
{	
	m_board.Generate();
	m_board.Generate();
	m_board.Generate();
	m_board.Generate();
	m_board.Generate();

	m_Time=0;
}

CTresEnRaya::~CTresEnRaya()
{
	
}

void CTresEnRaya::Render		(CDebugPrintText2D& printText2d)
{
	m_board.Show(printText2d);	
}

void CTresEnRaya::Update		(float dt)
{
	m_Time += dt;
	if (m_Time>40000000000)
		m_Time=0.f;
	if (m_board.IsFinished)
	{
	}
	else if (m_Time>=0)
	{
		//Action A
			//Check(A)
			//m_Time = -2000;
	}
}