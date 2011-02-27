#pragma once
#include "GUIPlayerObjectASERender.h"

class CGUIPlayerFaceRender :
	public CGUIPlayerObjectASERender
{
public:
	CGUIPlayerFaceRender																(void);
	virtual ~CGUIPlayerFaceRender												(void);

	virtual	void											RenderScene				(CRenderManager* renderManager, CFontManager* fm, CCamera *camera,CGUIPlayer &guiplayer);
};
