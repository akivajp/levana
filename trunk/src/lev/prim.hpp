#ifndef _PRIM_HPP
#define _PRIM_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/prim.hpp
// Purpose:     header for primitive expressions
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     01/05/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

namespace levana
{
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

