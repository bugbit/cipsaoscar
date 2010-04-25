#include "Items.h"

#define ITEM_SIZE 10.f

CItems::CItems(float posx,float posy,ETYPEITEMS type)
:m_PosX(posx)
,m_PosY(posy)
,m_Type(type)
,m_Visible(true)
{
}

CItems::~CItems(void)
{
}

void CItems::Render(CDebugPrintText2D& printText2d)
{
	if (m_Visible)
	{
		switch (m_Type)
		{
		case TYPEITEMS_BONUS:
			printText2d.PrintText(m_PosX,m_PosY,0xffffffff,"B");
			break;
		case TYPEITEMS_WALL:
			printText2d.PrintText(m_PosX,m_PosY,0xffffffff,"*");
			break;
		}
	}
}

bool CItems::IsCollision(float posx,float posy)
{
	if (!m_Visible)
	{
		return false;
	}
	float dx=posx-m_PosX;
	float dy=posy-m_PosY;
	return (dx>=0 && dx<=ITEM_SIZE) && (dy>=0 && dy<=ITEM_SIZE);
}
