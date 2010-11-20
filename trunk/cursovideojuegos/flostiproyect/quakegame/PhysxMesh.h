#pragma once

//---Engine Includes----
#include "Graphics/Object3D.h"

//--Forward Declaration--
class CPhysicActor;

class CPhysxMesh:public CObject3D
{
public:
	CPhysxMesh																				(void);
	~CPhysxMesh																				(void);
	const Vect3f&																			GetPosition	() const;
	void																							SetPosition(const Vect3f& pos);
private:
	CPhysicActor																			*m_Actor;
};
