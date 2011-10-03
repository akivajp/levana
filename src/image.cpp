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

#include "lev/app.hpp"
#include "lev/draw.hpp"
#include "lev/font.hpp"
#include "lev/gui.hpp"
#include "lev/util.hpp"
#include "register.hpp"
#include "resource/levana.xpm"

#include <boost/shared_array.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/luabind.hpp>
#include <wx/filename.h>
#include <wx/glcanvas.h>
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
        .def("blit", &image::blit)
        .def("clear", &image::clear)
        .def("clear", &image::clear0)
        .def("compile", &image::compile)
        .def("compile", &image::compile1)
        .def("draw", &image::blit)
        .def("draw_pixel", &image::draw_pixel)
        .def("fill_circle", &image::fill_circle)
        .def("fill_rect", &image::fill_rect)
        .def("fill_rectangle", &image::fill_rect)
        .def("get_color", &image::get_pixel, adopt(result))
        .def("get_pixel", &image::get_pixel, adopt(result))
        .property("h", &image::get_h)
        .property("height", &image::get_h)
        .def("load", &image::reload)
        .def("reload", &image::reload)
        .def("save", &image::save)
        .def("set_color", &image::set_pixel)
        .def("set_pixel", &image::set_pixel)
        .property("size",  &image::get_size, adopt(result))
        .def("stroke_circle", &image::stroke_circle)
        .def("stroke_rect", &image::stroke_rect)
        .def("stroke_rectangle", &image::stroke_rect)
        .def("texturize", &image::texturize)
        .def("texturize", &image::texturize1)
        .property("w", &image::get_w)
        .property("width", &image::get_w)
        .scope
        [
          def("calc_string", &image::calc_string),
          def("calc_string", &image::calc_string1),
          def("capture", &image::capture, adopt(result)),
          def("create",  &image::create, adopt(result)),
          def("draw_text_c", &image::draw_text),
          def("init",    &image::init),
          def("levana_icon", &image::levana_icon),
          def("load",    &image::load, adopt(result)),
          def("string_c",  &image::string, adopt(result)),
          def("sub_image_c", &image::sub_image, adopt(result))
        ],
      class_<layout, image>("layout")
        .def("clear", &layout::clear)
        .def("clear", &layout::clear0)
        .def("complete", &layout::complete)
        .def("draw_next", &layout::draw_next)
        .def("reserve_image", &layout::reserve_image)
        .def("reserve_new_line", &layout::reserve_new_line)
        .def("reserve_word", &layout::reserve_word)
        .def("reserve_word", &layout::reserve_word1)
        .property("color", &layout::get_color, &layout::set_color)
        .property("font",  &layout::get_font, &layout::set_font)
        .property("fore",  &layout::get_color, &layout::set_color)
        .property("is_done", &layout::is_done)
        .scope
        [
          def("create", &layout::create, adopt(result))
        ]
    ]
  ];
  object lev = globals(L)["lev"];
  object classes = lev["classes"];
  object image = lev["image"];
  register_to(classes["image"], "draw_text", &image::draw_text_l);
  register_to(classes["image"], "get_sub", &image::sub_image_l);
  register_to(classes["image"], "get_sub_image", &image::sub_image_l);
  register_to(classes["image"], "string", &image::string_l);
  register_to(classes["image"], "sub", &image::sub_image_l);
  register_to(classes["image"], "sub_image", &image::sub_image_l);

  image["calc_string"] = classes["image"]["calc_string"];
  image["capture"]     = classes["image"]["capture"];
  image["create"]      = classes["image"]["create"];
  image["init"]        = classes["image"]["init"];
  image["levana_icon"] = classes["image"]["levana_icon"];
  image["load"]        = classes["image"]["load"];
  image["string"]      = classes["image"]["string"];
  image["layout"]      = classes["layout"]["create"];

  globals(L)["package"]["loaded"]["lev.image"] = image;
  return 0;
}

namespace lev
{

  class myImageStatus
  {
    public:
      myImageStatus() : compiled(false), texturized(false), index(-1) { }

