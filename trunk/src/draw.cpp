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
  draw::draw(ctrl *parent, int width, int height)
  {
    try {
      wxWindow *p = NULL;
      if (parent) { p = (wxWindow *)parent->_obj; }
      _obj = new wxGLCanvas(p, -1, wxDefaultPosition, wxSize(width, height));
    }
    catch(...) {
      throw "draw: allocation error";
    }
    this->use();
    glViewport(0, 0, width, height);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1.0, 1.0);
  }

  void draw::clear()
  {
//    ((wxGLCanvas *)_obj)->SetCurrent();
//    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
//    glFlush();
  }

  void draw::flush()
  {
    glFlush();
  }

  void draw::use()
  {
    ((wxGLCanvas *)_obj)->SetCurrent();
  }
}

