#include "__PCH_Tests.h"

#include "PhysxMesh.h"

//Includes para el test de físicas:
#include "PhysX/PhysicActor.h"

CPhysxMesh::CPhysxMesh(void)
{
}

CPhysxMesh::~CPhysxMesh(void)
{
}

const Vect3f& CPhysxMesh::GetPosition	() const
{
	CObject3D::m_Position = m_Actor->GetPosition();
	return CObject3D::GetPosition();
}

void CPhysxMesh::SetPosition(const Vect3f& pos)
{
}
