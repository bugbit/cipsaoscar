#include "__PCH_Tests.h"

#include "PhysxMesh.h"

//---Engine Includes--------
#include "Graphics/RenderManager.h"
#include "Graphics/ASEObject/ASEObject.h"
#include "Math/Matrix44.h"

//Includes para el test de físicas:
#include "PhysX/PhysicActor.h"

CPhysxMesh::CPhysxMesh(void)
{
}

CPhysxMesh::~CPhysxMesh(void)
{
}

void CPhysxMesh::UpdatePosition()
{
	CObject3D::m_Position = m_Actor->GetPosition();
}

const Vect3f& CPhysxMesh::GetPosition() 
{
	UpdatePosition();

	return CObject3D::GetPosition();
}

void CPhysxMesh::SetPosition(const Vect3f& pos)
{
	CObject3D::SetPosition(pos);
	m_Actor->SetGlobalPosition(pos);
}

void CPhysxMesh::Update(float elapsedTime)
{
	UpdatePosition();
}

void CPhysxMesh::RenderScene(CRenderManager* renderManager)
{
	Mat44f mat;
	
	m_Actor->GetMat44(mat);
	renderManager->SetTransform(mat);
	m_AseObject->Render(renderManager);
}
