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
#include "connect.hpp"
#include <lua.h>
#include <string>

namespace lev
{

  class myApp : public wxApp
  {
    public:
      inline myApp() : main(NULL), istate() {}

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

      void del_argv();
      int AutoLoop();

//      int FilterEvent(wxEvent &event)
//      {
//        if (event.GetEventType() == wxEVT_KEY_DOWN)
//        {
//          printf("ANY KEY!\n");
//          return true;
//        }
//        return -1;
//      }

      virtual bool OnInit(void);
      virtual int OnExit(void);
      bool Yield();
      lua_State *L;
      char **argv_utf8;
      class myMainThread* main;
      instate  istate;
      inrecord irecord;
    private:
      // Common Connect Interface
      DECLARE_CONNECT();
  };

  void myApp::del_argv()
  {
    for (int i = 0; i < this->argc; i++)
    {
      delete[] argv_utf8[i];
    }
    delete[] argv_utf8;
    argv_utf8 = NULL;
  }

  bool myApp::OnInit(void)
  {
    set_argv();
//    old_loop = GetMainLoop();
//    loop = new wxEventLoop;
    m_mainLoop = new wxEventLoop();
    wxEventLoop::SetActive(m_mainLoop);
    wxFileSystem::AddHandler(new wxInternetFSHandler);
    return true;
  }

  int myApp::OnExit(void)
  {
    del_argv();
    if (m_mainLoop)
    {
      delete m_mainLoop;
      m_mainLoop = NULL;
    }
  }

  bool myApp::Yield()
  {
    if (!this->Pending()) { this->ProcessIdle(); }
//    while(!this->Pending()) { this->ProcessIdle(); }
//    while (this->Pending() && Dispatch());
    return wxTheApp->Yield(true);
  }
}

DECLARE_APP(lev::myApp);

namespace lev
{

  class myMainThread : public wxThread
  {
    public:
//      myMainThread() {}
//      ~myMainThread() {}
      virtual ExitCode Entry() { return (ExitCode)wxGetApp().OnRun(); }
      virtual void OnExit() { wxGetApp().main = NULL; }
  };

  application::application() : control()
  {
    wxGetApp().OnInit();
    set_name("Levana Application");
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
      while (frm->isvalid())
      {
        wxGetApp().Yield();
      }
    }
  }

  bool application::entry(lua_State *L, int argc, char **argv)
  {
    static bool entried = false;
    if (entried) { return false; }
    if (!wxEntryStart(argc, argv)) { return false; }
//    myApp::L = L;
    entried = true;
    return true;
  }

  bool application::get_keydown(const char *key)
  {
    gui_lock lock;

    if (strstr(key, "CTRL") || strstr(key, "Ctrl") || strstr(key, "ctrl"))
    {
      return wxGetKeyState(WXK_CONTROL);
    }
    if (strstr(key, "SHIFT") || strstr(key, "Shift") || strstr(key, "shift"))
    {
      return wxGetKeyState(WXK_SHIFT);
    }
    if (strstr(key, "ALT") || strstr(key, "Alt") || strstr(key, "alt"))
    {
      return wxGetKeyState(WXK_ALT);
    }
    if (strstr(key, "ESC") || strstr(key, "Esc") || strstr(key, "esc"))
    {
      return wxGetKeyState(WXK_ESCAPE);
    }
    if (strlen(key) == 1)
    {
      return wxGetKeyState(wxKeyCode(key[0]));
    }
    return false;
  }

  inrecord* application::get_inrecord()
  {
    inrecord &in = wxGetApp().irecord;
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

  const char *application::get_name()
  {
    const std::string name =
      (const char *)wxGetApp().GetAppName().mb_str(wxConvUTF8);
    return name.c_str();
  }

  luabind::object application::get_onany()
  {
    return (wxGetApp())._fmap[-1][-1];
  }

  luabind::object application::get_onidle()
  {
    return (wxGetApp())._fmap[wxEVT_IDLE][-1];
  }

  luabind::object application::get_onkeydown()
  {
    return (wxGetApp())._fmap[wxEVT_KEY_DOWN][-1];
  }

  frame *application::get_top()
  {
    return frame::gettop();
  }

  void application::mainloop()
  {
    wxGetApp().MainLoop();
  }


  bool application::run(bool sync)
  {
    if (sync)
    {
      int result = wxGetApp().OnRun();
      return (result == 0);
    }
    else
    {
      myMainThread *main = NULL;
      if (wxGetApp().main) { return false; }
      try {
        myMainThread *main = new myMainThread;
        main->Create();
        main->Run();
        wxGetApp().main = main;
        return true;
      }
      catch (...) {
        delete main;
        return false;
      }
    }
  }

  void application::set_name(const char *name)
  {
    wxGetApp().SetAppName(wxString(name, wxConvUTF8));
  }

  bool application::set_onany(luabind::object lua_func)
  {
    wxGetApp().Connect(-1, -1, lua_func);
    return true;
  }

  bool application::set_onidle(luabind::object lua_func)
  {
    wxGetApp().Connect(-1, wxEVT_IDLE, lua_func);
  }

  bool application::set_onkeydown(luabind::object lua_func)
  {
    wxGetApp().Connect(-1, wxEVT_KEY_DOWN, lua_func);
    return true;
  }

  void application::settop(frame *top)
  {
    frame::settop(top);
  }

  bool application::sleep(int delay_in_msec)
  {
    inrecord &in = wxGetApp().irecord;
    wxStopWatch sw;

    if (delay_in_msec < 0) { return false; }
    sw.Start();
    in.clear();
    do {
      wxMouseState ms = wxGetMouseState();
      if (ms.LeftDown()) { in.set_left_down(); }
      if (ms.MiddleDown()) { in.set_middle_down(); }
      if (ms.RightDown()) { in.set_right_down(); }
      yield();
    } while (sw.Time() < delay_in_msec);
    return true;
  }

  bool application::yield()
  {
    return wxGetApp().Yield();
  }


  application *application::getapp()
  {
    static application app;
    return &app;
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
    if (wxGetApp().main && wxGetApp().main->IsRunning())
    {
//      printf("ENTER-");
      wxMutexGuiEnter();
//      printf("OK: ");
    }
  }

  gui_lock::~gui_lock()
  {
//    printf(" :LEAVE-");
    if (wxGetApp().main)
    {
      wxMutexGuiLeave();
//      printf("OK\n");
    }
//    else { printf("\n"); }
  }

}

IMPLEMENT_APP_NO_MAIN(lev::myApp);

