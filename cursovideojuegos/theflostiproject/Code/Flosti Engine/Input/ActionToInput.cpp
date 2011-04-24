#include "__PCH_Input.h"
#include "ActionToInput.h"
#include "InputManager.h"
#include "Logger/Logger.h"
#include "xml/XMLTreeNode.h"

bool CActionToInput::Init (CInputManager* im)
{
  m_bIsOk = (im != NULL);
	m_pInputManager = im;
	InitString2Input();
		
  return m_bIsOk;
}


void CActionToInput::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

void CActionToInput::Release ()
{
	/*Nothing...*/
}

bool CActionToInput::ReloadXML ()
{
	if (m_ActionXML != "")
	{
		return LoadXML(m_ActionXML);
	}
	return false;
}
	
bool CActionToInput::LoadXML (const std::string& xmlFile)
{
	//First of all clean previous information of m_Actions
	m_ActionXML = xmlFile;
	m_Actions.clear();

	//Read the xml gui file
	LOGGER->AddNewLog(ELL_INFORMATION, "CActionToInput:: Iniciando el parseo del fichero %s", xmlFile.c_str());
	bool isOK = false;

	CXMLTreeNode newFile;
	if (!newFile.LoadFile(xmlFile.c_str()))
	{
		LOGGER->AddNewLog(ELL_ERROR, "CActionToInput:: No se ha podido leer correctamente el fichero ->%s", xmlFile.c_str());
		isOK = false;
	}
	else
	{
		CXMLTreeNode  actions = newFile["Actions"];

		if (actions.Exists())
		{
			int count = actions.GetNumChildren();
			for (int i = 0; i < count; ++i)
			{
				CXMLTreeNode action = actions(i);
				if (action.IsComment())
				{
					continue;
				}
				
				VecInfoInputs vecInputs;
				//Para cada action leemos su informacion comun-->
				std::string name	= action.GetPszProperty("name", "");
				int count_aux = action.GetNumChildren();
				for (int j = 0; j < count_aux; ++j)
				{
					InfoInput new_input;
					CXMLTreeNode input = action(j);
					std::string DeviceType	= input.GetPszProperty("deviceType", "IDV_NOTHING", false);
					std::string EventType		= input.GetPszProperty("EventType", "EVENT_NOTHING", false);
					std::string AxisType		= input.GetPszProperty("AxisType", "AXIS_NOTHING", false);
					std::string Code				= input.GetPszProperty("Code", "MOUSE_BUTTON_NOTHING", false);
					float Delta							= input.GetFloatProperty("Delta", 1.f, false);
					
					new_input.m_sDeviceType	= DeviceType;
					new_input.m_sEventType	= EventType;
					new_input.m_sAxisType		= AxisType;
					new_input.m_sCode				= Code;

					//---Rellenamos la información de new_input.m_sInfo
					new_input.m_sInfo = DeviceType;
					if (EventType!="EVENT_NOTHING")
					{
						new_input.m_sInfo += " | ";
						new_input.m_sInfo += EventType;
					}
					if (AxisType!="AXIS_NOTHING")
					{
						new_input.m_sInfo += " | ";
						new_input.m_sInfo += AxisType;
						new_input.m_sShortInfo = AxisType;
					}
					if (Code!="MOUSE_BUTTON_NOTHING")
					{
						new_input.m_sInfo += " | ";
						new_input.m_sInfo += Code;
						new_input.m_sShortInfo = Code;
					}
					//---

					new_input.m_fDelta = Delta;
					MapString2Code::const_iterator it;
					it = m_String2Code.find(DeviceType);
					if (it!= m_String2Code.end())
					{
						new_input.m_DeviceType = (INPUT_DEVICE_TYPE) it->second;
					}

					it = m_String2Code.find(EventType);
					if (it!= m_String2Code.end())
					{
						new_input.m_EventType = (INPUT_EVENT_TYPE) it->second;
					}

					it = m_String2Code.find(AxisType);
					if (it!= m_String2Code.end())
					{
						new_input.m_AxisType = (INPUT_AXIS_TYPE) it->second;
					}

					it = m_String2Code.find(Code);
					if (it!= m_String2Code.end())
					{
						new_input.m_Code = it->second;
					}
				
					vecInputs.push_back(new_input);

				}// END for each input
				
				if (name != "" && (m_Actions.find(name)== m_Actions.end()) )
				{
					m_Actions.insert(std::pair<std::string, VecInfoInputs> (name, vecInputs));
				}

			}// END for each action

		}//END if (actions.Exists())

	}//END if (!newFile.LoadFile(xmlGuiFile.c_str()))

	return true;
}
	
