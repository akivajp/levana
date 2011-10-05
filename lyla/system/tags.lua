require 'lev.image'
require 'lev.package'
require 'lev.timer'
require 'lev.util'

module('tags', package.seeall)

wait_timer = lev.stop_watch()
wait_until = 0

replacers = {'a', 'ruby'}
stoppers = {'br', 'clear', 'r'}

function a(param)
  local text = param.text or param.txt or param[1]
  local href = param.href

  if text then
    local on_click
    if href then
      on_click = function()
        table.insert(jobs, function() lev.util.open(href) end)
      end
    end
    layers.msgfg:reserve_clickable('anchor', text, on_click)
    lyla.history = lyla.history .. text
    table.insert(lyla.do_list, function() layers.msgfg:draw_next() end)
  end
end

function br()
  layers.msgfg:reserve_new_line()
  layers.msgfg:draw_next()
  lyla.history = lyla.history .. '[br]'
end

function clear()
  lyla.history = lyla.history .. '\n'
  layers.msgfg:clear()
end

function eval(p)
  local expr = p.expression or p.expr or p.code or p[1] or ''
  if expr then
    local f = loadstring(string.format('return %s', tostring(expr)))
print('code: ', string.format('return %s', tostring(expr)))
print('f: ', f)
    local val
    if f then
      val = f()
    end
print('val: ', val)
    lyla.buffer = tostring(val) .. lyla.buffer
  end
end

function image(param)
  local src = param.src or param.storage
  local layer = param.layer
  local x = param.x
  local y = param.y

  if not src or not layer then
    return false
  end
  local src_path = lev.package.resolve(src)
  if not src_path then
    print(string.format('%s is not found', src))
    return false
  end

  if layers[layer] then
    layers[layer]:reload(src_path.full)
    if x then layers[layer].x = x end
    if y then layers[layer].y = y end
  end
end

function l()
  lyla.key_pressed = false
  lyla.status = 'wait_key'
  layers.wait_line.on = true
end

function p()
  lyla.key_pressed = false
  lyla.status = 'wait_key'
  layers.wait_page.on = true
end

function r()
  layers.msgfg:reserve_new_line()
  layers.msgfg:draw_next()
  lyla.history = lyla.history .. '[r]'
end

function ruby(param)
  local text = param.text or param[1]
  if text then
    local ch = tostring(lyla.buffer:index(0))
    lyla.buffer = lyla.buffer:sub(1)
    layers.msgfg:reserve_word(ch, text)
    lyla.history = lyla.history .. string.format('[ruby text="%s"]%s', text, ch)
    table.insert(lyla.do_list, function() layers.msgfg:draw_next() end)
  end
end

function wait(param)
  local delay = param.delay or param[1] or 100
  delay = tonumber(delay)
  if delay then
    wait_timer:start(0)
    wait_until = delay
    lyla.status = 'wait'
  else
  end
end

