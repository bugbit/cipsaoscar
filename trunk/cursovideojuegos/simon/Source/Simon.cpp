#include "Simon.h"

#include "InputManager.h"

CSimon::CSimon():m_State(CSimon::ESTATE::STATE_START),m_Time(0.f),m_CheckOk(true)
{
}

CSimon::~CSimon()
{
	
}

void CSimon::RenderStateStart(CDebugPrintText2D& printText2d)
{
	printText2d.PrintText(0,0,0xffffffff,"PULSE S PARA EMPEZAR");
}

void CSimon::UpdateStateStart(float dt)
{
	CInputManager * input = CInputManager::GetInstance();

	if( input->DoAction("startGame") )
	{
		m_board.Generate();
		m_board.ResetCount();
		m_State=ESTATE::STATE_SHOW;
	}
}

void CSimon::RenderStateShow(CDebugPrintText2D& printText2d)
{
	printText2d.PrintText(0, 0, 0xFFFFFFFF, "REPITE LA SIGUIENTE SECUENCIA:");
	Print(printText2d,m_board.Show());
}

void CSimon::UpdateStateShow(float dt)
{
}

void CSimon::Print(CDebugPrintText2D& pt2d, ESymbols e)
{
	switch (e)
	{
		case SYMBOL_A:
			pt2d.PrintText(0, 20, 0xFF0000FF, "W");
			break;
		case SYMBOL_B:
			pt2d.PrintText(0, 20, 0xFFFF0000, "D");
			break;
		case SYMBOL_C:
			pt2d.PrintText(0, 20, 0xFF00FFFF, "S");
			break;
		case SYMBOL_D:
			pt2d.PrintText(0, 20, 0xFF00FF00, "A");
			break;
	}
}

void CSimon::Render		(CDebugPrintText2D& printText2d)
{
	/*
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
	**/

	switch (m_State)
	{
		case ESTATE::STATE_START:
			RenderStateStart(printText2d);
			break;
		case ESTATE::STATE_SHOW:
			RenderStateShow(printText2d);
			break;
		default:
			break;
	}
}

void CSimon::Update		(float dt)
{
	/*
	CInputManager * input = CInputManager::GetInstance();

	m_Time +=dt;
	if (m_Time>400) m_Time = 0.f;
	int dy = 200;
	if (!m_board.IsFinished() && m_Time>=0)
	{

		if( input->DoAction("teclaA") )
		{
			m_CheckOk = m_board.Check(SYSMBOL_A);
			m_Time = -0.5;
		}
		if( input->DoAction("teclaB") )
		{
			m_CheckOk = m_board.Check(SYSMBOL_B);
			m_Time = -0.5;
		}
		if( input->DoAction("teclaC") )
		{
			m_CheckOk = m_board.Check(SYSMBOL_C);
			m_Time = -0.5;
		}
		if( input->DoAction("teclaD") )
		{
			m_CheckOk = m_board.Check(SYSMBOL_D);
			m_Time = -0.5;
		}
	}
	*/
	switch (m_State)
	{
		case CSimon::ESTATE::STATE_START:
			UpdateStateStart(dt);
			break;
		case ESTATE::STATE_SHOW:
			UpdateStateShow(dt);
			break;
		default:
			break;
	}
}