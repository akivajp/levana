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

#include "gui.hpp"
#include <lua.h>
#include <luabind/luabind.hpp>

namespace lev
{

  class menu : public handler
  {
    private:
      menu();
    public:
      ~menu();
      int append(int id, const char *str, const char *help_str = "");
      static int append_l(lua_State *L);
      static menu* create(const char *title);
      static int create_l(lua_State *L);
      virtual type_id get_type_id() const { return LEV_TMENU; }
      virtual const char *get_type_name() const { return "lev.gui.menu"; }
      friend class systray;
      friend class menubar;
  };

  class menubar : public control
  {
    private:
      menubar();
    public:
      ~menubar();
      bool append(menu *m, const char *title);
      static int append_l(lua_State *L);
      static menubar *create();
      static int create_l(lua_State *L);
      virtual type_id get_type_id() const { return LEV_TMENUBAR; }
      virtual const char *get_type_name() const { return "lev.gui.menubar"; }
      friend class frame;
  };

}

#endif // _MENU_HPP

