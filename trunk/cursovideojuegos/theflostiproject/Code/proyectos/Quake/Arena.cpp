#include "__PCH_Quake.h"

#include "Arena.h"
#include "world.h"
#include "Player.h"

CArena::CArena(void):m_bIsOk(true),m_pWorld(NULL)
{
}

CArena::~CArena(void)
{
	Done();
}

void CArena::Done()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

bool CArena::Init()
{
	m_bIsOk=true;
	
	if (m_ItemManager.Init())
	{
		if (m_pWorld!=NULL)
		{		
			if (m_pWorld->Init())
			{			
				m_bIsOk=true;
			}				
		}
		if (!m_bIsOk)
			m_ItemManager.Done();
	}

	return m_bIsOk;
}

void CArena::Release()
{
	m_ItemManager.Done();
	CHECKED_DELETE(m_pWorld);
	ReleasePlayers();
}

void CArena::ReleasePlayers()
{
	std::vector<CPlayer *>::iterator it=m_Players.begin(),
		it_end=m_Players.end();
	for (;it!=it_end;it++)
	{
		CPlayer *player=*it;		
		player->Done();
	}
}

void CArena::Update(float elapsedTime)
{
	if (m_pWorld!=NULL)
		m_pWorld->Update(elapsedTime);
}

void CArena::RenderScene(CRenderManager* renderManager, CFontManager* fontManager)
{
	if (m_pWorld!=NULL)
		m_pWorld->RenderScene(renderManager,fontManager);
}
