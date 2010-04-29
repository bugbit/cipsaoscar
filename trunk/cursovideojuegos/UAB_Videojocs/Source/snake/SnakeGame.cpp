#include "SnakeGame.h"
#include "ItemsWall.h"
#include "ItemsBonus.h"

#include "KeyboardPlayerInput.h"

#include <time.h>
#include <sstream>

#define XI	  40
#define YI	  40
#define XF	  750
#define YF	  550

CSnakeGame::CSnakeGame()
: m_bIsEnd(false) 
, m_fSpeed(100.f)
, m_fGrowTime(5.f)
, m_Nivel(1)
, m_CoutBonus(0)
, m_MsgEnd()
{
	CSnake *s1= new CSnake(400,400,1);
	CSnake *s2= new CSnake(200,200,2);
	CKeyboardPlayerInput *pi1=new CKeyboardPlayerInput();
	CKeyboardPlayerInput *pi2=new CKeyboardPlayerInput();
	pi1->SetSnake(s1);
	pi2->SetSnake(s2);
	pi2->SetMoveUp("snake2_up");
	pi2->SetMoveDown("snake2_down");
	pi2->SetMoveRight("snake2_right");
	pi2->SetMoveLeft("snake2_left");
	m_Snakes.push_back(s1);
	//m_Snakes.push_back(s2);
	m_PlayerInputs.push_back(pi1);
	//m_PlayerInputs.push_back(pi2);
	srand(unsigned(time(0)));
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
		dy += printText2d.PrintText(400,dy,0xffffffff,m_MsgEnd.c_str());	
	}
	for (int i=0;i<m_Snakes.size();i++)
		m_Snakes[i]->Render(printText2d);
	for (int i=0;i<m_Items.size();i++)
		m_Items[i]->Render(printText2d);
}

void CSnakeGame::Update		(float dt)
{
	if (m_bIsEnd)
		return;
	UpdateInputActions(dt);

	for (int i=0;i<m_PlayerInputs.size();i++)
	{
		m_PlayerInputs[i]->UpdateInputAction(dt);
	}
	/*
	if (	bodyHead.m_fPosX > 800 || bodyHead.m_fPosX < 0 ||
				bodyHead.m_fPosY > 600 || bodyHead.m_fPosY < 0 )
	{
		m_bIsEnd = true;
	}
	**/

	for (int i=0;i<m_Snakes.size();i++)
	{
		CSnake &snake=*m_Snakes[i];
		snake.Update(dt);
		SBody bodyHead=snake.GetBodyHead();
		CollisionItems(bodyHead.m_fPosX,bodyHead.m_fPosY);
		if (snake.IsCollision(bodyHead.m_fPosX,bodyHead.m_fPosY))
		{
			std::ostringstream ms;
			ms << "El snake " << snake.GetNSnake() << " se ha comido a si misma" << std::ends;
			m_MsgEnd=ms.str();
			m_bIsEnd=true;
		}
	}

	for (int i=0;i<m_Items.size();i++)
	{
		m_Items[i]->Update(dt);
	}

	//Update Logic Game:
	/*
	m_fGrowTime -= dt;
	if (m_fGrowTime <= 0)
	{
		m_fGrowTime = 5.f;
		m_Snake->Grow();
	}
	**/	
}

void CSnakeGame::UpdateInputActions( float dt )
{
}

void CSnakeGame::CollisionItems(float posx,float posy)
{
	for (int i=0;i<m_Snakes.size();i++)
	{
		CollisionItems(*m_Snakes[i],posx,posy);
	}
}
void CSnakeGame::CollisionItems(CSnake &snake,float posx,float posy)
{
	for (int i=0;i<m_Items.size();i++)
	{
		if (m_Items[i]->IsCollision(posx,posy))
		{
			if (m_Items[i]->GetType()==ETYPEITEMS::TYPEITEMS_BONUS)
			{
				m_Items[i]->SetVisible(false);
				snake.Grow();
				m_CoutBonus--;
				if (m_CoutBonus<=0)
				{
					m_Nivel++;
					BuildScreenNivel();
				}
			}
			else
			{
				std::ostringstream ms;
				ms << "El snake " << snake.GetNSnake() << " Ha chocado contra la pared" << std::ends;
				m_MsgEnd=ms.str();
				m_bIsEnd=true;
			}
		}
	}
}

void CSnakeGame::BuildScreenNivel()
{
	m_Items.clear();
	m_CoutBonus=2*m_Nivel;
	for (int i=0;i<m_CoutBonus;i++)
	{
		/*
		CItemBonus *bonus=new CItemsBonus();
		bonus->GeneratePosition();
		m_Items.push_back(bonus);
		**/
		m_Items.push_back(new CItemsBonus(XI+BODY_SIZE+(rand()%(int)(XF-XI-2*BODY_SIZE)),YI+BODY_SIZE+(rand()%(int)(YF-YI-2*BODY_SIZE))));
	}
	int coutWall=1+(rand() % (10*m_Nivel));
	for (int i=0;i<coutWall;i++)
	{
		int lng=1+(rand() % (5*m_Nivel));
		float posx=XI+BODY_SIZE+(rand() % (int)(XF-XI-2*lng*BODY_SIZE));
		float posy=YI+BODY_SIZE+(rand() % (int)(YF-YI-2*lng*BODY_SIZE));
		for (int i=0;i<lng;i++,posx += BODY_SIZE)
		{
			m_Items.push_back(new CItemsWall(posx,posy));
		}
	}
	for (int x=XI;x<XF;x += BODY_SIZE)
	{
		m_Items.push_back(new CItemsWall(x,YI));
		m_Items.push_back(new CItemsWall(x,YF-BODY_SIZE));
	}
	for (int y=YI;y<YF-BODY_SIZE;y += BODY_SIZE)
	{
		m_Items.push_back(new CItemsWall(XI,y));
		m_Items.push_back(new CItemsWall(XF-BODY_SIZE,y));
	}
}