/////////////////////////////////////////////////////////////////////////////
// Name:        src/frame.cpp
// Purpose:     source for menu and menubar handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/02/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/menu.hpp"

namespace lev
{

  menu::menu() : control()
  {
    try {
      wxMenu *menu = new wxMenu();
      _obj = menu;
    }
    catch(...) {
      throw "menu: allocation error";
    }
  }

  menu::menu(const char *title) : control()
  {
    try {
      wxMenu *menu = new wxMenu(wxString(title, wxConvUTF8));
      _obj = menu;
    }
    catch(...) {
      fprintf(stderr, "menu: allocation error");
    }
  }


  int menu::append(int id, const char *str, const char *help_str)
  {
    wxString new_str(str, wxConvUTF8);
    wxString new_help(help_str, wxConvUTF8);
    wxMenuItem *item = ((wxMenu *)_obj)->Append(id, new_str, new_help);
    if (item == NULL) { return 0; }
    return item->GetId();
  }

  menubar::menubar() : control()
  {
    try {
      wxMenuBar *mb = new wxMenuBar();
      _obj = mb;
    }
    catch(...) {
      fprintf(stderr, "menubar: allocation error");
    }
  }


  bool menubar::append(menu *m, const char *title)
  {
    wxString new_title(title, wxConvUTF8);
    return ((wxMenuBar *)_obj)->Append((wxMenu *)m->_obj, new_title);
  }
}

