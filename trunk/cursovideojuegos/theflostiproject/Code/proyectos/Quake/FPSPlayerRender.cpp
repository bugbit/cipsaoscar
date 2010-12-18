#include "__PCH_Quake.h"

#include "FPSPlayerRender.h"
#include "Player.h"

//---Engine Includes----
#include "Core/Core.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/ASEObject/ASETextureManager.h"
#include "Math/Matrix44.h"

CFPSPlayerRender::CFPSPlayerRender(void)
{
	m_FaceASE.CleanUp();
}

CFPSPlayerRender::~CFPSPlayerRender(void)
{
}

void CFPSPlayerRender::RenderScene(CRenderManager* renderManager, CFontManager* fontManager)
{
	Mat44f matp,matr,rotpic,rotyaw;

	matp.SetIdentity();
	matp.Translate(m_pPlayer->GetPosition());
	matr.SetIdentity();
	matr.Translate(Vect3f( 0.6f, -0.24f , 0.2f));
	rotpic.SetIdentity();
	rotpic.RotByAngleZ(m_pPlayer->GetPitch());
	rotyaw.SetIdentity();
	rotyaw.RotByAngleY(-m_pPlayer->GetYaw());
	renderManager->SetTransform(matr*rotpic*rotyaw*matp);
	m_FaceASE.Render(renderManager);
}

void  CFPSPlayerRender::LoadFaceASE(std::string filease,std::string pathTextures)
{
	CASETextureManager::GetInstance()->SetTexturePath(pathTextures);
	CRenderManager* rm = CORE->GetRenderManager();
	m_FaceASE.Load(filease.c_str(),rm);
}
