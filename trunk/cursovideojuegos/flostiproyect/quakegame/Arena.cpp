#include "__PCH_Tests.h"

#include "Arena.h"
#include "QuakePlayer.h"
#include "QuakePhysicsData.h"

CArena::CArena(void)
:m_World(NULL)
{
}

CArena::~CArena(void)
{
	Release();
}

void CArena::ReleasePlayers()
{
	std::vector<CQuakePlayer *>::iterator it=m_Players.begin(),
		it_end=m_Players.end();
	for (;it!=it_end;it++)
	{
		CQuakePlayer *player=*it;
		delete player;
	}
	std::vector<CQuakePhysicsData *>::iterator itd=m_PlayerDatas.begin(),
		itd_end=m_PlayerDatas.end();
	for (;itd!=itd_end;itd++)
	{
		CQuakePhysicsData *userData=*itd;
		delete userData;
	}
}

void CArena::Release()
{
	if (m_World!=NULL)
	{
		m_World->Release();
		delete m_World;
		m_World=NULL;
	}
	ReleasePlayers();
}

void CArena::LoadWorld(const char *pathtextures,const char *pathfmt,int numRooms)
{
	m_World=new CWorld();
	m_World->CreateRooms(pathtextures,pathfmt,numRooms);
}

void CArena::UpdatePlayers(float elapsedTime)
{
	std::vector<CQuakePlayer *>::iterator it=m_Players.begin(),
		it_end=m_Players.end();

	for (;it!=it_end;it++)
	{
		CQuakePlayer *item=*it;
		item->Update(elapsedTime);
	}
}

void CArena::Update(float elapsedTime)
{
	UpdatePlayers(elapsedTime);	
}

void CArena::RenderPlayers(CRenderManager* renderManager)
{
	std::vector<CQuakePlayer *>::iterator it=m_Players.begin(),
		it_end=m_Players.end();

	for (;it!=it_end;it++)
	{
		CQuakePlayer *item=*it;
		item->RenderScene(renderManager);
	}
}

void CArena::RenderScene(CRenderManager* renderManager)
{
	m_World->RenderScene(renderManager);
	RenderPlayers(renderManager);
}
