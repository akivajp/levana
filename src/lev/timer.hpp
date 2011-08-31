#ifndef _TIMER_HPP
#define _TIMER_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        lev/timer.hpp
// Purpose:     header for timer handling classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     08/26/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "event.hpp"
#include <luabind/luabind.hpp>

extern "C"
{
  int luaopen_lev_timer(lua_State *L);
}

namespace lev
{

  class timer : public handler
  {
    protected:
      timer();
    public:
      virtual ~timer();
      static timer* create();
      int get_interval();
      luabind::object get_notify();
      bool is_one_shot();
      bool is_running();
      bool set_notify(luabind::object func);
      bool start(int milliseconds, bool one_shot);
      bool start0() { return start(-1, false); }
      bool start1(int milliseconds) { return start(milliseconds, false); }
      bool stop();
  };

}

#endif // _TIMER_HPP

