/////////////////////////////////////////////////////////////////////////////
// Name:        src/gui.cpp
// Purpose:     source for GUI controls
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Created:     12/06/2010
// Copyright:   (C) 2010 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"

#include "deriver.hpp"
#include "lev/gui.hpp"
#include "lev/lev.hpp"
#include "register.hpp"

#include <string>
#include <luabind/luabind.hpp>
#include <wx/stc/stc.h>

int luaopen_lev_gui(lua_State *L)
{
  using namespace lev;
  using namespace luabind;

  globals(L)["require"]("lev");

  // GUI control, event handling
  module(L, "lev")
  [
    namespace_("gui"),
    namespace_("classes")
    [
      // base class
      class_<control, handler>("control")
        .def("hide", &control::hide)
        .def("show", &control::show)
        .property("exists", &control::is_valid)
        .property("id", &control::getid)
        .property("h", &control::get_height, &control::set_height)
        .property("height", &control::get_height, &control::set_height)
        .property("is_valid", &control::is_valid)
        .property("isvalid", &control::is_valid)
        .property("is_shown", &control::is_shown, &control::set_shown)
        .property("isshown", &control::is_shown, &control::set_shown)
        .property("sizer", &control::get_sizer, &control::set_sizer)
        .property("w", &control::get_width, &control::set_width)
        .property("width", &control::get_width, &control::set_width),
      class_<canvas, control>("canvas")
        .def("blendmode", &canvas::blendmode)
        .def("clear", &canvas::clear)
        .def("clearcolor", &canvas::clearcolor)
        .def("draw", &canvas::draw_image)
        .def("flush", &canvas::flush)
        .def("line",  &canvas::line)
        .def("set2d", &canvas::set2d)
        .def("setcurrent", &canvas::setcurrent)
        .def("swap", &canvas::swap)
        .scope
        [
          def("create_c", &canvas::create, adopt(result))
        ],
      class_<code_edit, control>("code_edit")
        .def("can_redo", &code_edit::can_redo)
        .def("can_undo", &code_edit::can_undo)
        .def("clear", &code_edit::clear)
        .def("copy", &code_edit::copy)
        .def("cut", &code_edit::cut)
        .def("redo", &code_edit::redo)
        .def("paste", &code_edit::paste)
        .def("save", &code_edit::save_file)
        .def("save_file", &code_edit::save_file)
        .def("select_all", &code_edit::select_all)
        .def("undo", &code_edit::undo)
        .property("lang", &code_edit::get_language, &code_edit::set_language)
        .property("language", &code_edit::get_language, &code_edit::set_language)
        .property("text", &code_edit::get_text, &code_edit::set_text)
        .scope
        [
          def("create_c", &code_edit::create)
        ],
      class_<frame, control>("frame")
        .def("close", &frame::close)
        .def("close", &frame::close_noforce)
        .def("fit", &frame::fit)
        .property("cap", &frame::get_title, &frame::set_title)
        .property("caption", &frame::get_title, &frame::set_title)
        .property("mb", &frame::get_menubar, &frame::set_menubar)
        .property("mbar", &frame::get_menubar, &frame::set_menubar)
        .property("menubar", &frame::get_menubar, &frame::set_menubar)
        .property("status", &frame::get_status, &frame::set_status)
        .property("title", &frame::get_title, &frame::set_title)
        .scope
        [
          def("create_c", &frame::create, adopt(result) )
        ],
      class_<htmlview, control>("htmlview")
        .def("loadpage", &htmlview::loadpage)
        .def("totext", &htmlview::totext)
        .property("page", &htmlview::get_page, &htmlview::set_page)
        .scope
        [
          def("create_c", &htmlview::create, adopt(result))
        ],
      class_<player, control>("player")
        .def("loadlocal", &player::loadlocal)
        .def("loaduri", &player::loaduri)
        .def("pause", &player::pause)
        .def("play", &player::play)
        .def("stop", &player::stop)
        .property("bestsize", &player::getbestsize)
        .property("ispaused", &player::ispaused)
        .property("isplaying", &player::isplaying)
        .property("isstopped", &player::isstopped)
        .property("volume", &player::getvolume, &player::setvolume)
        .scope
        [
          def("create_c", &player::create, adopt(result))
        ],
      class_<menu, handler>("menu")
        .def("enable", &menu::enable)
        .scope
        [
          def("build", &menu::build),
          def("create_c", &menu::create, adopt(result))
        ],
      class_<menubar, control>("menubar")
        .def("enable", &menubar::enable)
        .scope
        [
          def("build", &menubar::build),
          def("create_c", &menubar::create, adopt(result))
        ],
      class_<systray, handler>("systray")
        .def("popup", &systray::popup)
        .def("remove_icon", &systray::remove_icon)
        .def("set_icon", &systray::set_icon)
        .property("menugen", &systray::get_menu_generator, &systray::set_menu_generator)
        .property("menu_generator", &systray::get_menu_generator, &systray::set_menu_generator)
        .scope
        [
          def("create_c", &systray::create, adopt(result))
        ],
      class_<textbox, control>("textbox")
        .def("set_multiline", &textbox::set_multiline)
        .property("v", &textbox::get_value, &textbox::set_value)
        .property("val", &textbox::get_value, &textbox::set_value)
        .property("value", &textbox::get_value, &textbox::set_value)
        .scope
        [
          def("create_c", &textbox::create)
        ]
    ]
  ];
  object classes = globals(L)["lev"]["classes"];
  object gui = globals(L)["lev"]["gui"];
  register_to(L, gui, "msgbox", &gui::msgbox_l);
  register_to(L, gui, "file_selector", &gui::file_selector_l);
  register_to(L, classes["canvas"],    "create", &canvas::create_l);
  register_to(L, classes["code_edit"], "create", &code_edit::create_l);
  register_to(L, classes["frame"],     "create", &frame::create_l);
  register_to(L, classes["htmlview"],  "create", &htmlview::create_l);
  register_to(L, classes["menu"],      "create", &menu::create_l);
  register_to(L, classes["menubar"],   "create", &menubar::create_l);
  register_to(L, classes["player"],    "create", &player::create_l);
  register_to(L, classes["systray"],   "create", &systray::create_l);
  register_to(L, classes["textbox"],   "create", &textbox::create_l);
  gui["build_menu"]    = classes["menu"]["build"];
  gui["build_menubar"] = classes["menubar"]["build"];
  gui["canvas"]        = classes["canvas"]["create"];
  gui["code_edit"]     = classes["code_edit"]["create"];
  gui["frame"]         = classes["frame"]["create"];
  gui["htmlview"]      = classes["htmlview"]["create"];
  gui["menu"]          = classes["menu"]["create"];
  gui["menubar"]       = classes["menubar"]["create"];
  gui["player"]        = classes["player"]["create"];
  gui["systray"]       = classes["systray"]["create"];
  gui["textbox"]       = classes["textbox"]["create"];


  // sizers
  module(L, "lev")
  [
    namespace_("classes")
    [
      class_<sizer, base>("sizer")
        .def("fit", &sizer::fit)
        .def("fitinside", &sizer::fitinside)
        .def("layout", &sizer::layout),
      class_<gsizer, sizer>("gsizer")
        .scope
        [
          def("create_c", &gsizer::create, adopt(result))
        ],
      class_<hsizer, sizer>("hsizer")
        .scope
        [
          def("create_c", &hsizer::create, adopt(result))
        ],
      class_<vsizer, sizer>("vsizer")
        .scope
        [
          def("create_c", &vsizer::create, adopt(result))
        ]
    ]
  ];
  register_to(L, classes["gsizer"], "create", &gsizer::create_l);
  register_to(L, classes["hsizer"], "create", &hsizer::create_l);
  register_to(L, classes["vsizer"], "create", &vsizer::create_l);
  gui["gsizer"] = classes["gsizer"]["create"];
  gui["hsizer"] = classes["hsizer"]["create"];
  gui["vsizer"] = classes["vsizer"]["create"];

  return 0;
}

