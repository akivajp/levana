/////////////////////////////////////////////////////////////////////////////
// Name:        src/taskbar.cpp
// Purpose:     source for taskbar handling class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/taskbar.hpp"
#include "connect.hpp"

namespace levana
{
  class myTaskBarIcon : public wxTaskBarIcon
  {
    public:
      myTaskBarIcon() : wxTaskBarIcon() { menu = NULL; }
      void SetRClickMenu(wxMenu *m) { this->menu = m; }

      // Common Connect Interface
      inline void Connect(int id, wxEventType eventType, luabind::object lua_func)
      {
        levana::Connect(this, id, eventType, lua_func);
      }
      void ProcEvent(wxEvent &event)
      {
        levana::ProcEvent(this, event);
      }
      func_map fmap;
    private:
      wxMenu *menu;
      virtual wxMenu *CreatePopupMenu() { return menu; }
  };

  systray::systray() : control()
  {
    try {
      _obj = new myTaskBarIcon();
    }
    catch (...) {
      throw "systray: allocation error";
    }
    this->seticon(icon::levana_icon(), "Levana Application");
  }

  systray::~systray()
  {
    delete (myTaskBarIcon *)_obj;
  }

  bool systray::seticon(const icon& i, const char *tooltip)
  {
    myTaskBarIcon *tray = (myTaskBarIcon *)_obj;
    return tray->SetIcon(*((wxIcon*)i._obj), wxString(tooltip, wxConvUTF8));
  }

  void systray::setmenu(menu *m)
  {
    myTaskBarIcon *tray = (myTaskBarIcon *)_obj;
    tray->SetRClickMenu((wxMenu *)m->_obj);
  }

  void systray::setonmenu(int id, luabind::object lua_func)
  {
    ((myTaskBarIcon *)_obj)->Connect(id, wxEVT_COMMAND_MENU_SELECTED, lua_func);
  }
}

