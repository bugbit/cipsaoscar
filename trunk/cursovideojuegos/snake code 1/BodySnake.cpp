#include "BodySnake.h"

CBodySnake::CBodySnake(float posx,float posy):m_PosX(posx),m_PosY(posy)
{
}

CBodySnake::~CBodySnake(void)
{
}

void CBodySnake::Set(CBodySnake &body)
{
	Set(body.GetPosX(),body.GetPosY());
}

void CBodySnake::Set(float posx,float posy)
{
	m_PosX=posx;
	m_PosY=posy;
}