namespace lev
{

  static wxWindow *cast_win(void *obj) { return (wxWindow *)obj; }

  control::control() : handler(), _id(0), _sz(NULL)
  { }

  control::~control()
  {
    if (system_managed)
    {
      // wxWidgets will automatically delete
    }
    else if (_id && !is_valid())
    {
      // the object isn't already found
      _obj = NULL;
    }
  }

  int control::get_height()
  {
    int h;
    cast_win(_obj)->GetSize(NULL, &h);
    return h;
  }

  int control::getid()
  {
    return _id;
  }

  sizer *control::get_sizer()
  {
    return _sz;
  }

  long control::get_style()
  {
    return ((wxWindow *)_obj)->GetWindowStyle();
  }

  int control::get_width()
  {
    int w;
    cast_win(_obj)->GetSize(&w, NULL);
    return w;
  }

  bool control::is_shown()
  {
    return ((wxWindow *)_obj)->IsShown();
  }

  bool control::is_valid()
  {
    if (wxWindow::FindWindowById(_id)) { return true; }
    return false;
  }

  bool control::set_height(int h)
  {
    cast_win(_obj)->SetSize(get_width(), h);
    return true;
  }

  void control::set_sizer(sizer *s)
  {
    if (s)
    {
      ((wxWindow *)_obj)->SetSizer((wxSizer *)s->get_rawobj());
      _sz = s;
      s->wx_managed = true;
    }
  }

