#include "Board.h"
#include "InputManager.h"


CBoard::CBoard() {
	Reiniciar();
}

CBoard::~CBoard() {}

void CBoard::Reiniciar() {
	m_Symbols.clear(); //reinicio el vector
	m_Count = 0; //reinicio el contador de comprobacion
	m_Estado = LOOPSTATE_LAST; //reinicio el estado
}

void CBoard::Generate() {
	ESymbols symbol = (ESymbols)(rand() % SYMBOL_LAST); //genero un valor aleatorio
	m_Symbols.push_back(symbol); //lo guardo en el vector
	m_Count = 0; //reinicio el contador de comprobacion
	m_Estado = LOOPSTATE_LAST; //reinicio el estado
}

ESymbols CBoard::Show () {
	m_Count %= m_Symbols.size(); //compruebo OutOfBounceExcepcion
	return m_Symbols[m_Count]; //devuelvo el valor actual
}


bool CBoard::Check (ESymbols symbol) {
	m_Count %= m_Symbols.size(); //compruebo OutOfBounceExcepcion
	return ((int)m_Symbols[m_Count] == (int)symbol); //devuelvo si es correcto
}

bool CBoard::Increase() {
	m_Count++; //incremento el contador
	return (m_Count < m_Symbols.size()); //compruebo si el indice es válido
}

ELoopState CBoard::GetEstado() {
	return m_Estado;
}

void CBoard::SetEstado(ELoopState NuevoEstado) {
	m_Estado = NuevoEstado;
	m_Count = 0;
}