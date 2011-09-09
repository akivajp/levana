require 'string'
require 'lev.gui'

app = lev.app()
app:track_mouse()
app.fps = 2

frm = lev.gui.frame()
frm:show()

while frm.is_valid do
--  app:yield()
  if app:get_keydown('ESC') then
    break
  elseif app.inrec.left_down then
    print('Left Click!')
  else
    print(string.format('(X, Y) = (%d, %d)',app.input.x,app.input.y))
  end
  app:wait()
end

