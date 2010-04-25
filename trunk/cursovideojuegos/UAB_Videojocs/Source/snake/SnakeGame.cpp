#include "SnakeGame.h"
#include "../InputManager.h"

#include <time.h>

#define MOVE_TIME 0.1f
#define XI	  40
#define YI	  40
#define XF	  750
#define YF	  550

CSnakeGame::CSnakeGame()
: m_bIsEnd(false) 
, m_fMoveTime(MOVE_TIME)
, m_fSpeed(100.f)
, m_fGrowTime(5.f)
, m_Nivel(1)
, m_CoutBonus(0)
{
	srand(unsigned(time(0)));
	m_Snake= new CSnake();
	BuildScreenNivel();
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
	m_Snake->Render(printText2d);
	for (int i=0;i<m_Items.size();i++)
		m_Items[i]->Render(printText2d);
}

void CSnakeGame::Update		(float dt)
{
	if (m_bIsEnd)
		return;
	UpdateInputActions(dt);

	
	/*
	if (	bodyHead.m_fPosX > 800 || bodyHead.m_fPosX < 0 ||
				bodyHead.m_fPosY > 600 || bodyHead.m_fPosY < 0 )
	{
		m_bIsEnd = true;
	}
	**/

	m_Snake->Update(dt);

	SBody bodyHead=m_Snake->GetBodyHead();
	Collision(bodyHead.m_fPosX,bodyHead.m_fPosY);

	//Update Logic Game:
	/*
	m_fGrowTime -= dt;
	if (m_fGrowTime <= 0)
	{
		m_fGrowTime = 5.f;
		m_Snake->Grow();
	}
	**/

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

	if( input->DoAction("snake_down") )
	{
		m_Snake->SetDirection (DIR_DOWN);
	}
	else if( input->DoAction("snake_up") )
	{
		m_Snake->SetDirection (DIR_UP);
	}
	else if( input->DoAction("snake_right") )
	{
		m_Snake->SetDirection (DIR_RIGHT);
	}
	else if( input->DoAction("snake_left") )
	{
		m_Snake->SetDirection (DIR_LEFT);
	}
}

void CSnakeGame::Collision(float posx,float posy)
{
	for (int i=0;i<m_Items.size();i++)
		if (m_Items[i]->IsCollision(posx,posy))
		{
			if (m_Items[i]->GetType()==ETYPEITEMS::TYPEITEMS_BONUS)
			{
				m_Items[i]->SetVisible(false);
				m_Snake->Grow();
				m_CoutBonus--;
				if (m_CoutBonus<=0)
				{
					m_Nivel++;
					BuildScreenNivel();
				}
			}
			else
				m_bIsEnd=true;
		}

		if (m_Snake->IsCollision(posx,posy))
			m_bIsEnd=true;
}

void CSnakeGame::BuildScreenNivel()
{
	m_Items.clear();
	m_CoutBonus=2*m_Nivel;
	for (int i=0;i<m_CoutBonus;i++)
	{
		m_Items.push_back(new CItems(XI+BODY_SIZE+(rand()%(int)(XF-XI-2*BODY_SIZE)),YI+BODY_SIZE+(rand()%(int)(YF-YI-2*BODY_SIZE)),TYPEITEMS_BONUS));
	}
	int coutWall=1+(rand() % (10*m_Nivel));
	for (int i=0;i<coutWall;i++)
	{
		int lng=1+(rand() % (5*m_Nivel));
		float posx=XI+BODY_SIZE+(rand() % (int)(XF-XI-2*lng*BODY_SIZE));
		float posy=YI+BODY_SIZE+(rand() % (int)(YF-YI-2*lng*BODY_SIZE));
		for (int i=0;i<lng;i++,posx += BODY_SIZE)
		{
			m_Items.push_back(new CItems(posx,posy,TYPEITEMS_WALL));
		}
	}
	for (int x=XI;x<XF;x += BODY_SIZE)
	{
		m_Items.push_back(new CItems(x,YI,TYPEITEMS_WALL));
		m_Items.push_back(new CItems(x,YF-BODY_SIZE,TYPEITEMS_WALL));
	}
	for (int y=YI;y<YF-BODY_SIZE;y += BODY_SIZE)
	{
		m_Items.push_back(new CItems(XI,y,TYPEITEMS_WALL));
		m_Items.push_back(new CItems(XF-BODY_SIZE,y,TYPEITEMS_WALL));
	}
}