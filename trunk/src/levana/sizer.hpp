#ifndef _SIZER_HPP
#define _SIZER_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/sizer.hpp
// Purpose:     header for sizer object handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/17/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include "event.hpp"

namespace levana
{
  class sizer : public base
  {
    protected:
      inline sizer() : base() {}
      inline ~sizer() {}
    public:
      void addctrl(ctrl *c, int proportion = 0, int flag = 0, int border = 0);
      void addsizer(sizer *s, int proportion = 0, int flag = 0, int border = 0);
      void addspace(int width, int height, int proportion = 0, int flag = 0, int border = 0);
      void fit(ctrl *c);
      void fitinside(ctrl *c);
      void layout();
  };

  class hsizer : public sizer
  {
    public:
      hsizer();
      ~hsizer();
  };

  class vsizer : public sizer
  {
    public:
      vsizer();
      ~vsizer();
  };
}

#endif // _SIZER_HPP

