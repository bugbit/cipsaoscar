#pragma once

//---Engine Includes---
#include "PhysX/PhysicsManager.h"

//---Forward Declarations---
class CObject3D;

enum CollisioneGroup
{
	GROUP_BASIC_PRIMITIVES,
	GROUP_PLAYER,
	GROUP_ENEMY,
	GROUP_GRENADES,
};
#define IMPACT_MASK_1	 (1<<GROUP_BASIC_PRIMITIVES)
#define IMPACT_MASK_2 (1<<GROUP_BASIC_PRIMITIVES | 1<< GROUP_GRENADES|1<< GROUP_ENEMY)
#define COLLIDABLE_MASK (1<<GROUP_BASIC_PRIMITIVES | 1<< GROUP_GRENADES)

enum ETYPEOBJECT3D
{
	TYPE3D_NINGUNO,TYPE3D_PLAYER,TYPE3D_ITEM
};

ETYPEOBJECT3D GetType(CObject3D *obj3d);

class CQuakePhysicsData : public CPhysicUserData
{
public:
	CQuakePhysicsData(CObject3D	*object3d=NULL);
	CQuakePhysicsData(std::string &name,CObject3D	*object3d=NULL);
	CQuakePhysicsData(const char *name,CObject3D *object3d=NULL);
	~CQuakePhysicsData(void);

	inline const std::string&			GetName() const {return m_sName;}
	inline void										SetName(const std::string& name) {m_sName = name;}
	inline const CObject3D *			GetObject3D() const { return m_Object3D; }
	inline void										SetObject3D(CObject3D *object3d) { m_Object3D=object3d; }
	inline ETYPEOBJECT3D					GetType3D() const { return GetType(m_Object3D); }
private:
	std::string										m_sName;
	CObject3D	*										m_Object3D;
};