      ~myImageStatus()
      {
        Clear();
      }

      bool Call()
      {
        if (compiled)
        {
          glCallList(index);
          return true;
        }
        if (texturized)
        {
          glBindTexture(GL_TEXTURE_2D, index);
          return true;
        }
        return false;
      }

      bool Clear()
      {
        if (compiled)
        {
          glDeleteLists(index, 1);
          compiled = false;
        }
        if (texturized)
        {
          glDeleteTextures(1, &index);
          texturized = false;
        }
        return true;
      }

      bool compiled;
      bool texturized;
      GLuint index;
  };

  inline static wxBitmap* cast_image(void *obj) { return (wxBitmap *)obj; }
  inline static myImageStatus* cast_status(void *obj) { return (myImageStatus *)obj; }

  inline static wxColour to_wxcolor(int r = 0, int g = 0, int b = 0, int a = 255)
  {
    return wxColour(r, g, b, a);
  }

  inline static wxColour to_wxcolor(color c)
  {
    return wxColour(c.get_r(), c.get_g(), c.get_b(), c.get_a());
  }

  static bool image_draw_mask(image *img, wxBitmap *bmp, color *c)
  {
    if (c == NULL) { return false; }
    try {
      wxAlphaPixelData dst(*cast_image(img->get_rawobj()));
      dst.UseAlpha();
      wxAlphaPixelData src(*bmp);
      src.UseAlpha();
      wxAlphaPixelData::Iterator d(dst), s(src), d_start, s_start;
      for (int y = 0 ; y < img->get_h() ; y++)
      {
        d_start = d;
        s_start = s;
        for (int x = 0 ; x < img->get_w() ; x++)
        {
          double ave = ((double)s.Red() + (double)s.Green() + (double)s.Blue()) / 3.0;
          double alpha_norm = (ave / 255.0) * ((c->get_a()) / 255.0);
          double base_alpha = (1 - alpha_norm) * d.Alpha() / 255.0;
          d.Alpha() = 255 * (alpha_norm + base_alpha);
          if (d.Alpha() > 0)
          {
            double fix = 255.0 / d.Alpha();
            d.Red()   = (d.Red()   * base_alpha + c->get_r() * alpha_norm) * fix;
            d.Green() = (d.Green() * base_alpha + c->get_g() * alpha_norm) * fix;
            d.Blue()  = (d.Blue()  * base_alpha + c->get_b() * alpha_norm) * fix;
          }
          ++d;
          ++s;
        }
        d = d_start;
        s = s_start;
        d.OffsetY(dst, 1);
        s.OffsetY(src, 1);
      }
      return true;
    }
    catch (...) {
      return false;
    }
  }

  image::image() : base(), _obj(NULL), _status(NULL) { }

  image::~image()
  {
    if (_obj) { delete cast_image(_obj); }
    if (_status) { delete cast_status(_status); }
  }

  bool image::blit(image *img, int x_offset, int y_offset)
  {
    if (img == NULL) { return false; }
    try {
      wxAlphaPixelData dst(*cast_image(_obj));
      dst.UseAlpha();
      wxAlphaPixelData src(*cast_image(img->_obj));
      src.UseAlpha();

      wxAlphaPixelData::Iterator p(src), rawStart;
      for (int y = 0 ; y < img->get_h() ; y++)
      {
        rawStart = p;
        for (int x = 0 ; x < img->get_w() ; x++)
        {
          boost::shared_ptr<color> c(color::create(p.Red(), p.Green(), p.Blue(), p.Alpha()));
          this->draw_pixel(c.get(), x + x_offset, y + y_offset);
          ++p;
        }
        p = rawStart;
        p.OffsetY(src, 1);
      }
      return true;
    }
    catch (...) {
      return false;
    }
  }


  size image::calc_string(const std::string &str, font *f)
  {
    wxMemoryDC mdc;
    if (f)
    {
      wxFont *font = (wxFont *)f->get_rawobj();
      mdc.SetFont(*font);
    }
    else { mdc.SetFont(*wxNORMAL_FONT); }
    wxSize sz = mdc.GetTextExtent(wxString(str.c_str(), wxConvUTF8));
    return size(sz.GetWidth(), sz.GetHeight());
  }


