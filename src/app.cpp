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

    if (strstr(key, "ALT") || strstr(key, "Alt") || strstr(key, "alt"))
    {
      return wxGetKeyState(WXK_ALT);
    }
    if (strstr(key, "BACK") || strstr(key, "Back") ||
        strstr(key, "back") || strstr(key, "BS"))
    {
      return wxGetKeyState(WXK_BACK);
    }
    if (strstr(key, "CANCEL") || strstr(key, "Cancel") || strstr(key, "cancel"))
    {
      return wxGetKeyState(WXK_CANCEL);
    }
    if (strstr(key, "CAPITAL") || strstr(key, "Capital") || strstr(key, "capital"))
    {
      return wxGetKeyState(WXK_CAPITAL);
    }
    if (strstr(key, "CLEAR") || strstr(key, "Clear") || strstr(key, "clear"))
    {
      return wxGetKeyState(WXK_CLEAR);
    }
    if (strstr(key, "CONTROL") || strstr(key, "Control") || strstr(key, "control") || 
        strstr(key, "CTRL")    || strstr(key, "Ctrl")    || strstr(key, "ctrl"))
    {
      return wxGetKeyState(WXK_CONTROL);
    }
    if (strstr(key, "DEL") || strstr(key, "Del") || strstr(key, "del"))
    {
      return wxGetKeyState(WXK_DELETE);
    }
    if (strstr(key, "DOWN") || strstr(key, "Down") || strstr(key, "down"))
    {
      return wxGetKeyState(WXK_DOWN);
    }
    if (strstr(key, "END") || strstr(key, "End") || strstr(key, "end"))
    {
      return wxGetKeyState(WXK_END);
    }
    if (strstr(key, "ESC") || strstr(key, "Esc") || strstr(key, "esc"))
    {
      return wxGetKeyState(WXK_ESCAPE);
    }
    if (strstr(key, "EXE") || strstr(key, "Exe") || strstr(key, "exe"))
    {
      return wxGetKeyState(WXK_EXECUTE);
    }
    if (strstr(key, "F10") || strstr(key, "f10"))
    {
      return wxGetKeyState(WXK_F10);
    }
    if (strstr(key, "F11") || strstr(key, "f11"))
    {
      return wxGetKeyState(WXK_F11);
    }
    if (strstr(key, "F12") || strstr(key, "f12"))
    {
      return wxGetKeyState(WXK_F12);
    }
    if (strstr(key, "F1") || strstr(key, "f1"))
    {
      return wxGetKeyState(WXK_F1);
    }
    if (strstr(key, "F2") || strstr(key, "f2"))
    {
      return wxGetKeyState(WXK_F2);
    }
    if (strstr(key, "F3") || strstr(key, "f3"))
    {
      return wxGetKeyState(WXK_F3);
    }
    if (strstr(key, "F4") || strstr(key, "f4"))
    {
      return wxGetKeyState(WXK_F4);
    }
    if (strstr(key, "F5") || strstr(key, "f5"))
    {
      return wxGetKeyState(WXK_F5);
    }
    if (strstr(key, "F6") || strstr(key, "f6"))
    {
      return wxGetKeyState(WXK_F6);
    }
    if (strstr(key, "F7") || strstr(key, "f7"))
    {
      return wxGetKeyState(WXK_F1);
    }
    if (strstr(key, "F8") || strstr(key, "f8"))
    {
      return wxGetKeyState(WXK_F1);
    }
    if (strstr(key, "F9") || strstr(key, "f9"))
    {
      return wxGetKeyState(WXK_F1);
    }
    if (strstr(key, "HOME") || strstr(key, "Home") || strstr(key, "home"))
    {
      return wxGetKeyState(WXK_HOME);
    }
    if (strstr(key, "INS") || strstr(key, "Ins") || strstr(key, "ins"))
    {
      return wxGetKeyState(WXK_INSERT);
    }
    if (strstr(key, "LB") || strstr(key, "LeftButton"))
    {
      return wxGetKeyState(WXK_LBUTTON);
    }
    if (strstr(key, "LEFT") || strstr(key, "Left") || strstr(key, "left"))
    {
      return wxGetKeyState(WXK_LEFT);
    }
    if (strstr(key, "MB") || strstr(key, "MiddleButton"))
    {
      return wxGetKeyState(WXK_MBUTTON);
    }
    if (strstr(key, "MENU") || strstr(key, "Menu") || strstr(key, "menu"))
    {
      return wxGetKeyState(WXK_MENU);
    }
    if (strstr(key, "PAUSE") || strstr(key, "Pause") || strstr(key, "pause"))
    {
      return wxGetKeyState(WXK_PAUSE);
    }
    if (strstr(key, "PRINT") || strstr(key, "Print") || strstr(key, "print"))
    {
      return wxGetKeyState(WXK_PRINT);
    }
    if (strstr(key, "RB") || strstr(key, "RightButton"))
    {
      return wxGetKeyState(WXK_RBUTTON);
    }
    if (strstr(key, "RIGHT") || strstr(key, "Right") || strstr(key, "right"))
    {
      return wxGetKeyState(WXK_RIGHT);
    }
    if (strstr(key, "RETURN") || strstr(key, "Return") || strstr(key, "return") ||
        strstr(key, "ENTER")  || strstr(key, "Enter")  || strstr(key, "enter"))
    {
      return wxGetKeyState(WXK_RETURN);
    }
    if (strstr(key, "SHIFT") || strstr(key, "Shift") || strstr(key, "shift"))
    {
      return wxGetKeyState(WXK_SHIFT);
    }
    if (strstr(key, "SEL") || strstr(key, "Sel") || strstr(key, "sel"))
    {
      return wxGetKeyState(WXK_SELECT);
    }
    if (strstr(key, "SNAP") || strstr(key, "Snap") || strstr(key, "snap"))
    {
      return wxGetKeyState(WXK_SNAPSHOT);
    }
    if (strstr(key, "SPACE") || strstr(key, "Space") || strstr(key, "space"))
    {
      return wxGetKeyState(WXK_SPACE);
    }
    if (strstr(key, "START") || strstr(key, "Start") || strstr(key, "start"))
    {
      return wxGetKeyState(WXK_START);
    }
    if (strstr(key, "TAB") || strstr(key, "Tab") || strstr(key, "tab"))
    {
      return wxGetKeyState(WXK_TAB);
    }
    if (strstr(key, "UP") || strstr(key, "Up") || strstr(key, "up"))
    {
      return wxGetKeyState(WXK_UP);
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

