/////////////////////////////////////////////////////////////////////////////
// Name:        src/entry.cpp
// Purpose:     source for binding the C++ library to lua script
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/levana.hpp"

#include <sstream>

namespace levana
{

  int lua_entry(luabind::object table)
  {
    int argc = 0;
    char **argv = NULL;
    if (luabind::type(table) == LUA_TTABLE)
    {
      try {
        int j = 0;
        for (luabind::iterator i(table), end; i != end; i++) { argc++; }
        argv = new char*[argc];
        for (luabind::iterator i(table), end; i != end; i++)
        {
          std::stringstream ss;
          int len;
          ss << *i;
          len = ss.str().length();
          argv[j] = new char[len + 1];
          strcpy(argv[j++], ss.str().c_str());
        }
        wxEntryStart(argc, argv);
        for (int i = 0; i < argc; i++) { delete argv[i]; }
        delete argv;
      }
      catch(...) {
        printf("Initialization failed\n");
        exit(-1);
      }
    }
    else
    {
      // entry with the arguments
      wxEntryStart(argc, argv);
      app::init();
    }
    return 0;
  }

}

extern "C" {
  extern int luaopen_levana(lua_State *L)
  {
    using namespace levana;
    using namespace luabind;

    luabind::open(L);
    module(L, "levana")
    [
      namespace_("app")
      [
        def("autoloop", &app::autoloop),
        def("get_name", &app::get_name),
        def("get_top", &app::get_top),
        def("msgbox", &app::msgbox),
        def("yield", &app::yield),
        def("set_name", &app::set_name),
        def("set_top", &app::set_top)
      ],
      class_<frame>("frame")
        .def(constructor<>())
        .def(constructor<int, const char*, int, int, int, int,
                         long, const char*>())
        .def("close", &frame::close)
        .def("connect_menu", &frame::connect_menu)
        .def("create", &frame::create)
        .def("get_title", &frame::get_title)
        .def("set_menubar", &frame::set_menubar)
        .def("set_title", &frame::set_title)
        .def("show", &frame::show),
      class_<menu>("menu")
        .def(constructor<const char *>())
        .def("append", &menu::append),
      class_<menubar>("menubar")
        .def(constructor<>())
        .def("append", &menubar::append)
    ];
    lua_entry(globals(L)["arg"]);
    return 1;
  }
}

