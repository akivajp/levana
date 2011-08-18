/////////////////////////////////////////////////////////////////////////////
// Name:        src/locale.cpp
// Purpose:     source for localization managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     20/07/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/locale.hpp"

int luaopen_lev_locale(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  open(L);
  globals(L)["require"]("lev");

  module(L)
  [
    def("_", &locale::translate)
  ];

  module(L, "lev")
  [
    namespace_("locale")
    [
      def("translate", &locale::translate)
    ],
    namespace_("classes")
    [
      class_<locale, base>("locale")
        .def("add", &locale::add_catalog)
        .def("add_catalog", &locale::add_catalog)
        .property("encode", &locale::get_encoding)
        .property("encoding", &locale::get_encoding)
        .property("lang", &locale::get_language)
        .property("language", &locale::get_language)
        .property("name", &locale::get_name)
    ]
  ];

  globals(L)["package"]["loaded"]["lev.locale"] = globals(L)["lev"]["locale"];
  return 0;
}

namespace lev
{

  static wxLocale* cast_loc(void *obj) { return (wxLocale *)obj; }

  bool locale::add_catalog(const char *catalog)
  {
    return cast_loc(_obj)->AddCatalog(wxString(catalog, wxConvUTF8));
  }

  const char *locale::get_encoding()
  {
    std::string enc =
      (const char *)cast_loc(_obj)->GetSystemEncodingName().mb_str();
    return enc.c_str();
  }

  const char *locale::get_language()
  {
    wxString lang = cast_loc(_obj)->GetLocale();
    std::string l = (const char *)lang.mb_str();
    return l.c_str();
  }

  const char *locale::get_name()
  {
    std::string name = (const char *)cast_loc(_obj)->GetName().mb_str();
    return name.c_str();
  }

  const char *locale::translate(const char *src)
  {
    wxString s(src, wxConvUTF8);
    wxString t = wxGetTranslation(s);
    std::string trans = (const char *)t.mb_str();
    return trans.c_str();
  }

}

