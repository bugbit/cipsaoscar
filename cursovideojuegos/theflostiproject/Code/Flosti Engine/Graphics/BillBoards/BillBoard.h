#ifndef __BILLBOARD_H
#define __BILLBOARD_H

#include <d3dx9.h>
#include "vector"
//#include "Object3D.h"

class CCamera;
class CRenderManager;
class CTexture;
class CCamera;

class CBillBoard
{

public:
	CBillBoard();
	
	void Update	                    ( CCamera *camera ,float dt );
	virtual void Render	            ( CRenderManager *rm,const CTexture & texture );
	
	void SetPos		                  ( const Vect3f & position ) {m_Position = position;}
	void SetSize	                  ( const float &sizeX, const float &sizeY ) {m_SizeX = sizeX; m_SizeY = sizeY;}
  void SetTexCoordsDesfase        (float u, float v){m_fDesfaseU = u; m_fDesfaseV = v;}
  void SetTexCoordsDesfaseActual  (float u, float v){m_fDesfaseUActual = u; m_fDesfaseVActual = v;}
  void SetFixedAngles             (bool fixedYaw, bool fixedPitch){m_bFixedYaw = fixedYaw; m_bFixedPitch = fixedPitch;}
  void SetYaw                     (float yaw){m_fYaw = yaw;}
  void SetPitch                   (float pitch){m_fPitch = pitch;}

protected:
	Vect3f													m_Position;
	Vect3f													m_PointA, m_PointB, m_PointC, m_PointD;
	float														m_SizeX, m_SizeY;
  float														m_fDesfaseU, m_fDesfaseV;
  float														m_fDesfaseUActual, m_fDesfaseVActual;
  bool														m_bFixedYaw, m_bFixedPitch;
  float														m_fPitch, m_fYaw;
};

class CBillBoardColor : public CBillBoard
{
public:
	CBillBoardColor();
	
	virtual void Render	            ( CRenderManager *rm,const CTexture & texture );

	DWORD	GetAlpha									() const { return m_Color; }
	void SetAlpha										(DWORD alpha) { m_Color=alpha; }

protected:
	DWORD														m_Color;

};

#endif //__BILLBOARD_H