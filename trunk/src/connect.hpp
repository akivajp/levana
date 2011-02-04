#ifndef _CONNECT_HPP
#define _CONNECT_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/connect.hpp
// Purpose:     inner use header for event call back handling
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/17/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "lev/event.hpp"
#include "wx/wx.h"
#include <luabind/luabind.hpp>
#include <map>


#define DECLARE_CONNECT() \
    public:\
      inline void Connect(int id, wxEventType eventType, luabind::object lua_func)\
      {\
        lev::Connect(this, id, eventType, lua_func);\
      }\
      void ProcEvent(wxEvent &event)\
      {\
        lev::ProcEvent(this, event);\
      }\
      std::map<int, std::map<int, luabind::object> > _fmap;


namespace lev
{
  template <typename T>
  inline void ProcEvent(T *handler, wxEvent &evt)
  {
    event e(&evt);
    std::map<int, luabind::object> id_map;
    std::map<int, luabind::object>::iterator i;
    id_map = handler->_fmap[evt.GetEventType()];
    i = id_map.find(-1);
    if (i != id_map.end() && luabind::type(i->second) == LUA_TFUNCTION)
    {
      i->second(e);
      return;
    }
    i = id_map.find(evt.GetId());
    if (i != id_map.end() && luabind::type(i->second) == LUA_TFUNCTION)
    {
      i->second(e);
      return;
    }
  }

  template <typename T>
  inline void Connect(T *handler, int id, wxEventType eventType, luabind::object lua_func)
  {
    handler->_fmap[eventType][id] = lua_func;
    ((wxEvtHandler *)handler)->Connect(id, eventType, (wxObjectEventFunction)&T::ProcEvent);
  }
}

#endif // _CONNECT_HPP

