#include "Snake.h"

#include "../InputManager.h"

int CSnake::PIXELBODY=8;

CSnake::CSnake():m_MaxX(800),m_MaxY(600)
{
	Init();
}

CSnake::~CSnake()
{	

}

void CSnake::Init()
{
	m_PosX=m_MaxX/2;
	m_PosY=m_MaxY/2;
	m_Bodies.clear();
	m_Bodies.push_back(new CBodySnake(m_PosX,m_PosY));
	m_Direction=EDIRECTIONS::RIGHT;
	m_IsEnd=m_IsGrown=false;
	m_Speed=10;
	m_Time=0;
	m_PosXGrown=m_PosYGrown=0;
}

void CSnake::DeInit()
{
}

void CSnake::Render		(CDebugPrintText2D& printText2d)
{
	if (m_IsEnd)
		printText2d.PrintText(m_MaxX/2,m_MaxY/2,0xffffffff,"END GAME");
	for (int i=0;i<m_Bodies.size();i++)
		printText2d.PrintText((int)m_Bodies[i]->GetPosX(), (int)m_Bodies[i]->GetPosY(),0xffffffff,"X");			
}

void CSnake::UpdateSnake(float dt)
{
	switch (m_Direction)
	{
		case EDIRECTIONS::RIGHT:
			m_PosX += dt*m_Speed;
			if (m_PosX>m_MaxX)
				m_IsEnd=true;
			break;
		case EDIRECTIONS::LEFT:
			m_PosX -= dt*m_Speed;
			if (m_PosX<0)
				m_IsEnd=true;
			break;
		case EDIRECTIONS::UP:
			m_PosY -= dt*m_Speed;
			if (m_PosY<0)
				m_IsEnd=true;
			break;
		case EDIRECTIONS::DOWN:
			m_PosY += dt*m_Speed;
			if (m_PosY>m_MaxY)
				m_IsEnd=true;
			break;
	}
	if (m_IsGrown)
	{
		CBodySnake *body=m_Bodies[m_Bodies.size()-1];
		if (abs(body->GetPosX()-m_PosXGrown)>=PIXELBODY || abs(body->GetPosY()-m_PosYGrown)>=PIXELBODY)
		{
			m_Bodies.push_back(new CBodySnake(m_PosXGrown,m_PosYGrown));
			m_IsGrown=false;
		}
	}
	for (int i=m_Bodies.size()-1;i>=1;i--)
	{
		m_Bodies[i]->Set(*m_Bodies[i-1]);
	}
	m_Bodies[0]->Set(m_PosX,m_PosY);
}

void CSnake::GrownSnake()
{
	m_IsGrown=true;
	m_Time=0;
	CBodySnake *body=m_Bodies[m_Bodies.size()-1];
	m_PosXGrown=body->GetPosX();
	m_PosYGrown=body->GetPosY();
}

void CSnake::Update		(float dt)
{
	CInputManager * input = CInputManager::GetInstance();

	m_Time += dt;
	if (!m_IsEnd)
	{
		if (input->DoAction("snake_right"))
		{
			m_Direction=EDIRECTIONS::RIGHT;
		}
		if (input->DoAction("snake_left"))
		{
			m_Direction=EDIRECTIONS::LEFT;
		}
		if (input->DoAction("snake_up"))
		{
			m_Direction=EDIRECTIONS::UP;
		}
		if (input->DoAction("snake_down"))
		{
			m_Direction=EDIRECTIONS::DOWN;
		}
		if (m_Time>5)
		{
			GrownSnake();
		}
		UpdateSnake(dt);		
	}
}