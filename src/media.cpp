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
#include "lev/util.hpp"

#include <string>

class wxMediaCtrl;

namespace lev
{

  player::player() : control() {}

  player::~player() {}

  player* player::create(control *parent, int width, int height)
  {
    player *new_player = NULL;
    wxMediaCtrl *media = NULL;
    try {
      new_player = new player();
      wxWindow *p = NULL;
      if (parent) { p = (wxWindow *)parent->get_rawobj(); }
      media = new wxMediaCtrl(p, -1, wxEmptyString, wxDefaultPosition, wxSize(width, height));
    }
    catch (...) {
      delete media;
      delete new_player;
      return NULL;
    }
    new_player->_obj = media;
    new_player->_id = media->GetId();
    new_player->system_managed = true;
    return new_player;
  }

  int player::create_l(lua_State *L)
  {
    using namespace luabind;
    control* p = NULL;
    int w = -1, h = -1;

    int n = lua_gettop(L);
    lua_pushcfunction(L, &util::merge);
    newtable(L).push(L);
    for (int i = 1; i <= n; i++)
    {
      object(from_stack(L, i)).push(L);
    }
    lua_call(L, n + 1, 1);
    object t(from_stack(L, -1));
    lua_remove(L, -1);

    if (t["parent"]) { p = object_cast<control *>(t["parent"]); }
    else if (t["p"]) { p = object_cast<control *>(t["p"]); }
    else if (t["udata"]) { p = object_cast<control *>(t["udata"]); }

    if (t["width"]) { w = object_cast<int>(t["width"]); }
    else if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["num1"]) { w = object_cast<int>(t["num1"]); }

    if (t["height"]) { h = object_cast<int>(t["height"]); }
    else if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["num2"]) { h = object_cast<int>(t["num2"]); }

    object func = globals(L)["lev"]["classes"]["player"]["create_c"];
    object new_player = func(p, w, h);
    if (new_player)
    {
      // registration
    }
    new_player.push(L);
    return 1;
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
    wxString cwd = wxGetCwd();
    wxString path = cwd + wxT("/") + wxString(filename, wxConvUTF8);
    return ((wxMediaCtrl *)_obj)->Load(path);
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

