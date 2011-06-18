/////////////////////////////////////////////////////////////////////////////
// Name:        src/image.cpp
// Purpose:     source for image handling classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     01/25/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/image.hpp"

#include <luabind/luabind.hpp>
#include <wx/filename.h>
#include <wx/graphics.h>
#include <wx/rawbmp.h>

int luaopen_lev_image(lua_State *L)
{
  using namespace luabind;
  using namespace lev;

  open(L);
  globals(L)["require"]("lev");

  module(L, "lev")
  [
    class_<image, base>("image")
      .def("clear", &image::clear)
      .def("clear", &image::clear_with)
      .def("draw_circle", &image::draw_circle)
      .def("draw_circle", &image::draw_circle_fill)
      .def("draw_text", &image::draw_text)
      .def("save", &image::save)
      .property("h", &image::get_h)
      .property("height", &image::get_h)
      .property("w", &image::get_w)
      .property("width", &image::get_w)
      .scope
      [
        def("create", &image::create),
        def("init", &image::init),
        def("load", &image::load)
      ]
  ];

  return 0;
}

namespace lev
{

  inline static wxBitmap* cast_image(void *obj) { return (wxBitmap *)obj; }

  inline static wxColour to_wxcolor(int r = 0, int g = 0, int b = 0, int a = 255)
  {
    return wxColour(r, g, b, a);
  }

  inline static wxColour to_wxcolor(color c)
  {
    return wxColour(c.get_r(), c.get_g(), c.get_b(), c.get_a());
  }


  image::image() : base(), _obj(NULL) { }

  image::~image()
  {
    if (_obj) { delete cast_image(_obj); }
  }


  bool image::clear_with(color c)
  {
    int num_pix = get_h() * get_w();
    wxBitmap *bmp = cast_image(_obj);
    wxAlphaPixelData data(*bmp);
    data.UseAlpha();
    wxAlphaPixelData::Iterator p(data), rawStart;
    for (int y = 0 ; y < get_h() ; y ++)
    {
      rawStart = p;
      for (int x = 0 ; x < get_w() ; x++)
      {
        p.Red()   = c.get_r();
        p.Green() = c.get_g();
        p.Blue()  = c.get_b();
        p.Alpha() = c.get_a();
        ++p;
      }
      p = rawStart;
      p.OffsetY(data, 1);
    }
    return true;
  }


  image* image::create(int width, int height)
  {
    image *img = NULL;
    try {
      img = new image;
      img->_obj = new wxBitmap(width, height, 32);
    }
    catch (...) {
      delete img;
      return NULL;
    }
    cast_image(img->_obj)->UseAlpha();
#ifdef __WXGTK__
    img->clear_with(color(0, 0, 0, 255));
#else
    img->clear();
#endif
    return img;
  }

  bool image::draw_circle(int x, int y, int radius, color border_color)
  {
    return this->draw_circle_fill(x, y, radius, border_color, color::transparent());
  }

  bool image::draw_circle_fill(int x, int y, int radius, color border_color, color filling_color)
  {
    wxBitmap *bitmap = cast_image(_obj);
    wxMemoryDC dc(*bitmap);
    wxGCDC gdc(dc);
    gdc.SetPen(to_wxcolor(border_color));
    gdc.SetBrush(to_wxcolor(filling_color));
    gdc.DrawCircle(x, y, radius);
    return true;
  }

  void image::draw_text(const char *text, int x, int y, double angle, color c)
  {
    wxMemoryDC dc;
    wxBitmap *bitmap = cast_image(_obj);
    dc.SelectObject(*bitmap);
    wxGCDC gdc(dc);
    gdc.SetTextForeground(to_wxcolor(c));
    gdc.DrawRotatedText(wxString(text, wxConvUTF8), x, y, angle);
  }

  int image::get_h() const
  {
    return cast_image(_obj)->GetHeight();
  }


  int image::get_w() const
  {
    return cast_image(_obj)->GetWidth();
  }


  bool image::init()
  {
    static bool initialized = false;
    if (not initialized)
    {
      wxInitAllImageHandlers();
      initialized = true;
    }
    return true;
  }


  image* image::load(const char *filename)
  {
    image *img = NULL;
    wxBitmap *obj = NULL;
    try {
      image::init();
      img = new image;
      wxString f = wxString(filename, wxConvUTF8);
      wxImage local_img(f);
      obj = new wxBitmap(local_img);
      if (not obj->IsOk()) { throw -1; }
    }
    catch (...) {
      delete img;
      return NULL;
    }
    img->_obj = obj;
    return img;
  }

  bool image::save(const char *filename) const
  {
    wxString f = wxString(filename, wxConvUTF8);
    image::init();
    return cast_image(_obj)->ConvertToImage().SaveFile(f);
  }
}