  bool image::call_compiled(canvas *cv)
  {
    cv->set_current();
    return cast_status(_status)->Call();
  }

  bool image::call_texture(canvas *cv)
  {
    cv->set_current();
    return cast_status(_status)->Call();
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


  bool image::clear(const color &c)
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
    cast_status(_status)->Clear();
    return true;
  }


  image* image::clone()
  {
    image* img = NULL;
    try {
      img = new image;
      img->_obj = new wxBitmap(*cast_image(_obj));
      img->_status = new myImageStatus;
      cast_image(_obj)->UseAlpha();
      return img;
    }
    catch (...) {
      delete img;
      return NULL;
    }
  }


  bool image::compile(canvas *cv, bool force)
  {
    myImageStatus *st = cast_status(_status);
    cv->set_current();

    if (cv == NULL) { return false; }
    if (!force && st->compiled) { return false; }
    st->Clear();

    st->index = glGenLists(1);
    if (st->index <= 0) { return false; }
    st->compiled = true;

    try {
      wxAlphaPixelData pixels(*cast_image(_obj));
      pixels.UseAlpha();
      wxAlphaPixelData::Iterator p(pixels), rawStart;

      glNewList(st->index, GL_COMPILE);
      glBegin(GL_POINTS);
        for (int i = 0 ; i < get_h() ; i++)
        {
          rawStart = p;
          for (int j = 0 ; j < get_w() ; j++)
          {
            if (p.Alpha())
            {
              glColor4ub(p.Red(), p.Green(), p.Blue(), p.Alpha());
              glVertex2i(j, i);
            }
            p++;
          }
          p = rawStart;
          p.OffsetY(pixels, 1);
        }
      glEnd();
      glEndList();

      return true;
    }
    catch (...) {
      return false;
    }
  }



  image* image::create(int width, int height)
  {
    image *img = NULL;
    try {
      img = new image;
      img->_obj = new wxBitmap(width, height, 32);
      img->_status = new myImageStatus;

      cast_image(img->_obj)->UseAlpha();
      img->clear0();
      return img;
    }
    catch (...) {
      delete img;
      return NULL;
    }
  }

  bool image::draw_pixel(color *c, int x, int y)
  {
    if (x < 0 || x >= get_w() || y < 0 || y >= get_h()) { return false; }
    try {
      wxAlphaPixelData data(*cast_image(_obj));
      data.UseAlpha();
      wxAlphaPixelData::Iterator p;
      p.MoveTo(data, x, y);
      if (!p.IsOk()) { throw -1; }
      if (c->get_a() == 0) { return true; }
      if (p.Alpha() == 0)
      {
        p.Red()   = c->get_r();
        p.Green() = c->get_g();
        p.Blue()  = c->get_b();
        p.Alpha() = c->get_a();
        return true;
      }
      double alpha_norm = c->get_a() / 255.0;
      double base_alpha = (1 - alpha_norm) * p.Alpha() / 255.0;
      p.Alpha() = 255 * (alpha_norm + base_alpha);
      if (p.Alpha() > 0)
      {
        double fix = 255.0 / p.Alpha();
        p.Red()   = (p.Red()   * base_alpha + c->get_r() * alpha_norm) * fix;
        p.Green() = (p.Green() * base_alpha + c->get_g() * alpha_norm) * fix;
        p.Blue()  = (p.Blue()  * base_alpha + c->get_b() * alpha_norm) * fix;
      }
      cast_status(_status)->Clear();
      return true;
    }
    catch (...) {
      return false;
    }
  }

  bool image::draw_text(const std::string &text, font *f, color *fg, color *bg, int x, int y)
  {
    try {
      boost::shared_ptr<image> txt(image::string(text, f, fg, bg));
      if (txt.get() == NULL) { throw -1; }
      return blit(txt.get(), x, y);
    }
    catch (...) {
      return false;
    }
  }

