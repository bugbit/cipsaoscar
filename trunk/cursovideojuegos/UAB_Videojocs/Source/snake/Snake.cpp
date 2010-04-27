#include "Snake.h"

CSnake::CSnake(float posx,float posy)
: m_Direction(DIR_RIGHT)
, m_bMove(false)
, m_bGrow(false)
, m_fMoveTime(MOVE_TIME)
{
	SBody body;
	body.m_fPosX = posx;
	body.m_fPosY = posy;
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
	m_fMoveTime -= dt;
	if (m_fMoveTime <= 0 )
	{
		m_bMove=true;
		m_fMoveTime = MOVE_TIME;
	}
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

bool CSnake::IsCollision(float posx,float posy)
{
	//	en la collision no se mira la cabeza que es la posicion 0
	for (int i=1;i<m_Snake.size();i++)
	{
		float dx=posx-m_Snake[i].m_fPosX;
		float dy=posy-m_Snake[i].m_fPosY;
		if ((dx>0 && dx<BODY_SIZE) && (dy>0 && dy<BODY_SIZE))
			return true;
	}

	return false;
}