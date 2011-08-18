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
#include "image.hpp"

#include <lua.h>

extern "C" {
  int luaopen_lev_draw(lua_State *L);
}

namespace lev
{
  class canvas : public control
  {
    private:
      canvas() : control() {}
    public:
      ~canvas();
      void clear();
      void clear_color(unsigned char r, unsigned char g, unsigned char b);
      static canvas* create(control *parent, int width, int height);
      static int create_l(lua_State *L);
      bool draw_image(image *bmp, int x = 0, int y = 0, unsigned char alpha = 255);
      bool draw_image1(image *bmp) { return draw_image(bmp); }
      bool draw_image3(image *bmp, int x, int y) { return draw_image(bmp, x, y); }
      static int draw_l(lua_State *L);
      static int draw_points(lua_State *L);
      bool enable_alpha_blending(bool enable);
      bool enable_alpha_blending0() { return enable_alpha_blending(true); }
      void flush();
      void line(int x1, int y1, int x2, int y2);
      bool map2d_auto();
      bool map2d(int left, int right, int top, int bottom);
      void set_current();
      void swap();
  };
}

#endif // _DRAW_HPP

