#ifndef _PRIM_HPP
#define _PRIM_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/prim.hpp
// Purpose:     header for primitive expressions
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     01/05/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"

#include <luabind/luabind.hpp>
#include <string>
#include <wx/wx.h>

extern "C" {
  int luaopen_lev_prim(lua_State *L);
}

namespace lev
{

  class color : public base
  {
    public:
      color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255);
      color(wxUint32 argb_code);
      static color* create(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
      static int create_l(lua_State *L);
      unsigned char get_a() const { return _a; }
      unsigned char get_b() const { return _b; }
      unsigned char get_g() const { return _g; }
      unsigned char get_r() const { return _r; }
      wxUint32 get_code32() const;
      const std::string get_codestr() const;
      bool set_a(unsigned char a) { _a = a; return true; }
      bool set_b(unsigned char b) { _b = b; return true; }
      bool set_g(unsigned char g) { _g = g; return true; }
      bool set_r(unsigned char r) { _r = r; return true; }
      virtual type_id get_type_id() const { return LEV_TCOLOR; }
      virtual const char *get_type_name() const { return "lev.color"; }

      static const color black() { return color(0, 0, 0, 255); }
      static const color blue() { return color(0, 0, 255, 255); }
      static const color green() { return color(0, 255, 0, 255); }
      static const color red() { return color(255, 0, 0, 255); }
      static const color transparent() { return color(0, 0, 0, 0); }

    private:
      unsigned char _r, _g, _b, _a;
  };


  class size : public base
  {
    public:
      size(int w, int h, int d = 0) : _w(w), _h(h), _d(d), base() {}
      static size* create(int w, int h, int d = 0);
      static int create_l(lua_State *L);
      inline int get_d() const { return _d; }
      inline int get_h() const { return _h; }
      inline int get_w() const { return _w; }
      virtual type_id get_type_id() const { return LEV_TSIZE; }
      virtual const char *get_type_name() const { return "lev.size"; }
      inline void set_d(int d) { _d = d; }
      inline void set_h(int h) { _h = h; }
      inline void set_w(int w) { _w = w; }
    private:
      int _w;
      int _h;
      int _d;
  };


  class vector : public base
  {
    public:
      inline vector(int x = 0, int y = 0, int z = 0)
        : _x(x), _y(y), _z(z) {}
      static vector* create(int x = 0, int y = 0, int z = 0);
      static int create_l(lua_State *L);
      inline int get_x() const { return _x; }
      inline int get_y() const { return _y; }
      inline int get_z() const { return _z; }
      virtual type_id get_type_id() const { return LEV_TVECTOR; }
      virtual const char *get_type_name() const { return "lev.vector"; }
      inline void set_x(int x) { _x = x; }
      inline void set_y(int y) { _y = y; }
      inline void set_z(int z) { _z = z; }
      inline vector operator+(vector &rhs)
      { return vector(_x + rhs._x, _y + rhs._y, _z + rhs._z); }
    private:
      int _x;
      int _y;
      int _z;
  };

}

#endif // _PRIM_HPP

