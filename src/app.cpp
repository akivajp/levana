/////////////////////////////////////////////////////////////////////////////
// Name:        src/app.cpp
// Purpose:     source for application managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/app.hpp"
#include "lev/frame.hpp"
#include "deriver.hpp"
#include <lua.h>
#include <string>

int luaopen_lev_app(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  open(L);
  globals(L)["require"]("lev.input");

  module(L, "lev")
  [
    namespace_("classes")
    [
      // event class
      class_<event, base>("event")
        .property("char", &event::get_char)
        .property("id", &event::get_id)
        .property("key", &event::get_keystr)
        .property("keystr", &event::get_keystr)
        .property("keycode", &event::get_keycode)
        .def("request", &event::request)
        .def("skip", &event::skip)
        .property("x", &event::get_x)
        .property("y", &event::get_y),
      // event handler class
      class_<handler, base>("handler")
        .def("get_on_menu", &handler::get_on_menu)
        .def("set_on_menu", &handler::set_on_menu)
        .property("on_any", &handler::get_on_any, &handler::set_on_any)
        .property("on_char", &handler::get_on_char, &handler::set_on_char)
        .property("on_close", &handler::get_on_close, &handler::set_on_close)
        .property("on_idle", &handler::get_on_idle, &handler::set_on_idle)
        .property("on_key_down", &handler::get_on_key_down, &handler::set_on_key_down)
        .property("on_key_up", &handler::get_on_key_up, &handler::set_on_key_up)
        .property("on_left_down", &handler::get_on_left_down, &handler::set_on_left_down)
        .property("on_motion", &handler::get_on_motion, &handler::set_on_motion)
        .property("on_right_down", &handler::get_on_right_down, &handler::set_on_right_down),
      // application management class
      class_<application, handler>("app")
        .def("autoloop", &application::autoloop)
        .def("autoloop", &application::autoloop_with)
        .def("check_key_down", &application::check_key_down)
        .property("fps", &application::get_fps, &application::set_fps)
        .property("input", &application::get_instate)
        .property("inrec", &application::get_inrecord)
        .property("inrecord", &application::get_inrecord)
        .property("instate", &application::get_instate)
        .property("interval", &application::get_interval, &application::set_interval)
        .property("locale", &application::get_locale)
        .property("name", &application::get_name, &application::set_name)
        .property("pid", &application::get_process_id)
        .property("process_id", &application::get_process_id)
        .def("run", &application::run)
        .def("sleep", &application::sleep)
        .def("sleep", &application::sleep_def)
        .property("tick", &application::get_tick, &application::set_tick)
        .property("title", &application::get_name, &application::set_name)
        .property("top",  &application::get_top,  &application::set_top)
        .property("top_window",  &application::get_top,  &application::set_top)
        .def("track_key", &application::track_key)
        .def("track_mouse", &application::track_mouse)
        .def("wait",  &application::wait)
        .def("yield", &application::yield)
        .scope
        [
          def("get", &application::get_app)
        ]
    ]
  ];
  object lev = globals(L)["lev"];
  object classes = lev["classes"];

  lev["app"] = classes["app"]["get"];

  globals(L)["package"]["loaded"]["lev.app"] = true;
  return 0;
}

namespace lev
{

  class myApp : public myHandler<wxApp>
  {
    public:
      inline myApp()
        : istate(), rec(), fps(60), locale(wxLANGUAGE_DEFAULT),
          offset(0), sw() { }

      void set_argv()
      {
        try {
          argv_utf8 = new char*[this->argc];
          for (int i = 0; i < this->argc; i++)
          {
            wxString str(this->argv[i]);
            int len = strlen(str.mb_str(wxConvUTF8));
            argv_utf8[i] = new char[len + 1];
            strcpy(argv_utf8[i], str.mb_str(wxConvUTF8));
          }
        }
        catch(...) {
          throw "app: initialization failed";
          exit(-1);
        }
      }

      void del_argv()
      {
        for (int i = 0; i < this->argc; i++)
        {
          delete[] argv_utf8[i];
        }
        delete[] argv_utf8;
        argv_utf8 = NULL;
      }

//      int FilterEvent(wxEvent &event)
//      {
//        if (event.GetEventType() == wxEVT_KEY_DOWN)
//        {
//          printf("ANY KEY!\n");
//          return true;
//        }
//        return -1;
//      }

      virtual bool OnInit(void)
      {
        set_argv();
        old_loop = m_mainLoop;
        m_mainLoop = new wxEventLoop();
        wxEventLoop::SetActive(m_mainLoop);

        wxSocketBase::Initialize();
        wxInitAllImageHandlers();
        wxFileSystem::AddHandler(new wxInternetFSHandler);

        locale.AddCatalogLookupPathPrefix(wxT("locale"));

        sw.Start();
        return true;
      }

      virtual int OnExit(void)
      {
        del_argv();
        if (m_mainLoop)
        {
          delete m_mainLoop;
          m_mainLoop = NULL;
        }
      }

      bool Yield()
      {
        if (!this->Pending()) { this->ProcessIdle(); }
//        while(!this->Pending()) { this->ProcessIdle(); }
//        while (this->Pending() && Dispatch());
        return wxTheApp->Yield(true);
      }

      lua_State *L;
      char **argv_utf8;
      wxEventLoop *old_loop;
      double fps, offset;
      instate  istate;
      inrecord rec;
      wxLocale locale;
      wxStopWatch sw;
  };
  static myApp* cast_app(void *obj) { return (myApp *)obj; }

}

