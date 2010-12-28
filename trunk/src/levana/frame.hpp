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

#include "base.hpp"
#include "menu.hpp"
#include "icon.hpp"
#include "event.hpp"

namespace luabind { class object; }

namespace levana
{

  class frame : public base, public handler
  {
    public:
      inline frame() : base() {}
      frame(frame *parent, int id, const char *title,
            int x = -1, int y = -1, int w = -1, int h = -1,
            long style = -1, const char *name = "frame");
      ~frame();
      inline bool close(/* false */) { return close(false); }
      bool close(bool force);
      void seticon(const icon &i);
      void setmenubar(menubar *mb);
      void setonmenu(int id, luabind::object lua_func);
      inline bool show(/* true */) { return show(true); }
      bool show(bool bShow);
      // title property
      const char *gettitle();
      void settitle(const char *title);
      // static function
      static frame *gettop();
      static void settop(frame *top);
      friend class draw;
    private:
      bool create(frame *parent, int id, const char *title,
                  int x = -1, int y = -1, int w = -1, int h = -1,
                  long style = -1, const char *name = "frame");
  };

}

#endif // _FRAME_HPP

