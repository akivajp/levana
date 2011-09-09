/////////////////////////////////////////////////////////////////////////////
// Name:        src/db.cpp
// Purpose:     source for database management classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     14/07/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/db.hpp"
#include "register.hpp"
#include <sqlite3.h>
#include <utility>

int luaopen_lev_db(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  open(L);
  globals(L)["require"]("lev.base");

  module(L, "lev")
  [
    namespace_("classes")
    [
      class_<db, base>("db")
        .def("exec", &db::exec)
        .def("key", &db::key)
        .scope
        [
          def("open", &db::open),
          def("open", &db::open_memory)
        ]
    ],
    namespace_("db") // stub
  ];

  object lev = globals(L)["lev"];
  object classes = lev["classes"];
  object db = lev["db"];

  db["open"] = classes["db"]["open"];

  globals(L)["package"]["loaded"]["lev.db"] = db;
  return 0;
}

namespace lev
{

  static sqlite3* cast_db(void *obj) { return (sqlite3 *)obj; }

  db::db() : _obj(NULL) { }

  db::~db()
  {
    if (_obj) {
      sqlite3_close((sqlite3 *)_obj);
    }
    _obj = NULL;
  }

  static int callback(void *option, int count, char **values, char **names)
  {
    using namespace luabind;

    object *func = (object *)option;
    if ((*func) && luabind::type(*func) == LUA_TFUNCTION)
    {
      lua_State *L = func->interpreter();
      object result, row = newtable(L);
      for (int i = 0; i < count ; i++)
      {
        row[(const char *)names[i]] = (const char *)values[i];
      }
      result = (*func)(row);
      if (result == false) { return -1; }
      else if (!result || result == 0 || result == true) { return 0; }
      else { return -1; }
    }
    return 0;
  }
  bool db::exec(const char *sql, luabind::object func)
  {
    char *pStr;

    if (sqlite3_exec(cast_db(_obj), sql, callback, &func, NULL) != SQLITE_OK)
    {
      return false;
    }
    return true;
  }

  bool db::key(const char *keystr)
  {
//    if (sqlite3_key(cast_db(_obj), keystr, strlen(keystr)) == SQLITE_OK) { return true; }
    return false;
  }

  db* db::open(const char *filename)
  {
    db *data = NULL;
    sqlite3 *obj = NULL;
    try {
      data = new db;
      if (sqlite3_open(filename, &obj) != SQLITE_OK) { throw -1; }
      data->_obj = obj;
      return data;
    }
    catch (...) {
      delete data;
      return NULL;
    }
  }

  db* db::open_memory()
  {
    db *data = NULL;
    sqlite3 *obj = NULL;
    try {
      data = new db;
      if (sqlite3_open(":memory:", &obj) != SQLITE_OK) { throw -1; }
      data->_obj = obj;
      return data;
    }
    catch (...) {
      delete data;
      return NULL;
    }
  }

}

