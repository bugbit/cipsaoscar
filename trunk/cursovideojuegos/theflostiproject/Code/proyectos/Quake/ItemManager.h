#pragma once

#include <map>
#include <vector>

#include "Item.h"

//--Forward Declaration--
class CItemModel;
//Engine:
class CRenderManager;
class CFontManager;
class CXMLTreeNode;

class CItemManager
{
public:
	CItemManager(void);
	~CItemManager(void);

	bool																							Init	              ();
	void																							Done	              ();
	inline bool																				IsOk	              () const { return m_bIsOk; }
	inline void																				AddModel						(CItem::ETYTE type,CItemModel *model) { m_Models[type]=model; }
	inline CItemModel *																GetModel						(CItem::ETYTE type) { return m_Models[type]; }
	inline void																				AddItem							(CItem *item) { m_Items.push_back(item); }
	bool																							LoadModelsXml				(std::string filexml);
	bool																							ReloadModelsXml			();

		//---Update and Render function
	void																							Update	            (float elapsedTime);
	void																							RenderScene					(CRenderManager* renderManager, CFontManager* fontManager);
private:
	bool																							m_bIsOk;			      // Initialization boolean control	
	std::string																				m_sModelXML;
	std::map<CItem::ETYTE,CItemModel *>								m_Models;
	std::vector<CItem *>															m_Items;
	float																							m_fRotationY;

	void																							Release             ();
	void																							ReleaseModels				();
	void																							ReleaseItems				();

	CItemModel *																			CreateItemModel			(CXMLTreeNode &node);
	void																							LoadNodeItemModel		(CItemModel *itemmodel,CXMLTreeNode &node);
	Vect3f *																					LoadNodeItemModel		(CXMLTreeNode &node,std::string vname);
	void																							ReCreateItemsActor	();
	void																							RenderItem					(CItem *item,CRenderManager* renderManager, CFontManager* fontManager);
};
