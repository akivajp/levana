/////////////////////////////////////////////////////////////////////////////
// Name:        src/util.hpp
// Purpose:     source for utility functions
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     01/18/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/util.hpp"
#include "register.hpp"

#include <boost/format.hpp>
#include <luabind/luabind.hpp>

int luaopen_lev_util(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  open(L);
  globals(L)["require"]("table");
  globals(L)["require"]("lev");

  module(L, "lev")
  [
    namespace_("util")
  ];
  object util = globals(L)["lev"]["util"];
  register_to(L, util, "merge", &util::merge);
  register_to(L, util, "remove_first", &util::remove_first);
  register_to(L, util, "reverse", &util::reverse);
  register_to(L, util, "using", &util::using_l);

  globals(L)["package"]["loaded"] = util;
  return 0;
}

namespace lev
{

  luabind::object util::get_merged(lua_State *L, int begin, int end)
  {
    using namespace luabind;

    if (end < 0) { end = lua_gettop(L) + 1 + end; }
    if (end < begin) { return newtable(L); }
    lua_pushcfunction(L, &util::merge);
    newtable(L).push(L);
    for (int i = begin; i <= end; i++)
    {
      object(from_stack(L, i)).push(L);
    }
    lua_call(L, end + 2 - begin, 1);
    object t(from_stack(L, -1));
    lua_pop(L, 1);
    return t;
  }

  int util::merge(lua_State *L)
  {
    using namespace luabind;
    int n = lua_gettop(L);

    // 1st arg must be table
    luaL_checktype(L, 1, LUA_TTABLE);
    object target(from_stack(L, 1));

    for (int i = 2; i <= n; i++)
    {
      object arg(from_stack(L, i));
      switch (type(arg))
      {
        case LUA_TBOOLEAN:
          target["bool"] = arg;
          for (int j = 1; ; ++j)
          {
            std::string key = boost::io::str(boost::format("bool%1%") % j);
            if (!target[key.c_str()])
            {
              target[key.c_str()] = arg;
              break;
            }
          }
          break;
        case LUA_TFUNCTION:
          target["func"] = arg;
          for (int j = 1; ; ++j)
          {
            std::string key = boost::io::str(boost::format("func%1%") % j);
            if (!target[key.c_str()])
            {
              target[key.c_str()] = arg;
              break;
            }
          }
          break;
        case LUA_TNUMBER:
          target["num"] = arg;
          for (int j = 1; ; ++j)
          {
            std::string key = boost::io::str(boost::format("num%1%") % j);
            if (!target[key.c_str()])
            {
              target[key.c_str()] = arg;
              break;
            }
          }
          break;
        case LUA_TSTRING:
          target["str"] = arg;
          for (int j = 1; ; ++j)
          {
            std::string key = boost::io::str(boost::format("str%1%") % j);
            if (!target[key.c_str()])
            {
              target[key.c_str()] = arg;
              break;
            }
          }
          break;
        case LUA_TTABLE:
          for (iterator i(arg), end; i != end; ++i)
          {
            if (type(i.key()) == LUA_TNUMBER)
            {
              object tmp = *i;
              lua_pushcfunction(L, &util::merge);
              target.push(L);
              tmp.push(L);
              lua_call(L, 2, 0);
            }
            else if (type(i.key()) == LUA_TSTRING)
            {
              target[i.key()] = *i;
              for (int j = 1; ; ++j)
              {
                std::string key = (boost::format("%1%%2%") % i.key() % j).str();
                if (!target[key.c_str()])
                {
                  target[key.c_str()] = *i;
                  break;
                }
              }
            }
//            else { target[i.key()] = *i; }
          }
          break;
        case LUA_TUSERDATA:
          target["udata"] = arg;
          for (int j = 1; ; ++j)
          {
            std::string key = boost::io::str(boost::format("udata%1%") % j);
            if (!target[key.c_str()])
            {
              target[key.c_str()] = arg;
              break;
            }
          }
          break;
        case LUA_TNIL:
        default:
          break;
      }
    }

    target.push(L);
    return 1;
  }

