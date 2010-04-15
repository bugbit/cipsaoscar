/* 
 * File:   CLocalHumanPlayer.h
 * Author: oscar
 *
 * Created on 5 de abril de 2010, 19:31
 */

#ifndef _CLOCALHUMANPLAYER_H
#define	_CLOCALHUMANPLAYER_H

#include "Player.h"

class CLocalHumanPlayer : public CPlayer
{
public:
    CLocalHumanPlayer(CBoard::EFicha type,string name);
    virtual ~CLocalHumanPlayer();
    void update(CBoard &board,float dt);
private:
	CBoard::ECOORD	m_CoordAsk;
	int m_X,m_Y;
};

#endif	/* _CLOCALHUMANPLAYER_H */

