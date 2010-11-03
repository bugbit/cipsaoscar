#include "__PCH_Tests.h"

#include "Player.h"

//---Engine Includes--------
#include "PhysX/PhysicActor.h"
#include "PhysX/PhysicsManager.h"
#include "PhysX/PhysicController.h"

CPlayer::CPlayer()
:m_PhysicData(NULL)
,m_PControler(NULL)
{
}