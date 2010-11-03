#include "__PCH_Tests.h"

#include "World.h"
#include "QuakePhysicsData.h"

//---Engine Includes--------
#include "Core/Core.h"
#include "Graphics/ASEObject/ASETextureManager.h"
#include "Math/Matrix44.h"
#include "Graphics/RenderManager.h"

//Includes para el test de físicas:
#include "PhysX/PhysicActor.h"
#include "PhysX/PhysicsManager.h"
#include "PhysX/PhysicASELoader.h"
#include "PhysX/PhysicCookingMesh.h"

CWorld::CWorld(void)
:m_PhysicRoomData(NULL)
,m_PhysicRoom(NULL)
{
}

CWorld::~CWorld(void)
{
	Release();
}

void CWorld::Release()
{
	std::vector<CASEObject *>::iterator itend=m_RoomsASE.end();
	for (std::vector<CASEObject *>::iterator it=m_RoomsASE.begin();it!=itend;it++)
	{
		CASEObject *objASE=*it;
		if (objASE!=NULL)
		{
			objASE->CleanUp();
			delete objASE;
			objASE=NULL;
		}
	}
	m_RoomsASE.clear();
	CHECKED_DELETE(m_PhysicRoomData);
	m_PhysicRoomData=NULL;
	CHECKED_DELETE(m_PhysicRoom);
	m_PhysicRoom=NULL;
}

void CWorld::CreateRooms(const char *pathtextures,const char *pathfmt,int numRooms)
{
		CRenderManager* rm = CCore::GetSingletonPtr()->GetRenderManager();
		CPhysicsManager* physicManager = CCore::GetSingletonPtr()->GetPhysicManager();
		CASETextureManager::GetInstance()->SetTexturePath(pathtextures);
		m_PhysicRoomData=new CQuakePhysicsData("World");
		m_PhysicRoomData->SetPaint(false);
		m_PhysicRoom=new CPhysicActor(m_PhysicRoomData);
		for (int i=0;i<numRooms;i++)
		{
			char bufstr[100];
			sprintf_s(bufstr,pathfmt,i);
			CASEObject *pObjAse=new CASEObject();
			pObjAse->Load(bufstr,rm);
			m_RoomsASE.push_back(pObjAse);
			// Cargamos las fisica del .ASE (de cada room)
			std::vector<uint32> faces;
			std::vector<Vect3f> vertices;
			CPhysicASELoader::ReadMeshFromASE(bufstr,vertices, faces);
			sprintf_s(bufstr,"room%i",i);
			CPhysicCookingMesh *pCooking=physicManager->GetCookingMesh();
			pCooking->CreatePhysicMesh(vertices,faces,bufstr);
			m_PhysicRoom->AddMeshShape(pCooking->GetPhysicMesh(bufstr),v3fZERO,GROUP_BASIC_PRIMITIVES);
		}
		physicManager->AddPhysicActor(m_PhysicRoom);
}

void CWorld::RenderScene(CRenderManager* renderManager)
{
	renderManager->SetTransform((Mat44f) m44fIDENTITY);
	std::vector<CASEObject *>::iterator itend=m_RoomsASE.end();
	for (std::vector<CASEObject *>::iterator it=m_RoomsASE.begin();it!=itend;it++)
	{
		(*it)->Render(renderManager);
	}
}