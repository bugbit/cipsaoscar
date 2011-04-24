//----------------------------------------------------------------------------------
// Engine defines
// Author: Enric Vergara
//
// Description:
// ...
//----------------------------------------------------------------------------------
#ifndef __ENGINE_DEFS__
#define __ENGINE_DEFS__


//---- Declaracion de nuevos tipos------------
struct SEngineInitParams
{
	SEngineInitParams::SEngineInitParams():	m_ScreenResolution(Vect2i(800,600)),	m_sProcess(""),  
																					m_bActivateConsole(true),							m_bActivateLogger(true),
																					m_bFullScreen(false),									m_bDrawPointerMouse(true),
																					m_bExclusiveModeinMouse(true),				m_sInitGuiXML(""), 
																					m_sInitSoundsXML(""),									m_sInitAvisXML(""),
																					m_bInitPhysX(false),									m_bInitComputerVision(false),
																					m_sInitScript(""),										m_WindowsPosition(v2iZERO),
																					m_uLinesPerPageInLogger(30),					m_sInitFontsXML(""),
																					m_sAction2InputXML("")	{}

	bool											m_bActivateConsole;
	bool											m_bActivateLogger;
	uint8											m_uLinesPerPageInLogger;
	Vect2i										m_ScreenResolution;
	Vect2i										m_WindowsPosition;
	bool											m_bFullScreen;
	bool											m_bDrawPointerMouse;
	bool											m_bExclusiveModeinMouse;
	std::string								m_sInitFontsXML;
	std::string								m_sCurrentLanguage;
	std::vector<std::string>	m_vLanguageXML;	
	std::string								m_sInitGuiXML;
	std::string								m_sInitSoundsXML;
	std::string								m_sInitAvisXML;
	bool											m_bInitPhysX;
	bool											m_bInitComputerVision;
	std::string								m_sInitScript;
	std::string								m_sProcess;
	std::string								m_sAction2InputXML;
};
//---------------------------------------------

#endif //__ENGINE_DEFS__


