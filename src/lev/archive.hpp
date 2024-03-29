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
      bool add_data(const std::string &entry_name, const std::string &data);
      bool add_file(const std::string &file) { return add_file_to(file, file); }
      bool add_file_to(const std::string &file, const std::string &entry_name);
      bool find(const std::string &pattern, std::string &entry_name);
      static int find_l(lua_State *L);

      static bool find_direct(const std::string &archive_file,
                              const std::string &pattern,
                              std::string &entry_name);
      static int find_direct_l(lua_State *L);

      bool find_next(std::string &entry_name);
      static int find_next_l(lua_State *L);
      bool entry_exists(const std::string &entry_name);
      static bool entry_exists_direct(const std::string &archive_file,
                                      const std::string &entry_name);
      bool extract(const std::string &entry_name);
      static bool extract_direct(const std::string &archive_file,
                                 const std::string &entry_name);
      static bool extract_direct_to(const std::string &archive_file,
                                    const std::string &entry_name,
                                    const std::string &target_path);
      bool extract_to(const std::string &entry_name, const std::string &target_path);
      std::string get_compression_name();
      std::string get_format_name();
      virtual type_id get_type_id() const { return LEV_TARCHIVE; }
      virtual const char *get_type_name() const { return "lev.archive"; }
      static bool is_archive(const std::string &filename);
      static archive* open(const std::string &filename);
      bool read(const std::string &entry_name, std::string &data, int block_size);
      bool read2(const std::string &entry_name, std::string &data) { return read(entry_name, data, 4096); }
      static int read_l(lua_State *L);

      static bool read_direct(const std::string &archive_file,
                              const std::string &entry_name,
                              std::string &data,
                              int block_size = 4096);
      static int read_direct_l(lua_State *L);

      bool set_compression_gzip();
      bool set_compression_none();
      bool set_compression_xz();
      bool set_format_tar();
      bool set_format_zip();
    protected:
      void *_obj;
  };

}

#endif // _ARCHIVE_HPP

