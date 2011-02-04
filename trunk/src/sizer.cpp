/////////////////////////////////////////////////////////////////////////////
// Name:        src/sizer.cpp
// Purpose:     source for sizer object handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/17/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/sizer.hpp"
#include "lev/event.hpp"

namespace lev
{
  void sizer::addctrl(control *c, int proportion, int flag, int border)
  {
    ((wxSizer *)_obj.get())->Add((wxWindow*)c->_obj.get(), proportion, flag, border);
  }
  void sizer::addsizer(sizer *s, int proportion, int flag, int border)
  {
    ((wxSizer *)_obj.get())->Add((wxSizer*)s->_obj.get(), proportion, flag, border);
  }
  void sizer::addspace(int width, int height, int proportion, int flag, int border)
  {
    ((wxSizer *)_obj.get())->Add(width, height, proportion, flag, border);
  }

  void sizer::fit(control *c)
  {
    ((wxSizer *)_obj.get())->Fit((wxWindow*)c->_obj.get());
  }

  void sizer::fitinside(control *c)
  {
    ((wxSizer *)_obj.get())->FitInside((wxWindow *)c->_obj.get());
  }

  void sizer::layout()
  {
    ((wxSizer *)_obj.get())->Layout();
  }

  // lua cfunctions
  int sizer::ladd(lua_State *L)
  {
    return 0;
  }


  hsizer::hsizer() : sizer()
  {
    try {
      wxBoxSizer *s = new wxBoxSizer(wxHORIZONTAL);
      _obj.reset(s);
    }
    catch (...)
    {
      throw "hsizer: allocation error";
    }
  }


  vsizer::vsizer() : sizer()
  {
    try {
      wxBoxSizer *s = new wxBoxSizer(wxVERTICAL);
      _obj.reset(s);
    }
    catch (...)
    {
      throw "vsizer: allocation error";
    }
  }
}

