/////////////////////////////////////////////////////////////////////////////
// Name:        src/connect.cpp
// Purpose:     source for event call back handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/28/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/app.hpp"
#include "lev/event.hpp"
#include "lev/sizer.hpp"

namespace lev
{

  control::control() : base(), _id(0), _obj(NULL), _sz(NULL), wx_managed(false)
  { }

  control::~control()
  {
    if (wx_managed)
    {
      // wxWidgets will delete
    }
    else if (_id && !isvalid())
    {
      // the object isn't already found
    }
    else
    {
      // the object is under our management
      delete (wxWindow *)_obj;
    }
  }

  int control::getid()
  {
    return _id;
  }

  sizer *control::getsizer()
  {
    return _sz;
  }

  bool control::isshown()
  {
    return ((wxWindow *)_obj)->IsShown();
  }

  bool control::isvalid()
  {
    if (wxWindow::FindWindowById(_id)) { return true; }
    return false;
  }

  void control::setsizer(sizer *s)
  {
    if (s)
    {
      ((wxWindow *)_obj)->SetSizer((wxSizer *)s->get_rawobj());
      _sz = s;
      s->wx_managed = true;
    }
  }

  bool control::setshown(bool showing)
  {
    gui_lock();
    return ((wxWindow *)_obj)->Show(showing);
  }


  // event methods
  event::event(void *e) : base()
  {
    _obj = e;
  }

  event::~event() {}

  int event::get_id() const
  {
    return ((wxEvent *)_obj)->GetId();
  }

  const char *event::get_key() const
  {
    switch ( ((wxKeyEvent *)_obj)->GetKeyCode() )
    {
      case WXK_ALT:        return "ALT";
      case WXK_BACK:       return "BACK";
      case WXK_CANCEL:     return "CANCEL";
      case WXK_CAPITAL:    return "CAPITAL";
      case WXK_CLEAR:      return "CLEAR";
      case WXK_CONTROL:    return "CONTROL";
      case WXK_DELETE:     return "DELETE";
      case WXK_DOWN:       return "DOWN";
      case WXK_END:        return "END";
      case WXK_ESCAPE:     return "ESCAPE";
      case WXK_EXECUTE:    return "EXECUTE";
      case WXK_F1:         return "F1";
      case WXK_F2:         return "F2";
      case WXK_F3:         return "F3";
      case WXK_F4:         return "F4";
      case WXK_F5:         return "F5";
      case WXK_F6:         return "F6";
      case WXK_F7:         return "F7";
      case WXK_F8:         return "F8";
      case WXK_F9:         return "F9";
      case WXK_F10:        return "F10";
      case WXK_F11:        return "F11";
      case WXK_F12:        return "F12";
      case WXK_HOME:       return "HOME";
      case WXK_INSERT:     return "INSERT";
      case WXK_LBUTTON:    return "LBUTTON";
      case WXK_LEFT:       return "LEFT";
      case WXK_MBUTTON:    return "MBUTTON";
      case WXK_MENU:       return "MENU";
      case WXK_PAUSE:      return "PAUSE";
      case WXK_PRINT:      return "PRINT";
      case WXK_RBUTTON:    return "RBUTTON";
      case WXK_RIGHT:      return "RIGHT";
      case WXK_RETURN:     return "RETURN";
      case WXK_SHIFT:      return "SHIFT";
      case WXK_SELECT:     return "SELECT";
      case WXK_SNAPSHOT:   return "SNAPSHOT";
      case WXK_SPACE:      return "SPACE";
      case WXK_START:      return "START";
      case WXK_TAB:        return "TAB";
      case WXK_UP:         return "UP";
      case wxKeyCode('A'): return "a";
      case wxKeyCode('B'): return "b";
      case wxKeyCode('C'): return "c";
      case wxKeyCode('D'): return "d";
      case wxKeyCode('E'): return "e";
      case wxKeyCode('F'): return "f";
      case wxKeyCode('G'): return "g";
      case wxKeyCode('H'): return "h";
      case wxKeyCode('I'): return "i";
      case wxKeyCode('J'): return "j";
      case wxKeyCode('K'): return "k";
      case wxKeyCode('L'): return "l";
      case wxKeyCode('M'): return "m";
      case wxKeyCode('N'): return "n";
      case wxKeyCode('O'): return "o";
      case wxKeyCode('P'): return "p";
      case wxKeyCode('Q'): return "q";
      case wxKeyCode('R'): return "r";
      case wxKeyCode('S'): return "s";
      case wxKeyCode('T'): return "t";
      case wxKeyCode('U'): return "u";
      case wxKeyCode('V'): return "v";
      case wxKeyCode('W'): return "w";
      case wxKeyCode('X'): return "x";
      case wxKeyCode('Y'): return "y";
      case wxKeyCode('Z'): return "z";
      case wxKeyCode('0'): return "0";
      case wxKeyCode('1'): return "1";
      case wxKeyCode('2'): return "2";
      case wxKeyCode('3'): return "3";
      case wxKeyCode('4'): return "4";
      case wxKeyCode('5'): return "5";
      case wxKeyCode('6'): return "6";
      case wxKeyCode('7'): return "7";
      case wxKeyCode('8'): return "8";
      case wxKeyCode('9'): return "9";
      default:             return "";
    }
  }

  int event::get_keycode() const
  {
    return ((wxKeyEvent *)_obj)->GetKeyCode();
  }

  bool event::request()
  {
    ((wxIdleEvent *)_obj)->RequestMore(true);
    return true;
  }

  void event::skip()
  {
    ((wxEvent *)_obj)->Skip(true);
  }
}

