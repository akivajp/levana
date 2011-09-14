/////////////////////////////////////////////////////////////////////////////
// Name:        src/package.cpp
// Purpose:     source for package management
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     09/01/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/archive.hpp"
#include "lev/fs.hpp"
#include "lev/package.hpp"
#include "lev/util.hpp"
#include "register.hpp"

int luaopen_lev_package(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  open(L);
  globals(L)["require"]("lev.archive");
  globals(L)["require"]("lev.fs");

  module(L, "lev")
  [
    namespace_("package")
    [
      def("resolve_c", &package::resolve, adopt(result))
    ]
  ];
  object lev = globals(L)["lev"];
  object package = lev["package"];

  register_to(package, "require", &package::require_l);
  register_to(package, "resolve", &package::resolve_l);

  lev["require"] = package["require"];

  globals(L)["package"]["loaded"]["lev.package"] = package;
  return 0;
}

namespace lev
{

  const char *package::get_archive_dir(lua_State *L)
  {
    open(L);
    module(L, "lev")
    [
      namespace_("package")
    ];
    if (! globals(L)["lev"]["package"]["archive_dir"]) { return NULL; }
    return object_cast<const char *>(globals(L)["lev"]["package"]["archive_dir"]);
  }

  const char *package::get_path(lua_State *L)
  {
    open(L);
    module(L, "lev")
    [
      namespace_("package")
    ];
    if (! globals(L)["lev"]["package"]["path"])
    {
      globals(L)["lev"]["package"]["path"] = file_system::get_cwd();
    }
    return object_cast<const char *>(globals(L)["lev"]["package"]["path"]);
  }

  int package::require_l(lua_State *L)
  {
    using namespace luabind;

    luaL_checkstring(L, 1);
    const char *module = object_cast<const char *>(object(from_stack(L, 1)));

    if (globals(L)["package"]["loaded"][module])
    {
      object t = globals(L)["package"]["loaded"][module];
      t.push(L);
      return 1;
    }

    const char *path = package::get_path(L);
    const char *dir = package::get_archive_dir(L);
    boost::shared_ptr<file_path> fpath;
    fpath.reset(package::resolve(path, module));
    if (fpath.get() == NULL) { fpath.reset(resolve(path, std::string(module) + ".lua")); }
    if (fpath.get() == NULL && dir) { fpath.reset(resolve(path, std::string(dir) + "/" + module)); }
    if (fpath.get() == NULL && dir) { fpath.reset(resolve(path, std::string(dir) + "/" + module + ".lua")); }
    if (fpath.get() != NULL)
    {
      if (luaL_dofile(L, fpath->get_full_path().c_str()))
      {
        wxMessageBox(wxString(lua_tostring(L, -1), wxConvUTF8), _("Lua runtime error"));
        lua_pushnil(L);
        return 1;
      }
      if (! globals(L)["package"]["loaded"][module])
      {
        globals(L)["package"]["loaded"][module] = true;
      }
      globals(L)["package"]["loaded"][module].push(L);
      return 1;
    }

    try {
      object result = globals(L)["require"](module);
      result.push(L);
      return 1;
    }
    catch (...) {
      luaL_error(L, (std::string("module '") + module + "' not found").c_str());
      return 0;
    }
  }

  file_path *package::resolve(const std::string &base_path, const std::string &file)
  {
    std::string full_path = file_system::to_full_path(base_path);
    if (file_system::dir_exists(full_path))
    {
      std::string path = std::string(full_path) + "/" + file;
      if (file_system::file_exists(path.c_str()))
      {
        return file_path::create(path);
      }
    }
    else if (lev::archive::is_archive(full_path))
    {
      if (archive::entry_exists_direct(full_path, file))
      {
        std::string app_name = application::get_app()->get_name();
        std::string ext = file_system::get_ext(file);
        if (! ext.empty()) { ext = "." + ext; }

        file_path *fpath = file_path::create_temp(app_name + "/", ext);
        if (fpath == NULL) { return NULL; }
        lev::archive::extract_direct_to(base_path, file, fpath->get_full_path());
        return fpath;
      }
    }
    return NULL;
  }

  int package::resolve_l(lua_State *L)
  {
    using namespace luabind;
    const char *path = NULL;
    const char *file = NULL;

    object t = util::get_merged(L, 1, -1);

    if (t["base_path"]) { path = object_cast<const char *>(t["base_path"]); }
    else if (t["path"]) { path = object_cast<const char *>(t["path"]); }
    else if (t["str2"]) // given 2 strings
    {
      // take the 1st string
      path = object_cast<const char *>(t["str1"]);
    }
    if (path == NULL) { path = package::get_path(L); }

    if (t["file"]) { file = object_cast<const char *>(t["file"]); }
    else if (t["f"]) { file = object_cast<const char *>(t["f"]); }
    else if (t["str2"]) { file = object_cast<const char *>(t["str2"]); }
    else if (t["str1"]) { file = object_cast<const char *>(t["str1"]); }
    if (file == NULL) { luaL_error(L, "file (string) was not given."); }

    object fpath = globals(L)["lev"]["package"]["resolve_c"](path, file);
    if (! fpath && package::get_archive_dir(L))
    {
      std::string new_file = std::string(package::get_archive_dir(L)) + "/" + file;
      fpath = globals(L)["lev"]["package"]["resolve_c"](path, new_file);
    }
    fpath.push(L);
    return 1;
  }

  bool package::set_archive_dir(lua_State *L, const std::string &archive_dir)
  {
    open(L);
    module(L, "lev")
    [
      namespace_("package")
    ];
    globals(L)["lev"]["package"]["archive_dir"] = archive_dir;
    return true;
  }

  bool package::set_path(lua_State *L, const std::string &path)
  {
    open(L);
    module(L, "lev")
    [
      namespace_("package")
    ];
    globals(L)["lev"]["package"]["path"] = path;
    return true;
  }

}

