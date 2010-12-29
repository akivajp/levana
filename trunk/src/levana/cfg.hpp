#ifndef _CFG_HPP
#define _CFG_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/cfg.hpp
// Purpose:     header defining constant values
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/23/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include <lua.h>

namespace levana
{
  class cfg
  {
    public:
      static int frame_style(lua_State *L);
      enum config
      {
        DEFAULT = -1,
        FIXED   =  1,
      };
    private:
      inline cfg() {}
  };
}

#endif // _CFG_HPP

