#include "TresEnRaya.h"

#include "InputManager.h"

CTresEnRaya::CTresEnRaya()
{
}

CTresEnRaya::~CTresEnRaya()
{

}

void CTresEnRaya::Render		(CDebugPrintText2D& printText2d)
{
	int dy;
	CInputManager * input = CInputManager::GetInstance();
		
	if (input->DoAction("changeControl"))
	{
		dy=printText2d.PrintText(400,400,0xffffffff,"XXXXX");	
	}
	//dy=printText2d.PrintText(400,400,0xffffffff,"XXXXX");	
}

void CTresEnRaya::Update		(float dt)
{
	
}