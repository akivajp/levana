-- Package Requirements

require 'lev.gui'
require 'lev.draw'
require 'lev.image'
require 'lev.package'
require 'lev.prim'
require 'lev.sound'
require 'lev.timer'

-- Path Settings
lev.package.clear_search()
lev.package.add_search('')
lev.package.add_search('image')
lev.package.add_search('scenario')

-- Configuration
conf = conf or {}
conf.w = 640
conf.h = 480
conf.first_load = 'start'
conf.caption = 'Lyla Scripting System'


-- Layers

layers = {}
layers[1] = lev.image.create(conf.w, conf.h)
layers.bg = layers[1]
layers[2] = lev.image.create(conf.w, conf.h)
layers.msgbg = layers[2]
layers[3] = lev.image.create(conf.w, conf.h)
layers.msgfg = layers[3]

layers.msgbg:fill_rect(50, 50, 540, 380, lev.prim.color(0, 0, 255, 128))

-- Lyla Module Requirements

lev.require 'lyla'

-- Initialize

app = lev.app()
mixer = lev.sound.mixer()
prim = lev.prim

-- Design

main = lev.gui.frame { caption = conf.caption, style = 'fixed' }
canvas = lev.gui.canvas { p = main, w = conf.w, h = conf.h }
main:fit()

-- Controls
--
if (not lyla.load(conf.first_load)) then
  lev.gui.msgbox(string.format(_'%s is not found!', conf.first_load))
  return -1
end

timer = lev.timer()
timer.notify = function()
  lyla.proc()
end
timer:start(100)

-- Execute

main:show()
canvas:map2d(0, conf.w, 0, conf.h)
canvas:enable_alpha_blending()

white = prim.color(255, 255, 255)

sw = lev.stop_watch()
while main.is_valid do
sw:start()
print('time1:', sw.time)
  canvas:clear(0, 0, 0)
print('time2:', sw.time)

  layers.msgfg:clear()
print('time3:', sw.time)
  layers.msgfg:draw_text(lyla.msg, lev.font.load('24'), lev.prim.color(0, 0, 0), 52, 52) 
  layers.msgfg:draw_text(lyla.msg, lev.font.load('24'), lev.prim.color(255, 255, 255), 50, 50)

  for i, j in ipairs(layers) do
print('time_: ', sw.time)
    canvas:compile(j, false)
    canvas:draw(j)
  end

  canvas:swap()
  app:wait()
print('time:', sw.time)
print()
end

