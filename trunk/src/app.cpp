/////////////////////////////////////////////////////////////////////////////
// Name:        src/app.hpp
// Purpose:     source for application managing class
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/01/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/app.hpp"
#include "levana/frame.hpp"

#include <string>

int lua_main(int argc, char **argv);

namespace levana
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
//      virtual int MainLoop(void);
      bool Yield();
      inline wxEventLoop *GetEventLoop() { return m_mainLoop; }
    private:
      char **argv_utf8;
  };

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
      printf("Initialization failed\n");
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

  int myApp::AutoLoop()
  {
    while(this->GetTopWindow())
    {
      this->Yield();
    }
    return 0;
  }

  bool myApp::OnInit(void)
  {
    set_argv();
    m_mainLoop = new wxEventLoop();
    wxEventLoop::SetActive(m_mainLoop);
    return true;
  }

  int myApp::OnExit(void)
  {
    del_argv();
  }

//  int myApp::MainLoop()
//  {
//    while(GetTopWindow())
//    {
//      this->Yield();
//    }
//    return 0;
//  }
  
  bool myApp::Yield()
  {
    while(!this->Pending()) { this->ProcessIdle(); }
    return wxTheApp->Yield(true);
  }
}

DECLARE_APP(levana::myApp);

namespace levana
{
  int app::autoloop()
  {
    return wxGetApp().AutoLoop();
  }

  void app::cleanup()
  {
    wxGetApp().OnExit();
  }

  bool app::entry(int argc, char **argv)
  {
    static bool entried = false;
    if (entried) { return false; }
    if (!wxEntryStart(argc, argv)) { return false; }
    app::init();
    return true;
  }

  const char *app::get_name()
  {
    const std::string name =
      (const char *)wxGetApp().GetAppName().mb_str(wxConvUTF8);
    return name.c_str();
  }

  frame *app::get_top()
  {
    return frame::get_top();
  }

  bool app::init()
  {
    return wxGetApp().OnInit();
  }

  int app::msgbox(const char *msg, const char *caption)
  {
    wxString new_msg(msg, wxConvUTF8);
    wxString new_caption(caption, wxConvUTF8);
    return wxMessageBox(new_msg, new_caption);
  }

  bool app::yield(bool onlyIfNeeded)
  {
    wxGetApp().Yield();
  }

  void app::set_name(const char *name)
  {
    wxGetApp().SetAppName(wxString(name, wxConvUTF8));
  }

  void app::set_top(frame *top)
  {
    frame::set_top(top);
  }

}

IMPLEMENT_APP_NO_MAIN(levana::myApp);

