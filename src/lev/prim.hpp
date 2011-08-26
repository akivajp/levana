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
      size(int w, int h, int d = 0) : w(w), h(h), d(d), base() {}
      static size* create(int w, int h, int d = 0);
      static int create_l(lua_State *L);
      int get_d() const { return d; }
      int get_h() const { return h; }
      int get_w() const { return w; }
      virtual type_id get_type_id() const { return LEV_TSIZE; }
      virtual const char *get_type_name() const { return "lev.prim.size"; }
      bool set_d(int new_d) { d = new_d; return true; }
      bool set_h(int new_h) { h = new_h; return true; }
      bool set_w(int new_w) { w = new_w; return true; }
    private:
      int w;
      int h;
      int d;
  };


  class vector : public base
  {
    public:
      vector(const vector &orig);
      vector(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) { }
      static vector* create(int x = 0, int y = 0, int z = 0);
      static int create_l(lua_State *L);
      int get_x() const { return x; }
      int get_y() const { return y; }
      int get_z() const { return z; }
      virtual type_id get_type_id() const { return LEV_TVECTOR; }
      virtual const char *get_type_name() const { return "lev.prim.vector"; }
      bool set_x(int new_x) { x = new_x; }
      bool set_y(int new_y) { y = new_y; }
      bool set_z(int new_z) { z = new_z; }
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

  class rect : public base
  {
    protected:
      rect() : pos(NULL), sz(NULL) { }
    public:
      virtual ~rect();
      static rect* create(int x, int y, int w, int h);
      static int create_l(lua_State *L);
      int get_h() { return sz->get_h(); }
      vector* get_position() { return pos; }
      size*   get_size() { return sz; }
      int get_x() { return pos->get_x(); }
      int get_y() { return pos->get_y(); }
      int get_w() { return sz->get_w(); }
      virtual type_id get_type_id() const { return LEV_TRECT; }
      virtual const char *get_type_name() const { return "lev.prim.rect"; }
      bool set_h(int h) { return sz->set_h(h); }
      bool set_position(vector *vec);
      bool set_size(size *new_size);
      bool set_x(int x) { return pos->set_x(x); }
      bool set_y(int y) { return pos->set_y(y); }
      bool set_w(int w) { return sz->set_w(w); }
    protected:
      vector *pos;
      size   *sz;
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