bool CActionToInput::SaveXML (const std::string& xmlFile)
{
	bool isOk = true;
	CXMLTreeNode NewXML;
	
	if (NewXML.StartNewFile(xmlFile.c_str()))
	{
		// We fill the doc here
		NewXML.StartElement("Actions");
		MapActions::const_iterator it			= m_Actions.begin();
		MapActions::const_iterator itEnd	= m_Actions.end();
		for ( ; it!= itEnd; ++it)
		{
			NewXML.StartElement("action");
			NewXML.WritePszProperty("name", it->first.c_str());
			VecInfoInputs vecInfoInputs = it->second;
			VecInfoInputs::const_iterator aux_it		= vecInfoInputs.begin();
			VecInfoInputs::const_iterator aux_itEnd	= vecInfoInputs.end();
			for(; aux_it != aux_itEnd; ++aux_it)
			{
				//<input deviceType="IDV_KEYBOARD"	EventType="EVENT_DOWN_UP"		Code="KEY_F4"/>
				//<input deviceType="IDV_MOUSE"			AxisType="AXIS_MOUSE_X"			Delta="1.f"/>
				NewXML.StartElement("input");
				NewXML.WritePszProperty("deviceType", aux_it->m_sDeviceType.c_str());

				if (aux_it->m_sEventType != "EVENT_NOTHING")
				{
					NewXML.WritePszProperty("EventType", aux_it->m_sEventType.c_str());
				}

				if (aux_it->m_sAxisType != "AXIS_NOTHING")
				{
					NewXML.WritePszProperty("AxisType", aux_it->m_sAxisType.c_str());
					NewXML.WriteFloatProperty("Delta", aux_it->m_fDelta);
				}

				if (aux_it->m_sCode != "MOUSE_BUTTON_NOTHING")
				{
					NewXML.WritePszProperty("Code", aux_it->m_sCode.c_str());
				}

				NewXML.EndElement(); //("input")
			}
			NewXML.EndElement(); //("action");
		}
		NewXML.EndElement(); //("Actions");
		
		NewXML.EndNewFile();
	}
	else
	{
		bool isOk = false;
	}
	return isOk;
}

bool CActionToInput::DoAction ( const std::string& action_name, float &delta) const
{
	bool doAction = false;
	delta = 0.f;
	MapActions::const_iterator it = m_Actions.find(action_name);
	if ( it != m_Actions.end() )
	{
		VecInfoInputs vecInfoInputs = it->second;
		VecInfoInputs::const_iterator aux_it		= vecInfoInputs.begin();
		VecInfoInputs::const_iterator aux_itEnd	= vecInfoInputs.end();
		doAction = true;
		for(; aux_it != aux_itEnd; ++aux_it)
		{
			InfoInput infoInput = *aux_it;
			if ( infoInput.m_AxisType != AXIS_NOTHING)
			{
				switch(infoInput.m_AxisType)
				{
				case AXIS_MOUSE_X:
					{
						Vect3i deltas = m_pInputManager->GetMouseDelta();
						delta = (float)deltas.x * infoInput.m_fDelta;
					}
					break;
				case AXIS_MOUSE_Y:
					{
						Vect3i deltas = m_pInputManager->GetMouseDelta();
						delta = (float)deltas.y * infoInput.m_fDelta;
					}
					break;
				case AXIS_MOUSE_Z:
					{
						Vect3i deltas = m_pInputManager->GetMouseDelta();
						delta = (float)deltas.z * infoInput.m_fDelta;
					}
					break;
				case AXIS_LEFT_THUMB_X:
					{
						float x,y;
						Vect3i deltas = m_pInputManager->GetGamePadLeftThumbDeflection(&x,&y, infoInput.m_DeviceType);
						delta = x * infoInput.m_fDelta;
					}
					break;
				case AXIS_LEFT_THUMB_Y:
					{
						float x,y;
						Vect3i deltas = m_pInputManager->GetGamePadLeftThumbDeflection(&x,&y, infoInput.m_DeviceType);
						delta = y * infoInput.m_fDelta;
					}
					break;
				case AXIS_RIGHT_THUMB_X:
					{
						float x,y;
						Vect3i deltas = m_pInputManager->GetGamePadRightThumbDeflection(&x,&y, infoInput.m_DeviceType);
						delta = x * infoInput.m_fDelta;
					}
					break;
				case AXIS_RIGHT_THUMB_Y:
					{
						float x,y;
						Vect3i deltas = m_pInputManager->GetGamePadRightThumbDeflection(&x,&y, infoInput.m_DeviceType);
						delta = y * infoInput.m_fDelta;
					}
					break;
				case AXIS_DELTA_TRIGGER_RIGHT:
					{
						float right, left;
						m_pInputManager->GetGamePadDeltaTriggers(&left, &right, infoInput.m_DeviceType );
						delta = right * infoInput.m_fDelta;
					}
					break;
				case AXIS_DELTA_TRIGGER_LEFT:
					{
						float right, left;
						m_pInputManager->GetGamePadDeltaTriggers(&left, &right, infoInput.m_DeviceType );
						delta = left * infoInput.m_fDelta;
					}
					break;
				
				}//END switch(infoInput.m_AxisType)

				if( delta == 0.f)	
				{
					doAction = false;
				}
			}
			else
			{
				switch (infoInput.m_EventType)
				{
				case EVENT_DOWN:
					{
						doAction = m_pInputManager->IsDown(infoInput.m_DeviceType,infoInput.m_Code);
					}
					break;
				case EVENT_UP_DOWN:
					{
						doAction = m_pInputManager->IsUpDown(infoInput.m_DeviceType,infoInput.m_Code);
					}
					break;
				case EVENT_DOWN_UP:
					{
						doAction = m_pInputManager->IsDownUp(infoInput.m_DeviceType,infoInput.m_Code);
					}
					break;
				}
			}

			if (doAction == false)
			{
				break;
			}
		}
	}
	
	return doAction;
}

