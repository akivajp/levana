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
#include <string>

namespace luabind { class object; }

namespace lev
{

  class frame : public control
  {
    public:
      inline frame() : control(), status(NULL) {}
      frame(frame *parent, const char *title, int w = -1, int h = -1, long style = -1);
      bool close(bool force);
      bool close_noforce() { return close(false); }
      void fit();
      void seticon(const icon &i);
      void setmenubar(menubar *mb);
      void setonmenu(int id, luabind::object lua_func);
      // statusbar property
      const char *getstatus();
      void setstatus(const char *str_status);
      // title property
      const char *gettitle();
      void settitle(const char *title);
      // static function
      static frame *gettop();
      static void settop(frame *top);
    private:
      const char *status;
  };

}

#endif // _FRAME_HPP

