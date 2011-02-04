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
#include "icon.hpp"
#include "menu.hpp"

namespace luabind { class object; }

namespace lev
{
  class systray : public control
  {
    public:
      systray();
      bool seticon(const icon& i, const char *tooltip = "");
      void setmenu(menu *m);
      void setonmenu(int id, luabind::object lua_func);
  };
}

#endif // _TASKBAR_HPP

