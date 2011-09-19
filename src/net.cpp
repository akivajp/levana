/////////////////////////////////////////////////////////////////////////////
// Name:        src/net.cpp
// Purpose:     source for network handling classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     06/14/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "register.hpp"
#include "lev/net.hpp"
#include "lev/util.hpp"

#include <luabind/luabind.hpp>
#include <sstream>
#include <string>
#include <wx/sstream.h>
#include <wx/url.h>

int luaopen_lev_net(lua_State *L)
{
  using namespace luabind;
  using namespace lev;

  open(L);
  globals(L)["require"]("lev");

  module(L, "lev")
  [
    namespace_("net")
    [
      def("bind", &acceptor::bind, adopt(result)),
      def("connect_c", &connector::connect, adopt(result)),
      class_<net_base, base>("net_base")
        .property("address", &net_base::get_address)
        .property("addr", &net_base::get_address)
        .property("hostname", &net_base::get_hostname)
        .property("host", &net_base::get_hostname)
        .property("ip_v4", &net_base::get_address)
        .property("ip", &net_base::get_address)
        .property("port", &net_base::get_port),
      class_<address, net_base>("address")
        .scope
        [
          def("any", &address::any, adopt(result)),
          def("resolve", &address::resolve, adopt(result))
        ],
      class_<lev::socket, net_base>("socket")
        .def("close", &socket::close)
        .def("receive", &socket::receive)
        .def("recv_msg", &socket::recv_msg)
        .def("send", &socket::send)
        .def("send_msg", &socket::send_msg)
        .property("is_connected", &socket::is_connected),
      class_<connector, lev::socket>("connector")
        .def("wait_on_connect", &connector::wait_on_connect),
      class_<acceptor, net_base>("acceptor")
        .def("accept", &acceptor::accept, adopt(result))
        .def("accept", &acceptor::accept_wait, adopt(result))
        .def("wait_for_accept", &acceptor::wait_for_accept),
      class_<url, net_base>("url")
        .property("content_type", &url::get_content_type)
        .property("data", &url::get_data)
        .property("response", &url::get_response)
        .property("scheme", &url::get_scheme)
        .property("size", &url::get_size)
        .property("server", &url::get_server)
        .scope
        [
          def("connect", &url::connect, adopt(result))
        ]
    ]
  ];
  register_to(globals(L)["lev"]["net"], "connect", &connector::connect_l);

  globals(L)["package"]["loaded"]["lev.net"] = globals(L)["lev"]["net"];
  return 0;
}

namespace lev
{
  inline static wxIPV4address *cast_addr(void *obj) { return (wxIPV4address *)obj; }

  address::address() : net_base() { }

  address::~address()
  {
    if (_obj) { delete cast_addr(_obj); }
  }

  address* address::any()
  {
    address *addr = NULL;
    wxIPV4address *obj;
    try {
      addr = new address;
      obj = new wxIPV4address;
      obj->Service(0);
      addr->_obj = obj;
      return addr;
    }
    catch (...) {
      delete addr;
      return NULL;
    }
  }

  const char *address::get_address()
  {
    std::string addr = (const char *)cast_addr(_obj)->IPAddress().mb_str(wxConvUTF8);
    return addr.c_str();
  }

  const char *address::get_hostname()
  {
    std::string host = (const char *)cast_addr(_obj)->Hostname().mb_str(wxConvUTF8);
    return host.c_str();
  }

  address* address::resolve(const char *host)
  {
    address *addr = NULL;
    try {
      addr = address::any();
      if (addr == NULL) { throw -1; }
      if (not cast_addr(addr->_obj)->Hostname(wxString(host, wxConvUTF8))) { throw -2; }
      return addr;
    }
    catch (...) {
      delete addr;
      return NULL;
    }
  }

  int address::get_port()
  {
    return cast_addr(_obj)->Service();
  }

  class mySocket
  {
    public:
      mySocket() : sock(NULL) { }

      ~mySocket()
      {
        if (sock) { delete sock; }
      }

      static mySocket *Connect(const char *hostname, const char *service, bool wait = true)
      {
        mySocket *sock = NULL;
        wxSocketClient *client = NULL;
        try {
          sock = new mySocket;
          client = new wxSocketClient;
          wxIPV4address addr;
          if (not addr.Hostname(wxString(hostname, wxConvUTF8))) { return false; }
          if (not addr.Service(wxString(service, wxConvUTF8))) { return false; }
          client->Connect(addr, wait);
          sock->sock = client;
          return sock;
        }
        catch (...) {
          delete sock;
          return NULL;
        }
      }

      const char *GetAddress()
      {
        wxIPV4address addr;
        if (not sock->GetPeer(addr)) { return NULL; }
        std::string ip = (const char *)addr.IPAddress().mb_str(wxConvUTF8);
        return ip.c_str();
      }

