-----------------------------------------------------------------------------
-- Name:        lev/gui.lua
-- Purpose:     gui control management
-- Author:      Akiva Miura <akiva.miura@gmail.com>
-- Created:     12/28/2010
-- Copyright:   (C) 2010 Akiva Miura
-- Licence:     MIT License
-----------------------------------------------------------------------------

-- dependency
require 'lev'
require 'lev/cfg'
require 'lev/util'

local _G = _G
local class = class
local lev = lev
local util = util

-- modulize
module 'gui'

-- class definer
local class = function(name)
  local deriver = class(name)
  _M[name] = _G[name]
  _G[name] = nil
  return deriver
end

-- porting
msgbox = lev.app.msgbox

-------------------------------------------------------------------
-- begin 'canvas' class rewrapping
class 'canvas' (lev.canvas)

function canvas:__init(...)
  local c = util.merge(...)
  c.p = c.p or c.parent
  c.w = c.w or c.width or -1
  c.h = c.h or c.height or -1
  lev.canvas.__init(self, c.p, c.w, c.h)
end

-- end 'canvas' class rewrapping
-------------------------------------------------------------------


-------------------------------------------------------------------
-- begin 'frame' class rewrapping
class 'frame' (lev.frame)


function frame.new(...)
  local c = util.merge({}, lev.cfg.defaults, ...)
  c.p = c.parent or c.p or nil
  c.t = c.title  or c.t or "Levana Application"
  c.h = c.height or c.h or -1
  c.w = c.width  or c.w or -1
  c.s = c.style  or c.s
  if (c.s == nil) then
    c.s = -1
  elseif (type(c.s) == 'table') then
    c.s = lev.cfg.frame_style(unpack(c.s))
  else
    c.s = lev.cfg.frame_style(c.s)
  end
  return lev.frame.new(c.p, c.t, c.w, c.h, c.s)
end


function frame:setmenubar(mb)
  lev.frame.setmenubar(self, mb)
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

-- end of 'frame' class rewrapping
-------------------------------------------------------------------


-------------------------------------------------------------------
-- begin 'menu' class rewrapping
class 'menu' (lev.menu)

function menu:__init(title)
  lev.menu.__init(self, title)
  self.items = {}
end

function menu:append(conf)
  local c = conf
  if (c.id == nil)       then c.id = -1 end
  if (c.help_str == nil) then c.help_str = '' end
  local id = lev.menu.append(self, c.id, c.str, c.help_str)
--    if (c.func) then self:setonmenu(id, c.func) end
  self.items[id] = c.func
  return id
end

-- end of 'menu' class rewrapping
-------------------------------------------------------------------

-------------------------------------------------------------------
-- begin 'menubar' class rewrapping
class 'menubar' (lev.menubar)

function menubar:__init()
  lev.menubar.__init(self)
  self.menus = {}
end

function menubar:append(menu, title)
  lev.menubar.append(self, menu, title)
  table.insert(self.menus, menu)
end

-- end of 'menubar' class rewrapping
-------------------------------------------------------------------


-------------------------------------------------------------------
-- begin 'sizer' classes rewrapping
class 'sizer' (lev.sizer)
hsizer = lev.hsizer
vsizer = lev.vsizer

function sizer:add(...)
  local c = ...
  c = c or {}
  if (_G.type(c) == 'userdata') then
    c = {}
    c.o, c.p, c.f, c.b = ...
  end

  c.o = c.o or c.object or c.control or c.ctrl or c.c or
        c.sizer or c.s or nil
  c.p = c.proportion or c.prop or c.p or 0
  c.f = c.flag or c.f or 0
  c.b = c.border or c.b or 0

  if c.o then
    lev.sizer.add(self, c.o, c.p, c.f, c.b)
  else
    c.w = c.width  or c.w or 10
    c.h = c.height or c.h or 10
    lev.sizer.addspace(self, c.w, c.h, c.p, c.f, c.b)
  end
end

hsizer.add = sizer.add
vsizer.add = sizer.add

-- end of 'sizer' classes rewrapping
-------------------------------------------------------------------

-------------------------------------------------------------------
-- begin 'systray' class rewrapping
class 'systray' (lev.systray)

function systray:__init()
  lev.systray.__init(self)
end

function systray:setmenu(m)
  lev.systray.setmenu(self, m)
  if m.items then
    for id, func in pairs(m.items) do
      if func then
        self:setonmenu(id, func)
      end
    end
  end
end

-- end of 'systray' class rewrapping
-------------------------------------------------------------------


-------------------------------------------------------------------
-- begin 'text' class rewrapping
class 'text' (lev.text)

function text:__init(conf)
  local c = conf
  c = c or {}
  c.p = c.parent or c.p
  c.w = c.width  or c.w or -1
  c.h = c.height or c.h or -1
  c.v = c.value  or c.v or ""
  lev.text.__init(self, c.p, c.w, c.h, c.v)
end

-- end of 'text' class rewrapping
-------------------------------------------------------------------

