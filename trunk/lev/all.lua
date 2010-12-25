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

  app = levana.appli()
  lev = {}

  function _(str)
    return str
  end

  -------------------------------------------------------------------
  -- begin 'frame' class rewrapping
  class 'frame' (levana.frame)

  function frame:__init(conf)
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
    levana.frame.__init(self, c.parent, c.id, c.title,c.x, c.y, c.w, c.h, c.style, c.name)
  end

--  function frame:close(force)
--    if (force == nil) then force = false end
--    return self.obj:close(force)
--  end
--
--  function frame:connect_menu(id, func)
--    self.obj:connect_menu(id, func)
--  end
--
--  function frame:title(title)
--    if (title == nil) then return self.obj:get_title() end
--    self.obj:set_title(title)
--  end
--
--  function frame:set_menubar(mb)
--    self.obj:set_menubar(mb.obj)
--  end
--
--  function frame:__tostring()
--    return self.obj:get_title()
--  end
--
  lev.frame = frame
  frame = nil
  -- end of 'frame' class rewrapping
  -------------------------------------------------------------------
 

  -------------------------------------------------------------------
  -- begin 'menu' class rewrapping
  class 'menu' (levana.menu)

  function menu:__init(title)
    levana.menu.__init(self, title)
    self.items = {}
  end

  function menu:append(conf)
    local c = conf
    if (c.id == nil)       then c.id = -1 end
    if (c.help_str == nil) then c.help_str = '' end
    local id = levana.menu.append(self, c.id, c.str, c.help_str)
    self.items[id] = conf.func
    return id
  end

  lev.menu = menu
  menu = nil
  -- end of 'menu' class rewrapping
  -------------------------------------------------------------------

  -------------------------------------------------------------------
  -- begin 'menubar' class rewrapping
  class 'menubar' (levana.menubar)

  function menubar:__init()
    levana.menubar.__init(self)
  end

 -- function menubar:append(menu, title)
 --   self.obj:append(menu.obj, title)
 -- end

  lev.menubar = menubar
  menubar = nil
  -- end of 'menubar' class rewrapping
  -------------------------------------------------------------------

  collectgarbage()
end

