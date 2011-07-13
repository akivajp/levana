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
    }
    return 0;
  }

}


extern int luaopen_lev(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  luabind::open(L);
  lua_entry(L);

  // base classes
  module(L, "lev")
  [
    namespace_("classes")
    [
      // base of all
      class_<base>("base")
        .def("__tostring", &base::tostring)
        .property("type_id", &base::get_type_id)
        .property("type_name", &base::get_type_name),
      // event class
      class_<event, base>("event")
        .def("request", &event::request)
        .def("skip", &event::skip)
        .property("id", &event::get_id)
        .property("key", &event::get_keystr)
        .property("keystr", &event::get_keystr)
        .property("keycode", &event::get_keycode),
      // event handler class
      class_<handler, base>("handler")
        .def("set_on_menu", &handler::set_on_menu)
        .property("on_any", &handler::get_on_any, &handler::set_on_any)
        .property("on_close", &handler::get_on_close, &handler::set_on_close)
        .property("on_idle", &handler::get_on_idle, &handler::set_on_idle)
        .property("on_keydown", &handler::get_on_keydown, &handler::set_on_keydown)
    ]
  ];
  object classes = globals(L)["lev"]["classes"];


  // GUI control, event handling
  module(L, "lev")
  [
    namespace_("gui")
    [
      def("file_selector", &gui::file_selector)
    ],
    namespace_("classes")
    [
      // base class
      class_<control, handler>("control")
        .def("hide", &control::hide)
        .def("show", &control::show)
        .property("exists", &control::is_valid)
        .property("id", &control::getid)
        .property("h", &control::get_height, &control::set_height)
        .property("height", &control::get_height, &control::set_height)
        .property("is_valid", &control::is_valid)
        .property("isvalid", &control::is_valid)
        .property("is_shown", &control::is_shown, &control::set_shown)
        .property("isshown", &control::is_shown, &control::set_shown)
        .property("sizer", &control::get_sizer, &control::set_sizer)
        .property("w", &control::get_width, &control::set_width)
        .property("width", &control::get_width, &control::set_width),
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
        .property("status", &frame::get_status, &frame::set_status)
        .property("title", &frame::gettitle, &frame::settitle)
        .scope
        [
          def("create_c", &frame::create, adopt(result) )
        ],
      class_<htmlview, control>("htmlview")
        .def("loadpage", &htmlview::loadpage)
        .def("totext", &htmlview::totext)
        .property("page", &htmlview::get_page, &htmlview::set_page)
        .scope
        [
          def("create_c", &htmlview::create, adopt(result))
        ],
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
      class_<menu, handler>("menu")
        .scope
        [
          def("create_c", &menu::create, adopt(result))
        ],
      class_<menubar, control>("menubar")
        .scope
        [
          def("create_c", &menubar::create, adopt(result))
        ],
      class_<systray, handler>("systray")
        .def("popup", &systray::popup)
        .def("remove_icon", &systray::remove_icon)
        .def("set_icon", &systray::set_icon)
        .property("menugen", &systray::get_menu_generator, &systray::set_menu_generator)
        .property("menu_generator", &systray::get_menu_generator, &systray::set_menu_generator)
        .scope
        [
          def("create_c", &systray::create, adopt(result))
        ],
      class_<textbox, control>("textbox")
        .def("set_multiline", &textbox::set_multiline)
        .property("v", &textbox::get_value, &textbox::set_value)
        .property("val", &textbox::get_value, &textbox::set_value)
        .property("value", &textbox::get_value, &textbox::set_value)
        .scope
        [
          def("create_c", &textbox::create)
        ]
    ]
  ];
  object gui = globals(L)["lev"]["gui"];
  register_to(L, gui, "msgbox", &gui::msgbox_l);
  register_to(L, classes["canvas"], "create", &canvas::create_l);
  register_to(L, classes["frame"], "create", &frame::create_l);
  register_to(L, classes["htmlview"], "create", &htmlview::create_l);
  register_to(L, classes["menu"], "create", &menu::create_l);
  register_to(L, classes["menubar"], "create", &menubar::create_l);
  register_to(L, classes["player"], "create", &player::create_l);
  register_to(L, classes["systray"], "create", &systray::create_l);
  register_to(L, classes["textbox"], "create", &textbox::create_l);
  gui["canvas"]   = classes["canvas"]["create"];
  gui["frame"]    = classes["frame"]["create"];
  gui["htmlview"] = classes["htmlview"]["create"];
  gui["menu"]     = classes["menu"]["create"];
  gui["menubar"]  = classes["menubar"]["create"];
  gui["player"]   = classes["player"]["create"];
  gui["systray"]  = classes["systray"]["create"];
  gui["textbox"]  = classes["textbox"]["create"];


  // Application management module
  module(L, "lev")
  [
    namespace_("classes")
    [
      class_<application, handler>("app")
        .def("autoloop", &application::autoloop)
        .def("autoloop", &application::autoloop_with)
        .def("get_keydown", &application::get_keydown)
        .def("sleep", &application::sleep)
        .def("sleep", &application::sleep_def)
        .def("track_key", &application::track_key)
        .def("track_mouse", &application::track_mouse)
        .def("wait",  &application::wait)
        .def("yield", &application::yield)
        .property("fps", &application::get_fps, &application::set_fps)
        .property("input", &application::get_instate)
        .property("inrec", &application::get_inrecord)
        .property("inrecord", &application::get_inrecord)
        .property("instate", &application::get_instate)
        .property("interval", &application::get_interval, &application::set_interval)
        .property("name", &application::get_name, &application::set_name)
        .property("title", &application::get_name, &application::set_name)
        .property("top",  &application::get_top,  &application::settop)
        .property("top_window",  &application::get_top,  &application::settop)
    ],
    namespace_("app")
    [
      def("get", &application::get_app)
    ]
  ];

  // sizers
  module(L, "lev")
  [
    namespace_("classes")
    [
      class_<sizer, base>("sizer")
        .def("fit", &sizer::fit)
        .def("fitinside", &sizer::fitinside)
        .def("layout", &sizer::layout),
      class_<gsizer, sizer>("gsizer")
        .scope
        [
          def("create_c", &gsizer::create, adopt(result))
        ],
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
  register_to(L, classes["gsizer"], "create", &gsizer::create_l);
  register_to(L, classes["hsizer"], "create", &hsizer::create_l);
  register_to(L, classes["vsizer"], "create", &vsizer::create_l);
  gui["gsizer"] = classes["gsizer"]["create"];
  gui["hsizer"] = classes["hsizer"]["create"];
  gui["vsizer"] = classes["vsizer"]["create"];


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
    def("get_app", &application::get_app)
  ];

  set_preloaders(L);
  luaopen_lev_input(L);
  luaopen_lev_prim(L);
//    register_to(L, globals(L)["package"]["preload"], "load_util", &util::luaopen_util);
//  register_to(L, globals(L)["lev"], "load_util", &util::luaopen_util);
  return 0;
}

int luaopen_lev_std(lua_State *L)
{
  using namespace luabind;
  using namespace lev;

  open(L);
  globals(L)["require"]("debug");
  globals(L)["require"]("io");
  globals(L)["require"]("math");
  globals(L)["require"]("os");
  globals(L)["require"]("string");
  globals(L)["require"]("table");

  globals(L)["require"]("lev");
  globals(L)["require"]("lev.gl");
  globals(L)["require"]("lev.image");
  globals(L)["require"]("lev.net");
  globals(L)["require"]("lev.sound");

  globals(L)["app"] = globals(L)["lev"]["app"]["get"]();
  globals(L)["mixer"] = globals(L)["lev"]["sound"]["mixer"]();
  globals(L)["collectgarbage"]();
  return 0;
}


namespace lev
{

  void set_preloaders(lua_State *L)
  {
    using namespace luabind;
    register_to(L, globals(L)["package"]["preload"], "lev", luaopen_lev);
    register_to(L, globals(L)["package"]["preload"], "lev.gl", luaopen_lev_gl);
    register_to(L, globals(L)["package"]["preload"], "lev.image", luaopen_lev_image);
    register_to(L, globals(L)["package"]["preload"], "lev.net", luaopen_lev_net);
    register_to(L, globals(L)["package"]["preload"], "lev.sound", luaopen_lev_sound);
    register_to(L, globals(L)["package"]["preload"], "lev.std", luaopen_lev_std);
  }

}

