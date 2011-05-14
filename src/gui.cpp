/////////////////////////////////////////////////////////////////////////////
// Name:        src/gui.cpp
// Purpose:     source for GUI controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/gui.hpp"
#include "connect.hpp"
#include <string>

namespace lev
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
      wxHtmlWindow *html = new myHtmlWindow(p, width, height);
      _id = html->GetId();
      _obj = html;
    }
    catch (...)
    {
      throw "htmlview: allocation error";
    }
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
    private:
      // Common Connect Interface
      DECLARE_CONNECT();
  };


  text::text(control *parent, int width, int height, const char *value)
  {
    try {
      wxWindow *p = NULL;
      wxString val = wxEmptyString;
      if (parent) { p = (wxWindow *)parent->_obj; }
      if (val) { val = wxString(value, wxConvUTF8); }
      myTextCtrl *text = new myTextCtrl(p, width, height, val);
      _id = text->GetId();
      _obj = text;
    }
    catch (...) {
      throw "text: allocation error";
    }
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

