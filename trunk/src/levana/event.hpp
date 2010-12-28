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
  class handler
  {
    protected:
      inline handler() {}
    public:
      virtual void connect(int type, luabind::object lua_func)
      { throw "derived handler::connect is not yet implemented"; }
      virtual void setonmenu(int id, luabind::object lua_func)
      { throw "derived handler::setonmenu is not yet implemented"; }
  };
}

#endif // _EVENT_HPP

