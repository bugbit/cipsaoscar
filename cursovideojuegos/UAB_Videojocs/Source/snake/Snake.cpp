#include "Snake.h"

#define BODY_SIZE 10.f

CSnake::CSnake(void)
: m_Direction(DIR_RIGHT)
, m_bMove(false)
, m_bGrow(false)
{
	SBody body;
	body.m_fPosX = 400;
	body.m_fPosY = 400;
	m_Snake.push_back(body);
}

CSnake::~CSnake(void)
{
}

void CSnake::Render(CDebugPrintText2D& printText2d)
{
	for(int cont = 0; cont < m_Snake.size(); cont++)
	{
		printText2d.PrintText(m_Snake[cont].m_fPosX,m_Snake[cont].m_fPosY,0xffffffff,"X");	
	}
}

void CSnake::Update(float dt)
{
	UpdateInputActions( dt );

	if (m_bMove)
	{
		m_bMove = false;

		if (m_bGrow)
		{
			m_bGrow = false;
			m_Snake.push_back(m_Snake[0]);
		}

		for( int cont = m_Snake.size()-1; cont > 0; cont--)
		{
			m_Snake[cont] = m_Snake[cont-1];
		}
		switch (m_Direction)
		{
		case DIR_RIGHT:
			m_Snake[0].m_fPosX += BODY_SIZE;
			break;

		case DIR_LEFT:
			m_Snake[0].m_fPosX -= BODY_SIZE;
			break;

		case DIR_UP:
			m_Snake[0].m_fPosY -= BODY_SIZE;
			break;

		case DIR_DOWN:
			m_Snake[0].m_fPosY += BODY_SIZE;
			break;

		default:
			break;
			//ERROR!!!
		}

		
	}
}