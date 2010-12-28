/////////////////////////////////////////////////////////////////////////////
// Name:        src/levana/gl.hpp
// Purpose:     source for OpenGL-Lua binding
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     12/23/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "levana/gl.hpp"
#include <wx/glcanvas.h>
#include <luabind/luabind.hpp>

/*
template <typename T>
class gl
{
  public:
    static luabind::object get(lua_State *L, GLenum pname)
    {
      int length;
      T *array;
      luabind::object table = luabind::newtable(L);
      switch (pname)
      {
        case GL_COLOR_CLEAR_VALUE:
        case GL_CURRENT_COLOR:
          length = 4;
          break;
        default:
          length = 0;
          break;
      }
      try {
        array = new T[length];
      }
      catch (...) {
        throw "gl.get*: allocation error";
      }
      gl<T>::get_array(pname, array);
      for (int i = 0; i < length; i++) { table[i+1] = array[i]; }
      delete[] array;
      return table;
    }
  private:
    static void get_array(GLenum pname, T *params);
};

template<> inline void gl<GLboolean>::get_array(GLenum pname, GLboolean *params)
{
  glGetBooleanv(pname, params);
}
template<> inline void gl<GLdouble>::get_array(GLenum pname, GLdouble *params)
{
  glGetDoublev(pname, params);
}
template<> inline void gl<GLfloat>::get_array(GLenum pname, GLfloat *params)
{
  glGetFloatv(pname, params);
}
template<> inline void gl<GLint>::get_array(GLenum pname, GLint *params)
{
  glGetIntegerv(pname, params);
}
template<> inline void gl<GLvoid*>::get_array(GLenum pname, GLvoid **params)
{
  glGetPointerv(pname, params);
}


static int lua_glGetb(lua_State *L)
{
  GLenum pname = luaL_checklong(L, 1);
  luabind::object table = gl<GLboolean>::get(L, pname);
  table.push(L);
  return 1;
}
static int lua_glGetd(lua_State *L)
{
  GLenum pname = luaL_checklong(L, 1);
  luabind::object table = gl<GLdouble>::get(L, pname);
  table.push(L);
  return 1;
}
static int lua_glGetf(lua_State *L)
{
  GLenum pname = luaL_checklong(L, 1);
  luabind::object table = gl<GLfloat>::get(L, pname);
  table.push(L);
  return 1;
}
static int lua_glGeti(lua_State *L)
{
  GLenum pname = luaL_checklong(L, 1);
  luabind::object table = gl<GLint>::get(L, pname);
  table.push(L);
  return 1;
}
static int lua_glGetp(lua_State *L)
{
  GLenum pname = luaL_checklong(L, 1);
  luabind::object table = gl<GLvoid*>::get(L, pname);
  table.push(L);
  return 1;
}
*/


static inline void register_to(lua_State *L, const char *lib, const char *funcname, lua_CFunction func)
{
  lua_getglobal(L, lib);
  lua_pushcfunction(L, func);
  lua_setfield(L, -2, funcname);
  lua_pop(L, 1);
}


extern "C"
{

  extern int luaopen_gl(lua_State *L)
  {
    using namespace luabind;
    /*
    luabind::open(L);
    module(L, "gl")
    [
      def("beginprim", &glBegin),
      def("clear", &glClear),
      def("clearcolor", &glClearColor),
      def("color3b",  &glColor3b),
      def("color3d",  &glColor3d),
      def("color3f",  &glColor3f),
      def("color3i",  &glColor3i),
      def("color3s",  &glColor3s),
      def("color3ub", &glColor3ub),
      def("color3ui", &glColor3ui),
      def("color3us", &glColor3us),
      def("color4b",  &glColor4b),
      def("color4d",  &glColor4d),
      def("color4f",  &glColor4f),
      def("color4i",  &glColor4i),
      def("color4s",  &glColor4s),
      def("color4ub", &glColor4ub),
      def("color4ui", &glColor4ui),
      def("color4us", &glColor4us),
      def("disable", &glDisable),
      def("disable_cstate", &glDisableClientState),
      def("enable", &glEnable),
      def("enable_cstate", &glEnableClientState),
      def("endprim", &glEnd),
      def("finish", &glFinish),
      def("flush", &glFlush),
      def("rectd", &glRectd),
      def("rectf", &glRectf),
      def("recti", &glRecti),
      def("rects", &glRects),
      def("vertex2d", &glVertex2d),
      def("vertex2f", &glVertex2f),
      def("vertex2i", &glVertex2i),
      def("vertex2s", &glVertex2s),
      def("vertex3d", &glVertex3d),
      def("vertex3f", &glVertex3f),
      def("vertex3i", &glVertex3i),
      def("vertex3s", &glVertex3s),
      def("vertex4d", &glVertex4d),
      def("vertex4f", &glVertex4f),
      def("vertex4i", &glVertex4i),
      def("vertex4s", &glVertex4s)
    ];
    register_to(L, "gl", "getbv", &lua_glGetb);
    register_to(L, "gl", "getdv", &lua_glGetd);
    register_to(L, "gl", "getfv", &lua_glGetf);
    register_to(L, "gl", "getiv", &lua_glGeti);
    register_to(L, "gl", "getpv", &lua_glGetp);

    object gl = globals(L)["gl"];
    gl["COLOR_ARRAY"]         = GL_COLOR_ARRAY;
    gl["COLOR_CLEAR_VALUE"]   = GL_COLOR_CLEAR_VALUE;
    gl["CURRENT_COLOR"]       = GL_CURRENT_COLOR;
    gl["EDGE_FLAG_ARRAY"]     = GL_EDGE_FLAG_ARRAY;
    gl["FALSE"]               = GL_FALSE;
    gl["INDEX_ARRAY"]         = GL_INDEX_ARRAY;
    gl["LINE_LOOP"]           = GL_LINE_LOOP;
    gl["LINE_STRIP"]          = GL_LINE_STRIP;
    gl["LINES"]               = GL_LINES;
    gl["NORMAL_ARRAY"]        = GL_NORMAL_ARRAY;
    gl["POINTS"]              = GL_POINTS;
    gl["POLYGON"]             = GL_POLYGON;
    gl["QUAD_STRIP"]          = GL_QUAD_STRIP;
    gl["QUADS"]               = GL_QUADS;
    gl["TEXTURE_COORD_ARRAY"] = GL_TEXTURE_COORD_ARRAY;
    gl["TRIANGLE_FAN"]        = GL_TRIANGLE_FAN;
    gl["TRIANGLE_STRIP"]      = GL_TRIANGLE_STRIP;
    gl["TRIANGLES"]           = GL_TRIANGLES;
    gl["TRUE"]                = GL_TRUE;
    gl["VERTEX_ARRAY"]        = GL_VERTEX_ARRAY;
    */
  }
}

