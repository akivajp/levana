/////////////////////////////////////////////////////////////////////////////
// Name:        src/font.cpp
// Purpose:     source for font management classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     16/08/2010
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"

#include "lev/app.hpp"
#include "lev/font.hpp"
#include "lev/util.hpp"
#include "register.hpp"
#include <wx/wx.h>
#include <wx/fontdlg.h>

int luaopen_lev_font(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  open(L);
  globals(L)["require"]("lev");

  module_(L, "lev")
  [
    namespace_("font"),
    namespace_("classes")
    [
      class_<font, base>("font")
        .def("clone", &font::clone, adopt(result))
        .property("face", &font::get_face_name, &font::set_face_name)
        .property("face_name", &font::get_face_name, &font::set_face_name)
        .property("name", &font::get_face_name, &font::set_face_name)
        .property("point_size", &font::get_point_size, &font::set_point_size)
        .property("pt_size", &font::get_point_size, &font::set_point_size)
        .property("size", &font::get_point_size, &font::set_point_size)
        .property("sz", &font::get_point_size, &font::set_point_size)
        .scope
        [
          def("clone", &font::clone, adopt(result)),
          def("load_c", &font::load, adopt(result)),
          def("select", &font::select_font)
        ]
    ]
  ];
  object lev = globals(L)["lev"];
  object classes = lev["classes"];
  object font = lev["font"];

  register_to(classes["font"], "load", &font::load_l);

  font["clone"]  = classes["font"]["clone"];
  font["load"]   = classes["font"]["load"];
  font["select"] = classes["font"]["select"];

  globals(L)["package"]["loaded"]["lev.font"] = font;
  return 0;
}

namespace lev
{

  static wxFont* cast_font(void *obj) { return (wxFont *)obj; }

  font::font() : base(), _obj(NULL) { }

  font::~font()
  {
    if (_obj) { delete cast_font(_obj); }
    _obj = NULL;
  }


  font* font::clone()
  {
    font *f = NULL;
    try {
      f = new font;
      f->_obj = new wxFont(*cast_font(_obj));
      return f;
    }
    catch (...) {
      delete f;
      return NULL;
    }
  }


  font* font::load(const char *desc)
  {
    font *f = NULL;
    try {
      f = new font;
      if (load)
      {
        f->_obj = new wxFont(wxString(desc, wxConvUTF8));
      }
      else
      {
        f->_obj = new wxFont(*wxNORMAL_FONT);
      }
      return f;
    }
    catch (...) {
      delete f;
      return NULL;
    }
  }


  int font::load_l(lua_State *L)
  {
    using namespace luabind;
    const char *desc = NULL;
    object f;

    object t = util::get_merged(L, 1, -1);

    if (t["description"]) { desc = object_cast<const char *>(t["description"]); }
    else if (t["desc"]) { desc = object_cast<const char *>(t["desc"]); }
    else if (t["lua.string1"]) { desc = object_cast<const char *>(t["lua.string1"]); }

    f = globals(L)["lev"]["classes"]["font"]["load_c"](desc);
    f.push(L);
    return 1;
  }

  const char *font::get_face_name()
  {
    std::string face = (const char *)cast_font(_obj)->GetFaceName().mb_str();
    return face.c_str();
  }

  int font::get_point_size()
  {
    return cast_font(_obj)->GetPointSize();
  }

  font* font::select_font()
  {
    font *f = NULL;
    wxFont *obj = NULL;
    wxFontData data;
    try {
      wxFontDialog dlg(NULL, data);
      if (dlg.ShowModal() != wxID_OK) { throw -1; }
      f = new font;
      f->_obj = obj = new wxFont(dlg.GetFontData().GetChosenFont());
      return f;
    }
    catch (...) {
      delete f;
      return NULL;
    }
  }

  bool font::set_face_name(const char *face)
  {
    return cast_font(_obj)->SetFaceName(wxString(face, wxConvUTF8));
  }

  bool font::set_point_size(int size)
  {
    cast_font(_obj)->SetPointSize(size);
    return true;
  }

}


