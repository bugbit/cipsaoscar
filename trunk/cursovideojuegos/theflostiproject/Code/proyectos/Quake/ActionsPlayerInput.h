#pragma once
#include "playerinput.h"

class CActionsPlayerInput :
	public CPlayerInput
{
public:
	CActionsPlayerInput(void);
	~CActionsPlayerInput(void);
	virtual void UpdateInputAction	(float elapsedTime);
};
