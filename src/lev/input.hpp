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

extern "C" {
  int luaopen_lev_input(lua_State *L);
}

namespace lev
{

  class inbase : public base
  {
    protected:
      inbase() {}
      virtual ~inbase() {}
    public:
      virtual bool get_left_down() const = 0;
      virtual bool get_middle_down() const = 0;
      virtual bool get_right_down() const = 0;
      virtual type_id get_type_id() const { return LEV_TINPUT; }
      virtual const char *get_type_name() const { return "lev.input.inbase"; }
  };

  class instate : public inbase
  {
    public:
      instate();
      virtual ~instate();
      int get_x() const;
      int get_y() const;
      virtual bool get_left_down() const;
      virtual bool get_middle_down() const;
      virtual bool get_right_down() const;
      virtual type_id get_type_id() const { return LEV_TINSTATE; }
      virtual const char *get_type_name() const { return "lev.input.instate"; }

      friend class application;
    private:
      void *_obj;
  };

  class inrecord : public inbase
  {
    public:
      inrecord();
      virtual ~inrecord();
      bool clear();
      virtual bool get_left_down() const;
      virtual bool get_middle_down() const;
      virtual bool get_right_down() const;
      virtual type_id get_type_id() const { return LEV_TINRECORD; }
      virtual const char *get_type_name() const { return "lev.inrecord"; }
      bool set_left_down();
      bool set_middle_down();
      bool set_right_down();

      friend class application;
    private:
      void *_obj;
  };

}

#endif // _INPUT_HPP

