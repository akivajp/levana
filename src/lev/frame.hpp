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
      virtual ~frame() { }
      static frame *get_top();
      bool close(bool force);
      bool close_noforce() { return close(false); }
      static frame *create(control *parent, const char *title, int w, int h, long style);
      static int create_l(lua_State *L);
      void fit();
      luabind::object get_menubar();
      virtual luabind::object get_on_key_down();
      virtual luabind::object get_on_key_up();
      const char *get_status();
      const char *get_title();
      virtual type_id get_type_id() const { return LEV_TFRAME; }
      virtual const char *get_type_name() const { return "frame"; }
      bool set_icon(image *i);
      bool set_menubar(luabind::object mb);
      virtual bool set_on_key_down(luabind::object func);
      virtual bool set_on_key_up(luabind::object func);
      void set_status(const char *str_status);
      void set_title(const char *title);
      static void set_top(frame *top);
    private:
      const char *status;
  };

}

#endif // _FRAME_HPP

