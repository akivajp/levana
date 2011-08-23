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
        .property("char", &event::get_char)
        .property("id", &event::get_id)
        .property("key", &event::get_keystr)
        .property("keystr", &event::get_keystr)
        .property("keycode", &event::get_keycode),
      // event handler class
      class_<handler, base>("handler")
        .def("get_on_menu", &handler::get_on_menu)
        .def("set_on_menu", &handler::set_on_menu)
        .property("on_any", &handler::get_on_any, &handler::set_on_any)
        .property("on_char", &handler::get_on_char, &handler::set_on_char)
        .property("on_close", &handler::get_on_close, &handler::set_on_close)
        .property("on_idle", &handler::get_on_idle, &handler::set_on_idle)
        .property("on_keydown", &handler::get_on_keydown, &handler::set_on_keydown)
    ]
  ];
  object classes = globals(L)["lev"]["classes"];

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
        .property("locale", &application::get_locale)
        .property("name", &application::get_name, &application::set_name)
        .property("title", &application::get_name, &application::set_name)
        .property("top",  &application::get_top,  &application::set_top)
        .property("top_window",  &application::get_top,  &application::set_top)
    ],
    namespace_("app")
    [
      def("get", &application::get_app)
    ]
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
    def("get_app", &application::get_app)
  ];

  globals(L)["package"]["loaded"]["lev"] = globals(L)["lev"];
  set_preloaders(L);
  luaopen_lev_db(L);
  luaopen_lev_fs(L);
  luaopen_lev_gui(L);
  luaopen_lev_input(L);
  luaopen_lev_locale(L);
  luaopen_lev_prim(L);
  luaopen_lev_util(L);
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
  globals(L)["require"]("lev.draw");
  globals(L)["require"]("lev.font");
  globals(L)["require"]("lev.gl");
  globals(L)["require"]("lev.image");
  globals(L)["require"]("lev.info");
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
    register_to(globals(L)["package"]["preload"], "lev", luaopen_lev);
    register_to(globals(L)["package"]["preload"], "lev.db", luaopen_lev_db);
    register_to(globals(L)["package"]["preload"], "lev.draw", luaopen_lev_draw);
    register_to(globals(L)["package"]["preload"], "lev.font", luaopen_lev_font);
    register_to(globals(L)["package"]["preload"], "lev.fs", luaopen_lev_fs);
    register_to(globals(L)["package"]["preload"], "lev.gl", luaopen_lev_gl);
    register_to(globals(L)["package"]["preload"], "lev.gui", luaopen_lev_gui);
    register_to(globals(L)["package"]["preload"], "lev.image", luaopen_lev_image);
    register_to(globals(L)["package"]["preload"], "lev.info", luaopen_lev_info);
    register_to(globals(L)["package"]["preload"], "lev.input", luaopen_lev_input);
    register_to(globals(L)["package"]["preload"], "lev.locale", luaopen_lev_locale);
    register_to(globals(L)["package"]["preload"], "lev.net", luaopen_lev_net);
    register_to(globals(L)["package"]["preload"], "lev.sound", luaopen_lev_sound);
    register_to(globals(L)["package"]["preload"], "lev.std", luaopen_lev_std);
    register_to(globals(L)["package"]["preload"], "lev.util", luaopen_lev_util);
  }

}

