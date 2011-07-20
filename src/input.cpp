/////////////////////////////////////////////////////////////////////////////
// Name:        input.cpp
// Purpose:     source for input managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     06/08/2011
// Copyright:   (C) 2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/input.hpp"

#include <algorithm>
#include <luabind/luabind.hpp>
#include <map>

int luaopen_lev_input(lua_State *L)
{
  using namespace luabind;
  using namespace lev;

  open(L);
  globals(L)["require"]("lev");

  module(L, "lev")
  [
    namespace_("input")
    [
      def("to_keycode", &input::to_keycode),
      def("to_keystr", &input::to_keystr),
      class_<instate, base>("instate")
        .property("left_down", &instate::get_left_down)
        .property("middle_down", &instate::get_middle_down)
        .property("right_down", &instate::get_right_down)
        .property("x", &instate::get_x)
        .property("y", &instate::get_y),
      class_<inrecord, base>("inrecord")
        .def("get_keydown", &inrecord::get_keydown)
        .def("track_key", &inrecord::track_key)
        .def("track_mouse", &inrecord::track_mouse)
        .property("left_down", &inrecord::get_left_down)
        .property("middle_down", &inrecord::get_middle_down)
        .property("right_down", &inrecord::get_right_down)
        .property("x", &inrecord::get_x)
        .property("y", &inrecord::get_y)
    ]
  ];

  globals(L)["package"]["loaded"]["lev.input"] = true;
  return 0;
}

namespace lev
{

