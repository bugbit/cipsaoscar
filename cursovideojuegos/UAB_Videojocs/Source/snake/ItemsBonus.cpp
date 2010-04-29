#include "ItemsBonus.h"

#define ITEM_SIZE 10.f

#define	TIME_PHOENIX	5.f

CItemsBonus::CItemsBonus(float posx,float posy)
: CItems(posx,posy,ETYPEITEMS::TYPEITEMS_BONUS)
{
	Init();
}

CItemsBonus::CItemsBonus():CItems(ETYPEITEMS::TYPEITEMS_BONUS)
{
	Init();
}

CItemsBonus::~CItemsBonus(void)
{
}

void CItemsBonus::Init()
{
	m_TimePhoenix=TIME_PHOENIX;
	m_bIsPhoenix=false;
}

void CItemsBonus::GeneratePosition()
{
	m_PosX=100+(rand()%(int)600);
	m_PosY=100+(rand()%(int) 500);
}

void CItemsBonus::Render(CDebugPrintText2D& printText2d)
{
	if (m_Visible)
	{
		printText2d.PrintText(m_PosX,m_PosY,0xffffffff,"B");
	}
}

void CItemsBonus::Update(float dt)
{
	if (!m_Visible)
	{
		if (!m_bIsPhoenix)
		{
			m_TimePhoenix=TIME_PHOENIX;
			m_bIsPhoenix=true;
		}
		else
		{
			m_TimePhoenix -= dt;
			if (m_TimePhoenix<=0)
			{
				GeneratePosition();
				m_Visible=true;
			}
		}
	}
}