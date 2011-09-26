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
      bool call_compiled(image *img);
      bool call_texture(image *img);
      void clear();
      void clear_color(unsigned char r, unsigned char g, unsigned char b);
      bool compile(image *img, bool force = false);
      bool compile1(image *img) { return compile(img, false); }
      static canvas* create(control *parent, int width, int height);
      static int create_l(lua_State *L);
      bool draw_image(image *bmp, int x = 0, int y = 0);
      bool draw_image1(image *bmp) { return draw_image(bmp); }
      bool draw_point(point *pt);
      static int draw_l(lua_State *L);
      static int draw_points(lua_State *L);
      bool enable_alpha_blending(bool enable);
      bool enable_alpha_blending0() { return enable_alpha_blending(true); }
      void flush();
      virtual luabind::object get_on_right_down();
      void line(int x1, int y1, int x2, int y2);
      bool map2d_auto();
      bool map2d(int left, int right, int top, int bottom);
      bool print(const char *text);
      bool set_current();
      virtual bool set_on_right_down(luabind::object func);
      bool swap();
      bool texturize(image *img, bool force = false);
      bool texturize1(image *img) { return texturize(img, false); }
  };
}

#endif // _DRAW_HPP

