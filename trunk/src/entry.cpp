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
        appli::entry(argc, argv);
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
      appli::entry(0, NULL);
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

    // constants
    module(L, "levana")
    [
      class_<cfg>("cfg")
        .enum_("constants")
        [
          value("default", levana::cfg::DEFAULT),
          value("fixed",   levana::cfg::FIXED)
        ]
    ];

    // application management
    module(L, "levana")
    [
      class_<appli>("appli")
        .def(constructor<>())
        .def("autoloop", &appli::autoloop)
        .def("msgbox", &appli::msgbox)
        .def("msgbox", &appli::msgbox_msg)
        .def("yield", &appli::yield)
        .property("name", &appli::getname, &appli::setname)
        .property("top",  &appli::gettop,  &appli::settop)
    ];

    // GUI control, event handling
    module(L, "levana")
    [
      // base class
      class_<ctrl>("ctrl")
        .def("connect", &ctrl::connect)
        .def("exists", &ctrl::exists)
        .def("setonmenu", &ctrl::setonmenu),
      // derived classes
      class_<draw, ctrl>("draw")
        .def(constructor<frame*, int, int>())
        .def("clear", &draw::clear)
        .def("flush", &draw::flush)
        .def("using", &draw::use),
      class_<frame, ctrl>("frame")
        .def(constructor<frame*, int, const char*, int, int, int, int,
                         long, const char*>())
        .def("close", &frame::close)
        .def("close", &frame::close_noforce)
        .def("fit", &frame::fit)
        .def("setmenubar", (void(frame::*)(menubar *))&frame::setmenubar)
        .def("show", &frame::show)
        .def("show", &frame::show_true)
        .property("title", &frame::gettitle, &frame::settitle)
        .scope
        [
          def("setmenubar", (void(*)(frame*, menubar*))&frame::setmenubar)
        ],
      class_<menu, ctrl>("menu")
        .def(constructor<>())
        .def(constructor<const char *>())
        .def("append", (int(menu::*)(int, const char*, const char *))&menu::append)
        .scope
        [
          def("append", (int(*)(menu*, int, const char*, const char *))&menu::append)
        ],
      class_<menubar, ctrl>("menubar")
        .def(constructor<>())
        .def("append", (bool(menubar::*)(menu*, const char *))&menubar::append)
        .scope
        [
          def("append", (bool(*)(menubar*, menu*, const char *))&menubar::append)
        ],
      class_<systray, ctrl>("systray")
        .def(constructor<>())
        .def("seticon", &systray::seticon)
        .def("setmenu", (void(systray::*)(menu*))&systray::setmenu)
        .scope
        [
          def("setmenu", (void(*)(systray*, menu*))&systray::setmenu)
        ]
    ];

    module(L, "levana")
    [
      class_<icon>("icon")
        .def(constructor<>())
        .def(constructor<const char *>())
        .def("levana_icon", &icon::levana_icon)
        .def("load_xpm", &icon::load_xpm)
    ];
    luaopen_gl(L);
    return 1;
  }
}

