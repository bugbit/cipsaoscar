#include "__PCH_Tests.h"

#include "QuakePhysicsData.h"

CQuakePhysicsData::CQuakePhysicsData(void)
:m_sName("Default")
{
}

CQuakePhysicsData::CQuakePhysicsData(std::string &name)
:m_sName(name)
{
}

CQuakePhysicsData::CQuakePhysicsData(const char *name)
:m_sName(name)
{
}

CQuakePhysicsData::~CQuakePhysicsData(void)
{
}
