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
  class draw : public ctrl
  {
    public:
      draw(ctrl *parent, int width, int height);
      void clear();
      void flush();
      void use();
  };
}

#endif // _DRAW_HPP

