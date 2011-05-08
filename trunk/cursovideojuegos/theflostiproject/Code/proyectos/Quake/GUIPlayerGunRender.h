#pragma once
#include "guiplayerobjectaserender.h"

class CGUIPlayerGunRender :
	public CGUIPlayerObjectASERender
{
public:
	CGUIPlayerGunRender																(bool bAnimatedShot,bool bAnimatedSetBack);
	virtual ~CGUIPlayerGunRender											(void);

	inline int GetAnimatedShot												() const { return m_bAnimatedShot; }
	inline int GetAnimatedSetBack											() const { return m_bAnimatedSetBack; }

	virtual	void											RenderScene				(CRenderManager* renderManager, CFontManager* fm, CCamera *camera,CGUIPlayer &guiplayer);
protected:
	bool																								m_bAnimatedShot;
	bool																								m_bAnimatedSetBack;
};
