#pragma once

#include "Item.h"
//--Forward Declaration--
class CPlayer;
class CGUIPlayerObjectRender;
class CGUIPlayerObjectASERender;
class CRenderManager;
class CFontManager;
class CCamera;
class CTexture;
class CXMLTreeNode;

//---Engine Includes--------
#include "Graphics/ASEObject/ASEObject.h"

class CGUIPlayer
{
public:
	CGUIPlayer														(void);
	virtual										~CGUIPlayer	(void);

	bool											Init									();
	void											Done									();
	inline bool								IsOk									() const { return m_bIsOk; }

	void											RenderScene						(CRenderManager* renderManager, CFontManager* fontManager, CCamera *camera);
	void											RenderScene2D					(CRenderManager* renderManager, CFontManager* fm);
	void											Update								(float elapsedTime);
	inline void								SetPlayer							(CPlayer * player) {m_pPlayer = player;}
	void											LoadTextureNumber			(int i,std::string filetexture);
	void											LoadTextureCrossHair	(std::string filetexture);
	bool											LoadXML								(std::string filexml);
	bool											ReloadXML							();

	inline CPlayer*						GetPlayer							() const { return m_pPlayer; }
	inline float							GetYawGun							() const { return m_fYawGun;  }
	inline float							GetPitchGun						() const { return m_fPitchGun; }
	inline bool								IsShot								() const { return m_bShot; }
	inline void								ChangeShot						(bool bshot) { m_bShot=bshot; }
	inline bool								IsSetBack							() const { return m_bSetBack; }
	inline void								ChangeSetBack					(bool bsetback){ m_bSetBack=bsetback; }
	inline float							GetAmountSetBack			() const { return m_fAmountSetBack; }
private:
	bool																						m_bIsOk;			      // Initialization boolean control
	std::string																			m_sFileXML;
	CPlayer*																				m_pPlayer;
	std::map<CItem::ETYTE,CGUIPlayerObjectRender *>	m_ItemsRender;
	float																						m_fYawGun;
	float																						m_fPitchGun;
	float																						m_fAmountSetBack;
	static const int																m_iWidthNumber;
	bool																						m_bShot;
	bool																						m_bSetBack;

	// 2D
	CTexture																				*m_TexturesNumbers[10];
	CTexture																				*m_TextureCrossHair;

	void											Release								();
	void											ReleaseItemRender			();
	void											LoadItemNode					(CItem::ETYTE type,CGUIPlayerObjectRender *render);
	void											LoadItemNode					(CXMLTreeNode &node,CItem::ETYTE type,CGUIPlayerObjectASERender *render);
	void											LoadFaceNode					(CXMLTreeNode &node);
	void											LoadGUNNode						(CXMLTreeNode &node,CItem::ETYTE type);
	void											LoadAmmoNode					(CXMLTreeNode &node,CItem::ETYTE type);
	inline int								RangTexturesNumbers() const { return sizeof(m_TexturesNumbers)/sizeof(*m_TexturesNumbers); }

	void											RenderItem						(CItem::ETYTE type,CRenderManager* renderManager, CFontManager* fontManager, CCamera *camera);
	
	// Funciones render2D

	void											RenderContador2D			(CRenderManager* renderManager, CFontManager* fm,int x,int y,int contador,int maxbasecontador=100);
	void											RenderCrossHair2D			(CRenderManager* renderManager, CFontManager* fm);
};
