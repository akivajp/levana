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

  control::control() : base(), _id(0), _obj(NULL), _sz()
  { }

  control::~control()
  {
    if (_id && !isvalid())
    {
      // object is in wxWidgets control, and already deleted
    }
    else
    {
      delete (wxWindow *)_obj;
    }
  }

  int control::getid()
  {
    return _id;
  }

  sizer *control::getsizer()
  {
    wxSizer *sz = ((wxWindow *)_obj)->GetSizer();
    if (sz == NULL) { return NULL; }
    this->_sz._obj = sz;
    return &(this->_sz);
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
      s->_adopt = false;
    }
  }

  bool control::setshown(bool showing)
  {
    return ((wxWindow *)_obj)->Show(showing);
  }


  // event methods
  event::event(void *e) : base()
  {
    _obj.reset((wxEvent *)e);
  }


  int event::getkey() const
  {
    return ((wxKeyEvent *)_obj.get())->GetKeyCode();
  }

  void event::skip()
  {
    ((wxEvent *)_obj.get())->Skip(true);
  }
}

