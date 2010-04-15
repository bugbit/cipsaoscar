/* 
 * File:   CLocalHumanPlayer.h
 * Author: oscar
 *
 * Created on 5 de abril de 2010, 19:31
 */

#ifndef _CLOCALHUMANPLAYER_H
#define	_CLOCALHUMANPLAYER_H

#include "CPlayer.h"

class CLocalHumanPlayer : public CPlayer
{
public:
    CLocalHumanPlayer(CBoard::EFicha type,string name);
    virtual ~CLocalHumanPlayer();
    void update(CBoard *board);
private:

};

#endif	/* _CLOCALHUMANPLAYER_H */

