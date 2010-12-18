#pragma once

//--Forward Declaration--
class CPlayer;
class CRenderManager;
class CFontManager;

class CPlayerRender
{
public:
	inline CPlayerRender								(void):m_pPlayer(NULL)	{	}
	inline virtual			~CPlayerRender	(void)	{	}
	virtual void				RenderScene			(CRenderManager* renderManager, CFontManager* fontManager)=0;
	inline void					SetPlayer				(CPlayer * player) {m_pPlayer = player;}
protected:
	CPlayer*												m_pPlayer;	
};
