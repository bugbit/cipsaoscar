#include "TresEnRaya.h"
#include "InputManager.h"

#include "DebugPrintText2D.h"
#include "InputManager.h"

CTresEnRaya::CTresEnRaya()
{
	m_board.Generate();
	m_board.Generate();
	m_board.Generate();
	m_board.Generate();
	m_board.Generate();
	m_Time = 0.f;
	m_CheckOk = true;
}

CTresEnRaya::~CTresEnRaya()
{	

}

void CTresEnRaya::Render		(CDebugPrintText2D& printText2d)
{
	m_board.Show(printText2d);
	int dy = 400;
	if (m_board.IsFinished())
	{
		dy += printText2d.PrintText(400,dy,0xffffffff,"Muy bien");
	}
	if (!m_CheckOk)
	{
		dy += printText2d.PrintText(400,dy,0xffffffff,"ERROR!!!");
	}
	
}

void CTresEnRaya::Update		(float dt)
{
	CInputManager * input = CInputManager::GetInstance();

	m_Time +=dt;
	if (m_Time>400) m_Time = 0.f;
	int dy = 200;
	if (!m_board.IsFinished() && m_Time>=0)
	{

		if( input->DoAction("teclaA") )
		{
			m_CheckOk = m_board.Check(SYMBOL_A);
			m_Time = -0.5;
		}
		if( input->DoAction("teclaB") )
		{
			m_CheckOk = m_board.Check(SYMBOL_B);
			m_Time = -0.5;
		}
		if( input->DoAction("teclaC") )
		{
			m_CheckOk = m_board.Check(SYMBOL_C);
			m_Time = -0.5;
		}
		if( input->DoAction("teclaD") )
		{
			m_CheckOk = m_board.Check(SYMBOL_D);
			m_Time = -0.5;
		}
	}
}