void CActionToInput::PrintToLogger () const
{
	MapActions::const_iterator it			= m_Actions.begin();
	MapActions::const_iterator itEnd	= m_Actions.end();
	for( ; it != itEnd; ++it)
	{
		VecInfoInputs vecInfoInputs = it->second;
		VecInfoInputs::const_iterator aux_it		= vecInfoInputs.begin();
		VecInfoInputs::const_iterator aux_itEnd = vecInfoInputs.end();

		LOGGER->AddNewLog(ELL_INFORMATION, "CActionToInput:: <Action name %s>", it->first.c_str());
		for( ; aux_it != aux_itEnd; ++aux_it)
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "CActionToInput::  |___<Input: %s>", aux_it->m_sInfo.c_str());
		}
	}

}
	
bool CActionToInput::DoAction ( const std::string& action_name ) const
{
	float delta = 0.f;
	return DoAction(action_name, delta);
}

void CActionToInput::SetAction (const std::string& action_name, VecInfoInputs& vecInfoInputs)
{
	MapActions::const_iterator it = m_Actions.find(action_name);
	if (it != m_Actions.end())
	{
		m_Actions.erase(it);
	}
	m_Actions.insert(std::pair<std::string, VecInfoInputs>( action_name, vecInfoInputs));
}

bool CActionToInput::GetActionInfo (const std::string& action_name, std::string& info, std::string& shortInfo) const
{
	bool isOk = false;
	info			= "";
	shortInfo = "";
	MapActions::const_iterator it = m_Actions.find(action_name);
	if (it != m_Actions.end())
	{
		isOk = true;
		VecInfoInputs vecInputs = it->second;
		VecInfoInputs::iterator aux_it		= vecInputs.begin();
		VecInfoInputs::iterator aux_itEnd = vecInputs.end();
		for(; aux_it != aux_itEnd; ++aux_it)
		{
			if (aux_it != vecInputs.begin())
			{
				info			+= " + ";
				shortInfo	+= " + ";
			}
			info			+= "[";
			shortInfo += "[";
			
			info				+= aux_it->m_sInfo;
			shortInfo		+= aux_it->m_sShortInfo;
			
			info				+= "]";
			shortInfo		+= "]";

		}
	}

	return isOk;
}

