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
        application::entry(argc, argv);
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
      application::entry(0, NULL);
      atexit(final_release);
    }
    return 0;
  }

}


static inline void register_to(lua_State *L, luabind::object to, const char *funcname, lua_CFunction func)
{
  lua_pushcfunction(L, func);
  luabind::object f(luabind::from_stack(L, -1));
  lua_pop(L, 1);
  to[funcname] = f;
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
          value("default", cfg::DEFAULT),
          value("fixed",   cfg::FIXED)
        ]
    ];
    register_to(L, globals(L)["levana"]["cfg"], "frame_style", &cfg::frame_style);

    // event class, and its derived ones
    module(L, "levana")
    [
      // base class
      class_<event>("event")
        .def("getkey", &event::getkey)
        .def("skip", &event::skip)
    ];

    // GUI control, event handling
    module(L, "levana")
    [
      // base class
      class_<control>("control")
        .def("connect", &control::connect)
        .def("exists", &control::exists)
        .def("setonkeydown", &control::setonkeydown)
        .def("setonmenu", &control::setonmenu),
      // derived classes
      class_<application, control>("application")
        .def(constructor<>())
        .def("autoloop", &application::autoloop)
        .def("msgbox", &application::msgbox)
        .def("msgbox", &application::msgbox_msg)
        .def("yield", &application::yield)
        .property("name", &application::getname, &application::setname)
        .property("top",  &application::gettop,  &application::settop),
      class_<draw, control>("draw")
        .def(constructor<frame*, int, int>())
        .def("clear", &draw::clear)
        .def("flush", &draw::flush)
        .def("line",  &draw::line)
        .def("set2d", &draw::set2d)
        .def("swap", &draw::swap)
        .def("using", &draw::use),
      class_<frame, control>("frame")
        .def(constructor<frame*, int, const char*, int, int, int, int,
                         long, const char*>())
        .def("close", &frame::close)
        .def("close", &frame::close_noforce)
        .def("fit", &frame::fit)
        .def("setmenubar", (void(frame::*)(menubar *))&frame::setmenubar)
        .def("show", &frame::show)
        .def("show", &frame::show_true)
        .property("status", &frame::getstatus, &frame::setstatus)
        .property("title", &frame::gettitle, &frame::settitle)
        .scope
        [
          def("setmenubar", (void(*)(frame*, menubar*))&frame::setmenubar)
        ],
      class_<htmlview, control>("htmlview")
        .def(constructor<control*, int, int>())
        .def("loadpage", &htmlview::loadpage)
        .def("setpage", &htmlview::setpage),
      class_<player, control>("player")
        .def(constructor<control*,int,const char*,int,int>())
        .def("load", &player::load)
        .def("play", &player::play)
        .def("playing", &player::playing),
      class_<menu, control>("menu")
        .def(constructor<>())
        .def(constructor<const char *>())
        .scope
        [
          def("append", &menu::append)
        ],
      class_<menubar, control>("menubar")
        .def(constructor<>())
        .scope
        [
          def("append", &menubar::append)
        ],
      class_<systray, control>("systray")
        .def(constructor<>())
        .def("seticon", &systray::seticon)
        .scope
        [
          def("setmenu", &systray::setmenu)
        ]
    ];

    module(L, "levana")
    [
      class_<icon>("icon")
        .def(constructor<>())
        .def(constructor<const char *>())
        .def("load_xpm", &icon::load_xpm)
        .scope
        [
          def("levana_icon", &icon::levana_icon)
        ]
    ];
    luaopen_gl(L);
    return 1;
  }
}

