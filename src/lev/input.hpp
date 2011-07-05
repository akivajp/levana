#ifndef _INPUT_HPP
#define _INPUT_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        lev/input.hpp
// Purpose:     header for input managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     06/08/2011
// Copyright:   (C) 2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include <lua.h>
#include <string>

extern "C" {
  int luaopen_lev_input(lua_State *L);
}

namespace lev
{

  class input
  {
    public:
      static long to_keycode(const char *keystr);
      static const char *to_keystr(long code);
  };

  class instate : public base
  {
    public:
      instate();
      virtual ~instate();
      int get_x() const;
      int get_y() const;
      bool get_left_down() const;
      bool get_middle_down() const;
      bool get_right_down() const;
      virtual type_id get_type_id() const { return LEV_TINSTATE; }
      virtual const char *get_type_name() const { return "lev.input.instate"; }

      friend class application;
    private:
      void *_obj;
  };

  class inrecord : public base
  {
    public:
      inrecord();
      virtual ~inrecord();
      bool clear();
      bool get_keydown(const char *keystr) const;
      bool get_left_down() const;
      bool get_middle_down() const;
      bool get_right_down() const;
      int get_x() const;
      int get_y() const;
      virtual type_id get_type_id() const { return LEV_TINRECORD; }
      virtual const char *get_type_name() const { return "lev.inrecord"; }
      bool record();
      bool track_key(const char *keystr);
      bool track_mouse();

      friend class application;
    private:
      void *_obj;
  };

}

#endif // _INPUT_HPP

