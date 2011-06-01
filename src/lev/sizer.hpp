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
      void add_ctrl(control *c, int proportion = 0, int flag = 0, int border = 0);
      void add_sizer(sizer *s, int proportion = 0, int flag = 0, int border = 0);
      void add_space(int width, int height, int proportion = 0, int flag = 0, int border = 0);
      static int add_l(lua_State *L);
      void fit(control *c);
      void fitinside(control *c);
      virtual type_id get_type_id() { return LEV_TSIZER; }
      void layout();
      // lua cfunctions
      static int ladd(lua_State *L);
      // friend classes
      friend class control;

    protected:
      bool _managing;
      void *_obj;
  };

  class hsizer : public sizer
  {
    private:
      hsizer();
    public:
      ~hsizer();
      static hsizer *create();
      static int create_l(lua_State *L);
      virtual type_id get_type_id() { return LEV_THSIZER; }
  };

  class vsizer : public sizer
  {
    private:
      vsizer();
    public:
      ~vsizer();
      static vsizer *create();
      static int create_l(lua_State *L);
      virtual type_id get_type_id() { return LEV_TVSIZER; }
  };
}

#endif // _SIZER_HPP