  long input::to_keycode(const char *keystr)
  {
    static std::map<std::string, long> *codemap = NULL;
    if (keystr == NULL) { return -1; }

    std::string str = keystr;
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if (codemap == NULL)
    {
      try {
        codemap = new std::map<std::string, long>;

        (*codemap)["add"]              = WXK_ADD;
        (*codemap)["alt"]              = WXK_ALT;
        (*codemap)["back"]             = WXK_BACK;
        (*codemap)["backspace"]        = WXK_BACK;
        (*codemap)["bs"]               = WXK_BACK;
        (*codemap)["cancel"]           = WXK_CANCEL;
        (*codemap)["capital"]          = WXK_CAPITAL;
        (*codemap)["clear"]            = WXK_CLEAR;
        (*codemap)["command"]          = WXK_COMMAND;
        (*codemap)["control"]          = WXK_CONTROL;
        (*codemap)["ctrl"]             = WXK_CONTROL;
        (*codemap)["decimal"]          = WXK_DECIMAL;
        (*codemap)["del"]              = WXK_DELETE;
        (*codemap)["delete"]           = WXK_DELETE;
        (*codemap)["divide"]           = WXK_DIVIDE;
        (*codemap)["down"]             = WXK_DOWN;
        (*codemap)["end"]              = WXK_END;
        (*codemap)["enter"]            = WXK_RETURN;
        (*codemap)["esc"]              = WXK_ESCAPE;
        (*codemap)["escape"]           = WXK_ESCAPE;
        (*codemap)["execute"]          = WXK_EXECUTE;
        (*codemap)["f1"]               = WXK_F1;
        (*codemap)["f2"]               = WXK_F2;
        (*codemap)["f3"]               = WXK_F3;
        (*codemap)["f4"]               = WXK_F4;
        (*codemap)["f5"]               = WXK_F5;
        (*codemap)["f6"]               = WXK_F6;
        (*codemap)["f7"]               = WXK_F7;
        (*codemap)["f8"]               = WXK_F8;
        (*codemap)["f9"]               = WXK_F9;
        (*codemap)["f10"]              = WXK_F10;
        (*codemap)["f11"]              = WXK_F11;
        (*codemap)["f12"]              = WXK_F12;
        (*codemap)["f13"]              = WXK_F13;
        (*codemap)["f14"]              = WXK_F14;
        (*codemap)["f15"]              = WXK_F15;
        (*codemap)["f16"]              = WXK_F16;
        (*codemap)["f17"]              = WXK_F17;
        (*codemap)["f18"]              = WXK_F18;
        (*codemap)["f19"]              = WXK_F19;
        (*codemap)["f20"]              = WXK_F20;
        (*codemap)["f21"]              = WXK_F21;
        (*codemap)["f22"]              = WXK_F22;
        (*codemap)["f23"]              = WXK_F23;
        (*codemap)["f24"]              = WXK_F24;
        (*codemap)["help"]             = WXK_HELP;
        (*codemap)["home"]             = WXK_HOME;
        (*codemap)["insert"]           = WXK_INSERT;
        (*codemap)["lb"]               = WXK_LBUTTON;
        (*codemap)["lbutton"]          = WXK_LBUTTON;
        (*codemap)["left"]             = WXK_LEFT;
        (*codemap)["leftbutton"]       = WXK_LBUTTON;
        (*codemap)["mb"]               = WXK_MBUTTON;
        (*codemap)["mbutton"]          = WXK_MBUTTON;
        (*codemap)["menu"]             = WXK_MENU;
        (*codemap)["middlebutton"]     = WXK_MBUTTON;
        (*codemap)["multiply"]         = WXK_MULTIPLY;
        (*codemap)["numlock"]          = WXK_NUMLOCK;
        (*codemap)["numpad0"]          = WXK_NUMPAD0;
        (*codemap)["numpad1"]          = WXK_NUMPAD1;
        (*codemap)["numpad2"]          = WXK_NUMPAD2;
        (*codemap)["numpad3"]          = WXK_NUMPAD3;
        (*codemap)["numpad4"]          = WXK_NUMPAD4;
        (*codemap)["numpad5"]          = WXK_NUMPAD5;
        (*codemap)["numpad6"]          = WXK_NUMPAD6;
        (*codemap)["numpad7"]          = WXK_NUMPAD7;
        (*codemap)["numpad8"]          = WXK_NUMPAD8;
        (*codemap)["numpad9"]          = WXK_NUMPAD9;
        (*codemap)["numpad_add"]       = WXK_NUMPAD_ADD;
        (*codemap)["numpad_begin"]     = WXK_NUMPAD_BEGIN;
        (*codemap)["numpad_decimal"]   = WXK_NUMPAD_DECIMAL;
        (*codemap)["numpad_delete"]    = WXK_NUMPAD_DELETE;
        (*codemap)["numpad_divide"]    = WXK_NUMPAD_DIVIDE;
        (*codemap)["numpad_down"]      = WXK_NUMPAD_DOWN;
        (*codemap)["numpad_end"]       = WXK_NUMPAD_END;
        (*codemap)["numpad_enter"]     = WXK_NUMPAD_ENTER;
        (*codemap)["numpad_equal"]     = WXK_NUMPAD_EQUAL;
        (*codemap)["numpad_f1"]        = WXK_NUMPAD_F1;
        (*codemap)["numpad_f2"]        = WXK_NUMPAD_F2;
        (*codemap)["numpad_f3"]        = WXK_NUMPAD_F3;
        (*codemap)["numpad_f4"]        = WXK_NUMPAD_F4;
        (*codemap)["numpad_home"]      = WXK_NUMPAD_HOME;
        (*codemap)["numpad_insert"]    = WXK_NUMPAD_INSERT;
        (*codemap)["numpad_left"]      = WXK_NUMPAD_LEFT;
        (*codemap)["numpad_multiply"]  = WXK_NUMPAD_MULTIPLY;
        (*codemap)["numpad_pagedown"]  = WXK_NUMPAD_PAGEDOWN;
        (*codemap)["numpad_pageup"]    = WXK_NUMPAD_PAGEUP;
        (*codemap)["numpad_right"]     = WXK_NUMPAD_RIGHT;
        (*codemap)["numpad_separator"] = WXK_NUMPAD_SEPARATOR;
        (*codemap)["numpad_space"]     = WXK_NUMPAD_SPACE;
        (*codemap)["numpad_subtract"]  = WXK_NUMPAD_SUBTRACT;
        (*codemap)["numpad_tab"]       = WXK_NUMPAD_TAB;
        (*codemap)["numpad_up"]        = WXK_NUMPAD_UP;
        (*codemap)["pagedown"]         = WXK_PAGEDOWN;
        (*codemap)["pageup"]           = WXK_PAGEUP;
        (*codemap)["pause"]            = WXK_PAUSE;
        (*codemap)["pgdn"]             = WXK_PAGEDOWN;
        (*codemap)["pgup"]             = WXK_PAGEUP;
        (*codemap)["print"]            = WXK_PRINT;
        (*codemap)["rb"]               = WXK_RBUTTON;
        (*codemap)["rbutton"]          = WXK_RBUTTON;
        (*codemap)["rightbutton"]      = WXK_RBUTTON;
        (*codemap)["return"]           = WXK_RETURN;
        (*codemap)["right"]            = WXK_RIGHT;
        (*codemap)["scroll"]           = WXK_SCROLL;
        (*codemap)["select"]           = WXK_SELECT;
        (*codemap)["separator"]        = WXK_SEPARATOR;
        (*codemap)["shift"]            = WXK_SHIFT;
        (*codemap)["snapshot"]         = WXK_SNAPSHOT;
        (*codemap)["space"]            = WXK_SPACE;
        (*codemap)["special1"]         = WXK_SPECIAL1;
        (*codemap)["special2"]         = WXK_SPECIAL2;
        (*codemap)["special3"]         = WXK_SPECIAL3;
        (*codemap)["special4"]         = WXK_SPECIAL4;
        (*codemap)["special5"]         = WXK_SPECIAL5;
        (*codemap)["special6"]         = WXK_SPECIAL6;
        (*codemap)["special7"]         = WXK_SPECIAL7;
        (*codemap)["special8"]         = WXK_SPECIAL8;
        (*codemap)["special9"]         = WXK_SPECIAL9;
        (*codemap)["special10"]        = WXK_SPECIAL10;
        (*codemap)["special11"]        = WXK_SPECIAL11;
        (*codemap)["special12"]        = WXK_SPECIAL12;
        (*codemap)["special13"]        = WXK_SPECIAL13;
        (*codemap)["special14"]        = WXK_SPECIAL14;
        (*codemap)["special15"]        = WXK_SPECIAL15;
        (*codemap)["special16"]        = WXK_SPECIAL16;
        (*codemap)["special17"]        = WXK_SPECIAL17;
        (*codemap)["special18"]        = WXK_SPECIAL18;
        (*codemap)["special19"]        = WXK_SPECIAL19;
        (*codemap)["special20"]        = WXK_SPECIAL20;
        (*codemap)["start"]            = WXK_START;
        (*codemap)["subtract"]         = WXK_SUBTRACT;
        (*codemap)["tab"]              = WXK_TAB;
        (*codemap)["up"]               = WXK_UP;
        (*codemap)["windows_left"]     = WXK_WINDOWS_LEFT;
        (*codemap)["windows_menu"]     = WXK_WINDOWS_MENU;
        (*codemap)["windows_right"]    = WXK_WINDOWS_RIGHT;

        for (int i = 0; i <= 127; i++)
        {
          char ascii[2] = {tolower(i), 0};
          (*codemap)[ascii] = wxKeyCode(toupper(i));
        }
      }
      catch (...) {
        delete codemap;
        codemap = NULL;
        return -1;
      }
    }

    std::map<std::string, long>::iterator found;
    found = codemap->find(str);
    if (found == codemap->end()) { return -1; }
    else { return found->second; }
  }

