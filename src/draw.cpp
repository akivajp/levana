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
#include "lev/draw.hpp"

#include <wx/glcanvas.h>
#include <wx/rawbmp.h>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

namespace lev
{
  canvas::canvas(control *parent, int width, int height) : control()
  {
    int attribs[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };
    wxWindow *p = NULL;
    if (parent) { p = (wxWindow *)parent->_obj; }
    try {
      wxGLCanvas *canvas = new wxGLCanvas(p, -1, wxDefaultPosition, wxSize(width, height),
                            0, wxGLCanvasName, attribs);
      _obj = canvas;
    }
    catch(...) {
      fprintf(stderr, "canvas: allocation error");
      return;
    }

    this->setcurrent();
    glViewport(0, 0, width, height);
  }


  void canvas::blendmode(bool enable)
  {
    if (enable)
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
      glDisable(GL_BLEND);
    }
  }


  void canvas::clear()
  {
    glClear(GL_COLOR_BUFFER_BIT);
  }


  void canvas::clearcolor(unsigned char r, unsigned char g, unsigned char b)
  {
    glClearColor(r / 255.0, g / 255.0, b / 255.0, 1.0);
  }


  bool canvas::drawbitmap(bitmap *bmp, int x, int y)
  {
    wxBitmap *b = (wxBitmap*)bmp->_obj.get();
    wxAlphaPixelData pixels(*b);
    int w = bmp->getw();
    int h = bmp->geth();
    unsigned char *data = new unsigned char [4 * w * h];
    if (!data) { return false; }
    wxAlphaPixelData::Iterator p(pixels);
    for (int i = 0; i < h; i++)
    {
      for (int j = 0; j < w; j++, p++)
      {
        int k = (h - i - 1) * w + j;
        data[4*k]     = p.Red();
        data[4*k + 1] = p.Green();
        data[4*k + 2] = p.Blue();
        data[4*k + 3] = p.Alpha();
      }
    }
    glRasterPos2i(x, y + h);
    glDrawPixels(bmp->getw(), bmp->geth(), GL_RGBA, GL_UNSIGNED_BYTE, data);
    delete[] data;
    return true;
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
    this->setcurrent();
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
  }


  void canvas::setcurrent()
  {
    ((wxGLCanvas *)_obj)->SetCurrent();
  }


  void canvas::swap()
  {
    ((wxGLCanvas *)_obj)->SwapBuffers();
  }
}

