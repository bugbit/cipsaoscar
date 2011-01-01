#include "__PCH_Quake.h"

#include "Item.h"

CItem::CItem()
:m_pPhysxData(NULL)
,m_pActor(NULL)
,m_fTimer(NULL)
,m_bSelected(false)
{
}

CItem::~CItem()
{
}