  const char *input::to_keystr(long code)
  {
    static std::map<long, std::string> *keymap = NULL;

    if (keymap == NULL)
    {
      try {
        keymap = new std::map<long, std::string>;

        for (int i = 0; i <= 127; i++)
        {
          char ascii[2] = {tolower(i), 0};
          (*keymap)[wxKeyCode(toupper(i))] = ascii;
        }

        (*keymap)[WXK_ADD]              = "add";
        (*keymap)[WXK_ALT]              = "alt";
        (*keymap)[WXK_BACK]             = "back";
        (*keymap)[WXK_CANCEL]           = "cancel";
        (*keymap)[WXK_CAPITAL]          = "capital";
        (*keymap)[WXK_CLEAR]            = "clear";
        (*keymap)[WXK_COMMAND]          = "command";
        (*keymap)[WXK_CONTROL]          = "control";
        (*keymap)[WXK_DECIMAL]          = "decimal";
        (*keymap)[WXK_DELETE]           = "delete";
        (*keymap)[WXK_DIVIDE]           = "divide";
        (*keymap)[WXK_DOWN]             = "down";
        (*keymap)[WXK_END]              = "end";
        (*keymap)[WXK_ESCAPE]           = "escape";
        (*keymap)[WXK_EXECUTE]          = "execute";
        (*keymap)[WXK_F1]               = "f1";
        (*keymap)[WXK_F2]               = "f2";
        (*keymap)[WXK_F3]               = "f3";
        (*keymap)[WXK_F4]               = "f4";
        (*keymap)[WXK_F5]               = "f5";
        (*keymap)[WXK_F6]               = "f6";
        (*keymap)[WXK_F7]               = "f7";
        (*keymap)[WXK_F8]               = "f8";
        (*keymap)[WXK_F9]               = "f9";
        (*keymap)[WXK_F10]              = "f10";
        (*keymap)[WXK_F11]              = "f11";
        (*keymap)[WXK_F12]              = "f12";
        (*keymap)[WXK_F13]              = "f13";
        (*keymap)[WXK_F14]              = "f14";
        (*keymap)[WXK_F15]              = "f15";
        (*keymap)[WXK_F16]              = "f16";
        (*keymap)[WXK_F17]              = "f17";
        (*keymap)[WXK_F18]              = "f18";
        (*keymap)[WXK_F19]              = "f19";
        (*keymap)[WXK_F20]              = "f20";
        (*keymap)[WXK_F21]              = "f21";
        (*keymap)[WXK_F22]              = "f22";
        (*keymap)[WXK_F23]              = "f23";
        (*keymap)[WXK_F24]              = "f24";
        (*keymap)[WXK_HELP]             = "help";
        (*keymap)[WXK_HOME]             = "home";
        (*keymap)[WXK_INSERT]           = "insert";
        (*keymap)[WXK_LBUTTON]          = "lbutton";
        (*keymap)[WXK_LEFT]             = "left";
        (*keymap)[WXK_MBUTTON]          = "mbutton";
        (*keymap)[WXK_MENU]             = "menu";
        (*keymap)[WXK_MULTIPLY]         = "multiply";
        (*keymap)[WXK_NUMLOCK]          = "numlock";
        (*keymap)[WXK_NUMPAD0]          = "numpad0";
        (*keymap)[WXK_NUMPAD1]          = "numpad1";
        (*keymap)[WXK_NUMPAD2]          = "numpad2";
        (*keymap)[WXK_NUMPAD3]          = "numpad3";
        (*keymap)[WXK_NUMPAD4]          = "numpad4";
        (*keymap)[WXK_NUMPAD5]          = "numpad5";
        (*keymap)[WXK_NUMPAD6]          = "numpad6";
        (*keymap)[WXK_NUMPAD7]          = "numpad7";
        (*keymap)[WXK_NUMPAD8]          = "numpad8";
        (*keymap)[WXK_NUMPAD9]          = "numpad9";
        (*keymap)[WXK_NUMPAD_ADD]       = "numpad_add";
        (*keymap)[WXK_NUMPAD_BEGIN]     = "numpad_begin";
        (*keymap)[WXK_NUMPAD_DECIMAL]   = "numpad_decimal";
        (*keymap)[WXK_NUMPAD_DELETE]    = "numpad_delete";
        (*keymap)[WXK_NUMPAD_DIVIDE]    = "numpad_divide";
        (*keymap)[WXK_NUMPAD_DOWN]      = "numpad_down";
        (*keymap)[WXK_NUMPAD_END]       = "numpad_end";
        (*keymap)[WXK_NUMPAD_ENTER]     = "numpad_enter";
        (*keymap)[WXK_NUMPAD_EQUAL]     = "numpad_equal";
        (*keymap)[WXK_NUMPAD_F1]        = "numpad_f1";
        (*keymap)[WXK_NUMPAD_F2]        = "numpad_f2";
        (*keymap)[WXK_NUMPAD_F3]        = "numpad_f3";
        (*keymap)[WXK_NUMPAD_F4]        = "numpad_f4";
        (*keymap)[WXK_NUMPAD_HOME]      = "numpad_home";
        (*keymap)[WXK_NUMPAD_INSERT]    = "numpad_insert";
        (*keymap)[WXK_NUMPAD_LEFT]      = "numpad_left";
        (*keymap)[WXK_NUMPAD_MULTIPLY]  = "numpad_multiply";
        (*keymap)[WXK_NUMPAD_PAGEDOWN]  = "numpad_pagedown";
        (*keymap)[WXK_NUMPAD_PAGEUP]    = "numpad_pageup";
        (*keymap)[WXK_NUMPAD_RIGHT]     = "numpad_right";
        (*keymap)[WXK_NUMPAD_SEPARATOR] = "numpad_separator";
        (*keymap)[WXK_NUMPAD_SPACE]     = "numpad_space";
        (*keymap)[WXK_NUMPAD_SUBTRACT]  = "numpad_subtract";
        (*keymap)[WXK_NUMPAD_TAB]       = "numpad_tab";
        (*keymap)[WXK_NUMPAD_UP]        = "numpad_up";
        (*keymap)[WXK_PAGEDOWN]         = "pagedown";
        (*keymap)[WXK_PAGEUP]           = "pageup";
        (*keymap)[WXK_PAUSE]            = "pause";
        (*keymap)[WXK_PRINT]            = "print";
        (*keymap)[WXK_RBUTTON]          = "rbutton";
        (*keymap)[WXK_RETURN]           = "return";
        (*keymap)[WXK_RIGHT]            = "right";
        (*keymap)[WXK_SCROLL]           = "scroll";
        (*keymap)[WXK_SELECT]           = "select";
        (*keymap)[WXK_SEPARATOR]        = "separator";
        (*keymap)[WXK_SHIFT]            = "shift";
        (*keymap)[WXK_SNAPSHOT]         = "snapshot";
        (*keymap)[WXK_SPACE]            = "space";
        (*keymap)[WXK_SPECIAL1]         = "special1";
        (*keymap)[WXK_SPECIAL2]         = "special2";
        (*keymap)[WXK_SPECIAL3]         = "special3";
        (*keymap)[WXK_SPECIAL4]         = "special4";
        (*keymap)[WXK_SPECIAL5]         = "special5";
        (*keymap)[WXK_SPECIAL6]         = "special6";
        (*keymap)[WXK_SPECIAL7]         = "special7";
        (*keymap)[WXK_SPECIAL8]         = "special8";
        (*keymap)[WXK_SPECIAL9]         = "special9";
        (*keymap)[WXK_SPECIAL10]        = "special10";
        (*keymap)[WXK_SPECIAL11]        = "special11";
        (*keymap)[WXK_SPECIAL12]        = "special12";
        (*keymap)[WXK_SPECIAL13]        = "special13";
        (*keymap)[WXK_SPECIAL14]        = "special14";
        (*keymap)[WXK_SPECIAL15]        = "special15";
        (*keymap)[WXK_SPECIAL16]        = "special16";
        (*keymap)[WXK_SPECIAL17]        = "special17";
        (*keymap)[WXK_SPECIAL18]        = "special18";
        (*keymap)[WXK_SPECIAL19]        = "special19";
        (*keymap)[WXK_SPECIAL20]        = "special20";
        (*keymap)[WXK_START]            = "start";
        (*keymap)[WXK_SUBTRACT]         = "subtract";
        (*keymap)[WXK_TAB]              = "tab";
        (*keymap)[WXK_UP]               = "up";
        (*keymap)[WXK_WINDOWS_LEFT]     = "windows_left";
        (*keymap)[WXK_WINDOWS_MENU]     = "windows_menu";
        (*keymap)[WXK_WINDOWS_RIGHT]    = "windows_right";
      }
      catch (...) {
        delete keymap;
        keymap = NULL;
        return NULL;
      }
    }

    std::map<long, std::string>::iterator found;
    found = keymap->find(code);
    if (found == keymap->end()) { return NULL; }
    else { return found->second.c_str(); }
  }


