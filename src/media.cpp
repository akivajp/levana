/////////////////////////////////////////////////////////////////////////////
// Name:        src/media.cpp
// Purpose:     source for media controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     01/05/2011
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/media.hpp"
#include <string>
//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/operations.hpp>

class wxMediaCtrl;

namespace lev
{

  player::player(control *parent, int width, int height) : control()
  {
    try {
      wxWindow *p = NULL;
      if (parent) { p = (wxWindow *)parent->_obj.get(); }
      wxMediaCtrl *media = new wxMediaCtrl(p, -1, wxEmptyString, wxDefaultPosition, wxSize(width, height));
      _id = media->GetId();
      _obj.reset(media);
    }
    catch (...) {
      fprintf(stderr, "control: allocation error");
    }
  }


  size player::getbestsize()
  {
    wxSize sz = ((wxMediaCtrl *)_obj.get())->GetBestSize();
    return size(sz.GetWidth(), sz.GetHeight());
  }

  double player::getvolume()
  {
    return ((wxMediaCtrl *)_obj.get())->GetVolume();
  }

  bool player::ispaused()
  {
    if ( ((wxMediaCtrl *)_obj.get() )->GetState() == wxMEDIASTATE_PAUSED )
    { return true; }
    return false;
  }

  bool player::isplaying()
  {
    if ( ((wxMediaCtrl *)_obj.get() )->GetState() == wxMEDIASTATE_PLAYING )
    { return true; }
    return false;
  }

  bool player::isstopped()
  {
    if ( ((wxMediaCtrl *)_obj.get() )->GetState() == wxMEDIASTATE_STOPPED )
    { return true; }
    return false;
  }

  bool player::loadlocal(const char *filename)
  {
    wxString cwd = wxGetCwd();
    wxString path = cwd + wxT("/") + wxString(filename, wxConvUTF8);
    return ((wxMediaCtrl *)_obj.get())->Load(path);
  }

  bool player::loaduri(const char *uri)
  {
    return ((wxMediaCtrl *)_obj.get())->LoadURI(wxString(uri, wxConvUTF8));
  }

  bool player::pause()
  {
    return ((wxMediaCtrl *)_obj.get())->Pause();
  }

  bool player::play()
  {
    return ((wxMediaCtrl *)_obj.get())->Play();
  }

  bool player::setvolume(double vol)
  {
    return ((wxMediaCtrl *)_obj.get())->SetVolume(vol);
  }

  bool player::stop()
  {
    return ((wxMediaCtrl *)_obj.get())->Stop();
  }
}

