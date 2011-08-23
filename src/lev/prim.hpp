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
      color(const color &orig);
      color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255);
      color(wxUint32 argb_code);
      static color* create(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
      static int create_l(lua_State *L);
      unsigned char get_a() const { return a; }
      unsigned char get_b() const { return b; }
      unsigned char get_g() const { return g; }
      unsigned char get_r() const { return r; }
      wxUint32 get_code32() const;
      const std::string get_codestr() const;
      bool set_a(unsigned char new_a) { a = new_a; return true; }
      bool set_b(unsigned char new_b) { b = new_b; return true; }
      bool set_g(unsigned char new_g) { g = new_g; return true; }
      bool set_r(unsigned char new_r) { r = new_r; return true; }
      virtual type_id get_type_id() const { return LEV_TCOLOR; }
      virtual const char *get_type_name() const { return "lev.prim.color"; }

      static const color black() { return color(0, 0, 0, 255); }
      static const color blue() { return color(0, 0, 255, 255); }
      static const color green() { return color(0, 255, 0, 255); }
      static const color red() { return color(255, 0, 0, 255); }
      static const color white() { return color(255, 255, 255, 255); }
      static const color transparent() { return color(0, 0, 0, 0); }

    protected:
      unsigned char r, g, b, a;
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
      virtual const char *get_type_name() const { return "lev.prim.size"; }
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
      vector(const vector &orig);
      vector(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) { }
      static vector* create(int x = 0, int y = 0, int z = 0);
      static int create_l(lua_State *L);
      inline int get_x() const { return x; }
      inline int get_y() const { return y; }
      inline int get_z() const { return z; }
      virtual type_id get_type_id() const { return LEV_TVECTOR; }
      virtual const char *get_type_name() const { return "lev.prim.vector"; }
      inline void set_x(int new_x) { x = new_x; }
      inline void set_y(int new_y) { y = new_y; }
      inline void set_z(int new_z) { z = new_z; }
      vector operator+(vector &rhs)
      { return vector(x + rhs.x, y + rhs.y, z + rhs.z); }
    protected:
      int x;
      int y;
      int z;
  };


  class point : public base
  {
    protected:
      point() : vertex(NULL), col(NULL) { }
    public:
      virtual ~point();
      bool clear_color();
      static point* create(vector *vec, color *col = NULL);
      static int create_l(lua_State *L);
      color*  get_color()  { return col; }
      vector* get_vertex() { return vertex; }
      virtual type_id get_type_id() const { return LEV_TPOINT; }
      virtual const char *get_type_name() const { return "lev.prim.point"; }
      bool set_color(color *c);
      bool set_vertex(vector *v);
    protected:
      vector *vertex;
      color *col;
  };

  class text : public base
  {
    protected:
      text();
    public:
      virtual ~text();
      static text* create(const char *str);
//      virtual type_id get_type_id() const { return LEV_TEXT; }
      virtual const char *get_type_name() const { return "lev.prim.text"; }
    protected:
      std::string str;
  };

}

#endif // _PRIM_HPP

