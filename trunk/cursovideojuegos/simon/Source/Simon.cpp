#include "Simon.h"
#include "InputManager.h"
#include "time.h"

CSimon::CSimon():m_State(CSimon::ESTATE::STATE_START),m_Time(0.f),m_SymbolPlayer(SYMBOL_LAST)
{
	srand(unsigned(time(0)));
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
		m_board.Init();
		m_State=ESTATE::STATE_SHOW;
		m_Time=0;
	}
}

void CSimon::RenderStateShow(CDebugPrintText2D& printText2d)
{
	printText2d.PrintText(0, 0, 0xFFFFFFFF, "REPITE LA SIGUIENTE SECUENCIA:");
	Print(printText2d,m_board.Show());
}

void CSimon::UpdateStateShow(float dt)
{
	m_Time += dt;
	if (m_Time>0.8)
	{		
		if(m_board.nextSymbol())
		{
			m_Time = 0;
		}
		else
		{
			m_State=ESTATE::STATE_CHECK;
			m_board.ResetCount();m_SymbolPlayer= SYMBOL_LAST;
			m_TimeNextSymbol=m_Time=0;
		}
	}
}

void CSimon::RenderStateCheck(CDebugPrintText2D& printText2d)
{
	Print(printText2d,m_SymbolPlayer);
}

void CSimon::UpdateStateCheck(float dt)
{
	CInputManager *input = CInputManager::GetInstance();

	m_Time += dt;
	m_TimeNextSymbol += dt;
	if (m_Time>0)
	{			
		if( input->DoAction("teclaA") )
		{
			m_SymbolPlayer=SYMBOL_A;
			m_Time = -0.5;
			m_TimeNextSymbol=10;
		}
		if( input->DoAction("teclaB") )
		{
			m_SymbolPlayer=SYMBOL_B;
			m_Time = -0.5;
			m_TimeNextSymbol=10;
		}
		if( input->DoAction("teclaC") )
		{
			m_SymbolPlayer=SYMBOL_C;
			m_Time = -0.5;
			m_TimeNextSymbol=10;
		}
		if( input->DoAction("teclaD") )
		{
			m_SymbolPlayer=SYMBOL_D;
			m_Time = -0.5;
			m_TimeNextSymbol=10;
		}
	}
	if (m_TimeNextSymbol>1)
	{
		m_TimeNextSymbol = 0;
		if (m_SymbolPlayer==ESymbols::SYMBOL_LAST)
			m_State=ESTATE::STATE_ENDGAME;		
		else
		{
			if (!m_board.Check(m_SymbolPlayer))
				m_State=ESTATE::STATE_ENDGAME;
			else if (m_board.IsFinished())
				m_State=ESTATE::STATE_ENDGAME;
			else
				m_SymbolPlayer=ESymbols::SYMBOL_LAST;
		}
		if (m_State==ESTATE::STATE_ENDGAME)
			m_Time=0;
	}
}

void CSimon::RenderStateEndGame(CDebugPrintText2D& printText2d)
{
	if (m_board.IsFinished())
	{
		printText2d.PrintText(0,0,0xffffffff,"Muy bien");
	}
	else
	{
		printText2d.PrintText(0,0,0xffffffff,"ERROR!!!");
	}
}

void CSimon::UpdateStateEndGame(float dt)
{
	m_Time += dt;

	if (m_Time>2)
	{
		m_State=ESTATE::STATE_START;
		if (!m_board.IsFinished())
			m_board.ResetSymbols();
	}
}

void CSimon::Print(CDebugPrintText2D& pt2d, ESymbols e)
{
	switch (e)
	{
		case SYMBOL_A:
			pt2d.PrintText(0, 20, 0xFF0000FF, "A");
			break;
		case SYMBOL_B:
			pt2d.PrintText(0, 20, 0xFFFF0000, "B");
			break;
		case SYMBOL_C:
			pt2d.PrintText(0, 20, 0xFF00FFFF, "C");
			break;
		case SYMBOL_D:
			pt2d.PrintText(0, 20, 0xFF00FF00, "D");
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
		case ESTATE::STATE_CHECK:
			RenderStateCheck(printText2d);
			break;
		case ESTATE::STATE_ENDGAME:
			RenderStateEndGame(printText2d);
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
		case ESTATE::STATE_CHECK:
			UpdateStateCheck(dt);
			break;
		case ESTATE::STATE_ENDGAME:
			UpdateStateEndGame(dt);
			break;
		default:
			break;
	}
}