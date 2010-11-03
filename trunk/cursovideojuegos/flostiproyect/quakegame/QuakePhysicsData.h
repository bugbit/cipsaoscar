#pragma once

//---Engine Includes---
#include "PhysX/PhysicsManager.h"

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

class CQuakePhysicsData : public CPhysicUserData
{
public:
	CQuakePhysicsData(void);
	CQuakePhysicsData(std::string &name);
	CQuakePhysicsData(const char *name);
	~CQuakePhysicsData(void);

	inline const std::string&			GetName() const {return m_sName;}
	inline void							SetName(const std::string& name) {m_sName = name;}
private:
	std::string	m_sName;
};