  bool control::set_shown(bool showing)
  {
//    gui_lock();
    return ((wxWindow *)_obj)->Show(showing);
  }

  bool control::set_width(int w)
  {
    cast_win(_obj)->SetSize(w, get_height());
    return true;
  }


  const char *gui::file_selector(const char *message, const char *def_path, const char *def_file,
                                 const char *def_ext, const char *wildcard, control *parent)
  {
    wxString msg, path, file, ext, wild, result;
    std::string str;
    int flags = 0;
    wxWindow *p = NULL;
    if (message) { msg = wxString(message, wxConvUTF8); }
    else { msg = wxT("Choose a file"); }
    if (def_path) { path = wxString(def_path, wxConvUTF8); }
    if (def_file) { file = wxString(def_file, wxConvUTF8); }
    if (def_ext)  { ext  = wxString(def_ext,  wxConvUTF8); }
    if (wildcard) { wild = wxString(wildcard, wxConvUTF8); }
    else { wild = wxT("*.*"); }
    if (parent) { p = (wxWindow *)parent->get_rawobj(); }
    result = wxFileSelector(msg, path, file, ext, wild, flags, p);
    str = (const char *)result.mb_str(wxConvUTF8);
    return str.c_str();
  }

  int gui::file_selector_l(lua_State *L)
  {
    using namespace lev;
    using namespace luabind;
    const char *msg = "Choose a file to open";
    const char *def_path = "", *def_file = "";
    const char *def_ext = "", *wildcard = "*.*", *flags = "";
    control *parent = NULL;
    long f = 0;
    wxWindow *p = NULL;
    std::string filename;

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

    if (t["message"])   { msg = object_cast<const char *>(t["message"]); }
    else if (t["msg"])  { msg = object_cast<const char *>(t["msg"]); }
    else if (t["mes"])  { msg = object_cast<const char *>(t["mes"]); }
    else if (t["m"])    { msg = object_cast<const char *>(t["m"]); }
    else if (t["str1"]) { msg = object_cast<const char *>(t["str1"]); }

    if (t["default_path"])  { def_path = object_cast<const char *>(t["default_path"]); }
    else if (t["def_path"]) { def_path = object_cast<const char *>(t["def_path"]); }
    else if (t["path"])     { def_path = object_cast<const char *>(t["path"]); }
    else if (t["str2"])     { def_path = object_cast<const char *>(t["str2"]); }

    if (t["default_file"])  { def_file = object_cast<const char *>(t["default_file"]); }
    else if (t["def_file"])  { def_file = object_cast<const char *>(t["def_file"]); }
    else if (t["file"])  { def_file = object_cast<const char *>(t["file"]); }
    else if (t["str3"])  { def_file = object_cast<const char *>(t["str3"]); }

    if (t["default_extension"]) { def_ext = object_cast<const char *>(t["default_extension"]); }
    else if (t["default_ext"])  { def_ext = object_cast<const char *>(t["default_ext"]); }
    else if (t["def_ext"])      { def_ext = object_cast<const char *>(t["def_ext"]); }
    else if (t["extension"])    { def_ext = object_cast<const char *>(t["extension"]); }
    else if (t["ext"])          { def_ext = object_cast<const char *>(t["ext"]); }
    else if (t["str4"])         { def_ext = object_cast<const char *>(t["str4"]); }

    if (t["wildcard"]) { wildcard = object_cast<const char *>(t["wildcard"]); }
    else if (t["wild"]) { wildcard = object_cast<const char *>(t["wild"]); }
    else if (t["wc"]) { wildcard = object_cast<const char *>(t["wc"]); }
    else if (t["str5"]) { wildcard = object_cast<const char *>(t["str5"]); }

    if (t["flags"]) { flags = object_cast<const char *>(t["flags"]); }
    else if (t["flag"]) { flags = object_cast<const char *>(t["flag"]); }
    else if (t["f"]) { flags = object_cast<const char *>(t["f"]); }
    else if (t["str6"]) { flags = object_cast<const char *>(t["str6"]); }

    if (t["parent"]) { parent = object_cast<control *>(t["parent"]); }
    else if (t["p"]) { parent = object_cast<control *>(t["p"]); }
    else if (t["udata1"]) { parent = object_cast<control *>(t["udata1"]); }
    if (parent) { p = (wxWindow *)parent->get_rawobj(); }

    if (strcmp(flags, "r") == 0) { f = f | wxFD_OPEN; }
    if (strstr(flags, "open")) { f = f | wxFD_OPEN; }
    if (strcmp(flags, "w") == 0) { f = f | wxFD_SAVE; }
    if (strstr(flags, "save")) { f = f | wxFD_SAVE; }
    if (strstr(flags, "overwrite")) { f = f | wxFD_OVERWRITE_PROMPT; }
    if (strstr(flags, "must_exist")) { f = f | wxFD_FILE_MUST_EXIST; }

    filename = (const char *)wxFileSelector(
        wxString(msg, wxConvUTF8),
        wxString(def_path, wxConvUTF8),
        wxString(def_file, wxConvUTF8),
        wxString(def_ext, wxConvUTF8),
        wxString(wildcard, wxConvUTF8),
        f, p).mb_str();
    lua_pushstring(L, filename.c_str());
    return 1;
  }


