#include "Items.h"

#define ITEM_SIZE 10.f

CItems::CItems(float posx,float posy,ETYPEITEMS type)
:m_PosX(posx)
,m_PosY(posy)
,m_Type(type)
{
	Init();
}

CItems::CItems(ETYPEITEMS type)
:m_PosX(0)
,m_PosY(0)
,m_Type(type)
{
	Init();
}

CItems::~CItems(void)
{
}

void CItems::Init()
{
	m_Visible=true;
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
