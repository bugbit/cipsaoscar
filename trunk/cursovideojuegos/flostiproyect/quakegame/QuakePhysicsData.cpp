#include "__PCH_Tests.h"

#include "QuakePhysicsData.h"

CQuakePhysicsData::CQuakePhysicsData(ETYPEOBJECT3D typed3d,CObject3D *object3d)
:m_sName("Default"),m_Typed3D(typed3d),m_Object3D(object3d)
{
}

CQuakePhysicsData::CQuakePhysicsData(std::string &name,ETYPEOBJECT3D typed3d,CObject3D	*object3d)
:m_sName(name),m_Typed3D(typed3d),m_Object3D(object3d)
{
}

CQuakePhysicsData::CQuakePhysicsData(const char *name,ETYPEOBJECT3D typed3d,CObject3D	*object3d)
:m_sName(name),m_Typed3D(typed3d),m_Object3D(object3d)
{
}

CQuakePhysicsData::~CQuakePhysicsData(void)
{
}
