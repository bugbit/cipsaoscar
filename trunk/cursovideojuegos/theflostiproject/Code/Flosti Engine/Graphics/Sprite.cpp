#include "__PCH_Graphics.h"
#include "Sprite.h"
#include <d3dx9.h>


#include "Graphics/RenderManager.h"
#include "Graphics/Textures/Texture.h"


CSprite::CSprite()
: m_pTexture(NULL)
, m_bImageLoaded(false)
,	m_Position(v2fZERO)
,	m_Velocity(v2fZERO)
,	m_uState(0)
,	m_uDirection(0)
,	m_uWidth(1)
,	m_uHeight(1)
,	m_uCurFrame(0)
,	m_uTotalFrames(1)
,	m_uAnimDir(1)
,	m_uAnimColumns(1)
,	m_uFrameStart(0)
,	m_uFrameTimer(0)
,	m_uAnimStartx(0)
,	m_uAnimstarty(0)
,	m_dFaceAngle(0)
,	m_dMoveAngle(0)
,	m_dRotation(0)
,	m_dScalingX(1.0f)
,	m_dScalingY(1.0f)
,	m_Color(colWHITE)
,	m_uMoveTimer(16)
,	m_uMoveStart(0)
,	m_bCollidable(true)
,	m_eCollisionMethod(COLLISION_RECT)
{}
	
CSprite::~CSprite()  
{
	//CHECKED_DELETE(m_pTexture)
}
	

void CSprite::Transform(CRenderManager* rm)
{
	D3DXMATRIX mat;
	D3DXVECTOR2 scale((float)m_dScalingX,(float)m_dScalingY);
	D3DXVECTOR2 center((float)(m_uWidth*m_dScalingX), (float)(m_uHeight*m_dScalingY));
	//D3DXVECTOR2 center(0.f,0.f);
	D3DXVECTOR2 trans((float)GetX(), (float)GetY());
	D3DXMatrixTransformation2D(&mat,NULL,0,&scale,&center,(float)m_dRotation,&trans);

	rm->GetSpriteHandler()->SetTransform(&mat);
}

void CSprite::SetTexture (CTexture* texture)
{
	m_pTexture = texture;
	assert(texture);
	uint32 w,h;
	m_pTexture->GetWidthAndHeight(w,h);
	m_uWidth	= w;
	m_uHeight	= h;
	
}

void CSprite::SetAbsWAndH(uint32 w, uint32 h)
{
	m_dScalingX = (w/(double)m_uWidth);
	m_dScalingY = (h/(double)m_uHeight);
}

void CSprite::Draw(CRenderManager* rm)
{

	int fx = (m_uCurFrame % m_uAnimColumns) * m_uWidth;
	int fy = (m_uCurFrame / m_uAnimColumns) * m_uHeight;
	RECT srcRect = {fx,fy, fx+ m_uWidth, fy+m_uHeight};

	Transform(rm);
	D3DXCOLOR color(m_Color.GetUint32Argb());
	rm->GetSpriteHandler()->Draw(m_pTexture->GetD3DXTexture(),&srcRect,NULL,NULL,color);

}
	
void CSprite::Move()
{
	if (m_uMoveTimer > 0) 
	{
		if (timeGetTime() > (DWORD)(m_uMoveStart + m_uMoveTimer)) 
		{
			//reset move timer
			m_uMoveStart = timeGetTime();

			//move sprite by velocity amount
			SetX(GetX() + m_Velocity.x);
  		SetY(GetY() + m_Velocity.y);
		}
	}
	else 
	{
		//no movement timer--update at cpu clock speed
	  SetX(GetX() + m_Velocity.x);
		SetY(GetY() + m_Velocity.y);
	}
}
	

void CSprite::Animate() 
{
  //update frame based on animdir
  if (m_uFrameTimer > 0) 
	{
		if (timeGetTime() > (DWORD)(m_uFrameStart + m_uFrameTimer))  
		{
			//reset animation timer
			m_uFrameStart = timeGetTime();
			m_uCurFrame += m_uAnimDir;

			//keep frame within bounds	
			if (m_uCurFrame < 0) m_uCurFrame = m_uTotalFrames-1;
			if (m_uCurFrame > m_uTotalFrames-1) m_uCurFrame = 0;
		}
	}
	else 
	{
		//no animation timer--update at cpu clock speed
		m_uCurFrame += m_uAnimDir;
		if (m_uCurFrame < 0) m_uCurFrame = m_uTotalFrames-1;
		if (m_uCurFrame > m_uTotalFrames-1) m_uCurFrame = 0;
	}
}

//Rect Sprite::getBounds()
//{
//	Rect rect;
//    rect.left = getX();
//rect.top = getY();
//rect.right = getX() + getWidth() * getScale();
//rect.bottom = getY() + getHeight() * getScale();
//    return rect;
//}

