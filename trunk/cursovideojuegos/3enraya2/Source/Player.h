/* 
 * File:   CPlayer.h
 * Author: oscar
 *
 * Created on 5 de abril de 2010, 19:25
 */

#ifndef _CPLAYER_H
#define	_CPLAYER_H

#include "Board.h" // class's header file

#include <string>

using namespace std;

class CPlayer {
public:
    CPlayer(CBoard::EFicha type,string name);
    virtual ~CPlayer();
    virtual void update(CBoard &board,float dt)=0;
    inline string getName() const { return m_Name; }
    inline unsigned int getScore() const { return m_Score; }
    void winer();
	inline bool isChangePlayer() const { return m_isChangePlayer; }
	inline const char *getCoordStr() const { return m_CoordStr; }
protected:
    CBoard::EFicha m_Type;
    string m_Name;
    unsigned	int m_Score;
	bool		m_isChangePlayer;
	char		m_CoordStr[3];
	float		m_Time;
};

#endif	/* _CPLAYER_H */

