#pragma once

//--Forward Declaration--
class CPlayer;
class CRenderManager;
class CFontManager;
class CCamera;
class CTexture;

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
	void											LoadTextureNumber	(int i,std::string filetexture);
	void											LoadGUIPlayer			(std::string filexml);
	bool											ReloadGUIPlayer		();
private:
	bool																				m_bIsOk;			      // Initialization boolean control
	std::string																	m_sFileXML;
	CPlayer*																		m_pPlayer;	
	CASEObject																	m_FaceASE;
	float																				m_yawGun;

	// 2D
	CTexture																		*m_TexturesNumbers[10];

	virtual void							Release();

	// Funciones render2D

	void											RenderContador2D	(CRenderManager* renderManager, CFontManager* fm,int x,int y,int contador,int maxbasecontador=100);
};
