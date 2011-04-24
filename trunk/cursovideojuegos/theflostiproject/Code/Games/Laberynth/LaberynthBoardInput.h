//----------------------------------------------------------------------------------
// CBoardInput class
// Author: Enric Vergara
//
// Description:
// ...
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_LABERYNTH_BOARD_INPUT_H_
#define INC_LABERYNTH_BOARD_INPUT_H_


//---Forward Declarations--
class CLaberynthBoard;
//-------------------------

class CLaberynthBoardInput
{
public:
	CLaberynthBoardInput(CLaberynthBoard* board, float forceRJ1, float forceRJ2)
		:	m_pBoard(board), m_fForceRJ1(forceRJ1), m_fForceRJ2(forceRJ2) {assert(board);}
	virtual ~CLaberynthBoardInput() {/*Nothing*/;}

	virtual void	UpdateInputs	() = 0;
	void					SetForceRJ1		(float force)	{m_fForceRJ1 = force;}
	void					SetForceRJ2		(float force)	{m_fForceRJ2 = force;}
	float					GetForceRJ1		() const			{return m_fForceRJ1;}
	float					GetForceRJ2		() const			{return m_fForceRJ2;}

protected:
	CLaberynthBoard*	m_pBoard;
	float							m_fForceRJ1;
	float							m_fForceRJ2;
};

#endif //INC_LABERYNTH_BOARD_INPUT_H_