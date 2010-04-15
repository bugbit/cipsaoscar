/* 
 * File:   CPlayer.cpp
 * Author: oscar
 * 
 * Created on 5 de abril de 2010, 19:25
 */

#include "Player.h"

CPlayer::CPlayer(CBoard::EFicha type,string name):m_Type(type),m_Name(name),m_Score(0),m_isChangePlayer(false),m_Time(0.f)
{
	*m_CoordStr=0;
}

CPlayer::~CPlayer()
{
}

void CPlayer::winer()
{
    ++m_Score;
}