      const char *GetHost()
      {
        wxIPV4address addr;
        if (not sock->GetPeer(addr)) { return NULL; }
        std::string host = (const char *)addr.Hostname().mb_str(wxConvUTF8);
        return host.c_str();
      }

      int GetPort()
      {
        wxIPV4address addr;
        if (not sock->GetPeer(addr)) { return -1; }
        return addr.Service();
      }

      bool ReadAll(std::string &str)
      {
        str = "";
        const int buf_size = 4096;
        char *buf = NULL;
        int count = 0;
        try {
          buf = new char[buf_size + 1];
          for ( ; ; )
          {
            count = sock->Peek(buf, buf_size).LastCount();
            if (count == 0)
            {
              delete[] buf;
              if (str.length() == 0) { return false; }
              else { return true; }
            }
            str.append(buf, buf + count);
            sock->Read(buf, count);
          }
        }
        catch (...) {
          delete buf;
          return false;
        }
      }

      bool ReadLine(std::string &str)
      {
        str = "";
        const int buf_size = 4096;
        char *buf = NULL;
        char *pos = NULL;
        int count = 0;
        try {
          buf = new char[buf_size + 1];
          for ( ; ; )
          {
            count = sock->Peek(buf, buf_size).LastCount();
            if (count == 0)
            {
              delete[] buf;
              if (str.length() == 0) { return false; }
              else { return true; }
            }
            buf[count] = '\0';
            pos = strstr(buf, "\r\n");
            if (pos)
            {
              str.append(buf, pos);
              sock->Read(buf, (pos - buf) + 2);
              delete [] buf;
              return true;
            }
            pos = strstr(buf, "\n");
            if (pos)
            {
              str.append(buf, pos);
              sock->Read(buf, (pos - buf) + 1);
              delete [] buf;
              return true;
            }
            str.append(buf, buf + count);
            sock->Read(buf, count);
          }
        }
        catch (...) {
          delete [] buf;
          return false;
        }
      }

      const char *Receive(int max_length)
      {
//        asio::streambuf buf;
//        asio::streambuf::mutable_buffers_type bufs = buf.prepare(max_length);
//        int n = socket->receive(bufs);
//        buf.commit(n);
//        return asio::buffer_cast<const char *>(buf.data());
      }

      wxSocketBase *sock;
  };
  inline static mySocket *cast_sock(void *obj) { return (mySocket *)obj; }

  socket::socket() : net_base() { }

  socket::~socket()
  {
    if (_obj) { delete cast_sock(_obj); }
  }

  bool socket::close()
  {
    return cast_sock(_obj)->sock->Close();
  }

  const char *socket::get_address()
  {
    return cast_sock(_obj)->GetAddress();
  }

  const char *socket::get_hostname()
  {
    return cast_sock(_obj)->GetHost();
  }

  int socket::get_port()
  {
    return cast_sock(_obj)->GetPort();
  }

  bool socket::is_connected()
  {
    return cast_sock(_obj)->sock->IsConnected();
  }

  bool socket::read_all(std::string &str)
  {
    return cast_sock(_obj)->ReadAll(str);
  }

  int socket::read_all_l(lua_State *L)
  {
    using namespace luabind;
    std::string str;

    luaL_checktype(L, 1, LUA_TUSERDATA);
    object o(from_stack(L, 1));
    lev::socket *sock = object_cast<lev::socket *>(o);

    bool result = sock->read_all(str);
    if (not result)
    {
      lua_pushnil(L);
      return 1;
    }
    lua_pushstring(L, str.c_str());
    return 1;
  }

  bool socket::read_line(std::string &str)
  {
    return cast_sock(_obj)->ReadLine(str);
  }

  int socket::read_line_l(lua_State *L)
  {
    using namespace luabind;
    std::string str;

    luaL_checktype(L, 1, LUA_TUSERDATA);
    object o(from_stack(L, 1));
    lev::socket *sock = object_cast<lev::socket *>(o);

    bool result = sock->read_line(str);
    if (not result) { return 0; }
    lua_pushstring(L, str.c_str());
    return 1;
  }

  const char *socket::receive(int max_length)
  {
    return cast_sock(_obj)->Receive(max_length);
  }

  luabind::object socket::recv_msg(luabind::object max_length)
  {
    using namespace luabind;
    unsigned char *buf = NULL;
    try {
      if (type(max_length) != LUA_TNUMBER) { throw -1; }
      int max = object_cast<int>(max_length);
      if (max <= 0) { throw -2; }
      buf = new unsigned char[max];
      cast_sock(_obj)->sock->ReadMsg(buf, max);
      std::string buf_str(buf, buf + cast_sock(_obj)->sock->LastCount());
      delete [] buf;
      return object(max_length.interpreter(), buf_str);
    }
    catch (...) {
      delete [] buf;
      return luabind::object();
    }
  }

