#pragma once

#include "Math/Vector3.h"

//--Forward Declaration--
//Engine:
class CRenderManager;
class CFontManager;

class CItemModel
{
public:
	inline CItemModel																														(void)	{}
	inline virtual ~CItemModel																									(void) {}
	virtual void																						RenderModel					(CRenderManager* renderManager, CFontManager* fontManager)=0;
	inline const Vect3f &																		GetSize() const { return m_Size; }
	inline void																							SetSize(Vect3f &size) { m_Size=size; }
private:
	Vect3f																									m_Size;
};
