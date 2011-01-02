#pragma once

#include <map>
#include <vector>

//--Forward Declaration--
class CItemModel;
class CItem;
//Engine:
class CRenderManager;
class CFontManager;

class CItemManager
{
public:
	CItemManager(void);
	~CItemManager(void);

	enum ETYTE { SHOTGUN, LIFE, ROCKETL, MACHINEGUN, AMMOMACHINEGUN, AMMOROCKET, AMMOSHOTGUN };

	static const std::string														NameLife;

	bool																							Init	              ();
	void																							Done	              ();
	inline bool																				IsOk	              () const { return m_bIsOk; }
	inline void																				AddModel						(CItemManager::ETYTE type,CItemModel *model) { m_Models[type]=model; }
	inline void																				AddItem							(CItem *item) { m_Items.push_back(item); }

		//---Update and Render function
	void																								Update	            (float elapsedTime);
	void																								RenderScene					(CRenderManager* renderManager, CFontManager* fontManager);
private:
	bool																							m_bIsOk;			      // Initialization boolean control	
	std::map<CItemManager::ETYTE,CItemModel *>								m_Models;
	std::vector<CItem *>															m_Items;

	void																							Release             ();
	void																							ReleaseModels				();
	void																							ReleaseItems				();
};
