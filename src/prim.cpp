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
    namespace_("prim"),
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
        ],
      class_<point, base>("point")
        .property("c", &point::get_color, &point::set_color)
        .property("col", &point::get_color, &point::set_color)
        .property("color", &point::get_color, &point::set_color)
        .property("colour", &point::get_color, &point::set_color)
        .property("p", &point::get_vertex, &point::set_vertex)
        .property("pos", &point::get_vertex, &point::set_vertex)
        .property("position", &point::get_vertex, &point::set_vertex)
        .property("vec", &point::get_vertex, &point::set_vertex)
        .property("vector", &point::get_vertex, &point::set_vertex)
        .property("vert", &point::get_vertex, &point::set_vertex)
        .property("vertex", &point::get_vertex, &point::set_vertex)
        .scope
        [
          def("create_c", &point::create, adopt(result))
        ],
      class_<rect, base>("rect")
        .property("h", &rect::get_h, &rect::set_h)
        .property("height", &rect::get_h, &rect::set_h)
        .property("p", &rect::get_position, &rect::set_position)
        .property("pos", &rect::get_position, &rect::set_position)
        .property("position", &rect::get_position, &rect::set_position)
        .property("s", &rect::get_size, &rect::set_size)
        .property("size", &rect::get_size, &rect::set_size)
        .property("sz", &rect::get_size, &rect::set_size)
        .property("v", &rect::get_position, &rect::set_position)
        .property("vec", &rect::get_position, &rect::set_position)
        .property("vector", &rect::get_position, &rect::set_position)
        .property("x", &rect::get_x, &rect::set_x)
        .property("y", &rect::get_y, &rect::set_y)
        .property("w", &rect::get_w, &rect::set_w)
        .property("width", &rect::get_w, &rect::set_w)
        .scope
        [
          def("create_c", &rect::create)
        ],
      class_<text, base>("text")
        .scope
        [
          def("create", &text::create)
        ]
    ]
  ];
  object lev = globals(L)["lev"];
  object classes = lev["classes"];
  object prim = lev["prim"];

  register_to(classes["color"],  "create", &color::create_l);
  register_to(classes["point"],  "create", &point::create_l);
  register_to(classes["rect"],   "create", &rect::create_l);
  register_to(classes["size"],   "create", &size::create_l);
  register_to(classes["vector"], "create", &vector::create_l);

  prim["color"]    = classes["color"]["create"];
  prim["colour"]   = classes["color"]["create"];
  prim["point"]    = classes["point"]["create"];
  prim["position"] = classes["vector"]["create"];
  prim["rect"]     = classes["rect"]["create"];
  prim["size"]     = classes["size"]["create"];
  prim["vector"]   = classes["vector"]["create"];

  globals(L)["package"]["loaded"]["lev.prim"] = prim;
  return 0;
}

namespace lev
{

  color::color(const color &orig)
    : r(orig.r), g(orig.g), b(orig.b), a(orig.a) { }

  color::color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    : r(r), g(g), b(b), a(a) { }

  color::color(wxUint32 argb_code)
  {
    a = (argb_code & 0xFF000000) >> 24;
    r = (argb_code & 0x00FF0000) >> 16;
    g = (argb_code & 0x0000FF00) >>  8;
    b = (argb_code & 0x000000FF) >>  0;
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
    return (a << 24) | (r << 16) | (g << 8) | (b << 0);
  }

