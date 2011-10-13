#ifndef _EVENT_HPP
#define _EVENT_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/connect.cpp
// Purpose:     header for event call back handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/28/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include "sizer.hpp"
#include <boost/function.hpp>
#include <luabind/luabind.hpp>

namespace lev
{
  using namespace luabind;

  class handler : public base
  {
    protected:
      handler();
      virtual ~handler();
    public:
      bool connect(int type, object lua_func) { return connect(-1, type, lua_func); }
      bool connect(int id, int type, object lua_func);
      virtual object get_func(int type) { return get_func(-1, type); }
      virtual object get_func(int id, int type);
      virtual object get_on_any();
      virtual object get_on_char();
      virtual object get_on_close();
      virtual object get_on_idle();
      virtual object get_on_key_down();
      virtual object get_on_key_up();
      virtual object get_on_left_down();
      virtual object get_on_menu(int id);
      virtual object get_on_motion();
      virtual object get_on_paint();
      virtual object get_on_right_down();
      void *get_rawobj() { return _obj; }
      virtual type_id get_type_id() const { return LEV_THANDLER; }
      virtual const char *get_type_name() const { return "lev.handler"; }
      bool hold();
      virtual bool set_on_any(object lua_func);
      virtual bool set_on_char(object lua_func);
      virtual bool set_on_close(object lua_func);
      virtual bool set_on_idle(object lua_func);
      virtual bool set_on_key_down(object lua_func);
      virtual bool set_on_key_up(object lua_func);
      virtual bool set_on_left_down(object lua_func);
      virtual bool set_on_menu(int id, object lua_func);
      virtual bool set_on_motion(object lua_func);
      virtual bool set_on_paint(object lua_func);
      virtual bool set_on_right_down(object lua_func);

    protected:
      boost::function<void (int, int, luabind::object)> connector;
      boost::function<luabind::object (int, int)> func_getter;
      bool system_managed;
      void *_obj;
  };

  class event : public base
  {
    public:
      event(void *e);
      ~event();
      const char *get_char() const;
      int get_id() const;
      const char *get_keystr() const;
      long get_keycode() const;
      virtual type_id get_type_id() const { return LEV_TEVENT; }
      virtual const char *get_type_name() const { return "lev.event"; }
      int get_x() const;
      int get_y() const;
      bool request();
      void skip();
    protected:
      void *_obj;
  };
}

#endif // _EVENT_HPP

