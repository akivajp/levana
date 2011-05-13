/////////////////////////////////////////////////////////////////////////////
// Name:        src/connect.cpp
// Purpose:     source for event call back handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/28/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/event.hpp"
#include "lev/sizer.hpp"

namespace lev
{
  sizer *control::getsizer()
  {
    /*
    wxSizer *sz = ((wxWindow *)_obj.get())->GetSizer();
    if (sz == NULL) { return NULL; }
    sizer *s = new sizer();
    s->_obj = sz;
    return s;
    */
  }

  bool control::isshown()
  {
    return ((wxWindow *)_obj.get())->IsShown();
  }

  bool control::isvalid()
  {
    if (wxWindow::FindWindowById(_id)) { return true; }
    return false;
  }

  void control::setsizer(sizer *s)
  {
    ((wxWindow *)_obj.get())->SetSizer((wxSizer *)s->_obj.get());
  }

  bool control::setshown(bool showing)
  {
    return ((wxWindow *)_obj.get())->Show(showing);
  }


  // event methods
  event::event(void *e) : base()
  {
    _obj = boost::shared_ptr<void>((wxEvent *)e);
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

