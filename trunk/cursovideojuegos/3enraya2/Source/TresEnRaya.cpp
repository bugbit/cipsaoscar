#include "TresEnRaya.h"

#include "InputManager.h"

#include "BoardRenderPrintText.h" // class's header file
#include "LocalHumanPlayer.h"

CTresEnRaya::CTresEnRaya():m_TurnoPlayer(0),m_Time(0.f)
{	
	m_BoardRender=new CBoardRenderPrintText(800, 640);
	/*
	m_Board.set(CBoard::EFicha::P1,1,1);
	m_Board.set(CBoard::EFicha::P2,2,1);
	m_Board.set(CBoard::EFicha::P1,3,1);
	m_Board.set(CBoard::EFicha::P2,1,2);
	m_Board.set(CBoard::EFicha::P1,2,2);
	m_Board.set(CBoard::EFicha::P2,3,2);
	m_Board.set(CBoard::EFicha::P1,1,3);
	m_Board.set(CBoard::EFicha::P2,2,3);
	m_Board.set(CBoard::EFicha::P2,3,3);
	**/
	m_P[0]=new CLocalHumanPlayer(CBoard::EFicha::P1,"P1");
	m_P[1]=new CLocalHumanPlayer(CBoard::EFicha::P2,"P2");
}

CTresEnRaya::~CTresEnRaya()
{
	delete m_BoardRender;
	delete m_P[0];
	delete m_P[1];
}

void CTresEnRaya::Render		(CDebugPrintText2D& printText2d)
{
	m_BoardRender->render(m_Board,printText2d);
	if (m_Board.IsFinished())
	{
	}
	else
		printText2d.PrintText(0,500,0xffffffff,"%s : %s",m_P[m_TurnoPlayer]->getName().c_str(),m_P[m_TurnoPlayer]->getCoordStr());
}

void CTresEnRaya::Update		(float dt)
{
	m_Time +=dt;
	if (m_Time>400) m_Time = 0.f;
	if (!m_Board.IsFinished() && m_Time>0)
	{
		if (m_P[m_TurnoPlayer]->isChangePlayer())
			m_TurnoPlayer=(m_TurnoPlayer+1) % 2;
		m_P[m_TurnoPlayer]->update(m_Board,dt);
	}
}