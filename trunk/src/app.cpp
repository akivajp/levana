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
    wxInitAllImageHandlers();
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

DECLARE_APP(levana::myApp);

namespace levana
{
  appli::appli()
  {
    wxGetApp().OnInit();
    setname("Levana Application");
  }

  appli::~appli()
  {
    wxGetApp().OnExit();
  }

  int appli::autoloop()
  {
    return wxGetApp().AutoLoop();
//    return wxGetApp().OnRun();
  }


  int appli::msgbox(const char *msg, const char *caption)
  {
    int result;
    wxString new_msg(msg, wxConvUTF8);
    wxString new_caption(caption, wxConvUTF8);
    result = wxMessageBox(new_msg, new_caption);
    this->yield();
    return result;
  }

  bool appli::yield()
  {
    return wxGetApp().Yield();
  }


  // name property base
  const char *appli::getname()
  {
    const std::string name =
      (const char *)wxGetApp().GetAppName().mb_str(wxConvUTF8);
    return name.c_str();
  }
  void appli::setname(const char *name)
  {
    wxGetApp().SetAppName(wxString(name, wxConvUTF8));
  }


  // top window property base
  frame *appli::gettop()
  {
    return frame::gettop();
  }
  void appli::settop(frame *top)
  {
    frame::settop(top);
  }

  // static method
  bool appli::entry(int argc, char **argv)
  {
    static bool entried = false;
    if (entried) { return false; }
    if (!wxEntryStart(argc, argv)) { return false; }
    entried = true;
    return true;
  }

}

IMPLEMENT_APP_NO_MAIN(levana::myApp);

