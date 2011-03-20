#include "__PCH_Quake.h"

#include "ItemAmmo.h"

CItemAmmo::CItemAmmo(CItem::ETYTE type,int gunstate):CItemGun(type)
,m_iGunState(gunstate)
{
}

CItemAmmo::~CItemAmmo(void)
{
}
