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
      const char* getname();
      frame *gettop();
      void setname(const char *name);
      void setonkeydown(luabind::object lua_func);
      void settop(frame *top);
      bool sleep(int delay_in_msec);
//      void wait(int delay_in_msec);
      bool yield();
      // static method
      static bool entry(lua_State *L, int argc, char **argv);
      static application *getapp();
      static lua_State* getL();
      static int msgbox(const char *msg, const char *caption);
      static inline int msgbox_nocap(const char *msg) { return msgbox(msg, "Message"); }

    private:
  };
}

#endif // _APP_HPP

