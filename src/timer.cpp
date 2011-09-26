/////////////////////////////////////////////////////////////////////////////
// Name:        src/timer.cpp
// Purpose:     source for timer handling classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     08/26/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "deriver.hpp"
#include "lev/timer.hpp"

int luaopen_lev_timer(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  open(L);
  globals(L)["require"]("lev.app");

  module(L, "lev")
  [
    namespace_("classes")
    [
      class_<stop_watch, base>("stop_watch")
        .def("get_time", &stop_watch::get_time)
        .def("pause", &stop_watch::pause)
        .def("resume", &stop_watch::resume)
        .def("start", &stop_watch::start)
        .def("start", &stop_watch::start0)
        .property("time", &stop_watch::get_time)
        .scope
        [
          def("create", &stop_watch::create)
        ],
      class_<timer, handler>("timer")
        .def("start", &timer::start)
        .def("start", &timer::start0)
        .def("start", &timer::start1)
        .def("stop", &timer::stop)
        .property("is_one_shot", &timer::is_one_shot)
        .property("is_running", &timer::is_running)
        .property("interval", &timer::get_interval)
        .property("notify", &timer::get_notify, &timer::set_notify)
        .scope
        [
          def("create", &timer::create)
        ]
    ]
  ];
  object lev = globals(L)["lev"];

  lev["stop_watch"] = lev["classes"]["stop_watch"]["create"];
  lev["timer"] = lev["classes"]["timer"]["create"];

  globals(L)["package"]["loaded"]["lev.timer"] = true;
  return 0;
}

namespace lev
{

  static wxStopWatch *cast_watch(void *obj) { return (wxStopWatch *)obj; }

  stop_watch::stop_watch() : base(), _obj(NULL) { }

  stop_watch::~stop_watch()
  {
    if (_obj) { delete cast_watch(_obj); }
  }

  stop_watch* stop_watch::create()
  {
    stop_watch *sw = NULL;
    try {
      sw = new stop_watch;
      sw->_obj = new wxStopWatch;
      return sw;
    }
    catch(...) {
      delete sw;
      return NULL;
    }
  }

  long stop_watch::get_time()
  {
    return cast_watch(_obj)->Time();
  }

  bool stop_watch::pause()
  {
    cast_watch(_obj)->Pause();
    return true;
  }

  bool stop_watch::resume()
  {
    cast_watch(_obj)->Resume();
    return true;
  }

  bool stop_watch::start(long initial_msec)
  {
    if (initial_msec < 0) { return false; }
    cast_watch(_obj)->Start(initial_msec);
    return true;
  }


  class myTimer : public myHandler<wxTimer>
  {
    public:

      myTimer() : myHandler<wxTimer>() { }

      ~myTimer() { }

      luabind::object GetNotify()
      {
        return notify;
      }

      void Notify()
      {
        if (notify && type(notify) == LUA_TFUNCTION) { notify(); }
      }

      bool SetNotify(luabind::object func)
      {
        notify = func;
        return true;
      }

      luabind::object notify;
  };

  static myTimer* cast_timer(void *obj) { return (myTimer *)obj; }

  timer::timer() : handler() { }

  timer::~timer() { }

  timer* timer::create()
  {
    timer* t = NULL;
    myTimer *obj = NULL;
    try {
      t = new timer;
      t->_obj = obj = new myTimer;
      return t;
    }
    catch (...) {
      delete t;
      return NULL;
    }
  }

  int timer::get_interval()
  {
    return cast_timer(_obj)->GetInterval();
  }

  luabind::object timer::get_notify()
  {
    return cast_timer(_obj)->GetNotify();
  }

  bool timer::is_one_shot()
  {
    return cast_timer(_obj)->IsOneShot();
  }

  bool timer::is_running()
  {
    return cast_timer(_obj)->IsRunning();
  }

  bool timer::set_notify(luabind::object func)
  {
    return cast_timer(_obj)->SetNotify(func);
  }

  bool timer::start(int milliseconds, bool one_shot)
  {
    return cast_timer(_obj)->Start(milliseconds, one_shot);
  }

  bool timer::stop()
  {
    cast_timer(_obj)->Stop();
    return true;
  }

}

