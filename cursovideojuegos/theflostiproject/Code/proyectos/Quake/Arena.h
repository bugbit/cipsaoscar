#pragma once

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
	virtual void																				Update	            (float elapsedTime);
	virtual void																				RenderScene					(CRenderManager* renderManager, CFontManager* fontManager);

	inline IWorld &																			GetWorld() const  { return *m_pWorld; }
	inline void																					SetWorld(IWorld *world) { m_pWorld=world; }
	inline void																					AddPlayer(CPlayer *player) { m_Players.push_back(player); }

private:
	bool																								m_bIsOk;			      // Initialization boolean control
	IWorld *																						m_pWorld;
	std::vector <CPlayer *>															m_Players;

	void																								Release             ();
	void																								ReleasePlayers();
};
