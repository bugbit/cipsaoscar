#pragma once
#include "itemgun.h"

class CItemAmmo :
	public CItemGun
{
public:
	CItemAmmo												(CItem::ETYTE type,int gunstate=20);
	virtual ~CItemAmmo							(void);
	inline int					GetGunState	() const { return m_iGunState; }
protected:
	int															m_iGunState;
};
