#ifndef _GUI_HPP
#define _GUI_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/gui.hpp
// Purpose:     header for GUI controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "event.hpp"
#include <lua.h>

namespace lev
{
  class gui
  {
    public:
      static int msgbox_l(lua_State *L);
      static const char *file_selector(const char *message, const char *def_path, const char *def_file,
                                       const char *def_ext, const char *wildcard, control *parent);
      static int file_selector_l(lua_State *L);
  };

  class control : public handler
  {
    protected:
      control();
      virtual ~control();
    public:
      virtual luabind::object get_onmenu(int id) { return luabind::object(); }
      int get_height();
      int getid();
      sizer *get_sizer();
      long get_style();
      int get_width();
      virtual type_id get_type_id() const { return LEV_TCONTROL; }
      virtual const char *get_type_name() const { return "lev.gui.control"; }
      bool hide() { return set_shown(false); }
      bool is_shown();
      bool is_valid();
      bool set_height(int h);
      void set_sizer(sizer *s);
      bool set_shown(bool showing);
      bool set_width(int w);
      bool show() { return set_shown(true); }

    protected:
      sizer *_sz;
      int _id;
  };

  class htmlview: public control
  {
    public:
      static htmlview* create(control *parent, int width, int height);
      static int create_l(lua_State *L);
      const char *get_page();
      virtual type_id get_type_id() const { return LEV_THTMLVIEW; }
      virtual const char *get_type_name() const { return "lev.gui.htmlview"; }
      bool loadpage(const char *url);
      bool set_page(const char *src);
      const char *totext();
  };

  class textbox: public control
  {
    public:
      static textbox* create(control *parent, int width, int height, const char *value = "", long style = 0);
      static int create_l(lua_State *L);
      virtual type_id get_type_id() const { return LEV_TTEXTBOX; }
      virtual const char *get_type_name() const { return "lev.gui.textbox"; }
      const char* get_value();
      bool set_multiline();
      void set_value(const char *value);
  };

}

#endif // _GUI_HPP

