-----------------------------------------------------------------------------
-- Name:        lev/util.lua
-- Purpose:     definition of usable functions
-- Author:      Akiva Miura <akiva.miura@gmail.com>
-- Created:     01/20/2011
-- Copyright:   (C) 2010-2011 Akiva Miura
-- Licence:     MIT License
-----------------------------------------------------------------------------

-- usage:
--   * using() : clearing the lookup setting
--   * using(...) : adding tables to the lookup setting where ... is a list of tables
--   * using(nil, ...) : clearing and resetting the lookup tables

-- dependency
local _G = _G

-- modulizing
module 'util'


-- find "value" from "t" and remove first one
local find_and_remove = function(t, value)
  for i,j in _G.ipairs(t) do
    if (j == value) then
      _G.table.remove(t, i)
      return
    end
  end
end


-- return new order-reversed table of "t"
local reverse = function(t, ...)
  local r = {}
  for i,val in _G.ipairs(t) do
    _G.table.insert(r, 1, val)
  end
  return r
end


-- looking up of "varnames"
local lookup = function(env, varname)
  meta = _G.getmetatable(env)
  if meta.__owner[varname] ~= nil then
    return meta.__owner[varname]
  end
  for i,t in _G.ipairs(meta.__lookup) do
    if t[varname] ~= nil then
      return t[varname]
    end
  end
  if (meta.__parent == meta.__owner) then
    -- non sense to look up twice for the same table
    return nil
  end
  return meta.__parent[varname]
end


-- subtituting the new value
local substitute = function(env, key, value)
  meta = _G.getmetatable(env)
  meta.__owner[key] = value
end


-- like "using directive"
function using(...)
  -- getting environment of the caller and its metatable
  local env  = _G.getfenv(2)
  local meta = _G.getmetatable(env) or {}
  -- getting the caller itself
  local f = _G.setfenv(2, env)
  if (meta.__caller == f) then
    -- setup was already done, changing looking up preference
    if (...) == nil then
      -- 1st arg is nil, resetting looking up setting
      if #{...} >= 2 then
        meta.__lookup = reverse({_G.select(2, ...)})
      else
        meta.__lookup = {}
      end
      return env
    end
    for i,val in _G.ipairs({...}) do
      find_and_remove(meta.__lookup, val)
      _G.table.insert(meta.__lookup, 1, val)
    end
    return env
  end

  -- setting new looking up mechanism
  local newenv  = {}
  local newmeta = {}
  newmeta.__caller = f
  newmeta.__index = lookup
  newmeta.__lookup = reverse({...})
  newmeta.__newindex = substitute
  newmeta.__owner = meta.__owner or env
  newmeta.__parent = env
  _G.setmetatable(newenv, newmeta)
  _G.setfenv(2, newenv)
  return newenv
end


-- enabling to call using without module prefix
_G.using = using

