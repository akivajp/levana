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
#include "lev/util.hpp"

#include <wx/glcanvas.h>
#include <wx/rawbmp.h>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

namespace lev
{

  class myCanvas : public wxGLCanvas
  {
    public:

      myCanvas(wxWindow *parent, int *attribList, int width, int height)
        : wxGLCanvas(parent, wxID_ANY, attribList, wxDefaultPosition, wxSize(width, height)),
          context(NULL)
      {}

      virtual ~myCanvas()
      {
        if (context) { delete context; }
      }

      bool SetCurrent()
      {
        if (context == NULL)
        {
          try
          {
            context = new wxGLContext(this);
          }
          catch (...)
          {
            return false;
          }
        }
        ((wxGLCanvas *)this)->SetCurrent(*context);
        return true;
      }

      wxGLContext *context;
  };

  canvas::~canvas() { }

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

  canvas* canvas::create(control *parent, int width, int height)
  {
    int attribs[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };
    canvas *cv = NULL;
    wxWindow *p = NULL;
    try
    {
      cv = new canvas();
      if (parent) { p = (wxWindow *)parent->get_rawobj(); }
      cv->_obj = new myCanvas(p, attribs, width, height);
    }
    catch (...)
    {
      delete cv;
      return NULL;
    }
    cv->wx_managed = true;
    cv->setcurrent();
    glViewport(0, 0, width, height);
    return cv;
  }

  int canvas::create_l(lua_State *L)
  {
    using namespace luabind;
    object p;
    int w = -1, h = -1;

    int n = lua_gettop(L);
    lua_pushcfunction(L, &util::merge);
    newtable(L).push(L);
    for (int i = 1; i <= n; i++)
    {
      object(from_stack(L, i)).push(L);
    }
    lua_call(L, n + 1, 1);
    object t(from_stack(L, -1));
    lua_remove(L, -1);

    if (t["parent"]) { p = t["parent"]; }
    else if (t["p"]) { p = t["p"]; }
    else if (t["udata"]) { p = t["udata"]; }

    if (t["width"]) { w = object_cast<int>(t["width"]); }
    else if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["num1"]) { w = object_cast<int>(t["num1"]); }

    if (t["height"]) { h = object_cast<int>(t["height"]); }
    else if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["num2"]) { h = object_cast<int>(t["num2"]); }

    object func = globals(L)["lev"]["gui"]["canvas"]["create_c"];
    object result = func(p, w, h);
    result.push(L);
    return 1;
  }

  bool canvas::draw_image(image *bmp, int x, int y)
  {
    wxBitmap *b = (wxBitmap*)bmp->_obj;
    wxAlphaPixelData pixels(*b);
    int w = bmp->get_w();
    int h = bmp->get_h();
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
    glDrawPixels(bmp->get_w(), bmp->get_h(), GL_RGBA, GL_UNSIGNED_BYTE, data);
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
    ((myCanvas *)_obj)->SetCurrent();
  }


  void canvas::swap()
  {
    ((wxGLCanvas *)_obj)->SwapBuffers();
  }
}

