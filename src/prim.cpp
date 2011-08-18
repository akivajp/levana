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
#include "lev/util.hpp"
#include "register.hpp"
#include <sstream>
#include <boost/format.hpp>

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
      class_<color, base>("color")
        .property("a", &color::get_a, &color::set_a)
        .property("alpha", &color::get_a, &color::set_a)
        .property("b", &color::get_b, &color::set_b)
        .property("blue", &color::get_b, &color::set_b)
        .property("code", &color::get_code32)
        .property("code32", &color::get_code32)
        .property("codestr", &color::get_codestr)
        .property("g", &color::get_g, &color::set_g)
        .property("green", &color::get_g, &color::set_g)
        .property("r", &color::get_r, &color::set_r)
        .property("red", &color::get_r, &color::set_r)
        .property("str", &color::get_codestr)
        .property("string", &color::get_codestr)
        .scope
        [
          def("create_c", &color::create, adopt(result))
        ],
      class_<size, base>("size")
        .property("d", &size::get_d, &size::set_d)
        .property("depth", &size::get_h, &size::set_d)
        .property("h", &size::get_h, &size::set_h)
        .property("height", &size::get_h, &size::set_h)
        .property("w", &size::get_w, &size::set_w)
        .property("width", &size::get_w, &size::set_w)
        .scope
        [
          def("create_c", &size::create, adopt(result))
        ],
      class_<vector, base>("vector")
        .def(self + vector())
        .property("x", &vector::get_x, &vector::set_x)
        .property("y", &vector::get_y, &vector::set_y)
        .property("z", &vector::get_z, &vector::set_z)
        .scope
        [
          def("create_c", &vector::create, adopt(result))
        ]
    ]
  ];
  object lev = globals(L)["lev"];
  object classes = lev["classes"];

  register_to(classes["color"],    "create", &color::create_l);
  register_to(classes["size"],     "create", &size::create_l);
  register_to(classes["vector"],   "create", &vector::create_l);

  lev["color"]    = classes["color"]["create"];
  lev["size"]     = classes["size"]["create"];
  lev["point"]    = classes["vector"]["create"];
  lev["position"] = classes["vector"]["create"];
  lev["vector"]   = classes["vector"]["create"];

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
      c = new color(r, g, b, a);
      return c;
    }
    catch (...) {
      return NULL;
    }
  }

  int color::create_l(lua_State *L)
  {
    using namespace luabind;
    unsigned char r = 0, g = 0, b = 0, a = 255;

    object t = util::get_merged(L, 1, -1);

    if (t["red"]) { r = object_cast<unsigned char>(t["red"]); }
    else if (t["r"]) { r = object_cast<unsigned char>(t["r"]); }
    else if (t["num1"]) { r = object_cast<unsigned char>(t["num1"]); }

    if (t["green"]) { g = object_cast<unsigned char>(t["green"]); }
    else if (t["g"]) { g = object_cast<unsigned char>(t["g"]); }
    else if (t["num2"]) { g = object_cast<unsigned char>(t["num2"]); }

    if (t["blue"]) { b = object_cast<unsigned char>(t["blue"]); }
    else if (t["b"]) { b = object_cast<unsigned char>(t["b"]); }
    else if (t["num3"]) { b = object_cast<unsigned char>(t["num3"]); }

    if (t["alpha"]) { a = object_cast<unsigned char>(t["alpha"]); }
    else if (t["a"]) { a = object_cast<unsigned char>(t["a"]); }
    else if (t["num4"]) { a = object_cast<unsigned char>(t["num4"]); }

    object o = globals(L)["lev"]["classes"]["color"]["create_c"](r, g, b, a);
    o.push(L);
    return 1;
  }

  wxUint32 color::get_code32() const
  {
    return (_a << 24) | (_r << 16) | (_g << 8) | (_b << 0);
  }

  const std::string color::get_codestr() const
  {
    char str[9];
    sprintf(str, "%02x%02x%02x%02x", _a, _r, _g, _b);
    return str;
  }


  size* size::create(int w, int h, int d)
  {
    size* s = NULL;
    try {
      s = new size(w, h, d);
      return s;
    }
    catch (...) {
      return NULL;
    }
  }


  int size::create_l(lua_State *L)
  {
    using namespace luabind;
    int w = 0, h = 0, d = 0;

    object t = util::get_merged(L, 1, -1);

    if (t["width"]) { w = object_cast<int>(t["width"]); }
    else if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["num1"]) { w = object_cast<int>(t["num1"]); }

    if (t["height"]) { h = object_cast<int>(t["height"]); }
    else if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["num2"]) { h = object_cast<int>(t["num2"]); }

    if (t["depth"]) { d = object_cast<int>(t["depth"]); }
    else if (t["d"]) { d = object_cast<int>(t["d"]); }
    else if (t["num3"]) { d = object_cast<int>(t["num3"]); }

    object o = globals(L)["lev"]["classes"]["size"]["create_c"](w, h, d);
    o.push(L);
    return 1;
  }


  vector* vector::create(int x, int y, int z)
  {
    vector* vec = NULL;
    try {
      vec = new vector(x, y, z);
      return vec;
    }
    catch (...) {
      return NULL;
    }
  }


  int vector::create_l(lua_State *L)
  {
    using namespace luabind;
    int x = 0, y = 0, z = 0;

    object t = util::get_merged(L, 1, -1);

    if (t["x"]) { x = object_cast<int>(t["x"]); }
    else if (t["num1"]) { x = object_cast<int>(t["num1"]); }

    if (t["y"]) { y = object_cast<int>(t["y"]); }
    else if (t["num2"]) { y = object_cast<int>(t["num2"]); }

    if (t["z"]) { z = object_cast<int>(t["z"]); }
    else if (t["num3"]) { z = object_cast<int>(t["num3"]); }

    object o = globals(L)["lev"]["classes"]["vector"]["create_c"](x, y, z);
    o.push(L);
    return 1;
  }

}

