#include "__PCH_Graphics.h"
#include "ParticleEmitter2D.h"
#include "Sprite.h"
#include "Assert.h"
#include "Math\MathUtils.h"
#include "RenderManager.h"
#include "Core/Core.h"
#include "Textures\TextureManager.h"
#include "RenderManager.h"

CParticleEmitter2D::CParticleEmitter2D()
: m_sImage("")
, m_uMax(100)
, m_fLength(100)
, m_fDirection(0)
, m_uAlphaMin(254), m_uAlphaMax(255)
, m_uMinR(0), m_uMinG(0), m_uMinB(0), m_uMaxR(255), m_uMaxG(255), m_uMaxB(255)
, m_uSpread(10)
, m_fVelocity(1.0f)
, m_fScale(1.0f)
, m_fTimer(1.f)
{
}


CParticleEmitter2D::~CParticleEmitter2D()
{
	//destroy particles
	std::vector<CSprite*>::iterator it		= m_Particles2D.begin();
	std::vector<CSprite*>::iterator itEnd	= m_Particles2D.end();
	while( it != itEnd )
	{
		CSprite* sprite = *it;
		delete sprite;
		++it;
	}

	m_Particles2D.clear();
}

void CParticleEmitter2D::Add ()
{
	static float PI_DIV_180 = 3.1415926535f / 180.0f;
	float vx,vy;

	//create a new particle
	CSprite *p = new CSprite();
	p->SetTexture(CORE->GetTextureManager()->GetTexture(m_sImage));
	p->SetPosition(m_Position);

	//add some randomness to the spread
	float variation = (rand() % m_uSpread - m_uSpread/2) / 100.0f;

	//set linear velocity
	float dir = m_fDirection - 90.0f;
	vx = cos( dir * PI_DIV_180) + variation;
	vy = sin( dir * PI_DIV_180) + variation;
	p->SetVelocity(Vect2f(vx * m_fVelocity,vy * m_fVelocity));

	//set random color based on ranges
	float r = (rand()%(m_uMaxR-m_uMinR)+m_uMinR)/255.f;
	float g = (rand()%(m_uMaxG-m_uMinG)+m_uMinG)/255.f;
	float b = (rand()%(m_uMaxB-m_uMinB)+m_uMinB)/255.f;
	float a = (rand()%(m_uAlphaMax-m_uAlphaMin)+m_uAlphaMin)/255.f;
	p->SetColor(CColor(r,g,b,a));

	//set the scale
	p->SetScaleX(m_fScale);
	p->SetScaleY(m_fScale);

	//add particle to the emitter
	m_Particles2D.push_back(p);

}

void CParticleEmitter2D::Draw(CRenderManager * rm)
{
	//draw particles
	std::vector<CSprite*>::iterator i;
	for (i = m_Particles2D.begin(); i != m_Particles2D.end(); ++i)
	{
		(*i)->Draw(rm);
	}
}

void CParticleEmitter2D::Update (float dt)
{
	//static Timer timer;

	//do we need to add a new particle?
	if (m_Particles2D.size() < m_uMax) 
	{
		//trivial but necessary slowdown
		m_fTimer -= dt;
		if (m_fTimer < 0.f )
		{
			Add();
			m_fTimer = 1.f;
		}
	}



	std::vector<CSprite*>::iterator i;
	for (i = m_Particles2D.begin(); i != m_Particles2D.end(); ++i)
	{
		//update particle's position
		(*i)->Move();

		//is particle beyond the emitter's range?
		if ( (*i)->GetPosition().Distance(m_Position) > m_fLength)
		{
			//reset particle to the origin
			(*i)->SetX(m_Position.x);
			(*i)->SetY(m_Position.y);
		}
	}
}

void CParticleEmitter2D::SetAlphaRange (uint32 min, uint32 max) 
{ 
	m_uAlphaMin=min; 
	m_uAlphaMax=max; 
}

void CParticleEmitter2D::SetColorRange (uint32 r1, uint32 g1, uint32 b1, uint32 r2, uint32 g2, uint32 b2)
{
	m_uMinR = r1; m_uMaxR = r2;
	m_uMinG = g1; m_uMaxG = g2;
	m_uMinB = b1; m_uMaxB = b2;
}