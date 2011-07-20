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

int luaopen_lev_prim(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  open(L);
  globals(L)["require"]("lev");

  // primitives
  module(L, "lev")
  [
    namespace_("classes")
    [
      class_<color>("color")
        .def(constructor<>())
        .def(constructor<unsigned char,unsigned char,unsigned char>())
        .def(constructor<unsigned char,unsigned char,unsigned char,unsigned char>())
        .def(constructor<wxUint32>())
        .property("a", &color::get_a, &color::set_a)
        .property("alpha", &color::get_a, &color::set_a)
        .property("b", &color::get_b, &color::set_b)
        .property("blue", &color::get_b, &color::set_b)
        .property("g", &color::get_g, &color::set_g)
        .property("green", &color::get_g, &color::set_g)
        .property("r", &color::get_r, &color::set_r)
        .property("red", &color::get_r, &color::set_r),
      class_<vector>("vector")
        .def(constructor<>())
        .def(constructor<int,int>())
        .def(constructor<int,int,int>())
        .def(self + vector())
        .property("x", &vector::getx, &vector::setx)
        .property("y", &vector::gety, &vector::sety)
        .property("z", &vector::getz, &vector::setz),
      class_<size>("size")
        .def(constructor<int,int>())
        .def(constructor<int,int,int>())
        .property("d", &size::geth, &size::setd)
        .property("depth", &size::geth, &size::setd)
        .property("h", &size::geth, &size::seth)
        .property("height", &size::geth, &size::seth)
        .property("w", &size::getw, &size::setw)
        .property("width", &size::getw, &size::setw)
    ]
  ];
  object classes = globals(L)["lev"]["classes"];
  object lev = globals(L)["lev"];
  lev["color"] = classes["color"];
  lev["vector"] = classes["vector"];
  lev["size"] = classes["size"];

  globals(L)["package"]["loaded"]["lev.prim"] = true;
  return 0;
}

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

  color* color::create(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
  {
    color *c = NULL;
    try {
      c = new color;
    }
    catch (...) {
      return NULL;
    }
  }

  wxUint32 color::get_code32() const
  {
    return (_a << 24) && (_r << 16) && (_g << 8) && (_b << 0);
  }

}