  int socket::send(const std::string &data)
  {
    cast_sock(_obj)->sock->Write(data.data(), data.length());
    return cast_sock(_obj)->sock->LastCount();
  }

  bool socket::send_msg(const std::string &msg)
  {
    cast_sock(_obj)->sock->WriteMsg(msg.c_str(), msg.length());
    if (cast_sock(_obj)->sock->LastCount() == msg.length()) { return true; }
    else { return false; }
  }

  connector* connector::connect(const char *addr, const char *service, bool wait)
  {
    connector *client = NULL;
    mySocket *obj = NULL;
    try {
      client = new connector;
      obj = mySocket::Connect(addr, service, wait);
      if (obj == NULL) { throw -1; }
      client->_obj = obj;
      return client;
    }
    catch (...) {
      delete client;
      return NULL;
    }
  }

  int connector::connect_l(lua_State *L)
  {
    using namespace luabind;
    const char *host = NULL;
    std::ostringstream os;
    bool wait = true;

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

    if (t["hostname"]) { host = object_cast<const char *>(t["hostname"]); }
    if (t["host"]) { host = object_cast<const char *>(t["host"]); }
    else if (t["address"]) { host = object_cast<const char *>(t["address"]); }
    else if (t["addr"]) { host = object_cast<const char *>(t["addr"]); }
    else if (t["lua.string1"]) { host = object_cast<const char *>(t["lua.string1"]); }

    if (t["service"]) { os << t["service"]; }
    else if (t["serv"]) { os << t["serv"]; }
    else if (t["port"]) { os << t["port"]; }
    else if (t["lua.string2"]) { os << t["lua.string2"]; }
    else if (t["lua.number1"]) { os << t["lua.number1"]; }

    if (t["wait"]) { wait = object_cast<bool>(t["wait"]); }
    else if (t["w"]) { wait = object_cast<bool>(t["w"]); }
    else if (t["lua.boolean1"]) { wait = object_cast<bool>(t["lua.boolean1"]); }

    if (host == NULL) { return 0; }
    if (os.str() == "") { return 0; }
    object func = globals(L)["lev"]["net"]["connect_c"];
    object client = func(host, os.str().c_str(), wait);
    if (client)
    {
      if (not client["read_all"])
      {
        register_to(client, "read_all", &socket::read_all_l);
        register_to(client, "read_line", &socket::read_line_l);
      }
    }
    client.push(L);
    return 1;
  }

  bool connector::wait_on_connect(int wait_msec)
  {
    if (wait_msec == -1)
    {
      return ((wxSocketClient *)_obj)->WaitOnConnect(-1, 0);
    }
    else
    {
      return ((wxSocketClient *)_obj)->WaitOnConnect(0, wait_msec);
    }
  }

  class myAcceptor
  {
    public:
      myAcceptor() : acc(NULL) { }

      ~myAcceptor()
      {
        if (acc) { delete acc; }
      }

      mySocket* Accept(bool wait = true)
      {
        mySocket *sock = NULL;
        try {
          sock = new mySocket;
          sock->sock = acc->Accept(wait);
          if (sock->sock == NULL) { throw -1; }
          return sock;
        }
        catch (...) {
          delete sock;
          return NULL;
        }
      }

      bool Bind(const char *hostname, const char *port)
      {
        if (acc) { delete acc; }
        acc = NULL;
        try {
          wxIPV4address addr;
          if (strcmp(hostname, "*") == 0)
          {
            if (not addr.AnyAddress()) { throw -1; }
          }
          else
          {
            if (not addr.Hostname(wxString(hostname, wxConvUTF8))) { throw -2; }
          }
          if (not addr.Service(wxString(port, wxConvUTF8))) { throw -3; }
          acc = new wxSocketServer(addr);
          if (! acc->Ok()) { throw -4; }
          return true;
        }
        catch (...) {
          delete acc;
          acc = NULL;
          return false;
        }
      }

      const char *GetAddress()
      {
        wxIPV4address addr;
        if (not acc->GetLocal(addr)) { return NULL; }
        std::string ip = (const char *)addr.IPAddress().mb_str(wxConvUTF8);
        return ip.c_str();
      }

      const char *GetHost()
      {
        wxIPV4address addr;
        if (not acc->GetLocal(addr)) { return NULL; }
        std::string host = (const char *)addr.Hostname().mb_str(wxConvUTF8);
        return host.c_str();
      }

      int GetPort()
      {
        wxIPV4address addr;
        if (not acc->GetLocal(addr)) { return -1; }
        return addr.Service();
      }

      wxSocketServer *acc;
  };
  inline static myAcceptor* cast_acc(void *obj) { return (myAcceptor *)obj; }

