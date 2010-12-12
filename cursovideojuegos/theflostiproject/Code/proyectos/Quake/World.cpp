#include "__PCH_Quake.h"

#include <stdlib.h>

#include "world.h"
#include "QuakePhysicsData.h"

//---Engine Includes--------
#include "Core/Core.h"
#include "PhysX/PhysicsManager.h"
#include "PhysX/PhysicActor.h"

IWorld::IWorld(uint32 group)
:m_bIsOk(false)
,m_uPhysxGroup(group)
,m_pPhysxData(NULL)
,m_pActor(NULL)
{
}

IWorld::~IWorld(void)	{ Done(); }

bool IWorld::Init()
{
	m_bIsOk=false;
	m_pPhysxData=new CQuakePhysicsData("World");
	if (m_pPhysxData!=NULL)
	{
		m_bIsOk=true;
		m_pPhysxData->SetPaint(false);
		m_pActor=new CPhysicActor(m_pPhysxData);
		//CORE->GetPhysicManager()->AddPhysicActor(m_pActor);
	}

	return m_bIsOk;
}

void IWorld::Done()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

void IWorld::Release() { ClearWorld(); ClearModels(); ClearPhysx(); }

#pragma warning( push )	
#pragma warning(disable : 4996)
std::string IWorld::GetDirWorld() const
{
	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];

	_splitpath(m_sWorldXML.c_str(),drive,dir,NULL,NULL);
	_makepath(path_buffer,drive,dir,NULL,NULL);
	return path_buffer;
}
#pragma warning( pop )

bool IWorld::ReloadWorld				()
{
	ClearWorld();
	ClearModels();
	ClearPhysx();
	if (m_sWorldXML!="")
	{
		if (!LoadWorld(m_sWorldXML))
		{
			if (m_sPathTextures!="" && m_sPathTextures!="")
				LoadModels();
		}
	}
	return true;
}

void IWorld::ClearPhysx()
{
	if (m_pActor!=NULL)
	{
		CORE->GetPhysicManager()->ReleasePhysicActor(m_pActor);
		delete m_pActor;
	}
	CHECKED_DELETE(m_pPhysxData);
}