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

  sizer::sizer() : base(), _adopt(false), _obj(NULL)
  { }

  sizer::~sizer()
  {
    if (_adopt)
    {
      delete (wxSizer *)_obj;
    }
    else
    {
      // object is in wxWidgets control
    }
  }

  void sizer::addctrl(control *c, int proportion, int flag, int border)
  {
    ((wxSizer *)_obj)->Add((wxWindow*)c->_obj, proportion, flag, border);
  }
  void sizer::addsizer(sizer *s, int proportion, int flag, int border)
  {
    ((wxSizer *)_obj)->Add((wxSizer*)s->_obj, proportion, flag, border);
    s->_adopt = false;
  }
  void sizer::addspace(int width, int height, int proportion, int flag, int border)
  {
    ((wxSizer *)_obj)->Add(width, height, proportion, flag, border);
  }

  void sizer::fit(control *c)
  {
    ((wxSizer *)_obj)->Fit((wxWindow*)c->_obj);
  }

  void sizer::fitinside(control *c)
  {
    ((wxSizer *)_obj)->FitInside((wxWindow *)c->_obj);
  }

  void sizer::layout()
  {
    ((wxSizer *)_obj)->Layout();
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
      _obj = s;
      _adopt = true;
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
      _obj = s;
      _adopt = true;
    }
    catch (...)
    {
      throw "vsizer: allocation error";
    }
  }
}

