#include "__PCH_Quake.h"

#include <assert.h>

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
#include "Graphics/Textures/Texture.h"
#include "Logger/Logger.h"
#include "xml/XMLTreeNode.h"

const int CGUIPlayer::m_iWidthNumber=40;

CGUIPlayer::CGUIPlayer(void)
:m_bIsOk(false)
,m_yawGun(0)
{
	for(int i=0;i<RangTexturesNumbers();i++)
		m_TexturesNumbers[i]=NULL;
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

void CGUIPlayer::Release()
{
	m_FaceASE.CleanUp();
	for(int i=0;i<RangTexturesNumbers();i++)
	{
		CTexture *texture=m_TexturesNumbers[i];
		if (texture!=NULL)
		{
			delete texture;
			m_TexturesNumbers[i]=NULL;
		}
	}
}

bool CGUIPlayer::Init()
{
	m_sFileXML="";
	m_bIsOk=true;

	return m_bIsOk;
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

bool CGUIPlayer::LoadXML(std::string filexml)
{
	m_sFileXML=filexml;
	LOGGER->AddNewLog(ELL_INFORMATION, "CGUIPlayer::LoadXML: Iniciando el parseo del fichero %s", filexml.c_str());
	CXMLTreeNode newFile;
	m_bIsOk = false;
	if (!newFile.LoadFile((filexml).c_str()))
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIPlayer::LoadXML: No se ha podido leer correctamente el fichero ->%s", filexml.c_str());
		m_bIsOk = false;
	}
	else
	{
		CXMLTreeNode guiplayer = newFile["guiplayer"];
		CXMLTreeNode face=guiplayer["face"];
		if (!face.Exists())
		{
			LOGGER->AddNewLog(ELL_ERROR, "CGUIPlayer::LoadXML->Error al intentar leer el tag <face> del fichero ->%s", filexml.c_str());
			return false;
		}
		std::string model=face.GetPszProperty("model");
		std::string texture=face.GetPszProperty("texture");
		LoadFaceASE(model,texture);
		LOGGER->AddNewLog(ELL_INFORMATION, "CGUIPlayer::LoadXML: Cargado face, model : %s, pathtextures : %s",model.c_str(),texture.c_str());
		m_bIsOk=true;
		CXMLTreeNode numbers=guiplayer["numbers"];
		int count=numbers.GetNumChildren();
		for (int i=0;i<count;i++)
		{
			CXMLTreeNode number=numbers(i);
			int id=number.GetIntProperty("id");
			texture=number.GetPszProperty("texture");
			LoadTextureNumber(id,texture);
			LOGGER->AddNewLog(ELL_INFORMATION, "CGUIPlayer::LoadXML: Number %d texture: %s", id,texture.c_str());
		}
	}

	return m_bIsOk;
}

bool CGUIPlayer::ReloadXML()
{
	Done();
	Init();
	if (m_sFileXML!="")
		return LoadXML(m_sFileXML);

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
	/*int mulU=7;
	int mulD=8;
	int mulC=9;
	Vect2i pos((int) ((float)(w)*0.5f -20 -40*mulU), (h)-60-20);
	Vect2i pos2((int) ((float)(w)*0.5f -20 -40*mulD), (h)-60-20);
	Vect2i pos3((int) ((float)(w)*0.5f -20 -40*mulC), (h)-60-20);
	if (m_TexturesNumbers[0]!=NULL)
	{
		renderManager->DrawQuad2D(pos,32,32,UPPER_LEFT,m_TexturesNumbers[0]);	
		renderManager->DrawQuad2D(pos2,32,32,UPPER_LEFT,m_TexturesNumbers[0]);
		renderManager->DrawQuad2D(pos3,32,32,UPPER_LEFT,m_TexturesNumbers[0]);
	}*/
	RenderContador2D(renderManager,fm,(int) ((float)(w)*0.5f -20 -m_iWidthNumber*9),(h)-60-20,m_pPlayer->GetStatusPlayer(),100);
}

void CGUIPlayer::RenderContador2D	(CRenderManager* renderManager, CFontManager* fm,int x,int y,int contador,int maxbasecontador)
{
	assert(maxbasecontador % 10 ==0);
	// maxbasecontador = 100, contador <1000
	assert(contador<10*maxbasecontador);
	Vect2i pos(x,y);
	for (;maxbasecontador>=10;maxbasecontador /= 10,pos.x += m_iWidthNumber)
	{
		/*
				253

				contador					x1					x2												maxbasecontador
					253						253/100=2		253 % 100 =53											100
					 53						 53/10=5			53 % 10 =3											 10
					  3							3/1=3					3 %  1=0												1

					203						203/100=2		203 % 100 =3											100
					  3																													 10
						3							3/1=3				3 % 1 =0													1
		*/
		if (contador>=maxbasecontador)
		{
			int digito=contador/maxbasecontador;
			renderManager->DrawQuad2D(pos,m_iWidthNumber,32,UPPER_LEFT,m_TexturesNumbers[digito]);	
			contador %= maxbasecontador;
		}
	}
	if (contador>=0)
		renderManager->DrawQuad2D(pos,m_iWidthNumber,32,UPPER_LEFT,m_TexturesNumbers[contador]);	
}

void CGUIPlayer::Update(float elapsedTime)
{
	m_yawGun += elapsedTime;
}
