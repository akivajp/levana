#ifndef _DB_HPP
#define _DB_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        lev/db.hpp
// Purpose:     header for database management classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     14/07/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include <luabind/luabind.hpp>

extern "C" {
  int luaopen_lev_db(lua_State *L);
}

namespace lev
{

  class db_table : public base
  {
    protected:
      db_table(const char *name) : table_name(name), owner(NULL) { }
    public:
      ~db_table() { }
    protected:
      std::string table_name;
      class db *owner;
  };

  class db : public base
  {
    public:
      db();
      ~db();
      bool exec(const char *sql, luabind::object func);
      virtual type_id get_type_id() const { return LEV_TDB; }
      virtual const char *get_type_name() const { return "lev.db"; }
      bool key(const char *keystr);
      static db* open(const char *filename);
      static db* open_memory();

    protected:
      void *_obj;
  };

}

#endif // _DB_HPP

