#ifndef _DRAW_HPP
#define _DRAW_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/draw.hpp
// Purpose:     header for drawing features
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/23/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "event.hpp"

namespace levana
{
  class canvas : public control
  {
    public:
      canvas(control *parent, int width, int height);
      void clear();
      void flush();
      void line(int x1, int y1, int x2, int y2);
      void set2d();
      void swap();
      void use();
  };
}

#endif // _DRAW_HPP

