#pragma once

//---Engine Includes----
#include "Graphics/Object3D.h"

//--Forward Declaration--
class CRenderManager;
class CPhysicActor;
class CASEObject;

class CPhysxMesh:public CObject3D
{
public:
	CPhysxMesh																				(void);
	~CPhysxMesh																				(void);
	const Vect3f&																			GetPosition();
	void																							SetPosition(const Vect3f& pos);
	virtual void																			Update(float elapsedTime);
	virtual void																			RenderScene(CRenderManager* renderManager);
private:
	CPhysicActor																			*m_Actor;
	CASEObject																				*m_AseObject;
	void																							UpdatePosition();
};
