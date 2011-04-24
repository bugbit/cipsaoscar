//----------------------------------------------------------------------------------
// CTexture class
// Author: Jordi Arnal & Enric Vergara
//
// Description:
// Clase...
//----------------------------------------------------------------------------------
#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3dx9.h>
#include "Math/Color.h"

class CTexture
{
public:
	CTexture(): m_Texture(NULL), m_FileName(""), m_bDefaultTexture(false) {}
  ~CTexture();

  const std::string &				GetFileName							() const {return m_FileName;}
  inline LPDIRECT3DTEXTURE9	GetD3DXTexture					() const {return m_Texture;}
	LPDIRECT3DSURFACE9				GetD3DSurface						() const;
	inline void								SetD3DXTexture					(LPDIRECT3DTEXTURE9	text) {m_Texture = text;}
  bool 											Load										(const std::string &FileName);
	bool 											Save										(const std::string &FileName);
  bool 											Reload									();
  void 											Activate								(size_t Stage);
  bool											CreateTexture						(uint32 width, uint32 height, bool renderTarget = false);
  bool											FillTextureFromBuffer		(uint32 width, uint32 height, uint8* pImgBuffer);
  bool											FillTextureWithColor		(uint32 width, uint32 height, const CColor& color);
	void											SetAsDefaultTexture			() {m_bDefaultTexture = true;}
	bool											IsDefaultTexture				() const {return m_bDefaultTexture;}
	void											GetWidthAndHeight				(uint32& w, uint32&h) {w = m_uWidth; h = m_uHeight;}

private:
  bool 											LoadTexture			();
  void 											UnLoad					();

private:
  LPDIRECT3DTEXTURE9	m_Texture;
	uint32							m_uWidth;
	uint32							m_uHeight;
  std::string					m_FileName;
	bool								m_bDefaultTexture;


};

#endif //TEXTURE_H