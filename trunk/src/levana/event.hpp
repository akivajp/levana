#ifndef _EVENT_HPP
#define _EVENT_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/connect.cpp
// Purpose:     header for event call back handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/28/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include <map>

namespace luabind { class object; }

namespace levana
{
  class ctrl : public base
  {
    protected:
      inline ctrl() : base(), id(0) {}
      inline ~ctrl() {}
    public:
      virtual void connect(int type, luabind::object lua_func)
      { throw "ctrl: derived handler::connect has not been implemented"; }
      bool exists();
      virtual void setonmenu(int id, luabind::object lua_func)
      { throw "ctrl: derived handler::setonmenu has not been implemented"; }
      friend class draw;
      friend class frame;
      friend class sizer;
    private:
      int id;
  };
}

#endif // _EVENT_HPP

