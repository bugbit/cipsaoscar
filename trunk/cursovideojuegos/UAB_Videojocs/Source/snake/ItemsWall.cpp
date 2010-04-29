#include "ItemsWall.h"

#define ITEM_SIZE 10.f

CItemsWall::CItemsWall(float posx,float posy):CItems(posx,posy,ETYPEITEMS::TYPEITEMS_WALL)
{
}

CItemsWall::~CItemsWall(void)
{
}

void CItemsWall::Render(CDebugPrintText2D& printText2d)
{
	if (m_Visible)
	{
		printText2d.PrintText(m_PosX,m_PosY,0xffffffff,"*");
	}
}

void CItemsWall::Update(float dt)
{
}
