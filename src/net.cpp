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

#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <luabind/luabind.hpp>
#include <sstream>
#include <string>

int luaopen_lev_net(lua_State *L)
{
  using namespace luabind;
  using namespace lev;

  open(L);
  globals(L)["require"]("lev");

  module(L, "lev")
  [
    class_<net, base>("net")
      .scope
      [
        def("bind", &acceptor::bind),
        def("connect", &socket::connect),
        class_<acceptor, base>("acceptor")
          .def("accept", &acceptor::accept)
          .property("address", &acceptor::get_address)
          .property("addr", &acceptor::get_address)
          .property("port", &acceptor::get_port),
        class_<http, base>("http"),
        class_<lev::socket, base>("socket")
          .def("close", &socket::close)
//          .def("read_all", &socket::read_all)
//          .def("read_line", &socket::read_line)
          .def("read_until", &socket::read_until)
          .def("receive", &socket::receive)
          .def("send", &socket::send)
          .property("address", &socket::get_address)
          .property("addr", &socket::get_address)
          .property("is_open", &socket::is_open)
          .property("port", &socket::get_port)
      ]
  ];
  register_to(L, globals(L)["lev"]["net"]["http"], "request", &http::request_l);
  register_to(L, globals(L)["lev"]["net"]["socket"], "read_all", &socket::read_all_l);
  register_to(L, globals(L)["lev"]["net"]["socket"], "read_line", &socket::read_line_l);

  return 0;
}

namespace lev
{
  namespace asio = boost::asio;
  typedef boost::asio::ip::tcp tcp;

  class mySocket
  {
    public:
      mySocket() : ios(NULL), socket(NULL) { }

      ~mySocket()
      {
        if (socket) { delete socket; }
        if (ios) { delete ios; }
      }

      bool Close()
      {
        if (socket == NULL) { return false; }
        socket->close();
        return true;
      }

      static mySocket *Create()
      {
        mySocket *sock = NULL;
        try {
          sock = new mySocket;
          sock->ios = new asio::io_service;
          sock->socket = new tcp::socket(*(sock->ios));
          return sock;
        }
        catch (...) {
          delete sock;
          return NULL;
        }
      }

      bool Connect(const char *addr, const char *service)
      {
        boost::system::error_code ec;
        tcp::resolver resolver(*ios);
        tcp::resolver::query q(addr, service);
        tcp::resolver::iterator i, end;
        i = resolver.resolve(q);
        for (; i != end; i++)
        {
          tcp::endpoint ep(*i);
          socket->connect(ep, ec);
          if (ec) { continue; }
          else { return true; }
        }
        return false;
      }

      const char *GetAddress()
      {
        return socket->local_endpoint().address().to_string().c_str();
      }

      int GetPort()
      {
        return socket->local_endpoint().port();
      }

      bool ReadAll(std::string &str)
      {
        asio::streambuf buf;
        boost::system::error_code ec;
        std::ostringstream os;
        while (asio::read(*socket, buf, boost::asio::transfer_at_least(1), ec))
        {
          os << &buf;
        }
        str = os.str();
        if (ec != boost::asio::error::eof) { return false; }
        return true;
      }

      bool ReadLine(std::string &str)
      {
        asio::streambuf buf;
        boost::system::error_code ec;
        asio::read_until(*socket, buf, "\r\n", ec);
        if (ec) { return false; }
        str = asio::buffer_cast<const char*>(buf.data());
        return true;
      }

      const char *ReadUntil(char delimiter)
      {
        asio::streambuf buf;
        asio::read_until(*socket, buf, delimiter);
        return asio::buffer_cast<const char *>(buf.data());
      }

      const char *Receive(int max_length)
      {
        asio::streambuf buf;
        asio::streambuf::mutable_buffers_type bufs = buf.prepare(max_length);
        int n = socket->receive(bufs);
        buf.commit(n);
        return asio::buffer_cast<const char *>(buf.data());
      }

