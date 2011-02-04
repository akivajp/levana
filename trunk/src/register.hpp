#ifndef _REGISTER_HPP
#define _REGISTER_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/register.hpp
// Purpose:     inner use header for lua cfunction register function
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     01/22/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include <luabind/luabind.hpp>

namespace lev
{

  inline void register_to(lua_State *L, luabind::object to, const char *funcname, lua_CFunction func)
  {
    lua_pushcfunction(L, func);
    luabind::object f(luabind::from_stack(L, -1));
    lua_pop(L, 1);
    to[funcname] = f;
  }

}

#endif // _REGISTER_HPP

