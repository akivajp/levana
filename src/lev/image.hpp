#ifndef _IMAGE_HPP
#define _IMAGE_HPP

/////////////////////////////////////////////////////////////////////////////
// Name:        src/lev/image.hpp
// Purpose:     header for image handling classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     01/25/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "base.hpp"
#include "prim.hpp"
#include <lua.h>

extern "C" {
  int luaopen_lev_image(lua_State *L);
}

namespace lev
{

  // class dependencies
  class canvas;
  class control;
  class font;


  class drawable : public base
  {
    protected:
      drawable();
    public:
      virtual ~drawable();
      virtual bool call_compiled(canvas *cv) { return false; }
      virtual bool call_texture(canvas *cv) { return false; }
      virtual bool compile(canvas *cv, bool force = false) { return false; }
      virtual bool compile2(canvas *cv) { return compile(cv); }
      virtual bool draw_on(canvas *cv, int x = 0, int y = 0, unsigned char alpha = 255) { return false; }
      virtual bool draw_on1(canvas *cv) { return false; }
      virtual bool draw_on3(canvas *cv, int x = 0, int y = 0) { return false; }
      virtual type_id get_type_id() const { return LEV_TDRAWABLE; }
      virtual const char *get_type_name() const { return "lev.drawable"; }
      virtual bool texturize(canvas *cv, bool force = false) { return false; }
      virtual bool texturize2(canvas *cv) { return texturize(cv); }
  };

  class image : public drawable
  {
    protected:
      image();
    public:
      virtual ~image();
      bool blit(image *img, int x = 0, int y = 0);
      static image* capture(control *src);
      bool call_compiled(canvas *cv);
      bool call_texture(canvas *cv);
      static size calc_string(const std::string &str, font *f);
      static size calc_string1(const std::string &str) { return calc_string(str, NULL); }
      virtual bool clear(const color &c);
      virtual bool clear0() { return clear(color::transparent()); }
      bool clear_rect(const rect &r, const color &c = color::transparent());
      bool clear_rect1(const rect &r) { return clear_rect(r); }
      image* clone();
      virtual bool compile(canvas *cv, bool force = false);
      static image* create(int width, int height);
      virtual bool draw_on(canvas *cv, int x = 0, int y = 0, unsigned char alpha = 255);
      virtual bool draw_on1(canvas *cv) { return draw_on(cv); }
      virtual bool draw_on3(canvas *cv, int x, int y) { return draw_on(cv, x, y); }
      bool draw_pixel(color *c, int x, int y);
      bool draw_text(const std::string &text, font *f = NULL, color *fg = NULL,
                     color *bg = NULL, int x = 0, int y = 0);
      static int draw_text_l(lua_State *L);
      bool fill_circle(int x, int y, int radius, color *filling);
      bool fill_rect(int x, int y, int w, int h, color *filling);
      virtual int get_h() const;
      color* get_pixel(int x, int y);
      void* get_rawobj() { return _obj; }
      const rect get_rect() const;
      const size get_size() const;
      virtual type_id get_type_id() const { return LEV_TIMAGE; }
      virtual const char *get_type_name() const { return "lev.image"; }
      virtual int get_w() const;
      static bool init();
      bool is_compiled();
      bool is_texturized();
      static image* levana_icon();
      static image* load(const std::string &filename);
      bool reload(const std::string &filename);
      bool save(const char *filename) const;
      bool set_pixel(color* c, int x, int y);
      static image* string(const std::string &str, font *f = NULL, color *fore = NULL, color *back = NULL);
      static image* string1(const std::string &str) { return string(str, NULL, NULL, NULL); }
      static int string_l(lua_State *L);
      bool stroke_circle(int x, int y, int radius, color *border, int width);
      bool stroke_line(int x1, int y1, int x2, int y2, color *border, int width,
                       const std::string &style = "");
      bool stroke_line6(int x1, int y1, int x2, int y2, color *border, int width)
      { return stroke_line(x1, y1, x2, y2, border, width); }

      bool stroke_rect(int x, int y, int w, int h, color *border, int width);
      image* sub_image(int x, int y, int w, int h);
      static int sub_image_l(lua_State *L);
      bool swap(image *img);
      virtual bool texturize(canvas *cv, bool force = false);
    protected:
      void *_obj;
      void *_status;
  };


  class animation : public drawable
  {
    protected:
      animation();
    public:
      virtual ~animation();
      virtual bool compile(canvas *cv, bool force = false);
      image* get_current();
      bool add_file(const std::string &filename, int duration);
      static animation* create(bool repeating = true);
      static animation* create0() { return create(); }
//      virtual type_id get_type_id() const { return LEV_TANIMATION; }
      virtual const char *get_type_name() const { return "lev.image.animation"; }
      virtual bool texturize(canvas *cv, bool force = false);
    protected:
      void *_obj;
  };

  class transition : public drawable
  {
    protected:
      transition();
    public:
      virtual ~transition();
      virtual bool draw_on(canvas *cv, int x = 0, int y = 0, unsigned char alpha = 255);
      virtual bool draw_on1(canvas *cv) { return draw_on(cv); }
      virtual bool draw_on3(canvas *cv, int x = 0, int y = 0) { return draw_on(cv, x, y); }
      static transition* create(luabind::object img);
      static transition* create0() { return create(luabind::object()); }
      virtual type_id get_type_id() const { return LEV_TTRANSITION; }
      virtual const char *get_type_name() const { return "lev.image.transition"; }
      bool is_running();
      bool set_current(luabind::object current);
      bool set_next(luabind::object next, int duration = 1000, const std::string &type = "");
      virtual bool texturize(canvas *cv, bool force = false);
    protected:
      void *_obj;
  };

  class layout : public drawable
  {
    protected:
      layout();
    public:
      virtual ~layout();
      virtual bool clear(const color &c);
      virtual bool clear0() { return clear(color::transparent()); }
      bool complete();
      static layout* create(int width_stop = -1);
      virtual bool draw_on(canvas *cv, int x = 0, int y = 0, unsigned char alpha = 255);
      luabind::object get_color();
      luabind::object get_font();
      virtual type_id get_type_id() const { return LEV_TLAYOUT; }
      virtual const char *get_type_name() const { return "lev.image.layout"; }
      bool is_done();
      bool on_hover(int x, int y);
      bool on_left_click(int x, int y);
      bool reserve_clickable(const std::string &name, image *normal, image *hover,
                             luabind::object lclick_func);
      bool reserve_clickable_text(const std::string &name, const std::string &text,
                                  luabind::object lclick_func);
      bool reserve_image(const std::string &name, image *img);
      bool reserve_new_line();
      bool reserve_word(const std::string &word, const std::string &ruby);
      bool reserve_word1(const std::string &word) { return reserve_word(word, ""); }
      bool set_color(luabind::object c);
      bool set_font(luabind::object f);
      bool set_on_hover(const std::string &name, luabind::object hover_func);
      bool show_next();
    protected:
      void *_obj;
  };

}

#endif // _IMAGE_HPP

