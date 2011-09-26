require 'lev.fs'
require 'lev.package'
require 'lev.string'

lev.require 'tags'

module('lyla', package.seeall)

-- state variables

buffer = lev.string.create()
content = ''
history = ''
key_pressed = false
new_line = false
status = ''
do_list = {}


-- functions

-- find tag starting with "left" and ending with "right". e.g: [ and ]
-- seek at next position from the correspondence termination
-- return the found string
function find_tag(left, right)
  local found = ''
  local ch = tostring(buffer:index(0))
  if not (ch == left) then
    return found
  end
  buffer = buffer:sub(1)

  local count = 1
  ch = tostring(buffer:index(0))
  buffer = buffer:sub(1)
  while #ch > 0 do
    if ch == left then
      count = count + 1
    elseif ch == right then
      count = count - 1
    end
    if count == 0 then
      return found
    end
    found = found .. ch
    ch = tostring(buffer:index(0))
    buffer = buffer:sub(1)
  end
  return ''
end



-- load file and init
function load(filename)
  local real_file
  local found = lev.package.resolve(filename)
              or lev.package.resolve(filename .. '.lyl')
              or lev.package.resolve(filename .. '.txt')
              or error(string.format('Neither %s.lyl nor %s.txt is not found', filename, filename))

  local infile = io.open(found.full, 'r')
  content = infile:read('*a')
  buffer = lev.string.mb_str(content)
  history = ''
  key_pressed = false
  new_line = true
  status = 'continue'
  do_list = {}
  return true
end

function load_list()
  while true do
    local ch = tostring(buffer:index(0))
--if ch then print('CH', ch) end
--print("BUFFERLEN:", #buffer)
    if #ch == 0 then return true end
    if ch == '[' then
      local tag = find_tag('[', ']')
      local code = tag:match('^%[(.*)%]$')
      if code then
        local f = loadstring(code)
        if f then f() end
      else
        local tag_name = tag:match('^([^%s]+)') or ''
        local tag_body = tag:match('^[^%s]+%s(.*)$')
        local params = {}
        if tag_body then
          local params_str = tag_body:gsub('%s+', ',')
          if params_str then params_str = params_str:gsub('%,+', ',') end
          if params_str then params_str = params_str:gsub('%,?%=%,?', '=') end
          if params_str then
            local f = loadstring(string.format('return {%s}', params_str))
            if f then
              params = f() or {}
            end
          end
        end
    
        if tags[tag_name] then
          local done = false
          for i, j in pairs(tags.replacers) do
            if j == tag_name then
              tags[tag_name](params)
              done = true
              break
            end
          end

          if not done then
            table.insert(do_list, function() tags[tag_name](params) end)
          end

          for i, j in pairs(tags.stoppers) do
            if j == tag_name then
              return true
            end
          end
        else
          print(string.format('tags[%s] is not found', tag_name))
        end
      end
    elseif ch == '*' and new_line then
      -- scene setting
      local line = seek_to_endl()
print('SCENE:', line)
    elseif ch == '\n' or ch == '\r' then
      -- nothing to do (no line feed)
      buffer = buffer:sub(1)
      history = history .. ch
      new_line = true
    elseif ch == ';' then
      -- comment line
      seek_to_endl()
    else
      layers.msgfg:reserve_word(ch)
      table.insert(do_list, function() layers.msgfg:draw_next() end)
      buffer = buffer:sub(1)
      history = history .. ch
    end
  end
  return true
end

function proc()
  if status == 'continue' then
    local f = table.remove(do_list, 1)
    if f then
      return f()
    else
      return load_list()
    end
  elseif status == 'wait' then
    return wait()
  elseif status == 'wait_key' then
    return wait_key()
  end
end

function seek_to_endl()
  local line = ''
  while true do
    ch = tostring(buffer:index(0))
    if #ch == 0 or ch == '\r' or ch == '\n' then
      break
    end
    line = line .. ch
    buffer = buffer:sub(1)
  end
  return line
end

function wait()
  if tags.wait_timer.time < tags.wait_until then
    status = 'wait'
  else
    status = 'continue'
  end
end

function wait_key()
  if key_pressed then
    layers.wait_line.on = false
    layers.wait_page.on = false
    key_pressed = false
    status = 'continue'
  end
end

