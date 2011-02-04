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

namespace lev
{
  class bitmap : public base
  {
    public:
      bitmap(int width, int height);
      bitmap(const char *filename);
      void clear();
      void drawcircle(int x, int y, int radius);
      void drawtext(const char *text, int x, int y, double angle = 0);
      int geth() const;
      int getw() const;
      bool isvalid() const;
      bool save(const char *filename) const;
      // friend classes
      friend class canvas;
    private:
  };
}

#endif // _IMAGE_HPP

