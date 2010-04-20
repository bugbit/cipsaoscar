#ifndef _BODYSNAKE_H

#define	_BODYSNAKE_H

class CBodySnake
{
	public:
		CBodySnake(float posx,float posy);
		~CBodySnake(void);
		inline float GetPosX() const { return m_PosX; }
		inline float GetPosY() const { return m_PosY; }
		inline void SetPosX(float posx) { m_PosX=posx; }
		inline void SetPosY(float posy) { m_PosY=posy; }
		void Set(CBodySnake &body);
		void Set(float posx,float posy);

	private:
		float m_PosX,m_PosY;
};

#endif