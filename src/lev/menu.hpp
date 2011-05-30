#ifndef _MENU_HPP
#define _MENU_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/frame.hpp
// Purpose:     header for menu and menubar handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/02/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "event.hpp"
#include <lua.h>
#include <luabind/luabind.hpp>

namespace lev
{

  class menu : public control
  {
    private:
      menu();
      ~menu();
    public:
      int append(int id, const char *str, const char *help_str = "");
      static int append_l(lua_State *L);
      static menu* create(const char *title);
      static int create_l(lua_State *L);
      friend class systray;
      friend class menubar;
  };

  class menubar : public control
  {
    private:
      menubar();
      ~menubar();
    public:
      bool append(menu *m, const char *title);
      static int append_l(lua_State *L);
      static menubar *create();
      static int create_l(lua_State *L);
      friend class frame;
  };

}

#endif // _MENU_HPP

