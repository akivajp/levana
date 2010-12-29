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
#include <GL/glu.h>

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


static inline void register_to(lua_State *L, luabind::object &to, const char *funcname, lua_CFunction func)
{
  lua_pushcfunction(L, func);
  luabind::object f(luabind::from_stack(L, -1));
  lua_pop(L, 1);
  to[funcname] = f;
}


extern "C"
{

  extern int luaopen_gl(lua_State *L)
  {
    using namespace luabind;

    luabind::open(L);
    module(L, "levana")
    [
      namespace_("gl")
      [
        def("beginprim", (void(*)(unsigned int))&glBegin),
        def("calllist", (void(*)(unsigned int))&glCallList),
        def("clear", (void(*)(unsigned int))&glClear),
        def("clearcolor", (void(*)(float,float,float,float))&glClearColor),
        def("color3d",  (void(*)(double,double,double))&glColor3d),
        def("color3f",  (void(*)(float,float,float))&glColor3f),
        def("color3ub", (void(*)(unsigned char,unsigned char,unsigned char))&glColor3ub),
        def("color4d",  (void(*)(double,double,double,double))&glColor4d),
        def("color4f",  (void(*)(float,float,float,float))&glColor4f),
        def("color4ub", (void(*)(unsigned char,unsigned char,
                                 unsigned char,unsigned char))&glColor4ub),
        def("deletelists", (void(*)(unsigned int, int))&glDeleteLists),
        def("disable", (void(*)(unsigned int))&glDisable),
        def("disable_cstate", (void(*)(unsigned int))&glDisableClientState),
        def("enable", (void(*)(unsigned int))&glEnable),
        def("enable_cstate", (void(*)(unsigned int))&glEnableClientState),
        def("endlist", (void(*)())&glEndList),
        def("endprim", (void(*)())&glEnd),
        def("finish", (void(*)())&glFinish),
        def("flush", (void(*)())&glFlush),
        def("frustum", (void(*)(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble))&glFrustum),
        def("genlists", (unsigned int(*)(int))&glGenLists),
        def("islist", (GLboolean(*)(unsigned int))&glIsList),
        def("loadidentity", (void(*)())&glLoadIdentity),
        def("lookat", (void(*)(double,double,double,double,double,
                               double,double,double,double))&gluLookAt),
        def("newlist", (void(*)(unsigned int,unsigned int))&glNewList),
        def("ortho", (void(*)(double,double,double,double,double,double))&glOrtho),
        def("perspective", (void(*)(double,double,double,double))&gluPerspective),
        def("popmatrix", (void(*)())&glPopMatrix),
        def("pushmatrix", (void(*)())&glPushMatrix),
        def("rectd", (void(*)(double,double,double,double))&glRectd),
        def("rectf", (void(*)(float,float,float,float))&glRectf),
        def("recti", (void(*)(int,int,int,int))&glRecti),
        def("rects", (void(*)(short int,short int,short int,short int))&glRects),
        def("rotated", (void(*)(double,double,double,double))&glRotated),
        def("rotatef", (void(*)(float,float,float,float))&glRotatef),
        def("scaled", (void(*)(double,double,double))&glScaled),
        def("scalef", (void(*)(float,float,float))&glScalef),
        def("translated", (void(*)(double,double,double))&glTranslated),
        def("translatef", (void(*)(float,float,float))&glTranslatef),
        def("vertex2d", (void(*)(double,double))&glVertex2d),
        def("vertex2f", (void(*)(float,float))&glVertex2f),
        def("vertex2i", (void(*)(int,int))&glVertex2i),
        def("vertex2s", (void(*)(short int,short int))&glVertex2s),
        def("vertex3d", (void(*)(double,double,double))&glVertex3d),
        def("vertex3f", (void(*)(float,float,float))&glVertex3f),
        def("vertex3i", (void(*)(int,int,int))&glVertex3i),
        def("vertex3s", (void(*)(short int,short int,short int))&glVertex3s),
        def("vertex4d", (void(*)(double,double,double,double))&glVertex4d),
        def("vertex4f", (void(*)(float,float,float,float))&glVertex4f),
        def("vertex4i", (void(*)(int,int,int,int))&glVertex4i),
        def("vertex4s", (void(*)(short int,short int,short int,short int))&glVertex4s),
        def("viewport", (void(*)(int,int,int,int))&glViewport)
      ]
    ];

    object gl = globals(L)["levana"]["gl"];
    register_to(L, gl, "getb", &lua_glGetb);
    register_to(L, gl, "getd", &lua_glGetd);
    register_to(L, gl, "getf", &lua_glGetf);
    register_to(L, gl, "geti", &lua_glGeti);
    register_to(L, gl, "getp", &lua_glGetp);

    gl["ACCUM_BUFFER_BIT"]    = GL_ACCUM_BUFFER_BIT;
    gl["COLOR_ARRAY"]         = GL_COLOR_ARRAY;
    gl["COLOR_BUFFER_BIT"]    = GL_COLOR_BUFFER_BIT;
    gl["COLOR_CLEAR_VALUE"]   = GL_COLOR_CLEAR_VALUE;
    gl["COMPILE"]             = GL_COMPILE;
    gl["CURRENT_COLOR"]       = GL_CURRENT_COLOR;
    gl["DEPTH_BUFFER_BIT"]    = GL_DEPTH_BUFFER_BIT;
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
    gl["STENCIL_BUFFER_BIT"]  = GL_STENCIL_BUFFER_BIT;
    gl["TEXTURE_COORD_ARRAY"] = GL_TEXTURE_COORD_ARRAY;
    gl["TRIANGLE_FAN"]        = GL_TRIANGLE_FAN;
    gl["TRIANGLE_STRIP"]      = GL_TRIANGLE_STRIP;
    gl["TRIANGLES"]           = GL_TRIANGLES;
    gl["TRUE"]                = GL_TRUE;
    gl["VERTEX_ARRAY"]        = GL_VERTEX_ARRAY;
  }
}

