#ifndef _APP_HPP
#define _APP_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/app.hpp
// Purpose:     header for application managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "event.hpp"
#include "frame.hpp"
#include "locale.hpp"
#include "input.hpp"
#include <luabind/luabind.hpp>

extern "C" {
  int luaopen_lev_app(lua_State *L);
}

namespace lev
{

  class application : public handler
  {
    protected:
      application();
      ~application();
    public:
      void autoloop();
      void autoloop_with(frame *frm);
      static bool entry(lua_State *L, int argc, char **argv);
      static application *get_app();
      bool check_key_down(const char *key);
      double get_fps();
      double get_interval();
      inrecord* get_inrecord();
      instate* get_instate();
      locale* get_locale();
      std::string get_name();
      unsigned long get_process_id();
      luabind::object get_tick();
      frame *get_top();
      virtual type_id get_type_id() const { return LEV_TAPP; }
      virtual const char *get_type_name() const { return "lev.app"; }
      void mainloop();
      bool on_tick();
      bool run();
      bool set_fps(double fps);
      bool set_interval(double interval_msec);
      bool set_name(const std::string &name);
      bool set_tick(luabind::object func);
      void set_top(frame *top);
      bool sleep(int delay_in_msec);
      bool sleep_def() { sleep(0); }
      bool track_key(const char *keystr);
      bool track_mouse();
      bool wait();
      bool yield();

      static lua_State* getL();
      static int msgbox(const char *msg, const char *caption);
      static inline int msgbox1(const char *msg) { return msgbox(msg, "Message"); }
    protected:
      luabind::object tick;
  };

  class gui_lock
  {
    public:
      gui_lock();
      ~gui_lock();
  };
}

#endif // _APP_HPP