  int gui::msgbox_l(lua_State *L)
  {
    wxString msg, cap;
    const char *m = "Message", *c = "Message", *s = "";
    long style = 0;

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

    if (t["message"]) { m = object_cast<const char *>(t["message"]); }
    else if (t["msg"]) { m = object_cast<const char *>(t["msg"]); }
    else if (t["mes"]) { m = object_cast<const char *>(t["mes"]); }
    else if (t["m"]) { m = object_cast<const char *>(t["m"]); }
    else if (t["str1"]) { m = object_cast<const char *>(t["str1"]); }

    if (t["caption"]) { c = object_cast<const char *>(t["caption"]); }
    else if (t["cap"]) { c = object_cast<const char *>(t["cap"]); }
    else if (t["c"]) { c = object_cast<const char *>(t["c"]); }
    else if (t["str2"]) { c = object_cast<const char *>(t["str2"]); }

    if (t["style"]) { s = object_cast<const char *>(t["style"]); }
    else if (t["s"]) { s = object_cast<const char *>(t["s"]); }
    else if (t["str3"]) { s = object_cast<const char *>(t["str3"]); }

    msg = wxString(m, wxConvUTF8);
    cap = wxString(c, wxConvUTF8);
    if (strstr(s, "yes")) { style = style | wxYES_NO; }
    if (strstr(s, "no")) { style = style | wxYES_NO; }
    if (strstr(s, "cancel")) { style = style | wxCANCEL; }
    if (strstr(s, "ok")) { style = style | wxOK; }
    if (strstr(s, "exclamation")) { style = style | wxICON_EXCLAMATION; }
    if (strstr(s, "hand")) { style = style | wxICON_HAND; }
    if (strstr(s, "error")) { style = style | wxICON_ERROR; }
    if (strstr(s, "question")) { style = style | wxICON_QUESTION; }
    if (strstr(s, "info")) { style = style | wxICON_INFORMATION; }

    int result = wxMessageBox(msg, cap, style);
    if (result == wxYES || result == wxOK) { lua_pushboolean(L, true); }
    else if (result == wxNO) { lua_pushboolean(L, false); }
    else if (result == wxCANCEL) { return 0; }
    return 1;
  }



