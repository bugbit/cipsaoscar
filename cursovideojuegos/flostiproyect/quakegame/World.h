#pragma once

//---Engine Includes----
#include "Graphics/ASEObject/ASEObject.h"
//-----------------------

//--Forward Declaration--
class CRenderManager;
class CASEObject;
class CPhysicActor;
class CQuakePhysicsData;

class CWorld
{
public:
	CWorld(void);
	~CWorld(void);

	void																								Release();
	void																								CreateRooms(const char *pathtextures,const char *pathfmt,int numRooms);
	void																								RenderScene(CRenderManager* renderManager);
private:
	std::vector<CASEObject *>															m_RoomsASE;
	CQuakePhysicsData																			*m_PhysicRoomData;
	CPhysicActor																					*m_PhysicRoom;
};
