#include "InputManager.h"
#include <string>
#include <map>

CInputManager *CInputManager::myInputManager = 0;


//--------CONSTRUCTOR & DESTRUCTOR------------------//
CInputManager::CInputManager()
: m_keyboard( true )
, m_mouse( true )
, m_debug_mode( true )
{}

CInputManager::~CInputManager()
{}
//----------------------------------------------//


//--------------SINGLETON FUNCTIONS------------------//
void CInputManager::CleanUP()
{
	if( myInputManager != NULL )
	{
		m_mouseInput.FreeDirectInput();
		m_keyboardInput.FreeDirectInput();
		
		delete myInputManager;
		myInputManager = NULL;
	}	
}

CInputManager *CInputManager::GetInstance()
{
	if( myInputManager == NULL )
	{
		myInputManager = new CInputManager;
	};

	return myInputManager;
}
//----------------------------------------------------//



//---------CREATE & READ INPUT FUNCTIONS-------------//
//Creamos los devices para los inputs Mouse y Keyboard
void CInputManager::OnCreateDevice(HWND hWnd)
{
	if(m_keyboard) //Si esta activado el flag utilizar keyboard => creamos su device
	{
		m_keyboardInput.OnCreateDevice(hWnd);
		InitString2IntDik();
	}
	if(m_mouse) //Si esta activado el flag utilizar mouse => creamos su device
	{
		m_mouseInput.OnCreateDevice(hWnd);
		InitIdInputMouse();
	}	
}

// A cada pasada del bucle Update hemos de llamar a esta funcion para tener
//actualizados los estados actuales del Mouse y Keyboard
void CInputManager::ReadInput(HWND hWnd)
{
	if(m_keyboard)
	{
		m_keyboardInput.ReadImmediateData(hWnd);
	}
	if(m_mouse)
	{
		m_mouseInput.ReadImmediateData(hWnd);
	}
}
//-----------------------------------------------//



bool CInputManager::DoAction(std::string action_name)
{
	float dummy;
	return DoAction(action_name, dummy);

}

// Con esta funcion comprobamos si el input relacionado con la accion "action_name" esta siendo 
//activado por el usuario. De ser asi retornamos true, en caso contrario false.
// El segundo parametro es de salida, y lo rellenaremos de informacion cuando el input a comprobar
//sea la posicion X,Y o Z del Mouse.
bool CInputManager::DoAction(std::string action_name, float &amount)
{
	InfoInput info;
	bool is_doAction = false;
	
	//Comprobamos si la accion "action_name" la tenemos mapeada con algun input
	it_mapInput = m_mapInput.find( action_name );
	
	//La primera condicion del if--> que exista mapeada la accion con un input
	//La segunda condicion--> Que la accion sea ejecutable en modo no debug o de serlo que estemos en modo debug
	if( (it_mapInput != m_mapInput.end()) && (m_debug_mode || !it_mapInput->second.debug )  )
	{
		info = it_mapInput->second;
		if( (info.type == MOUSE) && m_mouse )
		{
			switch(info.id_inputMouse)
			{
				case MOUSE_X:	amount = m_mouseInput.GetPosX();
								is_doAction = ( amount != 0);
								break;
				case MOUSE_Y:	amount = m_mouseInput.GetPosY();			
								is_doAction = ( amount != 0);
								break;
				case MOUSE_Z:	amount = m_mouseInput.GetPosZ();			
								is_doAction = ( amount != 0);
								break;
				case MOUSE_BL:	is_doAction = m_mouseInput.GetButonLeft();
								break;
				case MOUSE_BR:	is_doAction =  m_mouseInput.GetButonRight();
								break;
				case MOUSE_BM:	is_doAction =  m_mouseInput.GetButonMiddle();
								break;
			}
		}
		else if( (info.type == KEYBOARD) && m_keyboard )
		{
			char *keys;
		   	keys = m_keyboardInput.GetKey();
			if( ( KEYDOWN(keys, info.id_inputKey)) )
				is_doAction = true;
			else 
				is_doAction  = false;
		}
	}
	return is_doAction;
}




//-------------PARSERFILE FUNCTIONS---------------------------//
void CInputManager::Load (std::string xmlSoundsFile)
{
	//Read the xml input file
	xmlParseFile(xmlSoundsFile);
}



// Esta funcion permite leer de un fichero XML las asociaciones de accion-input que pueden 
//haber en el juego. Definiendo para cada una de ellas su tipo, el nombre de la accion con
//el que se llamara en el codigo, el codigo del input y un flag indicando si solo se ha 
//de ejecutar en modo debug.
// Aparte se define si en tiempo de ejecucion estaremos en modo debug respecto el input

// Un ejemplo de fichero XML podria ser el siguiente:

