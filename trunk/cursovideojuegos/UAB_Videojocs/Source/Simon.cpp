#include "Simon.h"
#include "InputManager.h"


CSimon::CSimon()
{

}

CSimon::~CSimon()
{	

}

void CSimon::Render		(CDebugPrintText2D& printText2d)
{
	int dy = 400;
	dy += printText2d.PrintText(400,dy,0xffffffff,"Soy Simon");	
}

void CSimon::Update		(float dt)
{
	
}