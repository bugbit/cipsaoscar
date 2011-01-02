#pragma once

#include "itemmodel.h"

//---Engine Includes--------
//#include "Core/Core.h"
#include "Graphics/ASEObject/ASEObject.h"
//#include "Graphics/ASEObject/ASETextureManager.h"

class CItemModelASE :	public CItemModel
{
public:
	CItemModelASE																	(void);
	virtual ~CItemModelASE												(void);
	void														LoadModelASE	(std::string filease,std::string pathTextures);
	virtual void										RenderModel		(CRenderManager* renderManager, CFontManager* fontManager);
private:
	CASEObject											m_ModelASE;
};
