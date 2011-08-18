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

#include "deriver.hpp"
#include "lev/draw.hpp"
#include "lev/util.hpp"
#include "register.hpp"

#include <wx/glcanvas.h>
#include <wx/rawbmp.h>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

int luaopen_lev_draw(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  open(L);
  globals(L)["require"]("lev");
  globals(L)["require"]("lev.gui");

  module(L, "lev")
  [
    namespace_("gui"),
    namespace_("classes")
    [
      class_<canvas, control>("canvas")
        .def("clear", &canvas::clear)
        .def("clear", &canvas::clear_color)
        .def("draw_image", &canvas::draw_image)
        .def("draw_image", &canvas::draw_image1)
        .def("draw_image", &canvas::draw_image3)
        .def("enable_alpha_blending", &canvas::enable_alpha_blending0)
        .def("enable_alpha_blending", &canvas::enable_alpha_blending)
        .def("flush", &canvas::flush)
        .def("line",  &canvas::line)
        .def("map2d", &canvas::map2d)
        .def("map2d", &canvas::map2d_auto)
        .def("set_current", &canvas::set_current)
        .def("swap", &canvas::swap)
        .scope
        [
          def("create_c", &canvas::create, adopt(result))
        ]
    ]
  ];
  object lev = globals(L)["lev"];
  object gui = lev["gui"];
  object classes = lev["classes"];

  register_to(classes["canvas"], "create", &canvas::create_l);
  register_to(classes["canvas"], "draw", &canvas::draw_l);
  gui["canvas"] = classes["canvas"]["create"];

  globals(L)["package"]["loaded"]["lev.draw"] = true;
  return 0;
}

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
  static wxGLCanvas* cast_draw(void *obj) { return (wxGLCanvas *)obj; }

  canvas::~canvas() { }


  void canvas::clear()
  {
    glClear(GL_COLOR_BUFFER_BIT);
  }


  void canvas::clear_color(unsigned char r, unsigned char g, unsigned char b)
  {
//    glClearColor(r / 255.0, g / 255.0, b / 255.0, 1.0);
    glClearColor(r / 255.0, g / 255.0, b / 255.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
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
    cv->system_managed = true;
    cv->set_current();
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

    object func = globals(L)["lev"]["classes"]["canvas"]["create_c"];
    object result = func(p, w, h);
    result.push(L);
    return 1;
  }

  bool canvas::draw_image(image *bmp, int x, int y, unsigned char alpha)
  {
    unsigned char *data = NULL;

    try {
      const int w = bmp->get_w();
      const int h = bmp->get_h();
      wxBitmap *b = (wxBitmap*)bmp->get_rawobj();
      wxAlphaPixelData pixels(*b);
      pixels.UseAlpha();
      wxAlphaPixelData::Iterator p(pixels), rawStart;

      data = new unsigned char [4 * w * h];
      for (int i = 0 ; i < h ; i++)
      {
        rawStart = p;
        for (int j = 0 ; j < w ; j++)
        {
          int k = (h - i - 1) * w + j;
          data[4*k]     = p.Red();
          data[4*k + 1] = p.Green();
          data[4*k + 2] = p.Blue();
          data[4*k + 3] = (alpha / 255.0) * p.Alpha();
          p++;
        }
        p = rawStart;
        p.OffsetY(pixels, 1);
      }
      glRasterPos2i(x, y + h);
      glDrawPixels(bmp->get_w(), bmp->get_h(), GL_RGBA, GL_UNSIGNED_BYTE, data);
      delete[] data;
      return true;
    }
    catch (...) {
      delete data;
      return false;
    }
  }


  int canvas::draw_l(lua_State *L)
  {
    using namespace luabind;
    canvas *cv = NULL;
    image *img = NULL;
    int x = 0, y = 0;
    unsigned char alpha = 255;

    luaL_checktype(L, 1, LUA_TUSERDATA);
    cv = object_cast<canvas *>(object(from_stack(L, 1)));
    object t = util::get_merged(L, 2, -1);

    if (t["udata1"])
    {
      object o = t["udata1"];
      switch(object_cast<int>(o["type_id"])) {
        case LEV_TIMAGE:
          img = object_cast<image *>(o);
          break;
        default:
          lua_pushboolean(L, false);
          return 1;
      }
    }

    if (img)
    {
      if (t["x"]) { x = object_cast<int>(t["x"]); }
      else if (t["num1"]) { x = object_cast<int>(t["num1"]); }

      if (t["y"]) { y = object_cast<int>(t["y"]); }
      else if (t["num2"]) { y = object_cast<int>(t["num2"]); }

      if (t["alpha"]) { alpha = object_cast<int>(t["alpha"]); }
      else if (t["a"]) { alpha = object_cast<int>(t["a"]); }
      else if (t["num3"]) { alpha = object_cast<int>(t["num3"]); }

      cv->draw_image(img, x, y, alpha);
      lua_pushboolean(L, true);
      return 1;
    }
  }


  int canvas::draw_points(lua_State *L)
  {
    using namespace luabind;

    int n = lua_gettop(L);
    luaL_checktype(L, 1, LUA_TUSERDATA);
    glBegin(GL_POINTS);
    for (int i = 2; i <= n; i++)
    {
      object o(from_stack(L, i));
      if (type(o) == LUA_TUSERDATA && o["type_id"] == LEV_TVECTOR)
      {
      }
    }
    glEnd();
  }


  bool canvas::enable_alpha_blending(bool enable)
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

  bool canvas::map2d_auto()
  {
    int w, h;
    ((wxGLCanvas *)_obj)->GetSize(&w, &h);
    this->set_current();
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
    return true;
  }

  bool canvas::map2d(int left, int right, int top, int bottom)
  {
    this->set_current();
    glLoadIdentity();
    glOrtho(left, right, bottom, top, -1, 1);
    return true;
  }

  void canvas::set_current()
  {
    ((myCanvas *)_obj)->SetCurrent();
  }

  void canvas::swap()
  {
    ((wxGLCanvas *)_obj)->SwapBuffers();
  }
}

