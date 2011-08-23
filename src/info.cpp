/////////////////////////////////////////////////////////////////////////////
// Name:        src/info.cpp
// Purpose:     source for platform infomation magaging class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     08/21/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"

#include "lev/info.hpp"

int luaopen_lev_info(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  open(L);
  globals(L)["require"]("lev");

  module(L, "lev")
  [
    namespace_("classes")
    [
      class_<info, base>("info")
        .property("arch", &info::get_architecture)
        .property("architecture", &info::get_architecture)
        .property("endian", &info::get_endianness)
        .property("endianness", &info::get_endianness)
        .property("major", &info::get_os_major_version)
        .property("major_version", &info::get_os_major_version)
        .property("minor", &info::get_os_minor_version)
        .property("minor_version", &info::get_os_minor_version)
        .property("os_major_version", &info::get_os_major_version)
        .property("os_minor_version", &info::get_os_minor_version)
        .scope
        [
          def("create", &info::create, adopt(result)),
          def("get_os_description", &info::get_os_description)
        ]
    ]
  ];
  object lev = globals(L)["lev"];
  object classes = lev["classes"];

  lev["info"]   = classes["info"]["create"];
  lev["get_os"] = classes["info"]["get_os_description"];

  globals(L)["package"]["loaded"]["lev.info"] = true;
  return 0;
}

namespace lev
{

  static wxPlatformInfo *cast_info(void *obj) { return (wxPlatformInfo *)obj; }

  info::info() : base(), _obj(NULL) { }

  info::~info()
  {
    if (_obj) { delete cast_info(_obj); }
    _obj = NULL;
  }

  info* info::create()
  {
    info* i = NULL;
    wxPlatformInfo *obj = NULL;
    try {
      i = new info;
      i->_obj = obj = new wxPlatformInfo();
      return i;
    }
    catch (...) {
      delete i;
      return NULL;
    }
  }

  int info::get_architecture()
  {
    switch(cast_info(_obj)->GetArchitecture())
    {
      case wxARCH_INVALID:
        return -1;
      case wxARCH_32:
        return 32;
      case wxARCH_64:
        return 64;
      default:
        return 0;
    }
  }

  int info::get_endianness()
  {
    switch(cast_info(_obj)->GetEndianness())
    {
      case wxENDIAN_INVALID:
        return -1;
      case wxENDIAN_BIG:
        return 4321;
      case wxENDIAN_LITTLE:
        return 1234;
      case wxENDIAN_PDP:
        return 3412;
      default:
        return 0;
    }
  }

  int info::get_os_major_version()
  {
    return cast_info(_obj)->GetOSMajorVersion();
  }

  int info::get_os_minor_version()
  {
    return cast_info(_obj)->GetOSMinorVersion();
  }

  std::string info::get_os_description()
  {
    return (const char *)wxGetOsDescription().mb_str();
  }

  std::string info::get_os_name()
  {
    switch (cast_info(_obj)->GetOperatingSystemId())
    {
      case wxOS_UNKNOWN:
        return "unknown";
    }
  }

}

