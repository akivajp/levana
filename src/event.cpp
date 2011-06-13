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

  control::control() : base(), _id(0), _obj(NULL), _sz(NULL), wx_managed(false)
  { }

  control::~control()
  {
    if (wx_managed)
    {
      // wxWidgets will delete
    }
    else if (_id && !isvalid())
    {
      // the object isn't already found
    }
    else
    {
      // the object is under our management
      delete (wxWindow *)_obj;
    }
  }

  int control::getid()
  {
    return _id;
  }

  sizer *control::getsizer()
  {
    return _sz;
  }

  bool control::isshown()
  {
    return ((wxWindow *)_obj)->IsShown();
  }

  bool control::isvalid()
  {
    if (wxWindow::FindWindowById(_id)) { return true; }
    return false;
  }

  void control::setsizer(sizer *s)
  {
    if (s)
    {
      ((wxWindow *)_obj)->SetSizer((wxSizer *)s->get_rawobj());
      _sz = s;
      s->wx_managed = true;
    }
  }

  bool control::setshown(bool showing)
  {
    gui_lock();
    return ((wxWindow *)_obj)->Show(showing);
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

  const char *event::get_key() const
  {
    switch ( ((wxKeyEvent *)_obj)->GetKeyCode() )
    {
      default:  return "";
    }
  }

  int event::get_keycode() const
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

