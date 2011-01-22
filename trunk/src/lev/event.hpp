#ifndef _EVENT_HPP
#define _EVENT_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/connect.cpp
// Purpose:     header for event call back handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/28/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include <luabind/luabind.hpp>
#include "base.hpp"

namespace luabind { class object; }

namespace levana
{
  class sizer;

  class control : public base
  {
    protected:
      inline control() : base(), id(0) {}
      inline ~control() {}
    public:
      virtual void connect(int type, luabind::object lua_func)
      { throw "control: derived connect method has not been implemented"; }
      virtual void setonkeydown(luabind::object lua_func)
      { throw "control: derived setonkeydown method has not been implemented"; }
      virtual void setonmenu(int id, luabind::object lua_func)
      { throw "control: derived setonmenu method has not been implemented"; }
      sizer *getsizer();
      inline bool hide() { return setshown(false); }
      bool isshown();
      bool isvalid();
      void setsizer(sizer *s);
      bool setshown(bool showing);
      inline bool show() { return setshown(true); }

      // friend classes
      friend class canvas;
      friend class frame;
      friend class htmlview;
      friend class player;
      friend class sizer;
      friend class text;
    private:
      int id;
  };


  class event : public base
  {
    public:
      inline event(void *e) : base() { _obj = e; }
      inline ~event() {}
      int getkey() const;
      void skip();
  };
}

#endif // _EVENT_HPP
