/////////////////////////////////////////////////////////////////////////////
// Name:        lev/string.hpp
// Purpose:     header for multi byte string classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     09/14/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include <luabind/luabind.hpp>

extern "C" {
  int luaopen_lev_string(lua_State *L);
}

namespace lev
{

  class mb_string : public base
  {
    protected:
      mb_string();
    public:
      virtual ~mb_string();
      static bool compare(luabind::object op1, luabind::object op2);
      bool compare_with(luabind::object rhs);
      static mb_string* concat(luabind::object op1, luabind::object op2);
      static mb_string *create(const std::string &str);
      static mb_string *create0() { return create(""); }
      bool empty() const;
      int find(const std::string &str);
      std::string get_string() const;
      virtual type_id get_type_id() const { return LEV_TMB_STRING; }
      virtual const char *get_type_name() const { return "lev.mb_string"; }
      mb_string* index(size_t pos) { return mid(pos, 1); }
      size_t len() const;
      mb_string* mid(size_t first, size_t count);
      mb_string* mid1(size_t first);
      bool set_string(const std::string &str);
      mb_string* sub_string(size_t from, size_t to);
      mb_string* sub_string1(size_t from) { return sub_string(from, -1); }
    protected:
      void *_obj;
  };

}

