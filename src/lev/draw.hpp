#ifndef _DRAW_HPP
#define _DRAW_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/draw.hpp
// Purpose:     header for drawing features
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/23/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "gui.hpp"
#include "prim.hpp"

#include <lua.h>

extern "C" {
  int luaopen_lev_draw(lua_State *L);
}

namespace lev
{

  class image;

  class canvas : public control
  {
    protected:
      canvas();
    public:
      ~canvas();
      bool call_compiled(drawable *img);
      bool call_texture(drawable *img);
      bool clear();
      bool clear_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
      bool clear_color1(const color &c);
      bool clear_color3(unsigned char r, unsigned char g, unsigned char b)
      { return clear_color(r, g, b, 0); }

      bool compile(drawable *img, bool force = false);
      bool compile1(drawable *img) { return compile(img); }
      static canvas* create(control *parent, int width, int height);
      static int create_l(lua_State *L);
      bool draw_image(drawable *img, int x = 0, int y = 0, unsigned char alpha = 255);
      bool draw_image1(drawable *img) { return draw_image(img); }
      bool draw_image3(drawable *img, int x, int y) { return draw_image(img, x, y); }
      bool draw_point(point *pt);
      static int draw_l(lua_State *L);
      static int draw_points(lua_State *L);
      bool fill_rect(int x, int y, int w, int h, color *filling);
      bool enable_alpha_blending(bool enable);
      bool enable_alpha_blending0() { return enable_alpha_blending(true); }
      void flush();
      virtual type_id get_type_id() const { return LEV_TCANVAS; }
      virtual const char *get_type_name() const { return "lev.gui.canvas"; }
      void line(int x1, int y1, int x2, int y2);
      bool map2d_auto();
      bool map2d(int left, int right, int top, int bottom);
      bool print(const char *text);
      bool redraw();
      bool set_current();
      bool swap();
      bool texturize(drawable *img, bool force = false);
      bool texturize1(drawable *img) { return texturize(img); }
  };
}

#endif // _DRAW_HPP

