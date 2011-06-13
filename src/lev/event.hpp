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

namespace luabind { class object; }

namespace lev
{
  class control : public base
  {
    protected:
      control();
      virtual ~control();
    public:
      virtual bool connect(int type, luabind::object lua_func) { return false; }
      virtual luabind::object get_onany() { return luabind::object(); }
      virtual luabind::object get_onidle() { return luabind::object(); }
      virtual luabind::object get_onmenu(int id) { return luabind::object(); }
      virtual luabind::object get_onkeydown() { return luabind::object(); }
      void *get_rawobj() { return _obj; }
      virtual bool set_onany(luabind::object lua_func) { return false; }
      virtual bool set_onidle(luabind::object lua_func) { return false; }
      virtual bool set_onkeydown(luabind::object lua_func) { return false; }
      virtual bool set_onmenu(int id, luabind::object lua_func) { return false; }
      int getid();
      sizer *getsizer();
      virtual type_id get_type_id() const { return LEV_TCONTROL; }
      virtual const char *get_type_name() const { return "control"; }
      inline bool hide() { return setshown(false); }
      bool isshown();
      bool isvalid();
      void setsizer(sizer *s);
      bool setshown(bool showing);
      inline bool show() { return setshown(true); }

      // friend classes
//      friend class canvas;
//      friend class frame;
//      friend class htmlview;
//      friend class player;
//      friend class sizer;
//      friend class textbox;

    protected:
      bool wx_managed;
      sizer *_sz;
      void *_obj;
      int _id;
  };


  class event : public base
  {
    public:
      event(void *e);
      ~event();
      int get_id() const;
      const char *get_key() const;
      int get_keycode() const;
      bool request();
      void skip();
    protected:
      void *_obj;
  };
}

#endif // _EVENT_HPP

