#pragma once
#include "guiplayerobjectaserender.h"

class CGUIPlayerObjectOfCounterRender :
	public CGUIPlayerObjectASERender
{
public:
	CGUIPlayerObjectOfCounterRender										();
	virtual ~CGUIPlayerObjectOfCounterRender					(void);

virtual	void											RenderScene				(CRenderManager* renderManager, CFontManager* fm, CCamera *camera,CGUIPlayer &guiplayer);
protected:
	D3DXMATRIX																				mTranslation;
};
