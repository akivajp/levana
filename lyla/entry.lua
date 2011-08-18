-- Package Requirements

require 'lev'
require 'lev.draw'
require 'lev.image'
require 'lev.sound'


-- Initialize

app = lev.app.get()
mixer = lev.sound.mixer()


-- Design

main = lev.gui.frame { title = 'Lyla Scripting System' }
canvas = lev.gui.canvas { p = main, w = 640, h = 480 }
main:fit()


-- Execute

img = lev.image.load('levana3.xpm')

require 'lev.gl'
gl = lev.gl

main:show()
canvas:set_current()
canvas:map2d(0, 640, 0, 480)
canvas:enable_alpha_blending()

while main.is_valid do
  canvas:set_current()
  canvas:clear(0, 0, 0)
  canvas:draw(img, { a = 200 })

gl.Begin(gl.QUADS)
  gl.Color3ub(255, 0, 0)
  gl.Vertex2d(20, 20)
  gl.Color3ub(0, 255, 0)
  gl.Vertex2d(20, 150)
  gl.Color3ub(255, 255, 0)
  gl.Vertex2d(150, 150)
  gl.Color3ub(0, 0, 255)
  gl.Vertex2d(200, 20)
gl.End()

  canvas:draw(img, 50, 50, 150)
  canvas:swap()
  app:wait()
end

