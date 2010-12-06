/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/icon.hpp
// Purpose:     Header for icon handling class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/icon.hpp"

#include "resource/levana.xpm"

namespace levana
{
  icon::icon()
  {
    _obj = NULL;
  }

  icon::icon(const char **bits)
  {
    try {
      _obj = new wxIcon(bits);
    }
    catch (...) {
      throw("icon: loading error");
    }
  }

  icon::~icon()
  {
    if (_obj) { delete (wxIcon *)_obj; }
  }

  bool icon::load_xpm(const char *filename)
  {
    if (_obj) { delete (wxIcon *)_obj; }
    _obj = new wxIcon(wxString(filename, wxConvUTF8), wxBITMAP_TYPE_XPM);
    if (_obj == NULL) { return false; }
    return true;
  }

  const icon& icon::levana_icon()
  {
    static icon levana(levana_xpm);
    return levana;
  }
}

