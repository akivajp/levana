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
#include "levana/media.hpp"
#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

class wxMediaCtrl;

namespace levana
{

  player::player(control *parent, int id, const char *filepath, int width, int height) : control()
  {
    try {
      wxWindow *p = NULL;
      wxString file;
      if (parent) { p = (wxWindow *)parent->_obj; }
      if (filepath == NULL) { file = wxEmptyString; }
      else { file = wxString(filepath, wxConvUTF8); }
      _obj = new wxMediaCtrl(p, id, file, wxDefaultPosition, wxSize(width, height));
    }
    catch (...) {
      throw "control: allocation error";
    }
  }

  player::~player()
  {
    if (_obj)
    {
      delete (wxMediaCtrl *)_obj;
    }
  }

  size player::getbestsize()
  {
    wxSize sz = ((wxMediaCtrl *)_obj)->GetBestSize();
    return size(sz.GetWidth(), sz.GetHeight());
  }

  double player::getvolume()
  {
    return ((wxMediaCtrl *)_obj)->GetVolume();
  }

  bool player::ispaused()
  {
    if ( ((wxMediaCtrl *)_obj)->GetState() == wxMEDIASTATE_PAUSED )
    { return true; }
    return false;
  }

  bool player::isplaying()
  {
    if ( ((wxMediaCtrl *)_obj)->GetState() == wxMEDIASTATE_PLAYING )
    { return true; }
    return false;
  }

  bool player::isstopped()
  {
    if ( ((wxMediaCtrl *)_obj)->GetState() == wxMEDIASTATE_STOPPED )
    { return true; }
    return false;
  }

  bool player::loadlocal(const char *filename)
  {
    namespace fs = boost::filesystem;
    fs::path path = fs::current_path();
    std::string full = path.string().c_str();
    full = full + "/" + filename;
    return ((wxMediaCtrl *)_obj)->Load(wxString(full.c_str(), wxConvUTF8));
  }

  bool player::loaduri(const char *uri)
  {
    return ((wxMediaCtrl *)_obj)->LoadURI(wxString(uri, wxConvUTF8));
  }

  bool player::pause()
  {
    return ((wxMediaCtrl *)_obj)->Pause();
  }

  bool player::play()
  {
    return ((wxMediaCtrl *)_obj)->Play();
  }

  bool player::setvolume(double vol)
  {
    return ((wxMediaCtrl *)_obj)->SetVolume(vol);
  }

  bool player::stop()
  {
    return ((wxMediaCtrl *)_obj)->Stop();
  }
}