  int util::remove_first(lua_State *L)
  {
    using namespace luabind;

    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checkany(L, 2);
    object table(from_stack(L, 1));
    object value(from_stack(L, 2));
    object fn_remove = globals(L)["table"]["remove"];

    for (iterator i(table), end; i != end; i++)
    {
      if (*i == value)
      {
        object res = (*i);
        res.push(L);
        fn_remove(table, i.key());
        return 1;
      }
    }
    return 0;
  }

  int util::reverse(lua_State *L)
  {
    using namespace luabind;

    luaL_checktype(L, 1, LUA_TTABLE);
    object table(from_stack(L, 1));
    object rev = newtable(L);
    object fn_insert = globals(L)["table"]["insert"];

    for (iterator i(table), end; i != end; i++)
    {
      fn_insert(rev, 1, *i);
    }
    rev.push(L);
    return 1;
  }

  static int lookup(lua_State *L)
  {
    using namespace luabind;

    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checkstring(L, 2);
    object env(from_stack(L, 1));
    object varname(from_stack(L, 2));
    object meta = getmetatable(env);
    if (meta["__owner"][varname] != nil)
    {
      meta["__owner"][varname].push(L);
      return 1;
    }
    for (iterator i(meta["__lookup"]), end; i != end; i++)
    {
      if ((*i)[varname] != nil)
      {
        (*i)[varname].push(L);
        return 1;
      }
    }
    // non sense to look up twice for the same table
    if (meta["__parent"] == meta["__owner"]) { return 0; }
    if (meta["__parent"][varname] != nil)
    {
      meta["__parent"][varname].push(L);
      return 1;
    }

    return 0;
  }

  int util::using_l(lua_State *L)
  {

/*

-- usage:
--   * using() : clearing the lookup setting
--   * using(...) : adding tables to the lookup setting where ... is a list of tables
--   * using(nil, ...) : clearing and resetting the lookup tables

-- looking up of "varnames"
local lookup = function(env, varname)
  meta = _G.getmetatable(env)
  if meta.__owner[varname] ~= nil then
    return meta.__owner[varname]
  end
  for i,t in _G.ipairs(meta.__lookup) do
    if t[varname] ~= nil then
      return t[varname]
    end
  end
  if (meta.__parent == meta.__owner) then
    -- non sense to look up twice for the same table
    return nil
  end
  return meta.__parent[varname]
end


-- subtituting the new value
local substitute = function(env, key, value)
  meta = _G.getmetatable(env)
  meta.__owner[key] = value
end


-- like "using directive"
function using(...)
  -- getting environment of the caller and its metatable
  local env  = _G.getfenv(2)
  local meta = _G.getmetatable(env) or {}
  -- getting the caller itself
  local f = _G.setfenv(2, env)
  if (meta.__caller == f) then
    -- setup was already done, changing looking up preference
    if (...) == nil then
      -- 1st arg is nil, resetting looking up setting
      if #{...} >= 2 then
        meta.__lookup = reverse({_G.select(2, ...)})
      else
        meta.__lookup = {}
      end
      return env
    end
    for i,val in _G.ipairs({...}) do
      find_and_remove(meta.__lookup, val)
      _G.table.insert(meta.__lookup, 1, val)
    end
    return env
  end

  -- setting new looking up mechanism
  local newenv  = {}
  local newmeta = {}
  newmeta.__caller = f
  newmeta.__index = lookup
  newmeta.__lookup = reverse({...})
  newmeta.__newindex = substitute
  newmeta.__owner = meta.__owner or env
  newmeta.__parent = env
  _G.setmetatable(newenv, newmeta)
  _G.setfenv(2, newenv)
  return newenv
end


-- enabling to call using without module prefix
_G.using = using

*/

    return 0;
  }

}

