/////////////////////////////////////////////////////////////////////////////
// Name:        src/draw.cpp
// Purpose:     source for drawing features
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/23/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/draw.hpp"
#include <wx/glcanvas.h>

namespace levana
{
  draw::draw(frame *parent, int width, int height)
  {
    try {
      wxFrame *p = NULL;
      if (parent) { p = (wxFrame *)parent->_obj; }
      _obj = new wxGLCanvas(p, -1, wxDefaultPosition, wxSize(width, height));
    }
    catch(...) {
      throw "draw: allocation error";
    }
  }

  void draw::use()
  {
    wxGLCanvas *gl = (wxGLCanvas *)_obj;
    gl->SetCurrent();
  }
}

