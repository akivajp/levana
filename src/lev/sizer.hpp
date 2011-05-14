#ifndef _SIZER_HPP
#define _SIZER_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/sizer.hpp
// Purpose:     header for sizer object handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/17/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include <lua.h>

namespace lev
{
  class control;

  class sizer : public base
  {
    protected:
      sizer();
      virtual ~sizer();
    public:
      void addctrl(control *c, int proportion = 0, int flag = 0, int border = 0);
      void addsizer(sizer *s, int proportion = 0, int flag = 0, int border = 0);
      void addspace(int width, int height, int proportion = 0, int flag = 0, int border = 0);
      void fit(control *c);
      void fitinside(control *c);
      void layout();
      // lua cfunctions
      static int ladd(lua_State *L);
      // friend classes
      friend class control;

    protected:
      bool _adopt;
      void *_obj;
  };

  class hsizer : public sizer
  {
    public:
      hsizer();
  };

  class vsizer : public sizer
  {
    public:
      vsizer();
  };
}

#endif // _SIZER_HPP

