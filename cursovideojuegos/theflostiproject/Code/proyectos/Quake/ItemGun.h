#pragma once
#include "item.h"

class CItemGun :
	public CItem
{
public:
	CItemGun																(void);
	virtual ~CItemGun												(void);
	inline int								GetGunState		() const { return m_iGunState; }
	inline void								SetGunState		(int gunstate) { m_iGunState=gunstate; }
protected:
	int												m_iGunState;
};
