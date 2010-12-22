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

namespace levana
{
  class myTaskBarIcon : public wxTaskBarIcon
  {
    public:
      myTaskBarIcon() : wxTaskBarIcon() { menu = NULL; }
      void SetRClickMenu(wxMenu *m) { this->menu = m; }
    private:
      wxMenu *menu;
      virtual wxMenu *CreatePopupMenu() { return menu; }
  };

  systray::systray()
  {
    try {
      _obj = new myTaskBarIcon();
    }
    catch (...) {
      throw "systray: allocation error";
    }
//    this->set_icon(icon::levana_icon(), "Levana Application");
  }

  systray::~systray()
  {
    delete (myTaskBarIcon *)_obj;
  }

  bool systray::set_icon(const icon& i, const char *tooltip)
  {
    myTaskBarIcon *tray = (myTaskBarIcon *)_obj;
    return tray->SetIcon(*((wxIcon*)i._obj), wxString(tooltip, wxConvUTF8));
  }

  void systray::set_menu(menu *m)
  {
    myTaskBarIcon *tray = (myTaskBarIcon *)_obj;
    tray->SetRClickMenu((wxMenu *)m->_obj);
  }
}