  instate::instate() : base(), _obj(NULL) { }

  instate::~instate() { delete (wxMouseState *)_obj; }

  int instate::get_x() const
  {
    return ((wxMouseState *)_obj)->GetX();
  }

  int instate::get_y() const
  {
    return ((wxMouseState *)_obj)->GetY();
  }

  bool instate::get_left_down() const
  {
    return ((wxMouseState *)_obj)->LeftIsDown();
  }

  bool instate::get_middle_down() const
  {
    return ((wxMouseState *)_obj)->MiddleIsDown();
  }

  bool instate::get_right_down() const
  {
    return ((wxMouseState *)_obj)->RightIsDown();
  }


  struct myRecord
  {
    public:
      myRecord() : mouse_tracking(false) { Clear(); }

      bool Clear()
      {
        std::map<long, bool>::iterator i = track_map.begin();
        for ( ; i != track_map.end() ; i++)
        {
          i->second = false;
        }
        left_down = left_up = false;
        middle_down = middle_up = false;
        right_down = right_up = false;
        x = -1;
        y = -1;
      }

      bool Record()
      {
        std::map<long, bool>::iterator i = track_map.begin();
        for ( ; i != track_map.end() ; i++ )
        {
          if (wxGetKeyState(wxKeyCode(i->first))) { i->second = true; }
        }
        if (mouse_tracking)
        {
          wxMouseState ms = wxGetMouseState();
          if (ms.LeftIsDown())   { left_down = true; }
          if (ms.MiddleIsDown()) { middle_down = true; }
          if (ms.RightIsDown())  { right_down = true; }
          x = ms.GetX();
          y = ms.GetY();
        }
      }

