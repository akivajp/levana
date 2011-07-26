#ifndef _MENU_HPP
#define _MENU_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/frame.hpp
// Purpose:     header for menu and menubar handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/02/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "gui.hpp"
#include <lua.h>
#include <luabind/luabind.hpp>

namespace lev
{

  class menuitem : public base
  {
    protected:
      menuitem() : base(), _obj(NULL), system_managed(false) { }
    public:
      virtual ~menuitem();
      static menuitem* create(const char *text, const char *help, long kind);
      static int create_l(lua_State *L);
      bool disable() { return enable(false); }
      bool enable_on() { return enable(true); }
      bool enable(bool enabling = true);
      int get_id();
      void *get_rawobj() { return _obj; }
//      virtual type_id get_type_id() const { return LEV_TMENUITEM; }
      virtual const char *get_type_name() const { return "lev.gui.menuitem"; }
      bool hold();
    protected:
      void *_obj;
      bool system_managed;
  };

  class menu : public handler
  {
    private:
      menu();
    public:
      virtual ~menu();
      int append(int id, const char *str, const char *help_str = "");
      static int append_l(lua_State *L);
      static luabind::object build(luabind::object setting);
      static menu* create(const char *title);
      static int create_l(lua_State *L);
      bool enable(int id, bool enabling = true);
      virtual type_id get_type_id() const { return LEV_TMENU; }
      virtual const char *get_type_name() const { return "lev.gui.menu"; }
      friend class systray;
      friend class menubar;
  };

  class menubar : public control
  {
    private:
      menubar() : control() { }
    public:
      virtual ~menubar() { }
      bool append(menu *m, const char *title);
      static int append_l(lua_State *L);
      static luabind::object build(luabind::object setting);
      static menubar *create();
      static int create_l(lua_State *L);
      bool enable(int id, bool enabling = true);
      virtual type_id get_type_id() const { return LEV_TMENUBAR; }
      virtual const char *get_type_name() const { return "lev.gui.menubar"; }
  };

}

#endif // _MENU_HPP

