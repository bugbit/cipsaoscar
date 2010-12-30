#include "__PCH_Quake.h"

#include "GUIPlayer.h"
#include "Player.h"

//---Engine Includes----
#include "Core/Core.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/ASEObject/ASETextureManager.h"
#include "Math/Matrix44.h"
#include "Graphics/Camera.h"
#include "Graphics/Textures/TextureManager.h"

CGUIPlayer::CGUIPlayer(void)
:m_bIsOk(false)
,m_yawGun(0)
{
}

CGUIPlayer::~CGUIPlayer(void)
{
	Done();
}

void CGUIPlayer::Done()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}


bool CGUIPlayer::Init()
{
	m_bIsOk=true;

	return m_bIsOk;
}

void CGUIPlayer::Release()
{
	m_FaceASE.CleanUp();
}

void CGUIPlayer::LoadFaceASE(std::string filease,std::string pathTextures)
{
	CASETextureManager::GetInstance()->SetTexturePath(pathTextures);
	CRenderManager* rm = CORE->GetRenderManager();
	m_FaceASE.Load(filease.c_str(),rm);
}

void CGUIPlayer::LoadTextureNumber(int i,std::string filetexture)
{
	m_TexturesNumbers[i]=CORE->GetTextureManager()->GetTexture(filetexture);
}

void CGUIPlayer::LoadGUIPlayer(std::string filexml)
{
}

bool CGUIPlayer::ReloadGUIPlayer()
{
	Done();
	Init();
	if (m_sFileXML!="")
		LoadGUIPlayer(m_sFileXML);

	return true;
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

void CGUIPlayer::RenderScene(CRenderManager* renderManager, CFontManager* fontManager, CCamera *camera)
{
	D3DXMATRIX matrix;
	D3DXMATRIX translation;
	D3DXMATRIX translation2;
	D3DXMATRIX rotation0;
	D3DXMATRIX rotation;
	D3DXMATRIX rotation2;
	Vect3f position = camera->GetObject3D()->GetPosition();
	float yaw =  camera->GetObject3D()->GetYaw();
	float pitch =  camera->GetObject3D()->GetPitch();

	D3DXMatrixRotationZ ( &rotation2,  pitch);

	//--------------Paint the FACE------------------------------
	D3DXMatrixTranslation( &translation, position.x,position.y ,position.z);
	D3DXMatrixTranslation( &translation2, 0.6f, -0.28f , 0.25f);
	
	D3DXMatrixRotationY ( &rotation,  -yaw);
	D3DXMatrixRotationY ( &rotation0,  ePIf+sin( -m_yawGun )*0.1f);
	
	matrix = rotation0 * translation2 * rotation2 * rotation * translation; 

	renderManager->SetTransform(matrix);
	m_FaceASE.Render(renderManager);
}

void CGUIPlayer::RenderScene2D(CRenderManager* renderManager, CFontManager* fm)
{
	uint32 w,h;
	renderManager->GetWidthAndHeight(w,h);
	int mulU=7;
	int mulD=8;
	int mulC=9;
	Vect2i pos((int) ((float)(w)*0.5f -20 -40*mulU), (h)-60-20);
	Vect2i pos2((int) ((float)(w)*0.5f -20 -40*mulD), (h)-60-20);
	Vect2i pos3((int) ((float)(w)*0.5f -20 -40*mulC), (h)-60-20);
	renderManager->DrawQuad2D(pos,32,32,UPPER_LEFT,m_TexturesNumbers[0]);	
	renderManager->DrawQuad2D(pos2,32,32,UPPER_LEFT,m_TexturesNumbers[0]);
	renderManager->DrawQuad2D(pos3,32,32,UPPER_LEFT,m_TexturesNumbers[0]);
}

void CGUIPlayer::RenderContador2D	(CRenderManager* renderManager, CFontManager* fm,int x,int y,int contador,int maxbasecontador)
{
}

void CGUIPlayer::Update(float elapsedTime)
{
	m_yawGun += elapsedTime;
}