  int image::draw_text_l(lua_State *L)
  {
    using namespace luabind;
    int x = 0, y = 0;

    try {
      luaL_checktype(L, 1, LUA_TUSERDATA);
      object img(from_stack(L, 1));
      object t = util::get_merged(L, 2, -1);

      object txt_img = globals(L)["lev"]["classes"]["image"]["string"](t);

      if (t["x"]) { x = object_cast<int>(t["x"]); }
      else if (t["lua.number1"]) { x = object_cast<int>(t["lua.number1"]); }

      if (t["y"]) { y = object_cast<int>(t["y"]); }
      else if (t["lua.number2"]) { y = object_cast<int>(t["lua.number2"]); }

      object res = img["blit"](img, txt_img, x, y);
      res.push(L);
      return 1;
    }
    catch (...) {
      lua_pushnil(L);
      return 1;
    }
  }

  int image::get_h() const
  {
    return cast_image(_obj)->GetHeight();
  }

  bool image::fill_circle(int x, int y, int radius, color *filling)
  {
    wxBitmap tmp(get_w(), get_h(), 32);
    try {
      wxMemoryDC mdc(tmp);
      mdc.SetPen(wxColour(255, 255, 255, 255));
      mdc.SetBrush(wxColour(255, 255, 255, 255));
      mdc.SetBackground(wxColour(0, 0, 0, 255));
      mdc.Clear();
      mdc.DrawCircle(x, y, radius);
    }
    catch (...) {
      return false;
    }
    return image_draw_mask(this, &tmp, filling);
  }

  bool image::fill_rect(int x, int y, int w, int h, color *filling)
  {
#ifdef __WXGTK__
    wxBitmap tmp(get_w(), get_h(), 32);

    try {
      wxMemoryDC mdc(tmp);
      mdc.SetPen(wxColour(255, 255, 255, 255));
      mdc.SetBrush(wxColour(255, 255, 255, 255));
      mdc.SetBackground(wxColour(0, 0, 0, 255));
      mdc.Clear();
      mdc.DrawRectangle(x, y, w, h);
      return image_draw_mask(this, &tmp, filling);
    }
    catch (...) {
      return false;
    }
#else
    try {
      wxMemoryDC mdc(*cast_image(_obj));
      wxGCDC gdc(mdc);
      if (filling)
      {
        gdc.SetPen(wxColour(to_wxcolor(*filling)));
        gdc.SetBrush(wxColour(to_wxcolor(*filling)));
      }
      gdc.DrawRectangle(x, y, w, h);
      return true;
    }
    catch (...) {
      return false;
    }
#endif
  }


