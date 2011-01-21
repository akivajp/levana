-----------------------------------------------------------------------------
-- Name:        lev/util.lua
-- Purpose:     definition of usable functions
-- Author:      Akiva Miura <akiva.miura@gmail.com>
-- Created:     01/20/2011
-- Copyright:   (C) 2010-2011 Akiva Miura
-- Licence:     MIT License
-----------------------------------------------------------------------------

-- dependency
local getfenv      = getfenv
local getmetatable = getmetatable
local ipairs       = ipairs
local setfenv      = setfenv
local setmetatable = setmetatable
local table = table

module 'util'


-- find "value" from "t" and remove first one
local find_and_remove = function(t, value)
  for i,j in ipairs(t) do
    if (j == value) then
      table.remove(t, i)
      return
    end
  end
end


-- return new order-reversed table of "t"
local reverse = function(t, ...)
  local r = {}
  for i,val in ipairs(t) do
    table.insert(r, 1, val)
  end
  return r
end


-- looking up of "varnames"
local lookup = function(env, varname)
  meta = getmetatable(env)
  if meta.__owner[varname] ~= nil then
    return meta.__owner[varname]
  end
  for i,t in ipairs(meta.__lookup) do
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
  meta = getmetatable(env)
  meta.__owner[key] = value
end


-- like "using directive"
function using(...)
  -- getting environment of the caller and its metatable
  local env  = getfenv(2)
  local meta = getmetatable(env) or {}
  -- getting the caller itself
  local f = setfenv(2, env)
  if (meta.__caller == f) then
    -- setup was already done, changing looking up preference
    if #{...} == 0 then
      meta.__lookup = {}
      return env
    end
    for i,val in ipairs({...}) do
      find_and_remove(meta.__lookup, val)
      table.insert(meta.__lookup, 1, val)
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
  setmetatable(newenv, newmeta)
  setfenv(2, newenv)
  return newenv
end
