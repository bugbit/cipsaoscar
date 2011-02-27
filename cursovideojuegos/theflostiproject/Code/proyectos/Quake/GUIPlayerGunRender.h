#pragma once
#include "guiplayerobjectaserender.h"

class CGUIPlayerGunRender :
	public CGUIPlayerObjectASERender
{
public:
	CGUIPlayerGunRender(bool bAnimatedShot,bool bAnimatedSetBack);
	virtual ~CGUIPlayerGunRender(void);

	virtual	void											RenderScene				(CRenderManager* renderManager, CFontManager* fm, CCamera *camera,CGUIPlayer &guiplayer);
protected:
	bool																								m_bAnimatedShot;
	bool																								m_bAnimatedSetBack;
};
