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
        .def("setonkeydown", &control::setonkeydown)
        .def("setonmenu", &control::setonmenu)
        .property("isvalid", &control::isvalid)
        .property("sizer", &control::getsizer, &control::setsizer),
      // derived classes
      class_<application, control>("application")
        .def(constructor<>())
        .def("autoloop", &application::autoloop)
        .def("autoloop", &application::autoloop_with)
        .def("yield", &application::yield)
        .property("name", &application::getname, &application::setname)
        .property("top",  &application::gettop,  &application::settop)
        .scope
        [
          def("msgbox", &application::msgbox),
          def("msgbox", &application::msgbox_nocap)
        ],
      class_<draw, control>("draw")
        .def(constructor<frame*, int, int>())
        .def("clear", &draw::clear)
        .def("flush", &draw::flush)
        .def("line",  &draw::line)
        .def("set2d", &draw::set2d)
        .def("swap", &draw::swap)
        .def("using", &draw::use),
      class_<frame, control>("frame")
        .def(constructor<frame*, const char*, int, int, long>())
        .def("close", &frame::close)
        .def("close", &frame::close_noforce)
        .def("fit", &frame::fit)
        .def("show", &frame::show)
        .def("show", &frame::show_true)
        .property("status", &frame::getstatus, &frame::setstatus)
        .property("title", &frame::gettitle, &frame::settitle)
        .scope
        [
          def("setmenubar", &frame::setmenubar)
        ],
      class_<htmlview, control>("htmlview")
        .def(constructor<control*, int, int>())
        .def("loadpage", &htmlview::loadpage)
        .def("setpage", &htmlview::setpage)
        .def("totext", &htmlview::totext),
      class_<player, control>("player")
        .def(constructor<control*,int,int>())
        .def("loadlocal", &player::loadlocal)
        .def("loaduri", &player::loaduri)
        .def("play", &player::play)
        .property("bestsize", &player::getbestsize)
        .property("ispaused", &player::ispaused)
        .property("isplaying", &player::isplaying)
        .property("isstopped", &player::isstopped)
        .property("volume", &player::getvolume, &player::setvolume),
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
        ],
      class_<text, control>("text")
        .def(constructor<control*,int,int,const char*>())
        .property("value", &text::getvalue, &text::setvalue)
    ];

    // sizers
    module(L, "levana")
    [
      class_<sizer>("sizer")
        .def("fit", &sizer::fit)
        .def("fitinside", &sizer::fitinside)
        .def("layout", &sizer::layout)
        .scope
        [
          def("add", &sizer::addctrl),
          def("add", &sizer::addsizer),
          def("addspace", &sizer::addspace)
        ],
      class_<hsizer, sizer>("hsizer")
        .def(constructor<>()),
      class_<vsizer, sizer>("vsizer")
        .def(constructor<>())
    ];

    // primitives
    module(L, "levana")
    [
      class_<vector>("vector")
        .def(constructor<>())
        .def(constructor<int,int>())
        .def(constructor<int,int,int>())
        .def(self + vector())
        .property("x", &vector::getx, &vector::setx)
        .property("y", &vector::gety, &vector::sety)
        .property("z", &vector::getz, &vector::setz),
      class_<size>("size")
        .def(constructor<int,int>())
        .def(constructor<int,int,int>())
        .property("d", &size::geth, &size::setd)
        .property("depth", &size::geth, &size::setd)
        .property("h", &size::geth, &size::seth)
        .property("hehgt", &size::geth, &size::seth)
        .property("w", &size::getw, &size::setw)
        .property("width", &size::getw, &size::setw)
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

