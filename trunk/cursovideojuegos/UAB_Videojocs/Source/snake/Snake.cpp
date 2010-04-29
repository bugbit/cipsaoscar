#include "Snake.h"

CSnake::CSnake(float posx,float posy,int nsnake)
: m_Direction(DIR_RIGHT)
, m_DirectionOld(DIR_RIGHT)
, m_bMove(false)
, m_bGrow(false)
, m_fMoveTime(MOVE_TIME)
, m_NSnake(nsnake)
{
	SBody body;
	body.m_fPosX = posx;
	body.m_fPosY = posy;
	m_Snake.push_back(body);
}

CSnake::~CSnake(void)
{
}

void CSnake::SetDirection(Direction direction)
{
	m_DirectionOld=m_Direction;
	m_Direction=direction;
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
	if (m_Snake.size()>1 && m_Direction!=m_DirectionOld && m_Direction==GetDireccionContraria(m_DirectionOld))
		return true;
	for (int i=1;i<m_Snake.size();i++)
	{
		float dx=posx-m_Snake[i].m_fPosX;
		float dy=posy-m_Snake[i].m_fPosY;
		if ((dx>0 && dx<BODY_SIZE) && (dy>0 && dy<BODY_SIZE))
			return true;
	}

	return false;
}

Direction CSnake::GetDireccionContraria(const Direction &dir) const
{
	switch(dir)
	{
	case Direction::DIR_DOWN:
		return Direction::DIR_UP;
	case Direction::DIR_UP:
		return Direction::DIR_DOWN;
	case Direction::DIR_LEFT:
		return Direction::DIR_RIGHT;
	case Direction::DIR_RIGHT:
		return Direction::DIR_LEFT;
	}
}