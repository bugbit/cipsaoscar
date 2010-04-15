/* 
 * File:   CIAPlayer.cpp
 * Author: oscar
 * 
 * Created on 5 de abril de 2010, 21:06
 */

#include "CIAPlayer.h"
#include "cboard.h"

#include <time.h>
#include <iostream>

using namespace std;


CIAPlayer::CIAPlayer(CBoard::EFicha type,string name,int nivel):CPlayer(type,name),m_Nivel(nivel)
{
}

CIAPlayer::~CIAPlayer()
{
}

void CIAPlayer::update(CBoard *board)
{
    int x,y;
    switch (m_Nivel)
    {
        case 1:
        default:
            update_nivel1(board,&x,&y);
    }
    board->set(m_Type,x,y);
    cout << m_Name << " ha pensado en la posicion x:" << x << ", y:" << y << endl;
    clock_t tiempo=clock()+CLOCKS_PER_SEC;
    while (clock()<tiempo);
}

void CIAPlayer::update_nivel1(CBoard *board,int *xs,int *ys)
{
    int xdim,ydim;

    board->getDimension(&xdim,&ydim);
    for (int y=1;y<=ydim;y++)
        for (int x=1;x<=xdim;x++)
            if (board->get(x,y)==CBoard::NOTHING)
            {
                *xs=x;
                *ys=y;
                return;
            }
}