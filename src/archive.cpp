/////////////////////////////////////////////////////////////////////////////
// Name:        src/archive.cpp
// Purpose:     source for archive file handling classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     09/06/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/archive.hpp"
#include "lev/fs.hpp"
#include "lev/util.hpp"
#include "register.hpp"

extern "C" {
  #include <archive.h>
  #include <archive_entry.h>
}

int luaopen_lev_archive(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  open(L);
  globals(L)["require"]("lev.base");

  module(L, "lev")
  [
    namespace_("archive"),
    namespace_("classes")
    [
      class_<lev::archive, base>("archive")
        .def("add_data", &lev::archive::add_data)
        .def("find", &lev::archive::find)
        .def("find", &lev::archive::find0)
        .def("find_next", &lev::archive::find_next)
        .def("entry_exists", &lev::archive::entry_exists)
        .def("extract", &lev::archive::extract)
        .def("extract_to", &lev::archive::extract_to)
        .scope
        [
          def("open", &lev::archive::open, adopt(result))
        ]
    ]
  ];
  object lev = globals(L)["lev"];
  object classes = lev["classes"];
  object arch = lev["archive"];

  register_to(classes["archive"], "read", &lev::archive::read_l);
  arch["open"] = classes["archive"]["open"];

  globals(L)["package"]["loaded"]["lev.archive"] = true;
}

namespace lev
{

  class myArchive
  {
    public:

      myArchive() : r(NULL), w(NULL), filename(), compress(0), format(0)
      { }

      ~myArchive()
      {
        Clean();
      }

      bool Clean()
      {
        if (r)
        {
          archive_read_finish(r);
          r = NULL;
        }
        if (w)
        {
          archive_write_close(w);
          archive_write_finish(w);
          w = NULL;
        }
      }

      static myArchive* Create()
      {
        myArchive *arc = NULL;
        try {
          arc = new myArchive;
          return arc;
        }
        catch (...) {
          delete arc;
          return NULL;
        }
      }

      archive_entry* Find(const char *pattern)
      {
        archive_entry *entry;

        while (archive_read_next_header(r, &entry) == ARCHIVE_OK)
        {
          wxString path(archive_entry_pathname(entry), wxConvUTF8);
          if (path.Matches(wxString(pattern, wxConvUTF8)))
          {
            return entry;
          }
          else
          {
            continue;
          }
        }
        return NULL;
      }

      bool Extract(const char *entry_name)
      {
        if (!StartReading()) { return false; }

        archive_entry *entry = Find(entry_name);
        if (entry == NULL) { return false; }

        if (archive_read_extract(r, entry, 0) == ARCHIVE_OK) { return true; }
        else { return false; }
      }

      bool ExtractTo(const char *entry_name, const char *target)
      {
        int fd = 0;
        file_path *path = NULL;

        if (!StartReading()) { return false; }
        if (Find(entry_name) == NULL) { return false; }
        if (target == NULL) { target = entry_name; }
        try {
          path = file_path::create(target);
          if (path == NULL) { throw -1; }

          std::string dir = path->get_dir_path();
          file_system::mkdir(dir.c_str(), true);

          int fd = ::open(target, O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);
          if (fd == -1) { throw -2; }

          int size = archive_read_data_into_fd(r, fd);
          ::close(fd);
          delete path;

          if (size >= 0) { return true; }
          else { return false; }
        }
        catch (...) {
          if (fd > 0) { ::close(fd); }
          delete path;
          return false;
        }
      }

      bool StartReading()
      {
        Clean();
        r = archive_read_new();
        archive_read_support_compression_all(r);
        archive_read_support_format_all(r);

        if (archive_read_open_filename(r, filename.c_str(), 10240) != ARCHIVE_OK)
        {
          return false;
        }
        return true;
      }

      bool StartWriting()
      {
        Clean();
        w = archive_write_new();
        archive_write_set_format_pax_restricted(w);
//        archive_write_set_format_zip(w);
//        archive_write_set_format_by_name(w, filename.c_str());
        archive_write_set_compression_gzip(w);

        if (archive_write_open_filename(w, filename.c_str()) != ARCHIVE_OK)
        {
          return false;
        }
        return true;
      }

      ::archive *r;
      ::archive *w;
      std::string filename;
      int compress, format;
      std::string last_find;
  };

