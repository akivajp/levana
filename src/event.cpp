/////////////////////////////////////////////////////////////////////////////
// Name:        src/connect.cpp
// Purpose:     source for event call back handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/28/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/app.hpp"
#include "lev/event.hpp"
#include "lev/sizer.hpp"

namespace lev
{

  handler::handler() : base(), _obj(NULL), system_managed(false) { }

  handler::~handler()
  {
    if (system_managed)
    {
      // wxWidgets will automatically delete
    }
    else
    {
      // the object is under our management
      delete (wxEvtHandler *)_obj;
      _obj = NULL;
    }
  }

  bool handler::connect(int id, int type, luabind::object lua_func)
  {
    if (not connector) { return false; }
    connector(id, type, lua_func);
    return true;
  }

  luabind::object handler::get_func(int id, int type)
  {
    if (not func_getter) { return luabind::object(); }
    return func_getter(id, type);
  }

  object handler::get_on_any() { return get_func(-1); }
  object handler::get_on_close() { return get_func(wxEVT_CLOSE_WINDOW); }
  object handler::get_on_idle() { return get_func(wxEVT_IDLE); }
  object handler::get_on_keydown() { return get_func(wxEVT_KEY_DOWN); }

  bool handler::set_on_any(object lua_func) { return connect(-1, lua_func); }
  bool handler::set_on_close(object lua_func) { return connect(wxEVT_CLOSE_WINDOW, lua_func); }
  bool handler::set_on_idle(object lua_func) { return connect(wxEVT_IDLE, lua_func); }
  bool handler::set_on_keydown(object lua_func) { return connect(wxEVT_KEY_DOWN, lua_func); }
  bool handler::set_on_menu(int id, object lua_func)
  {
    return connect(id, wxEVT_COMMAND_MENU_SELECTED, lua_func);
  }

  // event methods
  event::event(void *e) : base()
  {
    _obj = e;
  }

  event::~event() {}

  int event::get_id() const
  {
    return ((wxEvent *)_obj)->GetId();
  }

  const char *event::get_keystr() const
  {
    return input::to_keystr( ((wxKeyEvent *)_obj)->GetKeyCode() );
  }

  long event::get_keycode() const
  {
    return ((wxKeyEvent *)_obj)->GetKeyCode();
  }

  bool event::request()
  {
    ((wxIdleEvent *)_obj)->RequestMore(true);
    return true;
  }

  void event::skip()
  {
    ((wxEvent *)_obj)->Skip(true);
  }
}

