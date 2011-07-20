/////////////////////////////////////////////////////////////////////////////
// Name:        src/fs.cpp
// Purpose:     source for filesystem management classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     14/07/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/fs.hpp"
#include "lev/util.hpp"
#include "register.hpp"
#include <string>

int luaopen_lev_fs(lua_State *L)
{
  using namespace luabind;
  using namespace lev;

  open(L);
  globals(L)["require"]("lev");

  module(L, "lev")
  [
    namespace_("classes")
    [
      class_<filepath, base>("filepath")
        .property("fullpath", &filepath::get_fullpath)
        .property("full", &filepath::get_fullpath)
        .property("ext", &filepath::get_ext)
        .property("is_dir", &filepath::is_dir)
        .property("size", &filepath::get_size)
        .scope
        [
          def("create_c", &filepath::create)
        ],
      class_<file_system, base>("file_system")
        .property("path", &file_system::get_path, &file_system::set_path)
        .scope
        [
          def("create_c", &file_system::create, adopt(result)),
          def("get_cwd", &file_system::get_cwd),
          def("get_executable_path", &file_system::get_executable_path),
          def("get_resource_dir", &file_system::get_resource_dir),
          def("get_temp_dir", &file_system::get_temp_dir),
          def("to_filepath", &file_system::to_filepath),
          def("to_url", &file_system::to_url)
        ]
    ],
    namespace_("fs") // stub
  ];

  object lev = globals(L)["lev"];
  object classes = lev["classes"];
  object fs = lev["fs"];
  register_to(L, classes["filepath"], "create", &filepath::create_l);
  register_to(L, classes["file_system"], "create", &file_system::create_l);
  fs["create"] = classes["file_system"]["create"];
  fs["cwd"] = classes["file_system"]["get_cwd"];
  fs["exe_path"] = classes["file_system"]["get_executable_path"];
  fs["get_cwd"] = classes["file_system"]["get_cwd"];
  fs["get_executable_path"] = classes["file_system"]["get_executable_path"];
  fs["get_resurce_dir"] = classes["file_system"]["get_resource_dir"];
  fs["get_temp_dir"] = classes["file_system"]["get_temp_dir"];
  fs["filepath"] = classes["filepath"]["create"];
  fs["path"] = classes["filepath"]["create"];
  fs["res_dir"] = classes["file_system"]["get_resource_dir"];
  fs["resorce_dir"] = classes["file_system"]["get_resource_dir"];
  fs["temp_dir"] = classes["file_system"]["get_temp_dir"];
  fs["tmp_dir"] = classes["file_system"]["get_temp_dir"];
  fs["to_filepath"] = classes["file_system"]["to_filepath"];
  fs["to_path"] = classes["file_system"]["to_filepath"];
  fs["to_url"] = classes["file_system"]["to_url"];

  globals(L)["package"]["loaded"]["lev.fs"] = fs;
  return 0;
}

namespace lev
{

  static wxFileName* cast_path(void *obj) { return (wxFileName *)obj; }

  filepath::filepath() : _obj(NULL) { }

  filepath::~filepath()
  {
    if (_obj)
    {
      delete cast_path(_obj);
      _obj = NULL;
    }
  }

  filepath* filepath::create(const char *path)
  {
    filepath* fpath = NULL;
    wxFileName *obj = NULL;
    wxString url = wxFileSystem::FileNameToURL(wxString(path, wxConvUTF8));
    try {
      fpath = new filepath;
      fpath->_obj = obj = new wxFileName(wxFileSystem::URLToFileName(url));
      return fpath;
    }
    catch (...) {
      delete fpath;
      return NULL;
    }
  }

  int filepath::create_l(lua_State *L)
  {
    using namespace luabind;
    const char *path = "";

    int n = lua_gettop(L);
    lua_pushcfunction(L, &util::merge);
    newtable(L).push(L);
    for (int i = 1; i <= n; i++)
    {
      object(from_stack(L, i)).push(L);
    }
    lua_call(L, n + 1, 1);
    object t(from_stack(L, -1));
    lua_remove(L, -1);

    if (t["filepath"]) { path = object_cast<const char *>(t["filepath"]); }
    else if (t["file_path"]) { path = object_cast<const char *>(t["file_path"]); }
    else if (t["file"]) { path = object_cast<const char *>(t["file"]); }
    else if (t["path"]) { path = object_cast<const char *>(t["path"]); }
    else if (t["f"]) { path = object_cast<const char *>(t["f"]); }
    else if (t["p"]) { path = object_cast<const char *>(t["p"]); }
    else if (t["str1"]) { path = object_cast<const char *>(t["str1"]); }

    object fpath = globals(L)["lev"]["classes"]["filepath"]["create_c"](path);
    fpath.push(L);
    return 1;
  }

  const char *filepath::get_fullpath()
  {
    std::string full = (const char *)cast_path(_obj)->GetFullPath().mb_str();
    return full.c_str();
  }

  const char *filepath::get_ext()
  {
    std::string ext = (const char *)cast_path(_obj)->GetExt().mb_str();
    return ext.c_str();
  }

  long filepath::get_size()
  {
    wxULongLong size = cast_path(_obj)->GetSize();
    if (size == wxInvalidSize) { return -1; }
    return size.GetValue();
  }

  bool filepath::is_dir()
  {
    return cast_path(_obj)->IsDir();
  }


  static wxFileSystem* cast_fs(void *obj) { return (wxFileSystem *)obj; }

  file_system::file_system() : _obj(NULL) { }

  file_system::~file_system()
  {
    if (_obj)
    {
      delete cast_fs(_obj);
      _obj = NULL;
    }
  }

  file_system* file_system::create()
  {
    file_system *fs = NULL;
    wxFileSystem *obj = NULL;
    try {
      fs = new file_system;
      fs->_obj = obj = new wxFileSystem;
      return fs;
    }
    catch (...) {
      delete fs;
      return NULL;
    }
  }

  int file_system::create_l(lua_State *L)
  {
    using namespace luabind;

    object fs = globals(L)["lev"]["classes"]["file_system"]["create_c"]();
    fs.push(L);
    return 1;
  }

  const char *file_system::get_cwd()
  {
    std::string path = (const char *)wxGetCwd().mb_str();
    return path.c_str();
  }

  const char *file_system::get_executable_path()
  {
    std::string path =
      (const char *)wxStandardPaths::Get().GetExecutablePath().mb_str();
    return path.c_str();
  }

  const char *file_system::get_resource_dir()
  {
    std::string path =
      (const char *)wxStandardPaths::Get().GetResourcesDir().mb_str();
    return path.c_str();
  }

  const char *file_system::get_temp_dir()
  {
    std::string path = (const char *)wxFileName::GetTempDir().mb_str();
    return path.c_str();
  }

  const char *file_system::get_path()
  {
    std::string path = (const char *)cast_fs(_obj)->GetPath().mb_str();
    return path.c_str();
  }

  bool file_system::set_path(const char *path)
  {
    cast_fs(_obj)->ChangePathTo(wxString(path, wxConvUTF8));
    return true;
  }

  const char *file_system::to_filepath(const char *url)
  {
    wxString u(url, wxConvUTF8);
    std::string filename =
      (const char *)wxFileSystem::URLToFileName(u).GetFullPath().mb_str();
    return filename.c_str();
  }

  const char *file_system::to_url(const char *filename)
  {
    wxString file(filename, wxConvUTF8);
    std::string url = (const char *)wxFileSystem::FileNameToURL(file).mb_str();
    return url.c_str();
  }

}

