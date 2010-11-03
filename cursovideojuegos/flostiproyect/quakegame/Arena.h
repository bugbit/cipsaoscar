#pragma once
#include "World.h"

//--Forward Declaration--
class CRenderManager;

class CArena
{
public:
	CArena(void);
	~CArena(void);
	inline const CWorld &																GetWorld() const { return *m_World;  }

	void																								Release();
	void																								LoadWorld(const char *pathtextures,const char *pathfmt,int numRooms);
	void																								RenderScene(CRenderManager* renderManager);
private:	
	CWorld																							*m_World;
};
