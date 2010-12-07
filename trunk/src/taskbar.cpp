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
  systray::systray()
  {
    try {
      _obj = new wxTaskBarIcon();
    }
    catch (...) {
      throw "systray: allocation error";
    }
    this->set_icon(icon::levana_icon(), "Levana Application");
  }

  systray::~systray()
  {
    delete (wxTaskBarIcon *)_obj;
  }

  bool systray::set_icon(const icon& i, const char *tooltip)
  {
    wxTaskBarIcon *tray = (wxTaskBarIcon *)_obj;
    return tray->SetIcon(*((wxIcon*)i._obj), wxString(tooltip, wxConvUTF8));
  }
}

