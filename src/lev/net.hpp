#ifndef _NET_HPP
#define _NET_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/net.hpp
// Purpose:     header for network handling classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     06/14/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include <lua.h>
#include <luabind/luabind.hpp>

extern "C" {
  int luaopen_lev_net(lua_State *L);
}

namespace lev
{

  class socket : public base
  {
    protected:
      socket();
    public:
      ~socket();
      bool close();
      static socket *connect(const char *addr, const char *service);
      const char *get_address();
      int get_port();
      virtual type_id get_type_id() const { return LEV_TSOCKET; }
      virtual const char *get_type_name() const { return "lev.net.socket"; }
      bool is_open();
      bool read_all(std::string &str);
      static int read_all_l(lua_State *L);
      bool read_line(std::string &str);
      static int read_line_l(lua_State *L);
      const char *read_until(char delimiter);
      const char *receive(int max_length);
      bool send(const char *data);
      bool send_str(std::string data) { return send(data.c_str()); }

      friend class acceptor;
    protected:
      void *_obj;
  };


  class acceptor : public base
  {
    protected:
      acceptor();
    public:
      ~acceptor();
      socket* accept(int timeout_seconds = 10);
      static acceptor* bind(const char *addr, luabind::object port);
      const char *get_address();
      int get_port();
      virtual type_id get_type_id() const { return LEV_TACCEPTOR; }
      virtual const char *get_type_name() const { return "lev.net.acceptor"; }
    protected:
      void *_obj;
  };

  class http : public base
  {
    protected:
      http() { }
    public:
      ~http() { }
      static bool request(std::string url, std::string *head, std::string *body);
      static int request_l(lua_State *L);
  };


  class net : public base
  {
    protected:
      net() { }
    public:
      ~net() { }
  };

}

#endif // _NET_HPP

