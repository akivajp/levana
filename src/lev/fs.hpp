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

  class filepath : public base
  {
    protected:
      filepath();
    public:
      virtual ~filepath();
      static filepath* create(const char *path);
      static int create_l(lua_State *L);
      const char *get_fullpath();
      const char *get_ext();
      long get_size();
//      virtual type_id get_type_id() const { return LEV_TFILEPATH; }
      virtual const char *get_type_name() const { return "lev.fs.filepath"; }
      bool is_dir();
      bool is_file();

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
      static bool dir_exists(const char *dirname);
      static bool file_exists(const char *filename);
      static const char *get_cwd();
      static const char *get_executable_path();
      const char *get_path();
      static const char *get_resource_dir();
      static long get_size(const char *path);
      static const char *get_temp_dir();
//      virtual type_id get_type_id() const { return LEV_TFILE_SYSTEM; }
      virtual const char *get_type_name() const { return "lev.fs.file_system"; }
      bool set_path(const char *path);
      static const char *to_filepath(const char *url);
      static const char *to_url(const char *filename);

    protected:
      void *_obj;
  };

}

