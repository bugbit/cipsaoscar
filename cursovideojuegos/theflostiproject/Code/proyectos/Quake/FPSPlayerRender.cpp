#include "__PCH_Quake.h"

#include "FPSPlayerRender.h"
#include "Player.h"

//---Engine Includes----
#include "Core/Core.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/ASEObject/ASETextureManager.h"
#include "Math/Matrix44.h"
#include "Graphics/Camera.h"

CFPSPlayerRender::CFPSPlayerRender(void)
{
	m_FaceASE.CleanUp();
}

CFPSPlayerRender::~CFPSPlayerRender(void)
{
}

/*
		A	/------------------\	B
			|              /   |
			|       POS /      |
			|      / *         |
			| /                |
		C	\------------------/	D*/
	//Vect3f vup=camera->GetVecUp();
	//Vect3f dir=camera->GetDirection();
	///*dir.Normalize();
	//Vect3f right=vup ^ dir;
	//Vect3f up=dir ^ right;
	//Vect3f lookAt=camera->GetEye();*/
	//Vect3f up=camera->GetVecUp();
	//float yaw  = camera->GetObject3D()->GetYaw();
	//Vect3f right=Vect3f(cos(yaw+ D3DX_PI * 0.5f), 0, sin(yaw+ D3DX_PI * 0.5f));
	//Vect3f lookAt=camera->GetLookAt();
	//right.Normalize();
	//up.Normalize();
	//float SizeX=1.6f;
	//float SizeY=1.1f;
	//Vect3f PointA = lookAt - (right*SizeX*0.5f) - (up*SizeY*0.5f);
	//Vect3f PointB = lookAt + (right*SizeX*0.5f) - (up*SizeY*0.5f);
	//Vect3f PointC = lookAt - (right*SizeX*0.5f) + (up*SizeY*0.5f);
	//Vect3f PointD = lookAt + (right*SizeX*0.5f) + (up*SizeY*0.5f);
	//renderManager->SetTransform((Mat44f) m44fIDENTITY);
	//renderManager->DrawLine(PointA,PointB,colGREEN);
	//renderManager->DrawLine(PointA,PointC,colGREEN);
	//renderManager->DrawLine(PointC,PointD,colGREEN);
	//renderManager->DrawLine(PointD,PointB,colGREEN);

void CFPSPlayerRender::RenderScene(CRenderManager* renderManager, CFontManager* fontManager)
{
	Mat44f mat;
	Mat44f rotYaw;
	Mat44f rotPitch;

	Vect3f vup=m_pCamera->GetVecUp();
	Vect3f dir=m_pCamera->GetDirection();
	Vect3f up=m_pCamera->GetVecUp();
	float yaw  = m_pCamera->GetObject3D()->GetYaw();
	Vect3f right=Vect3f(cos(yaw+ D3DX_PI * 0.5f), 0, sin(yaw+ D3DX_PI * 0.5f));
	Vect3f lookAt=m_pCamera->GetLookAt();
	right.Normalize();
	up.Normalize();
	rotPitch.SetIdentity();
	rotPitch.RotByAngleX(m_pCamera->GetObject3D()->GetPitch());
	rotYaw.SetIdentity();
	rotYaw.RotByAngleY(ePIf-yaw);
	mat.SetIdentity();
	// Punto cerca de C
	mat.Translate(lookAt + (right*1.f*0.5f) - (up*0.8f*0.5f));
	renderManager->SetTransform(mat*rotPitch*rotYaw);
	m_FaceASE.Render(renderManager);
}

void  CFPSPlayerRender::LoadFaceASE(std::string filease,std::string pathTextures)
{
	CASETextureManager::GetInstance()->SetTexturePath(pathTextures);
	CRenderManager* rm = CORE->GetRenderManager();
	m_FaceASE.Load(filease.c_str(),rm);
}
