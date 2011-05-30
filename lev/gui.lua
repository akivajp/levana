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
--require 'lev/cfg'
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
class 'systray' (lev.gui.systray)

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
class 'text' (lev.gui.text)

function text:__init(conf)
  local c = conf
  c = c or {}
  c.p = c.parent or c.p
  c.w = c.width  or c.w or -1
  c.h = c.height or c.h or -1
  c.v = c.value  or c.v or ""
  lev.gui.text.__init(self, c.p, c.w, c.h, c.v)
end

-- end of 'text' class rewrapping
-------------------------------------------------------------------

