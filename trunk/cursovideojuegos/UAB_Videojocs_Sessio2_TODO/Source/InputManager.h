#ifndef INC_INPUTMANAGER_H_
#define INC_INPUTMANAGER_H_

#include <d3dx9.h>
#include <vector>
#include <map>
#include "XMLParser.h"
#include "MouseInput.h"
#include "KeyboardInput.h"
#define KEYDOWN(name, key) (name[key] & 0x80) 

//------------------Definicion de nuevos tipos---------------------//
typedef enum {MOUSE, KEYBOARD} TypeInput;
typedef enum {MOUSE_X, MOUSE_Y, MOUSE_Z, MOUSE_BL, MOUSE_BR,MOUSE_BM, NOTHING} TypeInputMouse;

//Esta estructura contiene la informacion del XML y la utilizaremos para visualizarla
//en tiempo de ejecucion como HELP
struct InfoToPrint
{
	std::string		name_action;	//Nombre de la accion a ejecutar con el input
	std::string		name_input;		//Input asociado a la accion
	std::string		debug_mode;		//Indica si el input es solo en modo de debug
	std::string		type;			//Indica el tipo de input (MOUSE o KEYBOARD)
};

//Esta estructura contiene la información de un input
struct InfoInput
{
	TypeInput		type;			//KEYBOARD o MOUSE
	bool			debug;			//Indica si la accion solo se ejecutara en modo debug
	int				id_inputKey;	//Codigo (directInput) de la tecla, si es mouse = -1
	TypeInputMouse	id_inputMouse;	//Codigo del mouse, si es key = NOTHING
};

typedef std::vector<InfoToPrint>					VecInfoInput; 

typedef std::pair<std::string, InfoInput>			Input_Pair;
typedef std::map<std::string, InfoInput>			MapInput;
typedef std::map<std::string, InfoInput>::iterator	It_MapInput;

typedef std::map<std::string, int>					MapString2IntDik;
typedef std::map<std::string, TypeInputMouse>		MapString2TypeInputMouse;
//--------------------------------------------------------------------//


class CInputManager: public CXMLParser
{

public:
	
	CInputManager(); 
	~CInputManager();  
	

	//--- SINGLETON FUNCTIONS ---
	static CInputManager *	GetInstance		( void );
	void					CleanUP			( void );

	//--- Obtener la info del XML para pintarla en tiempo de ejecucion
	const VecInfoInput &	GetPrintInfo	( void ) { return m_vecInfoInput; }

	//--- Funcion para inicializar el map de conversion de String al codigo de teclas del DirectInput
	void	InitString2IntDik				( void );

	//--- Funcion para inicializar el map de conversion de String al codigo del mouse TypeInputMouse
	void	InitIdInputMouse				( void );

	//--- Crear los device para los inputs de entrada (Mouse y Keyboard)
	void	OnCreateDevice	( HWND hWnd );

	//--- Leer los datos de los inputs de entrada (Mouse y Keyboard)
	void	ReadInput		( HWND hWnd );

	//--- Mira si el input relacionado con la accion "action_name" esta siendo activado por el usuario
	bool	DoAction		( std::string action_name, float &amount);
	bool	DoAction		( std::string action_name );

	//--- Lee la informacion de configuracion de un archivo XML
	void	Load			(std::string xmlSoundsFile);

	CMouseInput&	GetMouse	( void ) {return m_mouseInput;}
	CKeyboardInput&	GetKeyboard	( void ) {return m_keyboardInput;}
	

private:	
	
	bool			m_keyboard;		//Indica si el input Keyboard esta activado o no
	bool			m_mouse;		//Indica si el input Mouse esta activado o no
	bool			m_debug_mode;	//Indica si han de ejecutarse las acciones que son solo para debug

	CMouseInput		m_mouseInput;		//Objeto con la informacion relacionada con el Mouse
	CKeyboardInput	m_keyboardInput;	//Objeto con la informacion relacionada con el Keyboard

	VecInfoInput	m_vecInfoInput;	//Contiene toda la info del XML de configuracion, para poder ser pintada en tiempo de ejecucion
	MapInput		m_mapInput;		//Contiene la info de dado el identificador de accion que input tiene relacionado
	It_MapInput		it_mapInput;	//iterador del map anterior (utilizada para recorrer el mapa)

	MapString2IntDik	m_translateStr2IntDik;				//Mapa para la conversion de String al codigo de teclas DirectInput
	MapString2TypeInputMouse	m_translateStr2TInputMouse;	//Mapa de conversion de String al codigo del mouse TypeInputMouse

	void	onStartElement	(const std::string &elem, MKeyValue &atts);
	static CInputManager*	myInputManager;
};

#endif //INC_INPUTMANAGER_H_