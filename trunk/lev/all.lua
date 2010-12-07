-----------------------------------------------------------------------------
-- Name:        lev.lua
-- Purpose:     levana library re-wrapping for flexible use
-- Author:      Akiva Miura <akiva.miura@gmail.com>
-- Created:     12/01/2010
-- Copyright:   (C) 2010 Akiva Miura
-- Licence:     MIT License
-----------------------------------------------------------------------------
-- Lev Engine

if lev then
  -- nothing to do
else
--  package.cpath = package.cpath .. ';./src/?.so'
  require 'levana'

  app   = {}
  lev   = {}

  function _(str)
    return str
  end

  -------------------------------------------------------------------
  -- begin 'app' table rewrapping
  app.autoloop = levana.app.autoloop
  app.yield    = levana.app.yield
  app.doevents = levana.app.yield

  app.top = function(top)
    if (top == nil) then return lev.frame(levana.app.get_top()) end
    levana.app.set_top(top.obj)
  end

  app.name = function(name)
    if (name == nil) then return levana.app.get_name() end
    levana.app.set_name(name)
  end

  app.msgbox = function(str, caption)
    if (caption == nil) then caption = '' end
    local result = levana.app.msgbox(str, caption)
    levana.app.yield(false)
    return result
  end
  -- end of 'app' table rewrapping
  -------------------------------------------------------------------


  -------------------------------------------------------------------
  -- begin 'frame' class rewrapping
  class 'frame'

  function frame:__init(conf)
    if (type(conf) == 'userdata') then
      self.obj = conf
      return
    end
    local c = conf
    if (c == nil) then c = {} end
    if (c.id == nil) then c.id = -1 end
    if (c.title == nil) then c.title = "Levana Application" end
    if (c.x == nil) then c.x = -1 end
    if (c.y == nil) then c.y = -1 end
    if (c.w == nil) then c.w = -1 end
    if (c.h == nil) then c.h = -1 end
    if (c.style == nil) then c.style = -1 end
    if (c.name == nil) then c.name = "frame" end
    self.obj =
      levana.frame(c.id, c.title,c.x, c.y, c.w, c.h, c.style, c.name)
  end

  function frame:close(force)
    if (force == nil) then show = false end
    return self.obj:close(force)
  end

  function frame:connect_menu(id, func)
    self.obj:connect_menu(id, func)
  end

  function frame:title(title)
    if (title == nil) then return self.obj:get_title() end
    self.obj:set_title(title)
  end

  function frame:set_menubar(mb)
    self.obj:set_menubar(mb.obj)
  end

  function frame:show(show)
    if (show == nil) then show = true end
    return self.obj:show(show)
  end

  function frame:__tostring()
    return self.obj:get_title()
  end

  lev.frame = frame
  frame = nil
  -- end of 'frame' class rewrapping
  -------------------------------------------------------------------
 

  -------------------------------------------------------------------
  -- begin 'menu' class rewrapping
  class 'menu'

  function menu:__init(title)
    if (title == nil) then title = '' end
    self.obj = levana.menu(title)
  end

  function menu:append(id, str, help_str)
    if (id == nil) then id = -1 end
    if (help_str == nil) then help_str = '' end
    return self.obj:append(id, str, help_str)
  end

  lev.menu = menu
  menu = nil
  -- end of 'menu' class rewrapping
  -------------------------------------------------------------------
 

  -------------------------------------------------------------------
  -- begin 'menubar' class rewrapping
  class 'menubar'
 
  function menubar:__init()
    self.obj = levana.menubar()
  end

  function menubar:append(menu, title)
    self.obj:append(menu.obj, title)
  end

  lev.menubar = menubar
  menubar = nil
  -- end of 'menubar' class rewrapping
  -------------------------------------------------------------------

  app.name("Levana Application")
  collectgarbage()
end

