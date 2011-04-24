//----------------------------------------------------------------------------------
// CActionToInpuit class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona la traduccion de una accion (un string) a un input o combinacion de inputs.
//----------------------------------------------------------------------------------

#ifndef INC_ACTION_TO_INPUT_H_
#define INC_ACTION_TO_INPUT_H_

#include "InputDefs.h"


//---Forward Declarations--
class CInputManager;
//-------------------------


//--------------ENGINE_ACTIONS----------------//
#define ACTION_LOGGER							"Logger"
#define ACTION_CONSOLE						"Console"
#define ACTION_PROCESS_INFO				"ProcessInfo"
#define ACTION_SCREENSHOT					"Screenshot"
#define ACTION_SAVE_LOGS_IN_FILE	"SaveLogsInFile"
#define ACTION_RELOAD_TTFS				"ReloadTTFs"
#define ACTION_RELOAD_LANGUAGES		"ReloadLanguageXMLs"
#define ACTION_RELOAD_ACTIONS			"ReloadActions"
#define ACTION_LOGRENDER_PAGEDOWN	"LogRender_PageDown"
#define ACTION_LOGRENDER_PAGEUP		"LogRender_PageUp"
#define ACTION_LOGRENDER_PREVLINE	"LogRender_PrevLine"
#define ACTION_LOGRENDER_NEXTLINE	"LogRender_NextLine"
//--------------------------------------------//


//-------------Declaracion de nuevos tipos-----------
struct InfoInput
{
	InfoInput::InfoInput():	m_DeviceType(IDV_NOTHING), m_EventType(EVENT_NOTHING),
													m_AxisType(AXIS_NOTHING), m_Code(MOUSE_BUTTON_NOTHING), 
													m_fDelta(1.f), m_sInfo("NOTHING"), m_sShortInfo("NOTHING"),
													m_sDeviceType(""), m_sEventType(""), m_sAxisType(""),
													m_sCode("") {}

	INPUT_DEVICE_TYPE	m_DeviceType; 
	INPUT_EVENT_TYPE	m_EventType;
	INPUT_AXIS_TYPE		m_AxisType;
	uint32						m_Code;
	float							m_fDelta;
	
	std::string				m_sDeviceType;
	std::string				m_sEventType;
	std::string				m_sAxisType;
	std::string				m_sCode;
	
	std::string				m_sInfo;		
	std::string				m_sShortInfo;		
};

typedef std::vector<InfoInput> VecInfoInputs;
//---------------------------------------------------



class CActionToInput
{
public:
	CActionToInput(): m_pInputManager(NULL), m_bIsOk(false), m_ActionXML("") {}
	virtual ~CActionToInput() {Done();}

	bool	Init							(CInputManager* im);
	void	Done        			();
	bool	IsOk        			() const { return m_bIsOk; }

	bool	LoadXML						(const std::string& xmlFile);
	bool	ReloadXML					();
	bool	SaveXML						(const std::string& xmlFile);

	void	SetAction					(const std::string& action_name, VecInfoInputs& vecInfoInput);

	bool	DoAction					(const std::string& action_name, float &amount) const;
	bool	DoAction					(const std::string& action_name ) const;
	
	void	PrintToLogger			() const;
	bool	GetActionInfo			(const std::string& action_name, std::string& info, std::string& shortInfo) const;
	
private:
	void	Release						();
	void	InitString2Input	();

private:
	typedef std::map< std::string, VecInfoInputs>	MapActions;
	typedef std::map<std::string, uint32>					MapString2Code;

	bool						m_bIsOk;
	CInputManager*	m_pInputManager;
	MapActions			m_Actions;
	MapString2Code	m_String2Code;
	std::string			m_ActionXML;
};

#endif // INC_ACTION_TO_INPUT_H_