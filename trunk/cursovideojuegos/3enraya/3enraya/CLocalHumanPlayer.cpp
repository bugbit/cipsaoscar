/* 
 * File:   CLocalHumanPlayer.cpp
 * Author: oscar
 * 
 * Created on 5 de abril de 2010, 19:31
 */

#include "CLocalHumanPlayer.h"

#include <iostream>

using namespace std;

CLocalHumanPlayer::CLocalHumanPlayer(CBoard::EFicha type,string name):CPlayer(type,name)
{
}

CLocalHumanPlayer::~CLocalHumanPlayer()
{
}

void CLocalHumanPlayer::update(CBoard *board)
{
    bool p_Finish=false;
    do
    {
        cout << m_Name << " : " << endl;
        int x,y;
        cout << "Ponga posicion y : ";
        cin >> y;
        cout << "Ponga posicion x : ";
        cin >> x;
        try
        {
            board->set(m_Type,x,y);
            p_Finish=true;
        }
        catch (string e)
        {
            cout << e << endl;
        }
    } while (!p_Finish);
}