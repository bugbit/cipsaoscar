#pragma once
#include "guiplayerobjectrender.h"

//---Engine Includes--------
#include "Graphics/ASEObject/ASEObject.h"


class CGUIPlayerObjectASERender :
	public CGUIPlayerObjectRender
{
public:
	CGUIPlayerObjectASERender														(void);
	virtual ~CGUIPlayerObjectASERender									(void);

	virtual	void											RenderScene				(CRenderManager* renderManager, CFontManager* fm,CCamera *camera,CGUIPlayer &guiplayer);

	void															LoadObjectASE			(std::string filease,std::string pathTextures);
protected:
	CASEObject																					m_ObjectASE;
	D3DXMATRIX																					m_Matrix;
};
