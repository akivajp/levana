/////////////////////////////////////////////////////////////////////////////
// Name:        src/entry.cpp
// Purpose:     source for binding the C++ library to lua script
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/lev.hpp"
#include "register.hpp"

#include <sstream>
#include <boost/ref.hpp>
#include <luabind/adopt_policy.hpp>

void final_release()
{
}

namespace lev
{

  int lua_entry(lua_State *L)
  {
    using namespace luabind;
    object table = globals(L)["arg"];
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
        application::entry(L, argc, argv);
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
      application::entry(L, 0, NULL);
      atexit(final_release);
    }
    return 0;
  }

}


extern "C" {
  extern int luaopen_lev(lua_State *L)
  {
    using namespace lev;
    using namespace luabind;

    luabind::open(L);
    lua_entry(L);

    luabind::object lev = globals(L)["lev"] = newtable(L);
    // configuration
    register_to(L, lev, "load_cfg", &cfg::luaopen_cfg);
    // usable functions
    register_to(L, lev, "load_util", &util::luaopen_util);


    // base of all
    module(L, "base")
    [
      class_<base>("base")
        .property("type", &base::gettype)
//        .property("isvalid", &base::isvalid)
    ];


    // event class, and its derived ones
    module(L, "lev")
    [
      // base class
      class_<event, base>("event")
        .def("getkey", &event::getkey)
        .def("skip", &event::skip)
    ];


    // GUI control, event handling
    module(L, "lev")
    [
      // base class
      class_<control, base>("control")
        .def("connect", &control::connect)
        .def("hide", &control::hide)
        .def("setonkeydown", &control::setonkeydown)
        .def("setonmenu", &control::setonmenu)
        .def("show", &control::show)
        .property("id", &control::getid)
        .property("isvalid", &control::isvalid)
        .property("isshown", &control::isshown, &control::setshown)
        .property("sizer", &control::getsizer, &control::setsizer),
      // derived classes
      class_<application, control>("app")
        .def("autoloop", &application::autoloop)
        .def("autoloop", &application::autoloop_with)
        .def("sleep", &application::sleep)
        .def("yield", &application::yield)
        .property("name", &application::getname, &application::setname)
        .property("title", &application::getname, &application::setname)
        .property("top",  &application::gettop,  &application::settop)
        .property("topwindow",  &application::gettop,  &application::settop)
        .scope
        [
          def("get", &application::getapp),
          def("msgbox", &application::msgbox),
          def("msgbox", &application::msgbox_nocap)
        ],
      class_<canvas, control>("canvas")
        .def(constructor<frame*, int, int>())
        .def("blendmode", &canvas::blendmode)
        .def("clear", &canvas::clear)
        .def("clearcolor", &canvas::clearcolor)
        .def("draw", &canvas::drawbitmap)
        .def("flush", &canvas::flush)
        .def("line",  &canvas::line)
        .def("set2d", &canvas::set2d)
        .def("setcurrent", &canvas::setcurrent)
        .def("swap", &canvas::swap),
      class_<frame, control>("frame")
        .def("close", &frame::close)
        .def("close", &frame::close_noforce)
        .def("fit", &frame::fit)
        .property("status", &frame::getstatus, &frame::setstatus)
        .property("title", &frame::gettitle, &frame::settitle)
        .scope
        [
          def("new", &frame::create, adopt(result) ),
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
        .def("pause", &player::pause)
        .def("play", &player::play)
        .def("stop", &player::stop)
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
    module(L, "lev")
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
    module(L, "lev")
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
        .property("height", &size::geth, &size::seth)
        .property("w", &size::getw, &size::setw)
        .property("width", &size::getw, &size::setw)
    ];

    module(L, "lev")
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

    module(L, "lev")
    [
      class_<bitmap, base>("bitmap")
        .def(constructor<int,int>())
        .def(constructor<const char *>())
        .def("drawcircle", &bitmap::drawcircle)
        .def("drawtext", &bitmap::drawtext)
        .def("save", &bitmap::save)
        .property("h", &bitmap::geth)
        .property("height", &bitmap::geth)
        .property("isvalid", &bitmap::isvalid)
        .property("w", &bitmap::getw)
        .property("width", &bitmap::getw)
    ];

    module(L, "lev")
    [
      namespace_("sound")
      [
        class_<channel, base>("channel")
          .def("clean", &channel::clean)
          .def("load", &channel::load)
          .def("open", &channel::open)
          .def("pause", &channel::pause)
          .def("play", &channel::play)
          .property("id", &channel::get_id)
          .property("is_playing", &channel::get_playing, &channel::set_playing)
          .property("isplaying", &channel::get_playing, &channel::set_playing)
          .property("is_valid", &channel::is_valid)
          .property("isvalid", &channel::is_valid),
        class_<mixer, base>("mixer")
          .def("get_channel", &mixer::get_channel)
          .def("pause", &mixer::pause)
          .def("play", &mixer::play)
          .def("__call", &mixer::get_channel)
          .property("is_playing", &mixer::get_playing, &mixer::set_playing)
          .property("isplaying", &mixer::get_playing, &mixer::set_playing)
          .scope
          [
            def("create", &mixer::create, adopt(result))
          ],
        def("init", &sound::init),
        def("play", &sound::play),
        def("stop", &sound::stop)
      ]
    ];

    module(L, "lev")
    [
      def("get_app", &application::getapp)
    ];

    register_to(L, globals(L)["package"]["preload"], "gl", luaopen_gl);
//    application::getapp();
    return 1;
  }
}

