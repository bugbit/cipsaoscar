#include "Simon.h"
#include "InputManager.h"
#include "DebugPrintText2D.h"
#include "time.h"

CSimon::CSimon() {
	srand(unsigned(time(0)));
	m_fTime = -2;
	m_hasBeep = false;
}

CSimon::~CSimon() {}

void CSimon::Render (CDebugPrintText2D& printText2d) {
	switch (m_board.GetEstado()) {
		case LOOPSTATE_LAST:
			// se encarga CSimon::Update
			//printText2d.PrintText(0, 0, 0xFF00C000, "GENERANDO NUEVO ELEMENTO");
			m_hasBeep = true;
			CSimon::Print(printText2d, SYMBOL_A);
			CSimon::Print(printText2d, SYMBOL_B);
			CSimon::Print(printText2d, SYMBOL_C);
			CSimon::Print(printText2d, SYMBOL_D);
			break;
		case LOOPSTATE_SHOW:
			printText2d.PrintText(0, 0, 0xFFFFFFFF, "REPITE LA SIGUIENTE SECUENCIA:");
			Print(printText2d, m_board.Show());
			break;
		case LOOPSTATE_CHECK:
			if (m_tecla != SYMBOL_LAST) {
				if (m_fTime < -0.5) m_fTime = -0.5; //recorta el tiempo de espera
				Print(printText2d, m_tecla); //muestra la pulsacion
				if (m_fTime > 0) { //espera a que termine el tiempo
					if (m_board.Check(m_tecla)) { //verifica la pulsacion
						if (!m_board.Increase()) { //si ha terminado la secuencia
							m_board.SetEstado(LOOPSTATE_LAST); //repite el bucle creando un nuevo item
						}
						m_fTime = -2;
						m_tecla = SYMBOL_LAST;
					} else {
						m_fTime = -2;
						m_board.SetEstado(LOOPSTATE_WAIT);
					}
					m_hasBeep = false;
				}
			} else if (m_fTime > 0) {
				m_fTime = -2;
				m_hasBeep = false;
				m_board.SetEstado(LOOPSTATE_WAIT);
			} else {
				printText2d.PrintText(0, 0, 0xFFFFFFFF, "TIEMPO RESTANTE: %.2lf", -m_fTime);
			}
			break;
		case LOOPSTATE_WAIT:		
			printText2d.PrintText(400, 300, 0xFF0000C0, "ERROR");
			if (!m_hasBeep && m_fTime > -2) {
				Beep(1500, 100);
				Beep(500, 500);
				m_hasBeep = true;
			}
			break;
	}
}

void CSimon::Update (float dt) {
	CInputManager *input = CInputManager::GetInstance();
	m_fTime += dt;
	if (m_fTime > 60) m_fTime = 0;

	switch (m_board.GetEstado()) {
		case LOOPSTATE_LAST:
			if (m_fTime > 0) {
				m_board.Generate();
				m_board.SetEstado(LOOPSTATE_SHOW);
				m_hasBeep = false;
				m_fTime = -2;
			}
			break;
		case LOOPSTATE_SHOW:
			if (m_fTime > 0) {
				m_hasBeep = false;
				m_fTime = -2;
				if (!m_board.Increase()) {
					m_tecla = SYMBOL_LAST;
					m_board.SetEstado(LOOPSTATE_CHECK);
				}
			}
			break;
		case LOOPSTATE_CHECK:
			if (m_tecla == SYMBOL_LAST) {
				if (input->DoAction("teclaW")) {
					m_tecla = SYMBOL_A;
				} else if (input->DoAction("teclaD")) {
					m_tecla = SYMBOL_B;
				} else if (input->DoAction("teclaS")) {
					m_tecla = SYMBOL_C;
				} else if (input->DoAction("teclaA")) {
					m_tecla = SYMBOL_D;
				}
			}
			_flushall();
			break;
		case LOOPSTATE_WAIT:
			if (m_fTime > 0) {
				m_board.Reiniciar();
				m_board.SetEstado(LOOPSTATE_LAST);
				m_hasBeep = false;
				m_fTime = -2;
			}
			break;
	}
}

void CSimon::Print(CDebugPrintText2D& pt2d, ESymbols e) {
	int iFreq = 0;

	switch (e) {
		case SYMBOL_A:
			pt2d.PrintText(400, 100, 0xFF0000FF, "W");
			iFreq = 1500;
			break;
		case SYMBOL_B:
			pt2d.PrintText(600, 300, 0xFFFF0000, "D");
			iFreq = 1000;
			break;
		case SYMBOL_C:
			pt2d.PrintText(400, 500, 0xFF00FFFF, "S");
			iFreq = 2000;
			break;
		case SYMBOL_D:
			pt2d.PrintText(200, 300, 0xFF00FF00, "A");
			iFreq = 500;
			break;
	}
	if (!m_hasBeep && iFreq > 0) {
		Beep(iFreq, 200);
		m_hasBeep = true;
	}
}