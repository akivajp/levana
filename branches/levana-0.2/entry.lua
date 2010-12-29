require 'lev/std'
require 'lev/gl'

frm = lev.frame()
draw = lev.draw(frm, 640, 480)
frm:fit()
frm:show()

draw:using()
gl.newlist(1, gl.COMPILE)
gl.beginprim(gl.QUADS)
  gl.color3ub(255, 0, 0)
  gl.vertex2d(-0.5, -0.5)
  gl.color3ub(0, 255, 0)
  gl.vertex2d(-0.5, 0.5)
  gl.color3ub(255, 255, 0)
  gl.vertex2d(0.5, 0.5)
  gl.color3ub(0, 0, 255)
  gl.vertex2d(0.5, -0.5)
gl.endprim()
gl.endlist()

while frm:exists() do
  draw:using()
  gl.clearcolor(0, 0, 0, 0)
  gl.clear(gl.COLOR_BUFFER_BIT)
  gl.loadidentity()
  gl.scaled(1.2, 1.5, 0)
  gl.calllist(1)
  gl.flush()
  draw:swap()
  frm.status = string.format("Used Memory: %d KB", collectgarbage("count"))
  app:yield()
end

