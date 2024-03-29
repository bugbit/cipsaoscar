#include "__PCH_PhysX.h"
#include "PhysicController.h"

//---PhysX Includes---//
#undef min
#undef max
#include "NxPhysics.h"
#include "NxController.h"
#include "NxCapsuleController.h"
//---------------------//

CPhysicController::CPhysicController(float radius, float height, float slope, float skinwidth, float stepOffset,
																		 uint32 collisionGroups, CPhysicUserData* userData, const Vect3f& pos, float gravity)
																		 : m_pPhXController(NULL)
																		 , m_pPhXControllerDesc(NULL)
																		 , m_pUserData(userData)
																		 , m_fRadius_Capsule(radius)
																		 , m_fHeight_Capsule(height)
																		 , m_fSlopeLimit_Capsule(slope)
																		 , m_fSkinWidth_Capsule(skinwidth)
																		 , m_fStepOffset_Capsule(stepOffset)
																		 , m_fGravity(gravity)
																		 , m_uCollisionGroups(collisionGroups)
{
	assert(userData);

	//---- Crear un nuevo NxController----
	m_pPhXControllerDesc = new NxCapsuleControllerDesc();

	m_pPhXControllerDesc->position.x		= pos.x;
	m_pPhXControllerDesc->position.y		= pos.y;
	m_pPhXControllerDesc->position.z		= pos.z;
	m_pPhXControllerDesc->radius				= m_fRadius_Capsule;
	m_pPhXControllerDesc->height				= m_fHeight_Capsule;
	m_pPhXControllerDesc->slopeLimit		= cosf(NxMath::degToRad(m_fSlopeLimit_Capsule));
	m_pPhXControllerDesc->skinWidth			= m_fSkinWidth_Capsule;
	m_pPhXControllerDesc->stepOffset		= m_fStepOffset_Capsule;
	m_pPhXControllerDesc->upDirection		= NX_Y;
}


CPhysicController::~CPhysicController()
{
	CHECKED_DELETE(m_pPhXControllerDesc);
}


void CPhysicController::CreateController (NxController* controller, NxScene* scene)
{
	m_pPhXScene = scene;
	m_pPhXController = controller;
	assert(m_pPhXScene);
	assert(m_pPhXController);
	CHECKED_DELETE(m_pPhXControllerDesc);
}

void CPhysicController::SetPosition	(const Vect3f& pos)
{
	if (m_pPhXController != NULL)
	{
		NxExtendedVec3 position;
		position.x = pos.x;
		position.y = pos.y;
		position.z = pos.z;
		m_pPhXController->setPosition(position);
	}
	else
	{
		m_pPhXControllerDesc->position.x		= pos.x;
		m_pPhXControllerDesc->position.y		= pos.y;
		m_pPhXControllerDesc->position.z		= pos.z;
	}
	CObject3D::m_Position = pos;
}

Vect3f CPhysicController::GetPosition ()
{
	Vect3f vec;
	if (m_pPhXController != NULL)
	{
		NxExtendedVec3 tmp = m_pPhXController->getPosition();
		vec.x = (float)tmp.x;
		vec.y = (float)tmp.y;
		vec.z = (float)tmp.z;
	}
	else
	{
		vec.x = (float)m_pPhXControllerDesc->position.x;
		vec.y = (float)m_pPhXControllerDesc->position.y;
		vec.z = (float)m_pPhXControllerDesc->position.z;
	}
	return vec;
}

void CPhysicController::Jump(float ammount)
{
	m_Jump.StartJump(ammount);
}

void CPhysicController::Move (const Vect3f& direction, float elapsedTime)
{
	assert (m_pPhXController!=NULL);

	NxVec3 d(direction.x, direction.y+(m_fGravity*elapsedTime), direction.z);
	NxF32 sharpness = 1.0f;
	NxU32 collisionFlags = 0;
	float heightDelta = m_Jump.GetHeight(elapsedTime);
	if( heightDelta != 0.f )
	{
		d.y+=heightDelta;
		d.x *= 0.3f;
		d.z *= 0.3f;
	}

	m_pPhXController->move(d, m_uCollisionGroups, 0.000001f, collisionFlags, sharpness);
	if(	(collisionFlags & NXCC_COLLISION_DOWN) || (collisionFlags & NXCC_COLLISION_UP) )
	{
		m_Jump.StopJump();
	}
	NxExtendedVec3 tmp = m_pPhXController->getPosition();
	CObject3D::m_Position.x = (float)tmp.x;
	CObject3D::m_Position.y = (float)tmp.y;
	CObject3D::m_Position.z = (float)tmp.z;
}


void CPhysicController::SetCollision (bool flag)
{
	assert(m_pPhXController);
	m_pPhXController->setCollision(flag);
}

bool CPhysicController::UpdateCharacterExtents (bool bent, float ammount)
{
	NxF32 height = m_fHeight_Capsule;
	NxF32 radius = m_fRadius_Capsule;
	NxExtendedVec3 pos = m_pPhXController->getPosition();
	if ( bent )
	{
		//Ponerse de pie
		height += ammount;
		pos.y += ammount*0.5f;
	} 
	else 
	{ 
		//Agacharse
		height -= ammount;
		pos.y -= ammount*0.5f;
	}

	NxCapsule worldCapsule;
	worldCapsule.p0.x = worldCapsule.p1.x = (NxReal)pos.x;
	worldCapsule.p0.y = worldCapsule.p1.y = (NxReal)pos.y;
	worldCapsule.p0.z = worldCapsule.p1.z = (NxReal)pos.z;
	worldCapsule.p0.y -= height*0.5f;
	worldCapsule.p1.y += height*0.5f;
	worldCapsule.radius = radius;
	m_pPhXController->setCollision(false);	// Avoid checking overlap with ourself
	bool Status = m_pPhXScene->checkOverlapCapsule(worldCapsule);
	m_pPhXController->setCollision(true);
	if(Status)
	{
		return false;
	}

	NxExtendedVec3 position(pos.x, pos.y, pos.z);
	m_pPhXController->setPosition(position);
	CObject3D::m_Position.x = (float)pos.x;
	CObject3D::m_Position.y = (float)pos.y;
	CObject3D::m_Position.z = (float)pos.z;
	NxCapsuleController* c = static_cast<NxCapsuleController*> (m_pPhXController);
	c->setHeight(height);
	m_fHeight_Capsule = height;
	return true;
}