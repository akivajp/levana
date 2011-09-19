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
#include "draw.hpp"
#include "font.hpp"
#include "gui.hpp"
#include "prim.hpp"
#include <lua.h>

extern "C" {
  int luaopen_lev_image(lua_State *L);
}

namespace lev
{

  // class dependencies
  class canvas;

  class image : public base
  {
    protected:
      image();
    public:
      ~image();
      bool blit(image *img, int x = 0, int y = 0);
      static image* capture(control *src);
      bool call_compiled(canvas *cv);
      bool clear(color c);
      bool clear0() { return clear(color::transparent()); }
      bool compile(canvas *cv, bool force = false);
      static image* create(int width, int height);
      bool draw_pixel(color *c, int x, int y);
      bool draw_text(const std::string &text, font *f = NULL, color *fg = NULL, color *bg = NULL, int x = 0, int y = 0);
      static int draw_text_l(lua_State *L);
      bool fill_circle(int x, int y, int radius, color *filling);
      bool fill_rect(int x, int y, int w, int h, color *filling);
      int get_h() const;
      color* get_pixel(int x, int y);
      void* get_rawobj() { return _obj; }
      size* get_size();
      virtual type_id get_type_id() const { return LEV_TIMAGE; }
      virtual const char *get_type_name() const { return "lev.image"; }
      int get_w() const;
      static bool init();
      bool is_compiled();
      static image* levana_icon();
      static image* load(const char *filename);
      bool save(const char *filename) const;
      bool set_pixel(color* c, int x, int y);
      static image* string(const std::string &str, font *f, color *fore, color *back);
      static image* string1(const std::string &str) { return string(str, NULL, NULL, NULL); }
      static int string_l(lua_State *L);
      bool stroke_circle(int x, int y, int radius, color *border, int width);
      bool stroke_rect(int x, int y, int w, int h, color *border, int width);
      image* sub_image(int x, int y, int w, int h);
      static int sub_image_l(lua_State *L);
    protected:
      void *_obj;
      void *_status;
  };
}

#endif // _IMAGE_HPP

