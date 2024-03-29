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
#include "lev/app.hpp"
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

  register_to(package, "add_font", &package::add_font_l);
  register_to(package, "add_path", &package::add_path_l);
  register_to(package, "add_search", &package::add_search_l);
  register_to(package, "clear_search", &package::clear_search_l);
  register_to(package, "require", &package::require_l);
  register_to(package, "resolve", &package::resolve_l);
  register_to(package, "search_font", &package::search_font_l);

  lev["require"] = package["require"];

  globals(L)["package"]["loaded"]["lev.package"] = package;
  return 0;
}

namespace lev
{

  bool package::add_font(lua_State *L, const std::string &filename)
  {
    open(L);
    globals(L)["require"]("table");
    module(L, "lev")
    [
      namespace_("package")
    ];
    if (! globals(L)["lev"]["package"]["fonts"])
    {
      globals(L)["lev"]["package"]["fonts"] = newtable(L);
    }
    globals(L)["table"]["insert"](globals(L)["lev"]["package"]["fonts"], 1, filename);
    return true;
  }

  int package::add_font_l(lua_State *L)
  {
    using namespace luabind;
    const char *file = NULL;

    object t = util::get_merged(L, 1, -1);
    if (t["file_name"]) { file = object_cast<const char *>(t["file_name"]); }
    else if (t["file"]) { file = object_cast<const char *>(t["file"]); }
    else if (t["lua.string1"]) { file = object_cast<const char *>(t["lua.string1"]); }
    if (file == NULL) { luaL_error(L, "file (string) is not given"); }

    lua_pushboolean(L, package::add_font(L, file));
    return 1;
  }

  bool package::add_path(lua_State *L, const std::string &path)
  {
    open(L);
    globals(L)["require"]("table");
    module(L, "lev")
    [
      namespace_("package")
    ];
    if (! globals(L)["lev"]["package"]["path_list"])
    {
      globals(L)["lev"]["package"]["path_list"] = newtable(L);
    }
    globals(L)["table"]["insert"](globals(L)["lev"]["package"]["path_list"], 1, path);
    return true;
  }

  int package::add_path_l(lua_State *L)
  {
    using namespace luabind;
    const char *path = NULL;

    object t = util::get_merged(L, 1, -1);
    if (t["path"]) { path = object_cast<const char *>(t["path"]); }
    else if (t["p"]) { path = object_cast<const char *>(t["p"]); }
    else if (t["lua.string1"]) { path = object_cast<const char *>(t["lua.string1"]); }
    if (path == NULL) { luaL_error(L, "path (string) is not given"); }

    lua_pushboolean(L, package::add_path(L, path));
    return 1;
  }

  bool package::add_search(lua_State *L, const std::string &search)
  {
    open(L);
    globals(L)["require"]("table");
    module(L, "lev")
    [
      namespace_("package")
    ];
    if (! globals(L)["lev"]["package"]["search_list"])
    {
      globals(L)["lev"]["package"]["search_list"] = newtable(L);
    }
    globals(L)["table"]["insert"](globals(L)["lev"]["package"]["search_list"], 1, search);
    return true;
  }

  int package::add_search_l(lua_State *L)
  {
    using namespace luabind;
    const char *search = NULL;

    object t = util::get_merged(L, 1, -1);
    if (t["search_path"]) { search = object_cast<const char *>(t["search_path"]); }
    else if (t["search"]) { search = object_cast<const char *>(t["search"]); }
    else if (t["path"]) { search = object_cast<const char *>(t["path"]); }
    else if (t["lua.string1"]) { search = object_cast<const char *>(t["lua.string1"]); }
    if (search == NULL) { luaL_error(L, "path (string) is not given"); }

    lua_pushboolean(L, package::add_search(L, search));
    return 1;
  }

  int package::clear_search_l(lua_State *L)
  {
    luabind::module(L, "lev") [ namespace_("package") ];
    luabind::globals(L)["lev"]["package"]["search_path"] = luabind::nil;
    lua_pushboolean(L, true);
    return 1;
  }


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


  luabind::object package::get_font_list(lua_State *L)
  {
    open(L);
    module(L, "lev")
    [
      namespace_("package")
    ];
    if (! globals(L)["lev"]["package"]["fonts"])
    {
      object fonts = newtable(L);
      globals(L)["table"]["insert"](fonts, "msgothic.ttc");
      globals(L)["table"]["insert"](fonts, "msgothic.ttf");
      globals(L)["table"]["insert"](fonts, "msmincho.ttc");
      globals(L)["table"]["insert"](fonts, "msmincho.ttf");
      globals(L)["lev"]["package"]["fonts"] = fonts;
    }
    return globals(L)["lev"]["package"]["fonts"];
  }

  luabind::object package::get_path_list(lua_State *L)
  {
    open(L);
    module(L, "lev")
    [
      namespace_("package")
    ];
    if (! globals(L)["lev"]["package"]["path_list"])
    {
      object path_list = newtable(L);
      path_list[1] = "./";
      globals(L)["lev"]["package"]["path_list"] = path_list;
    }
    return globals(L)["lev"]["package"]["path_list"];
  }