  const std::string color::get_codestr() const
  {
    char str[9];
    sprintf(str, "%02x%02x%02x%02x", a, r, g, b);
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


  vector::vector(const vector &orig)
    : x(orig.x), y(orig.y), z(orig.z) { }

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


  point::~point()
  {
    if (vertex)
    {
      delete vertex;
      vertex = NULL;
    }
    if (col)
    {
      delete col;
      col = NULL;
    }
  }


  bool point::clear_color()
  {
    if (col)
    {
      delete col;
      col = NULL;
    }
    return true;
  }

  point* point::create(vector *vec, color *col)
  {
    point *pt = NULL;
    try {
      if (vec == NULL) { throw -1; }
      pt = new point;
      pt->vertex = new vector(*vec);
      if (col) { pt->col = new color(*col); }
      return pt;
    }
    catch (...) {
      return NULL;
    }
  }

  int point::create_l(lua_State *L)
  {
    using namespace luabind;
    object ver, col;
    unsigned char r = 0, g = 0, b = 0, a = 255;

    object t = util::get_merged(L, 1, -1);

    if (t["udata1"])
    {
      switch ( object_cast<int>(t["udata1"]["type_id"]) )
      {
        case LEV_TCOLOR:
          col = t["udata1"];
          break;
        case LEV_TVECTOR:
          ver = t["udata1"];
          break;
      }
    }
    if (t["udata2"])
    {
      switch ( object_cast<int>(t["udata2"]["type_id"]) )
      {
        case LEV_TCOLOR:
          col = t["udata2"];
          break;
        case LEV_TVECTOR:
          ver = t["udata2"];
          break;
      }
    }

    if (t["vertex"]) { ver = t["vertex"]; }
    else if (t["ver"]) { ver = t["ver"]; }
    else if (t["v"]) { ver = t["v"]; }

    if (not ver) { ver = globals(L)["lev"]["classes"]["vector"]["create"](t); }

    if (t["color"]) { col = t["color"]; }
    else if (t["col"]) { col = t["col"]; }
    else if (t["c"]) { col = t["c"]; }

    if (not col)
    {
      if (t["red"] || t["r"] || t["green"] || t["g"] || t["blue"] || t["b"])
      {
        col = globals(L)["lev"]["classes"]["color"]["create"](t);
      }
    }

    object o = globals(L)["lev"]["classes"]["point"]["create_c"](ver, col);
    o.push(L);
    return 1;
  }

  bool point::set_color(color *c)
  {
    clear_color();
    if (c == NULL) { return true; }
    try {
      col = new color(*c);
      return true;
    }
    catch (...) {
      return false;
    }
  }

  bool point::set_vertex(vector *v)
  {
    if (v == NULL) { return false; }
    *vertex = *v;
    return true;
  }


  rect::~rect()
  {
    if (pos)
    {
      delete pos;
      pos = NULL;
    }
    if (sz)
    {
      delete sz;
      sz = NULL;
    }
  }

  rect* rect::create(int x, int y, int w, int h)
  {
    rect *r = NULL;
    try {
      r = new rect;
      r->pos = new vector(x, y);
      r->sz  = new size(w, h);
      return r;
    }
    catch (...) {
      delete r;
      return NULL;
    }
  }

  int rect::create_l(lua_State *L)
  {
    using namespace luabind;
    int x = 0, y = 0, w = 0, h = 0;

    object t = util::get_merged(L, 1, -1);

    if (t["x"]) { x = object_cast<int>(t["x"]); }
    else if (t["num1"]) { x = object_cast<int>(t["num1"]); }

    if (t["y"]) { y = object_cast<int>(t["y"]); }
    else if (t["num2"]) { y = object_cast<int>(t["num2"]); }

    if (t["width"]) { w = object_cast<int>(t["width"]); }
    else if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["num3"]) { w = object_cast<int>(t["num3"]); }

    if (t["height"]) { h = object_cast<int>(t["height"]); }
    else if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["num4"]) { h = object_cast<int>(t["num4"]); }

    if (t["lev.prim.vector1"])
    {
      object vec1 = t["lev.prim.vector1"];
      if (t["lev.prim.vector2"])
      {
        object vec2 = t["lev.prim.vector2"];
        int x1 = object_cast<int>(vec1["x"]);
        int y1 = object_cast<int>(vec1["y"]);
        int x2 = object_cast<int>(vec2["x"]);
        int y2 = object_cast<int>(vec2["y"]);
        int max_x = x1 > x2 ? x1 : x2;
        int min_x = x1 < x2 ? x1 : x2;
        int max_y = y1 > y2 ? y1 : y2;
        int min_y = y1 < y2 ? y1 : y2;
        x = min_x;
        y = min_y;
        w = max_x - min_x;
        h = max_y - min_y;
      }
      else
      {
        x = object_cast<int>(vec1["x"]);
        y = object_cast<int>(vec1["y"]);
      }
    }

    if (t["lev.prim.size1"])
    {
      object sz = t["lev.prim.size1"];
      w = object_cast<int>(sz["w"]);
      h = object_cast<int>(sz["h"]);
    }

    object o = globals(L)["lev"]["classes"]["rect"]["create_c"](x, y, w, h);
    o.push(L);
    return 1;
  }

  bool rect::set_position(vector *vec)
  {
    if (vec == NULL) { return false; }
    try {
      pos = new vector(*vec);
      return true;
    }
    catch (...) {
      return false;
    }
  }

  bool rect::set_size(size *new_size)
  {
    if (sz == NULL) { return false; }
    try {
      sz = new size(*new_size);
      return true;
    }
    catch (...) {
      return false;
    }
  }


  text::text() : str() { }

  text::~text() { }

  text* text::create(const char *str)
  {
    text* t = NULL;
    try {
      t = new text();
      t->str = str;
      return t;
    }
    catch (...) {
      delete t;
      return NULL;
    }
  }

}

