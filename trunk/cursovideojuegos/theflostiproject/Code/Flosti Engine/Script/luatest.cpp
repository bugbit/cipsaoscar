#include "__PCH_Script.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <iostream>
#include <string>


int f()
{
  std::cout << "f()" << std::endl;
  return 92;
}

class CPerson
{
public:
  CPerson(){}
  ~CPerson() {}
  const char* GetName   () const { return m_szName.c_str(); }
  void        SetName   (const char* _pszName) { m_szName = _pszName; }
  void        PrintName ()  { std::cout << "The name of the person is: " << m_szName; }
private:
  std::string m_szName;
};

struct A
{
  A() { std::cout << "A()" << std::endl; }
  ~A() { std::cout << "~A()" << std::endl; }

  void blah() { std::cout << "A::blah" << std::endl; }
};

int main()
{
  lua_State* L = lua_open(); // abrimos un interpretador
  luaopen_base(L);           // abrimos la biblioteca base
  luaopen_math(L);           // abrimos la biblioteca de mate
  luabind::open(L);          // requerido para luabind

  using namespace luabind;

  // ahora registramos lo que querramos
  module(L)
    [
      // registramos la clase A
      class_<A>("A")
      // registramos su constructor
      .def(constructor<>())
      // registramos su funcion blah()
      .def("blah", &A::blah)
      ,
      // registramos la funcion global f
      def("f", &f),

      class_<CPerson>("Person")
      .def(constructor<>())
      .property("name", &CPerson::GetName, &CPerson::SetName)
      .def("PrintName", &CPerson::PrintName)
    ];

  // ejecutamos un script, llamado lua_test.lua
  lua_dofile(L, "lua_test.lua");

  lua_close(L);
  return 0;
}


/*
luabind::def("getInstance", Object::getInstance), //Get instance should be a static or free function anyway...
luabind::class_<Object>("Object")
    .def("print", &Object::print)

--Note how we have to call member functions with the : operator.
--Static functions, however, can be called with operator .
obj = getInstance();
obj:print();
*/