      bool TrackKey(const char *key)
      {
        long code = input::to_keycode(key);
        if (code < 0) { return false; }
        track_map[code] = false;
        return true;
      }

      bool left_down, left_up;
      bool middle_down, middle_up;
      bool right_down, right_up;
      bool mouse_tracking;
      std::map<long, bool> track_map;
      int  x, y;
  };
  static myRecord* cast_rec(void *obj) { return (myRecord *)obj; }

  inrecord::inrecord() : base(), _obj(NULL)
  {
    try {
      _obj = new myRecord;
    }
    catch (...) { }
  }

  inrecord::~inrecord()
  {
    if (_obj) { delete (myRecord *)_obj; }
  }

  bool inrecord::clear()
  {
    if (_obj == NULL) { return false; }
    return ((myRecord *)_obj)->Clear();
  }

  bool inrecord::get_keydown(const char *keystr) const
  {
    if (_obj == NULL) { return false; }
    std::map<long, bool> &tmap = cast_rec(_obj)->track_map;
    std::map<long, bool>::iterator found = tmap.find(input::to_keycode(keystr));
    if (found == tmap.end()) { return false; }
    return found->second;
  }

  bool inrecord::get_left_down() const
  {
    if (_obj == NULL) { return false; }
    return ((myRecord *)_obj)->left_down;
  }

  bool inrecord::get_middle_down() const
  {
    if (_obj == NULL) { return false; }
    return ((myRecord *)_obj)->middle_down;
  }

  bool inrecord::get_right_down() const
  {
    if (_obj == NULL) { return false; }
    return ((myRecord *)_obj)->right_down;
  }

  int inrecord::get_x() const
  {
    return cast_rec(_obj)->x;
  }

  int inrecord::get_y() const
  {
    return cast_rec(_obj)->y;
  }

  bool inrecord::record()
  {
    if (_obj == NULL) { return false; }
    return cast_rec(_obj)->Record();
  }

  bool inrecord::track_key(const char *keystr)
  {
    if (_obj == NULL) { return false; }
    return cast_rec(_obj)->TrackKey(keystr);
  }

  bool inrecord::track_mouse()
  {
    if (_obj == NULL) { return false; }
    cast_rec(_obj)->mouse_tracking = true;
    return true;
  }

}

