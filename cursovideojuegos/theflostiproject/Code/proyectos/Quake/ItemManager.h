#pragma once

class CItemManager
{
public:
	CItemManager(void);
	~CItemManager(void);

	bool																							Init	              ();
	void																							Done	              ();
	inline bool																				IsOk	              () const { return m_bIsOk; }
private:
	bool																							m_bIsOk;			      // Initialization boolean control	

	void																							Release             ();
};
