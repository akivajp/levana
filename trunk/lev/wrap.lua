-----------------------------------------------------------------------------
-- Name:        wrap.lua
-- Purpose:     levana library re-wrapping for flexible use
-- Author:      Akiva Miura <akiva.miura@gmail.com>
-- Created:     12/28/2010
-- Copyright:   (C) 2010 Akiva Miura
-- Licence:     MIT License
-----------------------------------------------------------------------------

if lev then
  -- nothing to do
else
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

  function frame:setmenubar(mb)
    levana.frame.setmenubar(self, mb)
    if (mb.menus) then
      for i, menu in pairs(mb.menus) do
        for id, func in pairs(menu.items) do
          if func then
            self:setonmenu(id, func)
          end
        end
      end
    end
  end

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
--    if (c.func) then self:setonmenu(id, c.func) end
    self.items[id] = c.func
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
    self.menus = {}
  end

  function menubar:append(menu, title)
    levana.menubar.append(self, menu, title)
    table.insert(self.menus, menu)
  end


  lev.menubar = menubar
  menubar = nil
  -- end of 'menubar' class rewrapping
  -------------------------------------------------------------------


  -------------------------------------------------------------------
  -- begin 'systray' class rewrapping
  class 'systray' (levana.systray)

  function systray:__init()
    levana.systray.__init(self)
  end

  function systray:setmenu(m)
    levana.systray.setmenu(self, m)
    if m.items then
      for id, func in pairs(m.items) do
        if func then
          self:setonmenu(id, func)
        end
      end
    end
  end

  lev.systray = systray
  systray = nil
  -- end of 'systray' class rewrapping
  -------------------------------------------------------------------
  collectgarbage()
end