  class mySTC : public myHandler<wxStyledTextCtrl>
  {
    public:
      mySTC() : line_number_margin(0) { }

      ~mySTC() { }

      void OnCharAdded(wxStyledTextEvent &event)
      {
        char chr = (char)event.GetKey();
        int currentLine = GetCurrentLine();

        UpdateLineNumbers();
        SetSavePoint();
      }

      bool SetLevKeyWords()
      {
        // main
        SetKeyWords(0, wxT("and break do else elseif end false for function if "
                           "in local nil not or repeat return then true until while"));
        // lua std functions
        SetKeyWords(1, wxT("assert collectgarbage dofile error "
                           "gcinfo getfenv getmetatable "
                           "ipairs keyword "
                           "load loadfile loadstring "
                           "module next "
                           "pairs pcall print "
                           "rawequal rawget rawset require "
                           "select setfenv setmetatable "
                           "tonumber tostring type "
                           "unpack xpcall "
                           "_G _VERSION"));
        // lua module functions
        SetKeyWords(2, wxT("coroutine.running coroutine.create coroutine.resume "
                           "coroutine.status coroutine.wrap coroutine.yield "
                           "debug.debug debug.getfenv debug.gethook "
                           "debug.getinfo debug.getlocal debug.getmetatable "
                           "debug.getregistry debug.getupvalue debug.setlocal "
                           "debug.setupvalue debug.setfenv debug.sethook "
                           "debug.setmetatable debug.traceback "
                           "io.stdin io.stdout io.stderr io.close io.flush "
                           "io.input io.lines io.open io.output io.popen "
                           "io.read io.tmpfile io.type io.write "
                           "math.abs math.acos math.asin math.atan math.atan2 "
                           "math.ceil math.sin math.cos math.tan math.deg "
                           "math.exp math.floor math.log math.log10 math.max "
                           "math.min math.fmod math.modf math.cosh math.sinh "
                           "math.tanh math.pow math.rad math.sqrt math.frexp "
                           "math.ldexp math.random math.randomseed math.pi "
                           "os.clock os.date os.difftime os.execute os.exit os.getenv "
                           "os.remove os.rename os.setlocale os.time os.tmpname "
                           "package.cpath package.loaded package.loadlib "
                           "package.path package.preload package.seeall "
                           "string.byte string.char string.dump string.find "
                           "string.format string.gsub string.len string.lower "
                           "string.rep string.sub string.upper string.gmatch "
                           "string.match string.reverse "
                           "table.maxn table.concat table.sort "
                           "table.insert table.remove"));
        // lev functions
        SetKeyWords(3, wxT("lev.app.get lev.gui.frame lev.gui.vsizer"));
      }