  luabind::object package::get_search_list(lua_State *L)
  {
    open(L);
    module(L, "lev")
    [
      namespace_("package")
    ];
    if (! globals(L)["lev"]["package"]["search_list"])
    {
      object search_list = newtable(L);
      search_list[1] = "";
      globals(L)["lev"]["package"]["search_list"] = search_list;
    }
    return globals(L)["lev"]["package"]["search_list"];
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

    object path_list = package::get_path_list(L);
    boost::shared_ptr<file_path> fpath;
    fpath.reset(package::resolve(L, module));
    if (fpath.get() == NULL) { fpath.reset(resolve(L, std::string(module) + ".lua")); }
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

  file_path *package::resolve(lua_State *L, const std::string &file)
  {
    using namespace luabind;

    try {
      object path_list   = package::get_path_list(L);
      object search_list = package::get_search_list(L);

      for (iterator p(path_list), end; p != end; p++)
      {
        const char *path = object_cast<const char *>(*p);
        if (path == NULL) { throw -1; }
        std::string full = file_system::to_full_path(path);

        if (file_system::dir_exists(full))
        {
          for (iterator s(search_list); s != end; s++)
          {
            const char *search = object_cast<const char *>(*s);
            if (search == NULL) { throw -2; }

            std::string real_path = std::string(full) + "/" + search + "/" + file;
            if (file_system::file_exists(real_path.c_str()))
            {
              return file_path::create(real_path);
            }
          }
        }
        else if (lev::archive::is_archive(full))
        {
          for (iterator s(search_list); s != end; s++)
          {
            std::string entry = object_cast<const char *>(*s);
            if (entry.empty()) { entry = file; }
            else { entry = entry + "/" + file; }
            if (archive::entry_exists_direct(full, entry))
            {
              std::string app_name = application::get_app()->get_name();
              std::string ext = file_system::get_ext(file);
              if (! ext.empty()) { ext = "." + ext; }

              file_path *fpath = file_path::create_temp(app_name + "/", ext);
              if (fpath == NULL) { return NULL; }
              lev::archive::extract_direct_to(full, entry, fpath->get_full_path());
              return fpath;
            }
          }

          std::string arc_name = file_system::to_name(path);
          for (iterator s(search_list); s != end; s++)
          {
            std::string entry = object_cast<const char *>(*s);
            if (entry.empty()) { entry = arc_name + "/" + file; }
            else { entry = arc_name + "/" + entry + "/" + file; }
            if (archive::entry_exists_direct(full, entry))
            {
              std::string app_name = application::get_app()->get_name();
              std::string ext = file_system::get_ext(file);
              if (! ext.empty()) { ext = "." + ext; }

              file_path *fpath = file_path::create_temp(app_name + "/", ext);
              if (fpath == NULL) { return NULL; }
              lev::archive::extract_direct_to(full, entry, fpath->get_full_path());
              return fpath;
            }
          }
        }
      }
    }
    catch (...) {
      return NULL;
    }

    return NULL;
  }

  int package::resolve_l(lua_State *L)
  {
    using namespace luabind;
    const char *file = NULL;

    object t = util::get_merged(L, 1, -1);

    if (t["file"]) { file = object_cast<const char *>(t["file"]); }
    else if (t["f"]) { file = object_cast<const char *>(t["f"]); }
    else if (t["lua.string1"]) { file = object_cast<const char *>(t["lua.string1"]); }
    if (file == NULL) { luaL_error(L, "file (string) was not given."); }

    object fpath = globals(L)["lev"]["package"]["resolve_c"](file);
    fpath.push(L);
    return 1;
  }

  luabind::object package::search_font(lua_State *L, const std::string &filename)
  {
    globals(L)["require"]("lev.font");
    object dirs = globals(L)["lev"]["package"]["font_dirs"];

    if (file_system::file_exists(filename))
    {
      return globals(L)["lev"]["font"]["load"](filename);
    }
    for (iterator i(dirs), end; i != end; i++)
    {
      std::string path = object_cast<const char *>(*i);
      if (file_system::file_exists(path + "/" + filename))
      {
        return globals(L)["lev"]["font"]["load"](path + "/" + filename);
      }
    }
    return object();
  }

  int package::search_font_l(lua_State *L)
  {
    luaL_checkstring(L, 1);
    const char *file = object_cast<const char *>(object(from_stack(L, 1)));
    object found = search_font(L, file);
    if (found) { found.push(L); }
    else { lua_pushnil(L); }
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

  bool package::set_default_font_dirs(lua_State *L)
  {
    open(L);
    globals(L)["require"]("table");
    module(L, "lev")
    [
      namespace_("package")
    ];
    try {
      object list = newtable(L);
      globals(L)["lev"]["package"]["font_dirs"] = list;
#ifdef __WXGTK__
      boost::shared_ptr<file_system> fs(file_system::open("/usr/share/fonts/"));
      if (! fs) { throw -1; }
      std::string path;
      fs->find("*", path);
      while (! path.empty())
      {
        if (file_system::dir_exists(path))
        {
          globals(L)["table"]["insert"](list, 1, path);
        }
        fs->find_next(path);
      }
#endif
#ifdef __WXMSW__
      globals(L)["table"]["insert"](list, 1, file_system::to_full_path("C:/Windows/Fonts"));
#endif
      globals(L)["table"]["insert"](list, 1, file_system::to_full_path("./"));
      return true;
    }
    catch (...) {
      return false;
    }
  }

}

