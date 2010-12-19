#pragma once
#include "playerrender.h"

//--Forward Declaration--
class CCamera;

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
	inline void					SetCamera(CCamera * camera) { m_pCamera=camera; }
private:
	CASEObject														m_FaceASE;
	CCamera																*m_pCamera;
};
