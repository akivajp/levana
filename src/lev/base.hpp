#ifndef _BASE_HPP
#define _BASE_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/app.hpp
// Purpose:     header for base class of the all
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/12/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include <boost/format.hpp>
#include <luabind/luabind.hpp>
#include <string>

namespace lev
{

  class base
  {
    public:
      enum type_id
      {
        LEV_TNONE = -1,
        LEV_TBASE = 1,
          LEV_TARCHIVE,
          LEV_TCHANNEL,
          LEV_TCOLOR,
          LEV_TDB,

          LEV_TDRAWABLE,
            LEV_TTRANSITION,
            LEV_TIMAGE,
              LEV_TLAYOUT,
            LEV_TIMAGE_END,
          LEV_TDRAWABLE_END,

          LEV_TEVENT,
          LEV_TFILE_PATH,
          LEV_TFILE_SYSTEM,
          LEV_TFONT,

          LEV_THANDLER,
            LEV_TAPP,
            LEV_TCONTROL,
              LEV_TCANVAS,
              LEV_TFRAME,
              LEV_THTMLVIEW,
              LEV_TMENUBAR,
              LEV_TTEXTBOX,
            LEV_TCONTROL_END,
            LEV_TMENU,
            LEV_TSYSTRAY,
            LEV_TTIMER,
          LEV_THANDLER_END,

          LEV_TINFO,

          LEV_TINPUT,
            LEV_TINSTATE,
            LEV_TINRECORD,
          LEV_TINPUT_END,

          LEV_TMB_STRING,
          LEV_TMIXER,

          LEV_TLOCALE,

          LEV_TNET_BASE,
            LEV_TACCEPTOR,
            LEV_TADDRESS,
            LEV_TCONNECTOR,
            LEV_TSOCKET,
            LEV_TURL,
          LEV_TNET_BASE_END,

          LEV_TPOINT,
          LEV_TRECT,
          LEV_TSIZE,

          LEV_TSIZER,
            LEV_TGSIZER,
            LEV_THSIZER,
            LEV_TVSIZER,
          LEV_TSIZER_END,

          LEV_TTEMP_NAME,
          LEV_TVECTOR,
        LEV_TBASE_END,
      };
    protected:
      base() { }
      virtual ~base() { }
    public:
      virtual type_id get_type_id() const { return LEV_TBASE; }
      virtual const char *get_type_name() const { return "lev.base"; }

      static std::string tostring(const base *b)
      {
        return (boost::format("%1%: %2%") % b->get_type_name() % b).str();
      }

      static bool is_type_of(const luabind::object &obj, type_id id,
                             type_id id_stop = LEV_TNONE)
      {
        if (id_stop == LEV_TNONE) { id_stop = id; }
        if (! obj) { return false; }
        if (luabind::type(obj) != LUA_TUSERDATA) { return false; }

        luabind::object obj_id = obj["type_id"];
        if (! obj_id) { return false; }
        if (luabind::type(obj_id) != LUA_TNUMBER) { return false; }

        int id_of_obj = luabind::object_cast<int>(obj_id);
        if (id <= id_of_obj && id_of_obj <= id_stop) { return true; }
        return false;
      }
  };

}

#endif // _BASE_HPP

