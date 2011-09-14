-- Package Requirements

require 'lev.gui'
require 'lev.draw'
require 'lev.image'
require 'lev.package'
require 'lev.prim'
require 'lev.sound'

-- Configuration
conf = conf or {}
conf.w = 640
conf.h = 480
conf.caption = 'Lyla Scripting System'

layers = {}

-- Lyla Module Requirements

--lev.require 'lyla'


-- Initialize

app = lev.app()
mixer = lev.sound.mixer()
prim = lev.prim


-- Design

main = lev.gui.frame { caption = conf.caption, style = 'fixed' }
canvas = lev.gui.canvas { p = main, w = conf.w, h = conf.h }
main:fit()


-- Execute

tmp = lev.package.resolve('levana3.xpm')
if (not tmp) then return 0 end
img = lev.image.load(tmp.full)

main:show()
canvas:map2d(0, conf.w, 0, conf.h)
canvas:enable_alpha_blending()

white = prim.color(255, 255, 255)

--print(lyla.load('start'))

while main.is_valid do
  canvas:clear(0, 0, 0)
  canvas:draw(img, { a = 200 })

  canvas:draw(img, 50, 50, 150)

  lay = lev.image.create(600, 440)
  lay:clear(lev.prim.color(0, 255, 0, 50))
  canvas:draw(lay, 20, 20)
  msg = lev.image.stroke('test\n日本語テスト', lev.font.load('Meiryo 18'))
  canvas:draw(msg, 22, 22)
  msg = lev.image.stroke('test\n日本語テスト', lev.font.load('Meiryo 18'), lev.prim.color(255, 255, 255))
  canvas:draw(msg, 20, 20)

  canvas:swap()
  app:wait()
end

