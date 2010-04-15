/* 
 * File:   CPlayer.cpp
 * Author: oscar
 * 
 * Created on 5 de abril de 2010, 19:25
 */

#include "CPlayer.h"

CPlayer::CPlayer(CBoard::EFicha type,string name):m_Type(type),m_Name(name),m_Score(0)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::winer()
{
    ++m_Score;
}

