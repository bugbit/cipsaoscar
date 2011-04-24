//----------------------------------------------------------------------------------
// CParticleSystem class
// Author: Enric Vergara
//
// Description:
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PARTICLE_SYSTEM_H_
#define INC_PARTICLE_SYSTEM_H_

#include <d3dx9.h>
#include <string>

class CRenderManager;

//-----------------------------------------------------------------------------
// SYMBOLIC CONSTANTS
//-----------------------------------------------------------------------------

// Classify Point
const int CP_FRONT   = 0;
const int CP_BACK    = 1;
const int CP_ONPLANE = 2;

// Collision Results
const int CR_BOUNCE  = 0;
const int CR_STICK   = 1;
const int CR_RECYCLE = 2;

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------

struct Plane
{
	D3DXVECTOR3 m_vNormal;           // The plane's normal
	D3DXVECTOR3 m_vPoint;            // A coplanar point within the plane
	float       m_fBounceFactor;     // Coefficient of restitution (or how bouncy the plane is)
	int         m_nCollisionResult;  // What will particles do when they strike the plane
	Plane      *m_pNext;             // Next plane in list
};

struct Particle
{
	D3DXVECTOR3 m_vCurPos;    // Current position of particle
	D3DXVECTOR3 m_vCurVel;    // Current velocity of particle
	float       m_fInitTime;  // Time of creation of particle
	Particle   *m_pNext;      // Next particle in list
};

// Custom vertex and FVF declaration for point sprite vertex points
struct PointVertex
{
	D3DXVECTOR3 posit;
	D3DCOLOR    color;

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE
	};
};

//-----------------------------------------------------------------------------
// GLOBAL FUNCTIONS
//-----------------------------------------------------------------------------

// Helper function to stuff a FLOAT into a DWORD argument
inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }

//-----------------------------------------------------------------------------
// CLASSES
//-----------------------------------------------------------------------------

class CParticleSystem
{

public:

	CParticleSystem(void);
	~CParticleSystem(void);

	void				SetMaxParticles			(uint16 MaxParticles)			{ m_uMaxParticles = MaxParticles; }
	uint16			GetMaxParticles			()												{ return m_uMaxParticles; }

	void				SetNumToRelease			(uint16 NumToRelease)			{ m_uNumToRelease = NumToRelease; }
	uint16			GetNumToRelease			()												{ return m_uNumToRelease; }

	void				SetReleaseInterval	(float fReleaseInterval)	{ m_fReleaseInterval = fReleaseInterval; }
	float				GetReleaseInterval	()												{ return m_fReleaseInterval; }

	void				SetLifeCycle				(float fLifeCycle)				{ m_fLifeCycle = fLifeCycle; }
	float				GetLifeCycle				()												{ return m_fLifeCycle; }

	void				SetSize							(float fSize )						{ m_fSize = fSize; }
	float				GetSize							()												{ return m_fSize; }
	float				GetMaxPointSize			()												{ return m_fMaxPointSize; }

	void				SetColor						(const Vect3f& rgbColor, float alpha);
	Vect3f			GetColor						();

	void				SetPosition					(const Vect3f&  vPosition);
	Vect3f			GetPosition					();

	void				SetVelocity					(const Vect3f&  vVelocity);
	Vect3f			GetVelocity					();

	void				SetGravity					(const Vect3f&  vGravity);
	Vect3f			GetGravity					();

	void				SetWind							(const Vect3f&  vWind);
	Vect3f			GetWind							();

	void 				SetAirResistence		( bool bAirResistence )		 { m_bAirResistence = bAirResistence; }
	bool 				GetAirResistence		()												 { return m_bAirResistence; }

	void				SetVelocityVar			( float fVelocityVar )			{ m_fVelocityVar = fVelocityVar; }
	float				GetVelocityVar			()													{ return m_fVelocityVar; }

	void				SetCollisionPlane		( const Vect3f& vPlaneNormal, const Vect3f& vPoint, float fBounceFactor = 1.0f, uint32 nCollisionResult = CR_BOUNCE );

	HRESULT			Init								( CRenderManager* rm );
	HRESULT			Update							( float fElapsedTime );
	HRESULT			Render							( CRenderManager* rm );

	void				SetTexture					( const std::string& texFile ) {m_sTexFile = texFile;}
	void				RestartPartSyst			();

private:

	HRESULT			RestoreDeviceObjects		( CRenderManager* rm );
	HRESULT			InvalidateDeviceObjects	();
	

private:

	DWORD       m_dwVBOffset;
	DWORD       m_dwFlush;
	DWORD       m_dwDiscard;
	Particle   *m_pActiveList;
	Particle   *m_pFreeList;
	Plane      *m_pPlanes;
	DWORD       m_dwActiveCount;
	float       m_fCurrentTime;
	float       m_fLastUpdate;

	float       m_fMaxPointSize;
	bool        m_bDeviceSupportsPSIZE;

	LPDIRECT3DVERTEXBUFFER9 m_pVB;          // Vertex buffer for point sprites
	LPDIRECT3DTEXTURE9      m_ptexParticle; // Particle's texture

	// Particle Attributes
	uint16      m_uMaxParticles;
	uint16      m_uNumToRelease;
	float       m_fReleaseInterval;
	float       m_fLifeCycle;
	float       m_fSize;
	D3DXCOLOR   m_clrColor;
	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3 m_vVelocity;
	D3DXVECTOR3 m_vGravity;
	D3DXVECTOR3 m_vWind;
	bool        m_bAirResistence;
	float       m_fVelocityVar;
	std::string	m_sTexFile;;
};

#endif // INC_PARTICLE_SYSTEM_H_