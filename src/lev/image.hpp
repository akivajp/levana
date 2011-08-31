#ifndef _IMAGE_HPP
#define _IMAGE_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/image.hpp
// Purpose:     header for image handling classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     01/25/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include "font.hpp"
#include "gui.hpp"
#include "prim.hpp"
#include <lua.h>

extern "C" {
  int luaopen_lev_image(lua_State *L);
}

namespace lev
{

  class image : public base
  {
    protected:
      image();
    public:
      ~image();
      static image* capture(control *src);
      bool clear(color c);
      bool clear0() { return clear(color::transparent()); }
      static image* create(int width, int height);
      bool draw_circle(int x, int y, int radius, color border_color);
      bool draw_circle_fill(int x, int y, int radius, color border_color, color filling_color);
      void draw_text(const char *text, int x, int y, double angle = 0, color c = color::black());
      int get_h() const;
      color* get_pixel(int x, int y);
      void* get_rawobj() { return _obj; }
      size* get_size();
      virtual type_id get_type_id() const { return LEV_TIMAGE; }
      virtual const char *get_type_name() const { return "lev.image"; }
      int get_w() const;
      static bool init();
      static image* levana_icon();
      static image* load(const char *filename);
      bool save(const char *filename) const;
      bool set_pixel(color* c);
      static image* stroke(const char *str, font *f, color *fore, color *back);
      static image* stroke1(const char *str) { return stroke(str, NULL, NULL, NULL); }
      static int stroke_l(lua_State *L);
    private:
      void *_obj;
  };
}

#endif // _IMAGE_HPP

