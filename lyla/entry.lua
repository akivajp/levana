-- Package Requirements

require 'lev'
require 'lev.draw'
require 'lev.image'
require 'lev.prim'
require 'lev.sound'


-- Initialize

app = lev.app.get()
--mixer = lev.sound.mixer()
prim = lev.prim

-- Design

main = lev.gui.frame { title = 'Lyla Scripting System', style = 'fixed' }
canvas = lev.gui.canvas { p = main, w = 640, h = 480 }
main:fit()


-- Execute

img = lev.image.load('levana3.xpm')

require 'lev.gl'
gl = lev.gl

main:show()
canvas:map2d(0, 640, 0, 480)
canvas:enable_alpha_blending()

white = prim.color(255, 255, 255)

while main.is_valid do
  canvas:clear(0, 0, 0)
  canvas:draw(img, { a = 200 })

  canvas:draw(img, 50, 50, 150)
  canvas:draw_point(prim.point(300, 300, white))

  canvas:print('test')
  canvas:swap()
  app:wait()
end

