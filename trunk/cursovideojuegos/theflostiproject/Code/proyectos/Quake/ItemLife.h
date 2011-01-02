#pragma once
#include "item.h"

class CItemLife :
	public CItem
{
public:
	CItemLife(void);
	virtual ~CItemLife(void);
protected:
	virtual	bool							AccionSelected	(CObject3D &object3D);
};
