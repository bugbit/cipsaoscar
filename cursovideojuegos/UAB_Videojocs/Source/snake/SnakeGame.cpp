#include "SnakeGame.h"
#include "../InputManager.h"

#define MOVE_TIME 0.1f

CSnakeGame::CSnakeGame()
: m_bIsEnd(false) 
, m_fMoveTime(MOVE_TIME)
, m_fSpeed(100.f)
, m_fGrowTime(5.f)
{
	
	m_Snake= new CSnake();
}

CSnakeGame::~CSnakeGame()
{	

}

void CSnakeGame::Render		(CDebugPrintText2D& printText2d)
{
	if (m_bIsEnd)
	{
		int dy = 400;
		dy += printText2d.PrintText(400,dy,0xffffffff,"GAME OVER");	
	}
	else
	{
		m_Snake->Render(printText2d);
	}
}

void CSnakeGame::Update		(float dt)
{
	UpdateInputActions(dt);

	SBody bodyHead=m_Snake->GetBodyHead();
	if (	bodyHead.m_fPosX > 800 || bodyHead.m_fPosX < 0 ||
				bodyHead.m_fPosY > 600 || bodyHead.m_fPosY < 0 )
	{
		m_bIsEnd = true;
	}

	m_Snake->Update(dt);

	//Update Logic Game:
	m_fGrowTime -= dt;
	if (m_fGrowTime <= 0)
	{
		m_fGrowTime = 5.f;
		m_Snake->Grow();
	}


	m_fMoveTime -= dt;
	if (m_fMoveTime <= 0 )
	{
		m_Snake->Move();
		m_fMoveTime = MOVE_TIME;
	}

	
	


}

void CSnakeGame::UpdateInputActions( float dt )
{
	CInputManager * input = CInputManager::GetInstance();

	if( input->DoAction("tecla_Down") )
	{
		m_Snake->SetDirection (DIR_DOWN);
	}
	else if( input->DoAction("tecla_Up") )
	{
		m_Snake->SetDirection (DIR_UP);
	}
	else if( input->DoAction("tecla_Right") )
	{
		m_Snake->SetDirection (DIR_RIGHT);
	}
	else if( input->DoAction("tecla_Left") )
	{
		m_Snake->SetDirection (DIR_LEFT);
	}
}


