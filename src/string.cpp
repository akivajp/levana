/////////////////////////////////////////////////////////////////////////////
// Name:        src/string.cpp
// Purpose:     header for multi byte string classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     09/14/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/string.hpp"


int luaopen_lev_string(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  open(L);
  globals(L)["require"]("lev.base");
  globals(L)["require"]("string");
  module(L, "lev")
  [
    namespace_("string"),
    namespace_("classes")
    [
      class_<mb_string, base>("mb_string")
        .def("cmp", &mb_string::compare_with)
        .def("compare", &mb_string::compare_with)
        .def("empty", &mb_string::empty)
        .def("index", &mb_string::index, adopt(result))
        .property("len", &mb_string::len)
        .property("length", &mb_string::len)
        .def("mid", &mb_string::mid, adopt(result))
        .def("mid", &mb_string::mid1, adopt(result))
        .property("str", &mb_string::get_string, &mb_string::set_string)
        .property("string", &mb_string::get_string, &mb_string::set_string)
        .def("sub", &mb_string::sub_string, adopt(result))
        .def("sub", &mb_string::sub_string1, adopt(result))
        .def("__eq", &mb_string::compare)
        .def("__concat", &mb_string::concat, adopt(result))
        .def("__index", &mb_string::index, adopt(result))
        .def("__len", &mb_string::len)
        .def("__tostring", &mb_string::get_string)
        .scope
        [
          def("concat", &mb_string::concat, adopt(result)),
          def("create", &mb_string::create, adopt(result)),
          def("create", &mb_string::create0, adopt(result))
        ]
    ]
  ];
  object lev = globals(L)["lev"];
  object classes = lev["classes"];
  object string = lev["string"];

  string["concat"] = classes["mb_string"]["concat"];
  string["create"] = classes["mb_string"]["create"];
  string["mb_str"] = classes["mb_string"]["create"];

  globals(L)["package"]["loaded"]["lev.string"] = string;
  return 0;
}


namespace lev
{

  static wxString* cast_str(void *obj) { return (wxString *)obj; }

  mb_string::mb_string() : base(), _obj(NULL) { }

  mb_string::~mb_string()
  {
    if (_obj)
    {
      delete cast_str(_obj);
      _obj = NULL;
    }
  }

  bool mb_string::compare(luabind::object op1, luabind::object op2)
  {
    using namespace luabind;

    lua_State *L = op1.interpreter();
    try {
      const char *str1 = object_cast<const char *>(globals(L)["tostring"](op1));
      const char *str2 = object_cast<const char *>(globals(L)["tostring"](op2));
      if (strcmp(str1, str2) == 0) { return true; }
      else { return false; }
    }
    catch (...) {
      return false;
    }
  }

  bool mb_string::compare_with(luabind::object rhs)
  {
    using namespace luabind;

    lua_State *L = rhs.interpreter();
    try {
      const char *r = object_cast<const char *>(globals(L)["tostring"](rhs));
      if (strcmp((const char *)cast_str(_obj)->mb_str(), r) == 0) { return true; }
      else { return false; }
    }
    catch (...) {
      return false;
    }
  }

  mb_string* mb_string::concat(luabind::object op1, luabind::object op2)
  {
    using namespace luabind;

    mb_string *mb = NULL;
    lua_State *L = op1.interpreter();
    try {
      mb = new mb_string;
      const char *str1 = object_cast<const char *>(globals(L)["tostring"](op1));
      const char *str2 = object_cast<const char *>(globals(L)["tostring"](op2));
      mb->_obj = new wxString(wxString(str1, wxConvUTF8) + wxString(str2, wxConvUTF8));
      return mb;
    }
    catch (...) {
      delete mb;
      return NULL;
    }
  }

  mb_string* mb_string::create(const std::string &str)
  {
    mb_string *mstr = NULL;
    try {
      mstr = new mb_string;
      mstr->_obj = new wxString(str.c_str(), wxConvUTF8);
      return mstr;
    }
    catch (...) {
      delete mstr;
      return NULL;
    }
  }

  bool mb_string::empty() const
  {
    return cast_str(_obj)->empty();
  }

  std::string mb_string::get_string() const
  {
    return (const char *)cast_str(_obj)->mb_str();
  }

  size_t mb_string::len() const
  {
    return cast_str(_obj)->Len();
  }

  mb_string* mb_string::mid(size_t first, size_t count)
  {
    mb_string *mb = NULL;
    try {
      mb = new mb_string;
      mb->_obj = new wxString(cast_str(_obj)->Mid(first, count));
      return mb;
    }
    catch (...) {
      delete mb;
      return NULL;
    }
  }

  mb_string* mb_string::mid1(size_t first)
  {
    return mid(first, wxString::npos);
  }

  bool mb_string::set_string(const std::string &str)
  {
    *cast_str(_obj) = wxString(str.c_str(), wxConvUTF8);
    return true;
  }

  mb_string* mb_string::sub_string(size_t from, size_t to)
  {
    mb_string *mb = NULL;
    try {
      mb = new mb_string;
      mb->_obj = new wxString(cast_str(_obj)->SubString(from, to));
      return mb;
    }
    catch (...) {
      delete mb;
      return NULL;
    }
  }

}

