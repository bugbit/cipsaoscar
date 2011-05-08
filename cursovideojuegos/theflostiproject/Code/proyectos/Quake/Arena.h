#pragma once

#include "ItemManager.h"

//--Forward Declaration--
class IWorld;
class CPlayer;
//Engine:
class CRenderManager;
class CFontManager;

class CArena
{
public:
	CArena(void);
	~CArena(void);

	bool																								Init	              ();
	void																								Done	              ();
	inline bool																					IsOk	              () const { return m_bIsOk; }

	//---Update and Render function
	void																								Update	            (float elapsedTime);
	void																								RenderScene					(CRenderManager* renderManager, CFontManager* fontManager);

	inline IWorld &																			GetWorld						() const  { return *m_pWorld; }
	inline void																					SetWorld						(IWorld *world) { m_pWorld=world; }
	inline CItemManager &																GetItemManager			() { return m_ItemManager; }
	inline const std::vector<CPlayer *>	&								GetPlayers					() const { return m_Players; }
	inline void																					AddPlayer						(CPlayer *player) { m_Players.push_back(player); }

private:
	bool																								m_bIsOk;			      // Initialization boolean control
	IWorld *																						m_pWorld;
	std::vector <CPlayer *>															m_Players;
	CItemManager																				m_ItemManager;
	
	void																								UpdatePlayer				(float elapsedTime);
	void																								Release             ();
	void																								ReleasePlayers			();
};