//<InputManager>
//	<Debug_mode value="yes"/> 
//	<KEYBOARD	name_action="move_left"		name_input="DIK_A"		debug="not"/>	
//	<KEYBOARD	name_action="changeDepth-"	name_input="DIK_B"		debug="yes"/>
//	<MOUSE		name_action="changeGun"		name_input="MOUSE_BL"	debug="not"/>
//<InputManager/>
void CInputManager::onStartElement( const std::string &elem, MKeyValue &atts )
{
	if( elem == "Debug_mode" )
    {
		std::string s_value	= atts["value"];
		if(s_value.compare("yes") == 0)
			m_debug_mode = true;
		else
			m_debug_mode = false;
	}
	
	
	if( elem == "KEYBOARD" )
    {
		std::string s_name_action	= atts["name_action"];
		std::string s_name_input	= atts["name_input"];
		std::string s_debug			= atts["debug"];
		
		int input;
		std::map<std::string, int>::iterator it;
		if( (it=m_translateStr2IntDik.find(s_name_input)) != m_translateStr2IntDik.end() )
			input = it->second;
		else return; //Si no existe la tecla no hacemos nada.

		InfoToPrint new_infoInput;
		new_infoInput.debug_mode = s_debug;
		new_infoInput.name_action = s_name_action;
        new_infoInput.name_input = s_name_input;
		new_infoInput.type = "KEYBOARD";
		m_vecInfoInput.push_back(new_infoInput);
			
		InfoInput info;
		info.type = KEYBOARD;
		info.debug = (s_debug.compare("yes")==0);
		info.id_inputKey = input;
		info.id_inputMouse = NOTHING;

		m_mapInput.insert(Input_Pair(s_name_action.c_str(),info));
    }

    if( elem == "MOUSE" )
    {
		std::string s_name_action	= atts["name_action"];
		std::string s_name_input	= atts["name_input"];
		std::string s_debug			= atts["debug"];
		
		InfoToPrint new_infoInput;
		new_infoInput.debug_mode = s_debug;
		new_infoInput.name_action = s_name_action;
		new_infoInput.name_input = s_name_input;
		new_infoInput.type = "MOUSE";
		m_vecInfoInput.push_back(new_infoInput);

		InfoInput info;
		info.type = MOUSE;	
		if(s_debug.compare("yes")==0)
			info.debug = true;
		else
			info.debug = false;

		info.id_inputKey = -1;
		
		std::map<std::string, TypeInputMouse>::iterator it;
		if( (it=m_translateStr2TInputMouse.find(s_name_input)) != m_translateStr2TInputMouse.end() )
			info.id_inputMouse = it->second;
		else return; //Si no existe el input mouse no hacemos nada.

		m_mapInput.insert(Input_Pair(s_name_action.c_str(),info));
    }
}
//------------------------------------------------------//

void CInputManager::InitIdInputMouse( void )
{
	typedef std::pair<std::string, TypeInputMouse> PairString2TInputMouse;
	m_translateStr2TInputMouse.insert( PairString2TInputMouse("MOUSE_X",MOUSE_X) );
	m_translateStr2TInputMouse.insert( PairString2TInputMouse("MOUSE_Y",MOUSE_Y) );
	m_translateStr2TInputMouse.insert( PairString2TInputMouse("MOUSE_Z",MOUSE_Z) );
	m_translateStr2TInputMouse.insert( PairString2TInputMouse("MOUSE_BL",MOUSE_BL) );
	m_translateStr2TInputMouse.insert( PairString2TInputMouse("MOUSE_BM",MOUSE_BM) );
	m_translateStr2TInputMouse.insert( PairString2TInputMouse("MOUSE_BR",MOUSE_BR) );
}
void CInputManager::InitString2IntDik( void )
{
	typedef std::pair<std::string, int> PairString2IntDIK;

	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_A",DIK_A) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_B",DIK_B) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_C",DIK_C) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_D",DIK_D) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_E",DIK_E) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_F",DIK_F) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_G",DIK_G) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_H",DIK_H) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_I",DIK_I) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_J",DIK_J) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_K",DIK_K) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_L",DIK_L) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_M",DIK_M) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_N",DIK_N) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_O",DIK_O) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_P",DIK_P) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_Q",DIK_Q) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_R",DIK_R) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_S",DIK_S) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_T",DIK_T) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_U",DIK_U) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_V",DIK_V) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_W",DIK_W) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_X",DIK_X) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_Y",DIK_Y) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_Z",DIK_Z) );

	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_0",DIK_0) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_1",DIK_1) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_2",DIK_2) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_3",DIK_3) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_4",DIK_4) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_5",DIK_5) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_6",DIK_6) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_7",DIK_7) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_8",DIK_8) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_9",DIK_9) );

	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_F1",DIK_F1) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_F2",DIK_F2) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_F3",DIK_F3) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_F4",DIK_F4) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_F5",DIK_F5) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_F6",DIK_F6) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_F7",DIK_F7) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_F8",DIK_F8) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_F9",DIK_F9) );

	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_UP",DIK_UP) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_DOWN",DIK_DOWN) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_LEFT",DIK_LEFT) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_RIGHT",DIK_RIGHT) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_TAB",DIK_TAB) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_ADD",DIK_ADD) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_SUBTRACT",DIK_SUBTRACT) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_ESCAPE",DIK_ESCAPE) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_TAB",DIK_TAB) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_RETURN",DIK_RETURN) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_RCONTROL",DIK_RCONTROL) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_LCONTROL",DIK_LCONTROL) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_LSHIFT",DIK_LSHIFT) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_RSHIFT",DIK_RSHIFT) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_BACKSLASH",DIK_BACKSLASH) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_COMMA",DIK_COMMA) );
	m_translateStr2IntDik.insert( PairString2IntDIK("DIK_SPACE",DIK_SPACE) );

}








