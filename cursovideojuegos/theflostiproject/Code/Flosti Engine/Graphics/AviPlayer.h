//----------------------------------------------------------------------------------
// CAviPlayer class
// Author: Enric Vergara
//
// Description:
// Clase...
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_AVI_PLAYER_H
#define INC_AVI_PLAYER_H

#include <d3dx9.h>
#include <vfw.h>
#include <vector>
#include "Graphics/Textures/Texture.h"

//---Forward Declaration---
class CRenderManager;
//-------------------------


//--------Declaracion de nuevos tipos----------------------------
struct AUDIO_STREAM
{
  AUDIO_STREAM(): m_pStream(NULL), m_pData(NULL) {}
  ~AUDIO_STREAM()
  {  
    if(m_pStream){
      AVIStreamRelease(m_pStream);}
    CHECKED_DELETE(m_pData);  
  }

  PAVISTREAM  m_pStream;
  uint8 *     m_pData;
};


struct VIDEO_STREAM
{
  VIDEO_STREAM(): m_pStream(NULL), m_pGetFrame(NULL) {}
  ~VIDEO_STREAM() 
  {    
    if(m_pGetFrame)
      AVIStreamGetFrameClose(m_pGetFrame);
    if(m_pStream)
      AVIStreamRelease(m_pStream);
  }
  
  PAVISTREAM  m_pStream;
  PGETFRAME   m_pGetFrame;
  long        m_lRunTime;
};
//---------------------------------------------------------------


class CAviPlayer
{
public:
  CAviPlayer(CRenderManager* rm);
  ~CAviPlayer();

	bool    		Load      	(const std::string& fileName);
  void    		Release   	();
  void    		Play      	();
	void    		Pause      	();
  void    		Stop      	();
  bool    		Done      	();
  void    		Update			(float deltaTime);
	CTexture*		GetTexture	()									{return m_pTexture;}
	void				SetTexture	(CTexture* texture) {m_pTexture = texture;}


private:

	CTexture*											m_pTexture;
  IDirect3DDevice9*             m_pDevice;
  IAVIFile*                     m_pAviFile;
  std::vector<AUDIO_STREAM*>    m_audio;
  std::vector<VIDEO_STREAM*>    m_video;
  long                          m_lTimeMS;
  bool                          m_bPlaying;
  bool                          m_bDone;
  uint32                        m_iActiveStream;
  int32                         m_iLastFrame;
	float													m_fDecimalTime;
};

#endif //INC_AVI_PLAYER_H