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
      class_<file_path, base>("file_path")
        .def("clear", &file_path::clear)
        .property("dir_exists", &file_path::dir_exists)
        .property("dir", &file_path::get_dir_path)
        .property("dir_path", &file_path::get_dir_path)
        .property("file_exists", &file_path::file_exists)
        .property("file_name", &file_path::get_name)
        .property("full_path", &file_path::get_full_path)
        .property("full", &file_path::get_full_path)
        .property("ext", &file_path::get_ext)
        .property("is_dir", &file_path::is_dir)
        .property("is_dir_readable", &file_path::is_dir_readable)
        .property("is_dir_writable", &file_path::is_dir_writable)
        .property("is_file_executable", &file_path::is_file_executable)
        .property("is_file_readable", &file_path::is_file_readable)
        .property("is_file_writable", &file_path::is_file_writable)
        .def("mkdir", &file_path::mkdir)
        .def("mkdir", &file_path::mkdir0)
        .property("name", &file_path::get_name)
        .property("path", &file_path::get_dir_path)
        .property("size", &file_path::get_size)
        .property("url", &file_path::get_url)
        .scope
        [
          def("create_c", &file_path::create)
        ],
      class_<file_system, base>("file_system")
        .property("path", &file_system::get_path, &file_system::set_path)
        .scope
        [
          def("create_c", &file_system::create, adopt(result)),
          def("dir_exists", &file_system::dir_exists),
          def("file_exists", &file_system::file_exists),
          def("get_cwd", &file_system::get_cwd),
          def("get_executable_path", &file_system::get_executable_path),
          def("get_resource_dir", &file_system::get_resource_dir),
          def("get_temp_dir", &file_system::get_temp_dir),
          def("mkdir", &file_system::mkdir),
          def("mkdir", &file_system::mkdir1),
          def("to_file_path", &file_system::to_file_path),
          def("to_url", &file_system::to_url)
        ]
    ],
    namespace_("fs") // stub
  ];

  object lev = globals(L)["lev"];
  object classes = lev["classes"];
  object fs = lev["fs"];
  register_to(classes["file_path"], "create", &file_path::create_l);
  register_to(classes["file_system"], "create", &file_system::create_l);
  fs["create"] = classes["file_system"]["create"];
  fs["cwd"] = classes["file_system"]["get_cwd"];
  fs["exe_path"] = classes["file_system"]["get_executable_path"];
  fs["get_cwd"] = classes["file_system"]["get_cwd"];
  fs["get_executable_path"] = classes["file_system"]["get_executable_path"];
  fs["get_resurce_dir"] = classes["file_system"]["get_resource_dir"];
  fs["get_temp_dir"] = classes["file_system"]["get_temp_dir"];
  fs["file_path"] = classes["file_path"]["create"];
  fs["file_exists"] = classes["file_system"]["file_exists"];
  fs["mkdir"] = classes["file_system"]["mkdir"];
  fs["path"] = classes["file_path"]["create"];
  fs["res_dir"] = classes["file_system"]["get_resource_dir"];
  fs["resorce_dir"] = classes["file_system"]["get_resource_dir"];
  fs["temp_dir"] = classes["file_system"]["get_temp_dir"];
  fs["tmp_dir"] = classes["file_system"]["get_temp_dir"];
  fs["to_file_path"] = classes["file_system"]["to_file_path"];
  fs["to_path"] = classes["file_system"]["to_file_path"];
  fs["to_url"] = classes["file_system"]["to_url"];

  globals(L)["package"]["loaded"]["lev.fs"] = fs;
  return 0;
}

namespace lev
{

  static wxFileName* cast_path(void *obj) { return (wxFileName *)obj; }

  file_path::file_path() : _obj(NULL) { }

  file_path::~file_path()
  {
    if (_obj)
    {
      delete cast_path(_obj);
      _obj = NULL;
    }
  }

  bool file_path::clear()
  {
    cast_path(_obj)->Clear();
    return true;
  }

  file_path* file_path::create(const char *path)
  {
    file_path* fpath = NULL;
    wxFileName *obj = NULL;
    wxString url = wxFileSystem::FileNameToURL(wxString(path, wxConvUTF8));
    try {
      fpath = new file_path;
      fpath->_obj = obj = new wxFileName(wxFileSystem::URLToFileName(url));
      return fpath;
    }
    catch (...) {
      delete fpath;
      return NULL;
    }
  }

  int file_path::create_l(lua_State *L)
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

    object fpath = globals(L)["lev"]["classes"]["file_path"]["create_c"](path);
    fpath.push(L);
    return 1;
  }

  bool file_path::dir_exists()
  {
    return cast_path(_obj)->DirExists();
  }

  bool file_path::file_exists()
  {
    return cast_path(_obj)->FileExists();
  }

  const char *file_path::get_dir_path()
  {
    std::string path = (const char *)cast_path(_obj)->GetPath().mb_str();
    return path.c_str();
  }

  const char *file_path::get_full_path()
  {
    std::string full = (const char *)cast_path(_obj)->GetFullPath().mb_str();
    return full.c_str();
  }

  const char *file_path::get_ext()
  {
    std::string ext = (const char *)cast_path(_obj)->GetExt().mb_str();
    return ext.c_str();
  }

  const char *file_path::get_name()
  {
    std::string name = (const char *)cast_path(_obj)->GetName().mb_str();
    return name.c_str();
  }

  long file_path::get_size()
  {
    wxULongLong size = cast_path(_obj)->GetSize();
    if (size == wxInvalidSize) { return -1; }
    return size.GetValue();
  }

  std::string file_path::get_url()
  {
    return file_system::to_url(get_full_path());
  }

  bool file_path::is_dir()
  {
    return cast_path(_obj)->IsDir();
  }

  bool file_path::is_dir_readable()
  {
    return cast_path(_obj)->IsDirReadable();
  }

  bool file_path::is_dir_writable()
  {
    return cast_path(_obj)->IsDirWritable();
  }

  bool file_path::is_file_executable()
  {
    return cast_path(_obj)->IsFileExecutable();
  }

  bool file_path::is_file_readable()
  {
    return cast_path(_obj)->IsFileReadable();
  }

  bool file_path::is_file_writable()
  {
    return cast_path(_obj)->IsFileWritable();
  }

  bool file_path::mkdir(bool force)
  {
    int flags = 0;
    if (force) { flags = wxPATH_MKDIR_FULL; }
    return cast_path(_obj)->Mkdir(0755, flags);
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

  bool file_system::dir_exists(const char *dirpath)
  {
    return wxFileName::DirExists(wxString(dirpath, wxConvUTF8));
  }

  bool file_system::file_exists(const char *file_path)
  {
    return wxFileName::FileExists(wxString(file_path, wxConvUTF8));
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

  bool file_system::mkdir(const char *path, bool force)
  {
    int flags = 0;
    if (force) { flags = wxPATH_MKDIR_FULL; }
    return wxFileName::Mkdir(wxString(path, wxConvUTF8), 0755, flags);
  }

  bool file_system::set_path(const char *path)
  {
    cast_fs(_obj)->ChangePathTo(wxString(path, wxConvUTF8));
    return true;
  }

  const char *file_system::to_file_path(const char *url)
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

