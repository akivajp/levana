/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/sizer.hpp
// Purpose:     header for sizer object handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/17/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/sizer.hpp"
#include "levana/event.hpp"

namespace levana
{
  void sizer::addctrl(ctrl *c, int proportion, int flag, int border)
  {
    ((wxSizer *)_obj)->Add((wxWindow*)c->_obj, proportion, flag, border);
  }
  void sizer::addsizer(sizer *s, int proportion, int flag, int border)
  {
    ((wxSizer *)_obj)->Add((wxSizer*)s->_obj, proportion, flag, border);
  }
  void sizer::addspace(int width, int height, int proportion, int flag, int border)
  {
    ((wxSizer *)_obj)->Add(width, height, proportion, flag, border);
  }

  void sizer::fit(ctrl *c)
  {
    ((wxSizer *)_obj)->Fit((wxWindow*)c->_obj);
  }

  void sizer::fitinside(ctrl *c)
  {
    ((wxSizer *)_obj)->FitInside((wxWindow *)c->_obj);
  }

  void sizer::layout()
  {
    ((wxSizer *)_obj)->Layout();
  }

  hsizer::hsizer() : sizer()
  {
    try {
      _obj = new wxBoxSizer(wxHORIZONTAL);
    }
    catch (...)
    {
      throw "hsizer: allocation error";
    }
  }

  hsizer::~hsizer()
  {
    delete (wxBoxSizer *)_obj;
  }

  vsizer::vsizer() : sizer()
  {
    try {
      _obj = new wxBoxSizer(wxVERTICAL);
    }
    catch (...)
    {
      throw "vsizer: allocation error";
    }
  }

  vsizer::~vsizer()
  {
    delete (wxBoxSizer *)_obj;
  }
}