      bool SetStyles()
      {
        // obj->StyleSetForeground (wxSTC_STYLE_DEFAULT,     wxColour (_T("DARK GREY")));
        // obj->StyleSetForeground (wxSTC_STYLE_INDENTGUIDE, wxColour (_T("DARK GREY")));

        // comment?
        StyleSetForeground(wxSTC_LUA_COMMENT, wxColour(0, 0, 255));
        // -- comment
        StyleSetForeground(wxSTC_LUA_COMMENTLINE, wxColour(0, 0, 255));
        // comment?
        StyleSetForeground(wxSTC_LUA_COMMENTDOC, wxColour(0, 0, 255));

        // number([0-9]+)
        StyleSetForeground(wxSTC_LUA_NUMBER, wxColour(255, 0, 0));

        // 'string'
        StyleSetForeground(wxSTC_LUA_CHARACTER, wxColour(255, 0, 255));
        // "string"
        StyleSetForeground(wxSTC_LUA_STRING, wxColour(255, 0, 255));
        // string?
        StyleSetForeground(wxSTC_LUA_STRINGEOL, wxColour(255, 0, 255));
        // [[string]]
        StyleSetForeground(wxSTC_LUA_LITERALSTRING, wxColour(255, 0, 255));

        // identifier
        SetLevKeyWords();
        // main keywords (do end, etc...)
        StyleSetForeground(wxSTC_LUA_WORD, wxColour(222, 80, 0));
        StyleSetBold(wxSTC_LUA_WORD, true);
        // std functions (require, etc...)
        StyleSetForeground(wxSTC_LUA_WORD2, wxColour(0, 120, 200));
        // module functions (os.date, etc...)
        StyleSetForeground(wxSTC_LUA_WORD3, wxColour(0, 120, 200));
        // lev functions (lev.app.get, etc...)
        StyleSetForeground(wxSTC_LUA_WORD4, wxColour(0, 120, 200));
//        StyleSetForeground(wxSTC_LUA_WORD5, wxColour(255, 80, 0));
//        StyleSetForeground(wxSTC_LUA_WORD6, wxColour(255, 80, 0));

        // current line
        SetCaretLineBackground(wxColour(230, 230, 230));
        SetCaretLineVisible(true);

        // line numbers
        SetMarginType(0, wxSTC_MARGIN_NUMBER);
        //StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(0, 0, 0));

        UpdateLineNumbers();
        return true;
      }

      bool UpdateLineNumbers()
      {
        int count = GetLineCount();
        wxString text = wxString::Format(wxT("%d "), count);
        int margin = TextWidth(wxSTC_STYLE_LINENUMBER, text);
        if (margin != line_number_margin)
        {
          line_number_margin = margin;
          SetMarginWidth(0, margin);
        }
        return true;
      }

      int line_number_margin;
  };
  static mySTC* cast_stc(void *obj) { return (mySTC *)obj; }


  bool code_edit::can_redo()
  {
    return cast_stc(_obj)->CanRedo();
  }

  bool code_edit::can_undo()
  {
    return cast_stc(_obj)->CanUndo();
  }

  code_edit* code_edit::create(control *parent, int width, int height)
  {
    code_edit *stc = NULL;
    mySTC *obj = NULL;
    wxWindow *p = NULL;
    try {
      if (parent) { p = (wxWindow *)parent->get_rawobj(); }
      stc = new code_edit;
      stc->_obj = obj = new mySTC;
      if (not obj->Create(p, -1, wxDefaultPosition, wxSize(width, height))) { throw -1; }
      stc->_id = obj->GetId();
      stc->connector = obj->GetConnector();
      stc->func_getter = obj->GetFuncGetter();

      wxStyledTextCtrl *orig = (wxStyledTextCtrl *)obj;
      orig->Connect(wxEVT_STC_CHARADDED, (wxObjectEventFunction)&mySTC::OnCharAdded);
      obj->SetStyles();
      return stc;
    }
    catch (...) {
      delete stc;
      return NULL;
    }
  }

  int code_edit::create_l(lua_State *L)
  {
    using namespace luabind;
    control *p = NULL;
    int w = -1, h = -1;

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

    if (t["parent"]) { p = object_cast<control *>(t["parent"]); }
    else if (t["p"]) { p = object_cast<control *>(t["p"]); }
    else if (t["udata"]) { p = object_cast<control *>(t["udata"]); }

    if (t["width"]) { w = object_cast<int>(t["width"]); }
    else if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["num1"]) { w = object_cast<int>(t["num1"]); }

