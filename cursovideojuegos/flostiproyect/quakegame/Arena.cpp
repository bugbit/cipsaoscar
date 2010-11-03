#include "__PCH_Tests.h"

#include "Arena.h"

CArena::CArena(void)
:m_World(NULL)
{
}

CArena::~CArena(void)
{
	Release();
}

void CArena::Release()
{
	if (m_World!=NULL)
	{
		m_World->Release();
		delete m_World;
		m_World=NULL;
	}
}

void CArena::LoadWorld(const char *pathtextures,const char *pathfmt,int numRooms)
{
	m_World=new CWorld();
	m_World->CreateRooms(pathtextures,pathfmt,numRooms);
}

void CArena::RenderScene(CRenderManager* renderManager)
{
	m_World->RenderScene(renderManager);
}
