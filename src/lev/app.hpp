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
#include "input.hpp"
#include <luabind/luabind.hpp>

namespace lev
{

  class application : public control
  {
    protected:
      application();
      ~application();
    public:
      void autoloop();
      void autoloop_with(frame *frm);
      static bool entry(lua_State *L, int argc, char **argv);
      bool get_keydown(const char *key);
      inrecord* get_inrecord();
      instate* get_instate();
      const char* get_name();
      virtual luabind::object get_onany();
      virtual luabind::object get_onidle();
      virtual luabind::object get_onkeydown();
      frame *get_top();
      virtual type_id get_type_id() const { return LEV_TAPP; }
      virtual const char *get_type_name() const { return "app"; }
      void mainloop();
      bool run(bool sync = true);
      bool run_default() { run(true); }
      void set_name(const char *name);
      virtual bool set_onany(luabind::object lua_func);
      virtual bool set_onidle(luabind::object lua_func);
      virtual bool set_onkeydown(luabind::object lua_func);
      void settop(frame *top);
      bool sleep(int delay_in_msec);
      bool sleep_def() { sleep(0); }
      bool yield();

      static application *getapp();
      static lua_State* getL();
      static int msgbox(const char *msg, const char *caption);
      static inline int msgbox_nocap(const char *msg) { return msgbox(msg, "Message"); }
  };

  class gui_lock
  {
    public:
      gui_lock();
      ~gui_lock();
  };
}

#endif // _APP_HPP

