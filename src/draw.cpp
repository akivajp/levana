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
  globals(L)["require"]("lev.gui");

  module(L, "lev")
  [
    namespace_("gui"),
    namespace_("classes")
    [
      class_<canvas, control>("canvas")
        .def("clear", &canvas::clear)
        .def("clear", &canvas::clear_color)
        .def("compile", &canvas::compile)
        .def("compile", &canvas::compile1)
        .def("draw_image", &canvas::draw_image)
        .def("draw_point", &canvas::draw_point)
        .def("enable_alpha_blending", &canvas::enable_alpha_blending0)
        .def("enable_alpha_blending", &canvas::enable_alpha_blending)
        .def("flush", &canvas::flush)
        .def("line",  &canvas::line)
        .def("map2d", &canvas::map2d)
        .def("map2d", &canvas::map2d_auto)
        .property("on_right_down", &canvas::get_on_right_down, &canvas::set_on_right_down)
        .def("print", &canvas::print)
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

      void Connect(int id, wxEventType eventType, luabind::object lua_func)
      {
        fmap[eventType][id] = lua_func;
        wxGLCanvas::Connect(id, eventType, (wxObjectEventFunction)&myCanvas::ProcEvent);
      }

      boost::function<void (int, int, luabind::object)> GetConnector()
      {
        return boost::bind(&myCanvas::Connect, this, _1, _2, _3);
      }

      luabind::object GetFunc(int id, wxEventType eventType)
      {
        return fmap[eventType][id];
      }

      boost::function<luabind::object (int, int)> GetFuncGetter()
      {
        return boost::bind(&myCanvas::GetFunc, this, _1, _2);
      }

      void OnRightDown(wxMouseEvent &e)
      {
        if (func_right_down) { func_right_down(event(&e)); }
        else { e.Skip(); }
      }

      void ProcEvent(wxEvent &evt)
      {
        event e(&evt);

        luabind::object o = fmap[evt.GetEventType()][-1];
        if (o && luabind::type(o) == LUA_TFUNCTION)
        {
          o(e);
          return;
        }

        o = fmap[evt.GetEventType()][evt.GetId()];
        if (o && luabind::type(o) == LUA_TFUNCTION)
        {
          o(e);
          return;
        }

        evt.Skip();
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

      std::map<int, std::map<int, luabind::object> > fmap;
      wxGLContext *context;
      luabind::object func_right_down;

      DECLARE_EVENT_TABLE();
  };
  BEGIN_EVENT_TABLE(myCanvas, wxGLCanvas)
    EVT_RIGHT_DOWN(myCanvas::OnRightDown)
  END_EVENT_TABLE();

  static myCanvas* cast_draw(void *obj) { return (myCanvas *)obj; }

  canvas::canvas() : control() { }

  canvas::~canvas()
  {
  }

  bool canvas::call_compiled(image *img)
  {
    return img->call_compiled(this);
  }

  void canvas::clear()
  {
    set_current();
    glClear(GL_COLOR_BUFFER_BIT);
  }


  void canvas::clear_color(unsigned char r, unsigned char g, unsigned char b)
  {
    set_current();
//    glClearColor(r / 255.0, g / 255.0, b / 255.0, 1.0);
    glClearColor(r / 255.0, g / 255.0, b / 255.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  bool canvas::compile(image *img, bool force)
  {
    if (! this->is_valid()) { return false; }
    return img->compile(this, force);
  }

  canvas* canvas::create(control *parent, int width, int height)
  {
    int attribs[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };
    canvas *cv = NULL;
    wxWindow *p = NULL;
    myCanvas *obj = NULL;
    try
    {
      cv = new canvas();
      if (parent) { p = (wxWindow *)parent->get_rawobj(); }
      cv->_obj = obj = new myCanvas(p, attribs, width, height);
      cv->_id = obj->GetId();
      cv->connector = obj->GetConnector();
      cv->func_getter = obj->GetFuncGetter();

      cv->system_managed = true;
      cv->set_current();
      glViewport(0, 0, width, height);
      return cv;
    }
    catch (...)
    {
      delete cv;
      return NULL;
    }
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
    else if (t["lua.userdata1"]) { p = t["lua.userdata1"]; }

    if (t["width"]) { w = object_cast<int>(t["width"]); }
    else if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["lua.number1"]) { w = object_cast<int>(t["lua.number1"]); }

    if (t["height"]) { h = object_cast<int>(t["height"]); }
    else if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["lua.number2"]) { h = object_cast<int>(t["lua.number2"]); }

    object func = globals(L)["lev"]["classes"]["canvas"]["create_c"];
    object result = func(p, w, h);
    result.push(L);
    return 1;
  }

  bool canvas::draw_image(image *bmp, int x, int y)
  {
    if (! this->is_valid()) { return false; }
    if (bmp->is_compiled())
    {
      set_current();
      glRasterPos2i(x, y + bmp->get_h());
      return bmp->call_compiled(this);
    }

    try {
      boost::shared_array<unsigned char> data;
      const int w = bmp->get_w();
      const int h = bmp->get_h();
      wxBitmap *b = (wxBitmap*)bmp->get_rawobj();
      wxAlphaPixelData pixels(*b);
      pixels.UseAlpha();
      wxAlphaPixelData::Iterator p(pixels), rawStart;

      data.reset(new unsigned char [4 * w * h]);
      for (int i = 0 ; i < h ; i++)
      {
        rawStart = p;
        for (int j = 0 ; j < w ; j++)
        {
          int k = (h - i - 1) * w + j;
          data[4*k]     = p.Red();
          data[4*k + 1] = p.Green();
          data[4*k + 2] = p.Blue();
          data[4*k + 3] = p.Alpha();
          p++;
        }
        p = rawStart;
        p.OffsetY(pixels, 1);
      }

      set_current();
      glRasterPos2i(x, y + h);
      glDrawPixels(bmp->get_w(), bmp->get_h(), GL_RGBA, GL_UNSIGNED_BYTE, data.get());
      return true;
    }
    catch (...) {
      return false;
    }
  }

  int canvas::draw_l(lua_State *L)
  {
    using namespace luabind;
    canvas *cv = NULL;

    luaL_checktype(L, 1, LUA_TUSERDATA);
    cv = object_cast<canvas *>(object(from_stack(L, 1)));
    object t = util::get_merged(L, 2, -1);

    if (t["lev.image"])
    {
      image *img = object_cast<image *>(t["lev.image"]);
      int x = 0, y = 0;

      if (t["x"]) { x = object_cast<int>(t["x"]); }
      else if (t["lua.number1"]) { x = object_cast<int>(t["lua.number1"]); }

      if (t["y"]) { y = object_cast<int>(t["y"]); }
      else if (t["lua.number2"]) { y = object_cast<int>(t["lua.number2"]); }

      cv->draw_image(img, x, y);
      lua_pushboolean(L, true);
      return 1;
    }
    if (t["lev.prim.point"])
    {
      point *pt = object_cast<point *>(t["lev.prim.point"]);
      cv->draw_point(pt);
      lua_pushboolean(L, true);
      return 1;
    }
  }

  bool canvas::draw_point(point *pt)
  {
    set_current();

    glPushMatrix();
    glBegin(GL_POINTS);
    {
      color *c = pt->get_color();
      vector *v = pt->get_vertex();
      if (c)
      {
        glColor4ub(c->get_r(), c->get_g(), c->get_b(), c->get_a());
      }
      glVertex3i(v->get_x(), v->get_y(), v->get_z());
    }
    glEnd();
    glPopMatrix();

    return true;
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
    set_current();
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

  luabind::object canvas::get_on_right_down()
  {
    return cast_draw(_obj)->func_right_down;
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
    set_current();
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
    return true;
  }

  bool canvas::map2d(int left, int right, int top, int bottom)
  {
    set_current();
    glLoadIdentity();
    glOrtho(left, right, bottom, top, -1, 1);
    return true;
  }


  bool canvas::print(const char *text)
  {
    try {
      boost::shared_ptr<image> img(image::string(text, NULL, NULL, NULL));
      draw_image(img.get(), 200, 80);
    }
    catch (...) {
      return false;
    }
  }

  bool canvas::set_current()
  {
    cast_draw(_obj)->SetCurrent();
    return true;
  }

  bool canvas::set_on_right_down(luabind::object func)
  {
    cast_draw(_obj)->func_right_down = func;
    return true;
  }

  bool canvas::swap()
  {
    if (! this->is_valid()) { return false; }
    cast_draw(_obj)->SwapBuffers();
    return true;
  }

}

