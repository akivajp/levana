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
      inline wxEventLoop *GetEventLoop() { return m_mainLoop; }
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
    while(!this->Pending()) { this->ProcessIdle(); }
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

  const char *application::getname()
  {
    const std::string name =
      (const char *)wxGetApp().GetAppName().mb_str(wxConvUTF8);
    return name.c_str();
  }

  frame *application::gettop()
  {
    return frame::gettop();
  }

  void application::setname(const char *name)
  {
    wxGetApp().SetAppName(wxString(name, wxConvUTF8));
  }

  void application::setonkeydown(luabind::object lua_func)
  {
    wxGetApp().Connect(-1, wxEVT_KEY_DOWN, lua_func);
  }

  void application::settop(frame *top)
  {
    frame::settop(top);
  }

  void application::wait(int delay_in_msec)
  {
    wxSleep(delay_in_msec);
  }

  bool application::yield()
  {
    return wxGetApp().Yield();
  }


  // static methods
  bool application::entry(lua_State *L, int argc, char **argv)
  {
    static bool entried = false;
    if (entried) { return false; }
    if (!wxEntryStart(argc, argv)) { return false; }
    myApp::L = L;
    entried = true;
    return true;
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

