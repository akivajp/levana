#ifndef _CFG_HPP
#define _CFG_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/cfg.hpp
// Purpose:     header defining constant values
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/23/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include <lua.h>

namespace lev
{
  namespace cfg
  {
    int frame_style(lua_State *L);
    int luaopen_cfg(lua_State *L);
    enum config
    {
      DEFAULT = -1,
      FIXED   =  1,
    };
  };
}

#endif // _CFG_HPP

