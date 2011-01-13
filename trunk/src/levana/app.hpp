#ifndef _APP_HPP
#define _APP_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/app.hpp
// Purpose:     header for application managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "event.hpp"
#include "frame.hpp"
#include <luabind/luabind.hpp>

namespace levana
{
  class application : public control
  {
    public:
      application();
      ~application();
      void autoloop();
      void autoloop_with(frame *frm);
      const char* getname();
      frame *gettop();
      void setname(const char *name);
      void setonkeydown(luabind::object lua_func);
      void settop(frame *top);
      bool yield();
      // static method
      static bool entry(lua_State *L, int argc, char **argv);
      static lua_State* getL();
      static int msgbox(const char *msg, const char *caption);
      static inline int msgbox_nocap(const char *msg) { return msgbox(msg, "Message"); }
  };
}

#endif // _APP_HPP

