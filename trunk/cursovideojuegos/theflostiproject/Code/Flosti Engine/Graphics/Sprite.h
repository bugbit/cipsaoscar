//----------------------------------------------------------------------------------
// CSprite class
// Author: Enric Vergara
//
// Description:
// Esta clase será la base para pintar cualquier rectángulo 2d
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_SPRITE_H_
#define INC_SPRITE_H_

#include "Math\MathTypes.h"
#include "Math\Color.h"

//---Forward Declarations--
class CTexture;
class CRenderManager;
//-------------------------

typedef enum Collision2D_Type
{
		COLLISION_NONE = 0,
		COLLISION_RECT = 1,
		COLLISION_DIST = 2
};

class CSprite
{
public:
	CSprite();
	virtual ~CSprite(void);

	void							SetTexture						(CTexture* texture);
	void							Move									();
	void							Animate								();
	void							Draw									(CRenderManager* rm);
	void							Transform							(CRenderManager* rm);

	//screen position
	const Vect2f&			GetPosition						()										{ return m_Position; }
	void							SetPosition						(const Vect2f& pos)		{ m_Position = pos; }
	float							GetX									()										{ return m_Position.x; }
	float							GetY									()										{ return m_Position.y; }
	void							SetX									(float x)							{ m_Position.x = x; }
	void							SetY									(float y)							{ m_Position.y = y; }

	//movement velocity
	const Vect2f&			GetVelocity						()										{ return m_Velocity; }
	void							SetVelocity						(const Vect2f& v)			{ m_Velocity = v; }
	
	//image size
	void							SetSize								(uint32 width, int height) { m_uWidth = width; m_uHeight = height; }
	int								GetWidth							() { return m_uWidth; }
	void							setWidth							(uint32 value) { m_uWidth = value; }
	uint32						GetHeight							()		{ return m_uHeight; }
	void							SetHeight							(uint32 value) { m_uHeight = value; }

	uint32						GetState							() { return m_uState; }
	void							SetState							(uint32 value) { m_uState = value; }

	uint32						GetDirection					() { return m_uDirection; }
	void							SetDirection					(uint32 value) { m_uDirection = value; }

	uint32						GetColumns						() { return m_uAnimColumns; }
	void							SetColumns						(uint32 value) { m_uAnimColumns = value; }

	uint32						GetFrameTimer					() { return m_uFrameTimer; }
	void							SetFrameTimer					(uint32 value) { m_uFrameTimer = value; }

	uint32						GetCurrentFrame				() { return m_uCurFrame; }
	void							SetCurrentFrame				(uint32 value) { m_uCurFrame = value; }

	uint32						GetTotalFrames				() { return m_uTotalFrames; }
	void							SetTotalFrames				(uint32 value) { m_uTotalFrames = value; }

	uint32						GetAnimationDirection	() { return m_uAnimDir; }
	void							SetAnimationDirection	(uint32 value) { m_uAnimDir = value; }

	float							GetRotation						() { return (float)m_dRotation; }
	void							SetRotation						(double value) { m_dRotation = (double)value; }
	double						GetScaleX							() { return m_dScalingX; }
	double						GetScaleY							() { return m_dScalingY; }
	void							SetScaleX							(double value) { m_dScalingX = value; }
	void							SetScaleY							(double value) { m_dScalingY = value; }
	void							SetAbsWAndH						(uint32 w, uint32 h);

	//modified from original -- new accessor
	CColor&						GetColor							() { return m_Color; }
	void							SetColor							(const CColor& col) { m_Color = col; }

	uint32						GetMoveTimer					() { return m_uMoveTimer; }
	void							SetMoveTimer					(uint32 value) { m_uMoveTimer = value; }

	bool							IsCollidable					() { return m_bCollidable; }
	void							SetCollidable					(bool value) { m_bCollidable = value; }
	Collision2D_Type	GetCollisionMethod		() { return m_eCollisionMethod; }
	void							SetCollisionMethod		(Collision2D_Type type) { m_eCollisionMethod = type; }
	//Rect					GetBounds							();
	

private:
	Vect2f						m_Position;
	Vect2f						m_Velocity;
	bool							m_bImageLoaded;
	uint32						m_uState;
	uint32						m_uDirection;
	CTexture*					m_pTexture;
	uint32						m_uWidth;
	uint32						m_uHeight;
	uint32						m_uAnimColumns;
	uint32						m_uFrameStart;
	uint32						m_uFrameTimer;
	uint32						m_uMoveStart;
	uint32						m_uMoveTimer;
	bool							m_bCollidable;
	Collision2D_Type	m_eCollisionMethod;
	uint32						m_uCurFrame;
	uint32						m_uTotalFrames;
	uint32						m_uAnimDir;
	double						m_dFaceAngle;
	double						m_dMoveAngle;
	uint32						m_uAnimStartx;
	uint32						m_uAnimstarty;;
	double						m_dRotation;
	double						m_dScalingX;
	double						m_dScalingY;
	CColor						m_Color;
};

#endif //INC_SPRITE_H_