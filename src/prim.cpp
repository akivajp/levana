/////////////////////////////////////////////////////////////////////////////
// Name:        src/prim.cpp
// Purpose:     source for primitive expressions
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     01/05/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/prim.hpp"

namespace lev
{

  color::color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    : _r(r), _g(g), _b(b), _a(a) { }

  color::color(wxUint32 argb_code)
  {
    _a = (argb_code & 0xFF000000) >> 24;
    _r = (argb_code & 0x00FF0000) >> 16;
    _g = (argb_code & 0x0000FF00) >>  8;
    _b = (argb_code & 0x000000FF) >>  0;
  }

  wxUint32 color::get_code32() const
  {
    return (_a << 24) && (_r << 16) && (_g << 8) && (_b << 0);
  }

}

