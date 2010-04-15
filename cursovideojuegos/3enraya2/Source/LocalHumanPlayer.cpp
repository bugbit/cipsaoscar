/* 
 * File:   CLocalHumanPlayer.cpp
 * Author: oscar
 * 
 * Created on 5 de abril de 2010, 19:31
 */

#include "LocalHumanPlayer.h"

#include "InputManager.h"

CLocalHumanPlayer::CLocalHumanPlayer(CBoard::EFicha type,string name):CPlayer(type,name),m_CoordAsk(CBoard::ECOORD::Y)
{
}

CLocalHumanPlayer::~CLocalHumanPlayer()
{
}

void CLocalHumanPlayer::update(CBoard &board,float dt)
{
	m_Time +=dt;
	if (m_Time>40000) m_Time = 0.f;
	if (m_Time>0.f)
	{
		char car='\x0';
		CInputManager * input = CInputManager::GetInstance();
		int coord;
		switch (m_CoordAsk)
		{
			case CBoard::ECOORD::Y:
				m_isChangePlayer=false;
				if (input->DoAction("teclaA"))
				{
					car='A';					
					m_Time = -0.5;
				}
				else if (input->DoAction("teclaB"))
				{
					car='B';					
					m_Time = -0.5;
				}
				else if (input->DoAction("teclaC"))
				{
					car='C';					
					m_Time = -0.5;
				}
				if (car!='\x0')
				{
					coord=board.CarToCoord(car,m_CoordAsk);
					m_Y=coord;
					board.getCoordStr(m_CoordStr+(int) m_CoordAsk,coord,m_CoordAsk);						
					m_CoordAsk=CBoard::ECOORD::X;
				}
				break;
			case CBoard::ECOORD::X:
				if (input->DoAction("tecla1"))
				{
					car='1';					
					m_Time = -0.5;
				}
				else if (input->DoAction("tecla2"))
				{
					car='2';					
					m_Time = -0.5;
				}
				else if (input->DoAction("tecla3"))
				{
					car='3';					
					m_Time = -0.5;
				}
				if (car!='\x0')
				{
					coord=board.CarToCoord(car,m_CoordAsk);
					m_X=coord;
					board.getCoordStr(m_CoordStr+(int) m_CoordAsk,coord,m_CoordAsk);
					m_CoordAsk=CBoard::ECOORD::END;
					m_Time=0;
				}				
				break;
			case CBoard::ECOORD::END:
				if (m_Time>1)
				{
					*m_CoordStr=0;
					board.set(m_Type,m_X,m_Y);
					board.Check();
					m_CoordAsk=CBoard::ECOORD::Y;
					m_isChangePlayer=true;
				}
				break;
		}
	}
	/*
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
   **/
}