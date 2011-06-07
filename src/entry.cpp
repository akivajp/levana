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
#include "lev/sound.hpp"
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

    if (not globals(L)["lev"])
    {
      // base of all
      module(L, "lev")
      [
        class_<base>("base")
          .property("type_id", &base::get_type_id)
          .property("type_name", &base::get_type_name)
      ];
    }


    // event class, and its derived ones
    module(L, "lev")
    [
      // base class
      class_<event, base>("event")
        .def("request", &event::request)
        .def("skip", &event::skip)
        .property("id", &event::get_id)
        .property("key", &event::get_key)
    ];


    // GUI control, event handling
    module(L, "lev")
    [
      namespace_("gui")
      [
        def("msgbox", &application::msgbox),
        def("msgbox", &application::msgbox_nocap),
        // base class
        class_<control, base>("control")
          .def("connect", &control::connect)
          .def("hide", &control::hide)
          .def("set_onany", &control::set_onany)
          .def("set_onkeydown", &control::set_onkeydown)
          .def("set_onmenu", &control::set_onmenu)
          .def("show", &control::show)
          .property("id", &control::getid)
          .property("is_valid", &control::isvalid)
          .property("isvalid", &control::isvalid)
          .property("is_shown", &control::isshown, &control::setshown)
          .property("isshown", &control::isshown, &control::setshown)
          .property("onany", &control::get_onany, &control::set_onany)
          .property("onidle", &control::get_onidle, &control::set_onidle)
          .property("onkeydown", &control::get_onkeydown, &control::set_onkeydown)
          .property("sizer", &control::getsizer, &control::setsizer),
        class_<canvas, control>("canvas")
          .def("blendmode", &canvas::blendmode)
          .def("clear", &canvas::clear)
          .def("clearcolor", &canvas::clearcolor)
          .def("draw", &canvas::draw_image)
          .def("flush", &canvas::flush)
          .def("line",  &canvas::line)
          .def("set2d", &canvas::set2d)
          .def("setcurrent", &canvas::setcurrent)
          .def("swap", &canvas::swap)
          .scope
          [
            def("create_c", &canvas::create, adopt(result))
          ],
        class_<frame, control>("frame")
          .def("close", &frame::close)
          .def("close", &frame::close_noforce)
          .def("fit", &frame::fit)
          .property("status", &frame::getstatus, &frame::setstatus)
          .property("title", &frame::gettitle, &frame::settitle)
          .scope
          [
            def("create_c", &frame::create, adopt(result) )
          ],
        class_<htmlview, control>("htmlview")
          .def(constructor<control*, int, int>())
          .def("loadpage", &htmlview::loadpage)
          .def("setpage", &htmlview::setpage)
          .def("totext", &htmlview::totext),
        class_<player, control>("player")
          .def("loadlocal", &player::loadlocal)
          .def("loaduri", &player::loaduri)
          .def("pause", &player::pause)
          .def("play", &player::play)
          .def("stop", &player::stop)
          .property("bestsize", &player::getbestsize)
          .property("ispaused", &player::ispaused)
          .property("isplaying", &player::isplaying)
          .property("isstopped", &player::isstopped)
          .property("volume", &player::getvolume, &player::setvolume)
          .scope
          [
            def("create_c", &player::create, adopt(result))
          ],
        class_<menu, control>("menu")
          .scope
          [
            def("create_c", &menu::create, adopt(result))
          ],
        class_<menubar, control>("menubar")
          .scope
          [
            def("create_c", &menubar::create, adopt(result))
          ],
        class_<systray, control>("systray")
          .def("popup", &systray::popup)
          .def("set_icon", &systray::set_icon)
          .property("menu_generator", &systray::get_menu_generator, &systray::set_menu_generator)
          .scope
          [
            def("create_c", &systray::create, adopt(result))
          ],
        class_<textbox, control>("textbox")
          .property("value", &textbox::get_value, &textbox::set_value)
          .scope
          [
            def("create_c", &textbox::create)
          ]
      ]
    ];
    register_to(L, globals(L)["lev"]["gui"]["canvas"], "create", &canvas::create_l);
    register_to(L, globals(L)["lev"]["gui"]["frame"], "create", &frame::create_l);
    register_to(L, globals(L)["lev"]["gui"]["menu"], "create", &menu::create_l);
    register_to(L, globals(L)["lev"]["gui"]["menubar"], "create", &menubar::create_l);
    register_to(L, globals(L)["lev"]["gui"]["player"], "create", &player::create_l);
    register_to(L, globals(L)["lev"]["gui"]["systray"], "create", &systray::create_l);
    register_to(L, globals(L)["lev"]["gui"]["textbox"], "create", &textbox::create_l);

    // Application management module
    module(L, "lev")
    [
      // derived classes
      class_<application, control>("app")
        .def("autoloop", &application::autoloop)
        .def("autoloop", &application::autoloop_with)
        .def("get_keydown", &application::get_keydown)
        .def("run", &application::run)
        .def("run", &application::run_default)
        .def("sleep", &application::sleep)
        .def("yield", &application::yield)
        .property("name", &application::get_name, &application::set_name)
        .property("title", &application::get_name, &application::set_name)
        .property("top",  &application::get_top,  &application::settop)
        .property("topwindow",  &application::get_top,  &application::settop)
        .scope
        [
          def("get", &application::getapp)
        ]
    ];

    // sizers
    module(L, "lev")
    [
      namespace_("gui")
      [
        class_<sizer, base>("sizer")
          .def("fit", &sizer::fit)
          .def("fitinside", &sizer::fitinside)
          .def("layout", &sizer::layout),
        class_<hsizer, sizer>("hsizer")
          .scope
          [
            def("create_c", &hsizer::create, adopt(result))
          ],
        class_<vsizer, sizer>("vsizer")
          .scope
          [
            def("create_c", &vsizer::create, adopt(result))
          ]
      ]
    ];
    register_to(L, globals(L)["lev"]["gui"]["hsizer"], "create", &hsizer::create_l);
    register_to(L, globals(L)["lev"]["gui"]["vsizer"], "create", &vsizer::create_l);

    // primitives
    module(L, "lev")
    [
      class_<color>("color")
        .def(constructor<>())
        .def(constructor<unsigned char,unsigned char,unsigned char>())
        .def(constructor<unsigned char,unsigned char,unsigned char,unsigned char>())
        .def(constructor<wxUint32>())
        .property("a", &color::get_a, &color::set_a)
        .property("alpha", &color::get_a, &color::set_a)
        .property("b", &color::get_b, &color::set_b)
        .property("blue", &color::get_b, &color::set_b)
        .property("g", &color::get_g, &color::set_g)
        .property("green", &color::get_g, &color::set_g)
        .property("r", &color::get_r, &color::set_r)
        .property("red", &color::get_r, &color::set_r),
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
      def("get_app", &application::getapp)
    ];

    register_to(L, globals(L)["package"]["preload"], "gl", luaopen_gl);
//    register_to(L, globals(L)["package"]["preload"], "load_util", &util::luaopen_util);
    register_to(L, globals(L)["lev"], "load_util", &util::luaopen_util);

    luaopen_image(L);
    luaopen_sound(L);
    return 0;
  }
}

