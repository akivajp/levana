#ifndef _ARCHIVE_HPP
#define _ARCHIVE_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        lev/archive.hpp
// Purpose:     header for archive file handling classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     09/06/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include <luabind/luabind.hpp>


extern "C"
{
  int luaopen_lev_archive(lua_State *L);
}


namespace lev
{

  class archive : public base
  {
    protected:
      archive();
    public:
      ~archive();
      bool add_data(const char *entry_name, const std::string &data);
      const char* find(const char *pattern);
      const char* find0() { return find("*"); }
      const char* find_next();
      bool entry_exists(const char *entry_name);
      bool extract(const char *entry_name);
      bool extract_to(const char *entry_name, const char *target_path);
      virtual type_id get_type_id() const { return LEV_TARCHIVE; }
      virtual const char *get_type_name() const { return "lev.archive"; }
      static archive* open(const char *filename);
      bool read(const char *entry_name, std::string &data, int block_size);
      bool read2(const char *entry_name, std::string &data) { return read(entry_name, data, 4096); }
      static int read_l(lua_State *L);
    protected:
      void *_obj;
  };

}

#endif // _ARCHIVE_HPP

