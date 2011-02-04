/////////////////////////////////////////////////////////////////////////////
// Name:        src/taskbar.cpp
// Purpose:     source for taskbar handling class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/taskbar.hpp"
#include "connect.hpp"

namespace lev
{
  class myTaskBarIcon : public wxTaskBarIcon
  {
    public:
      myTaskBarIcon() : wxTaskBarIcon() { menu = NULL; }
      void SetRClickMenu(wxMenu *m) { this->menu = m; }
    private:
      // Common Connect Interface
      DECLARE_CONNECT();
    private:
      wxMenu *menu;
      virtual wxMenu *CreatePopupMenu() { return menu; }
  };


  systray::systray() : control()
  {
    try {
      myTaskBarIcon *systray = new myTaskBarIcon();
      _obj.reset(systray);
      seticon(icon::levana_icon(), "Levana Application");
    }
    catch (...) {
      throw "systray: allocation error";
    }
  }


  bool systray::seticon(const icon& i, const char *tooltip)
  {
    myTaskBarIcon *tray = (myTaskBarIcon *)_obj.get();
    return tray->SetIcon(*((wxIcon*)i._obj.get()), wxString(tooltip, wxConvUTF8));
  }

  void systray::setmenu(menu *m)
  {
    myTaskBarIcon *tray = (myTaskBarIcon *)_obj.get();
    tray->SetRClickMenu((wxMenu *)m->_obj.get());
  }

  void systray::setonmenu(int id, luabind::object lua_func)
  {
    ((myTaskBarIcon *)_obj.get())->Connect(id, wxEVT_COMMAND_MENU_SELECTED, lua_func);
  }
}

