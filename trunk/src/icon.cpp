/////////////////////////////////////////////////////////////////////////////
// Name:        src/icon.cpp
// Purpose:     source for icon handling class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/icon.hpp"

#include "resource/levana.xpm"

namespace lev
{
  icon::icon(const char **bits)
  {
    try {
      wxIcon *icon = new wxIcon(bits);
      _obj.reset(icon);
    }
    catch (...) {
      throw("icon: loading error");
    }
  }

  icon::icon(const char *filename)
  {
    // wxInitAllImageHandlers();
    try {
      wxIcon *icon = new wxIcon();
      wxImage img(wxString(filename, wxConvUTF8), wxBITMAP_TYPE_ANY);
      img.Rescale(25, 25);
      icon->CopyFromBitmap(img);
      _obj.reset(icon);
    }
    catch(...) {
      throw("icon: loading error");
    }
  }


  bool icon::load_xpm(const char *filename)
  {
    try {
      wxIcon *icon = new wxIcon(wxString(filename, wxConvUTF8), wxBITMAP_TYPE_XPM);
      _obj.reset(icon);
//    _obj = new wxIcon(wxString(filename, wxConvUTF8));
    }
    catch (...) {
      return false;
    }
    return true;
  }

  const icon& icon::levana_icon()
  {
    static icon levana(levana_xpm);
    return levana;
  }
}

