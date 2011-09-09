#ifndef _FS_HPP
#define _FS_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        lev/fs.hpp
// Purpose:     header for filesystem management classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     14/07/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#endif // _FS_HPP

#include "base.hpp"
#include <luabind/luabind.hpp>

extern "C" {
  int luaopen_lev_fs(lua_State *L);
}

namespace lev
{

  class temp_file : public base
  {
    protected:
      temp_file();
    public:
      virtual ~temp_file();
      static temp_file* create(const char *prefix, const char *suffix);
      static int create_l(lua_State *L);
//      virtual type_id get_type_id() const { return LEV_TTEMP_FILE; }
      virtual const char *get_type_name() const { return "lev.fs.temp_file"; }
    protected:
      std::string path;
  };


  class file_path : public base
  {
    protected:
      file_path();
    public:
      virtual ~file_path();
      bool clear();
      static file_path* create(const char *path);
      static int create_l(lua_State *L);
      bool dir_exists();
      bool file_exists();
      const char *get_dir_path();
      const char *get_full_path();
      const char *get_ext();
      const char *get_name();
      long get_size();
      std::string get_url();
      virtual type_id get_type_id() const { return LEV_TFILE_PATH; }
      virtual const char *get_type_name() const { return "lev.fs.file_path"; }
      bool is_dir();
      bool is_dir_readable();
      bool is_dir_writable();
      bool is_file_executable();
      bool is_file_readable();
      bool is_file_writable();
      bool mkdir(bool force);
      bool mkdir0() { return mkdir(false); }

    protected:
      void *_obj;
  };

  class file_system : public base
  {
    protected:
      file_system();
    public:
      virtual ~file_system();
      static file_system* create();
      static int create_l(lua_State *L);
      static bool dir_exists(const char *dirpath);
      static bool file_exists(const char *file_path);
      static const char *get_cwd();
      static const char *get_executable_path();
      const char *get_path();
      static const char *get_resource_dir();
      static long get_size(const char *path);
      static const char *get_temp_dir();
      virtual type_id get_type_id() const { return LEV_TFILE_SYSTEM; }
      virtual const char *get_type_name() const { return "lev.fs.file_system"; }
      static bool mkdir(const char *path, bool force);
      static bool mkdir1(const char *path) { return file_system::mkdir(path, false); }
      bool set_path(const char *path);
      static const char *to_file_path(const char *url);
      static const char *to_url(const char *filename);

    protected:
      void *_obj;
  };

}

