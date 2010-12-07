#include "__PCH_Quake.h"

#include "Arena.h"
#include "world.h"

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
	
	if (m_pWorld!=NULL)
	{
		m_bIsOk=true;
		m_pWorld->Init();
	}

	return m_bIsOk;
}

void  CArena::Release()
{
	CHECKED_DELETE(m_pWorld);
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
