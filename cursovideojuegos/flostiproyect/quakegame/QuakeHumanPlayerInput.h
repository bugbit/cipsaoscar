#pragma once
#include "quakeplayerinput.h"

class CQuakeHumanPlayerInput :
	public CQuakePlayerInput
{
public:
	CQuakeHumanPlayerInput(void);
	~CQuakeHumanPlayerInput(void);
	void UpdateInputAction(float elapsedTime);
};
