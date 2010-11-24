#pragma once
#include "World.h"

//--Forward Declaration--
class CRenderManager;
class CQuakePlayer;
class CQuakePhysicsData;
class CEnemy;

class CArena
{
public:
	CArena(void);
	~CArena(void);
	inline const CWorld &																GetWorld() const { return *m_World;  }
	inline std::vector <CQuakePlayer *> &								GetPlayers() { return m_Players; }
	inline std::vector <CQuakePhysicsData *> &					GetPlayerDatas() { return m_PlayerDatas; }

	void																								Release();
	void																								LoadWorld(const char *pathtextures,const char *pathfmt,int numRooms);
	void																								Update(float elapsedTime);
	void																								RenderScene(CRenderManager* renderManager);
private:	
	bool																								m_VisibleWorld;
	CWorld																							*m_World;
	std::vector <CQuakePlayer *>												m_Players;
	std::vector <CQuakePhysicsData *>										m_PlayerDatas;
	CEnemy																							*m_Enemy;

	
	void																								ReleasePlayers();
	void																								UpdatePlayers(float elapsedTime);
	void																								RenderPlayers(CRenderManager* renderManager);
};
