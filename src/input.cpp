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

#include <luabind/luabind.hpp>

int luaopen_lev_input(lua_State *L)
{
  using namespace luabind;
  using namespace lev;

  module(L, "lev")
  [
    namespace_("input")
    [
      class_<inbase, base>("inbase")
        .property("left_down", &inbase::get_left_down)
        .property("middle_down", &inbase::get_middle_down)
        .property("right_down", &inbase::get_right_down),
      class_<instate, inbase>("instate")
        .property("x", &instate::get_x)
        .property("y", &instate::get_y),
      class_<inrecord, inbase>("inrecord")
    ]
  ];

  return 0;
}

namespace lev
{

  instate::instate() : inbase(), _obj(NULL) { }

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
      myRecord() { Clear(); }

      bool Clear()
      {
        left_down = left_up = false;
        middle_down = middle_up = false;
        right_down = right_up = false;
      }

      bool left_down, left_up;
      bool middle_down, middle_up;
      bool right_down, right_up;
  };

  inrecord::inrecord() : inbase(), _obj(NULL)
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

  bool inrecord::set_left_down()
  {
    if (_obj == NULL) { return false; }
    ((myRecord *)_obj)->left_down = true;
    return true;
  }

  bool inrecord::set_middle_down()
  {
    if (_obj == NULL) { return false; }
    ((myRecord *)_obj)->middle_down = true;
    return true;
  }

  bool inrecord::set_right_down()
  {
    if (_obj == NULL) { return false; }
    ((myRecord *)_obj)->right_down = true;
    return true;
  }
}

