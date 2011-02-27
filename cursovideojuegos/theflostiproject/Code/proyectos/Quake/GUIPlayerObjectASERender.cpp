#include "__PCH_Quake.h"

#include "GUIPlayerObjectASERender.h"

//---Engine Includes----
#include "Core/Core.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/ASEObject/ASETextureManager.h"
#include "Graphics/Textures/TextureManager.h"
#include "Graphics/Textures/Texture.h"

CGUIPlayerObjectASERender::CGUIPlayerObjectASERender(void)
{
}

CGUIPlayerObjectASERender::~CGUIPlayerObjectASERender(void)
{
	m_ObjectASE.CleanUp();
}

void CGUIPlayerObjectASERender::RenderScene(CRenderManager *renderManager, CFontManager *fm, CCamera *camera, CGUIPlayer &guiplayer)
{
	renderManager->SetTransform(m_Matrix);
	m_ObjectASE.Render(renderManager);
}

void CGUIPlayerObjectASERender::LoadObjectASE(std::string filease,std::string pathTextures)
{
	CASETextureManager::GetInstance()->SetTexturePath(pathTextures);
	CRenderManager* rm = CORE->GetRenderManager();
	m_ObjectASE.Load(filease.c_str(),rm);
}

