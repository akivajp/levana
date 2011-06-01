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
#include "lev/sound.hpp"

#include <lua.h>
#include <string>

int lua_main(int argc, char **argv);

namespace lev
{
  class myApp : public wxApp
  {
    public:
      inline myApp() {}
      void set_argv();
      void del_argv();
      int AutoLoop();
      virtual bool OnInit(void);
      virtual int OnExit(void);
      bool Yield();
//      inline wxEventLoop *GetEventLoop() { return m_mainLoop; }
      static lua_State *L;
    private:
      char **argv_utf8;
      // Common Connect Interface
      DECLARE_CONNECT();
  };
  lua_State* myApp::L = NULL;


  void myApp::set_argv()
  {
    try
    {
      argv_utf8 = new char*[this->argc];
      for (int i = 0; i < this->argc; i++)
      {
        wxString str(this->argv[i]);
        int len = strlen(str.mb_str(wxConvUTF8));
        argv_utf8[i] = new char[len + 1];
        strcpy(argv_utf8[i], str.mb_str(wxConvUTF8));
      }
    }
    catch(...)
    {
      throw "app: initialization failed";
      exit(-1);
    }
  }

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
    m_mainLoop = new wxEventLoop();
    wxEventLoop::SetActive(m_mainLoop);
    wxInitAllImageHandlers();
    wxFileSystem::AddHandler(new wxInternetFSHandler);
    return true;
  }

  int myApp::OnExit(void)
  {
    del_argv();
  }

  bool myApp::Yield()
  {
    if (!this->Pending()) { this->ProcessIdle(); }
//    while(!this->Pending()) { this->ProcessIdle(); }
    return wxTheApp->Yield(true);
  }
}

DECLARE_APP(lev::myApp);

namespace lev
{

  application::application()
  {
    wxGetApp().OnInit();
    setname("Levana Application");
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
    myApp::L = L;
    entried = true;
    return true;
  }

  bool application::get_keystate(const char *key)
  {
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

  void application::setname(const char *name)
  {
    wxGetApp().SetAppName(wxString(name, wxConvUTF8));
  }

  bool application::set_onany(luabind::object lua_func)
  {
    wxGetApp().Connect(-1, -1, lua_func);
    return true;
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
    if (delay_in_msec < 0) { return false; }
    wxStopWatch sw;
    sw.Start();
    while (sw.Time() < delay_in_msec) {}
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
    return myApp::L;
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

}

IMPLEMENT_APP_NO_MAIN(lev::myApp);

