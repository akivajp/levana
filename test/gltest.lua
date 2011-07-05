require 'lev.std'
require 'lev.gl'

frm = lev.gui.frame {}
draw = lev.gui.canvas(frm, 640, 480)
frm:fit()
frm:show()

--draw:using()
gl = lev.gl
draw:setcurrent()
gl.NewList(1, gl.COMPILE)
gl.Begin(gl.QUADS)
  gl.Color3ub(255, 0, 0)
  gl.Vertex2d(-0.5, -0.5)
  gl.Color3ub(0, 255, 0)
  gl.Vertex2d(-0.5, 0.5)
  gl.Color3ub(255, 255, 0)
  gl.Vertex2d(0.5, 0.5)
  gl.Color3ub(0, 0, 255)
  gl.Vertex2d(0.5, -0.5)
gl.End()
gl.EndList()

while frm.isvalid do
  draw:setcurrent()
  gl.ClearColor(0, 0, 0, 0)
  gl.Clear(gl.COLOR_BUFFER_BIT)
  gl.LoadIdentity()
  gl.Scaled(1.2, 1.5, 0)
  gl.CallList(1)
  gl.Flush()
  draw:swap()
  frm.status = string.format("Used Memory: %d KB", collectgarbage("count"))
  app:yield()
end

