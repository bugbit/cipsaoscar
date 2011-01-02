#pragma once

#include "Graphics\Object3D.h"

//---Forward Declarations---
class CPhysicActor;
class CQuakePhysicsData;
class CItemModel;

class CItem :	public CObject3D
{
public:
	CItem																		(void);
	virtual	~CItem													(void);
	inline CItemModel &				GetModel			() { return *m_pModel; }
	inline void								SetModel			(CItemModel *model) { m_pModel=model; }
	inline CPhysicActor &			GetActor			() { return *m_pActor; }
	void											CreateActor		(const Vect3f &pos);
protected:
	CQuakePhysicsData					*m_pPhysxData;
	CPhysicActor							*m_pActor;
	CItemModel								*m_pModel;
	float											m_fTimer;
	bool											m_bSelected;

	virtual	bool							AccionSelected	(CObject3D &object3D)=0;
};
