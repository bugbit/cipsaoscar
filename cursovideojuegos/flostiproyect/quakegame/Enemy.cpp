#include "__PCH_Tests.h"

#include <stdlib.h>

#include "Enemy.h"

//---Engine Includes--------
#include "Graphics/RenderManager.h"
#include "Math/Matrix44.h"

CEnemy::CEnemy(void)
:m_State(CEnemy::REBIRTH)
{
	/*for (int i=0;i<10;i++)
	{
		float alpha=e2PIf*i/10.f;
		Vect3f pPos(35.f+15.f*cos(alpha),0.f,-5.f+15.f*sin(alpha));
		m_WayPoints.push_back(pPos);
	}*/
	m_WayPoints.push_back(Vect3f(5,2,3));
	m_WayPoints.push_back(Vect3f(-13,2,-7));
	m_WayPoints.push_back(Vect3f(-17,2,-15));
	m_Position=*(m_WayPoints.begin());
	m_CurrentWayP=1;
}

CEnemy::~CEnemy(void)
{
}

void CEnemy::Update(float elapsedTime)
{
	switch(m_State)
	{
		// REBIRTH, PATROL, WATCH, SHOOT, DEATH
		case CEnemy::REBIRTH:
			Rebirth(elapsedTime);
			break;		
		case CEnemy::PATROL:
			Patrol(elapsedTime);
			break;
		case CEnemy::WATCH:
			Watch(elapsedTime);
			break;
		case CEnemy::SHOOT:
			Shoot(elapsedTime);
			break;
		case CEnemy::DEATH:
			Death(elapsedTime);
			break;
	}
}

void CEnemy::RenderScene(CRenderManager* renderManager)
{
	RenderWayPoints(renderManager);
	RenderModel(renderManager);
}

void CEnemy::RenderWayPoints(CRenderManager* renderManager)
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
}

void CEnemy::RenderModel(CRenderManager* renderManager)
{
	float ang=mathUtils::ACos(m_VectDir.x);
	Mat44f mat;
	mat.SetIdentity();
	mat.GetRotedByAngleY(ang);
	mat.Translate(m_Position);
	renderManager->SetTransform(mat);
	renderManager->DrawCube(6.f,colGREEN);
	Vect3f posI=m_Position+Vect3f(0.f,1.5f,0.f);
	Vect3f posF=posI+3.f*m_VectDir;	
	renderManager->SetTransform((Mat44f&) m44fIDENTITY);
	renderManager->DrawLine(posI,posF,colGREEN);
	mat.SetIdentity();
	mat.GetRotedByAngleY(ang);
	mat.Translate(m_Position);
	renderManager->SetTransform(mat);
	renderManager->DrawSphere(.4f,colYELLOW);
}

void CEnemy::Rebirth(float elapsedTime)
{
	int n=m_WayPoints.size();
	m_CurrentWayP=rand() % n;
	m_Position=m_WayPoints[abs(m_CurrentWayP -1) % n];
	m_State=PATROL;
}

void CEnemy::Patrol(float elapsedTime)
{
	Vect3f dir=m_WayPoints[m_CurrentWayP]-m_Position;
	float pLng=dir.Length();
	dir.Normalize();
	m_VectDir=dir;
	m_Position += dir*elapsedTime;
	if (pLng<.5)
		m_CurrentWayP=(m_CurrentWayP+1) % m_WayPoints.size();
}

void CEnemy::Watch(float elapsedTime)
{
}

void CEnemy::Shoot(float elapsedTime)
{
}

void CEnemy::Death(float elapsedTime)
{
}
