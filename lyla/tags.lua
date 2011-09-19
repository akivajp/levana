require 'string'
require 'lev.timer'

module('tags', package.seeall)

wait_timer = lev.stop_watch()
wait_until = 0

function br()
  lyla.msg = lyla.msg .. '\n'
end

function clear()
  lyla.msg = ''
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

function wait(param)
  local delay = param.delay or param[1] or 100
  delay = tonumber(delay)
  if delay then
    wait_timer:start(0)
    wait_until = delay
    lyla.status = 'wait'
  end
end

