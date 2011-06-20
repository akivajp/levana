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

  class net_base : public base
  {
    protected:
      net_base() : _obj(NULL) { }
    public:
      virtual ~net_base() { }
      virtual const char *get_address() = 0;
      virtual const char *get_hostname() = 0;
      virtual type_id get_type_id() const { return LEV_TNET_BASE; }
      virtual const char *get_type_name() const { return "lev.net.net_base"; }
      virtual int get_port() = 0;
    protected:
      void *_obj;
  };

  class address : public net_base
  {
    protected:
      address();
    public:
      virtual ~address();
      static address *any();
      virtual const char *get_address();
      virtual const char *get_hostname();
      virtual type_id get_type_id() const { return LEV_TADDRESS; }
      virtual const char *get_type_name() const { return "lev.net.address"; }
      static address* resolve(const char *host);
      virtual int get_port();
    protected:
      void *_obj;
  };

  class socket : public net_base
  {
    protected:
      socket();
    public:
      virtual ~socket();
      bool close();
      virtual const char *get_address();
      virtual const char *get_hostname();
      virtual int get_port();
      virtual type_id get_type_id() const { return LEV_TSOCKET; }
      virtual const char *get_type_name() const { return "lev.net.socket"; }
      bool is_connected();
      bool read_all(std::string &str);
      static int read_all_l(lua_State *L);
      bool read_line(std::string &str);
      static int read_line_l(lua_State *L);
      const char *receive(int max_length);
      luabind::object recv_msg(luabind::object max_length);
      int send(const std::string &data);
      bool send_msg(const std::string &msg);
      bool send_str(std::string data) { return send(data.c_str()); }
//      bool write(unsigned char *data, int length);

      friend class acceptor;
  };

  class connector : public socket
  {
    protected:
      connector() : socket() { }
    public:
      virtual ~connector() { }
      static connector *connect(const char *addr, const char *service, bool wait = true);
      static int connect_l(lua_State *L);
      virtual type_id get_type_id() const { return LEV_TCONNECTOR; }
      virtual const char *get_type_name() const { return "lev.net.connector"; }
      bool wait_on_connect(int wait_msec = -1);
  };


  class acceptor : public net_base
  {
    protected:
      acceptor();
    public:
      virtual ~acceptor();
      socket* accept_wait() { return accept(true); }
      socket* accept(bool wait = true);
      static acceptor* bind(const char *hostname, luabind::object port);
      virtual const char *get_address();
      virtual const char *get_hostname();
      virtual int get_port();
      virtual type_id get_type_id() const { return LEV_TACCEPTOR; }
      virtual const char *get_type_name() const { return "lev.net.acceptor"; }
      bool wait_for_accept(int wait_msec = -1);
  };

  class url : public net_base
  {
    protected:
      url();
    public:
      virtual ~url();
      static url* connect(const char *str_url);
      virtual const char *get_address();
      virtual const char *get_hostname();
      const char *get_content_type();
      const char *get_data();
      virtual int get_port();
      const char *get_scheme();
      const char *get_server();
      long get_size();
      virtual type_id get_type_id() const { return LEV_TURL; }
      virtual const char *get_type_name() const { return "lev.net.url"; }
  };

}

#endif // _NET_HPP

