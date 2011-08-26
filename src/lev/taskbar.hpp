#ifndef _TASKBAR_HPP
#define _TASKBAR_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/taskbar.hpp
// Purpose:     header for taskbar handling class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "event.hpp"
#include "menu.hpp"

#include <lua.h>
#include <luabind/luabind.hpp>

namespace lev
{

  class systray : public handler
  {
    private:
      systray();
    public:
      virtual ~systray();
      static systray* create();
      static int create_l(lua_State *L);
      luabind::object get_menu_generator();
      virtual type_id get_type_id() const { return LEV_TSYSTRAY; }
      virtual const char *get_type_name() const { return "lev.gui.systray"; }
      bool popup(menu *m);
      bool remove_icon();
      bool set_icon(image *i, const char *tooltip = "");
      bool set_menu_generator(luabind::object lua_func);
  };

}

#endif // _TASKBAR_HPP

