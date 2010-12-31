#pragma once

#include "Player.h"

//--Forward Declaration--
class CRenderManager;
class CFontManager;

class CPlayerRender
{
public:
	inline CPlayerRender								(void):m_pPlayer(NULL)	{	}
	inline virtual			~CPlayerRender	(void)	{	}
	virtual void				RenderScene			(CRenderManager* renderManager, CFontManager* fontManager)=0;
	inline void					SetPlayer				(CPlayer * player) {m_pPlayer = player;}
	inline bool					IsOk	          () const { return !m_pPlayer==NULL || m_pPlayer->IsOk(); }
protected:
	CPlayer*												m_pPlayer;	
};
