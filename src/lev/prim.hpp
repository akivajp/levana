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
      unsigned char get_a() const { return _a; }
      unsigned char get_b() const { return _b; }
      unsigned char get_g() const { return _g; }
      unsigned char get_r() const { return _r; }
      wxUint32 get_code32() const;
      std::string get_codestr() const;
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

  class vector
  {
    public:
      inline vector(int x = 0, int y = 0, int z = 0)
        : _x(x), _y(y), _z(z) {}
      inline int getx() const { return _x; }
      inline int gety() const { return _y; }
      inline int getz() const { return _z; }
      inline void setx(int x) { _x = x; }
      inline void sety(int y) { _y = y; }
      inline void setz(int z) { _z = z; }
      inline vector operator+(vector &rhs)
      { return vector(_x + rhs._x, _y + rhs._y, _z + rhs._z); }
    private:
      int _x;
      int _y;
      int _z;
  };


  class size
  {
    public:
      inline size(int w, int h, int d = 0) : _w(w), _h(h), _d(d) {}
      inline int getd() const { return _d; }
      inline int geth() const { return _h; }
      inline int getw() const { return _w; }
      inline void setd(int d) { _d = d; }
      inline void seth(int h) { _h = h; }
      inline void setw(int w) { _w = w; }
    private:
      int _w;
      int _h;
      int _d;
  };
}

#endif // _PRIM_HPP

