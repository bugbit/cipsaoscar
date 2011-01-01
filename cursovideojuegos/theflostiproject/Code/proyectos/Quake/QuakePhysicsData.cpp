#include "__PCH_Quake.h"

#include "QuakePhysicsData.h"
#include "player.h"
#include "item.h"

CQuakePhysicsData::CQuakePhysicsData(CObject3D *object3d)
:m_sName("Default"),m_Object3D(object3d)
{
}

CQuakePhysicsData::CQuakePhysicsData(std::string &name,CObject3D	*object3d)
:m_sName(name),m_Object3D(object3d)
{
}

CQuakePhysicsData::CQuakePhysicsData(const char *name,CObject3D	*object3d)
:m_sName(name),m_Object3D(object3d)
{
}

CQuakePhysicsData::~CQuakePhysicsData(void)
{
}

ETYPEOBJECT3D GetType(CObject3D *obj3d)
{
	if (dynamic_cast<CPlayer *>(obj3d)!=NULL)
		return TYPE3D_PLAYER;
	else if (dynamic_cast<CItem *>(obj3d)!=NULL)
		return TYPE3D_ITEM;

	return TYPE3D_NINGUNO;
}

