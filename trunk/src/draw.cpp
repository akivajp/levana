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
  canvas::canvas(control *parent, int width, int height) : control()
  {
    int attribs[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };
    try {
      wxWindow *p = NULL;
      if (parent) { p = (wxWindow *)parent->_obj; }
      _obj = new wxGLCanvas(p, -1, wxDefaultPosition, wxSize(width, height),
                            0, wxGLCanvasName, attribs);
    }
    catch(...) {
      throw "canvas: allocation error";
    }
    this->use();
    glViewport(0, 0, width, height);
  }

  void canvas::clear()
  {
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void canvas::flush()
  {
    glFlush();
  }

  void canvas::line(int x1, int y1, int x2, int y2)
  {
    glBegin(GL_LINES);
      glVertex2i(x1, y1);
      glVertex2i(x2, y2);
    glEnd();
  }

  void canvas::set2d()
  {
    int w, h;
    ((wxGLCanvas *)_obj)->GetSize(&w, &h);
    this->use();
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
  }

  void canvas::swap()
  {
    ((wxGLCanvas *)_obj)->SwapBuffers();
  }

  void canvas::use()
  {
    ((wxGLCanvas *)_obj)->SetCurrent();
  }
}

