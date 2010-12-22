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

void final_release()
{
}

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
        app::entry(argc, argv);
        atexit(final_release);
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
      app::entry(0, NULL);
      atexit(final_release);
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
    lua_entry(globals(L)["arg"]);
    module(L, "levana")
    [
      class_<app>("app")
        .def(constructor<>())
        .def("autoloop", &app::autoloop)
        .def("msgbox", (int(app::*)(const char*))&app::msgbox)
        .def("msgbox", (int(app::*)(const char*, const char*))&app::msgbox)
        .def("yield", &app::yield)
        .property("name", &app::name_get, &app::name_set)
        .property("top",  &app::top_get,  &app::top_set),
      class_<frame>("frame")
        .def(constructor<>())
        .def(constructor<frame*, int, const char*, int, int, int, int,
                         long, const char*>())
        .def("close", (bool(frame::*)(void))&frame::close)
        .def("close", (bool(frame::*)(bool))&frame::close)
        .def("connect_menu", &frame::connect_menu)
        .def("get_title", &frame::get_title)
        .def("set_menubar", &frame::set_menubar)
        .def("set_title", &frame::set_title)
        .def("show", (bool(frame::*)(void))&frame::show)
        .def("show", (bool(frame::*)(bool))&frame::show),
      class_<icon>("icon")
        .def(constructor<>())
        .def(constructor<const char *>())
        .def("levana_icon", &icon::levana_icon)
        .def("load_xpm", &icon::load_xpm),
      class_<menu>("menu")
        .def(constructor<>())
        .def(constructor<const char *>())
        .def("append", (int(menu::*)(int, const char*, const char *))&menu::append)
        .scope
        [
          def("append", (int(*)(menu*, int, const char*, const char *))&menu::append)
        ],
      class_<menubar>("menubar")
        .def(constructor<>())
        .def("append", &menubar::append),
      class_<systray>("systray")
        .def(constructor<>())
        .def("set_icon", &systray::set_icon)
        .def("set_menu", &systray::set_menu)
    ];
//    lua_entry(globals(L)["arg"]);
    return 1;
  }
}

