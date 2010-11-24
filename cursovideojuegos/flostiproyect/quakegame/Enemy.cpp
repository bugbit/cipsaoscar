#include "__PCH_Tests.h"

#include "Enemy.h"

//---Engine Includes--------
#include "Graphics/RenderManager.h"
#include "Math/Matrix44.h"

CEnemy::CEnemy(void)
{
	for (int i=0;i<10;i++)
	{
		Vect3f pPos(50.f+15.f*cos(e2PIf*i/10.f),0.f,10.f+15.f*sin(e2PIf*i/10.f));
		m_WayPoints.push_back(pPos);
	}
	m_Position=*(m_WayPoints.begin());
	m_CurrentWayP=1;
}

CEnemy::~CEnemy(void)
{
}

void CEnemy::Update(float elapsedTime)
{
	Vect3f dir=m_WayPoints[m_CurrentWayP]-m_Position;
	float pLng=dir.Length();
	dir.Normalize();
	m_Position += dir*elapsedTime;
	if (pLng<.5)
		m_CurrentWayP=(m_CurrentWayP+1) % m_WayPoints.size();
}

void CEnemy::RenderScene(CRenderManager* renderManager)
{
	std::vector<Vect3f>::iterator it=m_WayPoints.begin(),itend=m_WayPoints.end();

	for (;it!=itend;it++)
	{
		Mat44f mat;
		mat.SetIdentity();
		mat.Translate(*it);
		renderManager->SetTransform(mat);
		renderManager->DrawSphere(.5f,colCYAN);
	}
	renderManager->SetTransform((Mat44f&) m44fIDENTITY);
	it=m_WayPoints.begin();
	Vect3f pPos=*it,pPosIni=pPos;
	while(++it<itend)
	{
		renderManager->DrawLine(pPos,*it,colCYAN);
		pPos=*it;
	}
	renderManager->DrawLine(pPos,pPosIni,colCYAN);
	Mat44f mat2;
	mat2.SetIdentity();
	mat2.Translate(m_Position);
	renderManager->SetTransform(mat2);
	renderManager->DrawCube(.8f,colGREEN);
}
