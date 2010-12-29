#ifndef _FRAME_HPP
#define _FRAME_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/frame.hpp
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

namespace levana
{

  class frame : public control
  {
    public:
      inline frame() : control(), status(NULL) {}
      frame(frame *parent, int id, const char *title,
            int x = -1, int y = -1, int w = -1, int h = -1,
            long style = -1, const char *name = "frame");
      ~frame();
      bool close(bool force);
      bool close_noforce() { return close(false); }
      void fit();
      void seticon(const icon &i);
      void setmenubar(menubar *mb);
      static void setmenubar(frame *f, menubar *mb);
      void setonmenu(int id, luabind::object lua_func);
//      static void setonmenu(frame *f, int id, luabind::object lua_func);
      bool show(bool showing);
      bool show_true() { return show(true); }
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
      bool create(frame *parent, int id, const char *title,
                  int x = -1, int y = -1, int w = -1, int h = -1,
                  long style = -1, const char *name = "frame");
      const char *status;
  };

}

#endif // _FRAME_HPP

