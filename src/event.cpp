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
  object handler::get_on_char() { return get_func(wxEVT_CHAR); }
  object handler::get_on_close() { return get_func(wxEVT_CLOSE_WINDOW); }
  object handler::get_on_idle() { return get_func(wxEVT_IDLE); }
  object handler::get_on_key_down() { return get_func(wxEVT_KEY_DOWN); }
  object handler::get_on_key_up() { return get_func(wxEVT_KEY_UP); }
  object handler::get_on_left_down() { return get_func(wxEVT_LEFT_DOWN); }
  object handler::get_on_menu(int id)
  {
    return get_func(id, wxEVT_COMMAND_MENU_SELECTED);
  }
  object handler::get_on_motion() { return get_func(wxEVT_MOTION); }
  object handler::get_on_paint() { return get_func(wxEVT_PAINT); }
  object handler::get_on_right_down() { return get_func(wxEVT_RIGHT_DOWN); }

  bool handler::hold()
  {
    if (system_managed) { return false; }
    system_managed = true;
    return true;
  }

  bool handler::set_on_any(object lua_func) { return connect(-1, lua_func); }
  bool handler::set_on_char(object lua_func) { return connect(wxEVT_CHAR, lua_func); }
  bool handler::set_on_close(object lua_func) { return connect(wxEVT_CLOSE_WINDOW, lua_func); }
  bool handler::set_on_idle(object lua_func) { return connect(wxEVT_IDLE, lua_func); }
  bool handler::set_on_key_down(object lua_func) { return connect(wxEVT_KEY_DOWN, lua_func); }
  bool handler::set_on_key_up(object lua_func) { return connect(wxEVT_KEY_UP, lua_func); }
  bool handler::set_on_left_down(object lua_func) { return connect(wxEVT_LEFT_DOWN, lua_func); }
  bool handler::set_on_menu(int id, object lua_func)
  {
    return connect(id, wxEVT_COMMAND_MENU_SELECTED, lua_func);
  }
  bool handler::set_on_motion(object lua_func) { return connect(wxEVT_MOTION, lua_func); }
  bool handler::set_on_paint(object lua_func) { return connect(wxEVT_PAINT, lua_func); }
  bool handler::set_on_right_down(object lua_func) { return connect(wxEVT_RIGHT_DOWN, lua_func); }


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

  const char *event::get_char() const
  {
    const char character[2] = {((wxKeyEvent *)_obj)->GetKeyCode(), '\0'};
    std::string str(character);
    return str.c_str();
  }

  const char *event::get_keystr() const
  {
    return input::to_keystr( ((wxKeyEvent *)_obj)->GetKeyCode() );
  }

  long event::get_keycode() const
  {
    return ((wxKeyEvent *)_obj)->GetKeyCode();
  }

  int event::get_x() const
  {
    return ((wxMouseEvent *)_obj)->GetX();
  }

  int event::get_y() const
  {
    return ((wxMouseEvent *)_obj)->GetY();
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