  acceptor::acceptor() : net_base() { }

  acceptor::~acceptor()
  {
    if (_obj) { delete cast_acc(_obj); }
  }

  socket* acceptor::accept(bool wait)
  {
    socket *sock = NULL;
    mySocket *obj = NULL;
    try {
      sock = new socket;
      obj = cast_acc(_obj)->Accept(wait);
      if (obj == NULL) { throw -1; }
      sock->_obj = obj;
      return sock;
    }
    catch (...) {
      delete sock;
      return NULL;
    }
  }

  acceptor* acceptor::bind(const char *hostname, luabind::object port)
  {
    acceptor *acc = NULL;
    myAcceptor *obj = NULL;
    std::ostringstream os;
    try {
      acc = new acceptor;
      os << port;
      obj = new myAcceptor;
      if (not obj->Bind(hostname, os.str().c_str())) { throw -1; }
      acc->_obj = obj;
      return acc;
    }
    catch (...) {
      delete acc;
      return NULL;
    }
  }

  const char *acceptor::get_address()
  {
    return cast_acc(_obj)->GetAddress();
  }

  const char *acceptor::get_hostname()
  {
    return cast_acc(_obj)->GetHost();
  }

  int acceptor::get_port()
  {
    return cast_acc(_obj)->GetPort();
  }

  bool acceptor::wait_for_accept(int wait_msec)
  {
    if (wait_msec == -1)
    {
      return cast_acc(_obj)->acc->WaitForAccept(-1, 0);
    }
    else
    {
      return cast_acc(_obj)->acc->WaitForAccept(0, wait_msec);
    }
  }

  class myURL
  {
    public:
      myURL() : url(NULL), data(NULL) { }
      ~myURL()
      {
        if (data) { delete data; }
        if (url) { delete url; }
      }

      bool Connect(const char *str_url)
      {
        if (data) { delete data; }
        if (url) { delete url; }
        url = NULL;
        data = NULL;
        try {
          url = new wxURL(wxString(str_url, wxConvUTF8));
          if (url->GetError() != wxURL_NOERR) { throw -1; }
          data = url->GetInputStream();
          if (data == NULL) { throw -1; }
          return true;
        }
        catch (...) {
          delete url;
          url = NULL;
          delete data;
          data = NULL;
          return false;
        }
      }

      wxURL *url;
      wxInputStream *data;
  };
  inline static myURL* cast_url(void *obj) { return (myURL *)obj; }

  url::url() : net_base() { }

  url::~url()
  {
    if (_obj) { delete cast_url(_obj); }
  }

  url* url::connect(const char *str_url)
  {
    url *u = NULL;
    myURL *obj = NULL;
    try {
      url *u = new url;
      u->_obj = obj = new myURL;
      if (not obj->Connect(str_url)) { throw -1; }
      return u;
    }
    catch (...) {
      delete u;
      return NULL;
    }
  };

  const char *url::get_address()
  {
    wxIPV4address peer;
    cast_url(_obj)->url->GetProtocol().GetPeer(peer);
    std::string str = (const char *)peer.IPAddress().mb_str(wxConvUTF8);
    return str.c_str();
  }

  const char *url::get_hostname()
  {
    wxIPV4address peer;
    cast_url(_obj)->url->GetProtocol().GetPeer(peer);
    std::string str = (const char *)peer.Hostname().mb_str(wxConvUTF8);
    return str.c_str();
  }

  int url::get_port()
  {
    wxIPV4address peer;
    cast_url(_obj)->url->GetProtocol().GetPeer(peer);
    return peer.Service();
  }

  const char *url::get_content_type()
  {
    std::string str =
      (const char *)cast_url(_obj)->url->GetProtocol().GetContentType().mb_str(wxConvUTF8);
    return str.c_str();
  }

  std::string url::get_data()
  {
    wxStringOutputStream out;
    cast_url(_obj)->data->Read(out);
    std::string str = (const char *)out.GetString().mb_str(wxConvUTF8);
    return str;
  }

  int url::get_response()
  {
    wxProtocol &protocol = cast_url(_obj)->url->GetProtocol();
    wxHTTP *http = dynamic_cast<wxHTTP *>(&protocol);
    if (http == NULL) { return -1; }
    else { return http->GetResponse(); }
  }

  const char *url::get_scheme()
  {
    std::string str = (const char *)cast_url(_obj)->url->GetScheme().mb_str(wxConvUTF8);
    return str.c_str();
  }

  const char *url::get_server()
  {
    std::string str = (const char *)cast_url(_obj)->url->GetServer().mb_str(wxConvUTF8);
    return str.c_str();
  }

  long url::get_size()
  {
    return cast_url(_obj)->data->GetSize();
  }

}

