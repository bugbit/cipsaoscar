#include "__PCH_Quake.h"

#include <sstream>

#include "WorldASE.h"
#include "Util.h"

//---Engine Includes--------
#include "Core/Core.h"
#include "Graphics/ASEObject/ASEObject.h"
#include "Graphics/ASEObject/ASETextureManager.h"
#include "Graphics/RenderManager.h"
#include "Math/Matrix44.h"
#include "Logger/Logger.h"
#include "xml/XMLTreeNode.h"
#include "PhysX/PhysicActor.h"
#include "PhysX/PhysicsManager.h"
#include "PhysX/PhysicCookingMesh.h"

CWorldASE::CWorldASE(void):m_iNumRooms(0) {}

CWorldASE::~CWorldASE(void)
{
	Done();
}

void CWorldASE::ClearWorld()
{
		m_RoomsFilesASE.clear();
}

void CWorldASE::ClearModels()
{
	std::vector<CASEObject *>::iterator itend=m_RoomsASE.end();
	for (std::vector<CASEObject *>::iterator it=m_RoomsASE.begin();it!=itend;it++)
	{
		CASEObject *objASE=*it;
		if (objASE!=NULL)
		{
			objASE->CleanUp();
			delete objASE;
		}
	}
	m_RoomsASE.clear();
}

bool CWorldASE::LoadWorld(std::string filexml)
{
	m_sWorldXML=filexml;
	std::string pathWorld=GetDirWorld();
	LOGGER->AddNewLog(ELL_INFORMATION, "CWorldASE::LoadWorld: Iniciando el parseo del fichero %s", filexml.c_str());
	CXMLTreeNode newFile;
	m_bIsOk = false;
	if (!newFile.LoadFile((filexml).c_str()))
	{
		LOGGER->AddNewLog(ELL_ERROR, "CWorldASE::LoadWorld:: No se ha podido leer correctamente el fichero ->%s", filexml.c_str());
		m_bIsOk = false;
	}
	else
	{
		m_bIsOk=true;
		CXMLTreeNode world = newFile["world"];
		m_iNumRooms=world.GetIntProperty("num_rooms");
		m_sPathTextures=world.GetPszProperty("textures");
		m_sPathPhysx=world.GetPszProperty("physx");
		m_RoomsFilesASE.resize(m_iNumRooms);
		m_RoomsASE.resize(m_iNumRooms);
		CXMLTreeNode rooms=world["rooms"];
		int count=rooms.GetNumChildren();
		for (int i=0;i<count;i++)
		{
			CXMLTreeNode room=rooms(i);
			int id=room.GetIntProperty("id");
			m_RoomsFilesASE[id]=pathWorld+room.GetPszProperty("file");
		}
	}

	return m_bIsOk;
}

void CWorldASE::LoadModels()
{	
	CASETextureManager::GetInstance()->SetTexturePath(m_sPathTextures);
	for (int i=0;i<m_iNumRooms;i++)
	{
		CASEObject *pObjAse=m_RoomsASE[i];
		if (pObjAse!=NULL)
			delete pObjAse;
		pObjAse=LoadModelsRoom(m_RoomsFilesASE[i],i);
		m_RoomsASE[i]=pObjAse;
	}
}

#pragma warning( push )	
#pragma warning(disable : 4996)
CASEObject * CWorldASE::LoadModelsRoom(std::string filease,int nRoom)
{
	CRenderManager* rm = CORE->GetRenderManager();
	CPhysicsManager* physicManager = CORE->GetPhysicManager();
	CASEObject *pObjAse=new CASEObject();
	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	std::stringstream st( std::stringstream::out);	
	
	pObjAse->Load(filease.c_str(),rm);
	st << "Room" << nRoom << std::ends;
	_splitpath(filease.c_str(),NULL,NULL,fname,NULL);
	_splitpath(m_sPathPhysx.c_str(),drive,dir,NULL,NULL);
	_makepath(path_buffer,drive,dir,fname,".BIN");
	CUtil::LoadOrCreatePhysXMesh(filease,std::string(path_buffer),st.str());
	NxTriangleMesh* mesh = physicManager->GetCookingMesh()->GetPhysicMesh(st.str());
	m_pActor->AddMeshShape(mesh,v3fZERO,m_uPhysxGroup);

	return pObjAse;
}
#pragma warning( pop )

void CWorldASE::Update(float elapsedTime)
{
}

void CWorldASE::RenderScene(CRenderManager* renderManager, CFontManager* fontManager)
{
	renderManager->SetTransform((Mat44f) m44fIDENTITY);
	std::vector<CASEObject *>::iterator itend=m_RoomsASE.end();
	for (std::vector<CASEObject *>::iterator it=m_RoomsASE.begin();it!=itend;it++)
	{
		(*it)->Render(renderManager);
	}
}
