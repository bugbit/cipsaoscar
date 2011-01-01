#pragma once
#include "Graphics\Object3D.h"

//---Forward Declarations---
class CPhysicActor;
class CQuakePhysicsData;

class CItem :	public CObject3D
{
public:
	CItem(void);
	virtual	~CItem(void);
protected:
	CQuakePhysicsData					*m_pPhysxData;
	CPhysicActor							*m_pActor;
	float											m_fTimer;
	bool											m_bSelected;

	virtual	bool							AccionSelected(CObject3D &object3D)=0;
};