    if (t["height"]) { h = object_cast<int>(t["height"]); }
    else if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["num2"]) { h = object_cast<int>(t["num2"]); }

    object func = globals(L)["lev"]["classes"]["code_edit"]["create_c"];
    object stc = func(p, w, h);
    if (stc)
    {
    }
    stc.push(L);
    return 1;
  }


  bool code_edit::clear()
  {
    cast_stc(_obj)->Clear();
    return true;
  }

  bool code_edit::copy()
  {
    cast_stc(_obj)->Copy();
    return true;
  }

  bool code_edit::cut()
  {
    cast_stc(_obj)->Cut();
    return true;
  }

  int code_edit::get_language()
  {
    return cast_stc(_obj)->GetLexer();
//    return lang.c_str();
  }

  std::string code_edit::get_text()
  {
    std::string text = (const char *)cast_stc(_obj)->GetTextRaw();
    return text;
  }

  bool code_edit::paste()
  {
    cast_stc(_obj)->Paste();
    return true;
  }

  bool code_edit::save_file(const char *file)
  {
    cast_stc(_obj)->SaveFile(wxString(file, wxConvUTF8));
    return true;
  }

  bool code_edit::select_all()
  {
    cast_stc(_obj)->SelectAll();
    return true;
  }

  bool code_edit::redo()
  {
    cast_stc(_obj)->Redo();
    return true;
  }

  bool code_edit::set_language(const char *language)
  {
    wxString lang(language, wxConvUTF8);
    this->lang = language;
    cast_stc(_obj)->SetLexerLanguage(lang);
    cast_stc(_obj)->SetHighlightGuide(1);
    return true;
  }

  bool code_edit::set_text(const char *text)
  {
    cast_stc(_obj)->SetTextRaw(text);
    return true;
  }

  bool code_edit::undo()
  {
    cast_stc(_obj)->Undo();
    return true;
  }


  // html viewer control definitions
  class myHtmlWindow : public wxHtmlWindow
  {
    public:
      inline myHtmlWindow(wxWindow *parent, int width, int height)
        : wxHtmlWindow(parent, -1, wxDefaultPosition, wxSize(width, height)),
          body()
      {}

      inline ~myHtmlWindow() {}
      std::string body;
  };
  inline static myHtmlWindow *cast_html(void *obj) { return (myHtmlWindow *)obj; }

  htmlview* htmlview::create(control *parent, int width, int height)
  {
    htmlview *html = NULL;
    myHtmlWindow *obj = NULL;
    wxWindow *p = NULL;
    try {
      html = new htmlview;
      if (parent)
      {
        p = (wxWindow *)parent->get_rawobj();
        html->system_managed = true;
      }
      obj = new myHtmlWindow(p, width, height);
      html->_id = obj->GetId();
      html->_obj = obj;
      if (p)
      {
        obj->SetRelatedFrame((wxFrame *)p, wxT("%s"));
        frame *frm = (frame *)parent;
        frm->set_status("");
        obj->SetRelatedStatusBar(0);
      }
      return html;
    }
    catch (...) {
      delete html;
      return NULL;
    }
  }

  int htmlview::create_l(lua_State *L)
  {
    using namespace luabind;
    object p;
    int w = -1, h = -1;

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

    if (t["parent"]) { p = t["parent"]; }
    else if (t["p"]) { p = t["p"]; }
    else if (t["udata"]) { p = t["udata"]; }

    if (t["width"]) { w = object_cast<int>(t["width"]); }
    else if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["num1"]) { w = object_cast<int>(t["num1"]); }

    if (t["height"]) { h = object_cast<int>(t["height"]); }
    else if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["num2"]) { h = object_cast<int>(t["num2"]); }

    object func = globals(L)["lev"]["classes"]["htmlview"]["create_c"];
    object html = func(p, w, h);
    if (html)
    {
      // do something
    }
    html.push(L);
    return 1;
  }

  const char *htmlview::get_page()
  {
    return cast_html(_obj)->body.c_str();
  }

  bool htmlview::loadpage(const char *str_url)
  {
    image::init();
    url *u = NULL;
    std::string body;
    try {
      u = url::connect(str_url);
      if (u == NULL) { throw -1; }
      body = u->get_data();
      cast_html(_obj)->body = body;
      delete u;
    }
    catch (...) {
      delete u;
      return false;
    }
    return cast_html(_obj)->SetPage(wxString(body.c_str(), wxConvUTF8));
  }

  bool htmlview::set_page(const char *src)
  {
    image::init();
    cast_html(_obj)->body = src;
    return cast_html(_obj)->SetPage(wxString(src, wxConvUTF8));
  }

  const char* htmlview::totext()
  {
    const std::string text =
      (const char *)((myHtmlWindow *)_obj)->ToText().mb_str(wxConvUTF8);
    return text.c_str();
  }




  typedef myHandler<wxTextCtrl> myTextCtrl;
  static myTextCtrl *cast_txt(void *obj) { return (myTextCtrl *)obj; }

  textbox* textbox::create(control *parent, int width, int height, const char *value, long style)
  {
    textbox *tb = NULL;
    myTextCtrl *obj = NULL;
    wxWindow *p = NULL;
    wxString val = wxEmptyString;
    try {
      tb = new textbox;
      if (parent)
      {
        p = (wxWindow *)parent->get_rawobj();
        tb->system_managed = true;
      }
      if (value) { val = wxString(value, wxConvUTF8); }
      obj = new myTextCtrl;
      if (not obj->Create(p, -1, val, wxDefaultPosition, wxSize(width, height), style))
      {
        throw -1;
      }
      tb->_obj = obj;
      tb->_id = obj->GetId();
      tb->connector = obj->GetConnector();
      tb->func_getter = obj->GetFuncGetter();
      return tb;
    }
    catch (...) {
      delete obj;
      delete tb;
      return NULL;
    }
  }

  int textbox::create_l(lua_State *L)
  {
    using namespace luabind;
    object p;
    int w = -1, h = -1;
    const char *v = "";
    const char *s = "";
    long style = 0;

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

    if (t["parent"]) { p = t["parent"]; }
    else if (t["p"]) { p = t["p"]; }
    else if (t["udata"]) { p = t["udata"]; }

    if (t["width"]) { w = object_cast<int>(t["width"]); }
    else if (t["w"]) { w = object_cast<int>(t["w"]); }
    else if (t["num1"]) { w = object_cast<int>(t["num1"]); }

    if (t["height"]) { h = object_cast<int>(t["height"]); }
    else if (t["h"]) { h = object_cast<int>(t["h"]); }
    else if (t["num2"]) { h = object_cast<int>(t["num2"]); }

    if (t["value"]) { v = object_cast<const char *>(t["value"]); }
    else if (t["val"]) { v = object_cast<const char *>(t["val"]); }
    else if (t["v"]) { v = object_cast<const char *>(t["v"]); }
    else if (t["str1"]) { v = object_cast<const char *>(t["str1"]); }

    if (t["style"]) { s = object_cast<const char *>(t["style"]); }
    else if (t["s"]) { s = object_cast<const char *>(t["s"]); }

    if (strstr(s, "multi")) { style = wxTE_MULTILINE; }

    object func = globals(L)["lev"]["classes"]["textbox"]["create_c"];
    object tb = func(p, w, h, v, style);
    if (tb)
    {
      // do something
    }
    tb.push(L);
    return 1;
  }

  const char* textbox::get_value()
  {
    const std::string value =
      (const char *)((wxTextCtrl *)_obj)->GetValue().mb_str(wxConvUTF8);
    return value.c_str();
  }

  bool textbox::set_multiline()
  {
    long style = get_style();
    cast_txt(_obj)->SetWindowStyle(style | wxTE_MULTILINE);
    return true;
  }

  void textbox::set_value(const char *value)
  {
    ((wxTextCtrl *)_obj)->SetValue(wxString(value, wxConvUTF8));
  }

}

