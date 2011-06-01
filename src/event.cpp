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

  control::control() : base(), _id(0), _obj(NULL), _sz(NULL), _managing(false)
  { }

  control::~control()
  {
    if (_id && !isvalid())
    {
      // object is in wxWidgets control, and already deleted
    }
    else if (_managing)
    {
      // the object is under self management
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
      ((wxWindow *)_obj)->SetSizer((wxSizer *)s->_obj);
      _sz = s;
      s->_managing = false;
    }
  }

  bool control::setshown(bool showing)
  {
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

  int event::get_key() const
  {
    return ((wxKeyEvent *)_obj)->GetKeyCode();
  }

  void event::skip()
  {
    ((wxEvent *)_obj)->Skip(true);
  }
}

