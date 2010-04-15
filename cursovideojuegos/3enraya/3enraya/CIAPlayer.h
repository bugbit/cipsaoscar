/* 
 * File:   CIAPlayer.h
 * Author: oscar
 *
 * Created on 5 de abril de 2010, 21:06
 */

#ifndef _CIAPLAYER_H
#define	_CIAPLAYER_H

#include "CPlayer.h"

class CIAPlayer : public CPlayer {
public:
    CIAPlayer(CBoard::EFicha type,string name,int nivel);
    virtual ~CIAPlayer();
    void update(CBoard *board);
private:
    int m_Nivel;
    void update_nivel1(CBoard *board,int *xs,int *ys);
};

#endif	/* _CIAPLAYER_H */

