#pragma once

#include "GUIPlayer.h"

//--Forward Declaration--
class CRenderManager;
class CFontManager;
class CCamera;
class CGUIPlayer;

class CGUIPlayerObjectRender
{
public:
	CGUIPlayerObjectRender															(void);
	virtual ~CGUIPlayerObjectRender											(void);

	virtual	void											RenderScene				(CRenderManager* renderManager, CFontManager* fm, CCamera *camera,CGUIPlayer &guiplayer)=0;
};
