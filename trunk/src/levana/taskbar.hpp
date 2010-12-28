#ifndef _TASKBAR_HPP
#define _TASKBAR_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/taskbar.hpp
// Purpose:     header for taskbar handling class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "event.hpp"
#include "icon.hpp"
#include "menu.hpp"

namespace luabind { class object; }

namespace levana
{
  class systray : public ctrl
  {
    public:
      systray();
      ~systray();
      bool seticon(const icon& i, const char *tooltip = "");
      void setmenu(menu *m);
      static void setmenu(systray *st, menu *m);
      void setonmenu(int id, luabind::object lua_func);
  };
}

#endif // _TASKBAR_HPP

