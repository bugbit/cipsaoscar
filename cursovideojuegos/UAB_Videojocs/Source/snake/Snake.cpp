#include "Snake.h"


CSnake::CSnake()
{

}

CSnake::~CSnake()
{	

}

void CSnake::Render		(CDebugPrintText2D& printText2d)
{
	int dy = 400;
	dy += printText2d.PrintText(400,dy,0xffffffff,"Soy Snake");	
}

void CSnake::Update		(float dt)
{

}