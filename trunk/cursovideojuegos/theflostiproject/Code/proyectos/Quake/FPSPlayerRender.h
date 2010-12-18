#pragma once
#include "playerrender.h"

//---Engine Includes--------
#include "Graphics/ASEObject/ASEObject.h"

class CFPSPlayerRender :
	public CPlayerRender
{
public:
	CFPSPlayerRender											(void);
	virtual							~CFPSPlayerRender	(void);
	virtual void				RenderScene				(CRenderManager* renderManager, CFontManager* fontManager);

	void								LoadFaceASE(std::string filease,std::string pathTextures);
private:
	CASEObject														m_FaceASE;
};
