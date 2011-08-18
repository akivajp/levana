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

static const char *using_test =
"-- looking up of varnames\n\
local lookup = function(env, varname)\n\
  meta = getmetatable(env)\n\
  if meta.__owner[varname] ~= nil then\n\
    return meta.__owner[varname]\n\
  end\n\
  for i,t in ipairs(meta.__lookup) do\n\
    if t[varname] ~= nil then\n\
      return t[varname]\n\
    end\n\
  end\n\
  if (meta.__parent == meta.__owner) then\n\
    -- non sense to look up twice for the same table\n\
    return nil\n\
  end\n\
  return meta.__parent[varname]\n\
end\n\
\n\
-- subtituting the new value\n\
local substitute = function(env, key, value)\n\
  meta = getmetatable(env)\n\
  meta.__owner[key] = value\n\
end\n\
\n\
-- like using directive\n\
return function(...)\n\
  -- getting environment of the caller and its metatable\n\
  local env  = getfenv(2)\n\
  local meta = getmetatable(env) or {}\n\
  -- getting the caller itself\n\
  local f = _G.setfenv(2, env)\n\
  if (meta.__caller == f) then\n\
    -- setup was already done, changing looking up preference\n\
    if (...) == nil then\n\
      -- 1st arg is nil, resetting looking up setting\n\
      if #{...} >= 2 then\n\
        meta.__lookup = lev.util.reverse({_G.select(2, ...)})\n\
      else\n\
        meta.__lookup = {}\n\
      end\n\
      return env\n\
    end\n\
    for i,val in _G.ipairs({...}) do\n\
      lev.util.remove_first(meta.__lookup, val)\n\
      table.insert(meta.__lookup, 1, val)\n\
    end\n\
    return env\n\
  end\n\
\n\
  -- setting new looking up mechanism\n\
  local newenv  = {}\n\
  local newmeta = {}\n\
  newmeta.__caller = f\n\
  newmeta.__index = lookup\n\
  newmeta.__lookup = lev.util.reverse({...})\n\
  newmeta.__newindex = substitute\n\
  newmeta.__owner = meta.__owner or env\n\
  newmeta.__parent = env\n\
  setmetatable(newenv, newmeta)\n\
  setfenv(2, newenv)\n\
  return newenv\n\
end\n\
";


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
  register_to(util, "merge", &util::merge);
  register_to(util, "remove_first", &util::remove_first);
  register_to(util, "reverse", &util::reverse);
//  register_to(util, "using", &util::using_l);
  load_to(util, "using", using_test);

  globals(L)["package"]["loaded"]["lev.util"] = util;
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

  // find "value" from "t" and remove first one
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

  // return new order-reversed table of "t"
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

  // looking up of "varnames"
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
    meta["__parent"][varname].push(L);
    return 1;
  }

  // subtituting the new value
  static int substitute(lua_State *L)
  {
    using namespace luabind;

    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checkstring(L, 2);
    luaL_checkany(L, 3);
    object env(from_stack(L, 1));
    object key(from_stack(L, 2));
    object value(from_stack(L, 3));

    object meta = getmetatable(env);
    meta["__owner"][key] = value;
    return 0;
  }

  // like "using directive"
  int util::using_l(lua_State *L)
  {
    using namespace luabind;
    int n = lua_gettop(L);

printf("N: %d\n", n);
printf("USING0\n");
    // getting environment of the caller and its metatable
    object env = globals(L)["getfenv"](2);
globals(L)["print"](env);
printf("USING0.1\n");
    object meta = getmetatable(env);
printf("USING0.2\n");
    if (!meta) { meta = newtable(L); }
printf("USING0.3\n");
    // getting the caller itself
printf("USING0.4\n");
    object f = globals(L)["setfenv"](2, env);

printf("USING1\n");
    if (meta["__caller"] != f)
    {
printf("USING1.1\n");
      // setting new looking up mechanism
      object newenv = newtable(L);
      object newmeta = newtable(L);
      newmeta["__caller"] = f;
      register_to(newmeta, "__index", &lookup);
      register_to(newmeta, "__newindex", &substitute);
      newmeta["__owner"] == meta["__owner"];
      if (!newmeta["__owner"]) { newmeta["__owner"] = env; }
      newmeta["__parent"] == env;
      setmetatable(newenv, newmeta);
      globals(L)["setfenv"](2, newenv);

      env = newenv;
      meta = newmeta;
    }

printf("USING2\n");
    // setup was already done, changing looking up preference
    if (n == 0 || !object(from_stack(L, 1)))
    {
printf("TYPE: %d\n", type(object(from_stack(L, 1))));
printf("USING2.1\n");
      // 1st arg is nil, resetting looking up setting
      meta["__lookup"] == newtable(L);
printf("USING2.2\n");
      if (n >= 2)
      {
printf("USING2.3\n");
        object lookup = meta["__lookup"];
printf("USING2.4\n");
        for (int i = n; i >= 2; i--)
        {
printf("USING2.5\n");
          lookup[n + 1 - i] = from_stack(L, n);
        }
      }
      env.push(L);
      return 1;
    }
printf("USING3\n");
    for (int i = 1; i <= n; i++)
    {
      object val(from_stack(L, i));
      globals(L)["lev"]["util"]["remove_first"](meta["__lookup"], val);
      globals(L)["table"]["insert"](meta["__lookup"], 1, val);
    }
    env.push(L);
    return 1;
  }

}

