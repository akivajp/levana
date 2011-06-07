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

#include "event.hpp"
#include "image.hpp"

#include <lua.h>

namespace lev
{
  class canvas : public control
  {
    private:
      canvas() : control() {}
    public:
      ~canvas();
      void blendmode(bool enable);
      void clear();
      void clearcolor(unsigned char r, unsigned char g, unsigned char b);
      static canvas* create(control *parent, int width, int height);
      static int create_l(lua_State *L);
      bool draw_image(image *bmp, int x, int y);
      void flush();
      void line(int x1, int y1, int x2, int y2);
      void set2d();
      void setcurrent();
      void swap();
  };
}

#endif // _DRAW_HPP

