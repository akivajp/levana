#ifndef _FRAME_HPP
#define _FRAME_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/frame.hpp
// Purpose:     header for frame managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "menu.hpp"
#include "icon.hpp"
#include "event.hpp"

#include <luabind/luabind.hpp>
#include <string>

namespace lev
{

  class frame : public control
  {
    protected:
      frame() : control(), status(NULL) { }
    public:
      ~frame() { }
      static frame *gettop();
      static void settop(frame *top);
      bool close(bool force);
      bool close_noforce() { return close(false); }
      static frame *create(control *parent, const char *title, int w, int h, long style);
      static int create_l(lua_State *L);
      void fit();
      luabind::object get_onkeydown();
      virtual type_id get_type_id() const { return LEV_TFRAME; }
      virtual const char *get_type_name() const { return "frame"; }
      void seticon(const icon &i);
      void set_menubar(menubar *mb);
      static int set_menubar_l(lua_State *L);
      bool set_onkeydown(luabind::object lua_func);
      bool set_onmenu(int id, luabind::object lua_func);
      // statusbar property
      const char *getstatus();
      void setstatus(const char *str_status);
      // title property
      const char *gettitle();
      void settitle(const char *title);
    private:
      const char *status;
  };

}

#endif // _FRAME_HPP

