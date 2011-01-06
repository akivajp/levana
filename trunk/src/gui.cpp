/////////////////////////////////////////////////////////////////////////////
// Name:        src/gui.cpp
// Purpose:     source for GUI controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/gui.hpp"
#include "connect.hpp"
#include <string>

namespace levana
{
  // html viewer control definitions
  class myHtmlWindow : public wxHtmlWindow
  {
    public:
      inline myHtmlWindow(wxWindow *parent, int width, int height)
        : wxHtmlWindow(parent, -1, wxDefaultPosition, wxSize(width, height))
      {}

      inline ~myHtmlWindow() {}
  };

  htmlview::htmlview(control *parent, int width, int height)
    : control()
  {
    try {
      wxWindow *p = NULL;
      if (parent) { p = (wxWindow *)parent->_obj; }
      _obj = new myHtmlWindow(p, width, height);
    }
    catch (...)
    {
      throw "htmlview: allocation error";
    }
  }

  htmlview::~htmlview()
  {
    delete (myHtmlWindow *)_obj;
  }

  bool htmlview::loadpage(const char *url)
  {
    return ((myHtmlWindow *)_obj)->LoadPage(wxString(url, wxConvUTF8));
  }

  bool htmlview::setpage(const char *src)
  {
    return ((myHtmlWindow *)_obj)->SetPage(wxString(src, wxConvUTF8));
  }

  const char* htmlview::totext()
  {
    const std::string text =
      (const char *)((myHtmlWindow *)_obj)->ToText().mb_str(wxConvUTF8);
    return text.c_str();
  }


  // text control definitions
  class myTextCtrl : public wxTextCtrl
  {
    public:
      inline myTextCtrl(wxWindow *parent, int width, int height, wxString &value) :
        wxTextCtrl(parent, -1, value, wxDefaultPosition, wxSize(width, height)) {}
      // Common Connect Interface
      inline void Connect(int id, wxEventType eventType, luabind::object lua_func)
      {
        levana::Connect(this, id, eventType, lua_func);
      }
      void ProcEvent(wxEvent &event)
      {
        levana::ProcEvent(this, event);
      }
      func_map fmap;
  };

  text::text(control *parent, int width, int height, const char *value)
  {
    try {
      wxWindow *p = NULL;
      wxString val = wxEmptyString;
      if (parent) { p = (wxWindow *)parent->_obj; }
      if (val) { val = wxString(value, wxConvUTF8); }
      _obj = new myTextCtrl(p, width, height, val);
    }
    catch (...) {
      throw "text: allocation error";
    }
  }

  text::~text()
  {
    if (_obj) { delete (wxTextCtrl *)_obj; }
  }

  const char* text::getvalue()
  {
    const std::string value =
      (const char *)((wxTextCtrl *)_obj)->GetValue().mb_str(wxConvUTF8);
    return value.c_str();
  }

  void text::setvalue(const char *value)
  {
    ((wxTextCtrl *)_obj)->SetValue(wxString(value, wxConvUTF8));
  }
}

