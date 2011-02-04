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


namespace lev
{
  namespace util
  {
    int luaopen_util(lua_State *L)
    {
      using namespace luabind;

      globals(L)["util"] = newtable(L);
      register_to(L, globals(L)["util"], "merge", &util::merge);
      return 1;
    }

    int merge(lua_State *L)
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
              else { target[i.key()] = *i; }
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
  }
}

