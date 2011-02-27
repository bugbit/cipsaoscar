#include "__PCH_Quake.h"

#include <assert.h>

#include "GUIPlayer.h"
#include "Player.h"
#include "GUIPlayerFaceRender.h"

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
,m_fYawGun(0)
,m_fPitchGun(0)
,m_pFaceRender(NULL)
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
	CHECKED_DELETE(m_pFaceRender);
	for(int i=0;i<RangTexturesNumbers();i++)
	{
		CTexture *texture=m_TexturesNumbers[i];
		if (texture!=NULL)
		{
			delete texture;
			m_TexturesNumbers[i]=NULL;
		}
	}
	ReleaseGUN();
}

void CGUIPlayer::ReleaseGUN()
{
	std::map<CItem::ETYTE,CGUIPlayerObjectRender *>::iterator it=m_GunsRender.begin(),itend=m_GunsRender.end();

	for (;it!=itend;it++)
	{
		CGUIPlayerObjectRender *render=(*it).second;
		CHECKED_DELETE(render);
	}
	m_GunsRender.clear();
}

bool CGUIPlayer::Init()
{
	m_sFileXML="";
	m_bIsOk=true;

	return m_bIsOk;
}

void CGUIPlayer::LoadFaceASE(std::string filease,std::string pathTextures)
{
	CGUIPlayerFaceRender *pFaceRender=new CGUIPlayerFaceRender();

	pFaceRender->LoadObjectASE(filease,pathTextures);
	m_pFaceRender=pFaceRender;
}

void CGUIPlayer::LoadGUNNode(CXMLTreeNode &node,CItem::ETYTE type,CGUIPlayerObjectASERender *render)
{
	std::string model=node.GetPszProperty("model");
	std::string texture=node.GetPszProperty("texture");
	render->LoadObjectASE(model,texture);
	LoadGUNNode(type,(CGUIPlayerObjectRender*) render);
	LOGGER->AddNewLog(ELL_INFORMATION, "CGUIPlayer::LoadGUNNode: Cargado model : %s, pathtextures : %s",model.c_str(),texture.c_str());
}

void CGUIPlayer::LoadGUNNode(CItem::ETYTE type,CGUIPlayerObjectRender *render)
{
	m_GunsRender[type]=render;
}

void CGUIPlayer::LoadTextureNumber(int i,std::string filetexture)
{
	m_TexturesNumbers[i]=CORE->GetTextureManager()->GetTexture(filetexture);
}

bool CGUIPlayer::LoadXML(std::string filexml)
{
	Release();
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
		LoadGUNNode(guiplayer,CItem::SHOTGUN);
		LoadGUNNode(guiplayer,CItem::ROCKETL);
		LoadGUNNode(guiplayer,CItem::MACHINEGUN);
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

void CGUIPlayer::LoadGUNNode(CXMLTreeNode &node,CItem::ETYTE type)
{
	std::string name=CItemTypeManager::GetInstance().GetNameForType(type);
	CXMLTreeNode node_type=node[name.c_str()];
	if (node_type.Exists())
	{
		switch(type)
		{
			case CItem::SHOTGUN:
				LoadShotGUNNode(node_type);
				break;
			case CItem::ROCKETL:
				LoadRocketlNode(node_type);
				break;
			case CItem::MACHINEGUN:
				LoadMachinegunNode(node_type);
				break;
		}
	}
}

void CGUIPlayer::LoadShotGUNNode(CXMLTreeNode &node)
{
	CGUIPlayerObjectASERender *render=new CGUIPlayerObjectASERender();
	LoadGUNNode(node,CItem::SHOTGUN,render);
}

void CGUIPlayer::LoadRocketlNode(CXMLTreeNode &node)
{
}

void CGUIPlayer::LoadMachinegunNode(CXMLTreeNode &node)
{
}

bool CGUIPlayer::ReloadXML()
{
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
	m_pFaceRender->RenderScene(renderManager,fontManager,camera,*this);
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
	for (bool painting=false;maxbasecontador>0;maxbasecontador /= 10,pos.x += m_iWidthNumber)
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
				  100						100/100=1		100 % 100 =0
		*/
		if (painting || contador>=maxbasecontador)
		{
			int digito=contador/maxbasecontador;
			renderManager->DrawQuad2D(pos,m_iWidthNumber,32,UPPER_LEFT,m_TexturesNumbers[digito]);	
			contador %= maxbasecontador;
			painting=true;
		}
	}
	/*if (contador>=0)
		renderManager->DrawQuad2D(pos,m_iWidthNumber,32,UPPER_LEFT,m_TexturesNumbers[contador]);	*/
}

void CGUIPlayer::Update(float elapsedTime)
{
	m_fYawGun += elapsedTime;
	m_fPitchGun += elapsedTime;
}
