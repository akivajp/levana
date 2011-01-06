/////////////////////////////////////////////////////////////////////////////
// Name:        src/connect.cpp
// Purpose:     source for event call back handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/28/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/event.hpp"
#include "levana/sizer.hpp"

namespace levana
{
  sizer *control::getsizer()
  {
    wxSizer *sz = ((wxWindow *)_obj)->GetSizer();
    if (sz == NULL) { return NULL; }
    sizer *s = new sizer();
    s->_obj = sz;
    return s;
  }

  bool control::isvalid()
  {
    if (wxWindow::FindWindowById(this->id)) { return true; }
    return false;
  }

  void control::setsizer(sizer *s)
  {
    ((wxWindow *)_obj)->SetSizer((wxSizer *)s->_obj);
  }


  int event::getkey() const
  {
    return ((wxKeyEvent *)_obj)->GetKeyCode();
  }

  void event::skip()
  {
    ((wxEvent *)_obj)->Skip(true);
  }
}

