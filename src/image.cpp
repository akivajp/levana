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
#include "lev/util.hpp"
#include "register.hpp"
#include "resource/levana.xpm"

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
  globals(L)["require"]("lev.font");
  globals(L)["require"]("lev.prim");

  module(L, "lev")
  [
    namespace_("image"),
    namespace_("classes")
    [
      class_<image, base>("image")
        .def("clear", &image::clear)
        .def("clear", &image::clear0)
        .def("draw_circle", &image::draw_circle)
        .def("draw_circle", &image::draw_circle_fill)
        .def("draw_text", &image::draw_text)
        .def("get_color", &image::get_pixel, adopt(result))
        .def("get_pixel", &image::get_pixel, adopt(result))
        .def("save", &image::save)
        .property("h", &image::get_h)
        .property("height", &image::get_h)
        .property("size",  &image::get_size, adopt(result))
        .property("w", &image::get_w)
        .property("width", &image::get_w)
        .scope
        [
          def("capture", &image::capture, adopt(result)),
          def("create",  &image::create, adopt(result)),
          def("init",    &image::init),
          def("levana_icon", &image::levana_icon),
          def("load",    &image::load, adopt(result)),
          def("stroke_c",  &image::stroke, adopt(result)),
          def("stroke_c",  &image::stroke1, adopt(result))
        ]
    ]
  ];
  object lev = globals(L)["lev"];
  object classes = lev["classes"];
  object image = lev["image"];
  register_to(classes["image"], "stroke", &image::stroke_l);

  image["capture"] = classes["image"]["capture"];
  image["create"]  = classes["image"]["create"];
  image["init"]    = classes["image"]["init"];
  image["levana_icon"] = classes["image"]["levana_icon"];
  image["load"]    = classes["image"]["load"];
  image["stroke"]  = classes["image"]["stroke"];

  globals(L)["package"]["loaded"]["lev.image"] = image;
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


  image* image::capture(control *src)
  {
    image* img = NULL;
    try {
      wxWindowDC wdc((wxWindow *)src->get_rawobj());
      wxSize size = wdc.GetSize();
      img = image::create(size.GetWidth(), size.GetHeight());

      if (img == NULL) { throw -1; }
      wxMemoryDC mdc(*cast_image(img->get_rawobj()));

      mdc.Blit(0, 0, size.GetWidth(), size.GetHeight(), &wdc, 0, 0);
      return img;
    }
    catch (...) {
      delete img;
      return NULL;
    }
  }


  bool image::clear(color c)
  {
    int num_pix = get_h() * get_w();
    wxBitmap *bmp = cast_image(_obj);
    wxAlphaPixelData data(*bmp);
    data.UseAlpha();
    wxAlphaPixelData::Iterator p(data), rawStart;
    for (int y = 0 ; y < get_h() ; y++)
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
    img->clear(color(0, 0, 0, 255));
#else
    img->clear0();
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


  color* image::get_pixel(int x, int y)
  {
    color *c = NULL;
    try {
      wxAlphaPixelData data(*cast_image(_obj));
      data.UseAlpha();
      wxAlphaPixelData::Iterator p;
      p.MoveTo(data, x, y);
      if (!p.IsOk()) { throw -1; }
      c = color::create(p.Red(), p.Green(), p.Blue(), p.Alpha());
      if (c == NULL) { throw -2; }
      return c;
    }
    catch (...) {
      delete c;
      return NULL;
    }
  }

  size* image::get_size()
  {
    return size::create(get_w(), get_h());
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

  image* image::levana_icon()
  {
    static image *img = NULL;
    wxBitmap *obj = NULL;

    if (img) { return img; }
    try {
      img = new image;
      img->_obj = obj = new wxBitmap(levana_xpm);
      return img;
    }
    catch (...) {
      delete img;
      return NULL;
    }
  }

  image* image::load(const char *filename)
  {
    image *img = NULL;
    wxBitmap *obj = NULL;
    try {
      image::init();
      img = new image;
      wxString f = wxString(filename, wxConvUTF8);
      img->_obj = obj = new wxBitmap(f);
      if (not obj->IsOk()) { throw -1; }
      return img;
    }
    catch (...) {
      delete img;
      return NULL;
    }
  }

  bool image::save(const char *filename) const
  {
    wxString f = wxString(filename, wxConvUTF8);
    image::init();
    return cast_image(_obj)->ConvertToImage().SaveFile(f);
  }

  image* image::stroke(const char *str, font *f, color *fore, color *back)
  {
    image *img = NULL;
    try {
      wxMemoryDC mdc;
      wxString s(str, wxConvUTF8);
      if (f)
      {
        wxFont *font = (wxFont *)f->get_rawobj();
        mdc.SetFont(*font);
      }
      else { mdc.SetFont(wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT)); }
      wxSize sz = mdc.GetTextExtent(s);
      if (sz.GetWidth() == 0 || sz.GetHeight() == 0) { throw -1; }

      img = image::create(sz.GetWidth(), sz.GetHeight());
      if (img == NULL) { throw -2; }
      mdc.SelectObject(*cast_image(img->get_rawobj()));
      mdc.SetTextForeground(wxColour(255, 255, 255, 255));
      mdc.DrawText(s, 0, 0);

      wxAlphaPixelData data(*cast_image(img->_obj));
      data.UseAlpha();
      wxAlphaPixelData::Iterator p(data), rawStart;
      for (int y = 0 ; y < sz.GetHeight() ; y++)
      {
        rawStart = p;
        for (int x = 0 ; x < sz.GetWidth() ; x++)
        {
          double ave = ((double)p.Red() + (double)p.Green() + (double)p.Blue()) / 3.0;
          double alpha_norm = ave / 255.0;
          if (fore)
          {
            if (back)
            {
              p.Red()   = back->get_r() * (1 - alpha_norm) + fore->get_r() * alpha_norm;
              p.Green() = back->get_g() * (1 - alpha_norm) + fore->get_g() * alpha_norm;
              p.Blue()  = back->get_b() * (1 - alpha_norm) + fore->get_b() * alpha_norm;
              p.Alpha() = back->get_a() + fore->get_a() * alpha_norm * ((255 - back->get_a()) / 255.0);
            }
            else // if (!back)
            {
              p.Red()   = fore->get_r();
              p.Green() = fore->get_g();
              p.Blue()  = fore->get_b();
              p.Alpha() = alpha_norm * fore->get_a();
            }
          }
          else // if (!fore)
          {
            if (back)
            {
              p.Red()   = back->get_r() * (1 - alpha_norm);
              p.Green() = back->get_g() * (1 - alpha_norm);
              p.Blue()  = back->get_b() * (1 - alpha_norm);
              p.Alpha() = back->get_a() * (1 - alpha_norm) + 255 * alpha_norm;
            }
            else // if (!back)
            {
              p.Red()   = 0;
              p.Green() = 0;
              p.Blue()  = 0;
              p.Alpha() = ave;
            }
          }
          ++p;
        }
        p = rawStart;
        p.OffsetY(data, 1);
      }
      return img;
    }
    catch (...) {
      delete img;
      return NULL;
    }
  }

  int image::stroke_l(lua_State *L)
  {
    using namespace luabind;
    const char *str = NULL;
    object font, fore, back;

    object t = util::get_merged(L, 1, -1);

    if (t["text"]) { str = object_cast<const char *>(t["text"]); }
    else if (t["t"]) { str = object_cast<const char *>(t["t"]); }
    else if (t["string"]) { str = object_cast<const char *>(t["string"]); }
    else if (t["str"]) { str = object_cast<const char *>(t["str"]); }

    if (not str)
    {
      luaL_error(L, "text (string) is not specified!\n");
      return 0;
    }

    if (t["lev.font"]) { font = t["lev.font"]; }
    else if (t["font"]) { font = t["font"]; }
    else if (t["f"]) { font = t["f"]; }

    if (t["lev.prim.color1"]) { fore = t["lev.prim.color1"]; }
    else if (t["fg_color"]) { fore = t["fg_color"]; }
    else if (t["fg"]) { fore = t["fg"]; }
    else if (t["fore"]) { fore = t["fore"]; }
    else if (t["f"]) { fore = t["f"]; }
    else if (t["color"]) { fore = t["color"]; }
    else if (t["c"]) { fore = t["c"]; }

    if (t["lev.prim.color2"]) { back = t["lev.prim.color2"]; }
    else if (t["bg_color"]) { back = t["bg_color"]; }
    else if (t["bg"]) { back = t["bg"]; }
    else if (t["back"]) { back = t["back"]; }
    else if (t["b"]) { back = t["b"]; }

    object o = globals(L)["lev"]["classes"]["image"]["stroke_c"](str, font, fore, back);
    o.push(L);
    return 1;
  }

}

