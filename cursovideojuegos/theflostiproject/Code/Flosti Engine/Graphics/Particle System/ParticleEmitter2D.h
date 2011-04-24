//----------------------------------------------------------------------------------
// CParticleEmitter2D class
// Author: Enric Vergara
//
// Description:
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PARTICLE_EMITTER_2D_H_
#define INC_PARTICLE_EMITTER_2D_H_

//---Forward Declaration---
class CSprite;
class CTexture;
class CRenderManager;
//-------------------------

class CParticleEmitter2D
{

public:

	CParticleEmitter2D();
	virtual ~CParticleEmitter2D();

	void			SetImage				(const std::string& imageFile) {m_sImage = imageFile;}
	void			Draw						(CRenderManager * rm);
	void			Update					(float dt);
	void			Add							();
	void			SetPosition			(float x, float y)		{ m_Position.x = x; m_Position.y = y; }
	void			SetPosition			(Vect2f& vec)					{ m_Position = vec; }
	Vect2f	 	GetPosition			()										{ return m_Position; }
	void			SetDirection		(float angle)					{ m_fDirection = angle; }
	float			GetDirection		()										{ return m_fDirection; }
	void			SetMax					(uint32 num)					{ m_uMax = num; }
	void 			SetAlphaRange		(uint32 min, uint32 max);
	void 			SetColorRange		(uint32 r1, uint32 g1, uint32 b1, uint32 r2, uint32 g2, uint32 b2);
	void 			SetSpread				(uint32 value) 				{ m_uSpread = value; }
	void 			SetLength				(float value) 				{ m_fLength = value; }
	void 			SetVelocity			(float value) 				{ m_fVelocity = value; }
	void 			SetScale				(float value) 				{ m_fScale = value; }

	
private:
	std::vector<CSprite*>	m_Particles2D;
	std::string						m_sImage;
	Vect2f								m_Position;
	float									m_fDirection;
	float									m_fLength;
	uint32								m_uMax;
	uint32								m_uAlphaMin, m_uAlphaMax;
	uint32								m_uMinR, m_uMinG, m_uMinB, m_uMaxR, m_uMaxG, m_uMaxB;
	uint32								m_uSpread;
	float									m_fVelocity;
	float									m_fScale;
	float									m_fTimer;
};

#endif // INC_PARTICLE_EMITTER_2D_H_