DECLARE_APP(lev::myApp);

namespace lev
{

  application::application() : handler(), tick()
  {
    wxGetApp().OnInit();
//    set_name("Levana Application");
    set_name("levana");
    _obj = &(wxGetApp());
    connector = wxGetApp().GetConnector();
    func_getter = wxGetApp().GetFuncGetter();
  }

  application::~application()
  {
    wxGetApp().OnExit();
  }

  void application::autoloop()
  {
    while (wxGetApp().GetTopWindow())
    {
      wxGetApp().Yield();
    }
  }
  void application::autoloop_with(frame *frm)
  {
    if (frm == NULL) { this->autoloop(); }
    else
    {
      while (frm->is_valid())
      {
        wxGetApp().Yield();
      }
    }
  }


  bool application::check_key_down(const char *key)
  {
    return wxGetKeyState(wxKeyCode(input::to_keycode(key)));
  }


  bool application::entry(lua_State *L, int argc, char **argv)
  {
    static bool entried = false;
    if (entried) { return false; }
    if (!wxEntryStart(argc, argv)) { return false; }
    wxGetApp().L = L;
    entried = true;
    return true;
  }

  application *application::get_app()
  {
    static application app;
    return &app;
  }

  double application::get_fps()
  {
    return cast_app(_obj)->fps;
  }

  double application::get_interval()
  {
    return 1000 / cast_app(_obj)->fps;
  }

  inrecord* application::get_inrecord()
  {
    inrecord &in = wxGetApp().rec;
    if (in._obj) { return &in; }
    else { return NULL; }
  }

  instate* application::get_instate()
  {
    instate &in = wxGetApp().istate;
    if (in._obj)
    {
      *((wxMouseState *)in._obj) = wxGetMouseState();
      return &in;
    }
    else
    {
      in._obj = new wxMouseState();
      if (in._obj == NULL) { return NULL; }
      *((wxMouseState *)in._obj) = wxGetMouseState();
      return &in;
    }
  }

  locale* application::get_locale()
  {
    static locale loc;
    if (loc._obj) { return &loc; }
    loc._obj = &(wxGetApp().locale);
    return &loc;
  }

  std::string application::get_name()
  {
    return (const char *)wxGetApp().GetAppName().mb_str();
  }

  unsigned long application::get_process_id()
  {
    return wxGetProcessId();
  }

  luabind::object application::get_tick()
  {
    return tick;
  }

  frame *application::get_top()
  {
    return frame::get_top();
  }

  void application::mainloop()
  {
    wxGetApp().MainLoop();
  }

  bool application::on_tick()
  {
    if (!tick) { return true; }
    if (luabind::type(tick) != LUA_TFUNCTION) { return false; }

    object result = tick();
    if (result == false) { return false; }
    if (!result) { return true; }
    return true;
  }

  bool application::run()
  {
    while (tick || get_top())
    {
      wait();
      if (on_tick() == false) { break; }
    }
    return true;
  }

  bool application::set_fps(double fps)
  {
    if (fps <= 0) { return false; }
    cast_app(_obj)->fps = fps;
    return true;
  }

  bool application::set_interval(double interval_msec)
  {
    cast_app(_obj)->fps = 1000 / interval_msec;
    return true;
  }

  bool application::set_name(const std::string &name)
  {
    wxGetApp().SetAppName(wxString(name.c_str(), wxConvUTF8));
    return true;
  }

  bool application::set_tick(luabind::object func)
  {
    tick = func;
    return true;
  }

  void application::set_top(frame *top)
  {
    frame::set_top(top);
  }

  bool application::sleep(int delay_in_msec)
  {
    wxStopWatch sw;
    sw.Start();
    if (delay_in_msec < 0) { return false; }
    do { yield(); } while (sw.Time() < delay_in_msec);
    return true;
  }

  bool application::track_key(const char *keystr)
  {
    return cast_app(_obj)->rec.track_key(keystr);
  }

  bool application::track_mouse()
  {
    return cast_app(_obj)->rec.track_mouse();
  }

  bool application::wait()
  {
    double &offset  = cast_app(_obj)->offset;
    wxStopWatch &sw = cast_app(_obj)->sw;
    inrecord &in = wxGetApp().rec;

    in.clear();
    do {
      in.record();
      yield();
    } while (sw.Time() + offset < get_interval());
    offset = sw.Time() + offset - get_interval();
    offset = offset - (int)offset;
    sw.Start();
    return true;
  }

  bool application::yield()
  {
    return wxGetApp().Yield();
  }


  lua_State* application::getL()
  {
    return (wxGetApp()).L;
  }

  int application::msgbox(const char *msg, const char *caption)
  {
    int result;
    wxString new_msg(msg, wxConvUTF8);
    wxString new_caption(caption, wxConvUTF8);
    result = wxMessageBox(new_msg, new_caption);
    wxGetApp().Yield();
    return result;
  }

  gui_lock::gui_lock()
  {
//    if (wxGetApp().main && wxGetApp().main->IsRunning())
    {
//      printf("ENTER-");
      wxMutexGuiEnter();
//      printf("OK: ");
    }
  }

  gui_lock::~gui_lock()
  {
//    printf(" :LEAVE-");
//    if (wxGetApp().main)
    {
      wxMutexGuiLeave();
//      printf("OK\n");
    }
//    else { printf("\n"); }
  }

}

IMPLEMENT_APP_NO_MAIN(lev::myApp);