      bool Send(const char *data)
      {
        try {
          socket->send(asio::buffer(data));
          return true;
        }
        catch (...) {
          return false;
        }
      }

      asio::io_service *ios;
      tcp::socket *socket;
  };
  inline static mySocket *cast_sock(void *obj) { return (mySocket *)obj; }

  socket::socket() : base(), _obj(NULL) { }

  socket::~socket() { }

  bool socket::close()
  {
    return cast_sock(_obj)->Close();
  }

  socket *socket::connect(const char *addr, const char *service)
  {
    socket *sock = NULL;
    mySocket *obj = NULL;
    try {
      sock = new socket;
      obj = mySocket::Create();
      if (obj == NULL) { throw -1; }
      sock->_obj = obj;
      if (not obj->Connect(addr, service)) { throw -1; }
      return sock;
    }
    catch (...) {
      delete sock;
      return NULL;
    }
  }

  const char *socket::get_address()
  {
    return cast_sock(_obj)->GetAddress();
  }

  int socket::get_port()
  {
    return cast_sock(_obj)->GetPort();
  }

  bool socket::is_open()
  {
    return cast_sock(_obj)->socket->is_open();
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
    if (not result) { return 0; }
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

  const char *socket::read_until(char delimiter)
  {
    return cast_sock(_obj)->ReadUntil(delimiter);
  }

  const char *socket::receive(int max_length)
  {
    return cast_sock(_obj)->Receive(max_length);
  }

  bool socket::send(const char *data)
  {
    return cast_sock(_obj)->Send(data);
  }


  class myAcceptor
  {
    public:
      myAcceptor() : ios(), acc(ios), timer(ios) { }

      ~myAcceptor() { }

      mySocket* Accept(int timeout_seconds)
      {
        mySocket *sock = NULL;
        try {
          sock = new mySocket;
          sock->socket = new tcp::socket(ios);
          if (timeout_seconds < 0) { throw -1; }
          else if (timeout_seconds == 0)
          {
            acc.accept(*(sock->socket), ec);
            if (ec) { throw -1; }
          }
          else
          {
            ios.reset();
            ec = asio::error::timed_out;
            timer.expires_from_now(boost::posix_time::seconds(timeout_seconds));
            acc.async_accept(*(sock->socket), boost::bind(&myAcceptor::OnAccept, this, _1));
            timer.async_wait(boost::bind(&myAcceptor::OnTimeOut, this, _1));
            ios.run();
            if (ec) { throw -1; }
          }
          return sock;
        }
        catch (...) {
          delete sock;
          return NULL;
        }
      }

      bool Bind(const char *addr, const char *port)
      {
        boost::system::error_code ec;
        const char *query_address = addr;
        if (std::string(addr) == "*") { query_address = "0"; }
        tcp::resolver resolver(ios);
        tcp::resolver::query q(query_address, port);
        tcp::resolver::iterator i, end;
        i = resolver.resolve(q);
        for (; i != end; i++)
        {
          tcp::endpoint ep(*i);
          acc.open(ep.protocol());
          acc.bind(ep, ec);
          if (ec) { continue; }
          else
          {
            // no error = success
            acc.listen();
            return true;
          }
        }
        return false;
      }

      const char *GetAddress()
      {
        return acc.local_endpoint().address().to_string().c_str();
      }

      int GetPort()
      {
        return acc.local_endpoint().port();
      }

      void OnAccept(const boost::system::error_code &error)
      {
        if (error)
        {
          ec = error;
        }
        else
        {
          // no error means success !
          ec = error;
          timer.cancel();
        }
      }

      void OnTimeOut(const boost::system::error_code &error)
      {
        if (!error)
        {
          ec = asio::error::timed_out;
          acc.cancel();
        }
      }

      asio::io_service ios;
      tcp::acceptor acc;
      asio::deadline_timer timer;
      boost::system::error_code ec;
  };
  inline static myAcceptor* cast_acc(void *obj) { return (myAcceptor *)obj; }

  acceptor::acceptor() : base(), _obj(NULL) { }

  acceptor::~acceptor()
  {
    if (_obj) { delete cast_acc(_obj); }
  }

  socket* acceptor::accept(int timeout_seconds)
  {
    socket *sock = NULL;
    mySocket *obj = NULL;
    try {
      sock = new socket;
      obj = cast_acc(_obj)->Accept(timeout_seconds);
      if (obj == NULL) { throw -1; }
      sock->_obj = obj;
      return sock;
    }
    catch (...) {
      delete obj;
      delete sock;
      return NULL;
    }
  }

  acceptor* acceptor::bind(const char *addr, luabind::object port)
  {
    acceptor *acc = NULL;
    myAcceptor *obj = NULL;
    std::ostringstream os;
    try {
      acc = new acceptor;
      obj = new myAcceptor;
      acc->_obj = obj;
      os << port;
      if (not obj->Bind(addr, os.str().c_str())) { throw -1; }
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

  int acceptor::get_port()
  {
    return cast_acc(_obj)->GetPort();
  }


  bool http::request(std::string url, std::string *head, std::string *body)
  {
    socket *sock = NULL;
    boost::regex r;
    boost::smatch m;

    // matching and request sending
    try {
      std::string protocol, user, password, host, port, path;
      // url format matching
      // URL FORMAT => [http://][<user>[:<password>]@]<host>[:<port>][/<path>]
      r = "^((\\w+)://)?((\\w+)(:(\\w+))?@)?(\\w+(\\.\\w+)+)(:(\\w+))?(/.*)?";
      if (boost::regex_match(url, m, r))
      {
        protocol = m.str(2);
        if (protocol == "") { protocol = "http"; }
        user = m.str(4);
        password = m.str(6);
        host = m.str(7);
        port = m.str(10);
        if (protocol == "http" && port == "") { port = protocol; }
        path = m.str(11);
        if (path == "") { path = "/"; }
      }
      else { throw -1; }
      sock = socket::connect(host.c_str(), port.c_str());
      if (sock == NULL) { throw -1; }
//      sock->send_str("GET " + path + " HTTP/1.0\r\n");
      sock->send_str("GET " + path + " HTTP/1.0\r\n");
      sock->send_str("Host: " + host + "\r\n");
      sock->send("Accept: */*\r\n");
      sock->send("Connection: close\r\n");
      sock->send("\r\n");
    }
    catch (...) {
      delete sock;
      return NULL;
    }

    // response checking and downloading
    try {
      std::string content;
//      std::string version, code, message;
      bool result;
      int pos, head_len, body_begin_pos;
      result = sock->read_all(content);
      if (not result) { throw -1; }

      pos = content.find("\r\n\r\n");
      if (pos != -1)
      {
        head_len = pos + 2;
        body_begin_pos = pos + 4;
      }
      else
      {
        pos = content.find("\n\n");
        if (pos != -1)
        {
          head_len = pos + 1;
          body_begin_pos = pos + 2;
        }
        else { throw -1; }
      }
      if (head) { *head = content.substr(0, head_len); }
      if (body) { *body = content.substr(body_begin_pos); }
//      r = "^HTTP/(\\d+(\\.\\d+)+) (\\d+) (\\w+)(.*)";
//      if (boost::regex_match(content, m, r))
//      {
//        version = m.str(1);
//        code = m.str(3);
//        message = m.str(4);
//      }
//      else
//      {
//        throw -1;
//      }
      delete sock;
      return true;
    }
    catch (...) {
      delete sock;
      return false;
    }
  }

  int http::request_l(lua_State *L)
  {
    using namespace luabind;

    luaL_checktype(L, 1, LUA_TSTRING);
    object o(from_stack(L, 1));
    std::string url = object_cast<const char *>(o);
    std::string head, body;
    int pos, head_len, body_begin_pos;

    bool result = http::request(url, &head, &body);
    if (not result) { return 0; }
    lua_pushstring(L, body.c_str());
    lua_pushstring(L, head.c_str());
    return 2;
  }

}

