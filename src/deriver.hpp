#ifndef _DERIVE_HPP
#define _DERIVE_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/derive.hpp
// Purpose:     inner use header for event handler class rewrapping template
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     05/07/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "lev/event.hpp"
#include <boost/function.hpp>
#include <luabind/luabind.hpp>

namespace lev
{

  template<typename T> class myHandler : public T
  {
    public:
      myHandler() : T() { }
      virtual ~myHandler() { }

      void Connect(int id, wxEventType eventType, luabind::object lua_func)
      {
        fmap[eventType][id] = lua_func;
        T::Connect(id, eventType, (wxObjectEventFunction)&myHandler<T>::ProcEvent);
      }

      boost::function<void (int, int, luabind::object)> GetConnector()
      {
        return boost::bind(&myHandler<T>::Connect, this, _1, _2, _3);
      }

      luabind::object GetFunc(int id, wxEventType eventType)
      {
        return fmap[eventType][id];
      }

      boost::function<luabind::object (int, int)> GetFuncGetter()
      {
        return boost::bind(&myHandler<T>::GetFunc, this, _1, _2);
      }

      void ProcEvent(wxEvent &evt)
      {
        event e(&evt);

        luabind::object o = fmap[evt.GetEventType()][-1];
        if (o && luabind::type(o) == LUA_TFUNCTION)
        {
          o(e);
          return;
        }

        o = fmap[evt.GetEventType()][evt.GetId()];
        if (o && luabind::type(o) == LUA_TFUNCTION)
        {
          o(e);
          return;
        }

        evt.Skip();
      }

      std::map<int, std::map<int, luabind::object> > fmap;
  };

}

#endif // _DERIVE_HPP

