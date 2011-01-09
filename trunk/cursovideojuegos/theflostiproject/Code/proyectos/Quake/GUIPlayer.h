#pragma once

#include "Item.h"
//--Forward Declaration--
class CPlayer;
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

	bool											Init								();
	void											Done	              ();
	inline bool								IsOk	              () const { return m_bIsOk; }

	void											RenderScene				(CRenderManager* renderManager, CFontManager* fontManager, CCamera *camera);
	void											RenderScene2D			(CRenderManager* renderManager, CFontManager* fm);
	void											Update	          (float elapsedTime);
	inline void								SetPlayer					(CPlayer * player) {m_pPlayer = player;}
	void											LoadFaceASE				(std::string filease,std::string pathTextures);
	void											LoadGUNASE				(CItem::ETYTE type,std::string filease,std::string pathTextures);
	void											LoadTextureNumber	(int i,std::string filetexture);
	bool											LoadXML						(std::string filexml);
	bool											ReloadXML					();
private:
	bool																				m_bIsOk;			      // Initialization boolean control
	std::string																	m_sFileXML;
	CPlayer*																		m_pPlayer;	
	CASEObject																	m_FaceASE;
	std::map<CItem::ETYTE,CASEObject *>					m_GunsASE;
	float																				m_yawGun;
	static const int														m_iWidthNumber;

	// 2D
	CTexture																		*m_TexturesNumbers[10];

	void											Release						();
	void											ReleaseGUN				();
	void											LoadGUNNode				(CXMLTreeNode &node,CItem::ETYTE type);
	inline int								RangTexturesNumbers() const { return sizeof(m_TexturesNumbers)/sizeof(*m_TexturesNumbers); }

	// Funciones render2D

	void											RenderContador2D	(CRenderManager* renderManager, CFontManager* fm,int x,int y,int contador,int maxbasecontador=100);
};