  color* image::get_pixel(int x, int y)
  {
    color *c = NULL;
    if (x < 0 || x >= get_w() || y < 0 || y >= get_h()) { return NULL; }
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

  bool image::is_compiled()
  {
    return cast_status(_status)->compiled;
  }

  bool image::is_texturized()
  {
    return cast_status(_status)->texturized;
  }

  image* image::levana_icon()
  {
    static image *img = NULL;
    wxBitmap *obj = NULL;

    if (img) { return img; }
    try {
      img = new image;
      img->_obj = obj = new wxBitmap(levana_xpm);
      img->_status = new myImageStatus;
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
      img->_obj = obj = new wxBitmap(wxImage(f));
      img->_status = new myImageStatus;
      if (not obj->IsOk()) { throw -1; }
      obj->UseAlpha();
      return img;
    }
    catch (...) {
      delete img;
      return NULL;
    }
  }

  bool image::reload(const std::string &filename)
  {
    try {
      wxBitmap *tmp = new wxBitmap(wxImage(wxString(filename.c_str(), wxConvUTF8)));
      tmp->UseAlpha();
      delete cast_image(_obj);
      _obj = tmp;
      cast_status(_status)->Clear();
      return true;
    }
    catch (...) {
      return false;
    }
  }

  bool image::save(const char *filename) const
  {
    wxString f = wxString(filename, wxConvUTF8);
    image::init();
    return cast_image(_obj)->ConvertToImage().SaveFile(f);
  }

  bool image::set_pixel(color *c, int x, int y)
  {
    if (x < 0 || x >= get_w() || y < 0 || y >= get_h()) { return false; }
    try {
      wxAlphaPixelData data(*cast_image(_obj));
      data.UseAlpha();
      wxAlphaPixelData::Iterator p;
      p.MoveTo(data, x, y);
      if (!p.IsOk()) { throw -1; }
      p.Red()   = c->get_r();
      p.Green() = c->get_g();
      p.Blue()  = c->get_b();
      p.Alpha() = c->get_a();
      return c;
    }
    catch (...) {
      return false;
    }
  }


  image* image::string(const std::string &str, font *f, color *fore, color *back)
  {
    image *img = NULL;
    if (str.empty()) { return NULL; }

#ifdef __WXMSW__
    try {
      wxString s(str.c_str(), wxConvUTF8);
      if (s.empty()) { s = wxString(str.c_str(), wxConvLibc); }
      wxMemoryDC mdc;
      wxGCDC gdc(mdc);
      if (f)
      {
        wxFont *ft = (wxFont *)f->get_rawobj();
        gdc.SetFont(*ft);
      }
      else { gdc.SetFont(*wxNORMAL_FONT); }
      wxSize sz = gdc.GetTextExtent(s);
      img = image::create(sz.GetWidth(), sz.GetHeight());
      if (! img) { throw -1; }
      mdc.SelectObject(*cast_image(img->_obj));
      if (fore) { gdc.SetTextForeground(to_wxcolor(*fore)); }
      gdc.DrawText(s, 0, 0);
      return img;
    }
    catch (...) {
      delete img;
      return NULL;
    }
#else
    try {
      wxMemoryDC mdc;
      wxString s(str.c_str(), wxConvUTF8);
      if (s.empty()) { s = wxString(str.c_str(), wxConvLibc); }
      if (f)
      {
        wxFont *font = (wxFont *)f->get_rawobj();
        mdc.SetFont(*font);
      }
      else { mdc.SetFont(*wxNORMAL_FONT); }
      wxSize sz = mdc.GetTextExtent(s);
      if (sz.GetWidth() == 0 || sz.GetHeight() == 0) { throw -1; }

      img = image::create(sz.GetWidth(), sz.GetHeight());
      if (img == NULL) { throw -2; }
      mdc.SelectObject(*cast_image(img->get_rawobj()));
      mdc.SetBrush(wxColour(0, 0, 0, 255));
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
              alpha_norm = alpha_norm * fore->get_a() / 255.0;
              double base_alpha = (1 - alpha_norm) * back->get_a() / 255.0;
              p.Alpha() = 255 * (alpha_norm + base_alpha);
              if (p.Alpha() > 0)
              {
                double fix = 255.0 / p.Alpha();
                p.Red()   = (back->get_r() * base_alpha + fore->get_r() * alpha_norm) * fix;
                p.Green() = (back->get_g() * base_alpha + fore->get_g() * alpha_norm) * fix;
                p.Blue()  = (back->get_b() * base_alpha + fore->get_b() * alpha_norm) * fix;
              }
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
#endif
  }

  int image::string_l(lua_State *L)
  {
    using namespace luabind;
    const char *str = NULL;
    object font, fore, back;

    object t = util::get_merged(L, 1, -1);

    if (t["lua.string1"]) { str = object_cast<const char *>(t["lua.string1"]); }
    else if (t["lev.mb_string1"]) { str = object_cast<const char *>(t["lev.mb_string1"]["str"]); }
    else if (t["text"]) { str = object_cast<const char *>(t["text"]); }
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

    object o = globals(L)["lev"]["classes"]["image"]["string_c"](str, font, fore, back);
    o.push(L);
    return 1;
  }

  bool image::stroke_circle(int x, int y, int radius, color *border, int width)
  {
    wxBitmap tmp(get_w(), get_h(), 32);
    try {
      wxMemoryDC mdc(tmp);
      mdc.SetPen(wxPen(wxColour(255, 255, 255, 255), width));
      mdc.SetBrush(wxColour(0, 0, 0, 255));
      mdc.SetBackground(wxColour(0, 0, 0, 255));
      mdc.Clear();
      mdc.DrawCircle(x, y, radius);
    }
    catch (...) {
      return false;
    }
    return image_draw_mask(this, &tmp, border);
  }

  bool image::stroke_rect(int x, int y, int w, int h, color *border, int width)
  {
    wxBitmap tmp(get_w(), get_h(), 32);
    try {
      wxMemoryDC mdc(tmp);
      mdc.SetPen(wxPen(wxColour(255, 255, 255, 255), width));
      mdc.SetBrush(wxColour(0, 0, 0, 255));
      mdc.SetBackground(wxColour(0, 0, 0, 255));
      mdc.Clear();
      mdc.DrawRectangle(x, y, w, h);
    }
    catch (...) {
      return false;
    }
    return image_draw_mask(this, &tmp, border);
  }

  image* image::sub_image(int x, int y, int w, int h)
  {
    image* img = NULL;
    try {
      img = new image;
      img->_obj = new wxBitmap(cast_image(_obj)->GetSubBitmap(wxRect(x, y, w, h)));
      img->_status = new myImageStatus;
      cast_image(_obj)->UseAlpha();
      return img;
    }
    catch (...) {
      delete img;
      return NULL;
    }
  }

  int image::sub_image_l(lua_State *L)
  {
    using namespace luabind;
    int x = 0, y = 0, w = -1, h = -1;

    luaL_checktype(L, 1, LUA_TUSERDATA);
    image *img = object_cast<image *>(object(from_stack(L, 1)));
    if (img == NULL) { return 0; }
    object t = util::get_merged(L, 2, -1);

    if (t["x"]) { x = object_cast<int>(t["x"]); }
    else if (t["lua.number1"]) { x = object_cast<int>(t["lua.number1"]); }
    if (x < 0) { x = 0; }

    if (t["y"]) { y = object_cast<int>(t["y"]); }
    else if (t["lua.number2"]) { y = object_cast<int>(t["lua.number2"]); }
    if (y < 0) { y = 0; }

    if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["lua.number3"]) { w = object_cast<int>(t["lua.number3"]); }
    if (w < 0) { w = img->get_w() - x; }

    if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["lua.number4"]) { h = object_cast<int>(t["lua.number4"]); }
    if (h < 0) { h = img->get_h() - y; }

    object o = globals(L)["lev"]["classes"]["image"]["sub_image_c"](img, x, y, w, h);
    o.push(L);
    return 1;
  }

