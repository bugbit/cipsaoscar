#pragma once

#include "Math/Vector3.h"

//--Forward Declaration--
//Engine:
class CRenderManager;
class CFontManager;

class CItemModel
{
public:
	CItemModel																																	(void);
	virtual ~CItemModel																													(void);

	bool																										Init								();
	void																										Done	              ();
	inline bool																							IsOk	              () const { return m_bIsOk; }

	virtual void																						RenderModel					(CRenderManager* renderManager, CFontManager* fontManager)=0;
	inline const Vect3f &																		GetSize							() const { return m_Size; }
	inline void																							SetSize							(Vect3f &size) { m_Size=size; }
	inline const Vect3f *																		GetScale						() const { return m_pScale; }
	inline void																							SetScale						(Vect3f *scale) { m_pScale=scale; }
	inline const Vect3f *																		GetDesp							() const { return m_pDesp; }
	inline void																							SetDesp							(Vect3f *desp) { m_pDesp=desp; }
	inline bool																							IsDespY							() const { return m_bDespY; }
	inline void																							SetFlagDespY				(bool bdespy) { m_bDespY=bdespy; }
	inline bool																							GetDebug						() const { return m_bDebug; }
	inline void																							SetDebug						(bool debug) { m_bDebug=debug; }
protected:
	bool																										m_bIsOk;			      // Initialization boolean control
	Vect3f																									m_Size;
	Vect3f *																								m_pScale;
	Vect3f *																								m_pDesp;
	bool																										m_bDespY;
	bool																										m_bDebug;

	virtual	void																						Release						();
};
