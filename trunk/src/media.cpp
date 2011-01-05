/////////////////////////////////////////////////////////////////////////////
// Name:        src/media.cpp
// Purpose:     source for media controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     01/05/2011
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/media.hpp"

namespace levana
{

  player::player(control *parent, int id, const char *uri, int width, int height) : control()
  {
    try {
      wxWindow *p = NULL;
      wxString file;
      if (parent) { p = (wxWindow *)parent->_obj; }
      if (uri == NULL) { file = wxEmptyString; }
      else { file = wxString(uri, wxConvUTF8); }
      _obj = new wxMediaCtrl(p, id, file, wxDefaultPosition, wxSize(width, height));
    }
    catch (...) {
      throw "control: allocation error";
    }
  }

  player::~player()
  {
    if (_obj)
    {
      delete (wxMediaCtrl *)_obj;
    }
  }

  bool player::load(const char *filename)
  {
    return ((wxMediaCtrl *)_obj)->Load(wxString(filename, wxConvUTF8));
  }

  bool player::play()
  {
    return ((wxMediaCtrl *)_obj)->Play();
  }

  bool player::playing()
  {
    if ( ((wxMediaCtrl *)_obj)->GetState() == wxMEDIASTATE_PLAYING)
    { return true; }
    return false;
  }

}

