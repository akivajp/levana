#ifndef _UTIL_HPP
#define _UTIL_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/util.hpp
// Purpose:     header for useful Lua functions
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     01/18/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include <lua.h>

namespace lev
{
  namespace util
  {
    int luaopen_util(lua_State *L);
    int merge(lua_State *L);
  }
}

#endif // _UTIL_HPP

