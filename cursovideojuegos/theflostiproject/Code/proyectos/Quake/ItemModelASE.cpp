#include "__PCH_Quake.h"

#include "ItemModelASE.h"

//---Engine Includes--------
#include "Core/Core.h"
#include "Graphics/ASEObject/ASETextureManager.h"
#include "Graphics/RenderManager.h"

CItemModelASE::CItemModelASE(void)
{
}

CItemModelASE::~CItemModelASE(void)
{
	m_ModelASE.CleanUp();
}

void CItemModelASE::LoadModelASE(std::string filease, std::string pathTextures)
{
	CASETextureManager::GetInstance()->SetTexturePath(pathTextures);
	CRenderManager* rm = CORE->GetRenderManager();
	m_ModelASE.Load(filease.c_str(),rm);
}

void CItemModelASE::RenderModel(CRenderManager *renderManager, CFontManager *fontManager)
{
	m_ModelASE.Render(renderManager);
}
