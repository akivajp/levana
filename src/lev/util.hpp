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

extern "C" {
  int luaopen_lev_util(lua_State *L);
}

namespace lev
{
  class util
  {
    public:
      static bool execute(const std::string &target);
      static int execute_code_l(lua_State *L);
      static luabind::object get_merged(lua_State *L, int start, int end);
      static int merge(lua_State *L);
      static int remove_first(lua_State *L);
      static int reverse(lua_State *L);
      static int using_l(lua_State *L);
  };
}

#endif // _UTIL_HPP