  static myArchive* cast_arc(void *obj) { return (myArchive *)obj; }

  archive::archive() : _obj(NULL) { }

  archive::~archive()
  {
    if (_obj) { delete cast_arc(_obj); }
  }

  bool archive::add_data(const char *entry_name, const std::string &data)
  {
    myArchive *arc = cast_arc(_obj);
    if (entry_name == NULL) { return false; }
    if (arc->w == NULL)
    {
      if (!arc->StartWriting()) { return false; }
    }

    archive_entry *entry = archive_entry_new();
    if (entry == NULL) { return false; }

    archive_entry_set_filetype(entry, AE_IFREG);
    archive_entry_set_pathname(entry, entry_name);
    archive_entry_set_perm(entry, 0644);
    archive_entry_set_size(entry, data.length());
    archive_write_header(arc->w, entry);
    archive_write_data(arc->w, data.c_str(), data.length());
    archive_entry_free(entry);
//    arc->Clean();
    return true;
  }


  const char *archive::find(const char *pattern)
  {
    myArchive *arc = cast_arc(_obj);
    arc->last_find = pattern;
    if (!arc->StartReading()) { return NULL; }

    return this->find_next();
  }

  const char *archive::find_next()
  {
    myArchive *arc = cast_arc(_obj);
    if (arc->last_find.empty()) { return NULL; }
    archive_entry *entry = arc->Find(arc->last_find.c_str());
    if (entry == NULL)
    {
      arc->last_find = "";
      return NULL;
    }
    return archive_entry_pathname(entry);
  }



  bool archive::entry_exists(const char *entry_name)
  {
    cast_arc(_obj)->StartReading();
    if (cast_arc(_obj)->Find(entry_name)) { return true; }
    return false;
  }

  bool archive::extract(const char *entry_name)
  {
    return cast_arc(_obj)->Extract(entry_name);
  }

  bool archive::extract_to(const char *entry_name, const char *target)
  {
    return cast_arc(_obj)->ExtractTo(entry_name, target);
  }

  archive* archive::open(const char *filename)
  {
    archive *arc = NULL;
    myArchive *obj = NULL;
    try {
      arc = new archive;
      arc->_obj = obj = myArchive::Create();
      if (obj == NULL) { throw -1; }
      obj->filename = filename;
      return arc;
    }
    catch (...) {
      delete arc;
      return NULL;
    }
  }

  bool archive::read(const char *entry_name, std::string &data, int block_size)
  {
    myArchive *arc = cast_arc(_obj);
    archive_entry *entry;

    if (!arc->StartReading()) { return NULL; }
    while (arc->Find(entry_name))
    {
      data = "";
      char buffer[block_size];
      for ( ; ; )
      {
        int size = archive_read_data(arc->r, buffer, block_size);
        if (size < 0) { break; }
        else if (size == 0) { return true; }
        else // if (size > 0)
        { data.append(buffer, size); }
      }
    }
    return false;
  }

  int archive::read_l(lua_State *L)
  {
    using namespace luabind;
    const char *entry = NULL;
    int bs = 4096;

    luaL_checktype(L, 1, LUA_TUSERDATA);
    lev::archive *arc = object_cast<lev::archive *>(object(from_stack(L, 1)));
    if (arc == NULL) { return 0; }
    object t = util::get_merged(L, 2, -1);

    if (t["entry_name"]) { entry = object_cast<const char *>(t["entry_name"]); }
    else if (t["entry"]) { entry = object_cast<const char *>(t["entry"]); }
    else if (t["e"]) { entry = object_cast<const char *>(t["e"]); }
    else if (t["str1"]) { entry = object_cast<const char *>(t["str1"]); }
    if (entry == NULL) { luaL_error(L, "entry (string) is not specified."); }

    if (t["block_size"]) { bs = object_cast<int>(t["block_size"]); }
    else if (t["block"]) { bs = object_cast<int>(t["block"]); }
    else if (t["bs"]) { bs = object_cast<int>(t["bs"]); }
    else if (t["num1"]) { bs = object_cast<int>(t["num1"]); }

    std::string data;
    if (arc->read(entry, data, bs))
    {
      lua_pushstring(L, data.c_str());
      return 1;
    }
    else
    {
      lua_pushnil(L);
      return 1;
    }
  }

}

