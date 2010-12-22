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

namespace luabind { class object; }

namespace levana
{

  class frame : public base
  {
    public:
      inline frame() : base() {}
      frame(frame *parent, int id, const char *title,
            int x = -1, int y = -1, int w = -1, int h = -1,
            long style = -1, const char *name = "frame");
      ~frame();
      inline bool close(/* false */) { return close(false); }
      bool close(bool force);
      void connect_menu(int id, luabind::object lua_func);
      const char *get_title();
      void set_icon(const icon &i);
      void set_menubar(menubar *mb);
      void set_title(const char *title);
      inline bool show(/* true */) { return show(true); }
      bool show(bool bShow);
      static frame *get_top();
      static void set_top(frame *top);
    private:
      bool create(frame *parent, int id, const char *title,
                  int x = -1, int y = -1, int w = -1, int h = -1,
                  long style = -1, const char *name = "frame");
  };

}

#endif // _FRAME_HPP

