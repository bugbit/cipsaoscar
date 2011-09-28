#include "__PCH_Quake.h"

#include <assert.h>

#include "GUIPlayer.h"
#include "Player.h"
#include "GUIPlayerFaceRender.h"
#include "GUIPlayerGunRender.h"
#include "GUIPlayerAmmoRender.h"

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
,m_TextureCrossHair(NULL)
,m_bShot(false)
,m_bSetBack(false)
,m_fAmountSetBack(0.f)
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
	for(int i=0;i<RangTexturesNumbers();i++)
	{
		CTexture *texture=m_TexturesNumbers[i];
		if (texture!=NULL)
		{
			delete texture;
			m_TexturesNumbers[i]=NULL;
		}
	}
	if (m_TextureCrossHair!=NULL)
	{
		delete m_TextureCrossHair;
		m_TextureCrossHair=NULL;
	}
	ReleaseItemRender();
}

void CGUIPlayer::ReleaseItemRender()
{
	std::map<CItem::ETYTE,CGUIPlayerObjectRender *>::iterator it=m_ItemsRender.begin(),itend=m_ItemsRender.end();

	for (;it!=itend;it++)
	{
		CGUIPlayerObjectRender *render=(*it).second;
		CHECKED_DELETE(render);
	}
	m_ItemsRender.clear();
}

bool CGUIPlayer::Init()
{
	m_sFileXML="";
	m_bIsOk=true;	
	m_fYawGun=0;
	m_fPitchGun=0;
	m_bShot=false;
	m_bSetBack=false;
	m_fAmountSetBack=0.f;

	return m_bIsOk;
}

void CGUIPlayer::LoadItemNode(CXMLTreeNode &node,CItem::ETYTE type,CGUIPlayerObjectASERender *render)
{
	std::string model=node.GetPszProperty("model");
	std::string texture=node.GetPszProperty("texture");
	render->LoadObjectASE(model,texture);
	LoadItemNode(type,(CGUIPlayerObjectRender*) render);
	LOGGER->AddNewLog(ELL_INFORMATION, "CGUIPlayer::LoadItemNode: Cargado model : %s, pathtextures : %s",model.c_str(),texture.c_str());
}

void CGUIPlayer::LoadItemNode(CItem::ETYTE type,CGUIPlayerObjectRender *render)
{
	m_ItemsRender[type]=render;
}

void CGUIPlayer::LoadTextureNumber(int i,std::string filetexture)
{
	m_TexturesNumbers[i]=CORE->GetTextureManager()->GetTexture(filetexture);
}

void CGUIPlayer::LoadTextureCrossHair(std::string filetexture)
{
	m_TextureCrossHair=CORE->GetTextureManager()->GetTexture(filetexture);
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
		std::string texture;
		LoadFaceNode(face);
		LoadGUNNode(guiplayer,CItem::SHOTGUN);
		LoadGUNNode(guiplayer,CItem::ROCKETL);
		LoadGUNNode(guiplayer,CItem::MACHINEGUN);
		LoadAmmoNode(guiplayer,CItem::AMMOSHOTGUN);
		LoadAmmoNode(guiplayer,CItem::AMMOROCKETL);
		LoadAmmoNode(guiplayer,CItem::AMMOMACHINEGUN);
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
		CXMLTreeNode crosshair=guiplayer["crosshair"];
		texture=crosshair.GetPszProperty("texture");
		LoadTextureCrossHair(texture);
		LOGGER->AddNewLog(ELL_INFORMATION, "CGUIPlayer::LoadXML: texture: %s", texture.c_str());
	}

	return m_bIsOk;
}

void CGUIPlayer::LoadFaceNode(CXMLTreeNode &node)
{
	CGUIPlayerFaceRender *render=new CGUIPlayerFaceRender();

	LoadItemNode(node,CItem::LIFE,render);
}

void CGUIPlayer::LoadGUNNode(CXMLTreeNode &node,CItem::ETYTE type)
{
	std::string name=CItemTypeManager::GetInstance().GetNameForType(type);
	CXMLTreeNode node_type=node[name.c_str()];

	if (node_type.Exists())
	{
		bool bAnimatedShot=node_type.GetBoolProperty("animatedShot");
		bool bAnimatedSetBack=node_type.GetBoolProperty("animatedsetback");
		CGUIPlayerGunRender* render=new CGUIPlayerGunRender(bAnimatedShot,bAnimatedSetBack);
		LoadItemNode(node_type,type,render);
	}
}

void CGUIPlayer::LoadAmmoNode(CXMLTreeNode &node,CItem::ETYTE type)
{
	std::string name=CItemTypeManager::GetInstance().GetNameForType(type);
	CXMLTreeNode node_type=node[name.c_str()];

	if (node_type.Exists())
	{
		CGUIPlayerAmmoRender* render=new CGUIPlayerAmmoRender();
		LoadItemNode(node_type,type,render);
	}
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

void CGUIPlayer::RenderItem(CItem::ETYTE type,CRenderManager* renderManager, CFontManager* fontManager, CCamera *camera)
{
	CGUIPlayerObjectRender *render=m_ItemsRender[type];
	if (render!=NULL)
		render->RenderScene(renderManager,fontManager,camera,*this);
}

void CGUIPlayer::RenderScene(CRenderManager* renderManager, CFontManager* fontManager, CCamera *camera)
{
	RenderItem(CItem::LIFE,renderManager,fontManager,camera);
	if (m_pPlayer!=NULL)
	{
		CItem::ETYTE type=m_pPlayer->GetTypeGun();
		if (type!=CItem::NONE)
		{
			RenderItem(type,renderManager,fontManager,camera);
			CItem::ETYTE ammo=CItemTypeManager::GetInstance().GetAmmo(type);
			if (ammo!=CItem::NONE)
				RenderItem(ammo,renderManager,fontManager,camera);
		}
	}
}

void CGUIPlayer::RenderScene2D(CRenderManager* renderManager, CFontManager* fm)
{
	uint32 w,h;
	renderManager->GetWidthAndHeight(w,h);
	int xcontadores=(int) ((float)(w)*0.5f -20 -m_iWidthNumber*9);
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
	RenderContador2D(renderManager,fm,xcontadores,(h)-60-20,m_pPlayer->GetStatusPlayer(),100);
	RenderContador2D(renderManager,fm,xcontadores,(h)-150-20,m_pPlayer->GetStatusGun(),100);
	RenderCrossHair2D(renderManager,fm);
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

void CGUIPlayer::RenderCrossHair2D(CRenderManager* renderManager, CFontManager* fm)
{
	uint32 w,h;
	renderManager->GetWidthAndHeight(w,h);
	Vect2i pos(w/2-20,h/2-20);
	renderManager->DrawQuad2D(pos,20,20,UPPER_LEFT,m_TextureCrossHair);	
}

void CGUIPlayer::Update(float elapsedTime)
{
	GUN *gun;

	m_fYawGun += elapsedTime;
	m_fPitchGun += elapsedTime;
	if (m_bSetBack)
	{
		m_fAmountSetBack += 4*elapsedTime;
		if( m_fAmountSetBack > D3DX_PI)
		{
			m_bSetBack = false;
			m_fAmountSetBack = 0.f;
		}
	}
	if (m_pPlayer!=NULL)
	{
		gun=m_pPlayer->GetGunSelected();
		if (m_pPlayer->IsShot())
		{
			m_bShot=true;
			if (gun!=NULL)
			{
				if (gun->back)
					if (!m_bSetBack)
						m_bSetBack=true;
			}
		}
		else
		{
			if (m_bShot)
				m_bShot=false;
		}
	}
}