void CActionToInput::InitString2Input ()
{
	typedef std::pair<std::string, uint32> PairString2Code;

	//----------------INPUT_DEVICE_TYPE---------------//
	m_String2Code.insert( PairString2Code("IDV_KEYBOARD",							IDV_KEYBOARD) );
	m_String2Code.insert( PairString2Code("IDV_MOUSE",								IDV_MOUSE) );
	m_String2Code.insert( PairString2Code("IDV_GAMEPAD1",							IDV_GAMEPAD1) );
	m_String2Code.insert( PairString2Code("IDV_GAMEPAD2",							IDV_GAMEPAD2) );
	m_String2Code.insert( PairString2Code("IDV_GAMEPAD3",							IDV_GAMEPAD3) );
	m_String2Code.insert( PairString2Code("IDV_GAMEPAD4",							IDV_GAMEPAD4) );
	m_String2Code.insert( PairString2Code("IDV_NOTHING",							IDV_NOTHING) );

	//----------------INPUT_AXIS_TYPE-----------------//
	m_String2Code.insert( PairString2Code("AXIS_MOUSE_X",							AXIS_MOUSE_X) );
	m_String2Code.insert( PairString2Code("AXIS_MOUSE_Y",							AXIS_MOUSE_Y) );
	m_String2Code.insert( PairString2Code("AXIS_MOUSE_Z",							AXIS_MOUSE_Z) );
	m_String2Code.insert( PairString2Code("AXIS_LEFT_THUMB_X",				AXIS_LEFT_THUMB_X) );
	m_String2Code.insert( PairString2Code("AXIS_LEFT_THUMB_Y",				AXIS_LEFT_THUMB_Y) );
	m_String2Code.insert( PairString2Code("AXIS_RIGHT_THUMB_X",				AXIS_RIGHT_THUMB_X) );
	m_String2Code.insert( PairString2Code("AXIS_RIGHT_THUMB_Y",				AXIS_RIGHT_THUMB_Y) );
	m_String2Code.insert( PairString2Code("AXIS_DELTA_TRIGGER_RIGHT",	AXIS_DELTA_TRIGGER_RIGHT) );
	m_String2Code.insert( PairString2Code("AXIS_DELTA_TRIGGER_LEFT",	AXIS_DELTA_TRIGGER_LEFT) );
	m_String2Code.insert( PairString2Code("AXIS_NOTHING",							AXIS_NOTHING) );
	
	//----------------INPUT_EVENT_TYPE-----------------//		
	m_String2Code.insert( PairString2Code("EVENT_DOWN",								EVENT_DOWN) );
	m_String2Code.insert( PairString2Code("EVENT_UP_DOWN",						EVENT_UP_DOWN) );
	m_String2Code.insert( PairString2Code("EVENT_DOWN_UP",						EVENT_DOWN_UP) );
	m_String2Code.insert( PairString2Code("EVENT_NOTHING",						EVENT_NOTHING) );

	//----------------MOUSE_BUTTON---------------------//
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_LEFT",				MOUSE_BUTTON_LEFT) );
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_RIGHT",				MOUSE_BUTTON_RIGHT) );
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_MIDDLE",			MOUSE_BUTTON_MIDDLE) );
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_MIDDLE",			MOUSE_BUTTON_MIDDLE) );
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_NOTHING",			MOUSE_BUTTON_NOTHING) );


	//----------------Gamepad codes from XInput.h------//
	m_String2Code.insert( PairString2Code("PAD_DPAD_UP",							PAD_DPAD_UP) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_DOWN",						PAD_DPAD_DOWN) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_LEFT",						PAD_DPAD_LEFT) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_RIGHT",						PAD_DPAD_RIGHT) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_START",						PAD_DPAD_START) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_BACK",						PAD_DPAD_BACK) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_LEFT_THUMB",		PAD_BUTTON_LEFT_THUMB) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_RIGHT_THUMB",		PAD_BUTTON_RIGHT_THUMB) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_LEFT_SHOULDER",	PAD_BUTTON_LEFT_SHOULDER) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_RIGHT_SHOULDER",PAD_BUTTON_RIGHT_SHOULDER) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_A",							PAD_BUTTON_A) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_B",							PAD_BUTTON_B) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_X",							PAD_BUTTON_X) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_Y",							PAD_BUTTON_Y) );

	//-----------Keyboard scan codes from dinput.h ------//
	m_String2Code.insert( PairString2Code("KEY_ESCAPE",								KEY_ESCAPE) );
	m_String2Code.insert( PairString2Code("KEY_TAB",									KEY_TAB) );
	m_String2Code.insert( PairString2Code("KEY_SPACE",								KEY_SPACE) );
	m_String2Code.insert( PairString2Code("KEY_RETURN",								KEY_RETURN) );
	m_String2Code.insert( PairString2Code("KEY_BACK",									KEY_BACK) );
	m_String2Code.insert( PairString2Code("KEY_CAPITAL",							KEY_CAPITAL) );
	m_String2Code.insert( PairString2Code("KEY_MINUS",								KEY_MINUS) );
	m_String2Code.insert( PairString2Code("KEY_EQUALS",								KEY_EQUALS) );
	m_String2Code.insert( PairString2Code("KEY_LBRACKET",							KEY_LBRACKET) );
	m_String2Code.insert( PairString2Code("KEY_RBRACKET",							KEY_RBRACKET) );
	m_String2Code.insert( PairString2Code("KEY_SEMICOLON",						KEY_SEMICOLON) );
	m_String2Code.insert( PairString2Code("KEY_APOSTROPHE",						KEY_APOSTROPHE) );
	m_String2Code.insert( PairString2Code("KEY_GRAVE",								KEY_GRAVE) );
	m_String2Code.insert( PairString2Code("KEY_BACKSLASH",						KEY_BACKSLASH) );
	m_String2Code.insert( PairString2Code("KEY_COMMA",								KEY_COMMA) );
	m_String2Code.insert( PairString2Code("KEY_PERIOD",								KEY_PERIOD) );
	m_String2Code.insert( PairString2Code("KEY_SLASH",								KEY_SLASH) );
	m_String2Code.insert( PairString2Code("KEY_A",										KEY_A) );
	m_String2Code.insert( PairString2Code("KEY_S",										KEY_S) );
	m_String2Code.insert( PairString2Code("KEY_D",										KEY_D) );
	m_String2Code.insert( PairString2Code("KEY_F",										KEY_F) );
	m_String2Code.insert( PairString2Code("KEY_G",										KEY_G) );
	m_String2Code.insert( PairString2Code("KEY_H",										KEY_H) );
	m_String2Code.insert( PairString2Code("KEY_J",										KEY_J) );
	m_String2Code.insert( PairString2Code("KEY_K",										KEY_K) );
	m_String2Code.insert( PairString2Code("KEY_L",										KEY_L) );
	m_String2Code.insert( PairString2Code("KEY_X",										KEY_X) );
	m_String2Code.insert( PairString2Code("KEY_C",										KEY_C) );
	m_String2Code.insert( PairString2Code("KEY_B",										KEY_B) );
	m_String2Code.insert( PairString2Code("KEY_N",										KEY_N) );
	m_String2Code.insert( PairString2Code("KEY_M",										KEY_M) );
	m_String2Code.insert( PairString2Code("KEY_Q",										KEY_Q) );
	m_String2Code.insert( PairString2Code("KEY_W",										KEY_W) );
	m_String2Code.insert( PairString2Code("KEY_E",										KEY_E) );
	m_String2Code.insert( PairString2Code("KEY_R",										KEY_R) );
	m_String2Code.insert( PairString2Code("KEY_T",										KEY_T) );
	m_String2Code.insert( PairString2Code("KEY_Y",										KEY_Y) );
	m_String2Code.insert( PairString2Code("KEY_U",										KEY_U) );
	m_String2Code.insert( PairString2Code("KEY_I",										KEY_I) );
	m_String2Code.insert( PairString2Code("KEY_O",										KEY_O) );
	m_String2Code.insert( PairString2Code("KEY_P",										KEY_P) );
	m_String2Code.insert( PairString2Code("KEY_1",										KEY_1) );
	m_String2Code.insert( PairString2Code("KEY_2",										KEY_2) );
	m_String2Code.insert( PairString2Code("KEY_3",										KEY_3) );
	m_String2Code.insert( PairString2Code("KEY_4",										KEY_4) );
	m_String2Code.insert( PairString2Code("KEY_5",										KEY_5) );
	m_String2Code.insert( PairString2Code("KEY_6",										KEY_6) );
	m_String2Code.insert( PairString2Code("KEY_7",										KEY_7) );
	m_String2Code.insert( PairString2Code("KEY_8",										KEY_8) );
	m_String2Code.insert( PairString2Code("KEY_9",										KEY_9) );
	m_String2Code.insert( PairString2Code("KEY_0",										KEY_0) );
	m_String2Code.insert( PairString2Code("KEY_F1",										KEY_F1) );
	m_String2Code.insert( PairString2Code("KEY_F2",										KEY_F2) );
	m_String2Code.insert( PairString2Code("KEY_F3",										KEY_F3) );
	m_String2Code.insert( PairString2Code("KEY_F4",										KEY_F4) );
	m_String2Code.insert( PairString2Code("KEY_F5",										KEY_F5) );
	m_String2Code.insert( PairString2Code("KEY_F6",										KEY_F6) );
	m_String2Code.insert( PairString2Code("KEY_F7",										KEY_F7) );
	m_String2Code.insert( PairString2Code("KEY_F8",										KEY_F8) );
	m_String2Code.insert( PairString2Code("KEY_F9",										KEY_F9) );
	m_String2Code.insert( PairString2Code("KEY_F10",									KEY_F10) );
	m_String2Code.insert( PairString2Code("KEY_F11",									KEY_F11) );
	m_String2Code.insert( PairString2Code("KEY_F12",									KEY_F12) );
	m_String2Code.insert( PairString2Code("KEY_NPPLUS",								KEY_NPPLUS) );
	m_String2Code.insert( PairString2Code("KEY_NPMINUS",							KEY_NPMINUS) );
	m_String2Code.insert( PairString2Code("KEY_NPDECIMAL",						KEY_NPDECIMAL) );
	m_String2Code.insert( PairString2Code("KEY_NPCOMMA",							KEY_NPCOMMA) );
	m_String2Code.insert( PairString2Code("KEY_NPDIVIDE",							KEY_NPDIVIDE) );
	m_String2Code.insert( PairString2Code("KEY_NPMULTIPLY",						KEY_NPMULTIPLY) );
	m_String2Code.insert( PairString2Code("KEY_NPENTER",							KEY_NPENTER) );
	m_String2Code.insert( PairString2Code("KEY_NUMLOCK",							KEY_NUMLOCK) );
	m_String2Code.insert( PairString2Code("KEY_NP1",									KEY_NP1) );
	m_String2Code.insert( PairString2Code("KEY_NP2",									KEY_NP2) );
	m_String2Code.insert( PairString2Code("KEY_NP3",									KEY_NP3) );
	m_String2Code.insert( PairString2Code("KEY_NP4",									KEY_NP4) );
	m_String2Code.insert( PairString2Code("KEY_NP5",									KEY_NP5) );
	m_String2Code.insert( PairString2Code("KEY_NP6",									KEY_NP6) );
	m_String2Code.insert( PairString2Code("KEY_NP7",									KEY_NP7) );
	m_String2Code.insert( PairString2Code("KEY_NP8",									KEY_NP8) );
	m_String2Code.insert( PairString2Code("KEY_NP9",									KEY_NP9) );
	m_String2Code.insert( PairString2Code("KEY_NP0",									KEY_NP0) );
	m_String2Code.insert( PairString2Code("KEY_RSHIFT",								KEY_RSHIFT) );
	m_String2Code.insert( PairString2Code("KEY_LSHIFT",								KEY_LSHIFT) );
	m_String2Code.insert( PairString2Code("KEY_RCTRL",								KEY_RCTRL) );
	m_String2Code.insert( PairString2Code("KEY_LCTRL",								KEY_LCTRL) );
	m_String2Code.insert( PairString2Code("KEY_RALT",									KEY_RALT) );
	m_String2Code.insert( PairString2Code("KEY_LALT",									KEY_LALT) );
	m_String2Code.insert( PairString2Code("KEY_LWIN",									KEY_LWIN) );
	m_String2Code.insert( PairString2Code("KEY_RWIN",									KEY_RWIN) );
	m_String2Code.insert( PairString2Code("KEY_UP",										KEY_UP) );
	m_String2Code.insert( PairString2Code("KEY_DOWN",									KEY_DOWN) );
	m_String2Code.insert( PairString2Code("KEY_LEFT",									KEY_LEFT) );
	m_String2Code.insert( PairString2Code("KEY_RIGHT",								KEY_RIGHT) );
	m_String2Code.insert( PairString2Code("KEY_INSERT",								KEY_INSERT) );
	m_String2Code.insert( PairString2Code("KEY_DELETE",								KEY_DELETE) );
	m_String2Code.insert( PairString2Code("KEY_HOME",									KEY_HOME) );
	m_String2Code.insert( PairString2Code("KEY_END",									KEY_END) );
	m_String2Code.insert( PairString2Code("KEY_PGDOWN",								KEY_PGDOWN) );
	m_String2Code.insert( PairString2Code("KEY_PGUP",									KEY_PGUP) );
	m_String2Code.insert( PairString2Code("KEY_PAUSE",								KEY_PAUSE) );
	m_String2Code.insert( PairString2Code("KEY_SCROLL",								KEY_SCROLL) );	
}