  bool image::swap(image *img)
  {
    if (! img) { return false; }
    void *tmp  = this->_obj;
    this->_obj = img->_obj;
    img->_obj  = tmp;
    return true;
  }

  bool image::texturize(canvas *cv, bool force)
  {
    myImageStatus *st = cast_status(_status);
    cv->set_current();

    if (cv == NULL) { return false; }
    if (!force && st->texturized) { return false; }
    st->Clear();

    glGenTextures(1, &st->index);
    if (st->index <= 0) { return false; }
    st->texturized = true;

    try {
      boost::shared_array<unsigned char> data;
      const int w = get_w();
      const int h = get_h();
      wxAlphaPixelData pixels(*cast_image(_obj));
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

      glBindTexture(GL_TEXTURE_2D, st->index);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      int res = gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA,
                                  GL_UNSIGNED_BYTE, data.get());
      if (res != 0) { throw -1; }
      return true;
    }
    catch (...) {
      st->Clear();
      return false;
    }

//    try {
//      boost::shared_array<unsigned char> data;
//      wxImage img = cast_image(_obj)->ConvertToImage();
//      const int w = get_w();
//      const int h = get_h();
//
//      data.reset(new unsigned char [4 * w * h]);
//      for (int i = 0 ; i < h ; i++)
//      {
//        for (int j = 0 ; j < w ; j++)
//        {
//          int k = (h - i - 1) * w + j;
//          data[4*k]     = img.GetRed(j, i);
//          data[4*k + 1] = img.GetGreen(j, i);
//          data[4*k + 2] = img.GetBlue(j, i);
//          data[4*k + 3] = img.HasAlpha() ? img.GetAlpha(j, i) : 255;
//        }
//      }
//
//      cv->set_current();
//      glBindTexture(GL_TEXTURE_2D, st->index);
//      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//      int res = gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA,
//                                  GL_UNSIGNED_BYTE, data.get());
//      if (res != 0) { throw -1; }
//      return true;
//    }
//    catch (...) {
//      st->Clear();
//      return false;
//    }
  }



  class myLayout
  {
    public:
      myLayout(int width_stop = -1)
        : h(-1), w(width_stop), current_x(0), index(0), font_obj(),
          index_to_col(), index_to_row(), log(), rows()
      {
        font_obj = globals(application::get_app()->getL())["lev"]["font"]["load"]();
        color_obj = globals(application::get_app()->getL())["lev"]["prim"]["color"](0, 0, 0);

        rows.push_back(std::vector<boost::shared_ptr<image> >());
      }

      int CalcMaxWidth()
      {
        int width = 0;
        for (int i = 0; i < rows.size(); i++)
        {
          int current = CalcRowWidth(rows[i]);
          if (current > width) { width = current; }
        }
        return width;
      }

      int CalcRowHeight(const std::vector<boost::shared_ptr<image> > &row)
      {
        int height = 0;
        for (int i = 0; i < row.size(); i++)
        {
          if (row[i]->get_h() > height) { height = row[i]->get_h(); }
        }
        return height;
      }

      int CalcRowWidth(const std::vector<boost::shared_ptr<image> > &row)
      {
        int width = 0;
        for (int i = 0; i < row.size(); i++)
        {
          width += row[i]->get_w();
        }
        return width;
      }

      int CalcTotalHeight()
      {
        int height = 0;
        for (int i = 0; i < rows.size(); i++)
        {
          height += CalcRowHeight(rows[i]);
        }
        return height;
      }

      bool Clear()
      {
        current_x = 0;
        index = 0;
        index_to_col.clear();
        index_to_row.clear();
        log.clear();
        rows.clear();

        rows.push_back(std::vector<boost::shared_ptr<image> >());
        return true;
      }

      bool Complete(image *img)
      {
        for (; index < log.size(); index++)
        {
          DrawIndex(img, index);
        }
        return true;
      }

      bool DrawIndex(image *img, int index)
      {
        int col_index = index_to_col[index];
        int row_index = index_to_row[index];

        int y = 0;
        for (int i = 0; i <= row_index; i++)
        {
          y += CalcRowHeight(rows[i]);
        }
        y -= rows[row_index][col_index]->get_h();

        int x = 0;
        const std::vector<boost::shared_ptr<image> > &row = rows[row_index];
        for (int i = 0; i < col_index; i++)
        {
          x += row[i]->get_w();
        }

        return img->blit(rows[row_index][col_index].get(), x, y);
      }

      bool ReserveImage(const std::string &name, boost::shared_ptr<image> img)
      {
        try {
          if (! img) { throw -1; }
          if (w >= 0 && current_x + img->get_w() > w)
          {
            rows.push_back(std::vector<boost::shared_ptr<image> >());
            current_x = 0;
          }
          current_x += img->get_w();
          index_to_col.push_back(rows[rows.size() - 1].size());
          index_to_row.push_back(rows.size() - 1);
          log.push_back(name);
          rows[rows.size() - 1].push_back(img);
          return true;
        }
        catch (...) {
          return false;
        }
      }

      bool ReserveNewLine()
      {
        rows.push_back(std::vector<boost::shared_ptr<image> >());
        current_x = 0;
      }

      bool ReserveWord(const std::string &word, const std::string &ruby)
      {
        if (word.empty()) { return false; }
        try {
          font *f = object_cast<font *>(font_obj);
          color *c = object_cast<color *>(color_obj);
          boost::shared_ptr<image> img;
          if (ruby.empty()) {
            img.reset(image::string(word, f, c));
            return ReserveImage(word, img);
          }
          else
          {
            boost::shared_ptr<font> f_ruby(f->clone());
            f_ruby->set_point_size(f->get_point_size() / 2);
            boost::shared_ptr<image> img_ruby(image::string(ruby, f_ruby.get(), c));
            boost::shared_ptr<image> img_word(image::string(word, f, c));
            if (!img_ruby || !img_word) { throw -1; }
            int h = img_ruby->get_h() + img_word->get_h();
            int w = img_ruby->get_w();
            if (img_word->get_w() > w) { w = img_word->get_w(); }

            img.reset(image::create(w, h));
            img->blit(img_ruby.get(), (w - img_ruby->get_w()) / 2, 0);
            img->blit(img_word.get(), (w - img_word->get_w()) / 2, img_ruby->get_h());
            return ReserveImage((boost::format("{ruby,%s,%s}") % word % ruby).str(), img);
          }
        }
        catch (...) {
          return false;
        }
      }

      int h, w;
      int current_x;
      int index;
      luabind::object color_obj;
      luabind::object font_obj;
      std::vector<int> index_to_row;
      std::vector<int> index_to_col;
      std::vector<std::string> log;
      std::vector<std::vector<boost::shared_ptr<image> > > rows;
  };
  static myLayout* cast_lay(void *obj) { return (myLayout *)obj; }

  layout::layout()
    : image(), _buf(NULL)
  {
  }

  layout::~layout()
  {
    if (_buf) { delete cast_lay(_buf); }
  }

  bool layout::clear(const color &c)
  {
    cast_lay(_buf)->Clear();
    return image::clear(c);
  }

  bool layout::complete()
  {
    if (is_done()) { return false; }
    cast_status(_status)->Clear();
    this->fit();
    return cast_lay(_buf)->Complete(this);
  }

  layout* layout::create(int width_stop)
  {
    layout* img = NULL;
    try {
      img = new layout;
      img->_obj = new wxBitmap(1, 1, 32);
      img->_status = new myImageStatus;
      img->_buf = new myLayout(width_stop);
      return img;
    }
    catch (...) {
      delete img;
      return NULL;
    }
  }

  bool layout::draw_next()
  {
    if (is_done()) { return false; }
    myLayout *lay = cast_lay(_buf);
    cast_status(_status)->Clear();
    this->fit();
    return lay->DrawIndex(this, lay->index++);
  }

  bool layout::fit()
  {
    int w = cast_lay(_buf)->CalcMaxWidth();
    if (get_w() > w) { w = get_w(); }
    int h = cast_lay(_buf)->CalcTotalHeight();
    if (get_h() > h) { h = get_h(); }
    if (h == get_h() && w == get_w()) { return false; }

    image *img2 = NULL;
    try {
      img2 = image::create(w, h);
      if (img2 == NULL) { throw -1; }
      img2->blit(this, 0, 0);
      this->swap(img2);

      delete img2;
      return true;
    }
    catch (...) {
      delete img2;
      return false;
    }
  }

  luabind::object layout::get_color()
  {
    return cast_lay(_buf)->color_obj;
  }


  luabind::object layout::get_font()
  {
    return cast_lay(_buf)->font_obj;
  }

  bool layout::is_done()
  {
    myLayout *lay = cast_lay(_buf);
    return lay->index >= lay->log.size();
  }

  bool layout::reserve_image(const std::string &name, image *img)
  {
    return cast_lay(_buf)->ReserveImage(name, boost::shared_ptr<image>(img->clone()));
  }

  bool layout::reserve_new_line()
  {
    return cast_lay(_buf)->ReserveNewLine();
  }

  bool layout::reserve_word(const std::string &word, const std::string &ruby)
  {
    return cast_lay(_buf)->ReserveWord(word, ruby);
  }


  bool layout::set_color(luabind::object c)
  {
    if (! base::is_type_of(c, LEV_TCOLOR)) { return false; }
    cast_lay(_buf)->color_obj = c;
    return true;
  }


  bool layout::set_font(luabind::object f)
  {
    if (! base::is_type_of(f, LEV_TFONT)) { return false; }
    cast_lay(_buf)->font_obj = f;
    return true;
  }

}

