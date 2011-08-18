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
        .property("face", &font::get_face_name, &font::set_face_name)
        .property("face_name", &font::get_face_name, &font::set_face_name)
        .property("name", &font::get_face_name, &font::set_face_name)
        .scope
        [
          def("create", &font::create),
          def("load", &font::load),
          def("select", &font::select_font)
        ]
    ]
  ];
  object lev = globals(L)["lev"];
  object classes = lev["classes"];
  object font = lev["font"];

//  lev["font"] = classes["font"]["create"];
  font["create"] = classes["font"]["create"];
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

  font* font::create()
  {
    font *f = NULL;
    wxFont *obj = NULL;
    try {
      f = new font;
      f->_obj = obj = new wxFont();
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
    wxFont *obj = NULL;
    try {
      f = new font;
      f->_obj = obj = new wxFont(wxString(desc, wxConvUTF8));
      return f;
    }
    catch (...) {
      delete f;
      return NULL;
    }
  }

  int font::create_l(lua_State *L)
  {
    using namespace luabind;
    const char *desc = NULL;
    object f;

    object t = util::get_merged(L, 1, -1);

    if (t["description"]) { desc = object_cast<const char *>(t["description"]); }
    else if (t["desc"]) { desc = object_cast<const char *>(t["desc"]); }
    else if (t["str1"]) { desc = object_cast<const char *>(t["str1"]); }

    if (desc)
    {
      f = globals(L)["lev"]["classes"]["font"]["create_c"](desc);
    }
    else
    {
      f = globals(L)["lev"]["classes"]["font"]["create_c"]();
    }
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
    try {
      wxFontDialog dlg;
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

}

